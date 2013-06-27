/* linux/arch/unicore/mach-sep0611/include/mach/hardware.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *
 * Southeast University ASIC SoC support
 *
 * This file contains the hardware definitions of the SEP0611.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

/*
I/O空间:
         物理地址       虚拟地址
sep0611: 0xB0000000    0xF4000000

MEM空间:
sep0611: 0x40000000    0xC0000000
*/


/*
 *  各模块寄存器基值
 */

/* ********************************物理地址************************************** */
// component base address define
// the base address of the memory

#define    NOR_MEM_BASE             0x20000000
#define    NOR_MEM_REMAP_ADDR       0x00000000
#define    ESRAM_BASE               0xB2000000
#define    ESRAM_REMAP_ADDR         0x00000000
#define    DRAM_BASE                0x40000000
#define    DRAM_REMAP_ADDR          0x00000000

// the module base address of bus1-ahb64 and bus4-apb32
#define    VIC_BASE                 0xB0000000
#define    PMU_BASE                 0xB0001000
#define    RTC_BASE                 0xB0002000
#define    TIMER_BASE               0xB0003000
#define    PWM_BASE                 0xB0004000
#define    GPIO_AO_BASE             0xB0005000
#define    GPIO_BASE                0xB0006000
#define    GPS_BASE                 0xB0007000
#define    SYSCTRL_BASE             0xB0008000

// the module base address of bus3-ahb32 and bus5-apb32
#define    UART_BASE                0xB1000000
#define    UART0_BASE               0xB1000000
#define    UART1_BASE               0xB1001000
#define    UART2_BASE               0xB1002000
#define    UART3_BASE               0xB1003000
#define    SSI_BASE                 0xB1004000
#define    SSI1_BASE                0xB1004000
#define    SSI2_BASE                0xB1005000
#define    SSI3_BASE                0xB1006000
#define    I2C_BASE                 0xB1007000
#define    I2C1_BASE                0xB1007000
#define    I2C2_BASE                0xB1008000
#define    I2C3_BASE                0xB1009000
#define    I2S_BASE                 0xB100A000
#define    USB_BASE                 0xB1010000
#define    DMAC2_BASE               0xB1011000
#define    GPU_BASE                 0xB1013000

// the module base address of bus2-ahb32
#define    NORCTL_BASE              0xB2020000
#define    NAND_BASE                0xB2021000
#define    SDIO_BASE                0xB2022000
#define    SDIO1_BASE               0xB2022000
#define    SDIO2_BASE               0xB2023000
#define    LCDC_BASE                0xB2025000
#define    VPU_BASE                 0xB2026000
#define    DMAC1_BASE               0xB2027000
#define    DDR2CTL_BASE             0xB3000000

/* *******************************虚拟地址********************************* */

#define	   SEP_IO_BASE_V			0xF4000000

//#define    ESRAM_BASE_V          	(SEP_IO_BASE_V + 0x2000000)
#define    ESRAM_BASE_V          	0xFC000000//zhangyue
#define    DRAM_BASE_V         		0xC0000000

// the module base address of bus1-ahb64 and bus4-apb32
#define    VIC_BASE_V             	(SEP_IO_BASE_V + 0x0000000)
#define    PMU_BASE_V              	(SEP_IO_BASE_V + 0x0001000)
#define    RTC_BASE_V             	(SEP_IO_BASE_V + 0x0002000)
#define    TIMER_BASE_V           	(SEP_IO_BASE_V + 0x0003000)
#define    PWM_BASE_V             	(SEP_IO_BASE_V + 0x0004000)
#define    GPIO_AO_BASE_V           (SEP_IO_BASE_V + 0x0005000)
#define    GPIO_BASE_V        		(SEP_IO_BASE_V + 0x0006000)
#define    SYSCTRL_BASE_V           (SEP_IO_BASE_V + 0x0008000)

// the module base address of bus3-ahb32 and bus5-apb32
#define    UART_BASE_V             	(SEP_IO_BASE_V + 0x1000000)
#define    UART0_BASE_V           	(SEP_IO_BASE_V + 0x1000000)
#define    UART1_BASE_V           	(SEP_IO_BASE_V + 0x1001000)
#define    UART2_BASE_V            	(SEP_IO_BASE_V + 0x1002000)
#define    UART3_BASE_V          	(SEP_IO_BASE_V + 0x1003000)
#define    SSI_BASE_V              	(SEP_IO_BASE_V + 0x1004000)
#define    SSI1_BASE_V             	(SEP_IO_BASE_V + 0x1004000)
#define    SSI2_BASE_V           	(SEP_IO_BASE_V + 0x1005000)
#define    SSI3_BASE_V            	(SEP_IO_BASE_V + 0x1006000)
#define    I2C_BASE_V             	(SEP_IO_BASE_V + 0x1007000)
#define    I2C1_BASE_V           	(SEP_IO_BASE_V + 0x1007000)
#define    I2C2_BASE_V             	(SEP_IO_BASE_V + 0x1008000)
#define    I2C3_BASE_V            	(SEP_IO_BASE_V + 0x1009000)
#define    I2S_BASE_V            	(SEP_IO_BASE_V + 0x100A000)
#define    USB_BASE_V              	(SEP_IO_BASE_V + 0x1010000)
#define    DMAC2_BASE_V           	(SEP_IO_BASE_V + 0x1011000)
#define    GPU_BASE_V             	(SEP_IO_BASE_V + 0x1013000)

// the module base address of bus2-ahb32
#define    NORCTL_BASE_V         	(SEP_IO_BASE_V + 0x2020000)
#define    NAND_BASE_V           	(SEP_IO_BASE_V + 0x2021000)
#define    SDIO_BASE_V           	(SEP_IO_BASE_V + 0x2022000)
#define    SDIO1_BASE_V           	(SEP_IO_BASE_V + 0x2022000)
#define    SDIO2_BASE_V           	(SEP_IO_BASE_V + 0x2023000)
#define    LCDC_BASE_V          	(SEP_IO_BASE_V + 0x2025000)
#define    VPU_BASE_V            	(SEP_IO_BASE_V + 0x2026000)
#define    DMAC1_BASE_V           	(SEP_IO_BASE_V + 0x2027000)
#define    DDR2CTL_BASE_V         	(SEP_IO_BASE_V + 0x3000000)


//后续为每个模块的地址，首先为物理地址，其次为虚拟地址

// VIC register list/*INT CONTROL PHY BASE*/
// VIC_BASE 0xB0000000
#define    VIC_INT_SLT_L            (VIC_BASE + 0x000)
#define    VIC_INT_SLT_H            (VIC_BASE + 0x004)
#define    VIC_INT_EN_L             (VIC_BASE + 0x008)
#define    VIC_INT_EN_H             (VIC_BASE + 0x00C)
#define    VIC_INT_EN_CLR_L         (VIC_BASE + 0x010)
#define    VIC_INT_EN_CLR_H         (VIC_BASE + 0x014)
#define    VIC_SFT_INT_L            (VIC_BASE + 0x018)
#define    VIC_SFT_INT_H            (VIC_BASE + 0x01C)
#define    VIC_SFT_INT_CLR_L        (VIC_BASE + 0x020)
#define    VIC_SFT_INT_CLR_H        (VIC_BASE + 0x024)
#define    VIC_INT_MSK_ALL          (VIC_BASE + 0x028)
#define    VIC_RAW_INT_SRC_L        (VIC_BASE + 0x030)
#define    VIC_RAW_INT_SRC_H        (VIC_BASE + 0x034)
#define    VIC_RAW_IRQ_STS_L        (VIC_BASE + 0x038)
#define    VIC_RAW_IRQ_STS_H        (VIC_BASE + 0x03C)
#define    VIC_RAW_FIQ_STS_L        (VIC_BASE + 0x040)
#define    VIC_RAW_FIQ_STS_H        (VIC_BASE + 0x044)
#define    VIC_MSK_IRQ_STS_L        (VIC_BASE + 0x048)
#define    VIC_MSK_IRQ_STS_H        (VIC_BASE + 0x04C)
#define    VIC_MSK_FIQ_STS_L        (VIC_BASE + 0x050)
#define    VIC_MSK_FIQ_STS_H        (VIC_BASE + 0x054)
#define    VIC_IRQ_PENDING_L        (VIC_BASE + 0x058)
#define    VIC_IRQ_PENDING_H        (VIC_BASE + 0x05C)
#define    VIC_FIQ_PENDING_L        (VIC_BASE + 0x060)
#define    VIC_FIQ_PENDING_H        (VIC_BASE + 0x064)
#define    VIC_IRQ_VECTOR_BASE      (VIC_BASE + 0x070)
#define    VIC_FIQ_VECTOR_BASE      (VIC_BASE + 0x074)
#define    VIC_IRQ_VECTOR_NUM       (VIC_BASE + 0x078)
#define    VIC_FIQ_VECTOR_NUM       (VIC_BASE + 0x07C)
#define    VIC_IRQ_VECTOR_ADDR 		(VIC_BASE + 0x080)
#define    VIC_FIQ_VECTOR_ADDR      (VIC_BASE + 0x084)
#define    VIC_PROIRTY_MASK         (VIC_BASE + 0x090)
#define    VIC_VECTOR_PROIRTY00     (VIC_BASE + 0x100)
#define    VIC_VECTOR_PROIRTY01     (VIC_BASE + 0x104)
#define    VIC_VECTOR_PROIRTY02     (VIC_BASE + 0x108)
#define    VIC_VECTOR_PROIRTY03     (VIC_BASE + 0x10C)
#define    VIC_VECTOR_PROIRTY04     (VIC_BASE + 0x110)
#define    VIC_VECTOR_PROIRTY05     (VIC_BASE + 0x114)
#define    VIC_VECTOR_PROIRTY06     (VIC_BASE + 0x118)
#define    VIC_VECTOR_PROIRTY07     (VIC_BASE + 0x11C)
#define    VIC_VECTOR_PROIRTY08     (VIC_BASE + 0x120)
#define    VIC_VECTOR_PROIRTY09     (VIC_BASE + 0x124)
#define    VIC_VECTOR_PROIRTY10     (VIC_BASE + 0x128)
#define    VIC_VECTOR_PROIRTY11     (VIC_BASE + 0x12C)
#define    VIC_VECTOR_PROIRTY12     (VIC_BASE + 0x130)
#define    VIC_VECTOR_PROIRTY13     (VIC_BASE + 0x134)
#define    VIC_VECTOR_PROIRTY14     (VIC_BASE + 0x138)
#define    VIC_VECTOR_PROIRTY15     (VIC_BASE + 0x13C)
#define    VIC_VECTOR_PROIRTY16     (VIC_BASE + 0x140)
#define    VIC_VECTOR_PROIRTY17     (VIC_BASE + 0x144)
#define    VIC_VECTOR_PROIRTY18     (VIC_BASE + 0x148)
#define    VIC_VECTOR_PROIRTY19     (VIC_BASE + 0x14C)
#define    VIC_VECTOR_PROIRTY20     (VIC_BASE + 0x150)
#define    VIC_VECTOR_PROIRTY21     (VIC_BASE + 0x154)
#define    VIC_VECTOR_PROIRTY22     (VIC_BASE + 0x158)
#define    VIC_VECTOR_PROIRTY23     (VIC_BASE + 0x15C)
#define    VIC_VECTOR_PROIRTY24     (VIC_BASE + 0x160)
#define    VIC_VECTOR_PROIRTY25     (VIC_BASE + 0x164)
#define    VIC_VECTOR_PROIRTY26     (VIC_BASE + 0x168)
#define    VIC_VECTOR_PROIRTY27     (VIC_BASE + 0x16C)
#define    VIC_VECTOR_PROIRTY28     (VIC_BASE + 0x170)
#define    VIC_VECTOR_PROIRTY29     (VIC_BASE + 0x174)
#define    VIC_VECTOR_PROIRTY30     (VIC_BASE + 0x178)
#define    VIC_VECTOR_PROIRTY31     (VIC_BASE + 0x17C)
#define    VIC_VECTOR_PROIRTY32     (VIC_BASE + 0x180)
#define    VIC_VECTOR_PROIRTY33     (VIC_BASE + 0x184)
#define    VIC_VECTOR_PROIRTY34     (VIC_BASE + 0x188)
#define    VIC_VECTOR_PROIRTY35     (VIC_BASE + 0x18C)
#define    VIC_VECTOR_PROIRTY36     (VIC_BASE + 0x190)
#define    VIC_VECTOR_PROIRTY37     (VIC_BASE + 0x194)
#define    VIC_VECTOR_PROIRTY38     (VIC_BASE + 0x198)
#define    VIC_VECTOR_PROIRTY39     (VIC_BASE + 0x19C)
#define    VIC_VECTOR_PROIRTY40     (VIC_BASE + 0x1A0)
#define    VIC_VECTOR_PROIRTY41     (VIC_BASE + 0x1A4)
#define    VIC_VECTOR_PROIRTY42     (VIC_BASE + 0x1A8)
#define    VIC_VECTOR_PROIRTY43     (VIC_BASE + 0x1AC)
#define    VIC_VECTOR_PROIRTY44     (VIC_BASE + 0x1B0)
#define    VIC_VECTOR_PROIRTY45     (VIC_BASE + 0x1B4)
#define    VIC_VECTOR_PROIRTY46     (VIC_BASE + 0x1B8)
#define    VIC_VECTOR_PROIRTY47     (VIC_BASE + 0x1BC)
#define    VIC_VECTOR_PROIRTY48     (VIC_BASE + 0x1C0)
#define    VIC_VECTOR_PROIRTY49     (VIC_BASE + 0x1C4)
#define    VIC_VECTOR_PROIRTY50     (VIC_BASE + 0x1C8)
#define    VIC_VECTOR_PROIRTY51     (VIC_BASE + 0x1CC)
#define    VIC_VECTOR_PROIRTY52     (VIC_BASE + 0x1D0)
#define    VIC_VECTOR_PROIRTY53     (VIC_BASE + 0x1D4)
#define    VIC_VECTOR_PROIRTY54     (VIC_BASE + 0x1D8)
#define    VIC_VECTOR_PROIRTY55     (VIC_BASE + 0x1DC)
#define    VIC_VECTOR_PROIRTY56     (VIC_BASE + 0x1E0)
#define    VIC_VECTOR_PROIRTY57     (VIC_BASE + 0x1E4)
#define    VIC_VECTOR_PROIRTY58     (VIC_BASE + 0x1E8)
#define    VIC_VECTOR_PROIRTY59     (VIC_BASE + 0x1EC)
#define    VIC_VECTOR_PROIRTY60     (VIC_BASE + 0x1F0)
#define    VIC_VECTOR_PROIRTY61     (VIC_BASE + 0x1F4)
#define    VIC_VECTOR_PROIRTY62     (VIC_BASE + 0x1F8)
#define    VIC_VECTOR_PROIRTY63     (VIC_BASE + 0x1FC)

// VIC register list/*INT CONTROL VIRTUAL ADDRESS*/

// VIC_BASE_V 0x30000000
#define    VIC_INT_SLT_L_V         	(VIC_BASE_V + 0x000)
#define    VIC_INT_SLT_H_V         	(VIC_BASE_V + 0x004)
#define    VIC_INT_EN_L_V         	(VIC_BASE_V + 0x008)
#define    VIC_INT_EN_H_V          	(VIC_BASE_V + 0x00C)
#define    VIC_INT_EN_CLR_L_V     	(VIC_BASE_V + 0x010)
#define    VIC_INT_EN_CLR_H_V       (VIC_BASE_V + 0x014)
#define    VIC_SFT_INT_L_V          (VIC_BASE_V + 0x018)
#define    VIC_SFT_INT_H_V          (VIC_BASE_V + 0x01C)
#define    VIC_SFT_INT_CLR_L_V      (VIC_BASE_V + 0x020)
#define    VIC_SFT_INT_CLR_H_V      (VIC_BASE_V + 0x024)
#define    VIC_INT_MSK_ALL_V        (VIC_BASE_V + 0x028)
#define    VIC_RAW_INT_SRC_L_V      (VIC_BASE_V + 0x030)
#define    VIC_RAW_INT_SRC_H_V      (VIC_BASE_V + 0x034)
#define    VIC_RAW_IRQ_STS_L_V      (VIC_BASE_V + 0x038)
#define    VIC_RAW_IRQ_STS_H_V      (VIC_BASE_V + 0x03C)
#define    VIC_RAW_FIQ_STS_L_V      (VIC_BASE_V + 0x040)
#define    VIC_RAW_FIQ_STS_H_V      (VIC_BASE_V + 0x044)
#define    VIC_MSK_IRQ_STS_L_V      (VIC_BASE_V + 0x048)
#define    VIC_MSK_IRQ_STS_H_V      (VIC_BASE_V + 0x04C)
#define    VIC_MSK_FIQ_STS_L_V      (VIC_BASE_V + 0x050)
#define    VIC_MSK_FIQ_STS_H_V      (VIC_BASE_V + 0x054)
#define    VIC_IRQ_PENDING_L_V      (VIC_BASE_V + 0x058)
#define    VIC_IRQ_PENDING_H_V      (VIC_BASE_V + 0x05C)
#define    VIC_FIQ_PENDING_L_V      (VIC_BASE_V + 0x060)
#define    VIC_FIQ_PENDING_H_V      (VIC_BASE_V + 0x064)
#define    VIC_IRQ_VECTOR_BASE_V    (VIC_BASE_V + 0x070)
#define    VIC_FIQ_VECTOR_BASE_V    (VIC_BASE_V + 0x074)
#define    VIC_IRQ_VECTOR_NUM_V     (VIC_BASE_V + 0x078)
#define    VIC_FIQ_VECTOR_NUM_V     (VIC_BASE_V + 0x07C)
#define    VIC_IRQ_VECTOR_ADDR_V    (VIC_BASE_V + 0x080)
#define    VIC_FIQ_VECTOR_ADDR_V    (VIC_BASE_V + 0x084)
#define    VIC_PROIRTY_MASK_V       (VIC_BASE_V + 0x090)
#define    VIC_VECTOR_PROIRTY00_V   (VIC_BASE_V + 0x100)
#define    VIC_VECTOR_PROIRTY01_V   (VIC_BASE_V + 0x104)
#define    VIC_VECTOR_PROIRTY02_V   (VIC_BASE_V + 0x108)
#define    VIC_VECTOR_PROIRTY03_V   (VIC_BASE_V + 0x10C)
#define    VIC_VECTOR_PROIRTY04_V   (VIC_BASE_V + 0x110)
#define    VIC_VECTOR_PROIRTY05_V   (VIC_BASE_V + 0x114)
#define    VIC_VECTOR_PROIRTY06_V   (VIC_BASE_V + 0x118)
#define    VIC_VECTOR_PROIRTY07_V   (VIC_BASE_V + 0x11C)
#define    VIC_VECTOR_PROIRTY08_V   (VIC_BASE_V + 0x120)
#define    VIC_VECTOR_PROIRTY09_V   (VIC_BASE_V + 0x124)
#define    VIC_VECTOR_PROIRTY10_V   (VIC_BASE_V + 0x128)
#define    VIC_VECTOR_PROIRTY11_V   (VIC_BASE_V + 0x12C)
#define    VIC_VECTOR_PROIRTY12_V   (VIC_BASE_V + 0x130)
#define    VIC_VECTOR_PROIRTY13_V   (VIC_BASE_V + 0x134)
#define    VIC_VECTOR_PROIRTY14_V   (VIC_BASE_V + 0x138)
#define    VIC_VECTOR_PROIRTY15_V   (VIC_BASE_V + 0x13C)
#define    VIC_VECTOR_PROIRTY16_V   (VIC_BASE_V + 0x140)
#define    VIC_VECTOR_PROIRTY17_V   (VIC_BASE_V + 0x144)
#define    VIC_VECTOR_PROIRTY18_V   (VIC_BASE_V + 0x148)
#define    VIC_VECTOR_PROIRTY19_V   (VIC_BASE_V + 0x14C)
#define    VIC_VECTOR_PROIRTY20_V   (VIC_BASE_V + 0x150)
#define    VIC_VECTOR_PROIRTY21_V   (VIC_BASE_V + 0x154)
#define    VIC_VECTOR_PROIRTY22_V   (VIC_BASE_V + 0x158)
#define    VIC_VECTOR_PROIRTY23_V   (VIC_BASE_V + 0x15C)
#define    VIC_VECTOR_PROIRTY24_V   (VIC_BASE_V + 0x160)
#define    VIC_VECTOR_PROIRTY25_V   (VIC_BASE_V + 0x164)
#define    VIC_VECTOR_PROIRTY26_V   (VIC_BASE_V + 0x168)
#define    VIC_VECTOR_PROIRTY27_V   (VIC_BASE_V + 0x16C)
#define    VIC_VECTOR_PROIRTY28_V   (VIC_BASE_V + 0x170)
#define    VIC_VECTOR_PROIRTY29_V   (VIC_BASE_V + 0x174)
#define    VIC_VECTOR_PROIRTY30_V   (VIC_BASE_V + 0x178)
#define    VIC_VECTOR_PROIRTY31_V   (VIC_BASE_V + 0x17C)
#define    VIC_VECTOR_PROIRTY32_V   (VIC_BASE_V + 0x180)
#define    VIC_VECTOR_PROIRTY33_V   (VIC_BASE_V + 0x184)
#define    VIC_VECTOR_PROIRTY34_V   (VIC_BASE_V + 0x188)
#define    VIC_VECTOR_PROIRTY35_V   (VIC_BASE_V + 0x18C)
#define    VIC_VECTOR_PROIRTY36_V   (VIC_BASE_V + 0x190)
#define    VIC_VECTOR_PROIRTY37_V   (VIC_BASE_V + 0x194)
#define    VIC_VECTOR_PROIRTY38_V   (VIC_BASE_V + 0x198)
#define    VIC_VECTOR_PROIRTY39_V   (VIC_BASE_V + 0x19C)
#define    VIC_VECTOR_PROIRTY40_V   (VIC_BASE_V + 0x1A0)
#define    VIC_VECTOR_PROIRTY41_V   (VIC_BASE_V + 0x1A4)
#define    VIC_VECTOR_PROIRTY42_V   (VIC_BASE_V + 0x1A8)
#define    VIC_VECTOR_PROIRTY43_V   (VIC_BASE_V + 0x1AC)
#define    VIC_VECTOR_PROIRTY44_V   (VIC_BASE_V + 0x1B0)
#define    VIC_VECTOR_PROIRTY45_V   (VIC_BASE_V + 0x1B4)
#define    VIC_VECTOR_PROIRTY46_V   (VIC_BASE_V + 0x1B8)
#define    VIC_VECTOR_PROIRTY47_V   (VIC_BASE_V + 0x1BC)
#define    VIC_VECTOR_PROIRTY48_V   (VIC_BASE_V + 0x1C0)
#define    VIC_VECTOR_PROIRTY49_V   (VIC_BASE_V + 0x1C4)
#define    VIC_VECTOR_PROIRTY50_V   (VIC_BASE_V + 0x1C8)
#define    VIC_VECTOR_PROIRTY51_V   (VIC_BASE_V + 0x1CC)
#define    VIC_VECTOR_PROIRTY52_V   (VIC_BASE_V + 0x1D0)
#define    VIC_VECTOR_PROIRTY53_V   (VIC_BASE_V + 0x1D4)
#define    VIC_VECTOR_PROIRTY54_V   (VIC_BASE_V + 0x1D8)
#define    VIC_VECTOR_PROIRTY55_V   (VIC_BASE_V + 0x1DC)
#define    VIC_VECTOR_PROIRTY56_V   (VIC_BASE_V + 0x1E0)
#define    VIC_VECTOR_PROIRTY57_V   (VIC_BASE_V + 0x1E4)
#define    VIC_VECTOR_PROIRTY58_V   (VIC_BASE_V + 0x1E8)
#define    VIC_VECTOR_PROIRTY59_V   (VIC_BASE_V + 0x1EC)
#define    VIC_VECTOR_PROIRTY60_V   (VIC_BASE_V + 0x1F0)
#define    VIC_VECTOR_PROIRTY61_V   (VIC_BASE_V + 0x1F4)
#define    VIC_VECTOR_PROIRTY62_V   (VIC_BASE_V + 0x1F8)
#define    VIC_VECTOR_PROIRTY63_V   (VIC_BASE_V + 0x1FC)

