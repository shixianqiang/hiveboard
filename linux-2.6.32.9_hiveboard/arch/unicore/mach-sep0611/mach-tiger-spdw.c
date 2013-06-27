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
#include <linux/gpio_keys.h>

#include <asm/io.h>
#include <asm/setup.h>
#include <asm/mach/arch.h>

#include <mach/hardware.h>
#include <mach/irqs.h>
#include <mach/devices.h>
#include <mach/nand.h>
#include <mach/regs-gpio.h>

#include <board/board.h>

#define MACH_TYPE_DDB0611 2556

extern struct sys_timer sep0611_timer;
extern void sep0611_map_io(void);
extern void sep0611_init_irq(void);


unsigned char * names[] = {
	"GPIO A",
	"GPIO B",
	"GPIO C",
	"GPIO D",
	"GPIO E",
	"GPIO F",
	"GPIO G",
	"GPIO H",
	"GPIO I",
	"GPIO AO",
};

static void gpio_get_name(unsigned long gpio)
{
	unsigned long port, pin;
	
	port = gpio >> 5;
	pin = gpio & 31;
	
	printk("%s bit %ld:", names[port], pin);
}

static void gpio_low_io(unsigned long gpio)
{
	sep0611_gpio_cfgpin(gpio, SEP0611_GPIO_IO);	/* GPIO */
	sep0611_gpio_dirpin(gpio, SEP0611_GPIO_OUT);/* output */
	sep0611_gpio_setpin(gpio, GPIO_LOW);/* output */
	
	gpio_get_name(gpio);
	printk("%d\n", sep0611_gpio_getpin(gpio));
}

static void gpio_high_io(unsigned long gpio)
{
	sep0611_gpio_cfgpin(gpio, SEP0611_GPIO_IO);	/* GPIO */
	sep0611_gpio_dirpin(gpio, SEP0611_GPIO_OUT);/* output */
	sep0611_gpio_setpin(gpio, GPIO_HIGH);/* output */
		
	gpio_get_name(gpio);
	printk("%d\n", sep0611_gpio_getpin(gpio));
}


int gpio_init(void)
{
	gpio_high_io(SEP0611_GPE3);
	gpio_low_io(SEP0611_GPE11);
	gpio_high_io(SEP0611_GPE15);
	gpio_high_io(SEP0611_GPE16);
	gpio_low_io(SEP0611_GPE17);
	gpio_low_io(SEP0611_GPA11);
	gpio_low_io(SEP0611_GPA12);
	
	sep0611_gpio_cfgpin(SEP0611_GPI5, SEP0611_GPIO_IO);	/* GPIO */
	sep0611_gpio_dirpin(SEP0611_GPI5, SEP0611_GPIO_IN);
	
	sep0611_gpio_cfgpin(SEP0611_AO3, SEP0611_GPIO_IO);	/* GPIO */
	sep0611_gpio_dirpin(SEP0611_AO3, SEP0611_GPIO_IN);
	
	sep0611_gpio_cfgpin(SEP0611_GPI7, SEP0611_GPIO_IO);	/* GPIO */
	sep0611_gpio_dirpin(SEP0611_GPI7, SEP0611_GPIO_IN);
	
	sep0611_gpio_cfgpin(SEP0611_GPI15, SEP0611_GPIO_IO);	/* GPIO */
	sep0611_gpio_dirpin(SEP0611_GPI15, SEP0611_GPIO_IN);

	sep0611_gpio_cfgpin(SEP0611_GPI9, SEP0611_GPIO_IO);	/* GPIO */
	sep0611_gpio_dirpin(SEP0611_GPI9, SEP0611_GPIO_IN);
	
	
	return 0;
}
EXPORT_SYMBOL(gpio_init);

/*****************SEP0611 GPIO keyboard support**********************************************/
static struct gpio_keys_button spdw_buttons[] = {
	[0] = {
		.gpio		= SEP0611_KEY_MENU,
		.code		= 139,
		.desc		= "menu",
		.active_low	= 1,
		.wakeup		= 0,
	},
	
	[1] = {
		.gpio		= SEP0611_KEY_HOME,
		.code		= 102,
		.desc		= "home",
		.active_low	= 1,
		.wakeup		= 0,
	},
	
	[2] = {
		.gpio		= SEP0611_KEY_BACK,
		.code		= 158,
		.desc		= "back",
		.active_low	= 1,
		.wakeup		= 0,
	},

	[3] = {
		.gpio		= SEP0611_PWR_KEY,
		.code		= 116,
		.desc		= "power",
		.active_low	= 0,
		.wakeup		= 1,
	},
};

static struct gpio_keys_platform_data spdw_button_data = {
	.buttons	= spdw_buttons,
	.nbuttons	= ARRAY_SIZE(spdw_buttons),
};

static struct platform_device sep0611_device_button = {
	.name	= "gpio-keys",
	.id		= -1,
	.dev	= {
		.platform_data	= &spdw_button_data,
	},
};

static struct platform_device spdw_device_battery = {
	.name	= "spdw-battery",
	.id		= -1,
};

static struct platform_device sep0611_device_backlight = {
	.name 	= "sep0611-backlight",
	.id		= -1,
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

static const struct ad7879_platform_data sep0611_ad7879_ts_info = {
	.model			= 7879,	/* Model = AD7879 */
	.x_plate_ohms	= 620,	/* 620 Ohm from the touch datasheet */
	.pressure_max	= 3000,
	.pressure_min	= 80,
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
		I2C_BOARD_INFO("ad7879", 0x2d),
		.irq			= INTSRC_EXT11,
		.platform_data	= &sep0611_ad7879_ts_info,
	},
	
	[1] = {
		I2C_BOARD_INFO("WM8976", 0x1A),
	},

	[2] = {
		I2C_BOARD_INFO("qn8027", 0x2c),
	},

	[3] = {
		I2C_BOARD_INFO("mma7660",0x4c),
	},

	[4] = {
		I2C_BOARD_INFO("hdmi",0x4d),
	},
};
static struct platform_device *devices[] __initdata = 
{
	&sep0611_device_serial,
	&sep0611_device_lcd,
	&sep0611_device_backlight,
#ifdef CONFIG_SEP0611_OVERLAY
	&sep0611_device_overlay,
#endif
	&sep0611_device_nand,
	&sep0611_device_i2c1,
	&sep0611_device_mmc,
	&sep0611_device_sdio,
	&sep0611_device_usb,
	&sep0611_device_rtc,
	&sep0611_device_gps,
	&spdw_device_battery,
    &sep0611_device_button,
#ifdef CONFIG_ANDROID_PMEM
	&android_pmem_device,
	//&android_pmem_adsp_device,
	//&android_pmem_vpu_device,
#endif
#ifdef CONFIG_USB_ANDROID
	&usb_mass_storage_device,
	&android_usb_device,
#endif
};

void __init sep0611_init(void)
{	
	SEP0611_INT_ENABLE(INTSRC_UART0);
	SEP0611_INT_ENABLE(INTSRC_UART1);
	
	gpio_init();

	platform_add_devices(devices, ARRAY_SIZE(devices));
	
	/* board infomation for I2C '1' */
	i2c_register_board_info(1, sep0611_i2c1_board_info, ARRAY_SIZE(sep0611_i2c1_board_info));
	
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

