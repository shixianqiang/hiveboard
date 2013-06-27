/**************************************************************************************
*      File name   :  "sep_pm.c"
*      INITIAL VERSION:		2010.11
*	order code:		2011.1
****************************************************************************************/
#include <linux/mm.h>
#include <linux/rtc.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/suspend.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/moduleparam.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/tlbflush.h>
#include <asm/cacheflush.h>	// local_flush_tlb_all(), flush_cache_all();

#include <mach/irqs.h> 
#include <mach/param.h>
#include <mach/devices.h>  
#include <mach/hardware.h>

#include "sep_pm.h"

/*********** debug dbg***********/
#if 1
#define dbg printk
#else /* no debug */
#define dbg(x...) do {} while(0)
#endif
/**********************************/

#define SEP_SLEEP_KEY		1
#define	SEP_SUSPEND_KEY		0


extern void sep_mode(int mode);
extern void sleep_esram (void);
extern void move2esram(unsigned int *esram_iomem);
void state_to_normal(void);
extern void stop_esram (void);
static void SPIO_port_disable(void)
{
}

typedef struct _SLEEP_SAVE_REGS_{
	unsigned int cpu[1024] ;
	unsigned int timer_regs[0x55] ;
	unsigned int uart0_regs[0x7];
	unsigned int uart1_regs[0x7];
	unsigned int uart2_regs[0x7];
	unsigned int uart3_regs[0x7];
	unsigned int vic_regs[0x80];
	unsigned int lcdc_regs[0x34];
	unsigned int gpio_regs[0x4b];
	unsigned int sdio1_regs[0x40];
	unsigned int sdio2_regs[0x40];
}SLEEP_SAVE_REGS;

SLEEP_SAVE_REGS* save2DDR;
long vic_int_en_l_save, vic_int_en_h_save; //save vic_int_en

void store_regs(void)
{
	int i;
	dbg("-------%s\n",__func__);//2f0

	for (i = 0; i < 0x55; i++)
		save2DDR->timer_regs[i] = *(volatile unsigned int *)(TIMER_BASE_V + i*4);

	for (i = 0; i < 0x7; i++)
		save2DDR->uart0_regs[i] = *(volatile unsigned int *)(UART0_BASE_V + i*4);
	
	for (i = 0; i < 0x7; i++)
		save2DDR->uart1_regs[i] = *(volatile unsigned int *)(UART1_BASE_V + i*4);

	for (i = 0; i < 0x7; i++)
		save2DDR->uart2_regs[i] = *(volatile unsigned int *)(UART2_BASE_V + i*4);

	for (i = 0; i < 0x7; i++)
		save2DDR->uart3_regs[i] = *(volatile unsigned int *)(UART3_BASE_V + i*4);

	for (i = 0; i < 0x80; i++)
		save2DDR->vic_regs[i] = *(volatile unsigned int *)(VIC_BASE_V + i*4);

	for (i = 0; i < 0x34; i++)
		save2DDR->lcdc_regs[i] = *(volatile unsigned int *)(LCDC_BASE_V + i*4);

	for (i = 0; i < 0x4b; i++)
		save2DDR->gpio_regs[i] = *(volatile unsigned int *)(GPIO_BASE_V + i*4);

	for (i = 0; i < 0x40; i++)
		save2DDR->sdio1_regs[i] = *(volatile unsigned int *)(SDIO1_BASE_V + i*4);
	
	for (i = 0; i < 0x40; i++)
		save2DDR->sdio2_regs[i] = *(volatile unsigned int *)(SDIO2_BASE_V + i*4);

}

