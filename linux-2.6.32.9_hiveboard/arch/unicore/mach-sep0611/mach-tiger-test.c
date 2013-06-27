/* linux/arch/unicore/mach-sep0611/mach_sep0611.c
 *
 * Copyright (c) 2009-2011 SEUIC
 *
 * Southeast University ASIC SoC support
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

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/serial_8250.h>
#include <linux/spi/spi.h>
#include <linux/i2c.h>
#include <linux/gpio_keys.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/mtd.h>

#include <asm/io.h>
#include <asm/setup.h>
#include <asm/mach/arch.h>

#include <mach/hardware.h>
#include <mach/irqs.h>
#include <mach/devices.h>
#include <mach/nand.h>

#include <board/board.h>

#define MACH_TYPE_TB0611 2556

extern struct sys_timer sep0611_timer;
extern void sep0611_map_io(void);
extern void sep0611_init_irq(void);


/***************SEP0611 tuchscreen (AR1020 SSI module)****************/

static struct platform_device sep0611_device_ts = {
	.name	= "sep0611_ts",
	.id		= -1,
};

/*****************SEP0611 GPIO keyboard support**********************************************/

static struct gpio_keys_button sep0611_buttons[] = {
	[0] = {
		.gpio		= SEP0611_GPI13,
		.code		= 139,
		.desc		= "MENU",
		.active_low	= 1,
		.wakeup		= 0,
	},
	
	[1] = {
		.gpio		= SEP0611_GPI14,
		.code		= 102,
		.desc		= "home",
		.active_low	= 1,
		.wakeup		= 0,
	},
	
	[2] = {
		.gpio		= SEP0611_GPI15,
		.code		= 158,
		.desc		= "back",
		.active_low	= 1,
		.wakeup		= 0,
	},
};

static struct gpio_keys_platform_data sep0611_button_data = {
	.buttons	= sep0611_buttons,
	.nbuttons	= ARRAY_SIZE(sep0611_buttons),
};

static struct platform_device sep0611_device_button = {
	.name	= "gpio-keys",
	.id		= -1,
	.dev	= {
		.platform_data	= &sep0611_button_data,
	},
};

static struct mtd_partition __initdata sep0611_mtd_parts[] = {
	[0] = {
		.name 	= "Bootloader",
		.offset	= 0,
		.size 	= MTD_SIZE_1MiB * 4,
		.mask_flags = MTD_WRITEABLE,
	},

	[1] = {
		.name	= "Kernel",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= MTD_SIZE_1MiB * 4,
		.mask_flags = MTD_WRITEABLE,
	},
	
	[2] = {
		.name	= "File System",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= MTDPART_SIZ_FULL,
	}
};

static struct spi_board_info __initdata sep0611_spi_board[] = {
	[0] = {
		.modalias	= "ar1020",
    	.mode   	= SPI_MODE_0,
   		.irq        = INTSRC_EXT11,
        .max_speed_hz	= 120000 * 16,
    	.bus_num    = 2,
  		.chip_select= 0,
	},
};

static struct i2c_board_info __initdata sep0611_i2c1_board_info[]  = {
	[0] = {
		I2C_BOARD_INFO("tps65010", 0x48),
	},
	
	[1] = {
		I2C_BOARD_INFO("WM8976", 0x1A),
	},
};

static struct platform_device hsd050_lcd = {
	.name	= "hsd050_lcd",
	.dev	= {
		//.platform_data	= &lcd_pdata,
	},
};

static struct platform_device *devices[] __initdata = 
{
	&sep0611_device_serial,
	&sep0611_device_dm9000,
	&sep0611_device_lcd,
	&sep0611_device_nand,
	&sep0611_device_spi1,
	&sep0611_device_spi2,
	&sep0611_device_spi3,
	&sep0611_device_i2c1,
	&sep0611_device_i2c2,
	&sep0611_device_i2c3,
 	&sep0611_device_mmc,
 	&sep0611_device_sdio,
	&sep0611_device_usb,
	&sep0611_device_rtc,
	&sep0611_device_button,
	&sep0611_device_ts,
	&sep0611_device_gps,
/*************comment by zyt
#ifdef CONFIG_ANDROID_PMEM
	&android_pmem_device,
	//&android_pmem_adsp_device,
	//&android_pmem_vpu_device,
#endif
#ifdef CONFIG_USB_ANDROID
	&usb_mass_storage_device,
	&android_usb_device,
#endif
comment by zyt**************/
	&hsd050_lcd,
};

void __init sep0611_init(void)
{	
	SEP0611_INT_ENABLE(INTSRC_UART0);
	SEP0611_INT_ENABLE(INTSRC_UART1);
	SEP0611_INT_ENABLE(INTSRC_UART2);
	SEP0611_INT_ENABLE(INTSRC_UART3);

	platform_add_devices(devices, ARRAY_SIZE(devices));
	
	/* board infomation for I2C '1' */
	i2c_register_board_info(1, sep0611_i2c1_board_info, ARRAY_SIZE(sep0611_i2c1_board_info));
	
	spi_register_board_info(sep0611_spi_board, ARRAY_SIZE(sep0611_spi_board));
	
	sep0611_partitions.table = sep0611_mtd_parts;
	sep0611_partitions.num = ARRAY_SIZE(sep0611_mtd_parts);	
}

MACHINE_START(TB0611, "SEP0611 board")
	.phys_io		= 0xB1000000,
	.io_pg_offst	= (SEP_IO_BASE_V >> 18) & 0xFFFC,
	.boot_params	= 0x40000100,
	.map_io			= sep0611_map_io,
	.init_irq		= sep0611_init_irq,
	.init_machine	= sep0611_init,
	.timer			= &sep0611_timer,
MACHINE_END

