/*
 * linux/arch/unicore/mach-v8/include/mach/system.h
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
#ifndef __MACH_V8_SYSTEM_H__
#define __MACH_V8_SYSTEM_H__

#include <mach/hardware.h>

static inline void arch_idle(void)
{
	cpu_do_idle();
}

static inline void arch_reset(char mode, const char *cmd)
{
	if (mode == 's') {
		/* Jump into ROM at address 0xffff0000 */
		cpu_reset(CONFIG_VECTORS_BASE);
	} else {
		PM_PLLSYSCFG = 0x00002001; /* cpu clk = 250M */
		PM_PLLDDRCFG = 0x00100800; /* ddr clk =  44M */
		PM_PLLVGACFG = 0x00002001; /* vga clk = 250M */

		/* Use on-chip reset capability */
		/* following instructions must be inserted into one icache line */
		__asm__ __volatile__(
			"	.align 5\n\t"
			"	stw	%1, [%0]\n\t"
			"201:	ldw	r0, [%0]\n\t"
			"	cmpsub.a	r0, #0\n\t"
			"	bne	201b\n\t"
			"	stw	%3, [%2]\n\t"
			"	nop; nop; nop\n\t" /* prefetch 3 instructions at most */
			:
			: "r" ((unsigned long)&PM_PMCR),
			  "r" (PM_PMCR_CFBSYS | PM_PMCR_CFBDDR | PM_PMCR_CFBVGA),
			  "r" ((unsigned long)&RESETC_SWRR),
			  "r" (RESETC_SWRR_SRB)
			: "r0", "memory");
	}
}

#endif
