/* linux/arch/unicore/mach-sep0611/devices.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *
 * Southeast University ASIC SoC support
 *
 * SEP0611 board devices declaration
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


#ifndef __ASM_ARCH_DEVICES_H
#define __ASM_ARCH_DEVICES_H

extern struct platform_device sep0611_device_serial;
extern struct platform_device sep0611_device_dm9000;
extern struct platform_device sep0611_device_lcd;
#ifdef CONFIG_SEP0611_OVERLAY
extern struct platform_device sep0611_device_overlay;
#endif
extern struct platform_device sep0611_device_nand;
extern struct platform_device sep0611_device_spi1;
extern struct platform_device sep0611_device_spi2;
extern struct platform_device sep0611_device_spi3;
extern struct platform_device sep0611_device_i2c1;
extern struct platform_device sep0611_device_i2c2;
extern struct platform_device sep0611_device_i2c3;
extern struct platform_device sep0611_device_usb;
extern struct platform_device sep0611_device_mmc;
extern struct platform_device sep0611_device_sdio;

extern struct platform_device sep0611_device_rtc;
extern struct platform_device sep0611_device_gps;
/************comment by zyt
extern struct platform_device android_pmem_device;
extern struct platform_device android_pmem_adsp_device;
extern struct platform_device android_pmem_vpu_device;

extern struct platform_device usb_mass_storage_device;
extern struct platform_device android_usb_device;
comment by zyt***************/



#endif /* __ASM_ARCH_DEVICES_H */

