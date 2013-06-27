/* linux/arch/unicore/mach-sep0611/devices.c
 *
 * Copyright (c) 2009-2011 SEUIC
 *
 * Southeast University ASIC SoC support
 *
 * SEP0611 board devices definition
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
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/usb/musb.h>
#include <linux/serial_8250.h>
#include <linux/dm9000.h>
#include <linux/clk.h>
//comment by zyt #include <linux/android_pmem.h>
#include <linux/spi/spi.h>
#include <linux/i2c.h>
//comment by zyt #include <linux/usb/android_composite.h>

#include <mach/hardware.h>
#include <mach/irqs.h>
#include <mach/spi.h>

#include <board/board.h>

/**the last 40MB memory reserved for pmem**/
#define PMEM_BASE 				0X4D800000//0X4E000000//
#define PMEM_BASE_SIZE 				SZ_1M*20
#define PMEM_ADSP_BASE 				0X4C800000//200MB
#define PMEM_ADSP_BASE_SIZE 			SZ_1M*8
//#define PMEM_VPU_BASE				0x4D800000
//#define PMEM_VPU_BASE_SIZE 			SZ_1M*20

#ifdef SEP0611_ETH_INTSRC
/***************** DM9000 support ********************/
static struct resource sep0611_dm9000_resource[] = {
	[0] = {
		.start 	= 0xA1000000,  //for SEP0611 board
		.end   	= 0xA1000000 + 3,
		.flags  = IORESOURCE_MEM,
	},

	[1] = {
		.start = 0xA1000800,
		.end   = 0xA1000803,
		.flags = IORESOURCE_MEM,
	},

	[2] = {
		.start = SEP0611_ETH_INTSRC,
		.end   = SEP0611_ETH_INTSRC,
		.flags = IORESOURCE_IRQ | IRQF_TRIGGER_RISING,
	},
};

static struct dm9000_plat_data sep0611_dm9000_platdata = {
	.flags = DM9000_PLATF_16BITONLY,	//work with 16bit mode
};

struct platform_device sep0611_device_dm9000 = {
	.name 	= "dm9000",
	.id 	= -1,
	.num_resources 	= ARRAY_SIZE(sep0611_dm9000_resource),
	.resource 		= sep0611_dm9000_resource,
	.dev 	= {
		.platform_data = &sep0611_dm9000_platdata,
	},
};
EXPORT_SYMBOL(sep0611_device_dm9000);
#endif

/************************android pmem support*************************************/

/*************comment by zyt
#ifdef CONFIG_ANDROID_PMEM
static struct android_pmem_platform_data android_pmem_data={
	.name 	= "pmem",
	.start 	= 0,//PMEM_BASE,
	.size 	= 0,//PMEM_BASE_SIZE,
	.no_allocator 	= 1,
	.cached = 1,
};
comment by zyt*************/

/*static struct android_pmem_platform_data android_pmem_adsp_data={
	.name 	= "pmem_adsp",
	.start 	= 0,//PMEM_ADSP_BASE,
	.size 	= 0,//PMEM_ADSP_BASE_SIZE,
	.no_allocator	= 0,
	.cached = 0,
};*/

/*static struct android_pmem_platform_data android_pmem_vpu_data={
	.name 	= "pmem_vpu",
	.start 	= PMEM_VPU_BASE,
	.size 	= PMEM_VPU_BASE_SIZE,
	.no_allocator	= 0,
	.cached = 0,
};*/

/*************comment by zyt
struct platform_device android_pmem_device = {
	.name 	= "android_pmem",
	.id 	= 0,
	.dev 	= {
		.platform_data	= &android_pmem_data
	},
};
EXPORT_SYMBOL(android_pmem_device);
comment by zyt*************/

/*struct platform_device android_pmem_adsp_device = {
	.name	= "android_pmem",
	.id 	= 1,
	.dev 	= {
		.platform_data	= &android_pmem_adsp_data
	},
};
EXPORT_SYMBOL(android_pmem_adsp_device);*/

/*struct platform_device android_pmem_vpu_device = {
	.name	= "android_pmem",
	.id 	= 1,
	.dev 	= {
		.platform_data	= &android_pmem_vpu_data
	},
};
EXPORT_SYMBOL(android_pmem_vpu_device);*/
//comment by zyt #endif


/*****************serial 8250 device**********************************************/
#if 0
static struct plat_serial8250_port serial_platform_data[] = {
	[0] = {
		.membase	= (void*)UART0_BASE_V,
		.mapbase	= UART0_BASE,
		.irq		= INTSRC_UART0,
		.regshift	= 2,
		.iotype		= UPIO_MEM,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST|UPF_LOW_LATENCY,
	},