/*
 * PMU模块
 * 物理基址: 0xB0001000
 * 虚拟基址: 0x30001000
 */
#ifdef PMU_BASE
#define    PMU_PLL_SET  			(PMU_BASE + 0x000)
#define    PMU_APLL_CFG  			(PMU_BASE + 0x004)
#define    PMU_MPLL_CFG  			(PMU_BASE + 0x008)
#define    PMU_DPLL_CFG  			(PMU_BASE + 0x00C)
#define    PMU_PMDR  				(PMU_BASE + 0x010)
#define    PMU_CLK_GT_CFG1  		(PMU_BASE + 0x014)
#define    PMU_CLK_GT_CFG2  		(PMU_BASE + 0x018)
#define    PMU_PWR_GT_CFG  			(PMU_BASE + 0x01C)
#define    PMU_AHB_CLK_CFG  		(PMU_BASE + 0x020)
#define    PMU_CORE_CLK_CFG  		(PMU_BASE + 0x024)
#define    PMU_DDR_CLK_CFG  		(PMU_BASE + 0x028)
#define    PMU_PIX_CLK_CFG  		(PMU_BASE + 0x02C)
#define    PMU_GPU_CLK2X_CFG  		(PMU_BASE + 0x030)
#define    PMU_DIV_SET  			(PMU_BASE + 0x034)
#define    PMU_CRYSTAL_CFG  		(PMU_BASE + 0x038)
#define    PMU_MSK_WAKEUP  			(PMU_BASE + 0x03C)
#define    PMU_RTCR  				(PMU_BASE + 0x040)
#define    PMU_CLR_WAKEUP  			(PMU_BASE + 0x044)
#define    PMU_WAKEUP_TIME  		(PMU_BASE + 0x048)
#define    PMU_SLEEP_FLAG  			(PMU_BASE + 0x04C)
#define    PMU_WAIT_PWR_SWITCH  	(PMU_BASE + 0x050)
#define    PMU_PWR_STATE  			(PMU_BASE + 0x054)
#define    PMU_IO_POL_SEL  			(PMU_BASE + 0x058)
#define    PMU_PLLLD  				(PMU_BASE + 0x05C)
#define    PMU_IC_ENABLE  			(PMU_BASE + 0x060)
#define    PMU_IC_TAR  				(PMU_BASE + 0x064)
#define    PMU_IC_SCL_LCNT  		(PMU_BASE + 0x068)
#define    PMU_IC_SCL_HCNT  		(PMU_BASE + 0x06C)
#define    PMU_IC_DATA_CMD  		(PMU_BASE + 0x070)
#define    PMU_IC_STATE  			(PMU_BASE + 0x074)
#define    PMU_IC_SET  				(PMU_BASE + 0x078)
#define    PMU_HA_PWR_OFF_DAT  		(PMU_BASE + 0x07C)
#define    PMU_HA_PWR_ON_DAT  		(PMU_BASE + 0x080)
#define    PMU_HA_PWR_OFF_DAT_CNT  	(PMU_BASE + 0x084)
#define    PMU_HA_PWR_ON_DAT_CNT  	(PMU_BASE + 0x088)
#define    PMU_PWR_OFF_TIME  		(PMU_BASE + 0x08C)
#define    PMU_PWR_ON_TIME  		(PMU_BASE + 0x090)
#define    PMU_POWERON_POL_SEL  	(PMU_BASE + 0x094)
#define    PMU_RETURN_ADDR  		(PMU_BASE + 0x098)
#define    PMU_INT  				(PMU_BASE + 0x09C)
#define    DDRPHY_RETENTION			(PMU_BASE + 0x0a0)
#define    WAITPOWERONREADYTIME		(PMU_BASE + 0x0a4)
#endif

#ifdef PMU_BASE_V
#define    PMU_PLL_SET_V			(PMU_BASE_V + 0x000)
#define    PMU_APLL_CFG_V			(PMU_BASE_V + 0x004)
#define    PMU_MPLL_CFG_V			(PMU_BASE_V + 0x008)
#define    PMU_DPLL_CFG_V			(PMU_BASE_V + 0x00C)
#define    PMU_PMDR_V				(PMU_BASE_V + 0x010)
#define    PMU_CLK_GT_CFG1_V		(PMU_BASE_V + 0x014)
#define    PMU_CLK_GT_CFG2_V		(PMU_BASE_V + 0x018)
#define    PMU_PWR_GT_CFG_V			(PMU_BASE_V + 0x01C)
#define    PMU_AHB_CLK_CFG_V		(PMU_BASE_V + 0x020)
#define    PMU_CORE_CLK_CFG_V		(PMU_BASE_V + 0x024)
#define    PMU_DDR_CLK_CFG_V		(PMU_BASE_V + 0x028)
#define    PMU_PIX_CLK_CFG_V		(PMU_BASE_V + 0x02C)
#define    PMU_GPU_CLK2X_CFG_V		(PMU_BASE_V + 0x030)
#define    PMU_DIV_SET_V			(PMU_BASE_V + 0x034)
#define    PMU_CRYSTAL_CFG_V		(PMU_BASE_V + 0x038)
#define    PMU_MSK_WAKEUP_V			(PMU_BASE_V + 0x03C)
#define    PMU_RTCR_V				(PMU_BASE_V + 0x040)
#define    PMU_CLR_WAKEUP_V			(PMU_BASE_V + 0x044)
#define    PMU_WAKEUP_TIME_V		(PMU_BASE_V + 0x048)
#define    PMU_SLEEP_FLAG_V			(PMU_BASE_V + 0x04C)
#define    PMU_WAIT_PWR_SWITCH_V	(PMU_BASE_V + 0x050)
#define    PMU_PWR_STATE_V			(PMU_BASE_V + 0x054)
#define    PMU_IO_POL_SEL_V			(PMU_BASE_V + 0x058)
#define    PMU_PLLLD_V				(PMU_BASE_V + 0x05C)
#define    PMU_IC_ENABLE_V			(PMU_BASE_V + 0x060)
#define    PMU_IC_TAR_V				(PMU_BASE_V + 0x064)
#define    PMU_IC_SCL_LCNT_V		(PMU_BASE_V + 0x068)
#define    PMU_IC_SCL_HCNT_V		(PMU_BASE_V + 0x06C)
#define    PMU_IC_DATA_CMD_V		(PMU_BASE_V + 0x070)
#define    PMU_IC_STATE_V			(PMU_BASE_V + 0x074)
#define    PMU_IC_SET_V				(PMU_BASE_V + 0x078)
#define    PMU_HA_PWR_OFF_DAT_V		(PMU_BASE_V + 0x07C)
#define    PMU_HA_PWR_ON_DAT_V		(PMU_BASE_V + 0x080)
#define    PMU_HA_PWR_OFF_DAT_CNT_V	(PMU_BASE_V + 0x084)
#define    PMU_HA_PWR_ON_DAT_CNT_V	(PMU_BASE_V + 0x088)
#define    PMU_PWR_OFF_TIME_V		(PMU_BASE_V + 0x08C)
#define    PMU_PWR_ON_TIME_V		(PMU_BASE_V + 0x090)
#define    PMU_POWERON_POL_SEL_V	(PMU_BASE_V + 0x094)
#define    PMU_RETURN_ADDR_V		(PMU_BASE_V + 0x098)
#define    PMU_INT_V				(PMU_BASE_V + 0x09C)
#define    DDRPHY_RETENTION_V		(PMU_BASE_V + 0x0a0)
#define    WAITPOWERONREADYTIME_V	(PMU_BASE_V + 0x0a4)
#endif


/*
 * TIMER模块
 * 物理基址: 0xB0003000
 * 虚拟基址: 0x30003000
 */

/* 物理地址 */

#define    TIMER_T1LCR         		(TIMER_BASE + 0X000)      /* 通道1加载计数寄存器 */
#define    TIMER_T1CCR              (TIMER_BASE + 0X004)      /* 通道1当前计数值寄存器 */
#define    TIMER_T1CR               (TIMER_BASE + 0X008)      /* 通道1控制寄存器 */
#define    TIMER_T1ISCR             (TIMER_BASE + 0X00C)      /* 通道1中断状态清除寄存器 */
#define    TIMER_T1IMSR             (TIMER_BASE + 0X010)      /* 通道1中断屏蔽状态寄存器 */
#define    TIMER_T2LCR              (TIMER_BASE + 0X020)      /* 通道2加载计数寄存器 */
#define    TIMER_T2CCR              (TIMER_BASE + 0X024)      /* 通道2当前计数值寄存器 */
#define    TIMER_T2CR               (TIMER_BASE + 0X028)      /* 通道2控制寄存器 */
#define    TIMER_T2ISCR             (TIMER_BASE + 0X02C)      /* 通道2中断状态清除寄存器 */
#define    TIMER_T2IMSR             (TIMER_BASE + 0X030)      /* 通道2中断屏蔽状态寄存器 */
#define    TIMER_T3LCR              (TIMER_BASE + 0X040)      /* 通道3加载计数寄存器 */
#define    TIMER_T3CCR              (TIMER_BASE + 0X044)      /* 通道3当前计数值寄存器 */
#define    TIMER_T3CR               (TIMER_BASE + 0X048)      /* 通道3控制寄存器 */
#define    TIMER_T3ISCR             (TIMER_BASE + 0X04C)      /* 通道3中断状态清除寄存器 */
#define    TIMER_T3IMSR             (TIMER_BASE + 0X050)      /* 通道3中断屏蔽状态寄存器 */
#define    TIMER_T3CAPR             (TIMER_BASE + 0X054)      /* 通道3捕获寄存器 */
#define    TIMER_T4LCR              (TIMER_BASE + 0X060)      /* 通道4加载计数寄存器 */
#define    TIMER_T4CCR              (TIMER_BASE + 0X064)      /* 通道4当前计数值寄存器 */
#define    TIMER_T4CR               (TIMER_BASE + 0X068)      /* 通道4控制寄存器 */
#define    TIMER_T4ISCR             (TIMER_BASE + 0X06C)      /* 通道4中断状态清除寄存器 */
#define    TIMER_T4IMSR             (TIMER_BASE + 0X070)      /* 通道4中断屏蔽状态寄存器 */
#define    TIMER_T4CAPR             (TIMER_BASE + 0X074)      /* 通道4捕获寄存器 */
#define    TIMER_T5LCR              (TIMER_BASE + 0X080)      /* 通道5加载计数寄存器 */
#define    TIMER_T5CCR              (TIMER_BASE + 0X084)      /* 通道5当前计数值寄存器 */
#define    TIMER_T5CR               (TIMER_BASE + 0X088)      /* 通道5控制寄存器 */
#define    TIMER_T5ISCR             (TIMER_BASE + 0X08C)      /* 通道5中断状态清除寄存器 */
#define    TIMER_T5IMSR             (TIMER_BASE + 0X090)      /* 通道5中断屏蔽状态寄存器 */
#define    TIMER_T5CAPR             (TIMER_BASE + 0X094)      /* 通道5捕获寄存器 */
#define    TIMER_T6LCR              (TIMER_BASE + 0X0A0)      /* 通道6加载计数寄存器 */
#define    TIMER_T6CCR              (TIMER_BASE + 0X0A4)      /* 通道6当前计数值寄存器 */
#define    TIMER_T6CR               (TIMER_BASE + 0X0A8)      /* 通道6控制寄存器 */
#define    TIMER_T6ISCR             (TIMER_BASE + 0X0AC)      /* 通道6中断状态清除寄存器 */
#define    TIMER_T6IMSR             (TIMER_BASE + 0X0B0)      /* 通道6中断屏蔽状态寄存器 */
#define    TIMER_T6CAPR             (TIMER_BASE + 0X0B4)      /* 通道6捕获寄存器 */
#define    TIMER_T7LCR              (TIMER_BASE + 0X0C0)      /* 通道7加载计数寄存器 */
#define    TIMER_T7CCR              (TIMER_BASE + 0X0C4)      /* 通道7当前计数值寄存器 */
#define    TIMER_T7CR               (TIMER_BASE + 0X0C8)      /* 通道7控制寄存器 */
#define    TIMER_T7ISCR             (TIMER_BASE + 0X0CC)      /* 通道7中断状态清除寄存器 */
#define    TIMER_T7IMSR             (TIMER_BASE + 0X0D0)      /* 通道7中断屏蔽状态寄存器 */
#define    TIMER_T8LCR              (TIMER_BASE + 0X0E0)      /* 通道8加载计数寄存器 */
#define    TIMER_T8CCR              (TIMER_BASE + 0X0E4)      /* 通道8当前计数值寄存器 */
#define    TIMER_T8CR               (TIMER_BASE + 0X0E8)      /* 通道8控制寄存器 */
#define    TIMER_T8ISCR             (TIMER_BASE + 0X0EC)      /* 通道8中断状态清除寄存器 */
#define    TIMER_T8IMSR             (TIMER_BASE + 0X0F0)      /* 通道8中断屏蔽状态寄存器 */
#define    TIMER_T9LCR              (TIMER_BASE + 0X100)      /* 通道9加载计数寄存器 */
#define    TIMER_T9CCR              (TIMER_BASE + 0X104)      /* 通道9当前计数值寄存器 */
#define    TIMER_T9CR               (TIMER_BASE + 0X108)      /* 通道9控制寄存器 */
#define    TIMER_T9ISCR             (TIMER_BASE + 0X10C)      /* 通道9中断状态清除寄存器 */
#define    TIMER_T9IMSR             (TIMER_BASE + 0X110)      /* 通道9中断屏蔽状态寄存器 */
#define    TIMER_T10LCR             (TIMER_BASE + 0X120)      /* 通道10加载计数寄存器 */
#define    TIMER_T10CCR             (TIMER_BASE + 0X124)      /* 通道10当前计数值寄存器 */
#define    TIMER_T10CR              (TIMER_BASE + 0X128)      /* 通道10控制寄存器 */
#define    TIMER_T10ISCR            (TIMER_BASE + 0X12C)      /* 通道10中断状态清除寄存器 */
#define    TIMER_T10IMSR            (TIMER_BASE + 0X130)      /* 通道10中断屏蔽状态寄存器 */
#define    TIMER_TIMSR              (TIMER_BASE + 0X140)      /* TIMER中断屏蔽状态寄存器 */
#define    TIMER_TISCR              (TIMER_BASE + 0X144)      /* TIMER中断状态清除寄存器 */
#define    TIMER_TISR               (TIMER_BASE + 0X148)      /* TIMER中断状态寄存器 */

/* 虚拟地址 */
/* 虚拟基址: 0x30003000*/
#define    TIMER_T1LCR_V            (TIMER_BASE_V + 0X000)    /* 通道1加载计数寄存器 */
#define    TIMER_T1CCR_V            (TIMER_BASE_V + 0X004)    /* 通道1当前计数值寄存器 */
#define    TIMER_T1CR_V             (TIMER_BASE_V + 0X008)    /* 通道1控制寄存器 */
#define    TIMER_T1ISCR_V           (TIMER_BASE_V + 0X00C)    /* 通道1中断状态清除寄存器 */
#define    TIMER_T1IMSR_V           (TIMER_BASE_V + 0X010)    /* 通道1中断屏蔽状态寄存器 */
#define    TIMER_T2LCR_V            (TIMER_BASE_V + 0X020)    /* 通道2加载计数寄存器 */
#define    TIMER_T2CCR_V            (TIMER_BASE_V + 0X024)    /* 通道2当前计数值寄存器 */
#define    TIMER_T2CR_V             (TIMER_BASE_V + 0X028)    /* 通道2控制寄存器 */
#define    TIMER_T2ISCR_V           (TIMER_BASE_V + 0X02C)    /* 通道2中断状态清除寄存器 */
#define    TIMER_T2IMSR_V           (TIMER_BASE_V + 0X030)    /* 通道2中断屏蔽状态寄存器 */
#define    TIMER_T3LCR_V            (TIMER_BASE_V + 0X040)    /* 通道3加载计数寄存器 */
#define    TIMER_T3CCR_V            (TIMER_BASE_V + 0X044)    /* 通道3当前计数值寄存器 */
#define    TIMER_T3CR_V             (TIMER_BASE_V + 0X048)    /* 通道3控制寄存器 */
#define    TIMER_T3ISCR_V           (TIMER_BASE_V + 0X04C)    /* 通道3中断状态清除寄存器 */
#define    TIMER_T3IMSR_V           (TIMER_BASE_V + 0X050)    /* 通道3中断屏蔽状态寄存器 */
#define    TIMER_T3CAPR_V           (TIMER_BASE_V + 0X054)    /* 通道3捕获寄存器 */
#define    TIMER_T4LCR_V            (TIMER_BASE_V + 0X060)    /* 通道4加载计数寄存器 */
#define    TIMER_T4CCR_V            (TIMER_BASE_V + 0X064)    /* 通道4当前计数值寄存器 */
#define    TIMER_T4CR_V             (TIMER_BASE_V + 0X068)    /* 通道4控制寄存器 */
#define    TIMER_T4ISCR_V           (TIMER_BASE_V + 0X06C)    /* 通道4中断状态清除寄存器 */
#define    TIMER_T4IMSR_V           (TIMER_BASE_V + 0X070)    /* 通道4中断屏蔽状态寄存器 */
#define    TIMER_T4CAPR_V           (TIMER_BASE_V + 0X074)    /* 通道4捕获寄存器 */
#define    TIMER_T5LCR_V            (TIMER_BASE_V + 0X080)    /* 通道5加载计数寄存器 */
#define    TIMER_T5CCR_V            (TIMER_BASE_V + 0X084)    /* 通道5当前计数值寄存器 */
#define    TIMER_T5CR_V             (TIMER_BASE_V + 0X088)    /* 通道5控制寄存器 */
#define    TIMER_T5ISCR_V           (TIMER_BASE_V + 0X08C)    /* 通道5中断状态清除寄存器 */
#define    TIMER_T5IMSR_V        	(TIMER_BASE_V +  + 0X090)    /* 通道5中断屏蔽状态寄存器 */
#define    TIMER_T5CAPR_V           (TIMER_BASE_V +  + 0X094)    /* 通道5捕获寄存器 */
#define    TIMER_T6LCR_V            (TIMER_BASE_V +  + 0X0A0)    /* 通道6加载计数寄存器 */
#define    TIMER_T6CCR_V            (TIMER_BASE_V +  + 0X0A4)    /* 通道6当前计数值寄存器 */
#define    TIMER_T6CR_V             (TIMER_BASE_V +  + 0X0A8)    /* 通道6控制寄存器 */
#define    TIMER_T6ISCR_V           (TIMER_BASE_V +  + 0X0AC)    /* 通道6中断状态清除寄存器 */
#define    TIMER_T6IMSR_V           (TIMER_BASE_V +  + 0X0B0)    /* 通道6中断屏蔽状态寄存器 */
#define    TIMER_T6CAPR_V           (TIMER_BASE_V +  + 0X0B4)    /* 通道6捕获寄存器 */
#define    TIMER_T7LCR_V            (TIMER_BASE_V +  + 0X0C0)    /* 通道7加载计数寄存器 */
#define    TIMER_T7CCR_V            (TIMER_BASE_V +  + 0X0C4)    /* 通道7当前计数值寄存器 */
#define    TIMER_T7CR_V             (TIMER_BASE_V +  + 0X0C8)    /* 通道7控制寄存器 */
#define    TIMER_T7ISCR_V           (TIMER_BASE_V +  + 0X0CC)    /* 通道7中断状态清除寄存器 */
#define    TIMER_T7IMSR_V           (TIMER_BASE_V +  + 0X0D0)    /* 通道7中断屏蔽状态寄存器 */
#define    TIMER_T8LCR_V            (TIMER_BASE_V +  + 0X0E0)    /* 通道8加载计数寄存器 */
#define    TIMER_T8CCR_V            (TIMER_BASE_V +  + 0X0E4)    /* 通道8当前计数值寄存器 */
#define    TIMER_T8CR_V             (TIMER_BASE_V +  + 0X0E8)    /* 通道8控制寄存器 */
#define    TIMER_T8ISCR_V           (TIMER_BASE_V +  + 0X0EC)    /* 通道8中断状态清除寄存器 */
#define    TIMER_T8IMSR_V           (TIMER_BASE_V +  + 0X0F0)    /* 通道8中断屏蔽状态寄存器 */
#define    TIMER_T9LCR_V            (TIMER_BASE_V +  + 0X100)    /* 通道9加载计数寄存器 */
#define    TIMER_T9CCR_V            (TIMER_BASE_V +  + 0X104)    /* 通道9当前计数值寄存器 */
#define    TIMER_T9CR_V             (TIMER_BASE_V +  + 0X108)    /* 通道9控制寄存器 */
#define    TIMER_T9ISCR_V           (TIMER_BASE_V +  + 0X10C)    /* 通道9中断状态清除寄存器 */
#define    TIMER_T9IMSR_V           (TIMER_BASE_V +  + 0X110)    /* 通道9中断屏蔽状态寄存器 */
#define    TIMER_T10LCR_V           (TIMER_BASE_V +  + 0X120)    /* 通道10加载计数寄存器 */
#define    TIMER_T10CCR_V           (TIMER_BASE_V +  + 0X124)    /* 通道10当前计数值寄存器 */
#define    TIMER_T10CR_V            (TIMER_BASE_V +  + 0X128)    /* 通道10控制寄存器 */
#define    TIMER_T10ISCR_V          (TIMER_BASE_V +  + 0X12C)    /* 通道10中断状态清除寄存器 */
#define    TIMER_T10IMSR_V          (TIMER_BASE_V +  + 0X130)    /* 通道10中断屏蔽状态寄存器 */
#define    TIMER_TIMSR_V            (TIMER_BASE_V +  + 0X140)    /* TIMER中断屏蔽状态寄存器 */
#define    TIMER_TISCR_V            (TIMER_BASE_V +  + 0X144)    /* TIMER中断状态清除寄存器 */
#define    TIMER_TISR_V             (TIMER_BASE_V +  + 0X148)    /* TIMER中断状态寄存器 */


