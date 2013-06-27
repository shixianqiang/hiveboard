/* linux/drivers/input/touchscreen/sep0611_ts.c
 *
 *
 *  Copyright (C) 2010 SEU ASIC.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/kernel.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/delay.h>

#include <asm/io.h>

#include <mach/hardware.h>
#include <mach/regs-spi.h>
#include <mach/irqs.h>

#include <board/board.h>

/* For ts->dev.id.version */
#define SEP_TSVERSION	0x0101
#define SPI_BASE 	SSI1_BASE

#define PEN_UP          0
#define PEN_UNSURE      1
#define PEN_DOWN        2

#define MAX_X  	        0x7ff
#define MIN_X  		0
#define MAX_Y  		0x7ff
#define MIN_Y  		0

#define CSL    	sep0611_gpio_setpin(SEP0611_GPE10, GPIO_LOW)	//cs 片选信号拉低
#define CSH     sep0611_gpio_setpin(SEP0611_GPE10, GPIO_HIGH)	//cs 片选信号拉高

#define PEN_TIMER_DELAY_JUDGE          	msecs_to_jiffies(5)	// judge whether the pendown message is a true pendown     jiffes
#define PEN_TIMER_DELAY_LONGTOUCH      	msecs_to_jiffies(3)	// judge whether the pendown message is a long-time touch  jiffes
#define PEN_TIMER_DELAY_DETECT			msecs_to_jiffies(100)	// judge whether there is a pendown  jiffes

#define X_LOCATION_CMD  0x90
#define Y_LOCATION_CMD  0xd0

static void __iomem  *base;

/* SEP0611 TS DEBUD SWITCH */
//#define _SEP0611_TS_DEBUGE

#ifdef _SEP0611_TS_DEBUGE
	#define ts_dbg(fmt, args...) printk(fmt, ##args)
#else
	#define ts_dbg(fmt, args...)
#endif

/* 触摸屏结构体 */
struct sep_ts{
	struct input_dev *input;
	struct timer_list ts_timer;
	struct timer_list detect_timer;

	unsigned int penStatus;
};

static struct sep_ts *ts;

static void sep0611_ts_setup(void)
{
//	disable_irq(SEP0611_TOUCH_INTSRC);
   	sep0611_gpio_setirq(SEP0611_TOUCH_INT, LOW_TRIG);

	sep0611_gpio_cfgpin(SEP0611_GPE12, SSI1_RXD);
	sep0611_gpio_cfgpin(SEP0611_GPE13, SSI1_TXD);
	sep0611_gpio_cfgpin(SEP0611_GPE14, SSI1_CS);
	sep0611_gpio_cfgpin(SEP0611_GPE15, SSI1_CLK);

	/* spi寄存器的配置  */
	writel(0x0, base + SSI_ENR);
	writel(0xcB, base + SSI_CONTROL0);
	writel(0x00, base + SSI_CONTROL1);
	writel(0x200, base + SSI_BAUDR);
	writel(0x1, base + SSI_TXFTLR);
	writel(0x0, base + SSI_RXFTLR);
	writel(0x0, base + SSI_DMACR);
	writel(0x0, base + SSI_IMR);
	writel(0x1, base + SSI_SER);
	writel(0x0, base + SSI_ENR);

	mdelay(2);

	sep0611_gpio_clrirq(SEP0611_TOUCH_INT); /* 清除中断  */
//	enable_irq(SEP0611_TOUCH_INTSRC);
}

static unsigned short PenSPIXfer(unsigned short ADCommd)
{
 	unsigned int data=0; //16位

	//传输命令
	writel(0x1, base + SSI_ENR);
	writel(ADCommd, base + SSI_DR);

	while (!(readl(base + SSI_SR) & 0x4));
	while (readl(base + SSI_SR) & 0x1);
	readl(base + SSI_DR);
#if 1
	writel(0, base + SSI_DR);
	while (!(readl(base + SSI_SR) & 0x4));
	while (readl(base + SSI_SR) & 0x1);
#endif
	data = readl(base + SSI_DR);

	writel(0x0, base + SSI_ENR);

	return data;
}

