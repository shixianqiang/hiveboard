/*
 *  linux/arch/unicore/mach-puv3/core.c
 *
 * Code specific to PKUnity SoC and UniCore ISA
 *
 *	Maintained by GUAN Xue-tao <gxt@mprc.pku.edu.cn>
 *	Copyright (C) 2001-2009 MPRC & PKUnity
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Contributors & Additions/Fixes:
 *	2009-04: First version by GUAN Xue-tao
 *	2009-07-21: add uart resources definitions by HU Dong-liang
 *
 * TODO:
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/sysdev.h>
#include <linux/amba/bus.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <asm/irq.h>
#include <linux/cnt32_to_63.h>
#include <linux/usb/musb.h>
#include <asm/mach/arch.h>
#include <mach/pm.h>

/*
 * This is the PKUnity sched_clock implementation.  This has
 * a resolution of 271ns, and a maximum value of 32025597s (370 days).
 *
 * The return value is guaranteed to be monotonic in that range as
 * long as there is always less than 582 seconds between successive
 * calls to this function.
 *
 *  ( * 1E9 / CLOCK_TICK_RATE ) -> about 2235/32
 */
unsigned long long sched_clock(void)
{
	unsigned long long v = cnt32_to_63(OST_OSCR);

	/* original conservative method, but overflow frequently
        v *= NSEC_PER_SEC >> 12;
        do_div(v, CLOCK_TICK_RATE >> 12);
        */
        v = ((v & 0x7fffffffffffffffULL) * 2235) >> 5;
        
	return v;
}