	[1] = {
		.membase	= (void*)UART1_BASE_V,
		.mapbase	= UART1_BASE,
		.irq		= INTSRC_UART1,
		.regshift	= 2,
		.iotype		= UPIO_MEM,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST|UPF_LOW_LATENCY,
	},

	[2] = {
		.membase    = (void*)UART2_BASE_V,
		.mapbase	= UART2_BASE,
		.irq		= INTSRC_UART2,
		.regshift	= 2,
		.iotype		= UPIO_MEM,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST|UPF_LOW_LATENCY,
	},

	[3] = {
		.membase    = (void*)UART3_BASE_V,
		.mapbase	= UART3_BASE,
		.irq		= INTSRC_UART3,
		.regshift	= 2,
		.iotype		= UPIO_MEM,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST|UPF_LOW_LATENCY,
	},
	
	{ },
};
#endif
static struct plat_serial8250_port serial_platform_data[] = {
	[0] = {
		.membase	= (void*)UART0_BASE_V,
		.mapbase	= UART0_BASE,
		.irq		= INTSRC_UART0,
		.regshift	= 2,
		.iotype		= UPIO_MEM,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
	},

	[1] = {
		.membase	= (void*)UART1_BASE_V,
		.mapbase	= UART1_BASE,
		.irq		= INTSRC_UART1,
		.regshift	= 2,
		.iotype		= UPIO_MEM,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
	},

	[2] = {
		.membase    = (void*)UART2_BASE_V,
		.mapbase	= UART2_BASE,
		.irq		= INTSRC_UART2,
		.regshift	= 2,
		.iotype		= UPIO_MEM,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
	},

	[3] = {
		.membase    = (void*)UART3_BASE_V,
		.mapbase	= UART3_BASE,
		.irq		= INTSRC_UART3,
		.regshift	= 2,
		.iotype		= UPIO_MEM,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
	},
	
	{ },
};

struct platform_device sep0611_device_serial = {
	.name	= "serial8250",
	.id		= PLAT8250_DEV_PLATFORM,
	.dev	= {
		.platform_data	= serial_platform_data,
	},
};
EXPORT_SYMBOL(sep0611_device_serial);


/************************ LCD Controller ****************************/

static struct resource sep0611_lcd_resource[] = {
	[0] = {
		.start = LCDC_BASE,
  		.end   = LCDC_BASE + SZ_4K - 1,
    	.flags = IORESOURCE_MEM,
	},
	
	[1] = {
    	.start = INTSRC_LCDC,
   		.end   = INTSRC_LCDC,
 		.flags = IORESOURCE_IRQ,
	},
};

static u64 sep0611_device_lcd_dmamask = 0xFFFFFFFFUL;
struct platform_device sep0611_device_lcd = {
	.name			= "sep0611-fb",
	.id				= -1,
	.num_resources	= ARRAY_SIZE(sep0611_lcd_resource),
	.resource		= sep0611_lcd_resource,
	.dev			= {
		.dma_mask			= &sep0611_device_lcd_dmamask,
   		.coherent_dma_mask	= 0xFFFFFFFFUL,
	},
};
EXPORT_SYMBOL(sep0611_device_lcd);

#ifdef CONFIG_SEP0611_OVERLAY
struct platform_device sep0611_device_overlay = {
	.name	= "sep0611_overlay",
	.id		= -1,
};
EXPORT_SYMBOL(sep0611_device_overlay);
#endif
/*************************sep0611 nandflash support*********************************/

static struct resource sep0611_nandflash_resource[] = {
	[0] = {
		.start 	= NAND_BASE,
    	.end 	= NAND_BASE + 0x100,
  		.flags 	= IORESOURCE_MEM,
	},
};

struct platform_device sep0611_device_nand = {
	.name 			= "sep0611_nandflash",
	.id 			= -1,
	.num_resources 	= ARRAY_SIZE(sep0611_nandflash_resource),
	.resource  		= sep0611_nandflash_resource,
};
EXPORT_SYMBOL(sep0611_device_nand);


/***********************SEP0611 SPI support*****************************/

static struct sep0611_spi_info sep0611_spi_platdata = {
	.pin_cs		= GPIO_PORTD_DATA_V,
	.num_cs		= 1,
	.bus_num 	= 2,
};

/*
 * SPI 1
 */
