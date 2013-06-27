/* linux/arch/unicore/mach-sep0611/clock.c
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
 *  06-02-2011	cgm initial version
 *  07-30-2011	cgm update version	
 *
 */      

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/spinlock.h>
#include <linux/delay.h>

#include <asm/io.h>

#include <mach/clock.h>
#include <mach/hardware.h>

#define XIN 12000000UL
#define SEP0611_FREQ_MORE_DEVICE
#define _CLK_DEBUG	0

#if _CLK_DEBUG
	#define clk_dbg(fmt, arg...) printk(fmt, ##arg)
#else
	#define clk_dbg(fmt, arg...)
#endif

static void print_clk(void);

#define HASH_TAB_LEN 64		//the value is determined by experiment
static int hash_tab[HASH_TAB_LEN] = {0};

static LIST_HEAD(clock_list);
static DEFINE_SPINLOCK(clock_lock);
static DEFINE_MUTEX(clock_mutex);

static struct clk *sep0611_clk;

#define SEP0611_CLK(_id, _parent_id, _name, _flags, _div, _set_rate)	\
	{								\
		.id			= _id,			\
		.parent_id	= _parent_id,	\
		.name		= _name,		\
		.flags		= _flags,		\
		.div		= _div,			\
		.set_rate	= _set_rate,	\
	}

#define PLL_CFG(_f, _r)	{.f = _f, .r = _r}	//f(frequency), unit: MHz; r(config register value)
#define MHz 1000000UL

static pll_t apll_tab[] = {
	PLL_CFG(800*MHz, 0x00010810),	// 800MHz
//	PLL_CFG(750*MHz, 0x0000F810),	// 700MHz
//	PLL_CFG(700*MHz, 0x0000E410),	// 700MHz
	PLL_CFG(650*MHz, 0x0000D410),	// 600MHz
//	PLL_CFG(600*MHz, 0x0000C410),	// 600MHz
//	PLL_CFG(550*MHz, 0x0000B410),	// 500MHz
	PLL_CFG(500*MHz, 0x0000A410),	// 500MHz
//	PLL_CFG(450*MHz, 0x00009410),	// 450MHz
//	PLL_CFG(400*MHz, 0x00010802),	// 400MHz
//	PLL_CFG(350*MHz, 0x00007000),	// 350MHz
	PLL_CFG(300*MHz, 0x0000C402),	// 300MHz
//	PLL_CFG(275*MHz, 0x00005800),	// 275MHz
//	PLL_CFG(250*MHz, 0x0000A402),	// 250MHz
//	PLL_CFG(200*MHz, 0x00010804),	// 200MHz
	PLL_CFG(175*MHz, 0x00007002),	// 350MHz
//	PLL_CFG(150*MHz, 0x0000C406),	// 150MHz
//	PLL_CFG(100*MHz, 0x00010806),	// 100MHz
};

static pll_t mpll_tab[] = {
	PLL_CFG(640*MHz, 0x0000d010),	// 640MHz
	PLL_CFG(480*MHz, 0x00013C12),	// 480MHz
	PLL_CFG(300*MHz, 0x00006000),	// 300MHz
	PLL_CFG(180*MHz, 0x0000EC14),	// 180MHz
	PLL_CFG(100*MHz, 0x00010816),	// 100MHz
};

static pll_t mpll_2i2s_tab[] = {
	PLL_CFG(640*MHz, 0x00043381),	// 640MHz
	PLL_CFG(480*MHz, 0x000322a1),	// 480MHz
	PLL_CFG(300*MHz, 0x000231a1),	// 300MHz
	PLL_CFG(180*MHz, 0x0002d0e1),	// 180MHz
	PLL_CFG(100*MHz, 0x00015081),	// 100MHz
};

static pll_t dpll_tab[] = {
	PLL_CFG(500*MHz, 0x0000A400),	// 500MHz
	PLL_CFG(400*MHz, 0x00010812),	// 402MHz
	PLL_CFG(300*MHz, 0x0000C412),	// 300MHz
	PLL_CFG(200*MHz, 0x00010814),	// 200MHz
	PLL_CFG(100*MHz, 0x00010816),	// 100MHz
};

/*
 * sync the clock and it's parent 
 */
