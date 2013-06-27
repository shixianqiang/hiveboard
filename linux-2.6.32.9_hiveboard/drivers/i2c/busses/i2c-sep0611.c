/* linux/drivers/i2c/busses/i2c-sep0611.c
 *
 * Copyright (C) 2010 Southeast university
 * chenguangming@wxseuic.com
 *
 * SEP0611 I2C Controller driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * Modifications:
 *  06-02-2011	cgm initial version
 *  07-30-2011	cgm update version, support clk get	
 *  08-05-2011	cgm update version, support set clk dynamically 
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/i2c-id.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <linux/clk.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#include <asm/irq.h>

#include <mach/regs-i2c.h>

#include <board/board.h>

#ifdef CONFIG_CPU_FREQ
#include <linux/cpufreq.h>
#endif

#ifdef CONFIG_PM
#include <linux/pm.h>
#endif

#define TIMEOUT 20

struct sep0611_i2c {
	struct completion cmd_complete;
	struct mutex lock;

	struct i2c_msg *msgs;
	int msgs_num;
	int msg_write_idx;
	int msg_read_idx;
	int cmd_err;
   	int msg_err;

	u16 tx_buf_len;
	u16 rx_buf_len;
	u8 *tx_buf;
	u8 *rx_buf;
	
	int status;
	struct resource *ioarea;
	void __iomem *base;
	struct clk *clk;
	struct device *dev;
	int irq;
   	u16 abort_source;
	struct i2c_adapter adapter;
	unsigned int tx_fifo_depth;
	unsigned int rx_fifo_depth;
};

struct sep0611_i2c * i2cs[3] = {NULL};

static char *abort_sources[] = {
	[ABRT_7B_ADDR_NOACK]	=
		"slave address not acknowledged (7bit mode)",
	[ABRT_10ADDR1_NOACK]	=
		"first address byte not acknowledged (10bit mode)",
	[ABRT_10ADDR2_NOACK]	=
		"second address byte not acknowledged (10bit mode)",
	[ABRT_TXDATA_NOACK]		=
		"data not acknowledged",
	[ABRT_GCALL_NOACK]		=
		"no acknowledgement for a general call",
	[ABRT_GCALL_READ]		=
		"read after general call",
	[ABRT_SBYTE_ACKDET]		=
		"start byte acknowledged",
	[ABRT_SBYTE_NORSTRT]	=
		"trying to send start byte when restart is disabled",
	[ABRT_10B_RD_NORSTRT]	=
		"trying to read when restart is disabled (10bit mode)",
	[ARB_MASTER_DIS]		=
		"trying to use disabled adapter",
	[ARB_LOST]				=
		"lost arbitration",
};

/**
 * sep0611_i2c_init() - initialize the designware i2c master hardware
 * @i2c: device private data
 *
 * This functions configures and enables the I2C master.
 * This function is called during I2C init function, and in case of timeout at
 * run time.
 */
