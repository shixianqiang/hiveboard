/*
 *kernel/driver/char/sep_pwm.c
 *init this version 
 *12-23-2011 zjw zhangjunwei166@163.com
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/gpio.h>
#include <mach/irqs.h>
#include <board/board.h>


/* QIHUAN PWM DEBUD SWITCH */
#define _QIHUAN_PWM_DEBUGE 1
/* TIMER CTRL */
#define TIMER_CTRL 0
#ifdef _QIHUAN_PWM_DEBUGE
	#define pwm_dbg(fmt, args...) printk(fmt, ##args)
#else
	#define pwm_dbg(fmt, args...)
#endif

#if TIMER_CTRL
#define TIMER_TXCR_MASK  0x0001
#define TIMER_TXCR_MASK_1  0xFFFE
#endif

#define STEPPER_RH_POLL_TIME  400
#define TIMER_T3LCR_INIT 0x7270E00 //500ms
#define TIMER_T3CR_INIT  0x06
#define PWM_MAJOR 255
#define PWM1_CONFIG 1
#define PWM2_CONFIG 3
#define PWM1_SHIFT 4
#define PWM2_SHIFT 5
#define LED_CTRL  6
#define DART_CTRL  7
#define LOW  0
#define HIGH 1
typedef unsigned long    U32;     /* unsigned 32-bit integer */

typedef	volatile U32 *	 RP;

#define    write_reg(reg, data) \
	       *(RP)(reg) = data
#define    read_reg(reg) \
	       *(RP)(reg)

static void __iomem *base;

#if TIMER_CTRL
static void __iomem *base1;

char motor1_advace_flag;
char motor1_retrate_flag;
char motor2_advace_flag;
char motor2_retrate_flag;
#endif

struct sep_pwm_dev{
	struct cdev cdev;
	struct class *pwm_class;

	struct timer_list stepper_timer;

	dev_t dev_num;	
};

static struct sep_pwm_dev *pwm_dev = NULL;
 
static int sep_pwm_open(struct inode *inode, struct file *file)
{
	//int data;	
	pwm_dbg("we are in sep_pwm_open before config\n");
/*	
	sep0611_gpio_cfgpin(SEP0611_GPE4,SEP0611_GPIO_IO);
	sep0611_gpio_cfgpin(SEP0611_GPE5,SEP0611_GPIO_IO);
	sep0611_gpio_dirpin(SEP0611_GPE4,SEP0611_GPIO_OUT);
	sep0611_gpio_dirpin(SEP0611_GPE5,SEP0611_GPIO_OUT);
*/
	write_reg(PWM_ENABLE_V,0x03);
/*	sep0611_gpio_cfgpin(SEP0611_GPE4,UART3_PWM0);
	sep0611_gpio_cfgpin(SEP0611_GPE5,UART3_PWM1);
	
	sep0611_gpio_dirpin(SEP0611_GPE4,SEP0611_GPIO_OUT);
	sep0611_gpio_dirpin(SEP0611_GPE5,SEP0611_GPIO_OUT);
	*/
	/*****add for motor shift**********
	 ******use PF_6 AND PF_8*********
	 **********************************/
/* 	sep0611_gpio_cfgpin(SEP0611_GPF6,SEP0611_GPIO_IO);
 	sep0611_gpio_cfgpin(SEP0611_GPF8,SEP0611_GPIO_IO);
 	sep0611_gpio_dirpin(SEP0611_GPF6,SEP0611_GPIO_OUT);
 	sep0611_gpio_dirpin(SEP0611_GPF8,SEP0611_GPIO_OUT);
	sep0611_gpio_setpin(SEP0611_GPF6,LOW);
	sep0611_gpio_setpin(SEP0611_GPF8,LOW);*/
	/***********************************
	 *****add for led on and off********/	
 /*	sep0611_gpio_cfgpin(SEP0611_GPI5,SEP0611_GPIO_IO);
 	sep0611_gpio_dirpin(SEP0611_GPI5,SEP0611_GPIO_OUT);
	sep0611_gpio_setpin(SEP0611_GPI5,LOW);
	sep0611_gpio_cfgpin(SEP0611_GPF10,SEP0611_GPIO_IO);
 	sep0611_gpio_dirpin(SEP0611_GPF10,SEP0611_GPIO_OUT);
	sep0611_gpio_setpin(SEP0611_GPF10,LOW);
*/
	write_reg(PWM1_CTRL_V,0x00);
	write_reg(PWM2_CTRL_V,0x00);
	write_reg(PWM1_DIV_V,0x960);//0xf0 240 0x960 2400
	write_reg(PWM2_DIV_V,0x960);
	write_reg(PWM1_PERIOD_V,0x3E8);//1000
	write_reg(PWM2_PERIOD_V,0x3E8);
	//write_reg(PWM1_DATA_V,0x1F4);//500
	//write_reg(PWM2_DATA_V,0x1F4);
	write_reg(PWM1_DATA_V,0x01);//500 
	write_reg(PWM2_DATA_V,0x01);//
	//write_reg(PWM_ENABLE_V,0x03);
	write_reg(PWM_ENABLE_V,0x00);//pengge
	return 0;
}

