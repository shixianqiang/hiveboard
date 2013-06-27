/* linux/arch/unicore/mach-sep0611/include/mach/regs-i2s.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *  cgm <chenguangming@wxseuic.com>
 *
 * Southeast University ASIC SoC support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  05-31-2011	cgm initial version
 *
 */
#ifndef __ASM_ARCH_REGS_I2S_H
#define __ASM_ARCH_REGS_I2S_H

/* global */
#define    I2S_IER                  0x000
#define    I2S_IRER                 0x004
#define    I2S_ITER                 0x008
#define    I2S_CER                  0x00C
#define    I2S_CCR                  0x010
#define    I2S_RXFFR                0x014
#define    I2S_TXFFR                0x018

/* channel 0*/
#define    I2S_LRBR0                0x020
#define    I2S_LTHR0                0x020
#define    I2S_RRBR0                0x024
#define    I2S_RTHR0                0x024
#define    I2S_RER0                 0x028
#define    I2S_TER0                 0x02C
#define    I2S_RCR0                 0x030
#define    I2S_TCR0                 0x034
#define    I2S_ISR0                 0x038
#define    I2S_IMR0                 0x03C
#define    I2S_ROR0                 0x040
#define    I2S_TOR0                 0x044
#define    I2S_RFCR0                0x048
#define    I2S_TFCR0                0x04C
#define    I2S_RFF0                 0x050
#define    I2S_TFF0                 0x054

/* channel 1*/
#define    I2S_LRBR1                0x060
#define    I2S_LTHR1                0x060
#define    I2S_RRBR1                0x064
#define    I2S_RTHR1                0x064
#define    I2S_RER1                 0x068
#define    I2S_TER1                 0x06C
#define    I2S_RCR1                 0x070
#define    I2S_TCR1                 0x074
#define    I2S_ISR1                 0x078
#define    I2S_IMR1                 0x07C
#define    I2S_ROR1                 0x080
#define    I2S_TOR1                 0x084
#define    I2S_RFCR1                0x088
#define    I2S_TFCR1                0x08C
#define    I2S_RFF1                 0x090
#define    I2S_TFF1                 0x094

/* channel 2*/
#define    I2S_LRBR2                0x0A0
#define    I2S_LTHR2                0x0A0
#define    I2S_RRBR2                0x0A4
#define    I2S_RTHR2                0x0A4
#define    I2S_RER2                 0x0A8
#define    I2S_TER2                 0x0AC
#define    I2S_RCR2                 0x0B0
#define    I2S_TCR2                 0x0B4
#define    I2S_ISR2                 0x0B8
#define    I2S_IMR2                 0x0BC
#define    I2S_ROR2                 0x0C0
#define    I2S_TOR2                 0x0C4
#define    I2S_RFCR2                0x0C8
#define    I2S_TFCR2                0x0CC
#define    I2S_RFF2                 0x0D0
#define    I2S_TFF2                 0x0D4

/* channel 3*/
#define    I2S_LRBR3                0x0E0
#define    I2S_LTHR3                0x0E0
#define    I2S_RRBR3                0x0E4
#define    I2S_RTHR3                0x0E4
#define    I2S_RER3                 0x0E8
#define    I2S_TER3                 0x0EC
#define    I2S_RCR3                 0x0F0
#define    I2S_TCR3                 0x0F4
#define    I2S_ISR3                 0x0F8
#define    I2S_IMR3                 0x0FC
#define    I2S_ROR3                 0x100
#define    I2S_TOR3                 0x104
#define    I2S_RFCR3                0x108
#define    I2S_TFCR3                0x10C
#define    I2S_RFF3                 0x110
#define    I2S_TFF3                 0x114
#define    I2S_SCR		    		0x118

/* I2S DMA CTL REG*/
#define    I2S_RXDMA                0x1C0
#define    I2S_RRXDMA               0x1C4
#define    I2S_TXDMA                0x1C8
#define    I2S_RTXDMA               0x1CC

/* CONSTANT REG*/
#define    I2S_COMP_PARAM2          0x1F0
#define    I2S_COMP_PARAM1          0x1F4
#define    I2S_COMP_VERSION         0x1F8
#define    I2S_COMP_TYPE            0x1FC

#endif /* __ASM_ARCH_REGS_I2S_H */