static void sep0611_i2c_init(struct sep0611_i2c *i2c)
{
	u32 input_clock_mhz;
	u32 ic_con, hcnt, lcnt;
	struct clk *clk = i2c->clk;
	
	input_clock_mhz = clk_get_rate(clk) / 1000000UL;

	/* Disable the adapter */
	writew(0, i2c->base + I2C_ENABLE);

	/* set standard and fast speed dividers for high/low periods */
	/* Standard-mode */
	hcnt = input_clock_mhz * 4 + 1; /* std speed high, 4us */
	lcnt = input_clock_mhz * 47 / 10 + 1; /* std speed low, 4.7us */
	writew(hcnt, i2c->base + I2C_SS_SCL_HCNT);
	writew(lcnt, i2c->base + I2C_SS_SCL_LCNT);
	dev_dbg(i2c->dev, "Standard-mode HCNT:LCNT = %d:%d\n", hcnt, lcnt);

	/* Fast-mode */
	hcnt = input_clock_mhz *  6 / 10 + 1; /* fast speed high, 0.6us */
	lcnt = input_clock_mhz * 13 / 10 + 1; /* fast speed low, 1.3us */
	writew(hcnt, i2c->base + I2C_FS_SCL_HCNT);
	writew(lcnt, i2c->base + I2C_FS_SCL_LCNT);
	dev_dbg(i2c->dev, "Fast-mode HCNT:LCNT = %d:%d\n", hcnt, lcnt);
	
	/* High-mode */
	hcnt = input_clock_mhz *  6 / 100 + 1; /* fast speed high, 0.06us */
	lcnt = input_clock_mhz * 16 / 100 + 1; /* fast speed low, 0.16us */
	writew(hcnt, i2c->base + I2C_HS_SCL_HCNT);
	writew(lcnt, i2c->base + I2C_HS_SCL_LCNT);
	dev_dbg(i2c->dev, "High-mode HCNT:LCNT = %d:%d\n", hcnt, lcnt);

	/* Configure Tx/Rx FIFO threshold levels */
	writew(i2c->tx_fifo_depth - 1, i2c->base + I2C_TX_TL);
	writew(0, i2c->base + I2C_RX_TL);

	/* configure the i2c master */
	ic_con = I2C_CON_MASTER_MODE | I2C_CON_SLAVE_DISABLE |
		I2C_CON_RESTART_EN | I2C_CON_F_SPEED;
	writew(ic_con, i2c->base + I2C_CON);	
}

/*
 * Waiting for bus not busy
 */
static int sep0611_i2c_wait_bus_not_busy(struct sep0611_i2c *i2c)
{
	int timeout = TIMEOUT;

	while (readl(i2c->base + I2C_STATUS) & I2C_STATUS_ACTIVITY) {
		if (timeout <= 0) {
			dev_warn(i2c->dev, "timeout waiting for bus ready!\n");
			return -ETIMEDOUT;
		}
		timeout--;
		mdelay(1);
	}

	return 0;
}

static void sep0611_i2c_xfer_init(struct sep0611_i2c *i2c)
{
	struct i2c_msg *msgs = i2c->msgs;
	u32 ic_con/*,ic_con1*/;
	
	/* Disable the adapter */
	writel(0, i2c->base + I2C_ENABLE);

	/* set the slave (target) address */
	writel(msgs[i2c->msg_write_idx].addr, i2c->base + I2C_TAR);

	/* if the slave address is ten bit address, enable 10BITADDR */
	ic_con = readl(i2c->base + I2C_CON);
	if (msgs[i2c->msg_write_idx].flags & I2C_M_TEN)
		ic_con |= I2C_CON_10ADD_M;
	else
		ic_con &= ~I2C_CON_10ADD_M;
	writel(ic_con, i2c->base + I2C_CON);

	/* Enable interrupts */
	writel(I2C_INTR_DEFAULT_MASK, i2c->base + I2C_INTR_MASK);

   	/* Enable I2C interrupt */
   	SEP0611_INT_ENABLE(i2c->irq);

	/* Enable the adapter */
	writel(1, i2c->base + I2C_ENABLE);
}

/*
 * Initiate (and continue) low level master read/write transaction.
 * This function is only called from sep0611_i2c_isr, and pumping i2c_msg
 * messages into the tx buffer.  Even if the size of i2c_msg data is
 * longer than the size of the tx buffer, it handles everything.
 */