static int sep_pwm_release(struct inode *inode, struct file *file)
{
	pwm_dbg("in sep_pwm_release\n");	
	sep0611_gpio_cfgpin(SEP0611_GPE4,UART3_PWM0);
	sep0611_gpio_cfgpin(SEP0611_GPE5,UART3_PWM1);
	write_reg(PWM_ENABLE_V,0x00);
	write_reg(PWM1_DATA_V,0x01);
	write_reg(PWM2_DATA_V,0x01);
	sep0611_gpio_setpin(SEP0611_GPF6,LOW);
	sep0611_gpio_setpin(SEP0611_GPF8,LOW);
	return 0;
}

static int sep_pwm_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	int data;
	u32 DATA;
	//pwm_dbg("cmd is :%d\n",cmd);
	switch (cmd)
	{
		case PWM1_CONFIG:
			{
				#if TIMER_CTRL
				data = read_reg(TIMER_T3CR_V);//disable
        			data &= TIMER_TXCR_MASK_1;
        			write_reg(TIMER_T3CR_V,data);
        			write_reg(TIMER_T3LCR_V,TIMER_T3LCR_INIT);
				/*************enable timer***************/
				data = read_reg(TIMER_T3CR_V);//enable
        			data |= TIMER_TXCR_MASK;
        			write_reg(TIMER_T3CR_V,data);
				#endif
				mod_timer(&pwm_dev->stepper_timer,jiffies + msecs_to_jiffies(STEPPER_RH_POLL_TIME));

				if(get_user(data,(int __user *)arg))
					return -EFAULT;
				pwm_dbg("1data is :%d\n",data);
				DATA = (u32)data;
				if (DATA == 1)
				{
					write_reg(PWM_ENABLE_V,read_reg(PWM_ENABLE_V) & 0xFE);//disable 
				}
				else
				{
					write_reg(PWM_ENABLE_V,read_reg(PWM_ENABLE_V) | 0x01);//enable
					write_reg(PWM1_DATA_V,DATA);
				}
				break;
			}	
		case PWM2_CONFIG:
			{
				#if TIMER_CTRL
				data = read_reg(TIMER_T4CR_V);//disable
        			data &= TIMER_TXCR_MASK_1;
        			write_reg(TIMER_T4CR_V,data);
        			write_reg(TIMER_T4LCR_V,TIMER_T3LCR_INIT);
				/*************enable timer***************/
				data = read_reg(TIMER_T4CR_V);//enable
       				data |= TIMER_TXCR_MASK;
    				write_reg(TIMER_T4CR_V,data);
				#endif
				mod_timer(&pwm_dev->stepper_timer,jiffies + msecs_to_jiffies(STEPPER_RH_POLL_TIME));
				if(get_user(data,(int __user *)arg))
					return -EFAULT;
				pwm_dbg("2data is :%d\n",data);
				DATA = (u32)data;
				if (DATA == 1)
				{
					write_reg(PWM_ENABLE_V,read_reg(PWM_ENABLE_V) & 0xFD);//disable
				}
				else
				{
					write_reg(PWM_ENABLE_V,read_reg(PWM_ENABLE_V) | 0x02);//enable
					write_reg(PWM2_DATA_V,DATA);	
				}
				break;
			}
		case PWM1_SHIFT:
			{
				/* 0 mean retreat; 1 mean advance */
				if(get_user(data,(int __user *)arg))
					return -EFAULT;
				if (data == 0)//advance
				{	
					#if TIMER_CTRL
					motor1_advace_flag = 1;
					#endif
					pwm_dbg("*******motor 1 advance\n");
					sep0611_gpio_setpin(SEP0611_GPF8,HIGH);
				}
				else
				{
					pwm_dbg("*******motor 1 retreat\n");
					#if TIMER_CTRL
					motor1_retrate_flag = 1;
					#endif
					sep0611_gpio_setpin(SEP0611_GPF8,LOW);
				}
				break;
			}
		case PWM2_SHIFT:
			{
				if(get_user(data,(int __user *)arg))
					return -EFAULT;
				if (data == 0)//advance
				{
					#if TIMER_CTRL
					motor2_advace_flag = 1;
					#endif
					pwm_dbg("*******motor 2 advance\n");
					sep0611_gpio_setpin(SEP0611_GPF6,LOW);
				}
				else
				{
					#if TIMER_CTRL
					motor2_retrate_flag = 1;
					#endif
					pwm_dbg("*******motor 2 retreat\n");
					sep0611_gpio_setpin(SEP0611_GPF6,HIGH);
				}
				break;
			}
		case LED_CTRL:
			{
				if(get_user(data,(int __user *)arg))
					return -EFAULT;
				if (data == 0)//off
					sep0611_gpio_setpin(SEP0611_GPI5,LOW);
				else
					sep0611_gpio_setpin(SEP0611_GPI5,HIGH);
				break;
			}
		case DART_CTRL:
			{
				if(get_user(data,(int __user *)arg))
					return -EFAULT;
				if (data == 0)//off
					sep0611_gpio_setpin(SEP0611_GPF10,LOW);
				else
					sep0611_gpio_setpin(SEP0611_GPF10,HIGH);
				break;
			}
		default:
			return -EINVAL;
	}
	return 0;

}

