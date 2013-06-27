/* linux/arch/unicore/mach-sep0611/include/mach/regs-lcd.h
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

#ifndef	__ASM_ARCH_REGS_LCD_H
#define	__ASM_ARCH_REGS_LCD_H

#define LCD_ECR							0X00
#define LCD_BECR						0X04
#define LCD_W1ECR						0X08
#define LCD_W2ECR						0X0C
#define LCD_CECR						0X10
#define LCD_BCR							0X14
#define LCD_PLCR						0X18
#define LCD_PFCR						0X1C
#define LCD_PCCR						0X20
#define LCD_IOCR						0X24
#define LCD_IECR						0X28
#define LCD_W1CKR						0X2C
#define LCD_W2CKR						0X30	
#define LCD_W1BVR						0X34
#define LCD_W2BVR						0X38
#define LCD_SCR							0X3C
#define LCD_W1TPCR						0X40
#define LCD_W1BPCR						0X44
#define LCD_W2TPCR						0X48
#define LCD_W2BPCR						0X4C
#define LCD_CTPCR						0X50
#define LCD_CBPCR						0X54
#define LCD_ACSR						0X58
#define LCD_BBSAR						0X5C
#define LCD_W1YBSAR						0X60
#define LCD_W1UBSAR						0X64
#define LCD_W1VBSAR						0X68
#define LCD_W2BSAR						0X6C
#define LCD_CBSAR						0X70
#define LCD_CC0R						0X8C
#define LCD_CC1R						0X90
#define LCD_CC2R						0X94
#define LCD_CC3R						0X98
#define LCD_BASE_RAW_IMAGE_WIDTH		0x09c
#define LCD_OVER1_RAW_IMAGE_WIDTH		0x0A0
#define LCD_OVER2_RAW_IMAGE_WIDTH		0x0A4
#define LCD_BTPCR						0x0B4
#define LCD_BBPCR						0x0B8
#define LCD_BAKGRND_COLOR				0XBC
#define LCD_R_ORDER						0XC0
#define LCD_G_ORDER						0XC4
#define LCD_B_ORDER						0XC8
#define LCD_RGB_ORDER					0XCC

#endif /* __ASM_ARCH_REGS_LCD_H */

