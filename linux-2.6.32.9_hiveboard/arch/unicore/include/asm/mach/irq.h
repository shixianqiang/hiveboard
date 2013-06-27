/*
 * linux/arch/unicore/include/asm/mach/irq.h
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
#ifndef __UNICORE_MACH_IRQ_H__
#define __UNICORE_MACH_IRQ_H__

#include <linux/irq.h>

struct seq_file;

/*
 * This is internal.  Do not use it.
 */
extern void (*init_arch_irq)(void);
extern void init_FIQ(void);
extern int show_fiq_list(struct seq_file *, void *);

/*
 * This is for easy migration, but should be changed in the source
 */
#define do_bad_IRQ(irq,desc)				\
do {							\
	spin_lock(&desc->lock);				\
	handle_bad_irq(irq, desc);			\
	spin_unlock(&desc->lock);			\
} while(0)

#endif