static void __inline clk_sync(enum sep0611_clk_gate id)
{
	struct clk *clk;

	clk = &sep0611_clk[id];

	if(unlikely(clk->div)){
		clk->rate = sep0611_clk[clk->parent_id].rate / (clk->div + 1);
	}
	else{
		clk->rate = sep0611_clk[clk->parent_id].rate;
	}
}

static void clk_sync_bus2(void)
{
	clk_sync(DMAC1);
	clk_sync(LCDC);
	clk_sync(VPU);
	clk_sync(ESRAM);
 
#ifdef SEP0611_FREQ_MORE_DEVICE
	clk_sync(NOR);
#endif 

	clk_sync(NAND);
	clk_sync(SDIO1);
 
#ifdef SEP0611_FREQ_MORE_DEVICE
	clk_sync(SDIO2);
#endif 

}

static void clk_sync_bus5(void)
{
	clk_sync(I2C1);
 
#ifdef SEP0611_FREQ_MORE_DEVICE
	clk_sync(I2C2);
	clk_sync(I2C3);
	clk_sync(I2S);	//set i2s rate without notifier to save broken sound
#endif 

	clk_sync(SPI1);
 
#ifdef SEP0611_FREQ_MORE_DEVICE
	clk_sync(SPI2);
	clk_sync(SPI3);
#endif 

	clk_sync(UART1);
 
#ifdef SEP0611_FREQ_MORE_DEVICE
	clk_sync(UART2);
	clk_sync(UART3);
	clk_sync(UART4);
#endif 

}

static void clk_sync_bus3(void)
{
	clk_sync(DMAC2);
	clk_sync(GPU);
	clk_sync(USBC);
	
	clk_sync(BUS5);
	clk_sync_bus5();
}

static void clk_sync_bus4(void)
{
	clk_sync(GPIO);
 
#ifdef SEP0611_FREQ_MORE_DEVICE
	clk_sync(GPSCTRL);
#endif 

	clk_sync(PWM);
	clk_sync(PRTC);
	clk_sync(GPIO);
	clk_sync(SYSCTRL);
	clk_sync(TIMER);
	clk_sync(VIC_);
}

static void clk_sync_bus1(void)
{
	clk_sync(BUS2);
	clk_sync(BUS3);
	clk_sync(BUS4);

	clk_sync_bus2();
	clk_sync_bus3();
	clk_sync_bus4();

#ifdef SEP0611_FREQ_MORE_DEVICE
	clk_sync(GPS);
#endif 

	clk_sync(DS1_2);
	clk_sync(DS1_3);
}

static int pll_find(pll_t pll_tab[], u32 num, u32 rate)
{
	u32 i;

	for(i=0; i<num; i++){
		if(pll_tab[i].f == rate)
			break;
	}
	
	if(i < num)
		return i;
	else
		return -1;
}

/* APLL */
static int apll_clk_set_rate(struct clk *clk, u32 rate)
{
	int pll_index;
	u32 r;

	pll_index = pll_find(apll_tab, ARRAY_SIZE(apll_tab), rate);
	
	if(pll_index< 0){
		printk("not found avilable apll rate.\n");
		return -1;
	}

	
	r = apll_tab[pll_index].r;

	writel(r, PMU_APLL_CFG_V);
	writel(readl(PMU_PLL_SET_V) | 0x01, PMU_PLL_SET_V);

	while(!(readl(PMU_PLLLD_V) & 0x01))
		;
	
	clk->rate = rate;
	clk_sync(CPU);

	return 0;
}

/* MPLL */
static int mpll_clk_set_rate(struct clk *clk, u32 rate)
{
	int pll_index;
	int i2s_index;
	unsigned int i2s_base;	
	u32 r;
	u32 r_i2s;

	pll_index = pll_find(mpll_tab, ARRAY_SIZE(mpll_tab), rate);
	i2s_index = pll_find(mpll_2i2s_tab, ARRAY_SIZE(mpll_2i2s_tab), rate);
	clk_dbg("mpll_tab[%d].r=%d\n", pll_index, mpll_tab[pll_index].r);
	if(pll_index< 0)
		return -1;
	
	if(i2s_index< 0)
		return -1;

	i2s_base = ioremap(0xB100A000, 0x200);

	r = mpll_tab[pll_index].r;
	r_i2s = mpll_2i2s_tab[i2s_index].r;
	clk_dbg("i2s_base_vitual_address = 0x%x\n", i2s_base);
	writel(r, PMU_MPLL_CFG_V);
	writel(readl(PMU_PLL_SET_V) | 0x02, PMU_PLL_SET_V);

	while(!(readl(PMU_PLLLD_V) & 0x02));
	
	writel(r_i2s, i2s_base + 0x118);	//set i2s rate without notifier to solve broken sound 
	clk->rate = rate;

	clk_sync(BUS1);
	clk_sync_bus1();
	
	return 0;
}

