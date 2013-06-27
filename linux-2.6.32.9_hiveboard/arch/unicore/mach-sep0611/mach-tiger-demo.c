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
#include <linux/spi/ad7879.h>
#include <linux/i2c.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/mtd.h>

#include <asm/io.h>
#include <asm/setup.h>
#include <asm/mach/arch.h>

#include <mach/hardware.h>
#include <mach/irqs.h>
#include <mach/devices.h>
#include <mach/nand.h>
#include <board/board-demo.h>

#include <linux/gpio_keys.h>
#include <linux/input.h>
#include <board/board.h>

#define MACH_TYPE_DDB0611 2556

extern struct sys_timer sep0611_timer;
extern void sep0611_map_io(void);
extern void sep0611_init_irq(void);

static struct platform_device sep0611_device_keyboard = {
	.name	= "sep0611-keypad",
	.id		= -1,
};

static struct gpio_keys_button sep0611_buttons[] = {
	[0] = {
		.gpio		= SEP0611_GPI15,
		.code	= KEY_END,
		.desc		= "back",
		.active_low	= 0,
		.wakeup		= 1,
		.debounce_interval = 20,
	},
};

static struct gpio_keys_platform_data sep0611_button_data = {
	.buttons	= sep0611_buttons,
	.nbuttons	= ARRAY_SIZE(sep0611_buttons),
};

static struct platform_device sep0611_device_gpiokeys = {
	.name	= "gpio-keys",
	.id		= -1,
	.dev	= {
		.platform_data	= &sep0611_button_data,
	},
};




static struct platform_device sep0611_device_battery = {
	.name	= "sep0611-battery",
	.id		= -1,
};

static struct platform_device sep0611_device_backlight = {
	.name 	= "sep0611-backlight",
	.id		= -1,
};


static const struct ad7879_platform_data sep0611_ad7879_ts_info = {
	.model			= 7879,	/* Model = AD7879 */
	.x_plate_ohms	= 620,	/* 620 Ohm from the touch datasheet */
	.pressure_max	= 10000,
	.pressure_min	= 0,
	.first_conversion_delay = 3,	/* wait 512us before do a first conversion */
	.acquisition_time 		= 1,	/* 4us acquisition time per sample */
	.median			= 2,	/* do 8 measurements */
	.averaging 		= 1,	/* take the average of 4 middle samples */
	.pen_down_acc_interval 	= 255,	/* 9.4 ms */
	.gpio_output	= 1,	/* configure AUX/VBAT/GPIO as GPIO output */
	.gpio_default 	= 1,	/* During initialization set GPIO = HIGH */
};

static struct i2c_board_info __initdata sep0611_i2c1_board_info[]  = {
	[0] = {
		I2C_BOARD_INFO("tps65010", 0x48),
	},
	
	[1] = {
		I2C_BOARD_INFO("WM8976", 0x1A),
	},
	
	[2] = {
		I2C_BOARD_INFO("ad7879", 0x2c),
		.irq			= INTSRC_EXT11,
		.platform_data	= &sep0611_ad7879_ts_info,
	},
	
	[3] = {
		I2C_BOARD_INFO("ad7997", 0x21),
	},

	[4] = {
		I2C_BOARD_INFO("hdmi", 0x4c),
	},
};

static struct i2c_board_info __initdata sep0611_i2c2_board_info[]  = {
	[0] = {
		I2C_BOARD_INFO("mma7660", 0x4c),
	},
};

static struct mtd_partition __initdata sep0611_mtd_parts[] = {
	[0] = {
		.name 	= "lk",
		.offset	= 0,
		.size 	= MTD_SIZE_1MiB * 2,
		.mask_flags = MTD_WRITEABLE,
	},

	[1] = {
		.name	= "misc",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= MTD_SIZE_1MiB * 2,
		//.mask_flags = MTD_WRITEABLE, //misc write is enable
	},
	
	[2] = {
		.name	= "recovery",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= MTD_SIZE_1MiB * 10,
		.mask_flags = MTD_WRITEABLE,
	},
	
	[3] = {
		.name	= "boot",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= MTD_SIZE_1MiB * 10,
		.mask_flags = MTD_WRITEABLE,
	},
	
	[4] = {
		.name	= "system",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= MTD_SIZE_1MiB * 800,
	},

	[5] = {
		.name	= "cache",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= MTD_SIZE_1MiB * 100,
	},
	
	[6] = {
		.name	= "userdata",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= MTD_SIZE_1MiB * 100,
	},

	[7] = {
		.name	= "reserved",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= MTDPART_SIZ_FULL,
	}
};

#ifdef CONFIG_LCD_AT070TN93
static struct platform_device at070tn93_lcd = {
	.name	= "at070tn93_lcd",
	.dev	= {
		//.platform_data	= &lcd_pdata,
	},
};
#endif

static struct platform_device *devices[] __initdata = 
{
	&sep0611_device_i2c1,
	&sep0611_device_i2c2,
	&sep0611_device_i2c3,
	&sep0611_device_rtc,
	&sep0611_device_nand,
	&sep0611_device_serial,
	&sep0611_device_dm9000,
	&sep0611_device_lcd,
#ifdef CONFIG_LCD_AT070TN93
	&at070tn93_lcd,
#endif
#ifdef CONFIG_SEP0611_OVERLAY
	&sep0611_device_overlay,
#endif
	&sep0611_device_backlight,
	&sep0611_device_mmc,
	&sep0611_device_sdio,
	&sep0611_device_gps,
	&sep0611_device_gpiokeys,
	&sep0611_device_keyboard,
	&sep0611_device_battery,
#ifdef CONFIG_ANDROID_PMEM
	&android_pmem_device,
	//&android_pmem_adsp_device,
	//&android_pmem_vpu_device,
#endif
	&sep0611_device_usb,
#ifdef CONFIG_USB_ANDROID
	&usb_mass_storage_device,
	&android_usb_device,
#endif
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

	/* board infomation for I2C '2' */
	i2c_register_board_info(2, sep0611_i2c2_board_info, ARRAY_SIZE(sep0611_i2c2_board_info));	
	
	sep0611_partitions.table = sep0611_mtd_parts;
	sep0611_partitions.num = ARRAY_SIZE(sep0611_mtd_parts);	
}

MACHINE_START(DDB0611, "SEP0611 board")
	.phys_io		= 0xB1000000,
	.io_pg_offst	= (SEP_IO_BASE_V >> 18) & 0xFFFC,
	.boot_params	= 0x40000100,
	.map_io			= sep0611_map_io,
	.init_irq		= sep0611_init_irq,
	.init_machine	= sep0611_init,
	.timer			= &sep0611_timer,
MACHINE_END