static void stepper_timer(void)
{
		printk("%s\n",__func__);
        write_reg(PWM_ENABLE_V,read_reg(PWM_ENABLE_V) & 0xFE);//disable 
        sep0611_gpio_setpin(SEP0611_GPF8,LOW);
	    write_reg(PWM_ENABLE_V,read_reg(PWM_ENABLE_V) & 0xFD);//disable
	    sep0611_gpio_setpin(SEP0611_GPF6,LOW);
}
#if TIMER_CTRL
/***************************************************************************/
/**************************use timer3***************************************/
/***************************************************************************/
static irqreturn_t sep0611_timer2_irqhandler(int irq , void *param)
{
	int data;
	printk("%s\n",__func__);
        SEP0611_INT_DISABLE(INTSRC_TIMER2);
	if (motor1_advace_flag)
	{
        	read_reg(TIMER_T3ISCR_V);       //clear interrupt
		motor1_advace_flag = 0;
		//write_reg(PWM1_DATA_V,0x01);
		write_reg(PWM_ENABLE_V,read_reg(PWM_ENABLE_V) & 0xFE);//disable 
		sep0611_gpio_setpin(SEP0611_GPF8,LOW);
		/*************disable enable timer***************/
		data = read_reg(TIMER_T3CR_V);//disable
        	data &= TIMER_TXCR_MASK_1;
        	write_reg(TIMER_T3CR_V,data);
	}
	if (motor1_retrate_flag)
	{
        	read_reg(TIMER_T3ISCR_V);       //clear interrupt
		motor1_retrate_flag = 0;
		//write_reg(PWM1_DATA_V,0x01);
		write_reg(PWM_ENABLE_V,read_reg(PWM_ENABLE_V) & 0xFE);//disable 
		sep0611_gpio_setpin(SEP0611_GPF8,LOW);
		/*************disable enable timer***************/
		data = read_reg(TIMER_T3CR_V);//disable
        	data &= TIMER_TXCR_MASK_1;
        	write_reg(TIMER_T3CR_V,data);
	}
	if (motor2_advace_flag)
	{
        	read_reg(TIMER_T4ISCR_V);       //clear interrupt
		motor2_advace_flag = 0;
		//write_reg(PWM2_DATA_V,0x01);
		write_reg(PWM_ENABLE_V,read_reg(PWM_ENABLE_V) & 0xFD);//disable
		sep0611_gpio_setpin(SEP0611_GPF6,LOW);
		/*************disable enable timer***************/
		data = read_reg(TIMER_T4CR_V);//disable
        	data &= TIMER_TXCR_MASK_1;
        	write_reg(TIMER_T4CR_V,data);
	}
	if (motor2_retrate_flag)
	{
        	read_reg(TIMER_T4ISCR_V);       //clear interrupt
		motor2_retrate_flag = 0;
		//write_reg(PWM2_DATA_V,0x01);
		write_reg(PWM_ENABLE_V,read_reg(PWM_ENABLE_V) & 0xFD);//disable
		sep0611_gpio_setpin(SEP0611_GPF6,LOW);
		/*************disable enable timer***************/
		data = read_reg(TIMER_T4CR_V);//disable
        	data &= TIMER_TXCR_MASK_1;
        	write_reg(TIMER_T4CR_V,data);
	}
	
        SEP0611_INT_ENABLE(INTSRC_TIMER2);
	return IRQ_HANDLED;
}

