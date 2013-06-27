/*
 * drivers/usb/musb/sep0611_musb.c
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
 * Based on: musb/blackfin_usb.c
 * Contributors & Additions/Fixes:
 *  First version by fgs(Neo)
 * 
 * TODO:
 */

#include <common.h>

#include <usb.h>

#include "musb_core.h"
#include "sep0611_musb.h"
#include <asm/hardware.h>

void musb_gpio_init(void);

/* MUSB platform configuration */
struct musb_config musb_cfg = {
	.regs       = (struct musb_regs *)SEP0611_USB_BASE,
	.timeout    = 0x3FFFFFF,
	.musb_speed = 0,
};

/*
 * This function read or write data to endpoint fifo
 *
 * ep		- Endpoint number
 * length	- Number of bytes to write to FIFO
 * fifo_data- Pointer to data buffer to be read/write
 * is_write	- Flag for read or write
 */
void rw_fifo(u8 ep, u32 length, void *fifo_data, int is_write)
{
	int i;  	
	struct musb_regs * const musbr	= sep0611_get_base_usb_device(); 
	u8  *data = (u8 *)fifo_data;

	writeb(ep, &musbr->index);
	if(is_write)
	{
   		for(i=0;i<length;i++)
    	{
			writeb(*data++, &musbr->fifox[ep]); 
    	}
	}
	else
	{
		for(i=0;i<length;i++)
    	{
			*data++ = readb(&musbr->fifox[ep]);
    	}
	}
}


void write_fifo(u8 ep, u32 length, void *fifo_data)
{
	rw_fifo(ep, length, fifo_data, 1);
}

void read_fifo(u8 ep, u32 length, void *fifo_data)
{
	rw_fifo(ep, length, fifo_data, 0);
}

int musb_platform_init(void)
{
	struct musb_regs * const musbr  = sep0611_get_base_usb_device();   
	unsigned int pmu_clk_gt_cfg1 = 0;
	u8 devctl = 0;
	u32 tmp32;

	pmu_clk_gt_cfg1 = readl(PMU_CLK_GT_CFG1);
	pmu_clk_gt_cfg1 |= 1<<6;
	writel(PMU_CLK_GT_CFG1,pmu_clk_gt_cfg1);

	musb_gpio_init();

	writeb(0, &musbr->index); 

    	devctl = readb(&musbr->devctl);
    	devctl |= MUSB_DEVCTL_SESSION; 
    	writeb(devctl, &musbr->devctl);

    	writeb((EP0_INT | EP1_INT), &musbr->intrtxe); 
    	writeb((EP3_INT), &musbr->intrrxe); 
    	writeb((MUSB_INTR_RESET | MUSB_INTR_SUSPEND | MUSB_INTR_RESUME), &musbr->intrusbe); 

	int_enable(40UL);

	return 0;
}

void musb_gpio_init(void)
{
//Init USB_EN
#ifdef SEP0611_USB_EN
    GPIO_OUT_HIGH(SEP0611_USB_EN);
#endif
//Init RESET RHY
#ifdef SEP0611_USB_RST
	GPIO_OUT_HIGH(SEP0611_USB_RST);
#endif
//Init USB_EN
#ifdef SEP0611_USB_EN
	GPIO_OUT_HIGH(SEP0611_USB_EN);
#endif
//Init USB_DE_SEL
#ifdef SEP0611_USB_DE_SEL
	GPIO_OUT_LOW(SEP0611_USB_DE_SEL);
#endif
//Init VBUS_IN_SEL
#ifdef SEP0611_VBUS_IN_SEL
	GPIO_OUT_HIGH(SEP0611_VBUS_IN_SEL);
#endif
//Init OTG_VBUS_ON
#ifdef SEP0611_OTG_VBUS_ON
	GPIO_OUT_LOW(SEP0611_OTG_VBUS_ON);
#endif
//Init HVBUS_ON
#ifdef SEP0611_HVBUS_ON
	GPIO_OUT_LOW(SEP0611_HVBUS_ON);
#endif
//Init HID_VAL
#ifdef SEP0611_HID_VAL
	GPIO_OUT_LOW(SEP0611_HID_VAL);
#endif
}
