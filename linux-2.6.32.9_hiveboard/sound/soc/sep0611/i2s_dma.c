/* linux/sound/soc/sep/i2s_dma.c
 *
 * Copyright (c) 2009-2011 SEUIC
 *  cgm <chenguangming@wxseuic.com>
 *
 * Southeast University ASIC SoC support
 *
 * DMAC configuration for SEP0611 ASoC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  05-31-2011	cgm initial version
 *
 */
 
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/list.h>

#include <asm/io.h>

#include <mach/hardware.h>
#include <mach/irqs.h>
#include <mach/io.h>
#include <mach/regs-i2s.h>

#include <sound/soc.h>
#include <sound/core.h>
#include <sound/pcm.h>

#include "sep0611-i2s.h"
#include "sep0611-pcm.h"
#include "i2s_dma.h"

#include "alsa_debug.h"

static struct tasklet_struct i2s_tx_tasklet;
static struct tasklet_struct i2s_rx_tasklet;
static int dev_id = DMA_ALSA_DEV_ID;

/* dma link list node struct */
struct dma_lli{
	unsigned long sar;
	unsigned long dar;
	unsigned long llp;
	unsigned long ctrl_lo;
	unsigned long ctrl_hi;
	unsigned long dstat;	
};

/* dma link list */
struct dma_list{
	struct dma_lli lli;
	struct list_head list;
};

/*
 * set up the dma circle link list
 */
static int set_dma_cll(struct dma_info *p_dma_info, unsigned long buf_len,
	unsigned long period_len)
{
	unsigned long num, i;
	unsigned long phy_addr;
	struct dma_list *p_node = NULL, *p_prev;
#ifdef _DEBUG_DMAC_REGS_VIEW
	struct list_head *p_pos;
#endif	
	if(!p_dma_info)
		return -EINVAL;
		
	INIT_LIST_HEAD(&(p_dma_info->dma_list_head));
		
	if(p_dma_info->trans_type == MEM_TO_DEV)
		phy_addr = p_dma_info->src_addr;
	else if(p_dma_info->trans_type == DEV_TO_MEM)
		phy_addr = p_dma_info->dst_addr;
	else
		return -EINVAL;
	
	num = buf_len/period_len;
	if(buf_len > period_len * num){
		printk(KERN_ERR "buffer length must be multiple to period length!\n");
		return -EINVAL;
	}
	p_prev = NULL;
	
	for(i=0; i<num; i++){
		p_node = kmalloc(sizeof(struct dma_list), GFP_KERNEL);
		if(!p_node)
			return -ENOMEM;

		if(p_prev){
			p_prev->lli.llp = virt_to_phys(&p_node->lli);
		}
		
		/* different transmit type, different way */
		if(p_dma_info->trans_type == MEM_TO_DEV){
			p_node->lli.sar = phy_addr + i*period_len;	
			p_node->lli.dar = p_dma_info->dst_addr;
		}
		else{
			p_node->lli.sar = p_dma_info->src_addr;
			p_node->lli.dar = phy_addr + i*period_len;
		}
		
		p_node->lli.ctrl_lo = p_dma_info->ctrl_lo;
		p_node->lli.ctrl_hi = p_dma_info->ctrl_hi;		
		
		list_add_tail(&(p_node->list), &(p_dma_info->dma_list_head));
		
		p_prev = p_node;
	}
	
	p_prev = container_of(p_dma_info->dma_list_head.next, struct dma_list, list);	/*find the fist node*/
	p_node->lli.llp = virt_to_phys(&p_prev->lli);			/*last llp <==> fisrt node, make the circle list*/
	p_dma_info->first_llp = p_node->lli.llp & 0xFFFFFFFC;	/*ensure the address alin in 32bits*/

#ifdef _DEBUG_DMAC_REGS_VIEW	
	list_for_each(p_pos, &(p_dma_info->dma_list_head)){
		p_node = list_entry(p_pos, struct dma_list, list);
		
		printk("\nlli.sar = 0x%08lx\n", p_node->lli.sar);
		printk("lli.dar = 0x%08lx\n", p_node->lli.dar);
		printk("lli.llp = 0x%08lx\n", p_node->lli.llp);
		printk("lli.ctrl_lo = 0x%08lx\n", p_node->lli.ctrl_lo);
		printk("lli.ctrl_hi = 0x%08lx\n", p_node->lli.ctrl_hi);
		printk("&lli = 0x%08lx\n", virt_to_phys(&p_node->lli));
	}
#endif
	return 0;
}