static struct resource sep0611_spi1_resource[] = {
	[0] = {
		.start = SSI1_BASE,
		.end   = SSI1_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	
	[1] = {
		.start = INTSRC_SSI1,
		.end   = INTSRC_SSI1,
		.flags = IORESOURCE_IRQ,
	}

};

static u64 sep0611_device_spi1_dmamask = 0xFFFFFFFFUL;

struct platform_device sep0611_device_spi1 = {
	.name	= "sep0611-spi",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(sep0611_spi1_resource),
	.resource		= sep0611_spi1_resource,
   	.dev	= {
		.dma_mask 			= &sep0611_device_spi1_dmamask,
    	.coherent_dma_mask 	= 0xFFFFFFFFUL,
		.platform_data 		= &sep0611_spi_platdata,
	}
};

EXPORT_SYMBOL(sep0611_device_spi1);

/*
 * SPI 2
 */
static struct resource sep0611_spi2_resource[] = {
	[0] = {
		.start = SSI2_BASE,
		.end   = SSI2_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	
	[1] = {
		.start = INTSRC_SSI2,
		.end   = INTSRC_SSI2,
		.flags = IORESOURCE_IRQ,
	}
};

static u64 sep0611_device_spi2_dmamask = 0xFFFFFFFFUL;

struct platform_device sep0611_device_spi2 = {
	.name	= "sep0611-spi",
	.id		= 2,
	.num_resources	= ARRAY_SIZE(sep0611_spi2_resource),
	.resource	  	= sep0611_spi2_resource,
  	.dev	= {
   		.dma_mask 			= &sep0611_device_spi2_dmamask,
      	.coherent_dma_mask 	= 0xFFFFFFFFUL,
		.platform_data 		= &sep0611_spi_platdata,
	},
};

EXPORT_SYMBOL(sep0611_device_spi2);

/*
 * SPI 3
 */
static struct resource sep0611_spi3_resource[] = {
	[0] = {
		.start = SSI3_BASE,
		.end   = SSI3_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	
	[1] = {
		.start = INTSRC_SSI3,
		.end   = INTSRC_SSI3,
		.flags = IORESOURCE_IRQ,
	},
};

static u64 sep0611_device_spi3_dmamask = 0xFFFFFFFFUL;

struct platform_device sep0611_device_spi3 = {
	.name	= "sep0611-spi",
	.id		= 3,
	.num_resources	= ARRAY_SIZE(sep0611_spi3_resource),
	.resource		= sep0611_spi3_resource,
	.dev	= {
    	.dma_mask 			= &sep0611_device_spi3_dmamask,
      	.coherent_dma_mask 	= 0xFFFFFFFFUL,
		.platform_data 		= &sep0611_spi_platdata,
 	},
};

EXPORT_SYMBOL(sep0611_device_spi3);


/***********************SEP0611 I2C support*****************************/

/*
 * I2C 1
 */
static struct resource sep0611_i2c1_resource[] = {
	[0] = {
		.start 	= I2C1_BASE,
	    .end   	= I2C1_BASE + SZ_4K - 1,
	    .flags 	= IORESOURCE_MEM,
	},
	
   	[1] = {
	    .start 	= INTSRC_I2C1,
	    .end	= INTSRC_I2C1,
	    .flags 	= IORESOURCE_IRQ,
	},
};

struct platform_device sep0611_device_i2c1 = {
    .name 	= "sep0611-i2c",
    .id 	= 1,
    .num_resources	= ARRAY_SIZE(sep0611_i2c1_resource),
    .resource	  	= sep0611_i2c1_resource,
};

EXPORT_SYMBOL(sep0611_device_i2c1);

/*
 * I2C 2
 */
static struct resource sep0611_i2c2_resource[] = {
	[0] = {
		.start 	= I2C2_BASE,
	    .end   	= I2C2_BASE + SZ_4K - 1,
	    .flags 	= IORESOURCE_MEM,
	},
	
   	[1] = {
	    .start 	= INTSRC_I2C2,
	    .end	= INTSRC_I2C2,
	    .flags 	= IORESOURCE_IRQ,
	},
};

struct platform_device sep0611_device_i2c2 = {
    .name 	= "sep0611-i2c",
    .id 	= 2,
    .num_resources	= ARRAY_SIZE(sep0611_i2c2_resource),
    .resource	  	= sep0611_i2c2_resource,
};

EXPORT_SYMBOL(sep0611_device_i2c2);

/*
 * I2C 3
 */
static struct resource sep0611_i2c3_resource[] = {
	[0] = {
		.start 	= I2C3_BASE,
	    .end   	= I2C3_BASE + SZ_4K - 1,
	    .flags 	= IORESOURCE_MEM,
	},
	