void restore_regs(void)
{
	int i;

	dbg("-------%s\n",__func__);
	
	for (i = 0; i < 0x55; i++)
		*(volatile unsigned int *)(TIMER_BASE_V + i*4) = save2DDR->timer_regs[i];

	for (i = 0; i < 0x7; i++)
		*(volatile unsigned int *)(UART0_BASE_V + i*4) = save2DDR->uart0_regs[i] ;

	for (i = 0; i < 0x7; i++)
		*(volatile unsigned int *)(UART1_BASE_V + i*4) = save2DDR->uart1_regs[i] ;

	for (i = 0; i < 0x7; i++)
		*(volatile unsigned int *)(UART2_BASE_V + i*4) = save2DDR->uart2_regs[i] ;

	for (i = 0; i < 0x7; i++)
		*(volatile unsigned int *)(UART3_BASE_V + i*4) = save2DDR->uart3_regs[i] ;

	for (i = 0; i < 0x80; i++)
		*(volatile unsigned int *)(VIC_BASE_V + i*4) = save2DDR->vic_regs[i] ;

	for (i = 0; i < 0x34; i++)
		*(volatile unsigned int *)(LCDC_BASE_V + i*4) = save2DDR->lcdc_regs[i] ;

	for (i = 0; i < 0x4b; i++)
		*(volatile unsigned int *)(GPIO_BASE_V + i*4) = save2DDR->gpio_regs[i] ;

	for (i = 0; i < 0x40; i++)
		*(volatile unsigned int *)(SDIO1_BASE_V + i*4) = save2DDR->sdio1_regs[i] ;

	for (i = 0; i < 0x40; i++)
		*(volatile unsigned int *)(SDIO2_BASE_V + i*4) = save2DDR->sdio2_regs[i] ;
}


void low_frequency(void)
{
//low frequency
//	*(volatile unsigned long *)PMU_MPLL_CFG_V 	= 0x00013c12;  //mpll 480MHz
//	*(volatile unsigned long *)PMU_PLL_SET_V 	= 0x00000002;  //set mpll 480MHz
//	*(volatile unsigned long *)PMU_AHB_CLK_CFG_V 	= 0x000000C1;  //bus5 80M
//	*(volatile unsigned long *)PMU_DIV_SET_V 	= 0x0000001f;  //set bus5 80MHz
//change baudrate
	*(volatile unsigned long *)UART0_LCR_V  = 0x83;
	*(volatile unsigned long *)UART0_DLBH_V = 0;
	*(volatile unsigned long *)UART0_DLBL_V = 0x20;
	*(volatile unsigned long *)UART0_LCR_V  = 0x03;
}

void suspend_mode_on(void)
{
	dbg("Enter into %s\n",__func__);

	store_regs();
	dbg("%p\n",ESRAM_BASE_V);
//	if( *(volatile unsigned long *)PMU_MPLL_CFG_V != 0x00013c12 )  
//		low_frequency();

	write_reg(PMU_MSK_WAKEUP_V, 0xf);
dbg("%p\n",read_reg(DDR2_DCR_V));

//disable the irq
	__asm__(
    		"mov r0, asr		\n"
       	"or      r0, r0, #0x80	\n"
       	"mov.a   asr, r0	\n"
		);

#ifdef CLOSE_VCC_IO
       *(volatile unsigned int*)(AO_PORT_SEL_V) |= (0x3);
       *(volatile unsigned int*)(AO_PORT_DIR_V) &= ~(0x3);
       *(volatile unsigned int*)(AO_PORT_DATA_V) &= ~(0x3);
#endif

	local_flush_tlb_all();
	flush_cache_all();

	move2esram(ESRAM_BASE_V);

// store pc and move to  easram
printk("%s,%d,%s\n",__FILE__,__LINE__,__func__);
	__asm__(
		"mov r0, pc			\n"
		"add r0, r0, #0x10		\n"
		"ldw r1, =0xF4001098		\n"
		"stw r0, [r1+]			\n"
		"mov r0, #0xfc000000		\n"
		"mov pc, r0			\n"
		);

	state_to_normal();
//PMU_CLR_WAKEUP_V
	write_reg(PMU_CLR_WAKEUP_V, 1);
	while(!(read_reg(PMU_INT_V) & 0x2))
	{
	write_reg(PMU_CLR_WAKEUP_V, 1);
	}
	write_reg(PMU_CLR_WAKEUP_V, 0);
	dbg("-------We are back\r\n");

	restore_regs();
}