/*
 * PWM模块
 * 物理基址: 0xB0004000
 * 虚拟基址: 0x30004000
 */

/* 物理地址 */
#define		PWM1_CTRL               (PWM_BASE + 0X000)        /* PWM1控制寄存器 */
#define		PWM1_DIV                (PWM_BASE + 0X004)        /* PWM1分频寄存器 */
#define		PWM1_PERIOD             (PWM_BASE + 0X008)        /* PWM1周期寄存器 */
#define		PWM1_DATA               (PWM_BASE + 0X00C)        /* PWM1数据寄存器 */
#define		PWM1_CNT                (PWM_BASE + 0X010)        /* PWM1计数寄存器 */
#define		PWM1_STATUS             (PWM_BASE + 0X014)        /* PWM1状态寄存器 */
#define		PWM2_CTRL               (PWM_BASE + 0X020)        /* PWM2控制寄存器 */
#define		PWM2_DIV                (PWM_BASE + 0X024)        /* PWM2分频寄存器 */
#define		PWM2_PERIOD             (PWM_BASE + 0X028)        /* PWM2周期寄存器 */
#define		PWM2_DATA               (PWM_BASE + 0X02C)        /* PWM2数据寄存器 */
#define		PWM2_CNT                (PWM_BASE + 0X030)        /* PWM2计数寄存器 */
#define		PWM2_STATUS             (PWM_BASE + 0X034)        /* PWM2状态寄存器 */
#define		PWM3_CTRL               (PWM_BASE + 0X040)        /* PWM3控制寄存器 */
#define		PWM3_DIV                (PWM_BASE + 0X044)        /* PWM3分频寄存器 */
#define		PWM3_PERIOD             (PWM_BASE + 0X048)        /* PWM3周期寄存器 */
#define		PWM3_DATA               (PWM_BASE + 0X04C)        /* PWM3数据寄存器 */
#define		PWM3_CNT                (PWM_BASE + 0X050)        /* PWM3计数寄存器 */
#define		PWM3_STATUS             (PWM_BASE + 0X054)        /* PWM3状态寄存器 */
#define		PWM4_CTRL               (PWM_BASE + 0X060)        /* PWM4控制寄存器 */
#define		PWM4_DIV                (PWM_BASE + 0X064)        /* PWM4分频寄存器 */
#define		PWM4_PERIOD             (PWM_BASE + 0X068)        /* PWM4周期寄存器 */
#define		PWM4_DATA               (PWM_BASE + 0X06C)        /* PWM4数据寄存器 */
#define		PWM4_CNT                (PWM_BASE + 0X070)        /* PWM4计数寄存器 */
#define		PWM4_STATUS             (PWM_BASE + 0X074)        /* PWM4状态寄存器 */
#define		PWM_INTMASK             (PWM_BASE + 0X080)        /* PWM中断屏蔽寄存器 */
#define		PWM_INT                 (PWM_BASE + 0X084)        /* PWM中断寄存器 */
#define		PWM_ENABLE              (PWM_BASE + 0X088)        /* PWM使能寄存器 */

/* 虚拟地址 */
#define		PWM1_CTRL_V             (PWM_BASE_V + 0X000)      /* PWM1控制寄存器 */
#define		PWM1_DIV_V              (PWM_BASE_V + 0X004)      /* PWM1分频寄存器 */
#define		PWM1_PERIOD_V           (PWM_BASE_V + 0X008)      /* PWM1周期寄存器 */
#define		PWM1_DATA_V             (PWM_BASE_V + 0X00C)      /* PWM1数据寄存器 */
#define		PWM1_CNT_V              (PWM_BASE_V + 0X010)      /* PWM1计数寄存器 */
#define		PWM1_STATUS_V           (PWM_BASE_V + 0X014)      /* PWM1状态寄存器 */
#define		PWM2_CTRL_V             (PWM_BASE_V + 0X020)      /* PWM2控制寄存器 */
#define		PWM2_DIV_V              (PWM_BASE_V + 0X024)      /* PWM2分频寄存器 */
#define		PWM2_PERIOD_V           (PWM_BASE_V + 0X028)      /* PWM2周期寄存器 */
#define		PWM2_DATA_V             (PWM_BASE_V + 0X02C)      /* PWM2数据寄存器 */
#define		PWM2_CNT_V              (PWM_BASE_V + 0X030)      /* PWM2计数寄存器 */
#define		PWM2_STATUS_V           (PWM_BASE_V + 0X034)      /* PWM2状态寄存器 */
#define		PWM3_CTRL_V             (PWM_BASE_V + 0X040)      /* PWM3控制寄存器 */
#define		PWM3_DIV_V              (PWM_BASE_V + 0X044)      /* PWM3分频寄存器 */
#define		PWM3_PERIOD_V           (PWM_BASE_V + 0X048)      /* PWM3周期寄存器 */
#define		PWM3_DATA_V             (PWM_BASE_V + 0X04C)      /* PWM3数据寄存器 */
#define		PWM3_CNT_V              (PWM_BASE_V + 0X050)      /* PWM3计数寄存器 */
#define		PWM3_STATUS_V           (PWM_BASE_V + 0X054)      /* PWM3状态寄存器 */
#define		PWM4_CTRL_V             (PWM_BASE_V + 0X060)      /* PWM4控制寄存器 */
#define		PWM4_DIV_V              (PWM_BASE_V + 0X064)      /* PWM4分频寄存器 */
#define		PWM4_PERIOD_V           (PWM_BASE_V + 0X068)      /* PWM4周期寄存器 */
#define		PWM4_DATA_V             (PWM_BASE_V + 0X06C)      /* PWM4数据寄存器 */
#define		PWM4_CNT_V              (PWM_BASE_V + 0X070)      /* PWM4计数寄存器 */
#define		PWM4_STATUS_V           (PWM_BASE_V + 0X074)      /* PWM4状态寄存器 */
#define		PWM_INTMASK_V           (PWM_BASE_V + 0X080)      /* PWM中断屏蔽寄存器 */
#define		PWM_INT_V               (PWM_BASE_V + 0X084)      /* PWM中断寄存器 */
#define		PWM_ENABLE_V            (PWM_BASE_V + 0X088)      /* PWM使能寄存器 */


/*
 * UART0模块
 * 物理基址: 0xB1000000
 * 虚拟基址: 0x31000000
 */

/* 物理地址 */
#define		UART0_DLBL              (UART0_BASE + 0X000)      /* 波特率设置低八位寄存器 */
#define		UART0_RXFIFO            (UART0_BASE + 0X000)      /* 接收FIFO */
#define		UART0_TXFIFO            (UART0_BASE + 0X000)      /* 发送FIFO */
#define		UART0_DLBH              (UART0_BASE + 0X004)      /* 波特率设置高八位寄存器 */
#define		UART0_IER               (UART0_BASE + 0X004)      /* 中断使能寄存器 */
#define		UART0_IIR               (UART0_BASE + 0X008)      /* 中断识别寄存器 */
#define		UART0_FCR               (UART0_BASE + 0X008)      /* FIFO控制寄存器 */
#define		UART0_LCR               (UART0_BASE + 0X00C)      /* 行控制寄存器 */
#define		UART0_MCR               (UART0_BASE + 0X010)      /* Modem控制寄存器 */
#define		UART0_LSR               (UART0_BASE + 0X014)      /* 行状态寄存器 */
#define		UART0_MSR               (UART0_BASE + 0X018)      /* Modem状态寄存器 */

/* 虚拟地址 */
#define		UART0_DLBL_V            (UART0_BASE_V + 0X000)    /* 波特率设置低八位寄存器 */
#define		UART0_RXFIFO_V          (UART0_BASE_V + 0X000)    /* 接收FIFO */
#define		UART0_TXFIFO_V          (UART0_BASE_V + 0X000)    /* 发送FIFO */
#define		UART0_DLBH_V            (UART0_BASE_V + 0X004)    /* 波特率设置高八位寄存器 */
#define		UART0_IER_V             (UART0_BASE_V + 0X004)    /* 中断使能寄存器 */
#define		UART0_IIR_V             (UART0_BASE_V + 0X008)    /* 中断识别寄存器 */
#define		UART0_FCR_V             (UART0_BASE_V + 0X008)    /* FIFO控制寄存器 */
#define		UART0_LCR_V             (UART0_BASE_V + 0X00C)    /* 行控制寄存器 */
#define		UART0_MCR_V             (UART0_BASE_V + 0X010)    /* Modem控制寄存器 */
#define		UART0_LSR_V             (UART0_BASE_V + 0X014)    /* 行状态寄存器 */
#define		UART0_MSR_V             (UART0_BASE_V + 0X018)    /* Modem状态寄存器 */


/*
 * UART1模块
 * 物理基址: 0x31001000
 * 虚拟基址: 0xA1001000
 */

/* 物理地址 */
#define		UART1_DLBL              (UART1_BASE + 0X000)      /* 波特率设置低八位寄存器 */
#define		UART1_RXFIFO            (UART1_BASE + 0X000)      /* 接收FIFO */
#define		UART1_TXFIFO            (UART1_BASE + 0X000)      /* 发送FIFO */
#define		UART1_DLBH              (UART1_BASE + 0X004)      /* 波特率设置高八位寄存器 */
#define		UART1_IER               (UART1_BASE + 0X004)      /* 中断使能寄存器 */
#define		UART1_IIR               (UART1_BASE + 0X008)      /* 中断识别寄存器 */
#define		UART1_FCR               (UART1_BASE + 0X008)      /* FIFO控制寄存器 */
#define		UART1_LCR               (UART1_BASE + 0X00C)      /* 行控制寄存器 */
#define		UART1_MCR               (UART1_BASE + 0X010)      /* Modem控制寄存器 */
#define		UART1_LSR               (UART1_BASE + 0X014)      /* 行状态寄存器 */
#define		UART1_MSR               (UART1_BASE + 0X018)      /* Modem状态寄存器 */

/* 虚拟地址 */
#define		UART1_DLBL_V            (UART1_BASE_V + 0X000)    /* 波特率设置低八位寄存器 */
#define		UART1_RXFIFO_V          (UART1_BASE_V + 0X000)    /* 接收FIFO */
#define		UART1_TXFIFO_V          (UART1_BASE_V + 0X000)    /* 发送FIFO */
#define		UART1_DLBH_V            (UART1_BASE_V + 0X004)    /* 波特率设置高八位寄存器 */
#define		UART1_IER_V             (UART1_BASE_V + 0X004)    /* 中断使能寄存器 */
#define		UART1_IIR_V             (UART1_BASE_V + 0X008)    /* 中断识别寄存器 */
#define		UART1_FCR_V             (UART1_BASE_V + 0X008)    /* FIFO控制寄存器 */
#define		UART1_LCR_V             (UART1_BASE_V + 0X00C)    /* 行控制寄存器 */
#define		UART1_MCR_V             (UART1_BASE_V + 0X010)    /* Modem控制寄存器 */
#define		UART1_LSR_V             (UART1_BASE_V + 0X014)    /* 行状态寄存器 */
#define		UART1_MSR_V             (UART1_BASE_V + 0X018)    /* Modem状态寄存器 */


/*
 * UART2模块
 * 物理基址: 0x31002000
 * 虚拟基址: 0xA1002000
 */

/* 物理地址 */
#define    UART2_DLBL               (UART2_BASE + 0X000)      /* 波特率设置低八位寄存器 */
#define    UART2_RXFIFO             (UART2_BASE + 0X000)      /* 接收FIFO */
#define    UART2_TXFIFO             (UART2_BASE + 0X000)      /* 发送FIFO */
#define    UART2_DLBH               (UART2_BASE + 0X004)      /* 波特率设置高八位寄存器 */
#define    UART2_IER                (UART2_BASE + 0X004)      /* 中断使能寄存器 */
#define    UART2_IIR                (UART2_BASE + 0X008)      /* 中断识别寄存器 */
#define    UART2_FCR                (UART2_BASE + 0X008)      /* FIFO控制寄存器 */
#define    UART2_LCR                (UART2_BASE + 0X00C)      /* 行控制寄存器 */
#define    UART2_MCR                (UART2_BASE + 0X010)      /* Modem控制寄存器 */
#define    UART2_LSR                (UART2_BASE + 0X014)      /* 行状态寄存器 */
#define    UART2_MSR                (UART2_BASE + 0X018)      /* Modem状态寄存器 */

/* 虚拟地址 */
#define    UART2_DLBL_V             (UART2_BASE_V + 0X000)    /* 波特率设置低八位寄存器 */
#define    UART2_RXFIFO_V           (UART2_BASE_V + 0X000)    /* 接收FIFO */
#define    UART2_TXFIFO_V           (UART2_BASE_V + 0X000)    /* 发送FIFO */
#define    UART2_DLBH_V             (UART2_BASE_V + 0X004)    /* 波特率设置高八位寄存器 */
#define    UART2_IER_V              (UART2_BASE_V + 0X004)    /* 中断使能寄存器 */
#define    UART2_IIR_V              (UART2_BASE_V + 0X008)    /* 中断识别寄存器 */
#define    UART2_FCR_V              (UART2_BASE_V + 0X008)    /* FIFO控制寄存器 */
#define    UART2_LCR_V              (UART2_BASE_V + 0X00C)    /* 行控制寄存器 */
#define    UART2_MCR_V              (UART2_BASE_V + 0X010)    /* Modem控制寄存器 */
#define    UART2_LSR_V              (UART2_BASE_V + 0X014)    /* 行状态寄存器 */
#define    UART2_MSR_V              (UART2_BASE_V + 0X018)    /* Modem状态寄存器 */


// UART3 register list
// UART3_BASE 0xB1003000
#define    UART3_DLBL         		(UART3_BASE + 0x000)
#define    UART3_RXFIFO            	(UART3_BASE + 0x000)
#define    UART3_TXFIFO            	(UART3_BASE + 0x000)
#define    UART3_DLBH              	(UART3_BASE + 0x004)
#define    UART3_IER               	(UART3_BASE + 0x004)
#define    UART3_IIR               	(UART3_BASE + 0x008)
#define    UART3_FCR               	(UART3_BASE + 0x008)
#define    UART3_LCR               	(UART3_BASE + 0x00C)
#define    UART3_MCR               	(UART3_BASE + 0x010)
#define    UART3_LSR               	(UART3_BASE + 0x014)
#define    UART3_MSR               	(UART3_BASE + 0x018)

// UART3 register list
// UART3_BASE_V 0x31003000
#define    UART3_DLBL_V            	(UART3_BASE_V + 0x000)
#define    UART3_RXFIF0_V           (UART3_BASE_V + 0x000)
#define    UART3_TXFIFO_V           (UART3_BASE_V + 0x000)
#define    UART3_DLBH_V             (UART3_BASE_V + 0x004)
#define    UART3_IER_V              (UART3_BASE_V + 0x004)
#define    UART3_IIR_V              (UART3_BASE_V + 0x008)
#define    UART3_FCR_V              (UART3_BASE_V + 0x008)
#define    UART3_LCR_V              (UART3_BASE_V + 0x00C)
#define    UART3_MCR_V              (UART3_BASE_V + 0x010)
#define    UART3_LSR_V              (UART3_BASE_V + 0x014)
#define    UART3_MSR_V              (UART3_BASE_V + 0x018)


// GPIO 物理地址
// GPIO_BASE 0xB0006000
#define    GPIO_AO					(GPIO_AO_BASE + 0x110)
#define    GPIO_PORTA				(GPIO_BASE + 0x010)
#define    GPIO_PORTB				(GPIO_BASE + 0x030)
#define    GPIO_PORTC				(GPIO_BASE + 0x050)
#define    GPIO_PORTD				(GPIO_BASE + 0x070)
#define    GPIO_PORTE				(GPIO_BASE + 0x090)
#define    GPIO_PORTF				(GPIO_BASE + 0x0B0)
#define    GPIO_PORTI				(GPIO_BASE + 0x110)

// GPIO AO 物理地址
#define    AO_DBCLK_DIV		   		(GPIO_AO_BASE + 0x000)
#define    AO_PORT_DIR  	     	(GPIO_AO_BASE + 0x110)
#define    AO_PORT_SEL  	     	(GPIO_AO_BASE + 0x114)
#define    AO_PORT_DATA           	(GPIO_AO_BASE + 0x118)
#define    AO_PORT_INTRSEL     	 	(GPIO_AO_BASE + 0x11C)
#define    AO_PORT_INTRLEL        	(GPIO_AO_BASE + 0x120)
#define    AO_PORT_INTRPOL         	(GPIO_AO_BASE + 0x124)
#define    AO_PORT_INTRCLR        	(GPIO_AO_BASE + 0x128)

#define    GPIO_DBCLK               (GPIO_BASE + 0x000)

#define    GPIO_PORTA_DIR           (GPIO_BASE + 0x010)
#define    GPIO_PORTA_SEL           (GPIO_BASE + 0x014)
#define    GPIO_PORTA_DATA          (GPIO_BASE + 0x018)
#define    GPIO_PORTA_INTSEL        (GPIO_BASE + 0x01C)
#define    GPIO_PORTA_INTLEL        (GPIO_BASE + 0x020)
#define    GPIO_PORTA_INTPOL        (GPIO_BASE + 0x024)
#define    GPIO_PORTA_INTCLR        (GPIO_BASE + 0x028)
#define    GPIO_PORTA_SECFUN        (GPIO_BASE + 0x02C)

#define    GPIO_PORTB_DIR           (GPIO_BASE + 0x030)
#define    GPIO_PORTB_SEL           (GPIO_BASE + 0x034)
#define    GPIO_PORTB_DATA          (GPIO_BASE + 0x038)
#define    GPIO_PORTB_INTSEL        (GPIO_BASE + 0x03C)
#define    GPIO_PORTB_INTLEL        (GPIO_BASE + 0x040)
#define    GPIO_PORTB_INTPOL        (GPIO_BASE + 0x044)
#define    GPIO_PORTB_INTCLR        (GPIO_BASE + 0x048)
#define    GPIO_PORTB_SECFUN        (GPIO_BASE + 0x04C)

#define    GPIO_PORTC_DIR           (GPIO_BASE + 0x050)
#define    GPIO_PORTC_SEL           (GPIO_BASE + 0x054)
#define    GPIO_PORTC_DATA          (GPIO_BASE + 0x058)
#define    GPIO_PORTC_INTSEL        (GPIO_BASE + 0x05C)
#define    GPIO_PORTC_INTLEL        (GPIO_BASE + 0x060)
#define    GPIO_PORTC_INTPOL        (GPIO_BASE + 0x064)
#define    GPIO_PORTC_INTCLR        (GPIO_BASE + 0x068)
#define    GPIO_PORTC_SECFUN        (GPIO_BASE + 0x06C)

#define    GPIO_PORTD_DIR           (GPIO_BASE + 0x070)
#define    GPIO_PORTD_SEL           (GPIO_BASE + 0x074)
#define    GPIO_PORTD_DATA          (GPIO_BASE + 0x078)
#define    GPIO_PORTD_INTSEL        (GPIO_BASE + 0x07C)
#define    GPIO_PORTD_INTLEL        (GPIO_BASE + 0x080)
#define    GPIO_PORTD_INTPOL        (GPIO_BASE + 0x084)
#define    GPIO_PORTD_INTCLR        (GPIO_BASE + 0x088)
#define    GPIO_PORTD_SECFUN        (GPIO_BASE + 0x08C)

#define    GPIO_PORTE_DIR           (GPIO_BASE + 0x090)
#define    GPIO_PORTE_SEL           (GPIO_BASE + 0x094)
#define    GPIO_PORTE_DATA          (GPIO_BASE + 0x098)
#define    GPIO_PORTE_INTSEL        (GPIO_BASE + 0x09C)
#define    GPIO_PORTE_INTLEL        (GPIO_BASE + 0x0A0)
#define    GPIO_PORTE_INTPOL        (GPIO_BASE + 0x0A4)
#define    GPIO_PORTE_INTCLR        (GPIO_BASE + 0x0A8)
#define    GPIO_PORTE_SECFUN        (GPIO_BASE + 0x0AC)

#define    GPIO_PORTF_DIR           (GPIO_BASE + 0x0B0)
#define    GPIO_PORTF_SEL           (GPIO_BASE + 0x0B4)
#define    GPIO_PORTF_DATA          (GPIO_BASE + 0x0B8)
#define    GPIO_PORTF_INTSEL        (GPIO_BASE + 0x0BC)
#define    GPIO_PORTF_INTLEL        (GPIO_BASE + 0x0C0)
#define    GPIO_PORTF_INTPOL        (GPIO_BASE + 0x0C4)
#define    GPIO_PORTF_INTCLR        (GPIO_BASE + 0x0C8)
#define    GPIO_PORTF_SECFUN        (GPIO_BASE + 0x0CC)

