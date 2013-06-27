/* linux/arch/unicore/mach-sep0611/gpio.c
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
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/io.h>

#include <asm/irq.h>

#include <mach/hardware.h>
#include <mach/gpio.h>
#include <mach/regs-gpio.h>

void sep0611_gpio_cfgpin(unsigned int pin, unsigned int function)
{
	unsigned long gpio_base;
	unsigned long offset;
	unsigned long sel;
	unsigned long sfn;
	unsigned long flags;
	
	gpio_base = SEP0611_GPIO_BASE(pin);
	offset = SEP0611_GPIO_OFFSET(pin);
	
	offset = 1<<offset;

	local_irq_save(flags);
	
	sel = readl(gpio_base + GP_SEL);
	sfn = readl(gpio_base + GP_SECFUN);
	
	switch (function) {
	case SEP0611_GPIO_IO:
		sel |= offset;
		break;
		
	case SEP0611_GPIO_SFN:
		sel &= ~offset;
		sfn &= ~offset;
		break;
	
	case SEP0611_GPIO_SFN2:
		sel &= ~offset;
		sfn |= offset;
		break;
		
	default:
		break;
	}
	
	writel(sel, gpio_base + GP_SEL);
	writel(sfn, gpio_base + GP_SECFUN);

	local_irq_restore(flags);
}
EXPORT_SYMBOL(sep0611_gpio_cfgpin);

void sep0611_gpio_setpin(unsigned int pin, unsigned int val)
{
	unsigned long gpio_base;
	unsigned long offset;
	unsigned long dat;
	unsigned long flags;
	
	gpio_base = SEP0611_GPIO_BASE(pin);
	offset = SEP0611_GPIO_OFFSET(pin);
	
	offset = 1<<offset;

	local_irq_save(flags);
	
	dat = readl(gpio_base + GP_DAT);
	if(val)
		dat |= offset;
	else
		dat &= ~offset;
	
	writel(dat, gpio_base + GP_DAT);

	local_irq_restore(flags);
}
EXPORT_SYMBOL(sep0611_gpio_setpin);

int sep0611_gpio_getpin(unsigned int pin)
{
	unsigned long gpio_base;
	unsigned long offset;
	unsigned long dat;
	
	gpio_base = SEP0611_GPIO_BASE(pin);
	offset = SEP0611_GPIO_OFFSET(pin);
	
	dat = readl(gpio_base + GP_DAT);
	
	return ((dat >> offset) & 0x01);
}
EXPORT_SYMBOL(sep0611_gpio_getpin);

void sep0611_gpio_dirpin(unsigned int pin, unsigned int in)
{
	unsigned long gpio_base;
	unsigned long offset;
	unsigned long dir;
	unsigned long flags;
	
	gpio_base = SEP0611_GPIO_BASE(pin);
	offset = SEP0611_GPIO_OFFSET(pin);
	
	offset = 1<<offset;

	local_irq_save(flags);
	
	dir = readl(gpio_base + GP_DIR);
	if(in)
		dir |= offset;
	else
		dir &= ~offset;
	
	writel(dir, gpio_base + GP_DIR);

	local_irq_restore(flags);
}
EXPORT_SYMBOL(sep0611_gpio_dirpin);

void sep0611_gpio_setirq(unsigned int pin, enum gpio_irq_trig trig)
{
	unsigned long gpio_base;
	unsigned long offset;
	unsigned long sel, dir, intr_sel, intr_lel, intr_pol;
	unsigned long flags;
	
	gpio_base = SEP0611_GPIO_BASE(pin);
	offset = SEP0611_GPIO_OFFSET(pin);
	
	offset = 1<<offset;

	local_irq_save(flags);
	
	sel = readl(gpio_base + GP_SEL);
	dir = readl(gpio_base + GP_DIR);
	intr_sel = readl(gpio_base + GP_INTSEL);
	intr_lel = readl(gpio_base + GP_INTLEL);
	intr_pol = readl(gpio_base + GP_INTPOL);
	
	sel |= offset;
	dir |= offset;
	intr_sel |= offset;
	
	if(trig & 0x01)
		intr_pol |= offset;
	else
		intr_pol &= ~offset;
		
	if(trig & 0x02)
		intr_lel |= offset;
	else
		intr_lel &= ~offset;

#if 0
	printk("gpio_base=0x%08lx, offset=%d\n", gpio_base, offset);
	printk("sel=0x%08lx\n", sel);
	printk("dir=0x%08lx\n", dir);
	printk("intr_sel=0x%08lx\n", intr_sel);
	printk("intr_lel=0x%08lx\n", intr_lel);
	printk("intr_pol=0x%08lx\n", intr_pol);
#endif	
	writel(sel, gpio_base + GP_SEL);
	writel(dir, gpio_base + GP_DIR);
	writel(intr_sel, gpio_base + GP_INTSEL);
	writel(intr_lel, gpio_base + GP_INTLEL);
	writel(intr_pol, gpio_base + GP_INTPOL);
	writel(readl(gpio_base + GP_INTCLR) | offset, gpio_base + GP_INTCLR);
	writel(readl(gpio_base + GP_INTCLR) & ~offset, gpio_base + GP_INTCLR);

	local_irq_restore(flags);
}
EXPORT_SYMBOL(sep0611_gpio_setirq);

int sep0611_gpio_getirq(unsigned int pin)
{
	unsigned long offset;
	
	if(pin < PORT_I || pin > (PORT_AO + 8))
		return -EINVAL;
	
	offset = SEP0611_GPIO_OFFSET(pin);
	
	if(unlikely(offset > INTSRC_EXT15))
		return -EINVAL;

	return offset;
}
EXPORT_SYMBOL(sep0611_gpio_getirq);

int sep0611_gpio_clrirq(unsigned int pin)
{
	unsigned long gpio_base;
	unsigned long offset;
	unsigned long intr_clr;
	unsigned long flags;
	
	if(pin < PORT_I || pin > (PORT_AO + 8))
		return -EINVAL;
		
	local_irq_save(flags);
	
	gpio_base = SEP0611_GPIO_BASE(pin);
	offset = SEP0611_GPIO_OFFSET(pin);
	
	offset = 1<< offset;
	
	intr_clr = readl(gpio_base + GP_INTCLR);
	intr_clr |= offset;
	writel(intr_clr, gpio_base + GP_INTCLR);
	intr_clr &= ~offset;
	writel(intr_clr, gpio_base + GP_INTCLR);
	
	local_irq_restore(flags);
	
	if(unlikely(offset > INTSRC_EXT15))
		return -EINVAL;

	return offset;
}
EXPORT_SYMBOL(sep0611_gpio_clrirq);


int sep0611_gpio_cfggrp(unsigned int start_pin, unsigned int num, unsigned int function)
{
	unsigned long gpio_base;
	unsigned long offset, mask;
	unsigned long sel;
	unsigned long sfn;
	unsigned long flags;
	
	if(num == 0)
		return -EINVAL;
	
	gpio_base = SEP0611_GPIO_BASE(start_pin);
	offset = SEP0611_GPIO_OFFSET(start_pin);
	
	if(offset + num > 32)
		num = 32 - offset;	/* if you cross the border, limit it in 32bits(one PORT's max width) */
	
	mask = (1<<num) - 1;
	mask = mask << offset;

	local_irq_save(flags);
	
	sel = readl(gpio_base + GP_SEL);
	sfn = readl(gpio_base + GP_SECFUN);
	
	switch (function) {
	case SEP0611_GPIO_IO:
		sel |= mask;
		break;
		
	case SEP0611_GPIO_SFN:
		sel &= ~mask;
		sfn &= ~mask;
		break;
	
	case SEP0611_GPIO_SFN2:
		sel &= ~mask;
		sfn |= mask;
		break;
		
	default:
		break;
	}
	
	writel(sel, gpio_base + GP_SEL);
	writel(sfn, gpio_base + GP_SECFUN);

	local_irq_restore(flags);
	
	return 0;
}
EXPORT_SYMBOL(sep0611_gpio_cfggrp);

