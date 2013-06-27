/*
 * linux/arch/unicore/lib/memmove.S
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

		.text

/*
 * Prototype: void *memmove(void *dest, const void *src, size_t n);
 *
 * Note:
 *
 * If the memory regions don't overlap, we simply branch to memcpy which is
 * normally a bit faster. Otherwise the copy is done going downwards.  This
 * is a transposition of the code from copy_template.S but with the copy
 * occurring in the opposite direction.
 */

ENTRY(memmove)

		sub.a	ip, r0, r1
		beb	memcpy
		cmpsub.a	r2, ip
		beb	memcpy

		stm.w	(r0), [sp-]
		add	r1, r1, r2
		add	r0, r0, r2
		sub.a	r2, r2, #4
		bsl	8f
		and.a	ip, r0, #3
		bne	9f
		and.a	ip, r1, #3
		bne	10f

1:		sub.a	r2, r2, #(28)
		bsl	5f

3:		ldm.w	(r3, r4, r5, r6, r7, r8, r14, r15), [r1-]
		sub.a	r2, r2, #32
		stm.w	(r3, r4, r5, r6, r7, r8, r14, r15), [r0-]
		beg	3b

5:		and.a	ip, r2, #28
		rsub	ip, ip, #32
		beq	7f
		add	pc, pc, ip		@ C is always clear here
6:		nop
		ldw.w	r3, [r1+], #-4
		ldw.w	r4, [r1+], #-4
		ldw.w	r5, [r1+], #-4
		ldw.w	r6, [r1+], #-4
		ldw.w	r7, [r1+], #-4
		ldw.w	r8, [r1+], #-4
		ldw.w	r15, [r1+], #-4

		add	pc, pc, ip
		nop
		stw.w	r3, [r0+], #-4
		stw.w	r4, [r0+], #-4
		stw.w	r5, [r0+], #-4
		stw.w	r6, [r0+], #-4
		stw.w	r7, [r0+], #-4
		stw.w	r8, [r0+], #-4
		stw.w	r15, [r0+], #-4
7:
8:		mov.a	r2, r2 << #31
		beq	201f
		ldb.w	r3, [r1+], #-1
		stb.w	r3, [r0+], #-1
201:
		bub	201f
		ldb.w	r4, [r1+], #-1
		ldb	ip, [r1+], #-1
		stb.w	r4, [r0+], #-1
		stb	ip, [r0+], #-1
201:
		ldm.w	(r0), [sp]+
		mov	pc, lr

9:		cmpsub.a	ip, #2
		bel	201f
		ldb.w	r3, [r1+], #-1
		stb.w	r3, [r0+], #-1
201:		
		bsl	201f
		ldb.w	r4, [r1+], #-1
		stb.w	r4, [r0+], #-1
201:		ldb.w	r15, [r1+], #-1
		sub.a	r2, r2, ip
		stb.w	r15, [r0+], #-1
		bsl	8b
		and.a	ip, r1, #3
		beq	1b

10:		andn	r1, r1, #3
		cmpsub.a	ip, #2
		ldw	r3, [r1+], #0
		beq	17f
		bsl	18f


		.macro	backward_copy_shift a b

		sub.a	r2, r2, #28
		bsl	14f

13:		ldm.w   (r7, r8, r9, r14), [r1-]
		mov     r15, r3 push #\a
		sub.a    r2, r2, #32
		ldm.w   (r3, r4, r5, r6), [r1-]
		or     r15, r15, r14 pull #\b
		mov     r14, r14 push #\a
		or     r14, r14, r9 pull #\b
		mov     r9, r9 push #\a
		or     r9, r9, r8 pull #\b
		mov     r8, r8 push #\a
		or     r8, r8, r7 pull #\b
		mov     r7, r7 push #\a
		or     r7, r7, r6 pull #\b
		mov     r6, r6 push #\a
		or     r6, r6, r5 pull #\b
		mov     r5, r5 push #\a
		or     r5, r5, r4 pull #\b
		mov     r4, r4 push #\a
		or     r4, r4, r3 pull #\b
		stm.w   (r4 - r9, r14, r15), [r0-]
		beg	13b

14:		and.a	ip, r2, #28
		beq	16f

15:		mov     r15, r3 push #\a
		ldw.w	r3, [r1+], #-4
		sub.a	ip, ip, #4
		or	r15, r15, r3 pull #\b
		stw.w	r15, [r0+], #-4
		bsg	15b

16:		add	r1, r1, #(\b / 8)
		b	8b

		.endm


		backward_copy_shift	a=8	b=24

17:		backward_copy_shift	a=16	b=16

18:		backward_copy_shift	a=24	b=8

ENDPROC(memmove)
