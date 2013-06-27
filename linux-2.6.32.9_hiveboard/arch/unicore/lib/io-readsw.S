/*
 * linux/arch/unicore/lib/io-readsw.S
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

		.macro	pack, rd, hw1, hw2
		or	\rd, \hw1, \hw2 << #16
		.endm

.Linsw_align:	mov.a	ip, r1 << #31
		bne	.Linsw_noalign
		ldh	ip, [r0]
		sub	r2, r2, #1
		sth.w	ip, [r1]+, #2

ENTRY(__raw_readsw)
		cmpxor.a	r2, #0
		cmoveq	pc, lr
		cmpand.a	r1, #3
		bne	.Linsw_align

		sub.a	r2, r2, #8
		bfs	.Lno_insw_8

.Linsw_8_lp:	ldh	r3, [r0]
		ldh	r4, [r0]
		pack	r3, r3, r4

		ldh	r4, [r0]
		ldh	r5, [r0]
		pack	r4, r4, r5

		ldh	r5, [r0]
		ldh	ip, [r0]
		pack	r5, r5, ip

		ldh	ip, [r0]
		ldh	r15, [r0]
		pack	ip, ip, r15

		sub.a	r2, r2, #8
		stm.w	(r3 - r5), [r1]+
		stm.w	(ip), [r1]+
		bns	.Linsw_8_lp

.Lno_insw_8:	cmpand.a	r2, #4
		beq	.Lno_insw_4

		ldh	r3, [r0]
		ldh	r4, [r0]
		pack	r3, r3, r4

		ldh	r4, [r0]
		ldh	ip, [r0]
		pack	r4, r4, ip

		stm.w	(r3, r4), [r1]+

.Lno_insw_4:	mov.a	r2, r2 << #31
		bub	.Lno_insw_2

		ldh	r3, [r0]
		ldh	ip, [r0]
		pack	r3, r3, ip
		stw.w	r3, [r1]+, #4

.Lno_insw_2:	cmoveq	pc, lr
		ldh	r3, [r0]
		sth	r3, [r1]
		mov	pc, lr

.Linsw_noalign:	bea	201f
		ldb.w	ip, [r1+], #-1
		b	1f
201:
		ldh	ip, [r0]
		sub	r2, r2, #1
		stb.w	ip, [r1]+, #1
		mov	ip, ip >> #8

1:		sub.a	r2, r2, #2
		bfs	3f

2:		ldh	r3, [r0]
		ldh	r4, [r0]
		sub.a	r2, r2, #2
		or	ip, ip, r3 << #8
		or	ip, ip, r4 << #24
		stw.w	ip, [r1]+, #4
		mov	ip, r4 >> #8
		bns	2b

3:		cmpand.a	r2, #1
		stb.w	ip, [r1]+, #1
		cmoveq	pc, lr
		ldh	ip, [r0]
		stb.w	ip, [r1]+, #1
		mov	ip, ip >> #8
		stb	ip, [r1]
		mov	pc, lr
ENDPROC(__raw_readsw)
