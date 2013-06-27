/****************************************************
 *	lcd backlight control driver for terminal board
 *
 *			Author 	: John Zhang (xtayyt@gmail.com)
 *			Date	: 2012-09-10
 ****************************************************/
#include <linux/miscdevice.h>
#include <mach/gpio.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <board/board.h>
#include <linux/delay.h>
#include "sep0611_fb.h"

#define DEVICE_NAME "lcd-backlight"
#define DEFAULT_BACKLIGHT_BRIGHTNESS 0x2710
#define MIN_BACKLIGHT_BRIGHTNESS 0x0000
#define MAX_BACKLIGHT_BRIGHTNESS 0x2710
#define SEP0611_BL_OPEN 	0x10
#define SEP0611_BL_CLOSE 	0x11
#define SEP0611_BL_SET 		0x12

void sep0611_pwm1_set(unsigned long value)
{
	*(volatile unsigned long*)PWM1_PERIOD_V	= 0x2710;
	if(value > 10000)
	{
		printk(KERN_INFO"lcd brightness value = %lu,which should be 0 ~ 10000",value);
		*(volatile unsigned long*)PWM1_DATA_V	= MAX_BACKLIGHT_BRIGHTNESS;
	}
	else
	{	
        *(volatile unsigned long*)PWM1_DATA_V	= value;
	}
}

void sep0611_pwm1_init(void)
{
	sep0611_gpio_cfgpin(SEP0611_LCD_BL_CTRL, SEP0611_GPIO_SFN);
	sep0611_gpio_dirpin(SEP0611_LCD_BL_CTRL, SEP0611_GPIO_OUT);
	//set pwm1
	*(volatile unsigned long*)PWM_ENABLE_V	|= 0x1;		//enable pwm1	
	udelay(100);
	*(volatile unsigned long*)PWM1_CTRL_V  &= (~0x3);	//set pwm mode
	*(volatile unsigned long*)PWM1_DIV_V 	= 0xA;		//set pre-scaler 10
	*(volatile unsigned long*)PWM1_PERIOD_V	= 0x2710;	//set period 10000,so the frequency is 600Hz
	//set value
	sep0611_pwm1_set(DEFAULT_BACKLIGHT_BRIGHTNESS);
	*(volatile unsigned long*)PWM_INTMASK_V |= 0x1;		//mask pwm1 irq
	*(volatile unsigned long*)PWM_INT_V		|= 0x1;		//clr pwm1 irq
}

void sep0611_pwm1_exit(void)
{
	*(volatile unsigned long*)PWM_ENABLE_V	&= (~0x2);	//disable pwm1
}

static long sep0611_bl_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
	case SEP0611_BL_OPEN:
		sep0611_enable_backlight(1);
		return 0;
	case SEP0611_BL_CLOSE:
		sep0611_enable_backlight(0);
		return 0;
	case SEP0611_BL_SET:
		sep0611_pwm1_set(arg);
		return 0;
	default:
		return -EINVAL;
	}
}

static struct file_operations sep0611_bl_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl	= sep0611_bl_ioctl,
};

static struct miscdevice sep0611_backlight = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= DEVICE_NAME,
	.fops	= &sep0611_bl_fops,
};

static int __init sep0611_bl_init(void)
{
	int ret;

	sep0611_pwm1_init();	

	ret = misc_register(&sep0611_backlight);
	
	printk(DEVICE_NAME" initialized\n");
	
	return ret;
}

static void __exit sep0611_bl_exit(void)
{
	misc_deregister(&sep0611_backlight);
}

module_init(sep0611_bl_init);
module_exit(sep0611_bl_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("John Zhang, Prochip Inc.");