#define    GPIO_PORTG_DIR           (GPIO_BASE + 0x0D0)
#define    GPIO_PORTG_SEL           (GPIO_BASE + 0x0D4)
#define    GPIO_PORTG_DATA          (GPIO_BASE + 0x0D8)
#define    GPIO_PORTG_INTSEL        (GPIO_BASE + 0x0DC)
#define    GPIO_PORTG_INTLEL        (GPIO_BASE + 0x0E0)
#define    GPIO_PORTG_INTPOL        (GPIO_BASE + 0x0E4)
#define    GPIO_PORTG_INTCLR        (GPIO_BASE + 0x0E8)
#define    GPIO_PORTG_SECFUN        (GPIO_BASE + 0x0EC)

#define    GPIO_PORTH_DIR           (GPIO_BASE + 0x0F0)
#define    GPIO_PORTH_SEL           (GPIO_BASE + 0x0F4)
#define    GPIO_PORTH_DATA          (GPIO_BASE + 0x0F8)
#define    GPIO_PORTH_INTSEL        (GPIO_BASE + 0x0FC)
#define    GPIO_PORTH_INTLEL        (GPIO_BASE + 0x100)
#define    GPIO_PORTH_INTPOL        (GPIO_BASE + 0x104)
#define    GPIO_PORTH_INTCLR        (GPIO_BASE + 0x108)
#define    GPIO_PORTH_SECFUN        (GPIO_BASE + 0x10C)

#define    GPIO_PORTI_DIR           (GPIO_BASE + 0x110)
#define    GPIO_PORTI_SEL           (GPIO_BASE + 0x114)
#define    GPIO_PORTI_DATA          (GPIO_BASE + 0x118)
#define    GPIO_PORTI_INTRSEL       (GPIO_BASE + 0x11C)
#define    GPIO_PORTI_INTRLEL       (GPIO_BASE + 0x120)
#define    GPIO_PORTI_INTRPOL       (GPIO_BASE + 0x124)
#define    GPIO_PORTI_INTRCLR       (GPIO_BASE + 0x128)
#define    GPIO_PORTI_SECFUN        (GPIO_BASE + 0x12C) 

// GPIO 虚拟地址
// GPIO_BASE_V 0x30006000
#define    GPIO_AO_V				(GPIO_AO_BASE_V + 0x110)
#define    GPIO_PORTA_V				(GPIO_BASE_V + 0x010)
#define    GPIO_PORTB_V				(GPIO_BASE_V + 0x030)
#define    GPIO_PORTC_V				(GPIO_BASE_V + 0x050)
#define    GPIO_PORTD_V				(GPIO_BASE_V + 0x070)
#define    GPIO_PORTE_V				(GPIO_BASE_V + 0x090)
#define    GPIO_PORTF_V				(GPIO_BASE_V + 0x0B0)
#define    GPIO_PORTI_V				(GPIO_BASE_V + 0x110)


// GPIO AO 虚拟地址
//GPIO_AO_BASE_V  0x30005000
#define    AO_DBCLK_DIV_V         	(GPIO_AO_BASE_V + 0x000)
#define    AO_PORT_DIR_V  	     	(GPIO_AO_BASE_V + 0x110)
#define    AO_PORT_SEL_V  	     	(GPIO_AO_BASE_V + 0x114)
#define    AO_PORT_DATA_V         	(GPIO_AO_BASE_V + 0x118)
#define    AO_PORT_INTRSEL_V       	(GPIO_AO_BASE_V + 0x11C)
#define    AO_PORT_INTRLEL_V       	(GPIO_AO_BASE_V + 0x120)
#define    AO_PORT_INTRPOL_V       	(GPIO_AO_BASE_V + 0x124)
#define    AO_PORT_INTRCLR_V       	(GPIO_AO_BASE_V + 0x128)

#define    GPIO_DBCLK_V           	(GPIO_BASE_V + 0x000)

#define    GPIO_PORTA_DIR_V         (GPIO_BASE_V + 0x010)
#define    GPIO_PORTA_SEL_V         (GPIO_BASE_V + 0x014)
#define    GPIO_PORTA_DATA_V        (GPIO_BASE_V + 0x018)
#define    GPIO_PORTA_INTSEL_V      (GPIO_BASE_V + 0x01C)
#define    GPIO_PORTA_INTLEL_V      (GPIO_BASE_V + 0x020)
#define    GPIO_PORTA_INTPOL_V      (GPIO_BASE_V + 0x024)
#define    GPIO_PORTA_INTCLR_V      (GPIO_BASE_V + 0x028)
#define    GPIO_PORTA_SECFUN_V      (GPIO_BASE_V + 0x02C)

#define    GPIO_PORTB_DIR_V         (GPIO_BASE_V + 0x030)
#define    GPIO_PORTB_SEL_V         (GPIO_BASE_V + 0x034)
#define    GPIO_PORTB_DATA_V        (GPIO_BASE_V + 0x038)
#define    GPIO_PORTB_INTSEL_V      (GPIO_BASE_V + 0x03C)
#define    GPIO_PORTB_INTLEL_V      (GPIO_BASE_V + 0x040)
#define    GPIO_PORTB_INTPOL_V      (GPIO_BASE_V + 0x044)
#define    GPIO_PORTB_INTCLR_V      (GPIO_BASE_V + 0x048)
#define    GPIO_PORTB_SECFUN_V      (GPIO_BASE_V + 0x04C)

#define    GPIO_PORTC_DIR_V         (GPIO_BASE_V + 0x050)
#define    GPIO_PORTC_SEL_V         (GPIO_BASE_V + 0x054)
#define    GPIO_PORTC_DATA_V        (GPIO_BASE_V + 0x058)
#define    GPIO_PORTC_INTSEL_V      (GPIO_BASE_V + 0x05C)
#define    GPIO_PORTC_INTLEL_V      (GPIO_BASE_V + 0x060)
#define    GPIO_PORTC_INTPOL_V      (GPIO_BASE_V + 0x064)
#define    GPIO_PORTC_INTCLR_V      (GPIO_BASE_V + 0x068)
#define    GPIO_PORTC_SECFUN_V      (GPIO_BASE_V + 0x06C)

#define    GPIO_PORTD_DIR_V         (GPIO_BASE_V + 0x070)
#define    GPIO_PORTD_SEL_V         (GPIO_BASE_V + 0x074)
#define    GPIO_PORTD_DATA_V        (GPIO_BASE_V + 0x078)
#define    GPIO_PORTD_INTSEL_V      (GPIO_BASE_V + 0x07C)
#define    GPIO_PORTD_INTLEL_V      (GPIO_BASE_V + 0x080)
#define    GPIO_PORTD_INTPOL_V      (GPIO_BASE_V + 0x084)
#define    GPIO_PORTD_INTCLR_V      (GPIO_BASE_V + 0x088)
#define    GPIO_PORTD_SECFUN_V      (GPIO_BASE_V + 0x08C)

#define    GPIO_PORTE_DIR_V         (GPIO_BASE_V + 0x090)
#define    GPIO_PORTE_SEL_V         (GPIO_BASE_V + 0x094)
#define    GPIO_PORTE_DATA_V        (GPIO_BASE_V + 0x098)
#define    GPIO_PORTE_INTSEL_V      (GPIO_BASE_V + 0x09C)
#define    GPIO_PORTE_INTLEL_V      (GPIO_BASE_V + 0x0A0)
#define    GPIO_PORTE_INTPOL_V      (GPIO_BASE_V + 0x0A4)
#define    GPIO_PORTE_INTCLR_V      (GPIO_BASE_V + 0x0A8)
#define    GPIO_PORTE_SECFUN_V      (GPIO_BASE_V + 0x0AC)

#define    GPIO_PORTF_DIR_V         (GPIO_BASE_V + 0x0B0)
#define    GPIO_PORTF_SEL_V         (GPIO_BASE_V + 0x0B4)
#define    GPIO_PORTF_DATA_V        (GPIO_BASE_V + 0x0B8)
#define    GPIO_PORTF_INTSEL_V      (GPIO_BASE_V + 0x0BC)
#define    GPIO_PORTF_INTLEL_V      (GPIO_BASE_V + 0x0C0)
#define    GPIO_PORTF_INTPOL_V      (GPIO_BASE_V + 0x0C4)
#define    GPIO_PORTF_INTCLR_V      (GPIO_BASE_V + 0x0C8)
#define    GPIO_PORTF_SECFUN_V      (GPIO_BASE_V + 0x0CC)

#define    GPIO_PORTG_DIR_V         (GPIO_BASE_V + 0x0D0)
#define    GPIO_PORTG_SEL_V         (GPIO_BASE_V + 0x0D4)
#define    GPIO_PORTG_DATA_V        (GPIO_BASE_V + 0x0D8)
#define    GPIO_PORTG_INTSEL_V      (GPIO_BASE_V + 0x0DC)
#define    GPIO_PORTG_INTLEL_V      (GPIO_BASE_V + 0x0E0)
#define    GPIO_PORTG_INTPOL_V      (GPIO_BASE_V + 0x0E4)
#define    GPIO_PORTG_INTCLR_V      (GPIO_BASE_V + 0x0E8)
#define    GPIO_PORTG_SECFUN_V      (GPIO_BASE_V + 0x0EC)

#define    GPIO_PORTH_DIR_V         (GPIO_BASE_V + 0x0F0)
#define    GPIO_PORTH_SEL_V         (GPIO_BASE_V + 0x0F4)
#define    GPIO_PORTH_DATA_V        (GPIO_BASE_V + 0x0F8)
#define    GPIO_PORTH_INTSEL_V      (GPIO_BASE_V + 0x0FC)
#define    GPIO_PORTH_INTLEL_V      (GPIO_BASE_V + 0x100)
#define    GPIO_PORTH_INTPOL_V      (GPIO_BASE_V + 0x104)
#define    GPIO_PORTH_INTCLR_V      (GPIO_BASE_V + 0x108)
#define    GPIO_PORTH_SECFUN_V      (GPIO_BASE_V + 0x10C)

#define    GPIO_PORTI_DIR_V         (GPIO_BASE_V + 0x110)
#define    GPIO_PORTI_SEL_V         (GPIO_BASE_V + 0x114)
#define    GPIO_PORTI_DATA_V        (GPIO_BASE_V + 0x118)
#define    GPIO_PORTI_INTRSEL_V     (GPIO_BASE_V + 0x11C)
#define    GPIO_PORTI_INTRLEL_V     (GPIO_BASE_V + 0x120)
#define    GPIO_PORTI_INTRPOL_V     (GPIO_BASE_V + 0x124)
#define    GPIO_PORTI_INTRCLR_V     (GPIO_BASE_V + 0x128)
#define    GPIO_PORTI_SECFUN_V      (GPIO_BASE_V + 0x12C) 

#ifdef SYSCTRL_BASE

#define    SYS_CTRL					(SYSCTRL_BASE + 0x00)
#define    REVERSION				(SYSCTRL_BASE + 0x04)
#define    DDR_PHY_CTRL				(SYSCTRL_BASE + 0x10)
#define    DDR_LPWR_CTRL			(SYSCTRL_BASE + 0x14)
#define    DDR_SSTL_SHUTDOWN		(SYSCTRL_BASE + 0x18)
#define    DDR_INIT_WAIT_CYCLE		(SYSCTRL_BASE + 0x1C)

#endif

#ifdef SYSCTRL_BASE_V

#define    SYS_CTRL_V				(SYSCTRL_BASE_V + 0x00)
#define    REVERSION_V				(SYSCTRL_BASE_V + 0x04)
#define    DDR_PHY_CTRL_V			(SYSCTRL_BASE_V + 0x10)
#define    DDR_LPWR_CTRL_V			(SYSCTRL_BASE_V + 0x14)
#define    DDR_SSTL_SHUTDOWN_V		(SYSCTRL_BASE_V + 0x18)
#define    DDR_INIT_WAIT_CYCLE_V	(SYSCTRL_BASE_V + 0x1C)

#endif

// SSI1 register list
// SSI1_BASE 0xB1004000
#ifdef SSI1_BASE

#define    SSI1_CONTROL0            (SSI1_BASE + 0x000)
#define    SSI1_CONTROL1            (SSI1_BASE + 0x004)
#define    SSI1_SSI1ENR             (SSI1_BASE + 0x008)
#define    SSI1_MWCR                (SSI1_BASE + 0x00C)
#define    SSI1_SER                 (SSI1_BASE + 0x010)
#define    SSI1_BAUDR               (SSI1_BASE + 0x014)
#define    SSI1_TXFTLR              (SSI1_BASE + 0x018)
#define    SSI1_RXFTLR              (SSI1_BASE + 0x01C)
#define    SSI1_TXFLR               (SSI1_BASE + 0x020)
#define    SSI1_RXFLR               (SSI1_BASE + 0x024)
#define    SSI1_SR                  (SSI1_BASE + 0x028)
#define    SSI1_IMR                 (SSI1_BASE + 0x02C)
#define    SSI1_ISR                 (SSI1_BASE + 0x030)
#define    SSI1_RISR                (SSI1_BASE + 0x034)
#define    SSI1_TXOICR              (SSI1_BASE + 0x038)
#define    SSI1_RXOICR              (SSI1_BASE + 0x03C)
#define    SSI1_RXUICR              (SSI1_BASE + 0x040)
#define    SSI1_ICR                 (SSI1_BASE + 0x048)
#define    SSI1_DMACR               (SSI1_BASE + 0x04C)
#define    SSI1_DMATDLR             (SSI1_BASE + 0x050)
#define    SSI1_DMARDLR             (SSI1_BASE + 0x054)
#define    SSI1_DR                  (SSI1_BASE + 0x060)

#endif // SSI1_BASE

// SSI2 register list
// SSI2_BASE 0xB1005000
#ifdef SSI2_BASE

#define    SSI2_CONTROL0            (SSI2_BASE + 0x000)
#define    SSI2_CONTROL1            (SSI2_BASE + 0x004)
#define    SSI2_SSI2ENR             (SSI2_BASE + 0x008)
#define    SSI2_MWCR                (SSI2_BASE + 0x00C)
#define    SSI2_SER                 (SSI2_BASE + 0x010)
#define    SSI2_BAUDR               (SSI2_BASE + 0x014)
#define    SSI2_TXFTLR              (SSI2_BASE + 0x018)
#define    SSI2_RXFTLR              (SSI2_BASE + 0x01C)
#define    SSI2_TXFLR               (SSI2_BASE + 0x020)
#define    SSI2_RXFLR               (SSI2_BASE + 0x024)
#define    SSI2_SR                  (SSI2_BASE + 0x028)
#define    SSI2_IMR                 (SSI2_BASE + 0x02C)
#define    SSI2_ISR                 (SSI2_BASE + 0x030)
#define    SSI2_RISR                (SSI2_BASE + 0x034)
#define    SSI2_TXOICR              (SSI2_BASE + 0x038)
#define    SSI2_RXOICR              (SSI2_BASE + 0x03C)
#define    SSI2_RXUICR              (SSI2_BASE + 0x040)
#define    SSI2_ICR                 (SSI2_BASE + 0x048)
#define    SSI2_DMACR               (SSI2_BASE + 0x04C)
#define    SSI2_DMATDLR             (SSI2_BASE + 0x050)
#define    SSI2_DMARDLR             (SSI2_BASE + 0x054)
#define    SSI2_DR                  (SSI2_BASE + 0x060)

#endif // SSI2_BASE

// SSI3 register list
// SSI3_BASE 0x31006000
#ifdef SSI3_BASE

#define    SSI3_CONTROL0            (SSI3_BASE + 0x000)
#define    SSI3_CONTROL1            (SSI3_BASE + 0x004)
#define    SSI3_SSI3ENR             (SSI3_BASE + 0x008)
#define    SSI3_MWCR                (SSI3_BASE + 0x00C)
#define    SSI3_SER                 (SSI3_BASE + 0x010)
#define    SSI3_BAUDR               (SSI3_BASE + 0x014)
#define    SSI3_TXFTLR              (SSI3_BASE + 0x018)
#define    SSI3_RXFTLR              (SSI3_BASE + 0x01C)
#define    SSI3_TXFLR               (SSI3_BASE + 0x020)
#define    SSI3_RXFLR               (SSI3_BASE + 0x024)
#define    SSI3_SR                  (SSI3_BASE + 0x028)
#define    SSI3_IMR                 (SSI3_BASE + 0x02C)
#define    SSI3_ISR                 (SSI3_BASE + 0x030)
#define    SSI3_RISR                (SSI3_BASE + 0x034)
#define    SSI3_TXOICR              (SSI3_BASE + 0x038)
#define    SSI3_RXOICR              (SSI3_BASE + 0x03C)
#define    SSI3_RXUICR              (SSI3_BASE + 0x040)
#define    SSI3_ICR                 (SSI3_BASE + 0x048)
#define    SSI3_DMACR               (SSI3_BASE + 0x04C)
#define    SSI3_DMATDLR             (SSI3_BASE + 0x050)
#define    SSI3_DMARDLR             (SSI3_BASE + 0x054)
#define    SSI3_DR                  (SSI3_BASE + 0x060)

#endif // SSI3_BASE
// SPI's virtual address is Dynamicly requsting


// I2C1 register list
// I2C1_BASE 0x31006000
#ifdef I2C1_BASE

#define    I2C1_CON                 (I2C1_BASE + 0x000)
#define    I2C1_TAR                 (I2C1_BASE + 0x004)
#define    I2C1_SAR                 (I2C1_BASE + 0x008)
#define    I2C1_HS_MADDR            (I2C1_BASE + 0x00C)
#define    I2C1_DATA_CMD            (I2C1_BASE + 0x010)
#define    I2C1_SS_SCL_HCNT         (I2C1_BASE + 0x014)
#define    I2C1_SS_SCL_LCNT         (I2C1_BASE + 0x018)
#define    I2C1_FS_SCL_HCNT         (I2C1_BASE + 0x01C)
#define    I2C1_FS_SCL_LCNT         (I2C1_BASE + 0x020)
#define    I2C1_HS_SCL_HCNT         (I2C1_BASE + 0x024)
#define    I2C1_HS_SCL_LCNT         (I2C1_BASE + 0x028)
#define    I2C1_INTR_STAR           (I2C1_BASE + 0x02C)
#define    I2C1_INTR_MASK           (I2C1_BASE + 0x030)
#define    I2C1_RAW_INTR_STAR       (I2C1_BASE + 0x034)
#define    I2C1_RX_TL               (I2C1_BASE + 0x038)
#define    I2C1_TX_TL               (I2C1_BASE + 0x03C)
#define    I2C1_CLR_INTR            (I2C1_BASE + 0x040)
#define    I2C1_CLR_RX_UNDER        (I2C1_BASE + 0x044)
#define    I2C1_CLR_RX_OVER         (I2C1_BASE + 0x048)
#define    I2C1_CLR_TX_OVER         (I2C1_BASE + 0x04C)
#define    I2C1_CLR_RD_REQ          (I2C1_BASE + 0x050)
#define    I2C1_CLR_TX_ABRT         (I2C1_BASE + 0x054)
#define    I2C1_CLR_RX_DONE         (I2C1_BASE + 0x058)
#define    I2C1_CLR_ACTIVITY        (I2C1_BASE + 0x05C)
#define    I2C1_CLR_STOP_DET        (I2C1_BASE + 0x060)
#define    I2C1_CLR_START_DET       (I2C1_BASE + 0x064)
#define    I2C1_CLR_GEN_CALL        (I2C1_BASE + 0x068)
#define    I2C1_ENABLE              (I2C1_BASE + 0x06C)
#define    I2C1_STATUS              (I2C1_BASE + 0x070)
#define    I2C1_TXFLR               (I2C1_BASE + 0x074)
#define    I2C1_RXFLR               (I2C1_BASE + 0x078)
#define    I2C1_TX_ABRT_SRC         (I2C1_BASE + 0x080)
#define    I2C1_DMA_CR              (I2C1_BASE + 0x088)
#define    I2C1_DMA_TDLR            (I2C1_BASE + 0x08C)
#define    I2C1_DMA_RDLR            (I2C1_BASE + 0x090)
#define    I2C1_COMP_PARAM1         (I2C1_BASE + 0x0F4)
#define    I2C1_COMP_VISION         (I2C1_BASE + 0x0F8)
#define    I2C1_COMP_TYPE           (I2C1_BASE + 0x0FC)

#endif // I2C1_BASE

// I2C2 register list
// I2C2_BASE 0x31006000
#ifdef I2C2_BASE

#define    I2C2_CON                 (I2C2_BASE + 0x000)
#define    I2C2_TAR                 (I2C2_BASE + 0x004)
#define    I2C2_SAR                 (I2C2_BASE + 0x008)
#define    I2C2_HS_MADDR            (I2C2_BASE + 0x00C)
#define    I2C2_DATA_CMD            (I2C2_BASE + 0x010)
#define    I2C2_SS_SCL_HCNT         (I2C2_BASE + 0x014)
#define    I2C2_SS_SCL_LCNT         (I2C2_BASE + 0x018)
#define    I2C2_FS_SCL_HCNT         (I2C2_BASE + 0x01C)
#define    I2C2_FS_SCL_LCNT         (I2C2_BASE + 0x020)
#define    I2C2_HS_SCL_HCNT         (I2C2_BASE + 0x024)
#define    I2C2_HS_SCL_LCNT         (I2C2_BASE + 0x028)
#define    I2C2_INTR_STAR           (I2C2_BASE + 0x02C)
#define    I2C2_INTR_MASK           (I2C2_BASE + 0x030)
#define    I2C2_RAW_INTR_STAR       (I2C2_BASE + 0x034)
#define    I2C2_RX_TL               (I2C2_BASE + 0x038)
#define    I2C2_TX_TL               (I2C2_BASE + 0x03C)
#define    I2C2_CLR_INTR            (I2C2_BASE + 0x040)
#define    I2C2_CLR_RX_UNDER        (I2C2_BASE + 0x044)
#define    I2C2_CLR_RX_OVER         (I2C2_BASE + 0x048)
#define    I2C2_CLR_TX_OVER         (I2C2_BASE + 0x04C)
#define    I2C2_CLR_RD_REQ          (I2C2_BASE + 0x050)
#define    I2C2_CLR_TX_ABRT         (I2C2_BASE + 0x054)
#define    I2C2_CLR_RX_DONE         (I2C2_BASE + 0x058)
#define    I2C2_CLR_ACTIVITY        (I2C2_BASE + 0x05C)
#define    I2C2_CLR_STOP_DET        (I2C2_BASE + 0x060)
#define    I2C2_CLR_START_DET       (I2C2_BASE + 0x064)
#define    I2C2_CLR_GEN_CALL        (I2C2_BASE + 0x068)
#define    I2C2_ENABLE              (I2C2_BASE + 0x06C)
#define    I2C2_STATUS              (I2C2_BASE + 0x070)
#define    I2C2_TXFLR               (I2C2_BASE + 0x074)
#define    I2C2_RXFLR               (I2C2_BASE + 0x078)
#define    I2C2_TX_ABRT_SRC         (I2C2_BASE + 0x080)
#define    I2C2_DMA_CR              (I2C2_BASE + 0x088)
#define    I2C2_DMA_TDLR            (I2C2_BASE + 0x08C)
#define    I2C2_DMA_RDLR            (I2C2_BASE + 0x090)
#define    I2C2_COMP_PARAM1         (I2C2_BASE + 0x0F4)
#define    I2C2_COMP_VISION         (I2C2_BASE + 0x0F8)
#define    I2C2_COMP_TYPE           (I2C2_BASE + 0x0FC)

