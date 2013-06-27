/* linux/drivers/mmc/host/sep0611_mmc.c
 *
 * Copyright (c) 2009-2011 SEUIC
 *  fjj <fanjianjun@wxseuic.com>
 *
 * Southeast University ASIC SoC support
 *
 * SD Driver for SEP0611 Board
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *	xx-xx-xxxx 	zwm		initial version
 *  08-02-2011 	fjj    	second version
 *  08-19-2011  fjj		third version
 */ 

#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/timer.h>
#include <linux/dma-mapping.h>
#include <linux/dw_dmac.h>

#include <asm/sizes.h>
#include <asm/scatterlist.h>
#include <asm/dma.h>
#include <mach/dma.h>
#include <mach/hardware.h>
#include <board/board.h>

#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/sd.h>

#include <mach/regs-sdio.h>

//#define MMC_DEBUG
#ifdef  MMC_DEBUG
#define mmc_debug(fmt...)	printk(fmt)
#else
#define mmc_debug(fmt...)
#endif

struct sepmmc_host {
	struct mmc_host	*mmc;

	struct mmc_request *mrq;
	struct mmc_command *cmd;
	struct mmc_data	*data;

	enum dma_data_direction dir;
	int seg_num;
	int cur_seg;

	spinlock_t lock;

	unsigned int hclk;

	struct completion cmd_complete_request;	
	struct completion data_complete_request;

	int suspend;

#ifdef CONFIG_MMC_SEP_DETECT_MODE_POLL_STATUS 
	struct timer_list timer;
#endif
};

static inline void clear_any_pending_int(struct sepmmc_host *host)
{
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	writel(0x1ffff, SDIO1_RINTSTS_V);
	spin_unlock_irqrestore(&host->lock, flags); 
}

static inline void disable_any_int(struct sepmmc_host *host)
{
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	writel(0, SDIO1_INTMASK_V);
	spin_unlock_irqrestore(&host->lock, flags); 
}

static inline void enable_command_done_int(struct sepmmc_host *host)
{
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	writel(SDIO_INT_CD(1), SDIO1_INTMASK_V);
	spin_unlock_irqrestore(&host->lock, flags); 
}

static inline void enable_data_transfer_over_int(struct sepmmc_host *host)
{
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	writel(SDIO_INT_DTO(1), SDIO1_INTMASK_V);
	spin_unlock_irqrestore(&host->lock, flags); 
}

#ifndef CONFIG_MMC_SEP_DETECT_MODE_POLL_STATUS 
static inline void enable_data3_card_detect_int(struct sepmmc_host *host)
{
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	writel(SDIO_INT_CDT(1), SDIO1_INTMASK_V);
	spin_unlock_irqrestore(&host->lock, flags); 
}
#endif

static inline void dma_write(dma_addr_t bus_addr, int blk_size)
{
	mmc_debug("[%s], bus_addr: 0x%x, blk_size: 0x%x\n", __func__, bus_addr, blk_size);	

	/* program SARx */
	writel(bus_addr, DMAC1_SAR_CH(DMA_SDIO1_CH));

	/* program DARx */
	writel(SDIO1_BASE + 0x100, DMAC1_DAR_CH(DMA_SDIO1_CH));

	/* program CTLx */	
	writel(DMA_CH_FOR_SDIO1_CTL_L_WR, DMAC1_CTL_CH(DMA_SDIO1_CH));
	writel(DMAC_DONE(0) | DMAC_BLK_TRAN_SZ(blk_size >> 2), DMAC1_CTL_CH(DMA_SDIO1_CH) + 4);
	
	/* program CFGx */	
	writel(DMA_CH_FOR_SDIO1_CFG_L_WR, DMAC1_CFG_CH(DMA_SDIO1_CH));
	writel(DMA_CH_FOR_SDIO1_CFG_H_WR, DMAC1_CFG_CH(DMA_SDIO1_CH) + 4);

	/* enable dma channel */
	writel(DMAC_CH_WRITE_EN(1 << DMA_SDIO1_CH) | DMAC_CH_EN(1 << DMA_SDIO1_CH), DMAC1_CHEN_V);
}