   	[1] = {
	    .start 	= INTSRC_I2C3,
	    .end	= INTSRC_I2C3,
	    .flags 	= IORESOURCE_IRQ,
	},
};

struct platform_device sep0611_device_i2c3 = {
    .name 	= "sep0611-i2c",
    .id 	= 3,
    .num_resources	= ARRAY_SIZE(sep0611_i2c3_resource),
    .resource	  	= sep0611_i2c3_resource,
};
EXPORT_SYMBOL(sep0611_device_i2c3);

/**************SEP0611 RTC support ***********************************/

static struct resource sep0611_rtc_resource[] = {
	[0] = {
	    .start = RTC_BASE,
	    .end   = RTC_BASE + SZ_4K - 1,
	    .flags = IORESOURCE_MEM,
	},
	
   	[1] = {
	    .start = INTSRC_RTC,
	    .end   = INTSRC_RTC,
	    .flags = IORESOURCE_IRQ,
	},
};

struct platform_device sep0611_device_rtc = {
    .name	= "sep0611-rtc",
    .id 	= -1,
    .num_resources	= ARRAY_SIZE(sep0611_rtc_resource),
    .resource		= sep0611_rtc_resource,
};
EXPORT_SYMBOL(sep0611_device_rtc);


/*----------------------------------------------------------------------
 * Device     : USB Android Gadget 
 * Description: support android usb_mass_storage and usb_adb 
 *----------------------------------------------------------------------*/
/******************* comment by zyt
static struct usb_mass_storage_platform_data mass_storage_pdata = {
#ifdef CONFIG_SCSI
	.nluns = 3,
#else
	.nluns = 2,
#endif
	.vendor = "SEUIC",
	.product = "Android",
	.release = 0x0100,                                  
};

struct platform_device usb_mass_storage_device = {
	.name = "usb_mass_storage",
	.id = -1,
	.dev = {
		.platform_data = &mass_storage_pdata,
		},
};
EXPORT_SYMBOL(usb_mass_storage_device);

#ifdef CONFIG_USB_ANDROID_RNDIS
static struct usb_ether_platform_data rndis_pdata = {
comment by zyt ***********************/
	/* ethaddr is filled by board_serialno_setup */
/***************comment by zyt
	.vendorID	= 0x0XXX,
	.vendorDescr	= "SEUIC",
};

struct platform_device rndis_device = {
	.name	= "rndis",
	.id	= -1,
	.dev	= {
		.platform_data = &rndis_pdata,
	},
};
EXPORT_SYMBOL(rndis_device);
#endif

static char *usb_functions_ums[] = {
	"usb_mass_storage",
};

static char *usb_functions_ums_adb[] = {
	"usb_mass_storage",
	"adb",
};

static char *usb_functions_rndis[] = {
	"rndis",
};

static char *usb_functions_rndis_adb[] = {
	"rndis",
	"adb",
};


static char *usb_functions_all[] = {
#ifdef CONFIG_USB_ANDROID_RNDIS
	"rndis",
#endif
	"usb_mass_storage",
	"adb",
#ifdef CONFIG_USB_ANDROID_ACM
	"acm",
#endif
};

static struct android_usb_product usb_products[] = {
	{
		.product_id	= 0xb058, 
		.num_functions	= ARRAY_SIZE(usb_functions_ums),
		.functions	= usb_functions_ums,
	},
	{
		.product_id	= 0xdeed,
		.num_functions	= ARRAY_SIZE(usb_functions_ums_adb),
		.functions	= usb_functions_ums_adb,
	},
	{
		.product_id	= 0x0002,
		.num_functions	= ARRAY_SIZE(usb_functions_rndis),
		.functions	= usb_functions_rndis,
	},
	{
		.product_id	= 0x0003,
		.num_functions	= ARRAY_SIZE(usb_functions_rndis_adb),
		.functions	= usb_functions_rndis_adb,
	},
};

static struct android_usb_platform_data android_usb_pdata = {
	.vendor_id      = 0x18D1,
	.product_id     = 0x0001,
	.version	= 0x0100,
	.product_name	= "Android SEP6200",
	.manufacturer_name = "SEUIC",
	.num_products = ARRAY_SIZE(usb_products),
	.products = usb_products,
	.num_functions = ARRAY_SIZE(usb_functions_all),
	.functions = usb_functions_all,
};

struct platform_device android_usb_device = {
	.name	= "android_usb",
	.id		= -1,
	.dev		= {
	.platform_data = &android_usb_pdata,
	},
};
EXPORT_SYMBOL(android_usb_device);

comment by zyt ******************/

/********************SEP0611 USB Controller***********************/

static struct resource sep0611_usb_resources[] = {
	[0] = {
		.start	= USB_BASE,
		.end	= USB_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	
	[1] = {
		.start	= INTSRC_USB,
		.flags	= IORESOURCE_IRQ,
	},
	
	[2] = {
		.start	= INTSRC_USBDMA,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct musb_hdrc_config sep0611_usb_config = {
	.dyn_fifo 	= 0,
	.num_eps	= 8,
	.multipoint = 1,
#ifdef CONFIG_USB_INVENTRA_DMA
	.dma 		= 1,
	.dma_channels	= 8,
#endif
};

static struct musb_hdrc_platform_data sep0611_usb_plat = {
#if defined(CONFIG_USB_MUSB_OTG)
	.mode	= MUSB_OTG,
#elif defined(CONFIG_USB_GADGET_MUSB_HDRC)
	.mode	= MUSB_PERIPHERAL,
#elif defined(CONFIG_USB_MUSB_HDRC_HCD)
	.mode	= MUSB_HOST,
#endif

	.clock	= 0,
	.config	= &sep0611_usb_config,
};

static u64 sep0611_usb_dmamask = 0xFFFFFFFFUL;

struct platform_device sep0611_device_usb = {
	.name	= "musb_hdrc",
	.id		= -1,
	.dev 	= {
 		.platform_data   	= &sep0611_usb_plat,
     	.dma_mask       	= &sep0611_usb_dmamask,
     	.coherent_dma_mask	= 0xFFFFFFFFUL,
	},
	.num_resources	= ARRAY_SIZE(sep0611_usb_resources),
	.resource		= sep0611_usb_resources,
};
EXPORT_SYMBOL(sep0611_device_usb);


/**************************mmc support************************************/

static struct resource sep0611_mmc_resources[] = {
	[0] = {
		.start	= SDIO1_BASE_V,
		.end	= SDIO1_BASE_V + 0xFFF,
		.flags	= IORESOURCE_MEM,
	},
	
	[1] = {
		.start	= INTSRC_SDIO1,
		.end	= INTSRC_SDIO1,
		.flags	= IORESOURCE_IRQ,
	},
};

static u64 sep0611_mmc_dmamask = 0xFFFFFFFFUL;

struct platform_device sep0611_device_mmc = {
	.name	= "sep0611_mmc",
	.id		= -1,
	.dev	= {
		.dma_mask 			= &sep0611_mmc_dmamask,
		.coherent_dma_mask	= 0xFFFFFFFFUL,
	},
	.num_resources	= ARRAY_SIZE(sep0611_mmc_resources),
	.resource		= sep0611_mmc_resources,
};
EXPORT_SYMBOL(sep0611_device_mmc);


/*****************SDIO2 WIFI support**********************************************/

static struct resource sep0611_sdio_resources[] = {
	[0] = {
		.start	= SDIO2_BASE_V,
		.end	= SDIO2_BASE_V + 0xFFF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= INTSRC_SDIO2,
		.end	= INTSRC_SDIO2,
		.flags	= IORESOURCE_IRQ,
	},
};

static u64 sep0611_sdio_dmamask = 0xFFFFFFFFUL;

struct platform_device sep0611_device_sdio = {
	.name	= "sep0611_sdio",
	.id		= -1,
	.dev	= {
		.dma_mask 			= &sep0611_sdio_dmamask,
		.coherent_dma_mask 	= 0xFFFFFFFFUL,
	},
	.num_resources	= ARRAY_SIZE(sep0611_sdio_resources),
	.resource		= sep0611_sdio_resources,
};
EXPORT_SYMBOL(sep0611_device_sdio);


/******************************gps support********************************/
static struct resource sep0611_gps_resources[] = {
	[0] = {
		.start	= GPS_BASE,
		.end	= GPS_BASE + 0x100,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= ESRAM_BASE,
		.end	= ESRAM_BASE + 96*1024,/* 96 KB */
		.flags	= IORESOURCE_MEM,
	},
};

static u64 sep0611_gps_dmamask = 0xFFFFFFFFUL;

struct platform_device sep0611_device_gps = {
	.name	= "sep-gps",
	.id		= -1,
	.dev	= {
		.dma_mask 			= &sep0611_gps_dmamask,
		.coherent_dma_mask 	= 0xFFFFFFFFUL,
	},
	.num_resources	= ARRAY_SIZE(sep0611_gps_resources),
	.resource		= sep0611_gps_resources,
};
EXPORT_SYMBOL(sep0611_device_gps);