#endif // I2C2_BASE

// I2C3 register list
// I2C3_BASE 0x31006000
#ifdef I2C3_BASE

#define    I2C3_CON                 (I2C3_BASE + 0x000)
#define    I2C3_TAR                 (I2C3_BASE + 0x004)
#define    I2C3_SAR                 (I2C3_BASE + 0x008)
#define    I2C3_HS_MADDR            (I2C3_BASE + 0x00C)
#define    I2C3_DATA_CMD            (I2C3_BASE + 0x010)
#define    I2C3_SS_SCL_HCNT         (I2C3_BASE + 0x014)
#define    I2C3_SS_SCL_LCNT         (I2C3_BASE + 0x018)
#define    I2C3_FS_SCL_HCNT         (I2C3_BASE + 0x01C)
#define    I2C3_FS_SCL_LCNT         (I2C3_BASE + 0x020)
#define    I2C3_HS_SCL_HCNT         (I2C3_BASE + 0x024)
#define    I2C3_HS_SCL_LCNT         (I2C3_BASE + 0x028)
#define    I2C3_INTR_STAR           (I2C3_BASE + 0x02C)
#define    I2C3_INTR_MASK           (I2C3_BASE + 0x030)
#define    I2C3_RAW_INTR_STAR       (I2C3_BASE + 0x034)
#define    I2C3_RX_TL               (I2C3_BASE + 0x038)
#define    I2C3_TX_TL               (I2C3_BASE + 0x03C)
#define    I2C3_CLR_INTR            (I2C3_BASE + 0x040)
#define    I2C3_CLR_RX_UNDER        (I2C3_BASE + 0x044)
#define    I2C3_CLR_RX_OVER         (I2C3_BASE + 0x048)
#define    I2C3_CLR_TX_OVER         (I2C3_BASE + 0x04C)
#define    I2C3_CLR_RD_REQ          (I2C3_BASE + 0x050)
#define    I2C3_CLR_TX_ABRT         (I2C3_BASE + 0x054)
#define    I2C3_CLR_RX_DONE         (I2C3_BASE + 0x058)
#define    I2C3_CLR_ACTIVITY        (I2C3_BASE + 0x05C)
#define    I2C3_CLR_STOP_DET        (I2C3_BASE + 0x060)
#define    I2C3_CLR_START_DET       (I2C3_BASE + 0x064)
#define    I2C3_CLR_GEN_CALL        (I2C3_BASE + 0x068)
#define    I2C3_ENABLE              (I2C3_BASE + 0x06C)
#define    I2C3_STATUS              (I2C3_BASE + 0x070)
#define    I2C3_TXFLR               (I2C3_BASE + 0x074)
#define    I2C3_RXFLR               (I2C3_BASE + 0x078)
#define    I2C3_TX_ABRT_SRC         (I2C3_BASE + 0x080)
#define    I2C3_DMA_CR              (I2C3_BASE + 0x088)
#define    I2C3_DMA_TDLR            (I2C3_BASE + 0x08C)
#define    I2C3_DMA_RDLR            (I2C3_BASE + 0x090)
#define    I2C3_COMP_PARAM1         (I2C3_BASE + 0x0F4)
#define    I2C3_COMP_VISION         (I2C3_BASE + 0x0F8)
#define    I2C3_COMP_TYPE           (I2C3_BASE + 0x0FC)

#endif //I2C3_BASE
// 清补充虚拟地址

// 请补充虚拟地址

// NAND Controller register list 物理地址
// NAND_BASE 0xB2021000
#ifdef NAND_BASE

#define    NAND_CFG                 (NAND_BASE + 0x000)
#define    NAND_CMD                 (NAND_BASE + 0x004)
#define    NAND_ADDR                (NAND_BASE + 0x008)
#define    NAND_SDATA               (NAND_BASE + 0x00C)
#define    NAND_CTRL                (NAND_BASE + 0x010)
#define    NAND_INIT                (NAND_BASE + 0x014)
#define    NAND_DMA                 (NAND_BASE + 0x018)
#define    NAND_STATUS              (NAND_BASE + 0x01C)
#define    NAND_ECC_PARITY0         (NAND_BASE + 0x020)
#define    NAND_ECC_PARITY1         (NAND_BASE + 0x024)
#define    NAND_ECC_PARITY2         (NAND_BASE + 0x028)
#define    NAND_ECC_PARITY3         (NAND_BASE + 0x02C)
#define    NAND_ECC_PARITY4         (NAND_BASE + 0x030)
#define    NAND_ECC_PARITY5         (NAND_BASE + 0x034)
#define    NAND_ECC_PARITY6         (NAND_BASE + 0x038)
#define    NAND_ECC_PARITY7         (NAND_BASE + 0x03C)
#define    NAND_ECC_PARITY8         (NAND_BASE + 0x040)
#define    NAND_ECC_PARITY9         (NAND_BASE + 0x044)
#define    NAND_ECC_PARITY10        (NAND_BASE + 0x048)
#define    NAND_ECC_PARITY11        (NAND_BASE + 0x04C)
#define    NAND_ECC_PARITY12        (NAND_BASE + 0x050)
#define    NAND_ECC_PARITY13        (NAND_BASE + 0x054)
#define    NAND_ERR_ADDR0           (NAND_BASE + 0x058)
#define    NAND_ERR_ADDR1           (NAND_BASE + 0x05C)
#define    NAND_ERR_ADDR2           (NAND_BASE + 0x060)
#define    NAND_ERR_ADDR3           (NAND_BASE + 0x064)
#define    NAND_ERR_ADDR4           (NAND_BASE + 0x068)
#define    NAND_ERR_ADDR5           (NAND_BASE + 0x06C)
#define    NAND_ERR_ADDR6           (NAND_BASE + 0x070)
#define    NAND_ERR_ADDR7           (NAND_BASE + 0x074)
#define    NAND_ERR_ADDR8           (NAND_BASE + 0x078)
#define    NAND_ERR_ADDR9           (NAND_BASE + 0x07C)
#define    NAND_ERR_ADDR10          (NAND_BASE + 0x080)
#define    NAND_ERR_ADDR11          (NAND_BASE + 0x084)
#define    NAND_ERR_ADDR12          (NAND_BASE + 0x088)
#define    NAND_ERR_ADDR13          (NAND_BASE + 0x08C)
#define    NAND_ERR_ADDR14          (NAND_BASE + 0x090)
#define    NAND_ERR_ADDR15          (NAND_BASE + 0x094)
#define    NAND_ERR_ADDR16          (NAND_BASE + 0x098)
#define    NAND_ERR_ADDR17          (NAND_BASE + 0x09C)
#define    NAND_ERR_ADDR18          (NAND_BASE + 0x0A0)
#define    NAND_ERR_ADDR19          (NAND_BASE + 0x0A4)
#define    NAND_ERR_ADDR20          (NAND_BASE + 0x0A8)
#define    NAND_ERR_ADDR21          (NAND_BASE + 0x0AC)
#define    NAND_ERR_ADDR22          (NAND_BASE + 0x0B0)
#define    NAND_ERR_ADDR23          (NAND_BASE + 0x0B4)
#define    NAND_ERR_ADDR24          (NAND_BASE + 0x0B8)
#define    NAND_ERR_ADDR25          (NAND_BASE + 0x0BC)
#define    NAND_ERR_ADDR26          (NAND_BASE + 0x0C0)
#define    NAND_ERR_ADDR27          (NAND_BASE + 0x0C4)
#define    NAND_ERR_ADDR28          (NAND_BASE + 0x0C8)
#define    NAND_ERR_ADDR29          (NAND_BASE + 0x0CC)
#define    NAND_FDATA               (NAND_BASE + 0x100)

#endif
//
// NAND Controller register list 虚拟地址
// NAND_BASE 0x32021000
#ifdef NAND_BASE_V

#define    NAND_CFG_V             	(NAND_BASE_V + 0x000)
#define    NAND_CMD_V               (NAND_BASE_V + 0x004)
#define    NAND_ADDR_V              (NAND_BASE_V + 0x008)
#define    NAND_SDATA_V             (NAND_BASE_V + 0x00C)
#define    NAND_CTRL_V              (NAND_BASE_V + 0x010)
#define    NAND_INIT_V              (NAND_BASE_V + 0x014)
#define    NAND_DMA_V               (NAND_BASE_V + 0x018)
#define    NAND_STATUS_V            (NAND_BASE_V + 0x01C)
#define    NAND_ECC_PARITY0_V       (NAND_BASE_V + 0x020)
#define    NAND_ECC_PARITY1_V       (NAND_BASE_V + 0x024)
#define    NAND_ECC_PARITY2_V       (NAND_BASE_V + 0x028)
#define    NAND_ECC_PARITY3_V       (NAND_BASE_V + 0x02C)
#define    NAND_ECC_PARITY4_V       (NAND_BASE_V + 0x030)
#define    NAND_ECC_PARITY5_V       (NAND_BASE_V + 0x034)
#define    NAND_ECC_PARITY6_V       (NAND_BASE_V + 0x038)
#define    NAND_ECC_PARITY7_V       (NAND_BASE_V + 0x03C)
#define    NAND_ECC_PARITY8_V       (NAND_BASE_V + 0x040)
#define    NAND_ECC_PARITY9_V       (NAND_BASE_V + 0x044)
#define    NAND_ECC_PARITY10_V      (NAND_BASE_V + 0x048)
#define    NAND_ECC_PARITY11_V      (NAND_BASE_V + 0x04C)
#define    NAND_ECC_PARITY12_V      (NAND_BASE_V + 0x050)
#define    NAND_ECC_PARITY13_V      (NAND_BASE_V + 0x054)
#define    NAND_ERR_ADDR0_V         (NAND_BASE_V + 0x058)
#define    NAND_ERR_ADDR1_V         (NAND_BASE_V + 0x05C)
#define    NAND_ERR_ADDR2_V         (NAND_BASE_V + 0x060)
#define    NAND_ERR_ADDR3_V         (NAND_BASE_V + 0x064)
#define    NAND_ERR_ADDR4_V         (NAND_BASE_V + 0x068)
#define    NAND_ERR_ADDR5_V         (NAND_BASE_V + 0x06C)
#define    NAND_ERR_ADDR6_V         (NAND_BASE_V + 0x070)
#define    NAND_ERR_ADDR7_V         (NAND_BASE_V + 0x074)
#define    NAND_ERR_ADDR8_V         (NAND_BASE_V + 0x078)
#define    NAND_ERR_ADDR9_V         (NAND_BASE_V + 0x07C)
#define    NAND_ERR_ADDR10_V        (NAND_BASE_V + 0x080)
#define    NAND_ERR_ADDR11_V        (NAND_BASE_V + 0x084)
#define    NAND_ERR_ADDR12_V        (NAND_BASE_V + 0x088)
#define    NAND_ERR_ADDR13_V        (NAND_BASE_V + 0x08C)
#define    NAND_ERR_ADDR14_V        (NAND_BASE_V + 0x090)
#define    NAND_ERR_ADDR15_V        (NAND_BASE_V + 0x094)
#define    NAND_ERR_ADDR16_V        (NAND_BASE_V + 0x098)
#define    NAND_ERR_ADDR17_V        (NAND_BASE_V + 0x09C)
#define    NAND_ERR_ADDR18_V        (NAND_BASE_V + 0x0A0)
#define    NAND_ERR_ADDR19_V        (NAND_BASE_V + 0x0A4)
#define    NAND_ERR_ADDR20_V        (NAND_BASE_V + 0x0A8)
#define    NAND_ERR_ADDR21_V        (NAND_BASE_V + 0x0AC)
#define    NAND_ERR_ADDR22_V        (NAND_BASE_V + 0x0B0)
#define    NAND_ERR_ADDR23_V        (NAND_BASE_V + 0x0B4)
#define    NAND_ERR_ADDR24_V        (NAND_BASE_V + 0x0B8)
#define    NAND_ERR_ADDR25_V        (NAND_BASE_V + 0x0BC)
#define    NAND_ERR_ADDR26_V        (NAND_BASE_V + 0x0C0)
#define    NAND_ERR_ADDR27_V        (NAND_BASE_V + 0x0C4)
#define    NAND_ERR_ADDR28_V        (NAND_BASE_V + 0x0C8)
#define    NAND_ERR_ADDR29_V        (NAND_BASE_V + 0x0CC)
#define    NAND_FDATA_V             (NAND_BASE_V + 0x100)

#endif


// SDIO1 register list 虚拟地址
// SDIO1_BASE 0x32012000
#ifdef SDIO1_BASE_V

#define    SDIO1_CTRL_V             (SDIO1_BASE_V + 0x000)
#define    SDIO1_PWREN_V            (SDIO1_BASE_V + 0x004)
#define    SDIO1_CLKDIV_V           (SDIO1_BASE_V + 0x008)
#define    SDIO1_CLKSRC_V           (SDIO1_BASE_V + 0x00C)
#define    SDIO1_CLKENA_V           (SDIO1_BASE_V + 0x010)
#define    SDIO1_TMOUT_V            (SDIO1_BASE_V + 0x014)
#define    SDIO1_CTYPE_V            (SDIO1_BASE_V + 0x018)
#define    SDIO1_BLKSIZ_V           (SDIO1_BASE_V + 0x01C)
#define    SDIO1_BYTCNT_V           (SDIO1_BASE_V + 0x020)
#define    SDIO1_INTMASK_V          (SDIO1_BASE_V + 0x024)
#define    SDIO1_CMDARG_V           (SDIO1_BASE_V + 0x028)
#define    SDIO1_CMD_V              (SDIO1_BASE_V + 0x02C)
#define    SDIO1_RESP0_V            (SDIO1_BASE_V + 0x030)
#define    SDIO1_RESP1_V            (SDIO1_BASE_V + 0x034)
#define    SDIO1_RESP2_V            (SDIO1_BASE_V + 0x038)
#define    SDIO1_RESP3_V            (SDIO1_BASE_V + 0x03C)
#define    SDIO1_MINTSTS_V          (SDIO1_BASE_V + 0x040)
#define    SDIO1_RINTSTS_V          (SDIO1_BASE_V + 0x044)
#define    SDIO1_STATUS_V           (SDIO1_BASE_V + 0x048)
#define    SDIO1_FIFOTH_V           (SDIO1_BASE_V + 0x04C)
#define    SDIO1_CDETECT_V          (SDIO1_BASE_V + 0x050)
#define    SDIO1_WRTPRT_V           (SDIO1_BASE_V + 0x054)
#define    SDIO1_GPIO_V             (SDIO1_BASE_V + 0x058)
#define    SDIO1_TCBCNT_V           (SDIO1_BASE_V + 0x05C)
#define    SDIO1_TBBCNT_V           (SDIO1_BASE_V + 0x060)
#define    SDIO1_DEBNCE_V           (SDIO1_BASE_V + 0x064)
#define    SDIO1_USRID_V            (SDIO1_BASE_V + 0x068)
#define    SDIO1_VERID_V            (SDIO1_BASE_V + 0x06C)
#define    SDIO1_HCON_V             (SDIO1_BASE_V + 0x070)
#define    SDIO1_APBCLKDIV_V        (SDIO1_BASE_V + 0x074)
#define    SDIO1_BMOD_V             (SDIO1_BASE_V + 0x080)
#define    SDIO1_PLDMND_V           (SDIO1_BASE_V + 0x084)
#define    SDIO1_DBADDR_V           (SDIO1_BASE_V + 0x088)
#define    SDIO1_IDSTS_V            (SDIO1_BASE_V + 0x08C)
#define    SDIO1_IDINTEN_V          (SDIO1_BASE_V + 0x090)
#define    SDIO1_DSCADDR_V          (SDIO1_BASE_V + 0x094)
#define    SDIO1_BUFADDR_V          (SDIO1_BASE_V  + 0x098)
#define    SDIO1_AHBCLKDIV_V        (SDIO1_BASE_V  + 0x0A0)
#define    SDIO1_DATA_V             (SDIO1_BASE_V  + 0x100)
#define    SDIO1_DATA_BE_V          (SDIO1_BASE_V  + 0x800)
#endif // SDIO1_BASE_V

// SDIO2 register list 虚拟地址

#ifdef SDIO2_BASE_V

#define    SDIO2_CTRL_V             (SDIO2_BASE_V + 0x000)
#define    SDIO2_PWREN_V            (SDIO2_BASE_V + 0x004)
#define    SDIO2_CLKDIV_V           (SDIO2_BASE_V + 0x008)
#define    SDIO2_CLKSRC_V           (SDIO2_BASE_V + 0x00C)
#define    SDIO2_CLKENA_V           (SDIO2_BASE_V + 0x010)
#define    SDIO2_TMOUT_V            (SDIO2_BASE_V + 0x014)
#define    SDIO2_CTYPE_V            (SDIO2_BASE_V + 0x018)
#define    SDIO2_BLKSIZ_V           (SDIO2_BASE_V + 0x01C)
#define    SDIO2_BYTCNT_V           (SDIO2_BASE_V + 0x020)
#define    SDIO2_INTMASK_V          (SDIO2_BASE_V + 0x024)
#define    SDIO2_CMDARG_V           (SDIO2_BASE_V + 0x028)
#define    SDIO2_CMD_V              (SDIO2_BASE_V + 0x02C)
#define    SDIO2_RESP0_V            (SDIO2_BASE_V + 0x030)
#define    SDIO2_RESP1_V            (SDIO2_BASE_V + 0x034)
#define    SDIO2_RESP2_V            (SDIO2_BASE_V + 0x038)
#define    SDIO2_RESP3_V            (SDIO2_BASE_V + 0x03C)
#define    SDIO2_MINTSTS_V          (SDIO2_BASE_V + 0x040)
#define    SDIO2_RINTSTS_V          (SDIO2_BASE_V + 0x044)
#define    SDIO2_STATUS_V           (SDIO2_BASE_V + 0x048)
#define    SDIO2_FIFOTH_V           (SDIO2_BASE_V + 0x04C)
#define    SDIO2_CDETECT_V          (SDIO2_BASE_V + 0x050)
#define    SDIO2_WRTPRT_V           (SDIO2_BASE_V + 0x054)
#define    SDIO2_GPIO_V             (SDIO2_BASE_V + 0x058)
#define    SDIO2_TCBCNT_V           (SDIO2_BASE_V + 0x05C)
#define    SDIO2_TBBCNT_V           (SDIO2_BASE_V + 0x060)
#define    SDIO2_DEBNCE_V           (SDIO2_BASE_V + 0x064)
#define    SDIO2_USRID_V            (SDIO2_BASE_V + 0x068)
#define    SDIO2_VERID_V            (SDIO2_BASE_V + 0x06C)
#define    SDIO2_HCON_V             (SDIO2_BASE_V + 0x070)
#define    SDIO2_APBCLKDIV_V        (SDIO2_BASE_V + 0x074)
#define    SDIO2_BMOD_V             (SDIO2_BASE_V + 0x080)
#define    SDIO2_PLDMND_V           (SDIO2_BASE_V + 0x084)
#define    SDIO2_DBADDR_V           (SDIO2_BASE_V + 0x088)
#define    SDIO2_IDSTS_V            (SDIO2_BASE_V + 0x08C)
#define    SDIO2_IDINTEN_V          (SDIO2_BASE_V + 0x090)
#define    SDIO2_DSCADDR_V          (SDIO2_BASE_V + 0x094)
#define    SDIO2_BUFADDR_V          (SDIO2_BASE_V  + 0x098)
#define    SDIO2_AHBCLKDIV_V        (SDIO2_BASE_V  + 0x0A0)
#define    SDIO2_DATA_V             (SDIO2_BASE_V  + 0x100)
#define    SDIO2_DATA_BE_V          (SDIO2_BASE_V  + 0x800)#endif // SDIO2_BASE

#ifdef DMAC1_BASE
// DMAC1_BASE 32027000
// DMAC1 channel 0
#define    DMAC1_SAR0               (DMAC1_BASE + 0x000)
#define    DMAC1_DAR0               (DMAC1_BASE + 0x008)
#define    DMAC1_LLP0               (DMAC1_BASE + 0x010)
#define    DMAC1_CTL0               (DMAC1_BASE + 0x018)
#define    DMAC1_SSTAT0             (DMAC1_BASE + 0x020)
#define    DMAC1_DSTAT0             (DMAC1_BASE + 0x028)
#define    DMAC1_SSTATR0            (DMAC1_BASE + 0x030)
#define    DMAC1_DSTATR0            (DMAC1_BASE + 0x038)
#define    DMAC1_CFG0               (DMAC1_BASE + 0x040)
#define    DMAC1_SGR0               (DMAC1_BASE + 0x048)
#define    DMAC1_DSR0               (DMAC1_BASE + 0x050)