static inline void dma_read(dma_addr_t bus_addr, int blk_size)
{
	mmc_debug("[%s], bus_addr: 0x%x, blk_size: 0x%x\n", __func__, bus_addr, blk_size);	

	/* program SARx */
	writel(SDIO1_BASE + 0x100, DMAC1_SAR_CH(DMA_SDIO1_CH));

	/* program DARx */
	writel(bus_addr, DMAC1_DAR_CH(DMA_SDIO1_CH));

	/* program CTLx */	
	writel(DMA_CH_FOR_SDIO1_CTL_L_RD, DMAC1_CTL_CH(DMA_SDIO1_CH));
	writel(DMAC_DONE(0) | DMAC_BLK_TRAN_SZ(blk_size >> 2), DMAC1_CTL_CH(DMA_SDIO1_CH) + 4);

	/* program CFGx */
	writel(DMA_CH_FOR_SDIO1_CFG_L_RD, DMAC1_CFG_CH(DMA_SDIO1_CH));
	writel(DMA_CH_FOR_SDIO1_CFG_H_RD, DMAC1_CFG_CH(DMA_SDIO1_CH) + 4);

	/* enable dma channel */
	writel(DMAC_CH_WRITE_EN(1 << DMA_SDIO1_CH) | DMAC_CH_EN(1 << DMA_SDIO1_CH), DMAC1_CHEN_V);
}

static inline void enable_blk_transfer_complete_int(struct sepmmc_host *host)
{
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	writel(DMAC_INT_WE(1 << (DMA_SDIO1_CH + 8)) | DMAC_INT_MASK(1 << DMA_SDIO1_CH), DMAC1_MASKBLK_V);	
	spin_unlock_irqrestore(&host->lock, flags);
}

static inline void disable_blk_transfer_complete_int(struct sepmmc_host *host)
{
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	writel(DMAC_INT_WE(1 << (DMA_SDIO1_CH + 8)) | DMAC_INT_MASK(0 << DMA_SDIO1_CH), DMAC1_MASKBLK_V);
	spin_unlock_irqrestore(&host->lock, flags);	
}

static inline void clear_blk_transfer_complete_int(struct sepmmc_host *host)
{
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	writel(1 << DMA_SDIO1_CH, DMAC1_CLRBLK_V);
	spin_unlock_irqrestore(&host->lock, flags);
}

irqreturn_t dma_chan_for_sdio1_irq_handler(int irq, void *devid)
{
	if (readl(DMAC1_STATBLK_V) & (1 << DMA_SDIO1_CH)) {
		struct sepmmc_host *host = devid;
		dma_addr_t bus_addr;
		int seg_len;

		clear_blk_transfer_complete_int(host);

		host->cur_seg++;

		if (host->cur_seg < host->seg_num) {
			bus_addr = sg_dma_address(&host->data->sg[host->cur_seg]);
			seg_len = sg_dma_len(&host->data->sg[host->cur_seg]);

			mmc_debug("[%s], cur_seg=%d, bus_addr=0x%08x, seg_len=0x%x\n", __func__, host->cur_seg, bus_addr, seg_len);

			if (host->dir == DMA_TO_DEVICE) /* write data to sd */
				dma_write(bus_addr, seg_len);
			else /* read data from sd */
				dma_read(bus_addr, seg_len);
		} else 
			mmc_debug("[%s], up to the last segment\n", __func__);
	}

	return IRQ_HANDLED;
}

static inline void sepmmc_dma_transfer(struct sepmmc_host *host)
{	
	dma_addr_t bus_addr;
	int seg_len;

	host->seg_num = dma_map_sg(mmc_dev(host->mmc), host->data->sg, host->data->sg_len, host->dir);

	mmc_debug("[%s], seg_num = %d\n", __func__, host->seg_num);	

	host->cur_seg = 0;

	bus_addr = sg_dma_address(&host->data->sg[host->cur_seg]);
	seg_len = sg_dma_len(&host->data->sg[host->cur_seg]);

	mmc_debug("[%s], cur_seg=%d, bus_addr=0x%08x, seg_len=0x%x\n", __func__, host->cur_seg, bus_addr, seg_len);

	clear_blk_transfer_complete_int(host);

	if (host->seg_num > 1)
		enable_blk_transfer_complete_int(host);
	else
		disable_blk_transfer_complete_int(host);

	if (host->dir == DMA_TO_DEVICE) /* write data to sd */
		dma_write(bus_addr, seg_len);
	else /* read data from sd */
		dma_read(bus_addr, seg_len);
}