/*
* copy function sleep_esram or stop_esram to ESRAM
*/
static void copy_func_to_sram(int mode)
{
	volatile unsigned int *fPtr, *p;
	int i;

	dbg("enter %s.\n",__func__);
	
	if(mode == SEP_SLEEP_KEY)
	{	
		fPtr = (volatile unsigned int *)sleep_esram;
		p = (volatile unsigned int *)ESRAM_BASE_V;

		for (i = 0; i < 0x400; i++) 
		{
			*(p++) = *(fPtr++);
		}

		for(i=0;i<0x400;i++)
		{
			;
		}
		suspend_mode_on();
	}
	else if(mode == SEP_SUSPEND_KEY)
	{
		//empty now
		fPtr = (volatile unsigned int *)stop_esram;
		p = (volatile unsigned int *)ESRAM_BASE_V;

		for (i = 0; i < 0x400; i++) 
		{
			*(p++) = *(fPtr++);
		}

		for(i=0;i<0x400;i++)
		{
			;
		}
		suspend_mode_on();
	}
	dbg("leave %s.\n",__func__);
}

void sleep_esram (void)
{
	int i;
	write_reg(DDR2_DCR_V, ((read_reg(DDR2_DCR_V)&0x07ffffff)|0x90000000));
	while (!(read_reg(DDR2_DCR_V)&0x80000000));

//DDR retention
	write_reg(DDRPHY_RETENTION_V,1);
	while(!(read_reg(DDRPHY_RETENTION_V)&1));
	for(i=0;i<=100000;i++)
	{	
		;
	}
//write pmu: HW sleep mode
	write_reg(PMU_PMDR_V, (*(volatile unsigned int*)PMU_PMDR_V)|0x00000002);
	for(i=0;i<=100000;i++)
	{	
		;
	}
	write_reg(PMU_PMDR_V, (*(volatile unsigned int*)PMU_PMDR_V)|0x80000000);
}

void state_to_normal(void)
{
	int i=0;
	printk("enter %s\n",__func__);

	write_reg(DDRPHY_RETENTION_V, 2);
 	while(!(read_reg(DDRPHY_RETENTION_V) & 2));
	write_reg(DDR2_DCR_V, ((read_reg(DDR2_DCR_V)&0x07ffffff)|0xb8000000));
	while (!(read_reg(DDR2_DCR_V)&0x80000000));
	
//write pmu: HW sleep mode
	write_reg(PMU_PMDR_V, 0);
}

void stop_esram (void)
{
//	dbg("before ddr self-refresh\n");
	write_reg(DDR2_DCR_V, ((read_reg(DDR2_DCR_V)&0x07ffffff)|0x90000000));
	while (!(read_reg(DDR2_DCR_V)&0x80000000));

//write pmu: HW sleep mode
	write_reg(PMU_PMDR_V, (*(volatile unsigned int*)PMU_PMDR_V)|0x80000001);

	__asm__(
	"nop				\n"
	"nop				\n"
	"nop				\n"
	"nop				\n"
	"nop				\n"
	"nop				\n"
	"nop				\n"
	);
	
	write_reg(DDR2_DCR_V,0xb90001b4);//access DRR  
	while (!(read_reg(DDR2_DCR_V)&0x80000000));
	
	__asm__(
	"ldw r0,=0x30001098		\n"
	"ldw r1,[r0]			\n"
	"add r1,r1,#0x8			\n"
	"mov pc,r1			\n"
	"nop				\n"
	"nop				\n"
	"nop				\n"
	"nop				\n"
	"nop				\n"
	"nop				\n"
	"nop				\n"
	);
}