static struct resource puv3_usb_resources[] = {
	/* order is significant! */
	{
		.start		= PKUNITY_USB_BASE,
		.end		= PKUNITY_USB_BASE + 0x3ff,
		.flags		= IORESOURCE_MEM,
	}, {
		.start		= IRQ_USB,
		.flags		= IORESOURCE_IRQ,
	}, {
		.start		= IRQ_USB,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct musb_hdrc_config	puv3_usb_config[] = {
	{
		.num_eps = 16,
                .multipoint = 1,
#ifdef CONFIG_USB_INVENTRA_DMA
                .dma = 1,
                .dma_channels = 8,
#endif
	},
};

static struct musb_hdrc_platform_data puv3_usb_plat = {
	.mode		= MUSB_HOST,
	.min_power 	= 100,
	.clock		= 0,
	.config		= puv3_usb_config,
};

static u64 puv3_usb_dmamask = 0xffffffffUL;

static struct platform_device puv3_device_usb = {
	.name		= "musb_hdrc",
	.id		= -1,
	.dev = {
                .platform_data          = &puv3_usb_plat,
                .dma_mask               = &puv3_usb_dmamask,
                .coherent_dma_mask      = 0xffffffff,
	},
	.num_resources	= ARRAY_SIZE(puv3_usb_resources),
	.resource	= puv3_usb_resources,
};

static struct resource puv3_mmc_resources[] = {
	[0] = {
		.start	= PKUNITY_SDC_BASE,
		.end	= PKUNITY_SDC_BASE + 0xfff,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_SDC,
		.end	= IRQ_SDC,
		.flags	= IORESOURCE_IRQ,
	},
};

static u64 puv3_mmc_dmamask = 0xffffffffUL;

static struct platform_device puv3_device_mmc = {
	.name		= "PKUnity-v3-MMC",
	.id		= -1,
	.dev		= {
		.dma_mask = &puv3_mmc_dmamask,
		.coherent_dma_mask = 0xffffffff,
	},
	.num_resources	= ARRAY_SIZE(puv3_mmc_resources),
	.resource	= puv3_mmc_resources,
};

static struct platform_device puv3_device_ac97 = {
	.name           = "PKUnity-v3-AC97",
	.id             = -1,
};

static struct resource puv3_rtc_resources[] = {
	[0] = {
		.start = PKUNITY_RTC_BASE,
		.end   = PKUNITY_RTC_BASE + 0xff,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_RTCAlarm,
		.end   = IRQ_RTCAlarm,
		.flags = IORESOURCE_IRQ,
	},
	[2] = {
		.start = IRQ_RTC,
		.end   = IRQ_RTC,
		.flags = IORESOURCE_IRQ
	}
};

static struct platform_device puv3_device_rtc = {
	.name		  = "PKUnity-v3-RTC",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(puv3_rtc_resources),
	.resource	  = puv3_rtc_resources,
};

static struct resource puv3_resource_pwm[] = {
	[0] = {
		.start	= PKUNITY_OST_BASE + 0x80,
		.end	= PKUNITY_OST_BASE + 0xff,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device puv3_device_pwm = {
	.name		= "PKUnity-v3-PWM",
	.id		= 0,
	.resource	= puv3_resource_pwm,
	.num_resources	= ARRAY_SIZE(puv3_resource_pwm),
};

static struct resource puv3_uart0_resources[] = {
	[0] = {
		.start = PKUNITY_UART0_BASE,
		.end   = PKUNITY_UART0_BASE + 0xff,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_UART0,
		.end   = IRQ_UART0,
		.flags = IORESOURCE_IRQ
	}
};
static struct resource puv3_uart1_resources[] = {
	[0] = {
		.start = PKUNITY_UART1_BASE,
		.end   = PKUNITY_UART1_BASE + 0xff,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_UART1,
		.end   = IRQ_UART1,
		.flags = IORESOURCE_IRQ
	}
};

static struct platform_device puv3_device_uart0 = {
	.name		= "PKUnity-v3-uart",
	.id		= 0,
	.num_resources	  = ARRAY_SIZE(puv3_uart0_resources),
	.resource	  = puv3_uart0_resources,
};

static struct platform_device puv3_device_uart1 = {
	.name		= "PKUnity-v3-uart",
	.id		= 1,
	.num_resources	  = ARRAY_SIZE(puv3_uart1_resources),
	.resource	  = puv3_uart1_resources,
};

static struct platform_device *puv3_devices[] __initdata = {
	&puv3_device_usb,
	&puv3_device_ac97,
	&puv3_device_mmc,
	&puv3_device_rtc,
	&puv3_device_pwm,
	&puv3_device_uart0,
	&puv3_device_uart1,
};

#ifdef CONFIG_PUV3_PM

#define SAVE(x)		sleep_save[SLEEP_SAVE_##x] = x
#define RESTORE(x)	x = sleep_save[SLEEP_SAVE_##x]

/*
 * List of global PXA peripheral registers to preserve.
 * More ones like CP and general purpose register values are preserved
 * with the stack pointer in sleep.S.
 */
enum {
	SLEEP_SAVE_PM_PLLDDRCFG,
	SLEEP_SAVE_COUNT
};


static void puv3_cpu_pm_save(unsigned long *sleep_save)
{
//	SAVE(PM_PLLDDRCFG);
}

static void puv3_cpu_pm_restore(unsigned long *sleep_save)
{
//	RESTORE(PM_PLLDDRCFG);
}

static int puv3_cpu_pm_prepare(void)
{
	/* set resume return address */
	PM_DIVCFG = virt_to_phys(puv3_cpu_resume);
	return 0;
}

static void puv3_cpu_pm_enter(suspend_state_t state)
{
	/* Clear reset status */
	RESETC_RSSR = RESETC_RSSR_HWR | RESETC_RSSR_WDR | RESETC_RSSR_SMR | RESETC_RSSR_SWR;

	switch (state) {
//	case PM_SUSPEND_ON:
//		puv3_cpu_idle();
//		break;
	case PM_SUSPEND_MEM:
		puv3_cpu_pm_prepare();
		puv3_cpu_suspend(PM_PMCR_SFB);
		break;
	}
}

static int puv3_cpu_pm_valid(suspend_state_t state)
{
	//return state == PM_SUSPEND_MEM || state == PM_SUSPEND_ON;
	return state == PM_SUSPEND_MEM;
}

static void puv3_cpu_pm_finish(void)
{
	/* ensure not to come back here if it wasn't intended */
//	PSPR = 0;
}

static struct puv3_cpu_pm_fns puv3_cpu_pm_fnss = {
	.save_count	= SLEEP_SAVE_COUNT,
	.valid		= puv3_cpu_pm_valid,
	.save		= puv3_cpu_pm_save,
	.restore	= puv3_cpu_pm_restore,
	.enter		= puv3_cpu_pm_enter,
	.prepare	= puv3_cpu_pm_prepare,
	.finish		= puv3_cpu_pm_finish,
};

static void __init puv3_init_pm(void)
{
	puv3_cpu_pm_fns = &puv3_cpu_pm_fnss;
}
#else
static inline void puv3_init_pm(void) {}
#endif

void __init puv3_init(void)
{
	puv3_init_pm();
	platform_add_devices(puv3_devices, ARRAY_SIZE(puv3_devices));
}

extern struct sys_timer puv3_timer;
extern void puv3_init_irq(void);

MACHINE_START(ARCH_PUV3, "PKUnity v3")
	.init_irq	= puv3_init_irq,
	.timer		= &puv3_timer,
	.init_machine	= puv3_init,
        .phys_io	= ((PKUNITY_UART0_BASE) & 0xffc00000),
        .io_pg_offst	= (((PKUNITY_UART0_BASE) >> 20) & 0xffc),
	.video_start    = PKUNITY_UNIGFX_MMAP_BASE,
	.video_end      = PKUNITY_UNIGFX_MMAP_BASE + PKUNITY_UNIGFX_MMAP_SIZE,
	.boot_params    = 0x00404000,
MACHINE_END