static void sep0611_i2c_xfer_msg(struct sep0611_i2c *i2c)
{
	struct i2c_msg *msgs = i2c->msgs;
	u32 intr_mask;
	int tx_limit, rx_limit;
	u32 addr = msgs[i2c->msg_write_idx].addr;
	u32 buf_len = i2c->tx_buf_len;
	u8 *buf = i2c->tx_buf;
	
	intr_mask = I2C_INTR_DEFAULT_MASK;

	for (; i2c->msg_write_idx < i2c->msgs_num; i2c->msg_write_idx++) {
		/*
		 * if target address has changed, we need to
		 * reprogram the target address in the i2c
		 * adapter when we are done with this transfer
		 */
		if (msgs[i2c->msg_write_idx].addr != addr) {
			dev_err(i2c->dev, "%s: invalid target address\n", __func__);
			i2c->msg_err = -EINVAL;
			break;
		}

		if (msgs[i2c->msg_write_idx].len == 0) {
			dev_err(i2c->dev, "%s: invalid message length\n", __func__);
			i2c->msg_err = -EINVAL;
			break;
		}

		if (!(i2c->status & STATUS_WRITE)) {
			/* new i2c_msg */
			buf = msgs[i2c->msg_write_idx].buf;
			buf_len = msgs[i2c->msg_write_idx].len;
		}

		tx_limit = i2c->tx_fifo_depth - readl(i2c->base + I2C_TXFLR);
		rx_limit = i2c->rx_fifo_depth - readl(i2c->base + I2C_RXFLR);

		while (buf_len > 0 && tx_limit > 0 && rx_limit > 0) {
			if (msgs[i2c->msg_write_idx].flags & I2C_M_RD) {
				writel(0x100, i2c->base + I2C_DATA_CMD);
				rx_limit--;
			}
			else{
				writel(*buf++, i2c->base + I2C_DATA_CMD);
			}
			tx_limit--;
			buf_len--;
		}

		i2c->tx_buf = buf;
		i2c->tx_buf_len = buf_len;

		if (buf_len > 0) {
			/* more bytes to be written */
			i2c->status |= STATUS_WRITE;
			break;
		}
		else
			i2c->status &= ~STATUS_WRITE;
	}

	/*
	 * If i2c_msg index search is completed, we don't need TX_EMPTY
	 * interrupt any more.
	 */
	if (i2c->msg_write_idx == i2c->msgs_num)
		intr_mask &= ~I2C_INTR_TX_EMPTY;

	if (i2c->msg_err)
		intr_mask = 0;

	writel(intr_mask, i2c->base + I2C_INTR_MASK);
}


static void sep0611_i2c_read(struct sep0611_i2c *i2c)
{
	struct i2c_msg *msgs = i2c->msgs;
	int rx_valid;

	for (; i2c->msg_read_idx < i2c->msgs_num; i2c->msg_read_idx++) {
		u32 len;
		u8 *buf;

		if (!(msgs[i2c->msg_read_idx].flags & I2C_M_RD))
			continue;

		if (!(i2c->status & STATUS_READ)) {
			len = msgs[i2c->msg_read_idx].len;
			buf = msgs[i2c->msg_read_idx].buf;
		}
		else {
			len = i2c->rx_buf_len;
			buf = i2c->rx_buf;
		}

		rx_valid = readl(i2c->base + I2C_RXFLR);

		for (; len > 0 && rx_valid > 0; len--, rx_valid--)
			*buf++ = readl(i2c->base + I2C_DATA_CMD);

		if (len > 0) {
			i2c->status |= STATUS_READ;
			i2c->rx_buf_len = len;
			i2c->rx_buf = buf;
			return;
		}
		else
			i2c->status &= ~STATUS_READ;
	}
}