/* DPLL */
static int dpll_clk_set_rate(struct clk *clk, u32 rate)
{
	int pll_index;
	u32 r;

	pll_index = pll_find(dpll_tab, ARRAY_SIZE(dpll_tab), rate);
	
	if(pll_index< 0)
		return -1;
	
	r = dpll_tab[pll_index].r;
#if 0
	writel(r, PMU_DPLL_CFG_V);
	writel(readl(PMU_PLL_SET_V) | 0x04, PMU_PLL_SET_V);

	while(!(readl(PMU_PLLLD_V) & 0x04))
		;
#else
//	clk_dbg("DEBUG:I refuse DDR rate set cfg=0x%08lx now.\n", r);
#endif
	clk->rate = rate;
	clk_sync(DDRC);
	
	return 0;
}

/* BUS1 clock */
static int bus1_clk_set_rate(struct clk *clk, u32 rate)
{
	u32 reg;

	clk->div = sep0611_clk[MPLL].rate / rate - 1;
	if(clk->div >= 16) 
		return -1;
	
	reg = (readl(PMU_AHB_CLK_CFG_V) & ~0xf) | clk->div;
	writel(reg, PMU_AHB_CLK_CFG_V);

	clk->rate = rate;
	clk_sync_bus1();

	return 0;
}

/* BUS4 clock */
static int bus4_clk_set_rate(struct clk *clk, u32 rate)
{
	u32 reg;

	clk->div = sep0611_clk[BUS1].rate / rate - 1;		
	if(clk->div >= 0x4)
		return	-1;

	if(clk->div * sep0611_clk[BUS1].div >= 16){
		clk->div = 0x0;
	}
	
	reg = (readl(PMU_AHB_CLK_CFG_V) & ~(0x3<<4)) | clk->div;
	writel(reg, PMU_AHB_CLK_CFG_V);

	clk->rate = rate;
	clk_sync_bus4();

	return 0;
}

/* BUS5 clock */
static int bus5_clk_set_rate(struct clk *clk, u32 rate)
{
	u32 reg;

	clk->div = sep0611_clk[BUS3].rate / rate - 1;		
	if(clk->div >= 0x4)
		return	-1;

	if(clk->div * sep0611_clk[BUS3].div >= 16){
		clk->div = 0x0;
	}
	
	reg = (readl(PMU_AHB_CLK_CFG_V) & ~(0x3<<6)) | clk->div;
	writel(reg, PMU_AHB_CLK_CFG_V);

	clk->rate = rate;
	clk_sync_bus5();

	return 0;
}

#if 0
/* UART clock */
static int uart_clk_set_rate(struct clk *clk, unsigned long rate)
{
	clk->rate = rate;
	
	return 0;
}

/* I2C clock */
static int i2c_clk_set_rate(struct clk *clk, unsigned long rate)
{
	clk->rate = rate;
	
	return 0;
}

/* SPI clock */
static int spi_clk_set_rate(struct clk *clk, unsigned long rate)
{
	clk->rate = rate;
	
	return 0;
}

/* SDIO1 clock */
static int sdio1_clk_set_rate(struct clk *clk, unsigned long rate)
{
	clk->rate = rate;
	
	return 0;
}

/* SDIO2 clock */
static int sdio2_clk_set_rate(struct clk *clk, unsigned long rate)
{
	clk->rate = rate;
	
	return 0;
}

/* USB clock */
static int usb_clk_set_rate(struct clk *clk, unsigned long rate)
{
	clk->rate = rate;
	
	return 0;
}
#else

#define uart_clk_set_rate	NULL
#define i2c_clk_set_rate	NULL
#define spi_clk_set_rate	NULL
#define sdio1_clk_set_rate	NULL
#define sdio2_clk_set_rate	NULL
#define usb_clk_set_rate	NULL

#endif

