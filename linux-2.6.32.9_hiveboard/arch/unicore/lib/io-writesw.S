/*
 * linux/arch/unicore/lib/io-writesw.S
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
#include <linux/linkage.h>
#include <asm/assembler.h>

		.macro	outword, rd
		sth	\rd, [r0]
		mov	\rd, \rd >> #16
		sth	\rd, [r0]
		.endm

.Loutsw_align:	mov.a	ip, r1 << #31
		bne	.Loutsw_noalign

		ldh.w	r3, [r1]+, #2
		sub	r2, r2, #1
		sth	r3, [r0]

ENTRY(__raw_writesw)
		cmpxor.a	r2, #0
		cmoveq	pc, lr
		and.a	r3, r1, #3
		bne	.Loutsw_align

		sub.a	r2, r2, #8
		bfs	.Lno_outsw_8

.Loutsw_8_lp:	ldm.w	(r3, r4, r5, r14), [r1]+
		sub.a	r2, r2, #8
		outword	r3
		outword	r4
		outword	r5
		outword	r14
		bns	.Loutsw_8_lp

.Lno_outsw_8:	cmpand.a	r2, #4
		beq	.Lno_outsw_4

		ldm.w	(r3, r14), [r1]+
		outword	r3
		outword	r14

.Lno_outsw_4:	mov.a	r2, r2 << #31
		bub	.Lno_outsw_2

		ldw.w	r3, [r1]+, #4
		outword	r3

.Lno_outsw_2:	cmoveq	pc, lr
		ldh	r3, [r1]
		sth	r3, [r0]

		mov	pc, lr

.Loutsw_noalign:
		ldw.w	r3, [r1-], r3
		bub	201f
		sub	r2, r2, #1
		b	2f
201:
		sub.a	r2, r2, #2
		bfs	3f

1:		mov	ip, r3 >> #8
		sth	ip, [r0]
2:		mov	ip, r3 >> #24
		ldw.w	r3, [r1+], #4
		sub.a	r2, r2, #2
		or	ip, ip, r3 << #8
		sth	ip, [r0]
		bns	1b

		cmpand.a	r2, #1
3:		cmoveq	pc, lr
		mov	ip, r3 >> #8
		sth	ip, [r0]
		mov	pc, lr
ENDPROC(__raw_writesw)
