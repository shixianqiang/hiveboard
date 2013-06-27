/*
 * drivers/usb/dnw/dnw.h
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
 * Contributors & Additions/Fixes:
 *  First version by fgs(Neo)
 * 
 * TODO:
 */

#ifndef __DNW_H__
#define __DNW_H__

#define NUM_CONFIGS         1
#define MAX_INTERFACES      1   
#define NUM_ENDPOINTS       2   //except ep0
#define NUM_DNW_INTERFACES  1

#define BULK_PKT_SIZE	    512 

#define EP0_PKT_SIZE        64	
#define EP1_PKT_SIZE        BULK_PKT_SIZE
#define EP2_PKT_SIZE        BULK_PKT_SIZE

#define STR_LANG		    0x00
#define STR_MANUFACTURER	0x01
#define STR_PRODUCT		    0x02
#define STR_SERIAL		    0x03
#define STR_COUNT		    0x04

#define CONFIG_USBD_VENDORID		0x5345
#define CONFIG_USBD_PRODUCTID		0x1234
#define CONFIG_USBD_MANUFACTURER	"SEUIC"
#define CONFIG_USBD_PRODUCT_NAME	"dnw for SEP0611"

#endif