static void sep_timer_setup(void)
{
        write_reg(TIMER_T3LCR_V,TIMER_T3LCR_INIT);
        write_reg(TIMER_T3CR_V,TIMER_T3CR_INIT);
        write_reg(TIMER_T4LCR_V,TIMER_T3LCR_INIT);
        write_reg(TIMER_T4CR_V,TIMER_T3CR_INIT);
}

#endif
static struct file_operations sep_pwm_fops={
	.owner = THIS_MODULE,
	.open = sep_pwm_open,
	.release = sep_pwm_release,
	.ioctl = sep_pwm_ioctl,
};

static int __init sep_pwm_init(void)
{
	int ret;

	pwm_dev = kmalloc(sizeof(struct sep_pwm_dev), GFP_KERNEL);
	if(pwm_dev == NULL){
		printk(KERN_ERR "Failed to allocate pwm device memory.\n");
		ret = -ENOMEM;
	}
	memset(pwm_dev,0,sizeof(struct sep_pwm_dev));
	pwm_dev->dev_num = MKDEV(PWM_MAJOR,0);

	ret = register_chrdev(PWM_MAJOR,"SEP_PWM",&sep_pwm_fops);
	if (ret<0)
	{
		printk("register sep_pwm failed\n");
		return ret;
	}
		
	/* create /dev/pwm */
	pwm_dev->pwm_class = class_create(THIS_MODULE, "SEP-PWM");
        device_create(pwm_dev->pwm_class, NULL, pwm_dev->dev_num, NULL, "pwm");

	base = ioremap(PWM_BASE,SZ_4K);
	if (base == NULL)
	{
		printk("base pwm_base is null\n");
	}

//	init_timer(&pwm_dev->stepper_timer);
//    pwm_dev->stepper_timer.function = stepper_timer;


	#if TIMER_CTRL
	SEP0611_INT_DISABLE(INTSRC_TIMER2);
        sep_timer_setup();
 
        if(request_irq(INTSRC_TIMER2, sep0611_timer2_irqhandler, IRQF_ONESHOT, "sep0611_timer2", pwm_dev)){
                printk("failed to request INTSRC_TIMER2 IRQ %d!\n",INTSRC_TIMER2 );
                return -1;
         }
	
  	 SEP0611_INT_ENABLE(INTSRC_TIMER2);
         base1 = ioremap(TIMER_BASE,SZ_4K);
         if (base1 == NULL)
         {
                printk("base timer_base is null\n");
         }
	#endif

//	sep0611_gpio_cfgpin(SEP0611_GPE4,SEP0611_GPIO_IO);
//	sep0611_gpio_cfgpin(SEP0611_GPE5,SEP0611_GPIO_IO);
//	sep0611_gpio_dirpin(SEP0611_GPE4,SEP0611_GPIO_OUT);
//	sep0611_gpio_dirpin(SEP0611_GPE5,SEP0611_GPIO_OUT);
//	sep0611_gpio_setpin(SEP0611_GPE4,LOW);
//	sep0611_gpio_setpin(SEP0611_GPE5,LOW);
//-----------------pengge-----------------
	 write_reg(PWM_ENABLE_V,0x02);
     write_reg(PWM2_CTRL_V,0x00);
     write_reg(PWM2_DIV_V,0x3fff);
     write_reg(PWM2_PERIOD_V,0x8);
	 write_reg(PWM2_DATA_V,0x1F4);
//----------------pengge----------------------
	printk("sep_pwm has registed\n");
	return ret;
}

static void __exit sep_pwm_exit(void)
{
	unregister_chrdev(PWM_MAJOR,"SEP_PWM");
	class_destroy(pwm_dev->pwm_class);

	printk("unregister sep_pwm\n");
}

module_init(sep_pwm_init);
module_exit(sep_pwm_exit);


MODULE_AUTHOR("zjw zhangjunwei166@163.com ");
MODULE_DESCRIPTION("sep0611 pwm driver");
MODULE_LICENSE("GPL");
