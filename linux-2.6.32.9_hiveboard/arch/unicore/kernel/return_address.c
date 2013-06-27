/*
 * linux/arch/unicore/kernel/return_address.c
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
 * Based on: arch/arm/kernel/return_address.c
 *
 * Contributors & Additions/Fixes:
 *
 * TODO:
 */
#include <linux/module.h>

#if defined(CONFIG_FRAME_POINTER)
#include <linux/sched.h>

#include <asm/stacktrace.h>

struct return_address_data {
	unsigned int level;
	void *addr;
};

static int save_return_addr(struct stackframe *frame, void *d)
{
	struct return_address_data *data = d;

	if (!data->level) {
		data->addr = (void *)frame->lr;

		return 1;
	} else {
		--data->level;
		return 0;
	}
}

void *return_address(unsigned int level)
{
	struct return_address_data data;
	struct stackframe frame;
	register unsigned long current_sp asm ("sp");

	data.level = level + 1;

	frame.fp = (unsigned long)__builtin_frame_address(0);
	frame.sp = current_sp;
	frame.lr = (unsigned long)__builtin_return_address(0);
	frame.pc = (unsigned long)return_address;

	walk_stackframe(&frame, save_return_addr, &data);

	if (!data.level)
		return data.addr;
	else
		return NULL;
}

#else /* if defined(CONFIG_FRAME_POINTER) */

void *return_address(unsigned int level)
{
	return NULL;
}

#endif /* if defined(CONFIG_FRAME_POINTER) / else */

EXPORT_SYMBOL_GPL(return_address);
