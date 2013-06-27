/*
 * linux/arch/unicore/include/asm/hw_irq.h
 *
 * Code specific to PKUnity SoC and UniCore ISA
 * Fragments that appear the same as the files in arm or x86
 *
 * Copyright (C) 2001-2008 GUAN Xue-tao
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __UNICORE_HW_IRQ_H__
#define __UNICORE_HW_IRQ_H__

static inline void ack_bad_irq(int irq)
{
	extern unsigned long irq_err_count;
	irq_err_count++;
}

/*
 * Obsolete inline function for calling irq descriptor handlers.
 */
static inline void desc_handle_irq(unsigned int irq, struct irq_desc *desc)
{
	desc->handle_irq(irq, desc);
}

void set_irq_flags(unsigned int irq, unsigned int flags);

#define IRQF_VALID	(1 << 0)
#define IRQF_PROBE	(1 << 1)
#define IRQF_NOAUTOEN	(1 << 2)

#endif