static inline void sepmmc_prepare_data(struct sepmmc_host *host, struct mmc_data *data)
{	
	host->data = data;

	/* block size register */
	writel(data->blksz, SDIO1_BLKSIZ_V);
	/* bytes count register */
	writel(data->blksz * data->blocks, SDIO1_BYTCNT_V);

	mmc_debug("blksz=%d, blocks=%d\n", data->blksz, data->blocks);

	if (host->data->flags & MMC_DATA_WRITE)
		host->dir = DMA_TO_DEVICE;
	else
		host->dir = DMA_FROM_DEVICE;
	
	sepmmc_dma_transfer(host);	
}

static void sepmmc_start_cmd(struct sepmmc_host *host, struct mmc_command *cmd)
{
	unsigned int cmd_reg = 0;

	mmc_debug("[%s], cmd:%d\n", __func__, cmd->opcode);

	host->cmd = cmd;
			
	if (cmd->data)
		sepmmc_prepare_data(host, cmd->data);

	/* command index */
	cmd_reg |= cmd->opcode;

	/* expect response */
	if (cmd->flags & MMC_RSP_PRESENT)
		cmd_reg |= SDIO_CMD_RESP_EXPE(1);
	
	/* response length */
	if (cmd->flags & MMC_RSP_136)
		cmd_reg |= SDIO_CMD_LONG_RESP(1);	

	/* check response crc */
	if (cmd->flags & MMC_RSP_CRC)
		cmd_reg |= SDIO_CMD_CHK_RESP_CRC(1);

	if (cmd->data) {
		/* expect data */
		cmd_reg |= SDIO_CMD_HAVE_DAT_TRAN(1);

		/* write to card */
		if (cmd->data->flags & MMC_DATA_WRITE)
			cmd_reg |= SDIO_CMD_WRITE(1);
	}

	/* wait previous data transfer completion */
	if ((cmd->opcode != MMC_STOP_TRANSMISSION) && (cmd->opcode != MMC_SEND_STATUS))
		cmd_reg |= SDIO_CMD_WAIT_DAT(1);

	/* send initialization */
	if (cmd->opcode == MMC_GO_IDLE_STATE)
		cmd_reg |= SDIO_CMD_SEND_INIT(1);

	/* start command */
	cmd_reg |= SDIO_CMD_START;

	/* write command argument */
	writel(cmd->arg, SDIO1_CMDARG_V);
	/* write command, at this moment, this command starts */
	writel(cmd_reg, SDIO1_CMD_V);
}

static inline void reset_hardware(struct sepmmc_host *host)
{
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	/* disable dma channel */
	writel(DMAC_CH_WRITE_EN(1 << DMA_SDIO1_CH) | DMAC_CH_EN(0 << DMA_SDIO1_CH), DMAC1_CHEN_V);
	while (readl(DMAC1_CHEN_V) & DMAC_CH_EN(1 << DMA_SDIO1_CH));
	/* reset fifo, dma */
	writel(readl(SDIO1_CTRL_V) | SDIO_DMA_RST(1) | SDIO_FIFO_RST(1), SDIO1_CTRL_V); 
	while (readl(SDIO1_CTRL_V) & SDIO_DMA_RST(1) & SDIO_FIFO_RST(1));
	spin_unlock_irqrestore(&host->lock, flags); 
}