/*
 * load the first dma llp into channel registers, but not start it
 */
static int load_first_llp(struct dma_info *p_dma_info, unsigned long ch)
{
	unsigned long ch_bit = 1<<ch;

    /* unmask the dma inerrupt*/	
	writel(DMAC_CH_WRITE_EN(ch_bit), DMAC2_MASKTRF_V);
	writel(DMAC_CH_WRITE_EN(ch_bit), DMAC2_MASKERR_V);
	writel((DMAC_CH_WRITE_EN(ch_bit) | ch_bit), DMAC2_MASKBLK_V);
    
    /* clear any pending interrupts */
    writel((readl(DMAC2_CLRTFR_V) | ch_bit), DMAC2_CLRTFR_V);
    writel((readl(DMAC2_CLRBLK_V) | ch_bit), DMAC2_CLRBLK_V);
    writel((readl(DMAC2_CLRERR_V) | ch_bit), DMAC2_CLRERR_V);
    writel((readl(DMAC2_CLRSRCTR_V) | ch_bit), DMAC2_CLRSRCTR_V);
    writel((readl(DMAC2_CLRDSTTR_V) | ch_bit), DMAC2_CLRDSTTR_V);
	
	/* enale the DMAC2 global */
    writel(0x00, (DMAC2_CFG_V + 4));
    writel(DMAC_CFG_REG_EN(1), DMAC2_CFG_V);
    
    /* disable the channel */
	writel(DMAC_CH_WRITE_EN(ch_bit), DMAC2_CHEN_V);     
	
	if(ch == PLAY_CH){
		/* write the llp */
		writel(0, (DMAC2_LLP_CH(PLAY_CH) + 4));
		writel(p_dma_info->first_llp, DMAC2_LLP_CH(PLAY_CH));    
		
		/* enable the llp to load the content(include ctl_lo, ctl_hi, next_llp, sar, dar) */
	    writel(0, (DMAC2_CTL_CH(PLAY_CH) + 4));
	    writel(DMAC_DST_LLP_EN(1) | DMAC_SRC_LLP_EN(1), DMAC2_CTL_CH(PLAY_CH)); /* enable dst_llp and src_llp to be sure */
	    
	    /* cfg is not in lli, so we need to write by ourselves */
	    writel(p_dma_info->cfg_hi, DMAC2_CFG_CH(PLAY_CH) + 4);
	    writel(p_dma_info->cfg_lo, DMAC2_CFG_CH(PLAY_CH)); 
	    
#ifdef _DEBUG_DMAC_REGS_VIEW	    
		printk("DMAC2_LLP(PLAY) = 0x%08lx\n", readl(DMAC2_LLP_CH(PLAY_CH)));
		printk("DMAC2_CTL(PLAY) = 0x%08lx\n", readl(DMAC2_CTL_CH(PLAY_CH)));
		printk("DMAC2_SAR(PLAY) = 0x%08lx\n", readl(DMAC2_SAR_CH(PLAY_CH)));
		printk("DMAC2_DAR(PLAY) = 0x%08lx\n", readl(DMAC2_DAR_CH(PLAY_CH)));
		printk("DMAC2_CFG = 0x%08lx\n", readl(DMAC2_CFG_V));
#endif
	}
	else{
		/* write the llp */
		writel(0, (DMAC2_LLP_CH(CAPT_CH) + 4));
		writel(p_dma_info->first_llp, DMAC2_LLP_CH(CAPT_CH));
		
		/* enable the llp to load the content(include ctl_lo, ctl_hi, next_llp, sar, dar) */
	    writel(0, (DMAC2_CTL_CH(CAPT_CH) + 4));
	    writel(DMAC_DST_LLP_EN(1) | DMAC_SRC_LLP_EN(1), DMAC2_CTL_CH(CAPT_CH));
	    
	    /* cfg is not in lli, so we need to write by ourselves */
	    writel(p_dma_info->cfg_hi, DMAC2_CFG_CH(CAPT_CH) + 4);
	    writel(p_dma_info->cfg_lo, DMAC2_CFG_CH(CAPT_CH));

#ifdef _DEBUG_DMAC_REGS_VIEW	    
		printk("DMAC2_LLP(CAPT) = 0x%08lx\n", readl(DMAC2_LLP_CH(CAPT_CH)));
		printk("DMAC2_CTL(CAPT) = 0x%08lx\n", readl(DMAC2_CTL_CH(CAPT_CH)));
		printk("DMAC2_SAR(CAPT) = 0x%08lx\n", readl(DMAC2_SAR_CH(CAPT_CH)));
		printk("DMAC2_DAR(CAPT) = 0x%08lx\n", readl(DMAC2_DAR_CH(CAPT_CH)));
		printk("DMAC2_CFG = 0x%08lx\n", readl(DMAC2_CFG_V));
#endif
	}
	
	return 0;
}