static u32 sep0611_i2c_read_clear_intrbits(struct sep0611_i2c *i2c)
{
	u32 stat;

	/*
	 * The IC_INTR_STAT register just indicates "enabled" interrupts.
	 * Ths unmasked raw version of interrupt status bits are available
	 * in the IC_RAW_INTR_STAT register.
	 *
	 * That is,
	 *   stat = readl(IC_INTR_STAT);
	 * equals to,
	 *   stat = readl(IC_RAW_INTR_STAT) & readl(IC_INTR_MASK);
	 *
	 * The raw version might be useful for debugging purposes.
	 */
	stat = readl(i2c->base + I2C_INTR_STAT);

	/*
	 * Do not use the IC_CLR_INTR register to clear interrupts, or
	 * you'll miss some interrupts, triggered during the period from
	 * readl(IC_INTR_STAT) to readl(IC_CLR_INTR).
	 *
	 * Instead, use the separately-prepared IC_CLR_* registers.
	 */
	if (stat & I2C_INTR_RX_UNDER)
		readl(i2c->base + I2C_CLR_RX_UNDER);
	if (stat & I2C_INTR_RX_OVER)
		readl(i2c->base + I2C_CLR_RX_OVER);
	if (stat & I2C_INTR_TX_OVER)
		readl(i2c->base + I2C_CLR_TX_OVER);
	if (stat & I2C_INTR_RD_REQ)
		readl(i2c->base + I2C_CLR_RD_REQ);
	if (stat & I2C_INTR_TX_ABRT) {
		/*
		 * The IC_TX_ABRT_SOURCE register is cleared whenever
		 * the IC_CLR_TX_ABRT is read.  Preserve it beforehand.
		 */
		i2c->abort_source = readl(i2c->base + I2C_TX_ABRT_SOURCE);
		readl(i2c->base + I2C_CLR_TX_ABRT);
	}
	if (stat & I2C_INTR_RX_DONE)
		readl(i2c->base + I2C_CLR_RX_DONE);
	if (stat & I2C_INTR_ACTIVITY)
		readl(i2c->base + I2C_CLR_ACTIVITY);
	if (stat & I2C_INTR_STOP_DET)
		readl(i2c->base + I2C_CLR_STOP_DET);
	if (stat & I2C_INTR_START_DET)
		readl(i2c->base + I2C_CLR_START_DET);
	if (stat & I2C_INTR_GEN_CALL)
		readl(i2c->base + I2C_CLR_GEN_CALL);

	return stat;
}


/*
 * Interrupt service routine. This gets called whenever an I2C interrupt
 * occurs.
 */
static irqreturn_t sep0611_i2c_isr(int this_irq, void *dev_id)
{
	struct sep0611_i2c *i2c = dev_id;
	u32 stat;
	
	stat = sep0611_i2c_read_clear_intrbits(i2c);
	dev_dbg(i2c->dev, "%s: stat=0x%x\n", __func__, stat);
	
	if (stat & I2C_INTR_TX_ABRT) {
		i2c->cmd_err |= I2C_ERR_TX_ABRT;
		i2c->status = STATUS_IDLE;

		/*
		 * Anytime TX_ABRT is set, the contents of the tx/rx
		 * buffers are flushed.  Make sure to skip them.
		 */
		writel(0, i2c->base + I2C_INTR_MASK);
		
		printk("abort!\n\n");
		
		goto tx_aborted;
	}
	
	if (stat & I2C_INTR_RX_FULL)
		sep0611_i2c_read(i2c);
		
	if (stat & I2C_INTR_TX_EMPTY)
		sep0611_i2c_xfer_msg(i2c);

	/*
	 * No need to modify or disable the interrupt mask here.
	 * i2c_dw_xfer_msg() will take care of it according to
	 * the current transmit status.
	 */

tx_aborted:
	if ((stat & (I2C_INTR_TX_ABRT | I2C_INTR_STOP_DET)) || i2c->msg_err)
		complete(&i2c->cmd_complete);

	return IRQ_HANDLED;
}

static int sep0611_i2c_handle_tx_abort(struct sep0611_i2c *i2c)
{
	unsigned long abort_source = i2c->abort_source;
	int i;

	if (abort_source & I2C_TX_ABRT_NOACK) {
		for_each_bit(i, &abort_source, ARRAY_SIZE(abort_sources))
		   dev_dbg(i2c->dev, "%s: %s\n", __func__, abort_sources[i]);
		return -EREMOTEIO;
	}

	for_each_bit(i, &abort_source, ARRAY_SIZE(abort_sources))
		dev_err(i2c->dev, "%s: %s\n", __func__, abort_sources[i]);

	if (abort_source & ARB_LOST)
		return -EAGAIN;
	else if (abort_source & ABRT_GCALL_READ)
		return -EINVAL; /* wrong msgs[] data */
	else
		return -EIO;
}