int sep0611_gpio_setgrp(unsigned int start_pin, unsigned int num, unsigned int val)
{
	unsigned long gpio_base;
	unsigned long offset, mask;
	unsigned long dat;
	unsigned long flags;
	
	if(num == 0)
		return -EINVAL;
	
	gpio_base = SEP0611_GPIO_BASE(start_pin);
	offset = SEP0611_GPIO_OFFSET(start_pin);
	
	if(offset + num > 32)
		num = 32 - offset;	/* if you cross the border, limit it in 32bits(one PORT's max width) */
	
	mask = (1<<num) - 1;
	mask = mask << offset;

	local_irq_save(flags);
	
	dat = readl(gpio_base + GP_DAT);
	if(val)
		dat |= mask;
	else
		dat &= ~mask;
	
	writel(dat, gpio_base + GP_DAT);

	local_irq_restore(flags);
	
	return 0;
}
EXPORT_SYMBOL(sep0611_gpio_setgrp);

int sep0611_gpio_getgrp(unsigned int start_pin, unsigned int num)
{
	unsigned long gpio_base;
	unsigned long offset, mask;
	unsigned long dat;
	
	if(num == 0)
		return -EINVAL;
	
	gpio_base = SEP0611_GPIO_BASE(start_pin);
	offset = SEP0611_GPIO_OFFSET(start_pin);
	
	if(offset + num > 32)
		num = 32 - offset;	/* if you cross the border, limit it in 32bits(one PORT's max width) */
	
	mask = (1<<num) - 1;
		
	dat = readl(gpio_base + GP_DAT);
	
	return ((dat >> offset) & mask);
}
EXPORT_SYMBOL(sep0611_gpio_getgrp);

int sep0611_gpio_dirgrp(unsigned int start_pin, unsigned int num, unsigned int in)
{
	unsigned long gpio_base;
	unsigned long offset, mask;
	unsigned long dir;
	unsigned long flags;
	
	if(num == 0)
		return -EINVAL;
	
	gpio_base = SEP0611_GPIO_BASE(start_pin);
	offset = SEP0611_GPIO_OFFSET(start_pin);
	
	if(offset + num > 32)
		num = 32 - offset;	/* if you cross the border, limit it in 32bits(one PORT's max width) */
	
	mask = (1<<num) - 1;
	mask = mask << offset;

	local_irq_save(flags);
	
	dir = readl(gpio_base + GP_DIR);
	if(in)
		dir |= mask;
	else
		dir &= ~mask;
	
	writel(dir, gpio_base + GP_DIR);

	local_irq_restore(flags);
	
	return 0;
}
EXPORT_SYMBOL(sep0611_gpio_dirgrp);