/*
 * prepare DMA for pcm
 */
int i2s_dma_prepare(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct sep0611_runtime_data *prtd = runtime->private_data;
	struct dma_info *p_dma_info;
	unsigned long i2s_fifo_width = 1;	/* 16 bits in default */
	int ret;
	
	/* we need call the i2s_dma_prepare after set hw_params, such as in pcm_prepare */
	if(runtime->format == 0){
		printk("ERR:please ensure the hw_params has been set correctly\n");
		return -EINVAL;
	}
	
	p_dma_info = kmalloc(sizeof(struct dma_info), GFP_KERNEL);
	if(!p_dma_info){
		return -ENOMEM;
	}
	
	if(substream->stream == SNDRV_PCM_STREAM_PLAYBACK){
		p_dma_info->trans_type = MEM_TO_DEV;
   		p_dma_info->src_addr = prtd->dma_buffer;
   		p_dma_info->dst_addr = I2S_BASE + I2S_TXDMA;
   		
  		/* 
  		 * match with tx data of (S16_LE | S20_3LE | S24_3LE | S32_LE)
		 */
		if(runtime->format != SNDRV_PCM_FORMAT_S16_LE)
			i2s_fifo_width = 2;
   		
   		/* CTLx and CFGx */
		p_dma_info->ctrl_hi = DMAC_DONE (0) | DMAC_BLK_TRAN_SZ(prtd->period_len/8);
		
		p_dma_info->ctrl_lo = DMAC_SRC_LLP_EN    (1) | DMAC_DST_LLP_EN 		(0) | DMAC_SRC_MASTER_SEL(0)\
			| DMAC_DST_MASTER_SEL(1)| DMAC_TRAN_TYPE_FLOW_CTL(1)| DMAC_DST_SCAT_EN	(0) \
            | DMAC_SRC_GATH_EN   (0)| DMAC_SRC_BST_SZ 		 (2)| DMAC_DST_BST_SZ	(3) \
            | DMAC_SRC_INCR    	 (0)| DMAC_DST_INCR          (3)| DMAC_SRC_TRAN_WIDTH(3)\
            | DMAC_DST_TRAN_WIDTH(i2s_fifo_width) | DMAC_INT_EN(1);
        
		p_dma_info->cfg_hi = DMAC_DST_PER    (7) | DMAC_SRC_PER  (6) | DMAC_SRC_STAT_UPD_EN(0) \
            | DMAC_DST_STAT_UPD_EN(0) | DMAC_PROT_CTL (1) | DMAC_FIFO_MODE(0) \
            | DMAC_FLOW_CTL_MODE  (0); 
		
		p_dma_info->cfg_lo = DMAC_AUTO_RELOAD_DST(0) | DMAC_AUTO_RELOAD_SRC(0) | DMAC_MAX_AMBA_BST_LEN(0) \
            | DMAC_SRC_HS_POL     (0) | DMAC_DST_HS_POL   (0) | DMAC_BUS_LOCK       (0) \
            | DMAC_CH_LOCK        (0) | DMAC_BUS_LOCK_LEVEL(1)| DMAC_HW_SW_SEL_SRC  (1) \
            | DMAC_HW_SW_SEL_DST  (0) | DMAC_CH_SUSP      (0) | DMAC_CH_PRIOR       (0);
   	}
   	else{
		p_dma_info->trans_type = DEV_TO_MEM;
   		p_dma_info->src_addr = I2S_BASE + I2S_RXDMA;
   		p_dma_info->dst_addr = prtd->dma_buffer;
   	
   		/* CTLx and CFGx */
   	    p_dma_info->ctrl_hi = DMAC_DONE(0) | DMAC_BLK_TRAN_SZ(prtd->period_len/2); 
   	    
    	p_dma_info->ctrl_lo = DMAC_SRC_LLP_EN (0)| DMAC_DST_LLP_EN (1) | DMAC_SRC_MASTER_SEL   (1) \
            | DMAC_DST_MASTER_SEL(0) | DMAC_TRAN_TYPE_FLOW_CTL(2) | DMAC_DST_SCAT_EN    (0) \
            | DMAC_SRC_GATH_EN   (0) | DMAC_SRC_BST_SZ        (2) | DMAC_DST_BST_SZ     (3) \
            | DMAC_SRC_INCR      (3) | DMAC_DST_INCR          (0) | DMAC_SRC_TRAN_WIDTH (i2s_fifo_width) \
            | DMAC_DST_TRAN_WIDTH(2) | DMAC_INT_EN(1);
 	   	
	   	p_dma_info->cfg_hi = DMAC_DST_PER(7) | DMAC_SRC_PER(6) | DMAC_SRC_STAT_UPD_EN(0) \
            | DMAC_DST_STAT_UPD_EN(0) | DMAC_PROT_CTL (1) | DMAC_FIFO_MODE      (0) \
            | DMAC_FLOW_CTL_MODE  (0);
                       
		p_dma_info->cfg_lo = DMAC_AUTO_RELOAD_DST(0) | DMAC_AUTO_RELOAD_SRC(0) | DMAC_MAX_AMBA_BST_LEN(0) \
			| DMAC_SRC_HS_POL     (0) | DMAC_DST_HS_POL     (0) | DMAC_BUS_LOCK        (0) \
	     	| DMAC_CH_LOCK        (0) | DMAC_BUS_LOCK_LEVEL (1) | DMAC_HW_SW_SEL_SRC   (0) \
	   		| DMAC_HW_SW_SEL_DST  (1) | DMAC_CH_SUSP        (0) | DMAC_CH_PRIOR        (0);
    }
    
    /* store p_dma_info in prtd */
    prtd->dma_info = p_dma_info;
    
    ret = set_dma_cll(p_dma_info, prtd->buffer_len, prtd->period_len);
    if(ret < 0){
    	return -ENOMEM;
    }
    
    load_first_llp(prtd->dma_info, prtd->dma_params->channel);
    
    return 0;
}

