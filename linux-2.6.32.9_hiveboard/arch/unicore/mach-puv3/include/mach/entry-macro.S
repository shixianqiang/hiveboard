/*
 * linux/arch/unicore/mach-v8/include/mach/entry-macro.S
 *
 * Code specific to PKUnity SoC and UniCore ISA
 * Fragments that appear the same as the files in arm or x86
 *
 * Copyright (C) 2001-2008 GUAN Xue-tao
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Low-level IRQ helper macros for PKUnity platforms
 */
#include <mach/hardware.h>

		.macro	disable_fiq
		.endm

		.macro  get_irqnr_preamble, base, tmp
		.endm

		.macro  arch_ret_to_user, tmp1, tmp2
		.endm

		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
		ldw	\base, =(io_p2v(PKUNITY_INTC_BASE))
		ldw	\irqstat, [\base+], #0xC	@ INTC_ICIP
		ldw	\tmp,	  [\base+], #0x4	@ INTC_ICMR
		and.a	\irqstat, \irqstat, \tmp
		beq	1001f
		cntlz	\irqnr, \irqstat
		rsub	\irqnr, \irqnr, #31
1001:		/* EQ will be set if no irqs pending */
		.endm

