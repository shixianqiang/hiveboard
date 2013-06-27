/* linux/arch/unicore/mach-sep0611/include/mach/irqs.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *
 * Southeast University ASIC SoC support
 *
 * This file contains the irq num definitions of the SEP0611
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
 
#ifndef __ASM_ARCH_IRQS_H
#define __ASM_ARCH_IRQS_H

#define    NR_IRQS				64

/* define the interrupt source number */
#define    INTSRC_RESERVE2      63
#define    INTSRC_RESERVE1      62
#define    INTSRC_LCDC          61
#define    INTSRC_GPU           60
#define    INTSRC_VPU           59
#define    INTSRC_TIMER3        58
#define    INTSRC_TIMER2        57
#define    INTSRC_TIMER1        56
#define    INTSRC_NAND          55
#define    INTSRC_I2S           54
#define    INTSRC_I2C3          53
#define    INTSRC_I2C2          52
#define    INTSRC_I2C1          51
#define    INTSRC_SSI3          50
#define    INTSRC_SSI2          49
#define    INTSRC_SSI1          48
#define    INTSRC_SDIO2         47
#define    INTSRC_SDIO1         46
#define    INTSRC_UART3         45
#define    INTSRC_UART2         44
#define    INTSRC_UART1         43
#define    INTSRC_UART0         42
#define    INTSRC_PWM           41
#define    INTSRC_USB           40
#define    INTSRC_USBDMA        39
#define    INTSRC_DMAC2         38
#define    INTSRC_DMAC1         37
#define    INTSRC_PMUIRQ_A11    36
#define    INTSRC_DMAIRQ_A11    35
#define    INTSRC_GPS           34
#define    INTSRC_RTC           33
#define    INTSRC_RESERVED16    32
#define    INTSRC_PORTE12       31
#define    INTSRC_PORTE11       30
#define    INTSRC_PORTE10       29
#define    INTSRC_PORTE9        28
#define    INTSRC_PORTE5        27
#define    INTSRC_PORTE4        26
#define    INTSRC_PORTD9        25
#define    INTSRC_PORTD8        24
#define    INTSRC_PORTD3        23
#define    INTSRC_PORTD2        22
#define    INTSRC_PORTD1        21
#define    INTSRC_PORTD0        20
#define    INTSRC_PORTC3        19
#define    INTSRC_PORTC2        18
#define    INTSRC_PORTC1        17
#define    INTSRC_PORTC0        16
#define    INTSRC_EXT15         15
#define    INTSRC_EXT14         14
#define    INTSRC_EXT13         13
#define    INTSRC_EXT12         12
#define    INTSRC_EXT11         11
#define    INTSRC_EXT10         10
#define    INTSRC_EXT9          9
#define    INTSRC_EXT8          8
#define    INTSRC_EXT7          7
#define    INTSRC_EXT6          6
#define    INTSRC_EXT5          5
#define    INTSRC_EXT4          4
#define    INTSRC_AO_EXT3     	3
#define    INTSRC_AO_EXT2   	2
#define    INTSRC_AO_EXT1     	1
#define    INTSRC_AO_EXT0       0


#define SEP0611_INT_SET(intnum)                       				\
do{                                                   				\
	if(intnum < 32)                                 				\
  		*(volatile unsigned long*)VIC_SFT_INT_L_V |= (1 << intnum);	\
  	else                                            				\
     	*(volatile unsigned long*)VIC_SFT_INT_H_V |= (1 << (intnum - 32));	\
}while(0)

#define SEP0611_INT_CLR(intnum)                           			\
do{                                                     			\
	if(intnum < 32)                                   				\
   		*(volatile unsigned long*)VIC_SFT_INT_L_V &= ~(1 << intnum);\
 	else                                              				\
      	*(volatile unsigned long*)VIC_SFT_INT_H_V &= ~(1 << (intnum - 32));	\
}while(0)

#define SEP0611_INT_ENABLE(intnum)                            		\
do{                                                   				\
	if(intnum < 32)                                 				\
		*(volatile unsigned long*)VIC_INT_EN_L_V |= (1 << intnum);	\
	else                                            				\
		*(volatile unsigned long*)VIC_INT_EN_H_V |= (1 << (intnum - 32));	\
}while(0)

#define SEP0611_INT_DISABLE(intnum)                      			\
do{                                                   				\
	if(intnum < 32)                                 				\
  		*(volatile unsigned long*)VIC_INT_EN_L_V &= ~(1 << intnum);	\
  	else                                            				\
    	*(volatile unsigned long*)VIC_INT_EN_H_V &= ~(1 << (intnum - 32)); 	\
}while(0)


#endif /* __ASM_ARCH_IRQS_H__ */