/*
 * get the dma hardwre position during the transmit
 */
unsigned long i2s_dma_pointer(struct snd_pcm_substream *substream)
{
	if(substream->stream == SNDRV_PCM_STREAM_PLAYBACK){
		return readl(DMAC2_SAR_CH(PLAY_CH));
	}
	else{
		return readl(DMAC2_DAR_CH(CAPT_CH));
	}
}

/*
 * dmac2 interrupt handler for pcm
 */
static irqreturn_t pcm_dma_handler(int irq, void *dev_id)
{
	unsigned long status = readl(DMAC2_STATBLK_V);
	
	if((status & (1<<PLAY_CH)) && sep0611_pcms.play_ptr){
   		writel(DMAC_CH_WRITE_EN(1<<PLAY_CH), DMAC2_MASKBLK_V);			/* mask play channel */
   		writel(readl(DMAC2_CLRBLK_V) | (1<<PLAY_CH), DMAC2_CLRBLK_V);	/* clear play channel's flag */
		
		tasklet_schedule(&i2s_tx_tasklet);
		
   		writel(DMAC_CH_WRITE_EN(1<<PLAY_CH) | (1<<PLAY_CH), DMAC2_MASKBLK_V);	/* unmask play channel */
	}
	
	if((status & (1<<CAPT_CH)) && sep0611_pcms.capt_ptr){
   		writel(DMAC_CH_WRITE_EN(1<<CAPT_CH), DMAC2_MASKBLK_V);			/* mask record channel */
   		writel(readl(DMAC2_CLRBLK_V) | (1<<CAPT_CH), DMAC2_CLRBLK_V);	/* clear record channel's flag */
		
		tasklet_schedule(&i2s_rx_tasklet);
		
   		writel(DMAC_CH_WRITE_EN(1<<CAPT_CH) | (1<<CAPT_CH), DMAC2_MASKBLK_V);	/* unmask record channel */
	}
	
	return IRQ_HANDLED;
}

