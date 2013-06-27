/* linux/arch/unicore/mach-sep0611/include/mach/regs-spi.h
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

#ifndef __ASM_ARCH_REGS_SPI_H
#define __ASM_ARCH_REGS_SPI_H

#define SSI_CONTROL0   			(0x00)
#define SSI_CONTROL1			(0x04)
#define SSI_ENR   				(0x08)
#define SSI_SER   				(0x10)
#define SSI_BAUDR   			(0x14)
#define SSI_TXFTLR   			(0x18)
#define SSI_RXFTLR   			(0x1C)
#define SSI_TXFLR   			(0x20)
#define SSI_RXFLR   			(0x24)
#define SSI_SR        			(0x28)
#define SSI_IMR      			(0x2C)
#define SSI_ISR      			(0x30)
#define SSI_RISR     			(0x34)
#define SSI_TXOICR   			(0x38)
#define SSI_RXOICR   			(0x3C)
#define SSI_RXUICR   			(0x40)
#define SSI_MSTICR   			(0x44)
#define SSI_ICR    				(0x48)
#define SSI_DMACR            	(0x4C)
#define SSI_DMATDLR        		(0x50)
#define SSI_DMARDLR         	(0x54)
#define SSI_IDR             	(0x58)
#define SSI_SSI_COMP_VERSION  	(0x5C)
#define SSI_DR              	(0x60)

#endif /* __ASM_ARCH_REGS_SPI_H */
