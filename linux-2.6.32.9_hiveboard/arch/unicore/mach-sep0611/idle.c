#include <asm/atomic.h>
//#include <asm/mach-types.h>
#include <mach/hardware.h>
//#include <bsp.h>
#include <linux/kernel.h>
//#include <mach/typedef.h>
#include <linux/suspend.h>

#include <linux/dma-mapping.h>
//#include <mach/board.h>
#include <mach/devices.h>
//#include <mach/typedef.h>
//#include <mach/vic.h>   
//#include "sep_pm.h"
//volatile static PCKC pCKC = (volatile PCKC)tcc_p2v(HwCLK_BASE);
//volatile static unsigned int bCLK0CTRL = 0;
volatile unsigned int idle_expired;
//extern unsigned int g_org_tcc_clk; /* backup cpu clock value */

inline void sep0611_idle(void)
{
	extern int sep_pm_enter(suspend_state_t);
//	idle_expired = 1;
//	printk("%s\n",__func__);
	//bCLK0CTRL = pCKC->CLK0CTRL;
//	pCKC->CLK0CTRL |= (Hw20|0xFF00);
	
//	*(volatile unsigned long*)VIC_INT_MSK_ALL_V = 0x0; //int no mask all
//	*(volatile unsigned long*)VIC_INT_EN_L_V = 0xffffffff;
//	*(volatile unsigned long*)VIC_INT_EN_H_V = 0xffffffff;    //int enable all

//write bus5div 3
//	*(volatile unsigned long *)0x30001020 = 0xc0;
//	i = *(volatile unsigned long *)0x30001020;
//	printk("clkahb is 0x%x\n",i);
//	printk("i\n");

	
//	sep_pm_enter(PM_SUSPEND_STANDBY);
	writel(0x80000000, PMU_PMDR_V);

//	local_irq_enable();
//	while (idle_expired);
	//pCKC->CLK0CTRL = bCLK0CTRL;
//	local_irq_disable();
}

inline void arch_idle_off(void)
{
    //if (g_org_tcc_clk && (g_org_tcc_clk != pCKC->CLK0CTRL)) {
    //    pCKC->CLK0CTRL = g_org_tcc_clk;
    //}
	idle_expired = 0;
}

/* end of file */