static void sepmmc_request(struct mmc_host *mmc, struct mmc_request *mrq)
{
	struct sepmmc_host *host = mmc_priv(mmc);		
	
	host->mrq = mrq;

	mmc_debug("[%s], into\n", __func__);

	reset_hardware(host);

	disable_any_int(host);
	clear_any_pending_int(host);

	sepmmc_start_cmd(host, mrq->cmd);

	init_completion(&host->cmd_complete_request);
	enable_command_done_int(host);
	wait_for_completion(&host->cmd_complete_request);

	if (!mrq->cmd->error && mrq->data) {
		init_completion(&host->data_complete_request);		
		enable_data_transfer_over_int(host);
		wait_for_completion(&host->data_complete_request);

		if (!mrq->data->error && mrq->stop) {
			init_completion(&host->cmd_complete_request);
			enable_command_done_int(host);
			wait_for_completion(&host->cmd_complete_request);
		}
	}

	host->mrq = NULL;
	host->cmd = NULL;
	host->data = NULL;

	mmc_request_done(mmc, mrq);

#ifndef CONFIG_MMC_SEP_DETECT_MODE_POLL_STATUS
	if (!host->suspend) 
		enable_data3_card_detect_int(host);
#endif

	mmc_debug("[%s], exit\n", __func__);
}

static inline void calculate_clock_divider(struct sepmmc_host *host, struct mmc_ios *ios, unsigned char *f_div, unsigned char *s_div)
{
#ifdef MMC_DEBUG
	unsigned char ahb_div;
	unsigned char clk_div0;
	unsigned int cclk_out;
#endif	

	if (ios->clock == host->mmc->f_min) { /* 400KHz */
		*f_div = 8;
		*s_div = (host->hclk / 16 + ios->clock - 1) / ios->clock;
	} else {	/* ios->clock == host->mmc->f_max, 50MHz */
		*f_div = (host->hclk / 2 + ios->clock - 1) / ios->clock * 2;
		*s_div = 0;
	}

#ifdef MMC_DEBUG
	ahb_div = *f_div;

	if (*s_div == 0)
		clk_div0 = 1;
	else
		clk_div0 = *s_div * 2;	

	cclk_out = host->hclk / ahb_div / clk_div0;
	
	printk("[%s], hclk=%u, ahb_divider=%u, clk_divider0=%u, cclk_out=%d\n", 
		__func__, host->hclk, *f_div, *s_div, cclk_out);	
#endif
}

static inline void update_clock_register(void)
{
	writel( SDIO_CMD_START            | SDIO_CMD_UPDATE_CLK_REG_ONLY(1) | SDIO_CMD_CARD_NUM (0) \
          | SDIO_CMD_SEND_INIT    (0) | SDIO_CMD_STP_ABT            (0) | SDIO_CMD_WAIT_DAT (1) \
          | SDIO_CMD_SET_STOP     (0) | SDIO_CMD_STR_TRAN           (0) | SDIO_CMD_WRITE    (0) \
          | SDIO_CMD_HAVE_DAT_TRAN(0) | SDIO_CMD_CHK_RESP_CRC       (0) | SDIO_CMD_LONG_RESP(0) \
          | SDIO_CMD_RESP_EXPE    (0) | CMD0, SDIO1_CMD_V);
     
    /* wait for the CIU to take the command by polling for 0 on the start_cmd bit. */
	while ((readl(SDIO1_CMD_V) & 0x80000000) == 0x80000000);
}

static void sepmmc_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
	struct sepmmc_host *host = mmc_priv(mmc);
	/* f_div >> ahbclkdiv, s_div >> clk_divider0 */
	unsigned char f_div, s_div;

	mmc_debug("[%s], into\n", __func__);

	if (ios->clock) { 
		calculate_clock_divider(host, ios, &f_div, &s_div);
	
		/* stop clock */
		writel(SDIO_CCLK_LW_PWR(0) | SDIO_CCLK_EN(0), SDIO1_CLKENA_V);
		update_clock_register();

		writel(f_div, SDIO1_AHBCLKDIV_V);
		writel(s_div, SDIO1_CLKDIV_V);
		update_clock_register();

		/* re-enable clock */
		writel(SDIO_CCLK_LW_PWR(0) | SDIO_CCLK_EN(1), SDIO1_CLKENA_V);
		update_clock_register();
	}

	switch (ios->bus_width) {
		case MMC_BUS_WIDTH_1: {
			writel(0x0, SDIO1_CTYPE_V);
			mmc_debug("[%s], 1 bit mode\n", __func__);	
			break;
		}

		case MMC_BUS_WIDTH_4: {
			writel(0x1, SDIO1_CTYPE_V);
			mmc_debug("[%s], 4 bit mode\n", __func__);
			break;
		}

		default : {	/* MMC_BUS_WIDTH_8 */
			printk("sep sd controller doesn't support this bus width\n");
			break;
		}
	} 

	mmc_debug("[%s], exit\n", __func__);
}

