/*
 * linux/arch/unicore/mach-v8/include/mach/uncompress.h
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

#include "hardware.h"

static void ocd_putc(int c)
{
	int status, i = 0x2000000;

	do {
		if (--i < 0)
			return;

		asm volatile ("movc %0, p1.c0, #0" : "=r" (status));
	} while (status & 2);

	asm("movc p1.c1, %0, #1" : : "r" (c));
}

#if defined(DEBUG) && defined(CONFIG_DEBUG_OCD)
#define putc(ch)	ocd_putc(ch)
#else
#define putc(ch)
#endif

static inline void flush(void)
{
}

/*
 * Nothing to do for these
 */
#define arch_decomp_setup()
#define arch_decomp_wdog()