/*
 * Prepare controller for a transaction and call sep0611_i2c_xfer_msg
 */
static int sep0611_i2c_xfer(struct i2c_adapter *adap, struct i2c_msg msgs[], int num)
{
	struct sep0611_i2c *i2c = i2c_get_adapdata(adap);
	int ret;

	dev_dbg(i2c->dev, "%s: msgs: %d\n", __func__, num);
	
	mutex_lock(&i2c->lock);

	INIT_COMPLETION(i2c->cmd_complete);
	i2c->msgs = msgs;
	i2c->msgs_num = num;
	i2c->cmd_err = 0;
	i2c->msg_write_idx = 0;
	i2c->msg_read_idx = 0;
	i2c->msg_err = 0;
	i2c->status = STATUS_IDLE;
	i2c->abort_source = 0;

	ret = sep0611_i2c_wait_bus_not_busy(i2c);
	if (ret < 0)
		goto done;
		
	/* start the transfers */
	sep0611_i2c_xfer_init(i2c);

	/* wait for tx to complete */
	ret = wait_for_completion_interruptible_timeout(&i2c->cmd_complete, HZ);
	if (ret == 0) {
		dev_err(i2c->dev, "controller timed out\n");
		sep0611_i2c_init(i2c);
		ret = -ETIMEDOUT;
		goto done;
	} else if (ret < 0)
		goto done;

	if (i2c->msg_err) {
		ret = i2c->msg_err;
		goto done;
	}

	/* no error */
	if (likely(!i2c->cmd_err)) {
		/* Disable the adapter */
		writel(0, i2c->base + I2C_ENABLE);
		ret = num;
		goto done;
	}

	/* We have an error */
	if (i2c->cmd_err == I2C_ERR_TX_ABRT) {
		ret = sep0611_i2c_handle_tx_abort(i2c);
		goto done;
	}
	ret = -EIO;

done:
	mutex_unlock(&i2c->lock);

	return ret;
}

static u32 sep0611_i2c_func(struct i2c_adapter *adap)
{
	return	I2C_FUNC_I2C |
		I2C_FUNC_10BIT_ADDR |
		I2C_FUNC_SMBUS_BYTE |
		I2C_FUNC_SMBUS_BYTE_DATA |
		I2C_FUNC_SMBUS_WORD_DATA |
		I2C_FUNC_SMBUS_I2C_BLOCK;
}

static struct i2c_algorithm sep0611_i2c_algo = {
	.master_xfer	= sep0611_i2c_xfer,
	.functionality	= sep0611_i2c_func,
};
/*************add by zy**************/
#ifdef CONFIG_CPU_FREQ
struct notifier_block   sep_i2c_freq_transition;
static int sep_i2c_cpufreq_transition(struct notifier_block *nb,
                      unsigned long val, void *data)
{
	int i;

	for(i=0; i<3; i++){
		if(i2cs[i])
			sep0611_i2c_init(i2cs[i]);
	}
    
	return 0;
}

static inline int sep_i2c_cpufreq_register(void)
{
    sep_i2c_freq_transition.notifier_call = sep_i2c_cpufreq_transition;

    return cpufreq_register_notifier(&sep_i2c_freq_transition,
                     CPUFREQ_TRANSITION_NOTIFIER);
}

static inline void sep_i2c_cpufreq_deregister(void)
{
    cpufreq_unregister_notifier(&sep_i2c_freq_transition,
                    CPUFREQ_TRANSITION_NOTIFIER);
}