static void tsevent(void)
{
	static int xp = 0, yp = 0;
	int x0 = 0, y0 = 0, x1 = 0,y1 = 0, tmp = 0, count = 5;

	if (ts->penStatus == PEN_DOWN) {
		while(count > 0) {
			count--;
			x0 = PenSPIXfer(X_LOCATION_CMD);
			x1 = PenSPIXfer(X_LOCATION_CMD);
			if (x0 == 0)
				continue;

			tmp = (x0 - x1) * (x0 - x1);
			if (tmp > 100)
				continue;

			y0 = PenSPIXfer(Y_LOCATION_CMD);
			y1 = PenSPIXfer(Y_LOCATION_CMD);
			if (y0 == 0)
				continue;

			tmp = (y0 - y1) * (y0 - y1);
			if (tmp > 100) {
				continue;
			} else {
				break;
			}
		}

		if (count > 0) {
			xp = (x0 + x1) >> 1;
			xp = 0x800 - xp;
			yp = (y0 + y1) >> 1;
		}
		printk("down xp = %d, yp = %d\n", xp,yp);

		input_report_abs(ts->input, ABS_X, xp);
		input_report_abs(ts->input, ABS_Y, yp);
		input_report_abs(ts->input, ABS_PRESSURE, 1);
		input_report_key(ts->input, BTN_TOUCH, 1);

		input_sync(ts->input);

	} else {
		if (ts->penStatus == PEN_UP) {
			input_report_key(ts->input, BTN_TOUCH, 0);
			input_report_abs(ts->input, ABS_PRESSURE, 0);
			input_sync(ts->input);
		}
	}
}
static void ts_timer_handler(unsigned long arg)
{
	int penval = 0;

//	printk("%s, %d\n.",__func__, __LINE__);	
	penval = sep0611_gpio_getpin(SEP0611_TOUCH_INT);

	if ( penval == 0) {			/* 如果是低电平，表示触摸屏仍然被按着 */
		if (ts->penStatus == PEN_UNSURE)
			ts->penStatus = PEN_DOWN;

		tsevent();				/* 读取触摸屏坐标 */
		ts->ts_timer.expires = jiffies + PEN_TIMER_DELAY_JUDGE;			/* 检测是否持续按键 */
		add_timer(&(ts->ts_timer));

		return;
	} else {
		del_timer_sync(&(ts->ts_timer));	//在定时器到期前禁止一个已注册的定时器
		if (ts->penStatus == PEN_DOWN)
			ts->penStatus = PEN_UP;

		tsevent();

//		SEP0611_INT_ENABLE(SEP0611_TOUCH_INTSRC);
		//zyt add open dect_timer
		ts->ts_timer.expires = jiffies + PEN_TIMER_DELAY_DETECT;
		add_timer(&(ts->detect_timer));
	}
}

/*
 *  触摸中断处理函数
 *  设定定时时间，添加定时器
 */
/**************comment by zyt**************
static irqreturn_t sep0611_ts_irqhandler(int irq, void *param)
{
	struct sep_ts *ts_data = (struct sep_ts *)param;

	//printk("Debug: %d, %s\n", __LINE__, __FUNCTION__);
	//ts_dbg("%s\n", __func__);

	SEP0611_INT_DISABLE(SEP0611_TOUCH_INTSRC);
	sep0611_gpio_clrirq(SEP0611_TOUCH_INT);		//清除中断

	ts->penStatus = PEN_UNSURE;

	ts_data->ts_timer.expires = jiffies + PEN_TIMER_DELAY_LONGTOUCH;
	add_timer(&ts->ts_timer);

	return IRQ_HANDLED;
}
**************comment by zyt**************/
static void detect_timer_handler(unsigned long arg)
{
	int penval = 0;
	
//	printk("%s, %d\n.",__func__, __LINE__);	

	del_timer_sync(&(ts->detect_timer));	//在定时器到期前禁止一个已注册的定时器

	penval = sep0611_gpio_getpin(SEP0611_TOUCH_INT);
	
	if (penval == 0)
	{
		ts->penStatus = PEN_UNSURE;
		ts->ts_timer.expires = jiffies + PEN_TIMER_DELAY_LONGTOUCH;
		add_timer(&ts->ts_timer);
	}else
	{
		ts->detect_timer.expires = jiffies + PEN_TIMER_DELAY_DETECT;
		add_timer(&ts->detect_timer);
	}
}
/*
 *  驱动注册函数
 */
