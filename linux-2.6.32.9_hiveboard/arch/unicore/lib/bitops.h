/*
 * linux/arch/unicore/lib/bitops.h
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

	.macro	bitop, instr
	and	r2, r0, #7
	mov	r3, #1
	mov	r3, r3 << r2
	save_and_disable_irqs ip r2
        ldb	r2, [r1+], r0 >> #3
	\instr	r2, r2, r3
        stb	r2, [r1+], r0 >> #3
	restore_irqs ip r2
	mov	pc, lr
	.endm

/**
 * testop - implement a test_and_xxx_bit operation.
 * @instr: operational instruction
 * @cond: conditional field
 *
 * Note: we can trivially conditionalise the store instruction
 * to avoid dirtying the data cache.
 */
        .macro	testop, instr, cond
	add	r1, r1, r0 >> #3
	and	r3, r0, #7
	mov	r0, #1
	save_and_disable_irqs ip r2
	ldb	r2, [r1]
	cmpand.a	r2, r0 << r3
	.ifnc	\cond, al
        b\cond  201f
        b       202f
        .endif
201:	\instr	r2, r2, r0 << r3
	stb	r2, [r1]
202:
	cmoveq	r0, #0
	restore_irqs ip r2
	mov	pc, lr
	.endm