#else
static inline int sep_i2c_cpufreq_register(void)
{
    return 0;
}

static inline void sep_i2c_cpufreq_deregister(void)
{
}
#endif
/*************add by zy**************/

static int sep0611_i2c_probe(struct platform_device *pdev)
{
   	struct sep0611_i2c *i2c;
   	struct i2c_adapter *adap;
   	struct resource *mem, *ioarea;
	struct clk *clk = NULL;
   	u32 param1;
   	int irq, ret;
	
   	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
   	if(!mem){
		dev_err(&pdev->dev, "No mem resource!\n");
     	return -EINVAL;
	} 

   	irq = platform_get_irq(pdev, 0);
   	if (irq < 0) {
		dev_err(&pdev->dev, "No irq resource!\n");
		return irq; /* -ENXIO */
	}

	switch(pdev->id){
	case 1:
		sep0611_gpio_cfgpin(SEP0611_GPE21, I2C1_CLK);
		sep0611_gpio_cfgpin(SEP0611_GPE20, I2C1_DAT);
		clk = clk_get(NULL, "i2c-1");
		break;
		
	case 2:
		sep0611_gpio_cfgpin(SEP0611_GPE19, I2C2_CLK);
		sep0611_gpio_cfgpin(SEP0611_GPE18, I2C2_DAT);
		clk = clk_get(NULL, "i2c-2");
		break;
	
	case 3:
		sep0611_gpio_cfgpin(SEP0611_GPE17, I2C3_CLK);
		sep0611_gpio_cfgpin(SEP0611_GPE16, I2C3_DAT);
		clk = clk_get(NULL, "i2c-3");
		break;
	
	default:
		break;
	}
	if(clk == NULL){
		printk("ERR:not found i2c%d clk\n", pdev->id);
		return -ENODEV;
	}
	clk_enable(clk);
	
   	ioarea = request_mem_region(mem->start, resource_size(mem), pdev->name);
	if (!ioarea) {
		dev_err(&pdev->dev, "I2C region already claimed\n");
		return -EBUSY;
	}

   	i2c = kzalloc(sizeof(struct sep0611_i2c), GFP_KERNEL);
	if (!i2c) {
		ret = -ENOMEM;
		goto err_release_region;
	}
	i2cs[pdev->id] = i2c;

	init_completion(&i2c->cmd_complete);
	mutex_init(&i2c->lock);
	i2c->dev = get_device(&pdev->dev);
	i2c->irq = irq;
	platform_set_drvdata(pdev, i2c);

   	i2c->base = ioremap(mem->start, resource_size(mem));
	if (i2c->base == NULL) {
		dev_err(&pdev->dev, "Failure mapping io resources\n");
		ret = -EBUSY;
		goto err_free_mem;
	}
	
	param1 = readl(i2c->base + I2C_COMP_PARAM_1);
	i2c->tx_fifo_depth = ((param1 >> 16) & 0xff) + 1;
	i2c->rx_fifo_depth = ((param1 >> 8)  & 0xff) + 1;

	i2c->clk = clk;
   	sep0611_i2c_init(i2c);

   	writel(0, i2c->base + I2C_INTR_MASK); /* disable IRQ */
	ret = request_irq(i2c->irq, sep0611_i2c_isr, IRQF_DISABLED, pdev->name, i2c);
	if (ret) {
		dev_err(&pdev->dev, "Failure requesting irq %i\n", i2c->irq);
		goto err_iounmap;
	}

   	adap = &i2c->adapter;
	i2c_set_adapdata(adap, i2c);
	adap->owner = THIS_MODULE;
	adap->class = I2C_CLASS_HWMON;
	strlcpy(adap->name, "SEP0611 I2C adapter", sizeof(adap->name));
	adap->algo = &sep0611_i2c_algo;
	adap->dev.parent = &pdev->dev;
   	adap->nr = pdev->id;
   	
	ret = i2c_add_numbered_adapter(adap);
	if (ret) {
		dev_err(&pdev->dev, "Failure adding adapter!\n");
		goto err_free_irq;
	}
	
	printk("Adapter %d at 0x%08lX, IRQ%d\n", pdev->id, (unsigned long)(mem->start), irq);	

	return 0;

err_free_irq:
	free_irq(i2c->irq, i2c);
err_iounmap:
   	iounmap(i2c->base);
err_free_mem:
	platform_set_drvdata(pdev, NULL);
	put_device(&pdev->dev);
	kfree(i2c);
err_release_region:
	release_mem_region(mem->start, resource_size(mem));

	return ret;
}

