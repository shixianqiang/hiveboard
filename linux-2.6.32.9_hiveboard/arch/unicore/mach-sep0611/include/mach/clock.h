/* linux/arch/unicore/mach-sep0611/include/mach/clock.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *  cgm <chenguangming@wxseuic.com>
 *
 * Southeast University ASIC SoC support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  05-31-2011	cgm initial version
 *
 *SEP0611_CLOCK
 *├── APLL
 *│   └── CPU
 *├── DPLL
 *│   └── DDR
 *└── MPLL
 *    └── BUS1
 *        ├── BUS2
 *        │   ├── DMAC1
 *        │   ├── ESRAM
 *        │   ├── LCDC
 *        │   ├── NAND
 *        │   ├── NOR
 *        │   ├── SDIO1
 *        │   ├── SDIO2
 *        │   └── VPU
 *        ├── BUS3
 *        │   ├── BUS5
 *        │   │   ├── I2C1
 *        │   │   ├── I2C2
 *        │   │   ├── I2C3
 *        │   │   ├── I2S
 *        │   │   ├── SPI1
 *        │   │   ├── SPI2
 *        │   │   ├── SPI3
 *        │   │   ├── UART1
 *        │   │   ├── UART2
 *        │   │   ├── UART3
 *        │   │   └── UART4
 *        │   ├── DMAC2
 *        │   ├── GPU
 *        │   └── USB
 *        ├── BUS4
 *        │   ├── GPIO
 *        │   ├── GPSCTRL
 *        │   ├── PWM
 *        │   ├── RTC
 *        │   ├── SYSCTRL
 *        │   ├── TIMER
 *        │   └── VIC
 *        ├── DS1_2
 *        ├── DS1_3
 *        └── GPS
 *
 */

#ifndef __ASM_ARCH_CLOCK_H
#define __ASM_ARCH_CLOCK_H

/* Flags */
#define CLK_FIXED_RATE		(1 << 0)
#define CLK_ALWAYS_ENABLED	(1 << 1)
#define CLK_AUTO_OFF		(1 << 2)
#define CLK_RATE_PROPAGATES	(1 << 3)

enum sep0611_clk_gate{
	DDRC = 0, BUS1, BUS2, BUS3,	DS1_2,
	DS1_3, USBC, DMAC1,	NAND, DMAC2,
	ESRAM, SDIO1, SDIO2, GPU, VPU,
	BUS4, BUS5, VIC_, SYSCTRL, PRTC,
	TIMER, GPSCTRL, GPIO, LCDC2HDMI, DDRPHY,
	UART1, UART2, UART3, UART4,	SPI1,
	SPI2, SPI3,
	
	I2C1 = 32, I2C2, I2C3, I2S, PWM, 
	H2X, LCDC, NOR, GPSHCLK, GPS,
	
	APLL, DPLL, MPLL,
	CPU, RESERVED,
};

typedef struct {
	unsigned long f;		//freq
	unsigned long r;		//config register
}pll_t;

struct clk {
	const char *name;
	struct device *dev;
	enum sep0611_clk_gate parent_id;
	enum sep0611_clk_gate id;
	unsigned int div;
	unsigned long rate;
	unsigned int flags;
	int usecount;
	int (*set_rate)(struct clk *, u32);
};
#if 0
#define CORE_FREQ 800000000	//core rate

#define BUS1_FREQ 320000000 //AHB64 clock rate

///////////////////BUS2/////////////////////////
#define BUS2_FREQ 320000000 //AHB32 (BUS2) rate
#define NAND_CLK BUS2_FREQ
#define LCDC_CLK BUS2_FREQ
#define SDIO_CLK BUS2_FREQ
#define VPU_CLK BUS2_FREQ

///////////////////BUS3/////////////////////////
#define BUS3_FREQ 320000000 //AHB32 (BUS3) rate
#define USB_CLK BUS3_FREQ
#define GPU_CLK BUS3_FREQ

///////////////////BUS4/////////////////////////
#endif
#define BUS4_FREQ 320000000 //APB32 (BUS4) rate
#define TIMER_CLK BUS4_FREQ
#if 0
#define PWM_CLK BUS4_FREQ
#define RTC_CLK BUS4_FREQ

///////////////////BUS5/////////////////////////
#define BUS5_FREQ 80000000  //APB32 (BUS5) rate
#define I2C_CLK BUS5_FREQ
#define I2S_MCLK BUS5_FREQ
#define SPI_CLK BUS5_FREQ
#define UART_CLK BUS5_FREQ
#endif
#endif /* __ASM_ARCH_CLOCK_H */