// DMAC1 channel 1
#define    DMAC1_SAR1               (DMAC1_BASE + 0x058)
#define    DMAC1_DAR1               (DMAC1_BASE + 0x060)
#define    DMAC1_LLP1               (DMAC1_BASE + 0x068)
#define    DMAC1_CTL1               (DMAC1_BASE + 0x070)
#define    DMAC1_SSTAT1             (DMAC1_BASE + 0x078)
#define    DMAC1_DSTAT1             (DMAC1_BASE + 0x080)
#define    DMAC1_SSTATR1            (DMAC1_BASE + 0x088)
#define    DMAC1_DSTATR1            (DMAC1_BASE + 0x090)
#define    DMAC1_CFG1               (DMAC1_BASE + 0x098)
#define    DMAC1_SGR1               (DMAC1_BASE + 0x0A0)
#define    DMAC1_DSR1               (DMAC1_BASE + 0x0A8)

// DMAC1 channel 2
#define    DMAC1_SAR2               (DMAC1_BASE + 0x0B0)
#define    DMAC1_DAR2               (DMAC1_BASE + 0x0B8)
#define    DMAC1_LLP2               (DMAC1_BASE + 0x0C0)
#define    DMAC1_CTL2               (DMAC1_BASE + 0x0C8)
#define    DMAC1_SSTAT2             (DMAC1_BASE + 0x0D0)
#define    DMAC1_DSTAT2             (DMAC1_BASE + 0x0D8)
#define    DMAC1_SSTATR2            (DMAC1_BASE + 0x0E0)
#define    DMAC1_DSTATR2            (DMAC1_BASE + 0x0E8)
#define    DMAC1_CFG2               (DMAC1_BASE + 0x0F0)
#define    DMAC1_SGR2               (DMAC1_BASE + 0x0F8)
#define    DMAC1_DSR2               (DMAC1_BASE + 0x100)

// DMAC1 channel 3
#define    DMAC1_SAR3               (DMAC1_BASE + 0x108)
#define    DMAC1_DAR3               (DMAC1_BASE + 0x110)
#define    DMAC1_LLP3               (DMAC1_BASE + 0x118)
#define    DMAC1_CTL3               (DMAC1_BASE + 0x120)
#define    DMAC1_SSTAT3             (DMAC1_BASE + 0x128)
#define    DMAC1_DSTAT3             (DMAC1_BASE + 0x130)
#define    DMAC1_SSTATR3            (DMAC1_BASE + 0x138)
#define    DMAC1_DSTATR3            (DMAC1_BASE + 0x140)
#define    DMAC1_CFG3               (DMAC1_BASE + 0x148)
#define    DMAC1_SGR3               (DMAC1_BASE + 0x150)
#define    DMAC1_DSR3               (DMAC1_BASE + 0x158)

// DMAC1 channel 4
#define    DMAC1_SAR4               (DMAC1_BASE + 0x160)
#define    DMAC1_DAR4               (DMAC1_BASE + 0x168)
#define    DMAC1_LLP4               (DMAC1_BASE + 0x170)
#define    DMAC1_CTL4               (DMAC1_BASE + 0x178)
#define    DMAC1_SSTAT4             (DMAC1_BASE + 0x180)
#define    DMAC1_DSTAT4             (DMAC1_BASE + 0x188)
#define    DMAC1_SSTATR4            (DMAC1_BASE + 0x190)
#define    DMAC1_DSTATR4            (DMAC1_BASE + 0x198)
#define    DMAC1_CFG4               (DMAC1_BASE + 0x1A0)
#define    DMAC1_SGR4               (DMAC1_BASE + 0x1A8)
#define    DMAC1_DSR4               (DMAC1_BASE + 0x1B0)

// DMAC1 channel 5
#define    DMAC1_SAR5               (DMAC1_BASE + 0x1B8)
#define    DMAC1_DAR5               (DMAC1_BASE + 0x1C0)
#define    DMAC1_LLP5               (DMAC1_BASE + 0x1C8)
#define    DMAC1_CTL5               (DMAC1_BASE + 0x1D0)
#define    DMAC1_SSTAT5             (DMAC1_BASE + 0x1D8)
#define    DMAC1_DSTAT5             (DMAC1_BASE + 0x1E0)
#define    DMAC1_SSTATR5            (DMAC1_BASE + 0x1E8)
#define    DMAC1_DSTATR5            (DMAC1_BASE + 0x1F0)
#define    DMAC1_CFG5               (DMAC1_BASE + 0x1F8)
#define    DMAC1_SGR5               (DMAC1_BASE + 0x200)
#define    DMAC1_DSR5               (DMAC1_BASE + 0x208)

// DMAC1 channel 6
#define    DMAC1_SAR6               (DMAC1_BASE + 0x210)
#define    DMAC1_DAR6               (DMAC1_BASE + 0x218)
#define    DMAC1_LLP6               (DMAC1_BASE + 0x220)
#define    DMAC1_CTL6               (DMAC1_BASE + 0x228)
#define    DMAC1_SSTAT6             (DMAC1_BASE + 0x230)
#define    DMAC1_DSTAT6             (DMAC1_BASE + 0x238)
#define    DMAC1_SSTATR6            (DMAC1_BASE + 0x240)
#define    DMAC1_DSTATR6            (DMAC1_BASE + 0x248)
#define    DMAC1_CFG6               (DMAC1_BASE + 0x250)
#define    DMAC1_SGR6               (DMAC1_BASE + 0x258)
#define    DMAC1_DSR6               (DMAC1_BASE + 0x260)

// DMAC1 channel 7
#define    DMAC1_SAR7               (DMAC1_BASE + 0x268)
#define    DMAC1_DAR7               (DMAC1_BASE + 0x270)
#define    DMAC1_LLP7               (DMAC1_BASE + 0x278)
#define    DMAC1_CTL7               (DMAC1_BASE + 0x280)
#define    DMAC1_SSTAT7             (DMAC1_BASE + 0x288)
#define    DMAC1_DSTAT7             (DMAC1_BASE + 0x290)
#define    DMAC1_SSTATR7            (DMAC1_BASE + 0x298)
#define    DMAC1_DSTATR7            (DMAC1_BASE + 0x2A0)
#define    DMAC1_CFG7               (DMAC1_BASE + 0x2A8)
#define    DMAC1_SGR7               (DMAC1_BASE + 0x2B0)
#define    DMAC1_DSR7               (DMAC1_BASE + 0x2B8)

#define    DMAC1_RAWTFR             (DMAC1_BASE + 0x2C0)
#define    DMAC1_RAWBLK             (DMAC1_BASE + 0x2C8)
#define    DMAC1_RAWSRCTR           (DMAC1_BASE + 0x2D0)
#define    DMAC1_RAWDSTTR           (DMAC1_BASE + 0x2D8)
#define    DMAC1_RAWERR             (DMAC1_BASE + 0x2E0)

#define    DMAC1_STATRF             (DMAC1_BASE + 0x2E8)
#define    DMAC1_STATBLK            (DMAC1_BASE + 0x2F0)
#define    DMAC1_STATSRCTR          (DMAC1_BASE + 0x2F8)
#define    DMAC1_STATDSTTR          (DMAC1_BASE + 0x300)
#define    DMAC1_STATERR            (DMAC1_BASE + 0x308)

#define    DMAC1_MASKTRF            (DMAC1_BASE + 0x310)
#define    DMAC1_MASKBLK            (DMAC1_BASE + 0x318)
#define    DMAC1_MASKSRCTR          (DMAC1_BASE + 0x320)
#define    DMAC1_MASKDSTTR          (DMAC1_BASE + 0x328)
#define    DMAC1_MASKERR            (DMAC1_BASE + 0x330)

#define    DMAC1_CLRTFR             (DMAC1_BASE + 0x338)
#define    DMAC1_CLRBLK             (DMAC1_BASE + 0x340)
#define    DMAC1_CLRSRCTR           (DMAC1_BASE + 0x348)
#define    DMAC1_CLRDSTTR           (DMAC1_BASE + 0x350)
#define    DMAC1_CLRERR             (DMAC1_BASE + 0x358)
#define    DMAC1_STATINT            (DMAC1_BASE + 0x360)

#define    DMAC1_SRCREQ             (DMAC1_BASE + 0x368)
#define    DMAC1_DSTREQ             (DMAC1_BASE + 0x370)
#define    DMAC1_SGSRCREQ           (DMAC1_BASE + 0x378)
#define    DMAC1_SGDSTREQ           (DMAC1_BASE + 0x380)
#define    DMAC1_LSTSRC             (DMAC1_BASE + 0x388)
#define    DMAC1_LSTDST             (DMAC1_BASE + 0x390)

#define    DMAC1_CFG                (DMAC1_BASE + 0x398)
#define    DMAC1_CHEN               (DMAC1_BASE + 0x3A0)
#define    DMAC1_ID                 (DMAC1_BASE + 0x3A8)
#define    DMAC1_TEST               (DMAC1_BASE + 0x3B0)
#define    DMAC1_COMP_PARAM6        (DMAC1_BASE + 0x3C8)
#define    DMAC1_COMP_PARAM5        (DMAC1_BASE + 0x3D0)
#define    DMAC1_COMP_PARAM4        (DMAC1_BASE + 0x3D8)
#define    DMAC1_COMP_PARAM3        (DMAC1_BASE + 0x3E0)
#define    DMAC1_COMP_PARAM2        (DMAC1_BASE + 0x3E8)
#define    DMAC1_COMP_PARAM1        (DMAC1_BASE + 0x3F0)
#define    DMAC1_COMP_ID            (DMAC1_BASE + 0x3F8)

#endif // DMAC1_BASE


#ifdef DMAC2_BASE

// DMAC2_BASE 31011000
// DMAC2 channel 0
#define    DMAC2_SAR0               (DMAC2_BASE + 0x000)
#define    DMAC2_DAR0               (DMAC2_BASE + 0x008)
#define    DMAC2_LLP0               (DMAC2_BASE + 0x010)
#define    DMAC2_CTL0               (DMAC2_BASE + 0x018)
#define    DMAC2_SSTAT0             (DMAC2_BASE + 0x020)
#define    DMAC2_DSTAT0             (DMAC2_BASE + 0x028)
#define    DMAC2_SSTATR0            (DMAC2_BASE + 0x030)
#define    DMAC2_DSTATR0            (DMAC2_BASE + 0x038)
#define    DMAC2_CFG0               (DMAC2_BASE + 0x040)
#define    DMAC2_SGR0               (DMAC2_BASE + 0x048)
#define    DMAC2_DSR0               (DMAC2_BASE + 0x050)

// DMAC2 channel 1
#define    DMAC2_SAR1               (DMAC2_BASE + 0x058)
#define    DMAC2_DAR1               (DMAC2_BASE + 0x060)
#define    DMAC2_LLP1               (DMAC2_BASE + 0x068)
#define    DMAC2_CTL1               (DMAC2_BASE + 0x070)
#define    DMAC2_SSTAT1             (DMAC2_BASE + 0x078)
#define    DMAC2_DSTAT1             (DMAC2_BASE + 0x080)
#define    DMAC2_SSTATR1            (DMAC2_BASE + 0x088)
#define    DMAC2_DSTATR1            (DMAC2_BASE + 0x090)
#define    DMAC2_CFG1               (DMAC2_BASE + 0x098)
#define    DMAC2_SGR1               (DMAC2_BASE + 0x0A0)
#define    DMAC2_DSR1               (DMAC2_BASE + 0x0A8)

// DMAC2 channel 2
#define    DMAC2_SAR2               (DMAC2_BASE + 0x0B0)
#define    DMAC2_DAR2               (DMAC2_BASE + 0x0B8)
#define    DMAC2_LLP2               (DMAC2_BASE + 0x0C0)
#define    DMAC2_CTL2               (DMAC2_BASE + 0x0C8)
#define    DMAC2_SSTAT2             (DMAC2_BASE + 0x0D0)
#define    DMAC2_DSTAT2             (DMAC2_BASE + 0x0D8)
#define    DMAC2_SSTATR2            (DMAC2_BASE + 0x0E0)
#define    DMAC2_DSTATR2            (DMAC2_BASE + 0x0E8)
#define    DMAC2_CFG2               (DMAC2_BASE + 0x0F0)
#define    DMAC2_SGR2               (DMAC2_BASE + 0x0F8)
#define    DMAC2_DSR2               (DMAC2_BASE + 0x100)

// DMAC2 channel 3
#define    DMAC2_SAR3               (DMAC2_BASE + 0x108)
#define    DMAC2_DAR3               (DMAC2_BASE + 0x110)
#define    DMAC2_LLP3               (DMAC2_BASE + 0x118)
#define    DMAC2_CTL3               (DMAC2_BASE + 0x120)
#define    DMAC2_SSTAT3             (DMAC2_BASE + 0x128)
#define    DMAC2_DSTAT3             (DMAC2_BASE + 0x130)
#define    DMAC2_SSTATR3            (DMAC2_BASE + 0x138)
#define    DMAC2_DSTATR3            (DMAC2_BASE + 0x140)
#define    DMAC2_CFG3               (DMAC2_BASE + 0x148)
#define    DMAC2_SGR3               (DMAC2_BASE + 0x150)
#define    DMAC2_DSR3               (DMAC2_BASE + 0x158)

// DMAC2 channel 4
#define    DMAC2_SAR4               (DMAC2_BASE + 0x160)
#define    DMAC2_DAR4               (DMAC2_BASE + 0x168)
#define    DMAC2_LLP4               (DMAC2_BASE + 0x170)
#define    DMAC2_CTL4               (DMAC2_BASE + 0x178)
#define    DMAC2_SSTAT4             (DMAC2_BASE + 0x180)
#define    DMAC2_DSTAT4             (DMAC2_BASE + 0x188)
#define    DMAC2_SSTATR4            (DMAC2_BASE + 0x190)
#define    DMAC2_DSTATR4            (DMAC2_BASE + 0x198)
#define    DMAC2_CFG4               (DMAC2_BASE + 0x1A0)
#define    DMAC2_SGR4               (DMAC2_BASE + 0x1A8)
#define    DMAC2_DSR4               (DMAC2_BASE + 0x1B0)

// DMAC2 channel 5
#define    DMAC2_SAR5               (DMAC2_BASE + 0x1B8)
#define    DMAC2_DAR5               (DMAC2_BASE + 0x1C0)
#define    DMAC2_LLP5               (DMAC2_BASE + 0x1C8)
#define    DMAC2_CTL5               (DMAC2_BASE + 0x1D0)
#define    DMAC2_SSTAT5             (DMAC2_BASE + 0x1D8)
#define    DMAC2_DSTAT5             (DMAC2_BASE + 0x1E0)
#define    DMAC2_SSTATR5            (DMAC2_BASE + 0x1E8)
#define    DMAC2_DSTATR5            (DMAC2_BASE + 0x1F0)
#define    DMAC2_CFG5               (DMAC2_BASE + 0x1F8)
#define    DMAC2_SGR5               (DMAC2_BASE + 0x200)
#define    DMAC2_DSR5               (DMAC2_BASE + 0x208)

// DMAC2 channel 6
#define    DMAC2_SAR6               (DMAC2_BASE + 0x210)
#define    DMAC2_DAR6               (DMAC2_BASE + 0x218)
#define    DMAC2_LLP6               (DMAC2_BASE + 0x220)
#define    DMAC2_CTL6               (DMAC2_BASE + 0x228)
#define    DMAC2_SSTAT6             (DMAC2_BASE + 0x230)
#define    DMAC2_DSTAT6             (DMAC2_BASE + 0x238)
#define    DMAC2_SSTATR6            (DMAC2_BASE + 0x240)
#define    DMAC2_DSTATR6            (DMAC2_BASE + 0x248)
#define    DMAC2_CFG6               (DMAC2_BASE + 0x250)
#define    DMAC2_SGR6               (DMAC2_BASE + 0x258)
#define    DMAC2_DSR6               (DMAC2_BASE + 0x260)

// DMAC2 channel 7
#define    DMAC2_SAR7               (DMAC2_BASE + 0x268)
#define    DMAC2_DAR7               (DMAC2_BASE + 0x270)
#define    DMAC2_LLP7               (DMAC2_BASE + 0x278)
#define    DMAC2_CTL7               (DMAC2_BASE + 0x280)
#define    DMAC2_SSTAT7             (DMAC2_BASE + 0x288)
#define    DMAC2_DSTAT7             (DMAC2_BASE + 0x290)
#define    DMAC2_SSTATR7            (DMAC2_BASE + 0x298)
#define    DMAC2_DSTATR7            (DMAC2_BASE + 0x2A0)
#define    DMAC2_CFG7               (DMAC2_BASE + 0x2A8)
#define    DMAC2_SGR7               (DMAC2_BASE + 0x2B0)
#define    DMAC2_DSR7               (DMAC2_BASE + 0x2B8)

#define    DMAC2_RAWTFR             (DMAC2_BASE + 0x2C0)
#define    DMAC2_RAWBLK             (DMAC2_BASE + 0x2C8)
#define    DMAC2_RAWSRCTR           (DMAC2_BASE + 0x2D0)
#define    DMAC2_RAWDSTTR           (DMAC2_BASE + 0x2D8)
#define    DMAC2_RAWERR             (DMAC2_BASE + 0x2E0)

#define    DMAC2_STATRF             (DMAC2_BASE + 0x2E8)
#define    DMAC2_STATBLK            (DMAC2_BASE + 0x2F0)
#define    DMAC2_STATSRCTR          (DMAC2_BASE + 0x2F8)
#define    DMAC2_STATDSTTR          (DMAC2_BASE + 0x300)
#define    DMAC2_STATERR            (DMAC2_BASE + 0x308)

#define    DMAC2_MASKTRF            (DMAC2_BASE + 0x310)
#define    DMAC2_MASKBLK            (DMAC2_BASE + 0x318)
#define    DMAC2_MASKSRCTR          (DMAC2_BASE + 0x320)
#define    DMAC2_MASKDSTTR          (DMAC2_BASE + 0x328)
#define    DMAC2_MASKERR            (DMAC2_BASE + 0x330)

#define    DMAC2_CLRTFR             (DMAC2_BASE + 0x338)
#define    DMAC2_CLRBLK             (DMAC2_BASE + 0x340)
#define    DMAC2_CLRSRCTR           (DMAC2_BASE + 0x348)
#define    DMAC2_CLRDSTTR           (DMAC2_BASE + 0x350)
#define    DMAC2_CLRERR             (DMAC2_BASE + 0x358)
#define    DMAC2_STATINT            (DMAC2_BASE + 0x360)

#define    DMAC2_SRCREQ             (DMAC2_BASE + 0x368)
#define    DMAC2_DSTREQ             (DMAC2_BASE + 0x370)
#define    DMAC2_SGSRCREQ           (DMAC2_BASE + 0x378)
#define    DMAC2_SGDSTREQ           (DMAC2_BASE + 0x380)
#define    DMAC2_LSTSRC             (DMAC2_BASE + 0x388)
#define    DMAC2_LSTDST             (DMAC2_BASE + 0x390)

#define    DMAC2_CFG                (DMAC2_BASE + 0x398)
#define    DMAC2_CHEN               (DMAC2_BASE + 0x3A0)
#define    DMAC2_ID                 (DMAC2_BASE + 0x3A8)
#define    DMAC2_TEST               (DMAC2_BASE + 0x3B0)
#define    DMAC2_COMP_PARAM6        (DMAC2_BASE + 0x3C8)
#define    DMAC2_COMP_PARAM5        (DMAC2_BASE + 0x3D0)
#define    DMAC2_COMP_PARAM4        (DMAC2_BASE + 0x3D8)
#define    DMAC2_COMP_PARAM3        (DMAC2_BASE + 0x3E0)
#define    DMAC2_COMP_PARAM2        (DMAC2_BASE + 0x3E8)
#define    DMAC2_COMP_PARAM1        (DMAC2_BASE + 0x3F0)
#define    DMAC2_COMP_ID         	(DMAC2_BASE + 0x3F8)

#endif // DMAC2_BASE


#ifdef DMAC1_BASE_V
// DMAC1_BASE 32027000
// DMAC1 channel 0
#define    DMAC1_SAR0_V            	(DMAC1_BASE_V + 0x000)
#define    DMAC1_DAR0_V             (DMAC1_BASE_V + 0x008)
#define    DMAC1_LLP0_V             (DMAC1_BASE_V + 0x010)
#define    DMAC1_CTL0_V             (DMAC1_BASE_V + 0x018)
#define    DMAC1_SSTAT0_V           (DMAC1_BASE_V + 0x020)
#define    DMAC1_DSTAT0_V           (DMAC1_BASE_V + 0x028)
#define    DMAC1_SSTATR0_V          (DMAC1_BASE_V + 0x030)
#define    DMAC1_DSTATR0_V          (DMAC1_BASE_V + 0x038)
#define    DMAC1_CFG0_V             (DMAC1_BASE_V + 0x040)
#define    DMAC1_SGR0_V             (DMAC1_BASE_V + 0x048)
#define    DMAC1_DSR0_V             (DMAC1_BASE_V + 0x050)

// DMAC1 channel 1
#define    DMAC1_SAR1_V             (DMAC1_BASE_V + 0x058)
#define    DMAC1_DAR1_V             (DMAC1_BASE_V + 0x060)
#define    DMAC1_LLP1_V             (DMAC1_BASE_V + 0x068)
#define    DMAC1_CTL1_V             (DMAC1_BASE_V + 0x070)
#define    DMAC1_SSTAT1_V           (DMAC1_BASE_V + 0x078)
#define    DMAC1_DSTAT1_V           (DMAC1_BASE_V + 0x080)
#define    DMAC1_SSTATR1_V          (DMAC1_BASE_V + 0x088)
#define    DMAC1_DSTATR1_V          (DMAC1_BASE_V + 0x090)
#define    DMAC1_CFG1_V             (DMAC1_BASE_V + 0x098)
#define    DMAC1_SGR1_V             (DMAC1_BASE_V + 0x0A0)
#define    DMAC1_DSR1_V             (DMAC1_BASE_V + 0x0A8)

