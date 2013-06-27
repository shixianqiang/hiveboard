/*
 * linux/arch/unicore/lib/io-writesl.S
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

ENTRY(__raw_writesl)
		cmpxor.a	r2, #0	@ do we have to check for the zero len?
		cmoveq	pc, lr
		and.a	ip, r1, #3
		bne	3f

		sub.a	r2, r2, #4
		bfs	2f
1:		ldm.w	(r3, r4, r14, r15), [r1]+
		sub.a	r2, r2, #4
		stw	r3, [r0+], #0
		stw	r4, [r0+], #0
		stw	r14, [r0+], #0
		stw	r15, [r0+], #0
		bns	1b
2:		mov.a	r2, r2 << #31
		bub	201f
		ldm.w	(r3, r14), [r1]+
		stw	r3, [r0+], #0
		stw	r14, [r0+], #0
201:		cmoveq	pc, lr
		ldw	r3, [r1+], #0
		stw	r3, [r0+], #0
		mov	pc, lr

3:		andn	r1, r1, #3
		ldw.w	r3, [r1]+, #4
		cmpsub.a	ip, #2
		bsl	5f
		bsg	6f

4:		mov	ip, r3 pull #16
		ldw.w	r3, [r1]+, #4
		sub.a	r2, r2, #1
		or	ip, ip, r3 push #16
		stw	ip, [r0]
		bne	4b
		mov	pc, lr

5:		mov	ip, r3 pull #8
		ldw.w	r3, [r1]+, #4
		sub.a	r2, r2, #1
		or	ip, ip, r3 push #24
		stw	ip, [r0]
		bne	5b
		mov	pc, lr

6:		mov	ip, r3 pull #24
		ldw.w	r3, [r1]+, #4
		sub.a	r2, r2, #1
		or	ip, ip, r3 push #8
		stw	ip, [r0]
		bne	6b
		mov	pc, lr
ENDPROC(__raw_writesl)
