/* linux/arch/unicore/mach-sep0611/io.c
 *
 * Copyright (c) 2009-2011 SEUIC
 *
 * Southeast University ASIC SoC support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  09-04-2010	Changelog initial version
 *  05-31-2011	cgm Updated for release version
 *
 */

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/bootmem.h>
#include <linux/clk.h>

#include <asm/sizes.h>
#include <mach/hardware.h>

#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/mach/map.h>
#include <asm/io.h>

extern int sep0611_clock_init(void);

/*
 * This maps the generic sep0611 registers
 */
static struct map_desc sep0611_io_desc[] __initdata = {
	/*{
		.virtual	=   DRAM_BASE_V,
		.pfn		= __phys_to_pfn(DRAM_BASE),
		.length		=  (SZ_256M - SZ_88M),     //this will be fixed up
		.type		=  MT_MEMORY,
	},*/

	{	
		.virtual	=  ESRAM_BASE_V,
		.pfn		=__phys_to_pfn(ESRAM_BASE),
		.length		=  SZ_4M,     //this will be fixed up
		.type		=  MT_MEMORY,
	},
	 
 	{
 		.virtual	=  VIC_BASE_V,
		.pfn		= __phys_to_pfn(VIC_BASE) ,
		.length		=  SZ_4K ,
		.type		=  MT_DEVICE,
 	},
 	
 	{
 		.virtual	= PMU_BASE_V,
		.pfn		= __phys_to_pfn(PMU_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
 	},
 	
 	{	
 		.virtual	= RTC_BASE_V,
		.pfn		= __phys_to_pfn(RTC_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
 	},
 	
 	{	
 		.virtual	= TIMER_BASE_V,
		.pfn		= __phys_to_pfn(TIMER_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
 	},
 	
 	{	
 		.virtual	= PWM_BASE_V,
		.pfn		= __phys_to_pfn(PWM_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
 	},
 	
 	{	
 		.virtual	= UART0_BASE_V,
		.pfn		= __phys_to_pfn(UART0_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
	},
	
	{	
		.virtual	= UART1_BASE_V,
		.pfn		= __phys_to_pfn(UART1_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
	},
	
 	{	
 		.virtual	= UART2_BASE_V,
		.pfn		= __phys_to_pfn(UART2_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
 	}, 
 	
 	{	
 		.virtual	= UART3_BASE_V,
		.pfn		= __phys_to_pfn(UART3_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
 	},
 	
 	{	
 		.virtual	= GPIO_AO_BASE_V,
		.pfn		= __phys_to_pfn(GPIO_AO_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
 	},
 	
 	{	
 		.virtual	= GPIO_BASE_V,
		.pfn		= __phys_to_pfn(GPIO_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
 	},
 	
 	{	
 		.virtual	= DMAC1_BASE_V,
		.pfn		= __phys_to_pfn(DMAC1_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
 	},
 	
 	{	
 		.virtual	= DMAC2_BASE_V,
		.pfn		= __phys_to_pfn(DMAC2_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
 	},
 	
 	{	
 		.virtual	= SDIO1_BASE_V,
		.pfn		= __phys_to_pfn(SDIO1_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
 	},
 	
 	{	
 		.virtual	= SDIO2_BASE_V,
		.pfn		= __phys_to_pfn(SDIO2_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
	},
	
  	{	
  		.virtual	= NAND_BASE_V,
		.pfn		= __phys_to_pfn(NAND_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
 	},
 	
	{	
  		.virtual	= SYSCTRL_BASE_V,
		.pfn		= __phys_to_pfn(SYSCTRL_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
 	},
	
  	{	
  		.virtual	= DDR2CTL_BASE_V,
		.pfn		= __phys_to_pfn(DDR2CTL_BASE),
		.length		=  SZ_4K,
		.type		=  MT_DEVICE,
 	},

	{
		.virtual    = LCDC_BASE_V,
		.pfn        = __phys_to_pfn(LCDC_BASE),
		.length     =  SZ_4K,
		.type       =  MT_DEVICE,
	},
//--------------add by zyt--------------
//	IO BASE for VPU
	{
		.virtual	= VPU_BASE_V,
		.pfn		= __phys_to_pfn(VPU_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
//--------------add by zyt--------------
};

void __init sep0611_map_io(void)
{
	struct clk *clk;

	iotable_init(sep0611_io_desc, ARRAY_SIZE(sep0611_io_desc));
	sep0611_clock_init();
}