/* sep0611_i2c_remove
 * called when adapter is removed from the bus
 */
static int sep0611_i2c_remove(struct platform_device *pdev)
{
	struct sep0611_i2c *i2c = platform_get_drvdata(pdev);
	struct resource *mem;

	platform_set_drvdata(pdev, NULL);
	i2c_del_adapter(&i2c->adapter);
	put_device(&pdev->dev);

	writel(0, i2c->base + I2C_ENABLE);
	free_irq(i2c->irq, i2c);
	if(i2c->clk)
		clk_disable(i2c->clk);
	kfree(i2c);

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	release_mem_region(mem->start, resource_size(mem));
	return 0;
}

#ifdef CONFIG_PM
static int sep0611_i2c_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct sep0611_i2c *i2c = platform_get_drvdata(pdev);

	if(i2c->clk)
		clk_disable(i2c->clk);

	return 0;
}

static int sep0611_i2c_resume(struct platform_device *pdev)
{
	struct sep0611_i2c *i2c = platform_get_drvdata(pdev);
	
	if (i2c == NULL)
		return -1;
	
	if(i2c->clk)
		clk_enable(i2c->clk);
	sep0611_i2c_init(i2c);

	switch(pdev->id){
	case 1:
		sep0611_gpio_cfgpin(SEP0611_GPE21, I2C1_CLK);
		sep0611_gpio_cfgpin(SEP0611_GPE20, I2C1_DAT);
		break;
		
	case 2:
		sep0611_gpio_cfgpin(SEP0611_GPE19, I2C2_CLK);
		sep0611_gpio_cfgpin(SEP0611_GPE18, I2C2_DAT);
		break;
	
	case 3:
		sep0611_gpio_cfgpin(SEP0611_GPE17, I2C3_CLK);
		sep0611_gpio_cfgpin(SEP0611_GPE16, I2C3_DAT);
		break;
	
	default:
		break;
	}

	return 0;
}
#else
#define sep0611_i2c_suspend	    NULL
#define sep0611_i2c_resume  	NULL
#endif


static struct platform_driver sep0611_i2c_driver = {
   	.probe     = sep0611_i2c_probe,
	.remove    = sep0611_i2c_remove,
	.suspend   = sep0611_i2c_suspend,
	.resume    = sep0611_i2c_resume,
	.driver    = {
        .owner = THIS_MODULE,
        .name  = "sep0611-i2c",
	},
};

static int __init i2c_adap_sep0611_init(void)
{
	printk("SEP0611 I2C adapter driver v1.2\n");
	
	/*************add by zy**************/
	sep_i2c_cpufreq_register();
	/*************add by zy**************/
	return platform_driver_register(&sep0611_i2c_driver);
}

static void __exit i2c_adap_sep0611_exit(void)
{
	platform_driver_unregister(&sep0611_i2c_driver);
	
	/*************add by zy**************/
	sep_i2c_cpufreq_deregister();
	/*************add by zy**************/
}

module_init(i2c_adap_sep0611_init);
module_exit(i2c_adap_sep0611_exit);

MODULE_DESCRIPTION("SEP0611 I2C adapter driver");
MODULE_AUTHOR("chenguangming@wxseuic.com");
MODULE_LICENSE("GPL");

