 /* board/seuic/sep0611/include/board-terminal.h
 *
 * Copyright (c) 2009-2012 Prochip
 *  woody.wu
 *
 * Southeast University ASIC SoC support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  07-23-2012	woody initial version
 *
 */

#ifndef __ASM_BOARD_TERMINAL_H
#define __ASM_BOARD_TERMINAL_H

#include <asm/arch/regs-gpio.h>

/*FOR POWER MANAGEMENT*/
#define SEP0611_HOLD_POWER      SEP0611_AO2
#define SEP0611_DDR_PWREN       SEP0611_GPI4
#define SEP0611_DC5V_DETC		SEP0611_GPI12
#define SEP0611_BAT_LOW		    SEP0611_AO3
//#define SEP0611_CHG_EN
//#define SEP0611_POWER_OK
#define SEP0611_CHG_OK          SEP0611_GPI9
#define SEP0611_PWROFF_DET      SEP0611_GPI5

/*FOR AD*/
//#define SEP0611_TOUCH_INT		    SEP0611_AO0

/*FOR USB (DNW)*/
#define SEP0611_USB_RST		    SEP0611_GPC2
#define SEP0611_USB_EN          SEP0611_GPD5
//#define SEP0611_OTG_VBUS_ON		SEP0611_GPD4
#define SEP0611_USB5V_EN		SEP0611_GPD4

/*FOR LAN ETH (TFTP\PING) */
//#define SEP0611_ETH_CMD
#define SEP0611_ETH_INT			SEP0611_GPI6
#define SEP0611_ETH_RST_        SEP0611_GPC3
#define SEP0611_ETH_EN          SEP0611_GPE1
#define SEP0611_ETH_CMD			SEP0611_GPA10

/*FOR LED TEST */
#define SEP0611_KB_LED          SEP0611_GPF4

/*FOR LCD&BACKLIGHT */
#define SEP0611_LCD_EN			SEP0611_GPE3
#define SEP0611_LCD_BL_EN		SEP0611_GPE2
#define SEP0611_LCD_RST         SEP0611_GPC15

/*INTERRUPT DEFINE IN SEP6200A */
#define SEP0611_ETH_INTSRC		INTSRC_EXT6
#define SEP0611_TOUCH_INTSRC	    INTSRC_AO_EXT0
//#define SEP0611_KB_INTSRC

#endif /* __ASM_BOARD_TERMINAL_H */