/*
 * pcm tx tasklet fun
 */
static void tx_tasklet_fun(unsigned long data)
{
	sep0611_pcm_period_done(sep0611_pcms.play_ptr);
}

/*
 * pcm rx tasklet fun
 */
static void rx_tasklet_fun(unsigned long data)
{
	sep0611_pcm_period_done(sep0611_pcms.capt_ptr);
}

/*
 * initialize the dmac2 for pcm
 */
void i2s_dma_init(void)
{
	int ret;

	/* shared interrupts must pass in a real dev-ID */
	ret = request_irq(INTSRC_DMAC2, pcm_dma_handler, IRQF_SHARED, "DMAC2", (void *)&dev_id);
	if (ret < 0){
		printk("sep0611-dmac2: interrupt request failed.\n");
		return;
	}
	
	tasklet_init(&i2s_tx_tasklet, tx_tasklet_fun, 0);
	tasklet_init(&i2s_rx_tasklet, rx_tasklet_fun, 0);
}

/*
 * start the dma transmit for pcm
 */
void i2s_dma_start(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct sep0611_runtime_data *prtd = runtime->private_data;
	unsigned long ch_bit = 1<<(prtd->dma_params->channel);

	writel(DMAC_CH_WRITE_EN(ch_bit) | ch_bit, DMAC2_MASKBLK_V);
	writel(DMAC_CH_WRITE_EN(ch_bit) | DMAC_CH_EN(ch_bit), DMAC2_CHEN_V);
}

/*
 * stop the dma transmit for pcm
 */
void i2s_dma_stop(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct sep0611_runtime_data *prtd = runtime->private_data;
	unsigned long ch_bit = 1<<(prtd->dma_params->channel);
	
	writel(DMAC_CH_WRITE_EN(ch_bit) | DMAC_CH_EN(0), DMAC2_CHEN_V);
	writel(DMAC_CH_WRITE_EN(ch_bit), DMAC2_MASKBLK_V);
}

/*
 * free the dma lli structures
 */
void i2s_dma_free(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct sep0611_runtime_data *prtd = runtime->private_data;
	struct dma_info *p_dma_info = prtd->dma_info;
	struct dma_list *p_node;
	struct list_head *p_pos;
	struct list_head *n;
	
	if(!p_dma_info)
		return;
		
	alsa_dbg("%s\n", __func__);	
	
	/* use list_for_each_safe when delete list members */
	list_for_each_safe(p_pos, n, &(p_dma_info->dma_list_head)){
		p_node = list_entry(p_pos, struct dma_list, list);
		list_del(p_pos);	/* we must delete node from list before free node */ 
		kfree(p_node);
	}
		
	kfree(p_dma_info);
	prtd->dma_info = NULL;	/* avoid the wild pointer */
}
