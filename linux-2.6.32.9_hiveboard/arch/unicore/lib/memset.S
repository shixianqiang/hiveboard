/*
 * linux/arch/unicore/lib/memset.S
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
 *  ASM optimised string functions
 */
#include <linux/linkage.h>
#include <asm/assembler.h>

	.text
	.align	5
	.word	0

1:	sub.a	r2, r2, #4		@ 1 do we have enough
	bsl	5f			@ 1 bytes to align with?
	cmpsub.a	r3, #2			@ 1
	beg	201f
	stb.w	r1, [r0]+, #1		@ 1
201:
	bsg	201f
	stb.w	r1, [r0]+, #1		@ 1
201:
	stb.w	r1, [r0]+, #1		@ 1
	add	r2, r2, r3		@ 1 (r2 = r2 - (4 - r3))
/*
 * The pointer is now aligned and the length is adjusted.  Try doing the
 * memset again.
 */

ENTRY(memset)
	and.a	r3, r0, #3		@ 1 unaligned?
	bne	1b			@ 1
/*
 * we know that the pointer in r0 is aligned to a word boundary.
 */
	or	r1, r1, r1 << #8
	or	r1, r1, r1 << #16
	mov	r3, r1
	cmpsub.a	r2, #16
	bsl	4f

/*
 * We need an extra register for this loop - save the return address and
 * use the LR
 */
	mov	r14, r1
	mov	r15, r1

2:	sub.a	r2, r2, #64
	bsl	201f
	stm.w	(r1, r3, r14, r15), [r0]+	@ 64 bytes at a time.
	stm.w	(r1, r3, r14, r15), [r0]+
	stm.w	(r1, r3, r14, r15), [r0]+
	stm.w	(r1, r3, r14, r15), [r0]+
	bsg	2b
	cmoveq	pc, lr			@ Now <64 bytes to go.
201:
/*
 * No need to correct the count; we're only testing bits from now on
 */
	cmpand.a	r2, #32
	beq	201f
	stm.w	(r1, r3, r14, r15), [r0]+
	stm.w	(r1, r3, r14, r15), [r0]+
201:
	cmpand.a	r2, #16
	beq	4f
	stm.w	(r1, r3, r14, r15), [r0]+

4:	cmpand.a	r2, #8
	beq	201f
	stm.w	(r1, r3), [r0]+
201:
	cmpand.a	r2, #4
	beq	201f
	stw.w	r1, [r0]+, #4
201:
/*
 * When we get here, we've got less than 4 bytes to zero.  We
 * may have an unaligned pointer as well.
 */
5:	cmpand.a	r2, #2
	beq	201f
	stb.w	r1, [r0]+, #1
	stb.w	r1, [r0]+, #1
201:
	cmpand.a	r2, #1
	cmoveq	pc, lr
	stb.w	r1, [r0]+, #1
	mov	pc, lr
ENDPROC(memset)
