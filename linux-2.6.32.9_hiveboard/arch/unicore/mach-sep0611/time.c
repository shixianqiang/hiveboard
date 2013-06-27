/* linux/arch/unicore/mach-sep0611/time.c
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
#include <linux/timex.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/types.h>
#include <linux/irq.h>
#include <linux/clk.h> 

#include <asm/system.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/mach/time.h>

#include <mach/hardware.h>

static irqreturn_t sep0611_timer_interrupt(int irq, void *dev_id)
{
	volatile int clear_int;
	
	if(readl(TIMER_T2IMSR_V) & 0X01)
		clear_int = readl(TIMER_T2ISCR_V);
	
	timer_tick();

	return IRQ_HANDLED;
}

static struct irqaction sep0611_timer_irq = {
	.name	= "SEP0611 Timer Tick",
	.flags	= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.handler= sep0611_timer_interrupt,
};
	
void __init sep0611_timer_init(void)
{
	struct clk *clk;
	unsigned long latch;

	clk = clk_get(NULL, "timer");
	if(clk == NULL)
		latch = LATCH;
	else
		latch = (clk_get_rate(clk) + HZ / 2) / HZ;

	writel(latch, TIMER_T2LCR_V);
	writel(0x07, TIMER_T2CR_V);
	
	SEP0611_INT_ENABLE(INTSRC_TIMER1);

	setup_irq(INTSRC_TIMER1, &sep0611_timer_irq);
}

struct sys_timer sep0611_timer = {
	.init	= sep0611_timer_init,
};