// DMAC1 channel 2
#define    DMAC1_SAR2_V             (DMAC1_BASE_V + 0x0B0)
#define    DMAC1_DAR2_V             (DMAC1_BASE_V + 0x0B8)
#define    DMAC1_LLP2_V             (DMAC1_BASE_V + 0x0C0)
#define    DMAC1_CTL2_V             (DMAC1_BASE_V + 0x0C8)
#define    DMAC1_SSTAT2_V           (DMAC1_BASE_V + 0x0D0)
#define    DMAC1_DSTAT2_V           (DMAC1_BASE_V + 0x0D8)
#define    DMAC1_SSTATR2_V          (DMAC1_BASE_V + 0x0E0)
#define    DMAC1_DSTATR2_V          (DMAC1_BASE_V + 0x0E8)
#define    DMAC1_CFG2_V             (DMAC1_BASE_V + 0x0F0)
#define    DMAC1_SGR2_V             (DMAC1_BASE_V + 0x0F8)
#define    DMAC1_DSR2_V             (DMAC1_BASE_V + 0x100)

// DMAC1 channel 3
#define    DMAC1_SAR3_V             (DMAC1_BASE_V + 0x108)
#define    DMAC1_DAR3_V             (DMAC1_BASE_V + 0x110)
#define    DMAC1_LLP3_V             (DMAC1_BASE_V + 0x118)
#define    DMAC1_CTL3_V             (DMAC1_BASE_V + 0x120)
#define    DMAC1_SSTAT3_V           (DMAC1_BASE_V + 0x128)
#define    DMAC1_DSTAT3_V           (DMAC1_BASE_V + 0x130)
#define    DMAC1_SSTATR3_V          (DMAC1_BASE_V + 0x138)
#define    DMAC1_DSTATR3_V          (DMAC1_BASE_V + 0x140)
#define    DMAC1_CFG3_V             (DMAC1_BASE_V + 0x148)
#define    DMAC1_SGR3_V             (DMAC1_BASE_V + 0x150)
#define    DMAC1_DSR3_V             (DMAC1_BASE_V + 0x158)

// DMAC1 channel 4
#define    DMAC1_SAR4_V             (DMAC1_BASE_V + 0x160)
#define    DMAC1_DAR4_V             (DMAC1_BASE_V + 0x168)
#define    DMAC1_LLP4_V             (DMAC1_BASE_V + 0x170)
#define    DMAC1_CTL4_V             (DMAC1_BASE_V + 0x178)
#define    DMAC1_SSTAT4_V           (DMAC1_BASE_V + 0x180)
#define    DMAC1_DSTAT4_V           (DMAC1_BASE_V + 0x188)
#define    DMAC1_SSTATR4_V          (DMAC1_BASE_V + 0x190)
#define    DMAC1_DSTATR4_V          (DMAC1_BASE_V + 0x198)
#define    DMAC1_CFG4_V             (DMAC1_BASE_V + 0x1A0)
#define    DMAC1_SGR4_V             (DMAC1_BASE_V + 0x1A8)
#define    DMAC1_DSR4_V             (DMAC1_BASE_V + 0x1B0)

// DMAC1 channel 5
#define    DMAC1_SAR5_V             (DMAC1_BASE_V + 0x1B8)
#define    DMAC1_DAR5_V             (DMAC1_BASE_V + 0x1C0)
#define    DMAC1_LLP5_V             (DMAC1_BASE_V + 0x1C8)
#define    DMAC1_CTL5_V             (DMAC1_BASE_V + 0x1D0)
#define    DMAC1_SSTAT5_V           (DMAC1_BASE_V + 0x1D8)
#define    DMAC1_DSTAT5_V           (DMAC1_BASE_V + 0x1E0)
#define    DMAC1_SSTATR5_V          (DMAC1_BASE_V + 0x1E8)
#define    DMAC1_DSTATR5_V          (DMAC1_BASE_V + 0x1F0)
#define    DMAC1_CFG5_V             (DMAC1_BASE_V + 0x1F8)
#define    DMAC1_SGR5_V             (DMAC1_BASE_V + 0x200)
#define    DMAC1_DSR5_V             (DMAC1_BASE_V + 0x208)

// DMAC1 channel 6
#define    DMAC1_SAR6_V             (DMAC1_BASE_V + 0x210)
#define    DMAC1_DAR6_V             (DMAC1_BASE_V + 0x218)
#define    DMAC1_LLP6_V             (DMAC1_BASE_V + 0x220)
#define    DMAC1_CTL6_V             (DMAC1_BASE_V + 0x228)
#define    DMAC1_SSTAT6_V           (DMAC1_BASE_V + 0x230)
#define    DMAC1_DSTAT6_V           (DMAC1_BASE_V + 0x238)
#define    DMAC1_SSTATR6_V          (DMAC1_BASE_V + 0x240)
#define    DMAC1_DSTATR6_V          (DMAC1_BASE_V + 0x248)
#define    DMAC1_CFG6_V             (DMAC1_BASE_V + 0x250)
#define    DMAC1_SGR6_V             (DMAC1_BASE_V + 0x258)
#define    DMAC1_DSR6_V             (DMAC1_BASE_V + 0x260)

// DMAC1 channel 7
#define    DMAC1_SAR7_V             (DMAC1_BASE_V + 0x268)
#define    DMAC1_DAR7_V             (DMAC1_BASE_V + 0x270)
#define    DMAC1_LLP7_V             (DMAC1_BASE_V + 0x278)
#define    DMAC1_CTL7_V             (DMAC1_BASE_V + 0x280)
#define    DMAC1_SSTAT7_V           (DMAC1_BASE_V + 0x288)
#define    DMAC1_DSTAT7_V           (DMAC1_BASE_V + 0x290)
#define    DMAC1_SSTATR7_V          (DMAC1_BASE_V + 0x298)
#define    DMAC1_DSTATR7_V          (DMAC1_BASE_V + 0x2A0)
#define    DMAC1_CFG7_V             (DMAC1_BASE_V + 0x2A8)
#define    DMAC1_SGR7_V             (DMAC1_BASE_V + 0x2B0)
#define    DMAC1_DSR7_V             (DMAC1_BASE_V + 0x2B8)

#define    DMAC1_RAWTFR_V           (DMAC1_BASE_V + 0x2C0)
#define    DMAC1_RAWBLK_V           (DMAC1_BASE_V + 0x2C8)
#define    DMAC1_RAWSRCTR_V         (DMAC1_BASE_V + 0x2D0)
#define    DMAC1_RAWDSTTR_V         (DMAC1_BASE_V + 0x2D8)
#define    DMAC1_RAWERR_V           (DMAC1_BASE_V + 0x2E0)

#define    DMAC1_STATRF_V           (DMAC1_BASE_V + 0x2E8)
#define    DMAC1_STATBLK_V          (DMAC1_BASE_V + 0x2F0)
#define    DMAC1_STATSRCTR_V        (DMAC1_BASE_V + 0x2F8)
#define    DMAC1_STATDSTTR_V        (DMAC1_BASE_V + 0x300)
#define    DMAC1_STATERR_V          (DMAC1_BASE_V + 0x308)

#define    DMAC1_MASKTRF_V          (DMAC1_BASE_V + 0x310)
#define    DMAC1_MASKBLK_V          (DMAC1_BASE_V + 0x318)
#define    DMAC1_MASKSRCTR_V        (DMAC1_BASE_V + 0x320)
#define    DMAC1_MASKDSTTR_V        (DMAC1_BASE_V + 0x328)
#define    DMAC1_MASKERR_V          (DMAC1_BASE_V + 0x330)

#define    DMAC1_CLRTFR_V           (DMAC1_BASE_V + 0x338)
#define    DMAC1_CLRBLK_V           (DMAC1_BASE_V + 0x340)
#define    DMAC1_CLRSRCTR_V         (DMAC1_BASE_V + 0x348)
#define    DMAC1_CLRDSTTR_V         (DMAC1_BASE_V + 0x350)
#define    DMAC1_CLRERR_V           (DMAC1_BASE_V + 0x358)
#define    DMAC1_STATINT_V          (DMAC1_BASE_V + 0x360)

#define    DMAC1_SRCREQ_V           (DMAC1_BASE_V + 0x368)
#define    DMAC1_DSTREQ_V           (DMAC1_BASE_V + 0x370)
#define    DMAC1_SGSRCREQ_V         (DMAC1_BASE_V + 0x378)
#define    DMAC1_SGDSTREQ_V         (DMAC1_BASE_V + 0x380)
#define    DMAC1_LSTSRC_V           (DMAC1_BASE_V + 0x388)
#define    DMAC1_LSTDST_V           (DMAC1_BASE_V + 0x390)

#define    DMAC1_CFG_V              (DMAC1_BASE_V + 0x398)
#define    DMAC1_CHEN_V             (DMAC1_BASE_V + 0x3A0)
#define    DMAC1_ID_V               (DMAC1_BASE_V + 0x3A8)
#define    DMAC1_TEST_V             (DMAC1_BASE_V + 0x3B0)
#define    DMAC1_COMP_PARAM6_V      (DMAC1_BASE_V + 0x3C8)
#define    DMAC1_COMP_PARAM5_V      (DMAC1_BASE_V + 0x3D0)
#define    DMAC1_COMP_PARAM4_V      (DMAC1_BASE_V + 0x3D8)
#define    DMAC1_COMP_PARAM3_V      (DMAC1_BASE_V + 0x3E0)
#define    DMAC1_COMP_PARAM2_V      (DMAC1_BASE_V + 0x3E8)
#define    DMAC1_COMP_PARAM1_V      (DMAC1_BASE_V + 0x3F0)
#define    DMAC1_COMP_ID_V          (DMAC1_BASE_V + 0x3F8)

#endif // DMAC1_BASE_V


#ifdef DMAC2_BASE_V

// DMAC2_BASE 31011000
// DMAC2 channel 0
#define    DMAC2_SAR0_V             (DMAC2_BASE_V + 0x000)
#define    DMAC2_DAR0_V             (DMAC2_BASE_V + 0x008)
#define    DMAC2_LLP0_V             (DMAC2_BASE_V + 0x010)
#define    DMAC2_CTL0_V             (DMAC2_BASE_V + 0x018)
#define    DMAC2_SSTAT0_V           (DMAC2_BASE_V + 0x020)
#define    DMAC2_DSTAT0_V           (DMAC2_BASE_V + 0x028)
#define    DMAC2_SSTATR0_V          (DMAC2_BASE_V + 0x030)
#define    DMAC2_DSTATR0_V          (DMAC2_BASE_V + 0x038)
#define    DMAC2_CFG0_V             (DMAC2_BASE_V + 0x040)
#define    DMAC2_SGR0_V             (DMAC2_BASE_V + 0x048)
#define    DMAC2_DSR0_V             (DMAC2_BASE_V + 0x050)

// DMAC2 channel 1
#define    DMAC2_SAR1_V             (DMAC2_BASE_V + 0x058)
#define    DMAC2_DAR1_V             (DMAC2_BASE_V + 0x060)
#define    DMAC2_LLP1_V             (DMAC2_BASE_V + 0x068)
#define    DMAC2_CTL1_V             (DMAC2_BASE_V + 0x070)
#define    DMAC2_SSTAT1_V           (DMAC2_BASE_V + 0x078)
#define    DMAC2_DSTAT1_V           (DMAC2_BASE_V + 0x080)
#define    DMAC2_SSTATR1_V          (DMAC2_BASE_V + 0x088)
#define    DMAC2_DSTATR1_V          (DMAC2_BASE_V + 0x090)
#define    DMAC2_CFG1_V             (DMAC2_BASE_V + 0x098)
#define    DMAC2_SGR1_V             (DMAC2_BASE_V + 0x0A0)
#define    DMAC2_DSR1_V             (DMAC2_BASE_V + 0x0A8)

// DMAC2 channel 2
#define    DMAC2_SAR2_V             (DMAC2_BASE_V + 0x0B0)
#define    DMAC2_DAR2_V             (DMAC2_BASE_V + 0x0B8)
#define    DMAC2_LLP2_V             (DMAC2_BASE_V + 0x0C0)
#define    DMAC2_CTL2_V             (DMAC2_BASE_V + 0x0C8)
#define    DMAC2_SSTAT2_V           (DMAC2_BASE_V + 0x0D0)
#define    DMAC2_DSTAT2_V           (DMAC2_BASE_V + 0x0D8)
#define    DMAC2_SSTATR2_V          (DMAC2_BASE_V + 0x0E0)
#define    DMAC2_DSTATR2_V          (DMAC2_BASE_V + 0x0E8)
#define    DMAC2_CFG2_V             (DMAC2_BASE_V + 0x0F0)
#define    DMAC2_SGR2_V             (DMAC2_BASE_V + 0x0F8)
#define    DMAC2_DSR2_V             (DMAC2_BASE_V + 0x100)

// DMAC2 channel 3
#define    DMAC2_SAR3_V             (DMAC2_BASE_V + 0x108)
#define    DMAC2_DAR3_V             (DMAC2_BASE_V + 0x110)
#define    DMAC2_LLP3_V             (DMAC2_BASE_V + 0x118)
#define    DMAC2_CTL3_V             (DMAC2_BASE_V + 0x120)
#define    DMAC2_SSTAT3_V           (DMAC2_BASE_V + 0x128)
#define    DMAC2_DSTAT3_V           (DMAC2_BASE_V + 0x130)
#define    DMAC2_SSTATR3_V          (DMAC2_BASE_V + 0x138)
#define    DMAC2_DSTATR3_V          (DMAC2_BASE_V + 0x140)
#define    DMAC2_CFG3_V             (DMAC2_BASE_V + 0x148)
#define    DMAC2_SGR3_V             (DMAC2_BASE_V + 0x150)
#define    DMAC2_DSR3_V             (DMAC2_BASE_V + 0x158)

// DMAC2 channel 4
#define    DMAC2_SAR4_V             (DMAC2_BASE_V + 0x160)
#define    DMAC2_DAR4_V             (DMAC2_BASE_V + 0x168)
#define    DMAC2_LLP4_V             (DMAC2_BASE_V + 0x170)
#define    DMAC2_CTL4_V             (DMAC2_BASE_V + 0x178)
#define    DMAC2_SSTAT4_V           (DMAC2_BASE_V + 0x180)
#define    DMAC2_DSTAT4_V           (DMAC2_BASE_V + 0x188)
#define    DMAC2_SSTATR4_V          (DMAC2_BASE_V + 0x190)
#define    DMAC2_DSTATR4_V          (DMAC2_BASE_V + 0x198)
#define    DMAC2_CFG4_V             (DMAC2_BASE_V + 0x1A0)
#define    DMAC2_SGR4_V             (DMAC2_BASE_V + 0x1A8)
#define    DMAC2_DSR4_V             (DMAC2_BASE_V + 0x1B0)

// DMAC2 channel 5
#define    DMAC2_SAR5_V             (DMAC2_BASE_V + 0x1B8)
#define    DMAC2_DAR5_V             (DMAC2_BASE_V + 0x1C0)
#define    DMAC2_LLP5_V             (DMAC2_BASE_V + 0x1C8)
#define    DMAC2_CTL5_V             (DMAC2_BASE_V + 0x1D0)
#define    DMAC2_SSTAT5_V           (DMAC2_BASE_V + 0x1D8)
#define    DMAC2_DSTAT5_V           (DMAC2_BASE_V + 0x1E0)
#define    DMAC2_SSTATR5_V          (DMAC2_BASE_V + 0x1E8)
#define    DMAC2_DSTATR5_V          (DMAC2_BASE_V + 0x1F0)
#define    DMAC2_CFG5_V             (DMAC2_BASE_V + 0x1F8)
#define    DMAC2_SGR5_V             (DMAC2_BASE_V + 0x200)
#define    DMAC2_DSR5_V             (DMAC2_BASE_V + 0x208)

// DMAC2 channel 6
#define    DMAC2_SAR6_V             (DMAC2_BASE_V + 0x210)
#define    DMAC2_DAR6_V             (DMAC2_BASE_V + 0x218)
#define    DMAC2_LLP6_V             (DMAC2_BASE_V + 0x220)
#define    DMAC2_CTL6_V             (DMAC2_BASE_V + 0x228)
#define    DMAC2_SSTAT6_V           (DMAC2_BASE_V + 0x230)
#define    DMAC2_DSTAT6_V           (DMAC2_BASE_V + 0x238)
#define    DMAC2_SSTATR6_V          (DMAC2_BASE_V + 0x240)
#define    DMAC2_DSTATR6_V          (DMAC2_BASE_V + 0x248)
#define    DMAC2_CFG6_V             (DMAC2_BASE_V + 0x250)
#define    DMAC2_SGR6_V             (DMAC2_BASE_V + 0x258)
#define    DMAC2_DSR6_V             (DMAC2_BASE_V + 0x260)

// DMAC2 channel 7
#define    DMAC2_SAR7_V             (DMAC2_BASE_V + 0x268)
#define    DMAC2_DAR7_V             (DMAC2_BASE_V + 0x270)
#define    DMAC2_LLP7_V             (DMAC2_BASE_V + 0x278)
#define    DMAC2_CTL7_V             (DMAC2_BASE_V + 0x280)
#define    DMAC2_SSTAT7_V           (DMAC2_BASE_V + 0x288)
#define    DMAC2_DSTAT7_V           (DMAC2_BASE_V + 0x290)
#define    DMAC2_SSTATR7_V          (DMAC2_BASE_V + 0x298)
#define    DMAC2_DSTATR7_V          (DMAC2_BASE_V + 0x2A0)
#define    DMAC2_CFG7_V             (DMAC2_BASE_V + 0x2A8)
#define    DMAC2_SGR7_V             (DMAC2_BASE_V + 0x2B0)
#define    DMAC2_DSR7_V             (DMAC2_BASE_V + 0x2B8)

#define    DMAC2_RAWTFR_V           (DMAC2_BASE_V + 0x2C0)
#define    DMAC2_RAWBLK_V           (DMAC2_BASE_V + 0x2C8)
#define    DMAC2_RAWSRCTR_V         (DMAC2_BASE_V + 0x2D0)
#define    DMAC2_RAWDSTTR_V         (DMAC2_BASE_V + 0x2D8)
#define    DMAC2_RAWERR_V           (DMAC2_BASE_V + 0x2E0)

#define    DMAC2_STATRF_V           (DMAC2_BASE_V + 0x2E8)
#define    DMAC2_STATBLK_V          (DMAC2_BASE_V + 0x2F0)
#define    DMAC2_STATSRCTR_V        (DMAC2_BASE_V + 0x2F8)
#define    DMAC2_STATDSTTR_V        (DMAC2_BASE_V + 0x300)
#define    DMAC2_STATERR_V          (DMAC2_BASE_V + 0x308)

#define    DMAC2_MASKTRF_V          (DMAC2_BASE_V + 0x310)
#define    DMAC2_MASKBLK_V          (DMAC2_BASE_V + 0x318)
#define    DMAC2_MASKSRCTR_V        (DMAC2_BASE_V + 0x320)
#define    DMAC2_MASKDSTTR_V        (DMAC2_BASE_V + 0x328)
#define    DMAC2_MASKERR_V          (DMAC2_BASE_V + 0x330)

#define    DMAC2_CLRTFR_V           (DMAC2_BASE_V + 0x338)
#define    DMAC2_CLRBLK_V           (DMAC2_BASE_V + 0x340)
#define    DMAC2_CLRSRCTR_V         (DMAC2_BASE_V + 0x348)
#define    DMAC2_CLRDSTTR_V         (DMAC2_BASE_V + 0x350)
#define    DMAC2_CLRERR_V           (DMAC2_BASE_V + 0x358)
#define    DMAC2_STATINT_V          (DMAC2_BASE_V + 0x360)

#define    DMAC2_SRCREQ_V           (DMAC2_BASE_V + 0x368)
#define    DMAC2_DSTREQ_V           (DMAC2_BASE_V + 0x370)
#define    DMAC2_SGSRCREQ_V         (DMAC2_BASE_V + 0x378)
#define    DMAC2_SGDSTREQ_V         (DMAC2_BASE_V + 0x380)
#define    DMAC2_LSTSRC_V           (DMAC2_BASE_V + 0x388)
#define    DMAC2_LSTDST_V           (DMAC2_BASE_V + 0x390)

#define    DMAC2_CFG_V              (DMAC2_BASE_V + 0x398)
#define    DMAC2_CHEN_V             (DMAC2_BASE_V + 0x3A0)
#define    DMAC2_ID_V               (DMAC2_BASE_V + 0x3A8)
#define    DMAC2_TEST_V             (DMAC2_BASE_V + 0x3B0)
#define    DMAC2_COMP_PARAM6_V      (DMAC2_BASE_V + 0x3C8)
#define    DMAC2_COMP_PARAM5_V      (DMAC2_BASE_V + 0x3D0)
#define    DMAC2_COMP_PARAM4_V      (DMAC2_BASE_V + 0x3D8)
#define    DMAC2_COMP_PARAM3_V      (DMAC2_BASE_V + 0x3E0)
#define    DMAC2_COMP_PARAM2_V      (DMAC2_BASE_V + 0x3E8)
#define    DMAC2_COMP_PARAM1_V      (DMAC2_BASE_V + 0x3F0)
#define    DMAC2_COMP_ID_V          (DMAC2_BASE_V + 0x3F8)

#endif // DMAC2_BASE_V


// LCDC register define  物理地址
// LCDC_BASE 0xB2025000
#ifdef LCDC_BASE

