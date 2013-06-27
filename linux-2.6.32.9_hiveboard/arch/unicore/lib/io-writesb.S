/*
 * linux/arch/unicore/lib/io-writesb.S
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
		stb	\rd, [r0]
		mov	\rd, \rd >> #8
		stb	\rd, [r0]
		mov	\rd, \rd >> #8
		stb	\rd, [r0]
		mov	\rd, \rd >> #8
		stb	\rd, [r0]
		.endm

.Loutsb_align:	rsub	ip, ip, #4
		cmpsub.a	ip, r2
		cmovsg	ip, r2
		cmpsub.a	ip, #2
		ldb.w	r3, [r1]+, #1
		stb	r3, [r0]
		bsl	201f
		ldb.w	r3, [r1]+, #1
		stb	r3, [r0]
		bel	201f
		ldb.w	r3, [r1]+, #1
		stb	r3, [r0]
201:
		sub.a	r2, r2, ip
		bne	.Loutsb_aligned

ENTRY(__raw_writesb)
		cmpxor.a	r2, #0	@ do we have to check for the zero len?
		cmoveq	pc, lr
		and.a	ip, r1, #3
		bne	.Loutsb_align

.Loutsb_aligned:

		sub.a	r2, r2, #16
		bfs	.Loutsb_no_16

.Loutsb_16_lp:	ldm.w	(r3, r4, r5, r14), [r1]+
		outword	r3
		outword	r4
		outword	r5
		outword	r14
		sub.a	r2, r2, #16
		bns	.Loutsb_16_lp

		cmpand.a	r2, #15
		cmoveq	pc, lr

.Loutsb_no_16:	cmpand.a	r2, #8
		beq	.Loutsb_no_8

		ldm.w	(r3, r4), [r1]+
		outword	r3
		outword	r4

.Loutsb_no_8:	cmpand.a	r2, #4
		beq	.Loutsb_no_4

		ldw.w	r3, [r1]+, #4
		outword	r3

.Loutsb_no_4:	and.a	r2, r2, #3
		cmoveq	pc, lr

		cmpsub.a	r2, #2
		ldb.w	r3, [r1]+, #1
		stb	r3, [r0]
		bsl	201f
		ldb.w	r3, [r1]+, #1
		stb	r3, [r0]
		bel	201f
		ldb	r3, [r1]
		stb	r3, [r0]
201:
		mov	pc, lr
ENDPROC(__raw_writesb)