static int __init sep0611_ts_probe(struct platform_device *pdev)
{
	struct input_dev *input_dev;
	int err;

	ts = kzalloc(sizeof(struct sep_ts), GFP_KERNEL);	/* 申请内存 */
	base = ioremap(SPI_BASE, SZ_4K);
	if (base == NULL) {
		printk("base is null\n");
	}

	input_dev = input_allocate_device();	/* 创建一个input设备  */

	if (!input_dev) {
		err = -ENOMEM;
		goto fail;
	}

	ts->input = input_dev;

	ts->input->evbit[0] = ts->input->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
	ts->input->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);
	//printk("keybit %x BIT_WORD(BTN_TOUCH) %x BIT_MASK(BTN_TOUCH) %x\n", ts->input->keybit[BIT_WORD(BTN_TOUCH)],BIT_WORD(BTN_TOUCH), BIT_MASK(BTN_TOUCH));

	input_set_abs_params(ts->input, ABS_X, MIN_X, MAX_X, 0, 0);	/* 设定ABS_X, ABS_Y, ABS_PRESSURE的绝对坐标值  */
	input_set_abs_params(ts->input, ABS_Y, MIN_Y, MAX_Y, 0, 0);
	input_set_abs_params(ts->input, ABS_PRESSURE, PEN_UP, PEN_DOWN, 0, 0);

	ts->input->name = "SEP0611 Touchscreen";
	ts->input->phys = "A/Fake/Path";
	ts->input->id.bustype = BUS_RS232;
	ts->input->id.vendor 	= 0xDEAD;
	ts->input->id.product = 0xBEEF;
	ts->input->id.version = SEP_TSVERSION;

	sep0611_ts_setup();

	init_timer(&ts->ts_timer);
	ts->ts_timer.function = ts_timer_handler;
	init_timer(&ts->detect_timer);
	ts->detect_timer.function = detect_timer_handler;

	ts->detect_timer.expires = jiffies + PEN_TIMER_DELAY_DETECT;
	add_timer(&ts->detect_timer);

//	if(request_irq(SEP0611_TOUCH_INTSRC, sep0611_ts_irqhandler, IRQF_ONESHOT, "sep0611_ts", ts)){	/* 申请i/o中断 */
//		printk("failed to request TouchScreen IRQ %d!\n", SEP0611_TOUCH_INTSRC);
//		kfree(ts);
//
//   		return -1;
//	}
	/* All went ok, so register to the input system */
	err = input_register_device(ts->input);
	if(err) {
//		free_irq(SEP0611_TOUCH_INTSRC, ts->input);
		return -EIO;
	}

	return 0;

fail:
	input_free_device(input_dev);
	kfree(ts);
	return err;
}

static int sep0611_ts_remove(struct platform_device *dev)
{
//	disable_irq(SEP0611_TOUCH_INTSRC);

//	free_irq(SEP0611_TOUCH_INTSRC, ts->input);
	del_timer(&ts->ts_timer);
	input_unregister_device(ts->input);

	return 0;
}


#ifdef CONFIG_PM
static int sep0611_ts_suspend(struct platform_device *dev, pm_message_t state)
{
	//disable_irq(SEP0611_TOUCH_INTSRC);
	return 0;
}

static int sep0611_ts_resume(struct platform_device *pdev)
{
	//del_timer(&ts->ts_timer);
	//setup_timer(&ts->ts_timer, ts_timer_handler, 0);

	//sep0611_ts_setup();

	return 0;
}
#else

#define sep0611_ts_suspend NULL
#define sep0611_ts_resume NULL

#endif

static struct platform_driver sep0611_ts_driver = {
	.probe   = sep0611_ts_probe,
	.remove  = sep0611_ts_remove,
 	.suspend = sep0611_ts_suspend,
 	.resume  = sep0611_ts_resume,
 	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "sep0611_ts",
	},
};

static char banner[] __initdata = KERN_INFO "SEP0611 Touchscreen driver v1.0, (c) 2010 SEU ASIC\n";

static int __init sep0611_ts_init(void)
{
	printk(banner);
	return platform_driver_register(&sep0611_ts_driver);
}

static void __exit sep0611_ts_exit(void)
{
	platform_driver_unregister(&sep0611_ts_driver);
}

module_init(sep0611_ts_init);
module_exit(sep0611_ts_exit);

MODULE_AUTHOR("John Zhang(xtayyt@gmail.com)");
MODULE_DESCRIPTION("sep0611 touchscreen driver");
MODULE_LICENSE("GPL");

