/*
 * drivers/usb/musb/sep0611_musb.h
 * Code specific to ASIC center of SouthEast University
 * 
 * Maintained by Fan Guisheng (Neo)<fgsink@gmail.com>
 * 
 * Copyright (C) 2001-2011 ASIC & SouthEast University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Based on: musb/blackfin_usb.h
 * Contributors & Additions/Fixes:
 *  First version by fgs(Neo)
 * 
 * TODO:
 */

#ifndef __SEP0611_MUSB_H__
#define __SEP0611_MUSB_H__

#include <asm/hardware.h>

#define SEP0611_USB_BASE USB_BASE 

#define EP0_INT                	0x01  // Endpoint 0, Control   
#define EP1_INT                 0x02  // Endpoint 1, (Bulk-In) 
#define EP2_INT                 0x04  // Endpoint 2, (Bulk-Out)  
#define EP3_INT			 	    0x08  // Endpoint 3
#define EP4_INT			 		0x10  // Endpoint 4

static inline struct musb_regs *sep0611_get_base_usb_device(void)
{
	return (struct musb_regs *)SEP0611_USB_BASE;
}

#endif