static struct clk sep0611_clocks[] = {
	SEP0611_CLK(DDRC, 	DPLL,	"ddr", 		CLK_ALWAYS_ENABLED,	0,	NULL),
	SEP0611_CLK(BUS1, 	MPLL,	"bus-1",	CLK_RATE_PROPAGATES,0,	bus1_clk_set_rate),
	SEP0611_CLK(BUS2, 	BUS1,	"bus-2", 	CLK_RATE_PROPAGATES,0, 	NULL),
	SEP0611_CLK(BUS3, 	BUS1,	"bus-3", 	CLK_RATE_PROPAGATES,0,	NULL),
	SEP0611_CLK(DS1_2, 	BUS1,	"ds1-2",	CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(DS1_3, 	BUS1,	"ds1-3", 	CLK_AUTO_OFF,		0, 	NULL),
	SEP0611_CLK(USBC, 	BUS3,	"usb", 		CLK_AUTO_OFF,		0, 	usb_clk_set_rate),
	SEP0611_CLK(DMAC1, 	BUS2,	"dmac-1", 	CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(NAND, 	BUS2,	"nfc", 		CLK_AUTO_OFF,		0, 	NULL),
	SEP0611_CLK(DMAC2, 	BUS3,	"dmac-2", 	CLK_AUTO_OFF,		0, 	NULL),
	SEP0611_CLK(ESRAM, 	BUS2,	"esram",	CLK_AUTO_OFF,		0, 	NULL),
	SEP0611_CLK(SDIO1, 	BUS2,	"sdio-1",	CLK_AUTO_OFF,		0, 	sdio1_clk_set_rate),
	SEP0611_CLK(SDIO2, 	BUS2,	"sdio-2",	CLK_AUTO_OFF,		0, 	sdio2_clk_set_rate),
	SEP0611_CLK(GPU, 	BUS3,	"gpuX2",	CLK_AUTO_OFF,		0, 	NULL),
	SEP0611_CLK(VPU, 	BUS2,	"vpu",		CLK_AUTO_OFF,		0, 	NULL),
	SEP0611_CLK(BUS4, 	BUS1,	"bus-4",	CLK_RATE_PROPAGATES,0, 	bus4_clk_set_rate),
	SEP0611_CLK(BUS5, 	BUS3,	"bus-5",	CLK_RATE_PROPAGATES,3, 	bus5_clk_set_rate),
	SEP0611_CLK(VIC_, 	BUS4, 	"vic",		CLK_AUTO_OFF,		0, 	NULL),
	SEP0611_CLK(SYSCTRL,BUS4, 	"systrcl",	CLK_AUTO_OFF,		0, 	NULL),
	SEP0611_CLK(PRTC,	BUS4, 	"rtc",		CLK_AUTO_OFF,		0, 	NULL),
	SEP0611_CLK(TIMER,	BUS4, 	"timer",	CLK_AUTO_OFF,		0, 	NULL),
	SEP0611_CLK(GPSCTRL,BUS4, 	"gpsctrl",	CLK_AUTO_OFF,		0, 	NULL),
	SEP0611_CLK(GPIO,	BUS4, 	"gpio",		CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(LCDC2HDMI,	-1,	"hdmi",		CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(DDRPHY,	-1,		"ddrphy",	CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(UART1,	BUS5, 	"uart-1",	CLK_AUTO_OFF,		0,	uart_clk_set_rate),
	SEP0611_CLK(UART2,	BUS5, 	"uart-2",	CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(UART3,	BUS5, 	"uart-3",	CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(UART4,	BUS5, 	"uart-4",	CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(SPI1,	BUS5, 	"spi-1",	CLK_AUTO_OFF,		0,	spi_clk_set_rate),
	SEP0611_CLK(SPI2,	BUS5, 	"spi-2",	CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(SPI3,	BUS5, 	"spi-3",	CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(I2C1,	BUS5, 	"i2c-1",	CLK_AUTO_OFF,		0,	i2c_clk_set_rate),
	SEP0611_CLK(I2C2,	BUS5, 	"i2c-2",	CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(I2C3,	BUS5, 	"i2c-3",	CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(I2S,	BUS5, 	"i2s",		CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(PWM,	BUS4, 	"pwm",		CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(H2X,	-1,		"h2x",		CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(LCDC,	BUS2,	"lcdc",		CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(NOR, 	BUS2,	"nor", 		CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(GPSHCLK,-1,		"gps-hclk",	CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(GPS, 	BUS1,	"gps", 		CLK_AUTO_OFF,		0,	NULL),
	SEP0611_CLK(APLL,	-1,		"apll",		CLK_ALWAYS_ENABLED,	0,	apll_clk_set_rate),
	SEP0611_CLK(DPLL,	-1,		"dpll",		CLK_ALWAYS_ENABLED,	0,	dpll_clk_set_rate),
	SEP0611_CLK(MPLL,	-1,		"mpll",		CLK_ALWAYS_ENABLED,	0,	mpll_clk_set_rate),
	SEP0611_CLK(CPU, 	APLL,	"cpu", 		CLK_ALWAYS_ENABLED,	0,	NULL),
};

#if 0
static void print_clk(void)
{
	struct clk *clk;

	for(clk = sep0611_clocks; clk < sep0611_clocks + ARRAY_SIZE(sep0611_clocks); clk++){
		printk("%s\t:%dHz\n", clk->name, _clk_get_rate(clk));
	}
	printk("\n");
}
#else
static void print_clk(void)
{
}
#endif

static void __init init_hash_tab(void)
{
	int i;

	for(i=0; i<HASH_TAB_LEN; i++)
		hash_tab[i] = -1;
}

static int get_hash_val(const char *str)
{
	const char *p = str;
	int hash = 0;

	while(*p) 
		hash += *p++;

	return (hash * 7) % HASH_TAB_LEN;
}

int store_clk(struct clk *clk)
{
	int pos, hash;
	int k = 1, n = 1, s = 1;
	int count = 0;

	hash = get_hash_val(clk->name);
	clk_dbg("%s:pos=%d\n", clk->name, hash);
	pos = hash;

	while(1){
		count ++;
		if(hash_tab[pos] < 0)
			break;

		k++;
		n = k>>1;
		s = -s;
		pos = hash + s*n;

		if(pos < 0)
			pos += HASH_TAB_LEN;

		if(pos >= HASH_TAB_LEN)
			pos -= HASH_TAB_LEN;

		if(pos == hash)
			return -1;
	}
	clk_dbg("count = %d\n", count);
	hash_tab[pos] = clk->id;

	return pos;
}

struct clk *search_clk(const char *name)
{
	struct clk *clk;
	int hash, pos;
	int id;
	int k = 1, n = 1, s = 1;

	hash = get_hash_val(name);

	pos = hash;

	while(1){
		id = hash_tab[pos];

		if(id >= 0){
			clk = &sep0611_clk[id];

			if(!strcmp(name, clk->name)){
				return clk;
			}
		}

		k++;
		n = k>>1;
		s = -s;
		pos = hash + s*n;
		
		if(pos < 0)
			pos += HASH_TAB_LEN;

		if(pos >= HASH_TAB_LEN)
			pos -= HASH_TAB_LEN;

		if(pos == hash)
			break;
	}

	return NULL;
}

static int __init clk_hash_init(void)
{
	int i;

	init_hash_tab();

	for(i=0; i<ARRAY_SIZE(sep0611_clocks); i++)
		store_clk(&sep0611_clocks[i]);

	return 0;
}

static u32 _clk_get_rate(struct clk *this)
{
	u32 rate = 0;
	u32 div;
	struct clk *clk = this;
	
    if(!clk)
        goto out;
	
    if(clk->rate){
        rate = clk->rate;
        goto out;
    }
	
	div = clk->div;
	
	while(clk->parent_id != -1){
		clk = &sep0611_clk[clk->parent_id];
		if(clk->rate){
			rate = clk->rate / (div + 1);
			this->rate = rate;
			goto out;
		}
		div = (div + 1) * (clk->div + 1) - 1;
	}
    
    if(clk->parent_id < 0)
    	printk("Failed to find clk %s\n", clk->name);

out: 
    return rate;
}

static int _clk_set_rate(struct clk *this, unsigned long rate)
{
	int ret = 0;
	struct clk *clk = this;
	
    if(!clk)
        goto out;
	
    if(clk->set_rate){
        ret = clk->set_rate(clk, rate);
        goto out;
    }
	
	while(clk->parent_id != -1){
		clk = &sep0611_clk[clk->parent_id];
		if(clk->set_rate){
        	ret = clk->set_rate(clk, rate);
			goto out;
		}
	}
    
    if(clk->parent_id < 0){
    	printk("Failed to set clk %s\n", clk->name);
		return -1;
	}

out:
	return ret;
}

static int _clk_enable(enum sep0611_clk_gate id)
{
	int ret = 0;
	u32 gate, gate_bit;
	struct clk *clk;

	if(id >= RESERVED)
		return -1;

	clk = &sep0611_clk[id];
	clk->usecount++;

	if (clk->usecount == 1) {
		if (clk->parent_id >= 0)
			ret = _clk_enable(clk->parent_id);
		
		if(clk->id <= SPI3){
			gate = readl(PMU_CLK_GT_CFG1_V);
			gate_bit = 1 << clk->id;
			writel(gate | gate_bit, PMU_CLK_GT_CFG1_V);
		}
		else if(clk->id <= GPS){
			gate = readl(PMU_CLK_GT_CFG2_V);
			gate_bit = 1 << (clk->id - I2C1);
			writel(gate | gate_bit, PMU_CLK_GT_CFG2_V);
		}
		else
			return 0;	
	}
	
	return ret;
}

static void _clk_disable(enum sep0611_clk_gate id)
{
	u32 gate, gate_bit;	
	struct clk *clk;

	if(id >= RESERVED)
		return;

	clk = &sep0611_clk[id];
	
	if (clk->flags & CLK_ALWAYS_ENABLED)
		return;
	clk->usecount--;
	
	if (clk->usecount == 0) {
		if(clk->id <= SPI3){
			gate = readl(PMU_CLK_GT_CFG1_V);
			gate_bit = ~(1 << clk->id);
			writel(gate & gate_bit, PMU_CLK_GT_CFG2_V);
		}
		else if(clk->id < GPS){
			gate = readl(PMU_CLK_GT_CFG2_V);
			gate_bit = ~(1 << (clk->id - I2C1));
			writel(gate & gate_bit, PMU_CLK_GT_CFG2_V);
		}
		else
			return;

		if (clk->parent_id)
			_clk_disable(clk->parent_id);
	}
}

struct clk *clk_get(struct device *dev, const char *id)
{
	struct clk *clk;

	mutex_lock(&clock_mutex);	
	clk = search_clk(id);
	mutex_unlock(&clock_mutex);

	return clk;
}
EXPORT_SYMBOL(clk_get);

int clk_enable(struct clk *clk)
{
	unsigned long flags;
	int ret = 0;

	clk_dbg("clk-%s enable\n", clk->name);

	BUG_ON(clk == 0);
	spin_lock_irqsave(&clock_lock, flags);
	ret = _clk_enable(clk->id);
	spin_unlock_irqrestore(&clock_lock, flags);

	return ret;
}
EXPORT_SYMBOL(clk_enable);


void clk_disable(struct clk *clk)
{
	unsigned long flags;

	clk_dbg("clk-%s disable\n", clk->name);

	BUG_ON(clk->usecount == 0);

	spin_lock_irqsave(&clock_lock, flags);
	_clk_disable(clk->id);
	spin_unlock_irqrestore(&clock_lock, flags);
}
EXPORT_SYMBOL(clk_disable);


/*
 * Get the clock rate for a clock source
 */
unsigned long clk_get_rate(struct clk *clk)
{
	return _clk_get_rate(clk);
}
EXPORT_SYMBOL(clk_get_rate);


/*
 * Set the clock rate for a clock source
 */
int clk_set_rate(struct clk *clk, unsigned long rate)
{
	int ret;
	unsigned long flags;

	spin_lock_irqsave(&clock_lock, flags);
	ret = _clk_set_rate(clk, rate);
	spin_unlock_irqrestore(&clock_lock, flags);
	
	return ret;
}
EXPORT_SYMBOL(clk_set_rate);

static u32 pll_to_rate(u32 pll)
{
	u32 NR, NF, NO;

	if(!pll)
		return 0;

	NR = ((pll >> 5) & 0x1F) + 1;
	NF = ((pll >> 10) & 0x7F) + 1;
	NO = 1 << ((pll >> 1) & 0x03);
	
	return (XIN * NF / NR / NO);
}

static void init_sys_rate(void)
{
	u32 apll, mpll, dpll;

	apll = readl(PMU_APLL_CFG_V);
	sep0611_clk[APLL].rate = pll_to_rate(apll);
	
	mpll = readl(PMU_MPLL_CFG_V);
	sep0611_clk[MPLL].rate = pll_to_rate(mpll);

	dpll = readl(PMU_DPLL_CFG_V);
	sep0611_clk[DPLL].rate = pll_to_rate(dpll);
}

static void init_bus_div(void)
{
	enum sep0611_clk_gate pll_table[4] = {MPLL, APLL, DPLL, DPLL};
	struct clk *bus1, *bus2, *bus3, *bus4, *bus5, *cpu, *gpu, *pix, *ddr;
	u32 cfg_ahb, cfg_core, cfg_ddr, cfg_pix, cfg_gpu;
	u32 div;

	bus1 = &sep0611_clk[BUS1];
	bus2 = &sep0611_clk[BUS2];
	bus3 = &sep0611_clk[BUS3];
	bus4 = &sep0611_clk[BUS4];
	bus5 = &sep0611_clk[BUS5];

	cpu = &sep0611_clk[CPU];
	ddr = &sep0611_clk[DDRC];
	gpu = &sep0611_clk[GPU];
	pix = &sep0611_clk[LCDC2HDMI]; //??? fix later
	
	cfg_ahb = readl(PMU_AHB_CLK_CFG_V);
	bus1->parent_id = pll_table[(cfg_ahb >> 8) & 0x03];
	
	div = cfg_ahb & 0xF;
	bus1->div = div;
	bus2->div = 0;
	bus3->div = 0;
	
	div = (cfg_ahb >> 6) & 0x03;
	if(((div + 1) * (bus1->div + 1)) > 16)
		div = bus1->div;
	bus5->div = div;
	
	div = (cfg_ahb >> 4) & 0x03;
	if(((div + 1) * (bus1->div + 1)) > 16)
		div = bus1->div;
	bus4->div = div;
	
	cfg_core = readl(PMU_CORE_CLK_CFG_V);
	cpu->div = (cfg_core & 0xF);
	
	cfg_ddr = readl(PMU_DDR_CLK_CFG_V);
	ddr->div = (cfg_ddr & 0xF);
	
	cfg_pix = readl(PMU_PIX_CLK_CFG_V);	
	pix->parent_id = pll_table[(cfg_pix >> 4) & 0x03];
	pix->div = cfg_pix & 0xF;;
	
	cfg_gpu = readl(PMU_GPU_CLK2X_CFG_V);	
	gpu->parent_id = pll_table[(cfg_gpu >> 4) & 0x03];
	gpu->div = cfg_gpu & 0xF;

#if 0	
	clk_dbg("CPU div = %d\n", cpu->div);
	clk_dbg("DDR div = %d\n", ddr->div);
	clk_dbg("PIX div = %d\n", pix->div);
	clk_dbg("GPUx2 div = %d\n", gpu->div);
	
	clk_dbg("BUS1 div = %d\n", bus1->div);	
	clk_dbg("BUS4 div = %d\n", bus4->div);
	clk_dbg("BUS5 div = %d\n", bus5->div);
#endif
}

int sep0611_clock_init(void)
{
	struct clk *clk;

	spin_lock_init(&clock_lock);
	mutex_lock(&clock_mutex);

	sep0611_clk = sep0611_clocks;
	

	clk_hash_init();
	init_sys_rate();
	init_bus_div();

	mutex_unlock(&clock_mutex);

	printk("SEP0611 clock driver initialized.(version 1.0)\n");

	clk = clk_get(NULL, "cpu");
	printk("Unicore\t:%ldMHz\n", (_clk_get_rate(clk) + 500000) / 1000000UL);
	clk_enable(clk);

	clk = clk_get(NULL, "ddr");
	printk("DDR2\t:%ldMHz\n", (_clk_get_rate(clk) + 500000) / 1000000UL);
	clk_enable(clk);
	
	clk = clk_get(NULL, "bus-1");
	printk("AHB\t:%ldMHz\n", (_clk_get_rate(clk) + 500000) / 1000000UL);
	
	clk = clk_get(NULL, "bus-5");
	printk("APB\t:%ldMHz\n", (_clk_get_rate(clk) + 500000) / 1000000UL);

	return 0;
}
EXPORT_SYMBOL(sep0611_clock_init);