static struct mmc_host_ops sepmmc_ops = {
	.request = sepmmc_request,
	.set_ios = sepmmc_set_ios,
};

static inline void sepmmc_command_done(struct sepmmc_host *host, unsigned int rintsts_reg)
{
	struct mmc_command *cmd = host->cmd;

	mmc_debug("[%s]\n", __func__);

	BUG_ON(rintsts_reg == 0);

	if (!cmd) {
		printk("[%s]: Got command done interrupt 0x%08x even "
				"though no command operation was in progress.\n",
				mmc_hostname(host->mmc), rintsts_reg);
		return;
	}

	if (cmd->flags & MMC_RSP_PRESENT) {
		if (cmd->flags & MMC_RSP_136) {
			cmd->resp[0] = readl(SDIO1_RESP3_V);	
			cmd->resp[1] = readl(SDIO1_RESP2_V);
			cmd->resp[2] = readl(SDIO1_RESP1_V);
			cmd->resp[3] = readl(SDIO1_RESP0_V);

			//mmc_debug("[%s], cmd:%d, arg:0x%08x, resp[0]=0x%08x, resp[1]=0x%08x, resp[2]=0x%08x, resp[3]=0x%08x\n",
			//		__func__, cmd->opcode, cmd->arg, cmd->resp[0], cmd->resp[1], cmd->resp[2], cmd->resp[3]);;
		} else {
			cmd->resp[0] = readl(SDIO1_RESP0_V);

			//mmc_debug("[%s], cmd:%d, arg:0x%08x, resp[0]=0x%08x\n",
			//		 __func__, cmd->opcode, cmd->arg, cmd->resp[0]);
		}
	}

	cmd->error = 0;

	if (rintsts_reg & SDIO_INT_RTO(1)) {                
     	cmd->error = -ETIMEDOUT; /* response timeout */
	} else if (rintsts_reg & (SDIO_INT_RE(1) | SDIO_INT_RCRC(1))) {
		cmd->error = -EILSEQ; /* response error or response crc error */
	}

	complete(&host->cmd_complete_request);		
}

static inline void sepmmc_data_transfer_over(struct sepmmc_host *host, unsigned int rintsts_reg)
{
	struct mmc_data *data = host->data;

	BUG_ON(rintsts_reg == 0);

	mmc_debug("[%s]\n", __func__);

	if (!data) {
		printk("[%s]: Got data transfer over interrupt 0x%08x even "
				"though no data operation was in progress.\n",
				mmc_hostname(host->mmc), rintsts_reg);
		return;
	}

	data->error = 0;

	if (rintsts_reg & SDIO_INT_DRTO(1)) {           
		data->error = -ETIMEDOUT;	/* data read timeout */	
	} else if (rintsts_reg & (SDIO_INT_DCRC(1) | SDIO_INT_SBE(1) | SDIO_INT_EBE(1))) {
		data->error = -EILSEQ;	/* data crc error or start bit error or ebe */
	}

	if (!data->error) {
		data->bytes_xfered = data->blocks * data->blksz;
	} else {	
		data->bytes_xfered = 0;
	}	

	complete(&host->data_complete_request);	

	if (data->stop)
		sepmmc_start_cmd(host, data->stop);
}

#ifndef CONFIG_MMC_SEP_DETECT_MODE_POLL_STATUS
static inline int sepmmc_card_presence(void)
{
	return (readl(SDIO1_CDETECT_V) & 0x1)? 0 : 1;	
}
#endif