void sep_mode(int mode)
{
	if(mode == SEP_SLEEP_KEY)
	{
		write_reg(PMU_SLEEP_FLAG_V, (*(volatile unsigned int*)PMU_SLEEP_FLAG_V)|0x1);//decide normal boot or wakeup in booting code
		copy_func_to_sram(SEP_SLEEP_KEY);	
	}	
	else if(mode == SEP_SUSPEND_KEY)
	{
		copy_func_to_sram(SEP_SUSPEND_KEY);	
	}
}
EXPORT_SYMBOL(sep_mode);

int sep_pm_enter(suspend_state_t state)
{
	dbg("Enter into %s\n",__func__);

	if (state != PM_SUSPEND_MEM) 
	{
		return -EINVAL;
	}

	SPIO_port_disable();

	if(state== PM_SUSPEND_MEM)
	{
		sep_mode(SEP_SLEEP_KEY);		
	}	
	else if(state == PM_SUSPEND_STANDBY)
	{
		sep_mode(SEP_SUSPEND_KEY);	
	}	
	
	dbg("Get out of %s\n",__func__);
	return 0;
}
EXPORT_SYSBOL(sep_pm_enter);


static int sep_pm_begin(suspend_state_t state)
{
	dbg("%s\n",__func__);

	save2DDR =  ioremap_nocache(0x40f05000, PAGE_ALIGN(sizeof(SLEEP_SAVE_REGS)));

	return 0;
}


static int sep_pm_prepare(void)
{
	dbg("%s\n",__func__);
	
	vic_int_en_l_save = *(volatile unsigned long*)VIC_INT_EN_L_V; 
	vic_int_en_h_save = *(volatile unsigned long*)VIC_INT_EN_H_V;
	
	return 0;
}

static void sep_pm_wake(void)
{
	printk("%s\n",__func__);
	
	*(volatile unsigned long*)VIC_INT_EN_L_V = vic_int_en_l_save;
	*(volatile unsigned long*)VIC_INT_EN_H_V = vic_int_en_h_save;
}

static void sep_pm_finish(void)
{
	dbg("%s\n",__func__);

	if(save2DDR != NULL)
	{
		iounmap(save2DDR);
		save2DDR = NULL;
	}
}


static void sep_pm_recover(void)
{
	dbg("%s\n",__func__);
}

static void sep_pm_end(void)
{
	dbg("%s\n",__func__);

	dbg("pmu state is 0x%x\n",(*(volatile unsigned int*)PMU_PMDR_V));
}

static int sep_pm_valid(suspend_state_t state)//
{
	dbg("%s\n",__func__);
	
	return state == PM_SUSPEND_MEM;
}

/************************************************************************************************
* FUNCTION	: plat_sep_reboot
* DESCRIPTION	: system reboot. 
************************************************************************************************/
void sep0611_reboot(void)
{
	printk("SEP0611:software reset");
	*(volatile unsigned long*)PMU_RTCR_V = 0x1;	//write 1 to trigger software interrupt
}
EXPORT_SYMBOL(sep0611_reboot);

void sep0611_poweroff(void)
{
	*(volatile unsigned long *)AO_PORT_SEL_V |= 0x1 << 2;	//AP2 通用
	*(volatile unsigned long *)AO_PORT_DIR_V &= ~(0x1 << 2);//AP2 输出
	*(volatile unsigned long *)AO_PORT_DATA_V &= ~(0x1 << 2);//AP2 输出0	
}


static struct platform_suspend_ops sep_pm_ops = 
{
	.valid		= sep_pm_valid,
	.begin		= sep_pm_begin,
	.prepare   	= sep_pm_prepare,	
	.enter		= sep_pm_enter,
	.wake		= sep_pm_wake,
	.finish     = sep_pm_finish,
	.end	    = sep_pm_end,
	.recover   	= sep_pm_recover,
};

static int __init sep_pm_init(void)
{
	dbg("%s\n",__func__);
	suspend_set_ops(&sep_pm_ops);
	return 0;
}

__initcall(sep_pm_init);