#define    LCDC_ECR                 (LCDC_BASE + 0x000)
#define    LCDC_BECR                (LCDC_BASE + 0x004)
#define    LCDC_W1ECR               (LCDC_BASE + 0x008)
#define    LCDC_W2ECR               (LCDC_BASE + 0x00C)
#define    LCDC_CECR                (LCDC_BASE + 0x010)
#define    LCDC_BCR                 (LCDC_BASE + 0x014)
#define    LCDC_PLCR                (LCDC_BASE + 0x018)
#define    LCDC_PFCR                (LCDC_BASE + 0x01C)
#define    LCDC_PCCR                (LCDC_BASE + 0x020)
#define    LCDC_IOCR                (LCDC_BASE + 0x024)
#define    LCDC_IECR                (LCDC_BASE + 0x028)
#define    LCDC_W1CKR               (LCDC_BASE + 0x02C)
#define    LCDC_W2CKR               (LCDC_BASE + 0x030)
#define    LCDC_W1BVR               (LCDC_BASE + 0x034)
#define    LCDC_W2BVR               (LCDC_BASE + 0x038)
#define    LCDC_SCR                 (LCDC_BASE + 0x03C)
#define    LCDC_W1TPCR              (LCDC_BASE + 0x040)
#define    LCDC_W1BPCR              (LCDC_BASE + 0x044)
#define    LCDC_W2TPCR              (LCDC_BASE + 0x048)
#define    LCDC_W2BPCR              (LCDC_BASE + 0x04C)
#define    LCDC_CTPCR               (LCDC_BASE + 0x050)
#define    LCDC_CBPCR               (LCDC_BASE + 0x054)
#define    LCDC_ACSR                (LCDC_BASE + 0x058)
#define    LCDC_BBSAR               (LCDC_BASE + 0x05C)
#define    LCDC_W1YBSAR             (LCDC_BASE + 0x060)
#define    LCDC_W1UBSAR             (LCDC_BASE + 0x064)
#define    LCDC_W1VBSAR             (LCDC_BASE + 0x068)
#define    LCDC_W2BSAR              (LCDC_BASE + 0x06C)
#define    LCDC_CBSAR               (LCDC_BASE + 0x070)
#define    LCDC_BBEAR               (LCDC_BASE + 0x074)
#define    LCDC_W1YBEAR             (LCDC_BASE + 0x078)
#define    LCDC_W1UBEAR             (LCDC_BASE + 0x07C)
#define    LCDC_W1VBEAR             (LCDC_BASE + 0x080)
#define    LCDC_W2BEAR              (LCDC_BASE + 0x084)
#define    LCDC_CBEAR               (LCDC_BASE + 0x088)
#define    LCDC_CC0R                (LCDC_BASE + 0x08C)
#define    LCDC_CC1R                (LCDC_BASE + 0x090)
#define    LCDC_CC2R                (LCDC_BASE + 0x094)
#define    LCDC_CC3R                (LCDC_BASE + 0x098)
#define    LCDC_BRIWR               (LCDC_BASE + 0x09C)
#define    LCDC_OVL1RIWR            (LCDC_BASE + 0x0A0)
#define    LCDC_OVL2RIWR            (LCDC_BASE + 0x0A4)
#define    LCDC_BDIWR               (LCDC_BASE + 0x0A8)
#define    LCDC_OVL1IWR             (LCDC_BASE + 0x0AC)
#define    LCDC_OVL2IWR             (LCDC_BASE + 0x0B0)
#define    LCDC_BTPCR               (LCDC_BASE + 0x0B4)
#define    LCDC_BBPCR               (LCDC_BASE + 0x0B8)
#endif // LCDC_BASE

// LCDC register define  虚拟地址
// LCDC_BASE_V 0x32025000
#ifdef LCDC_BASE_V

#define    LCDC_ECR_V               (LCDC_BASE_V + 0x000)
#define    LCDC_BECR_V              (LCDC_BASE_V + 0x004)
#define    LCDC_W1ECR_V             (LCDC_BASE_V + 0x008)
#define    LCDC_W2ECR_V             (LCDC_BASE_V + 0x00C)
#define    LCDC_CECR_V              (LCDC_BASE_V + 0x010)
#define    LCDC_BCR_V               (LCDC_BASE_V + 0x014)
#define    LCDC_PLCR_V              (LCDC_BASE_V + 0x018)
#define    LCDC_PFCR_V              (LCDC_BASE_V + 0x01C)
#define    LCDC_PCCR_V              (LCDC_BASE_V + 0x020)
#define    LCDC_IOCR_V              (LCDC_BASE_V + 0x024)
#define    LCDC_IECR_V              (LCDC_BASE_V + 0x028)
#define    LCDC_W1CKR_V             (LCDC_BASE_V + 0x02C)
#define    LCDC_W2CKR_V             (LCDC_BASE_V + 0x030)
#define    LCDC_W1BVR_V             (LCDC_BASE_V + 0x034)
#define    LCDC_W2BVR_V             (LCDC_BASE_V + 0x038)
#define    LCDC_SCR_V               (LCDC_BASE_V + 0x03C)
#define    LCDC_W1TPCR_V            (LCDC_BASE_V + 0x040)
#define    LCDC_W1BPCR_V            (LCDC_BASE_V + 0x044)
#define    LCDC_W2TPCR_V            (LCDC_BASE_V + 0x048)
#define    LCDC_W2BPCR_V            (LCDC_BASE_V + 0x04C)
#define    LCDC_CTPCR_V             (LCDC_BASE_V + 0x050)
#define    LCDC_CBPCR_V             (LCDC_BASE_V + 0x054)
#define    LCDC_ACSR_V              (LCDC_BASE_V + 0x058)
#define    LCDC_BBSAR_V             (LCDC_BASE_V + 0x05C)
#define    LCDC_W1YBSAR_V           (LCDC_BASE_V + 0x060)
#define    LCDC_W1UBSAR_V           (LCDC_BASE_V + 0x064)
#define    LCDC_W1VBSAR_V           (LCDC_BASE_V + 0x068)
#define    LCDC_W2BSAR_V            (LCDC_BASE_V + 0x06C)
#define    LCDC_CBSAR_V             (LCDC_BASE_V + 0x070)
#define    LCDC_BBEAR_V             (LCDC_BASE_V + 0x074)
#define    LCDC_W1YBEAR_V           (LCDC_BASE_V + 0x078)
#define    LCDC_W1UBEAR_V           (LCDC_BASE_V + 0x07C)
#define    LCDC_W1VBEAR_V           (LCDC_BASE_V + 0x080)
#define    LCDC_W2BEAR_V            (LCDC_BASE_V + 0x084)
#define    LCDC_CBEAR_V             (LCDC_BASE_V + 0x088)
#define    LCDC_CC0R_V              (LCDC_BASE_V + 0x08C)
#define    LCDC_CC1R_V              (LCDC_BASE_V + 0x090)
#define    LCDC_CC2R_V              (LCDC_BASE_V + 0x094)
#define    LCDC_CC3R_V              (LCDC_BASE_V + 0x098)
#define    LCDC_BRIWR_V             (LCDC_BASE_V + 0x09C)
#define    LCDC_OVL1RIWR_V          (LCDC_BASE_V + 0x0A0)
#define    LCDC_OVL2RIWR_V          (LCDC_BASE_V + 0x0A4)
#define    LCDC_BDIWR_V             (LCDC_BASE_V + 0x0A8)
#define    LCDC_OVL1IWR_V           (LCDC_BASE_V + 0x0AC)
#define    LCDC_OVL2IWR_V           (LCDC_BASE_V + 0x0B0)
#define    LCDC_BTPCR_V             (LCDC_BASE_V + 0x0B4)
#define    LCDC_BBPCR_V             (LCDC_BASE_V + 0x0B8)
#endif // LCDC_BASE_V


// DDR2 Controller register define 物理地址
// DDR2CTL_BASE 0xB3000000
#ifdef DDR2CTL_BASE
 
#define    DDR2_CCR                 (DDR2CTL_BASE + 0x000)
#define    DDR2_DCR                 (DDR2CTL_BASE + 0x004)
#define    DDR2_IOCR                (DDR2CTL_BASE + 0x008)
#define    DDR2_CSR                 (DDR2CTL_BASE + 0x00C)
#define    DDR2_DRR                 (DDR2CTL_BASE + 0x010)
#define    DDR2_TPR0                (DDR2CTL_BASE + 0x014)
#define    DDR2_TPR1                (DDR2CTL_BASE + 0x018)
#define    DDR2_TPR2                (DDR2CTL_BASE + 0x01C)
#define    DDR2_GDLLCR              (DDR2CTL_BASE + 0x020)
#define    DDR2_DLLCR0              (DDR2CTL_BASE + 0x024)
#define    DDR2_DLLCR1              (DDR2CTL_BASE + 0x028)
#define    DDR2_DLLCR2              (DDR2CTL_BASE + 0x02C)
#define    DDR2_DLLCR3              (DDR2CTL_BASE + 0x030)
#define    DDR2_DLLCR4              (DDR2CTL_BASE + 0x034)
#define    DDR2_DLLCR5              (DDR2CTL_BASE + 0x038)
#define    DDR2_DLLCR6              (DDR2CTL_BASE + 0x03C)
#define    DDR2_DLLCR7              (DDR2CTL_BASE + 0x040)
#define    DDR2_DLLCR8              (DDR2CTL_BASE + 0x044)
#define    DDR2_DLLCR9              (DDR2CTL_BASE + 0x048)
#define    DDR2_RSLR0               (DDR2CTL_BASE + 0x04C)
#define    DDR2_RSLR1               (DDR2CTL_BASE + 0x050)
#define    DDR2_RSLR2               (DDR2CTL_BASE + 0x054)
#define    DDR2_RSLR3               (DDR2CTL_BASE + 0x058)
#define    DDR2_RDGR0               (DDR2CTL_BASE + 0x05C)
#define    DDR2_RDGR1               (DDR2CTL_BASE + 0x060)
#define    DDR2_RDGR2               (DDR2CTL_BASE + 0x064)
#define    DDR2_RDGR3               (DDR2CTL_BASE + 0x068)
#define    DDR2_DQTR0               (DDR2CTL_BASE + 0x06C)
#define    DDR2_DQTR1               (DDR2CTL_BASE + 0x070)
#define    DDR2_DQTR2               (DDR2CTL_BASE + 0x074)
#define    DDR2_DQTR3               (DDR2CTL_BASE + 0x078)
#define    DDR2_DQTR4               (DDR2CTL_BASE + 0x07C)
#define    DDR2_DQTR5               (DDR2CTL_BASE + 0x080)
#define    DDR2_DQTR6               (DDR2CTL_BASE + 0x084)
#define    DDR2_DQTR7               (DDR2CTL_BASE + 0x088)
#define    DDR2_DQTR8               (DDR2CTL_BASE + 0x08C)
#define    DDR2_DQSTR               (DDR2CTL_BASE + 0x090)
#define    DDR2_DQSBTR              (DDR2CTL_BASE + 0x094)
#define    DDR2_ODTCR               (DDR2CTL_BASE + 0x098)
#define    DDR2_DTR0                (DDR2CTL_BASE + 0x09C)
#define    DDR2_DTR1                (DDR2CTL_BASE + 0x0A0)
#define    DDR2_DTAR                (DDR2CTL_BASE + 0x0A4)
#define    DDR2_ZQCR0               (DDR2CTL_BASE + 0x0A8)
#define    DDR2_ZQCR1               (DDR2CTL_BASE + 0x0AC)
#define    DDR2_ZQSR                (DDR2CTL_BASE + 0x0B0)

#define    DDR2_MR                  (DDR2CTL_BASE + 0x1F0)
#define    DDR2_EMR                 (DDR2CTL_BASE + 0x1F4)
#define    DDR2_EMR2                (DDR2CTL_BASE + 0x1F8)
#define    DDR2_EMR3                (DDR2CTL_BASE + 0x1FC)

#define    DDR2_HPCR0               (DDR2CTL_BASE + 0x400)
#define    DDR2_HPCR1               (DDR2CTL_BASE + 0x404)
#define    DDR2_HPCR2               (DDR2CTL_BASE + 0x408)
#define    DDR2_HPCR3               (DDR2CTL_BASE + 0x40C)
#define    DDR2_HPCR4               (DDR2CTL_BASE + 0x410)
#define    DDR2_HPCR5               (DDR2CTL_BASE + 0x414)
#define    DDR2_HPCR6               (DDR2CTL_BASE + 0x418)
#define    DDR2_HPCR7               (DDR2CTL_BASE + 0x41C)
#define    DDR2_HPCR8               (DDR2CTL_BASE + 0x420)
#define    DDR2_HPCR9               (DDR2CTL_BASE + 0x424)
#define    DDR2_HPCR10              (DDR2CTL_BASE + 0x428)
#define    DDR2_HPCR11              (DDR2CTL_BASE + 0x42C)
#define    DDR2_HPCR12              (DDR2CTL_BASE + 0x430)
#define    DDR2_HPCR13              (DDR2CTL_BASE + 0x434)
#define    DDR2_HPCR14              (DDR2CTL_BASE + 0x438)
#define    DDR2_HPCR15              (DDR2CTL_BASE + 0x43C)
#define    DDR2_HPCR16              (DDR2CTL_BASE + 0x440)
#define    DDR2_HPCR17              (DDR2CTL_BASE + 0x444)
#define    DDR2_HPCR18              (DDR2CTL_BASE + 0x448)
#define    DDR2_HPCR19              (DDR2CTL_BASE + 0x44C)
#define    DDR2_HPCR20              (DDR2CTL_BASE + 0x450)
#define    DDR2_HPCR21              (DDR2CTL_BASE + 0x454)
#define    DDR2_HPCR22              (DDR2CTL_BASE + 0x458)
#define    DDR2_HPCR23              (DDR2CTL_BASE + 0x45C)
#define    DDR2_HPCR24              (DDR2CTL_BASE + 0x460)
#define    DDR2_HPCR25              (DDR2CTL_BASE + 0x464)
#define    DDR2_HPCR26              (DDR2CTL_BASE + 0x468)
#define    DDR2_HPCR27              (DDR2CTL_BASE + 0x46C)
#define    DDR2_HPCR28              (DDR2CTL_BASE + 0x470)
#define    DDR2_HPCR29              (DDR2CTL_BASE + 0x474)
#define    DDR2_HPCR30              (DDR2CTL_BASE + 0x478)
#define    DDR2_HPCR31              (DDR2CTL_BASE + 0x47C)

#define    DDR2_PQCR0               (DDR2CTL_BASE + 0x480)
#define    DDR2_PQCR1               (DDR2CTL_BASE + 0x484)
#define    DDR2_PQCR2               (DDR2CTL_BASE + 0x488)
#define    DDR2_PQCR3               (DDR2CTL_BASE + 0x48C)
#define    DDR2_PQCR4               (DDR2CTL_BASE + 0x490)
#define    DDR2_PQCR5               (DDR2CTL_BASE + 0x494)
#define    DDR2_PQCR6               (DDR2CTL_BASE + 0x498)
#define    DDR2_PQCR7               (DDR2CTL_BASE + 0x49C)

#define    DDR2_MMGCR               (DDR2CTL_BASE + 0x4A0)

#endif // DDR2CTL_BASE_V

// DDR2 Controller register define 虚拟地址
// DDR2CTL_BASE_V 0x33000000
#ifdef DDR2CTL_BASE_V
 
#define    DDR2_CCR_V               (DDR2CTL_BASE_V + 0x000)
#define    DDR2_DCR_V               (DDR2CTL_BASE_V + 0x004)
#define    DDR2_IOCR_V              (DDR2CTL_BASE_V + 0x008)
#define    DDR2_CSR_V               (DDR2CTL_BASE_V + 0x00C)
#define    DDR2_DRR_V               (DDR2CTL_BASE_V + 0x010)
#define    DDR2_TPR0_V              (DDR2CTL_BASE_V + 0x014)
#define    DDR2_TPR1_V              (DDR2CTL_BASE_V + 0x018)
#define    DDR2_TPR2_V              (DDR2CTL_BASE_V + 0x01C)
#define    DDR2_GDLLCR_V            (DDR2CTL_BASE_V + 0x020)
#define    DDR2_DLLCR0_V            (DDR2CTL_BASE_V + 0x024)
#define    DDR2_DLLCR1_V            (DDR2CTL_BASE_V + 0x028)
#define    DDR2_DLLCR2_V            (DDR2CTL_BASE_V + 0x02C)
#define    DDR2_DLLCR3_V            (DDR2CTL_BASE_V + 0x030)
#define    DDR2_DLLCR4_V            (DDR2CTL_BASE_V + 0x034)
#define    DDR2_DLLCR5_V            (DDR2CTL_BASE_V + 0x038)
#define    DDR2_DLLCR6_V            (DDR2CTL_BASE_V + 0x03C)
#define    DDR2_DLLCR7_V            (DDR2CTL_BASE_V + 0x040)
#define    DDR2_DLLCR8_V            (DDR2CTL_BASE_V + 0x044)
#define    DDR2_DLLCR9_V            (DDR2CTL_BASE_V + 0x048)
#define    DDR2_RSLR0_V             (DDR2CTL_BASE_V + 0x04C)
#define    DDR2_RSLR1_V             (DDR2CTL_BASE_V + 0x050)
#define    DDR2_RSLR2_V             (DDR2CTL_BASE_V + 0x054)
#define    DDR2_RSLR3_V             (DDR2CTL_BASE_V + 0x058)
#define    DDR2_RDGR0_V             (DDR2CTL_BASE_V + 0x05C)
#define    DDR2_RDGR1_V             (DDR2CTL_BASE_V + 0x060)
#define    DDR2_RDGR2_V             (DDR2CTL_BASE_V + 0x064)
#define    DDR2_RDGR3_V             (DDR2CTL_BASE_V + 0x068)
#define    DDR2_DQTR0_V             (DDR2CTL_BASE_V + 0x06C)
#define    DDR2_DQTR1_V             (DDR2CTL_BASE_V + 0x070)
#define    DDR2_DQTR2_V             (DDR2CTL_BASE_V + 0x074)
#define    DDR2_DQTR3_V             (DDR2CTL_BASE_V + 0x078)
#define    DDR2_DQTR4_V             (DDR2CTL_BASE_V + 0x07C)
#define    DDR2_DQTR5_V             (DDR2CTL_BASE_V + 0x080)
#define    DDR2_DQTR6_V             (DDR2CTL_BASE_V + 0x084)
#define    DDR2_DQTR7_V             (DDR2CTL_BASE_V + 0x088)
#define    DDR2_DQTR8_V             (DDR2CTL_BASE_V + 0x08C)
#define    DDR2_DQSTR_V             (DDR2CTL_BASE_V + 0x090)
#define    DDR2_DQSBTR_V            (DDR2CTL_BASE_V + 0x094)
#define    DDR2_ODTCR_V             (DDR2CTL_BASE_V + 0x098)
#define    DDR2_DTR0_V              (DDR2CTL_BASE_V + 0x09C)
#define    DDR2_DTR1_V              (DDR2CTL_BASE_V + 0x0A0)
#define    DDR2_DTAR_V              (DDR2CTL_BASE_V + 0x0A4)
#define    DDR2_ZQCR0_V             (DDR2CTL_BASE_V + 0x0A8)
#define    DDR2_ZQCR1_V             (DDR2CTL_BASE_V + 0x0AC)
#define    DDR2_ZQSR_V              (DDR2CTL_BASE_V + 0x0B0)

#define    DDR2_MR_V                (DDR2CTL_BASE_V + 0x1F0)
#define    DDR2_EMR_V               (DDR2CTL_BASE_V + 0x1F4)
#define    DDR2_EMR2_V              (DDR2CTL_BASE_V + 0x1F8)
#define    DDR2_EMR3_V              (DDR2CTL_BASE_V + 0x1FC)

#define    DDR2_HPCR0_V             (DDR2CTL_BASE_V + 0x400)
#define    DDR2_HPCR1_V             (DDR2CTL_BASE_V + 0x404)
#define    DDR2_HPCR2_V             (DDR2CTL_BASE_V + 0x408)
#define    DDR2_HPCR3_V             (DDR2CTL_BASE_V + 0x40C)
#define    DDR2_HPCR4_V             (DDR2CTL_BASE_V + 0x410)
#define    DDR2_HPCR5_V             (DDR2CTL_BASE_V + 0x414)
#define    DDR2_HPCR6_V             (DDR2CTL_BASE_V + 0x418)
#define    DDR2_HPCR7_V             (DDR2CTL_BASE_V + 0x41C)
#define    DDR2_HPCR8_V             (DDR2CTL_BASE_V + 0x420)
#define    DDR2_HPCR9_V             (DDR2CTL_BASE_V + 0x424)
#define    DDR2_HPCR10_V            (DDR2CTL_BASE_V + 0x428)
#define    DDR2_HPCR11_V            (DDR2CTL_BASE_V + 0x42C)
#define    DDR2_HPCR12_V            (DDR2CTL_BASE_V + 0x430)
#define    DDR2_HPCR13_V            (DDR2CTL_BASE_V + 0x434)
#define    DDR2_HPCR14_V            (DDR2CTL_BASE_V + 0x438)
#define    DDR2_HPCR15_V            (DDR2CTL_BASE_V + 0x43C)
#define    DDR2_HPCR16_V            (DDR2CTL_BASE_V + 0x440)
#define    DDR2_HPCR17_V            (DDR2CTL_BASE_V + 0x444)
#define    DDR2_HPCR18_V            (DDR2CTL_BASE_V + 0x448)
#define    DDR2_HPCR19_V            (DDR2CTL_BASE_V + 0x44C)
#define    DDR2_HPCR20_V            (DDR2CTL_BASE_V + 0x450)
#define    DDR2_HPCR21_V            (DDR2CTL_BASE_V + 0x454)
#define    DDR2_HPCR22_V            (DDR2CTL_BASE_V + 0x458)
#define    DDR2_HPCR23_V            (DDR2CTL_BASE_V + 0x45C)
#define    DDR2_HPCR24_V            (DDR2CTL_BASE_V + 0x460)
#define    DDR2_HPCR25_V            (DDR2CTL_BASE_V + 0x464)
#define    DDR2_HPCR26_V            (DDR2CTL_BASE_V + 0x468)
#define    DDR2_HPCR27_V            (DDR2CTL_BASE_V + 0x46C)
#define    DDR2_HPCR28_V            (DDR2CTL_BASE_V + 0x470)
#define    DDR2_HPCR29_V            (DDR2CTL_BASE_V + 0x474)
#define    DDR2_HPCR30_V            (DDR2CTL_BASE_V + 0x478)
#define    DDR2_HPCR31_V            (DDR2CTL_BASE_V + 0x47C)

#define    DDR2_PQCR0_V             (DDR2CTL_BASE_V + 0x480)
#define    DDR2_PQCR1_V             (DDR2CTL_BASE_V + 0x484)
#define    DDR2_PQCR2_V             (DDR2CTL_BASE_V + 0x488)
#define    DDR2_PQCR3_V             (DDR2CTL_BASE_V + 0x48C)
#define    DDR2_PQCR4_V             (DDR2CTL_BASE_V + 0x490)
#define    DDR2_PQCR5_V             (DDR2CTL_BASE_V + 0x494)
#define    DDR2_PQCR6_V             (DDR2CTL_BASE_V + 0x498)
#define    DDR2_PQCR7_V             (DDR2CTL_BASE_V + 0x49C)

#define    DDR2_MMGCR_V             (DDR2CTL_BASE_V + 0x4A0)

#endif // DDR2CTL_BASE_V

#endif