irqreturn_t sdio1_irq_handler(int irq, void *devid)
{
	struct sepmmc_host *host = devid;
	unsigned int mintsts_reg;
	unsigned int rintsts_reg;

	/* read masked interrupt status register */
	mintsts_reg = readl(SDIO1_MINTSTS_V);
	/* read raw interrupt status register */
	rintsts_reg = readl(SDIO1_RINTSTS_V);

#ifndef CONFIG_MMC_SEP_DETECT_MODE_POLL_STATUS
	/* card detect */
	if (mintsts_reg & SDIO_INT_CDT(1)) {
		writel(SDIO_INT_CDT(1), SDIO1_RINTSTS_V);

		if (sepmmc_card_presence())	 {	/* insertion */
			printk("insert a card\n");
			mmc_detect_change(host->mmc, msecs_to_jiffies(500));
		} else {	/* removal */
			printk("remove a card\n");
			mmc_detect_change(host->mmc, msecs_to_jiffies(0));
		}
	}
#endif

	/* command done */
	if (mintsts_reg & SDIO_INT_CD(1)) {
		writel(SDIO_INT_RTO(1) | SDIO_INT_RCRC(1) | SDIO_INT_CD(1) | SDIO_INT_RE(1), SDIO1_RINTSTS_V);	
		sepmmc_command_done(host, rintsts_reg);
	}

	/* data transfer over */
	if (mintsts_reg & SDIO_INT_DTO(1)) {
		writel(SDIO_INT_EBE(1) | SDIO_INT_SBE(1) | SDIO_INT_DRTO(1) | SDIO_INT_DCRC(1) | SDIO_INT_DTO(1), SDIO1_RINTSTS_V);
		sepmmc_data_transfer_over(host, rintsts_reg);	
	}	

	return IRQ_HANDLED;
}

static void sdio1_conrtoller_init(struct sepmmc_host *host)
{
	clear_any_pending_int(host);

	writel(0, SDIO1_INTMASK_V);

  	/* enable data3_card_detect, enable dma transfer, enable global inttrupt, reset controller, fifo, dma */ 
	writel( SDIO_DAT3_DETECT_EN(1) | SDIO_BIG_ENDIAN_EN(0) | SDIO_ABT_RD_DAT(0) | SDIO_SEND_IRQ_RESP(0) \
		  | SDIO_READ_WAIT     (0) | SDIO_DMA_EN       (1) | SDIO_INT_EN    (1) | SDIO_DMA_RST      (1) \
		  | SDIO_FIFO_RST      (1) | SDIO_CTRL_RST     (1),  SDIO1_CTRL_V); 
	while (readl(SDIO1_CTRL_V) & (SDIO_DMA_RST(1) | SDIO_FIFO_RST(1) | SDIO_CTRL_RST(1))); 

	writel(MSIZE8 | RX_WMARK(7) | TX_WMARK(8), SDIO1_FIFOTH_V);
}

#ifdef CONFIG_MMC_SEP_DETECT_MODE_POLL_STATUS
void sepmmc_card_detect(void *pdev)
{
	struct sepmmc_host *host = pdev;

	mmc_detect_change(host->mmc, msecs_to_jiffies(500));
	mod_timer(&host->timer, jiffies + msecs_to_jiffies(HZ/2));
}
#endif

