/*
 * linux/arch/unicore/lib/div64.S
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

#define xl r0
#define xh r1
#define yl r2
#define yh r3

/*
 * __do_div64: perform a division with 64-bit dividend and 32-bit divisor.
 *
 * Note: Calling convention is totally non standard for optimal code.
 *       This is meant to be used by do_div() from include/asm/div64.h only.
 *
 * Input parameters:
 * 	xh-xl	= dividend (clobbered)
 * 	r4	= divisor (preserved)
 *
 * Output values:
 * 	yh-yl	= result
 * 	xh	= remainder
 *
 * Clobbered regs: xl, ip
 */

ENTRY(__do_div64)

	@ Test for easy paths first.
	sub.a	ip, r4, #1
	beb	9f			@ divisor is 0 or 1
	cmpand.a	ip, r4
	beq	8f			@ divisor is power of 2

	@ See if we need to handle upper 32-bit result.
	cmpsub.a	xh, r4
	mov	yh, #0
	bub	3f

	@ Align divisor with upper part of dividend.
	@ The aligned divisor is stored in yl preserving the original.
	@ The bit position is stored in ip.

	cntlz	yl, r4
	cntlz	ip, xh
	sub	yl, yl, ip
	mov	ip, #1
	mov	ip, ip << yl
	mov	yl, r4 << yl

	@ The division loop for needed upper bit positions.
 	@ Break out early if dividend reaches 0.
2:	cmpsub.a	xh, yl
	bub	201f
	or	yh, yh, ip
	sub.a	xh, xh, yl
201:
	cmovne.a	ip, ip >> #1
	mov	yl, yl >> #1
	bne	2b

	@ See if we need to handle lower 32-bit result.
3:	cmpsub.a	xh, #0
	mov	yl, #0
	bne	201f
	cmpsub.a	xl, r4
201:
	cmovub	xh, xl
	cmovub	pc, lr

	@ The division loop for lower bit positions.
	@ Here we shift remainer bits leftwards rather than moving the
	@ divisor for comparisons, considering the carry-out bit as well.
	mov	ip, #0x80000000
4:	mov.a	xl, xl << #1
	addc.a	xh, xh, xh
	beq	6f
	bea	5f
	cmpsub.a	xh, r4
5:	bub	201f
	or	yl, yl, ip
	sub	xh, xh, r4
201:
	mov.a	ip, ip >> #1
	bne	4b
	mov	pc, lr

	@ The top part of remainder became zero.  If carry is set
	@ (the 33th bit) this is a false positive so resume the loop.
	@ Otherwise, if lower part is also null then we are done.
6:	bea	5b
	cmpsub.a	xl, #0
	cmoveq	pc, lr

	@ We still have remainer bits in the low part.  Bring them up.

	cntlz	xh, xl			@ we know xh is zero here so...
	add	xh, xh, #1
	mov	xl, xl << xh
	mov	ip, ip >> xh

	@ Current remainder is now 1.  It is worthless to compare with
	@ divisor at this point since divisor can not be smaller than 3 here.
	@ If possible, branch for another shift in the division loop.
	@ If no bit position left then we are done.
	mov.a	ip, ip >> #1
	mov	xh, #1
	bne	4b
	mov	pc, lr

8:	@ Division by a power of 2: determine what that divisor order is
	@ then simply shift values around

	cntlz	ip, r4
	rsub	ip, ip, #31

	mov	yh, xh >> ip
	mov	yl, xl >> ip
	rsub	ip, ip, #32
	or	yl, yl, xh << ip
	mov	xh, xl << ip
	mov	xh, xh >> ip
	mov	pc, lr

	@ eq -> division by 1: obvious enough...
9:	cmoveq	yl, xl
	cmoveq	yh, xh
	cmoveq	xh, #0
	cmoveq	pc, lr

	@ Division by 0:
	stw.w	lr, [sp+], #-8
	b.l	__div0

	@ as wrong as it could be...
	mov	yl, #0
	mov	yh, #0
	mov	xh, #0
	ldw.w	pc, [sp]+, #8

ENDPROC(__do_div64)