static int sepmmc_probe(struct platform_device *pdev)
{
 	struct mmc_host *mmc = NULL;
	struct sepmmc_host *host = NULL;
	struct clk *sdio_clk = NULL;
	int ret;

	mmc = mmc_alloc_host(sizeof(struct sepmmc_host), &pdev->dev);
	if (!mmc)  {
		printk("[%s], mmc alloc host fail\n", __func__);
		return -ENOMEM;
	}

	host = mmc_priv(mmc);
	host->mmc = mmc;
	
	platform_set_drvdata(pdev, mmc);

	mmc->ops = &sepmmc_ops;
	mmc->f_min = 400000; /* 400KHz */
	mmc->f_max = 50000000; /* 50MHz */
	mmc->ocr_avail = MMC_VDD_32_33 | MMC_VDD_33_34;
	mmc->caps = MMC_CAP_4_BIT_DATA | MMC_CAP_SD_HIGHSPEED;
	
	mmc->max_hw_segs = 128;
	mmc->max_phys_segs = 128;
	mmc->max_seg_size = 512 * 30;	/* limited by dma block_ts field: 4095 * 4 ~= 512 * 31 */
	mmc->max_blk_size = 512;
	mmc->max_blk_count = 128 * 30;
	mmc->max_req_size = mmc->max_blk_size * mmc->max_blk_count;

	ret = request_irq(INTSRC_DMAC1, dma_chan_for_sdio1_irq_handler, IRQF_SHARED, "dmac1_4_sdio1", host);
	if (ret) {
		printk("[%s], request INTSRC_DMAC1 irq fail\n", __func__);
		goto out0;
	}
	
	ret = request_irq(INTSRC_SDIO1, sdio1_irq_handler, IRQF_DISABLED, "sdio1", host);
	if (ret) {
		printk("[%s], request INTSRC_SDIO1 irq fail\n", __func__);
		goto out1;
	}

	sdio_clk = clk_get(NULL, "sdio-1");
	if (sdio_clk == NULL) {
		printk("fail to get sdio1 clk, assume 320MiB");
		host->hclk = 320000000;
	} else
		host->hclk = clk_get_rate(sdio_clk);
	
	/* inital sdio1 controler */	
	sdio1_conrtoller_init(host);

	mmc_add_host(mmc);	

#ifdef CONFIG_MMC_SEP_DETECT_MODE_POLL_STATUS
	/* add a timer to card detect */	
	init_timer(&host->timer);
	host->timer.function = sepmmc_card_detect;
	host->timer.data = (unsigned long)host;
	host->timer.expires = jiffies + msecs_to_jiffies(HZ/2);
	add_timer(&host->timer);
#else
	host->suspend = 0;
	enable_data3_card_detect_int(host);	
#endif
	return 0;

 out1:
	free_irq(INTSRC_DMAC1, host);
 out0:
	mmc_free_host(mmc);
	return ret;
} 

static int sepmmc_remove(struct platform_device *pdev)
{	
	struct mmc_host *mmc = platform_get_drvdata(pdev);

	if (mmc) {
		struct sepmmc_host *host = mmc_priv(mmc);
		
		platform_set_drvdata(pdev, NULL);

		mmc_remove_host(mmc);
		mmc_free_host(mmc);
		
		free_irq(INTSRC_SDIO1, host);
		free_irq(INTSRC_DMAC1, host);
	}

	return 0;
}

#ifdef CONFIG_PM
static int sepmmc_suspend(struct platform_device *dev, pm_message_t state)
{
	struct mmc_host *mmc = platform_get_drvdata(dev);
#ifndef CONFIG_MMC_SEP_DETECT_MODE_POLL_STATUS
	struct sepmmc_host *host = mmc_priv(mmc);
#endif
	int ret = 0;
	
	printk("%s\n", __func__);  

	if (mmc) {
#ifndef CONFIG_MMC_SEP_DETECT_MODE_POLL_STATUS
		host->suspend = 1;
		disable_any_int(host);
#endif
		ret = mmc_suspend_host(mmc, state);
	}

	return ret;
}

static int sepmmc_resume(struct platform_device *dev)
{
	struct mmc_host *mmc = platform_get_drvdata(dev);
#ifndef CONFIG_MMC_SEP_DETECT_MODE_POLL_STATUS
	struct sepmmc_host *host = mmc_priv(mmc);
#endif	
	int ret = 0;
	
	printk("%s\n", __func__);

	if (mmc) {
		ret = mmc_resume_host(mmc);
#ifndef CONFIG_MMC_SEP_DETECT_MODE_POLL_STATUS
		host->suspend = 0;
		enable_data3_card_detect_int(host);	
#endif
	}

	return ret;
}
#else
#define sepmmc_suspend	NULL
#define sepmmc_resume	NULL
#endif

static struct platform_driver sepmmc_driver = {
	.probe		= sepmmc_probe,
	.remove		= sepmmc_remove,
	.suspend	= sepmmc_suspend,
	.resume		= sepmmc_resume,
	.driver		= {
		.name	= "sep0611_mmc",
		.owner	= THIS_MODULE,
	},
};

static int __init sepmmc_init(void)
{
	printk("SEP0611 MMC Driver v1.2\n");

	return platform_driver_register(&sepmmc_driver);
}

static void __exit sepmmc_exit(void)
{
	platform_driver_unregister(&sepmmc_driver);
}

module_init(sepmmc_init);
module_exit(sepmmc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("fjj <fanjianjun@wxseuic.com>");
MODULE_DESCRIPTION("SEP0611 MMC Driver");
