/* 
 * linux/arch/unicore/lib/memzero.S
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
	.align	5
	.word	0
/*
 * Align the pointer in r0.  r3 contains the number of bytes that we are
 * mis-aligned by, and r1 is the number of bytes.  If r1 < 4, then we
 * don't bother; we use byte stores instead.
 */
1:	sub.a	r1, r1, #4		@ 1 do we have enough
	bsl	5f			@ 1 bytes to align with?
	cmpsub.a	r3, #2		@ 1
	beg	201f
	stb.w	r2, [r0]+, #1		@ 1
201:
	bsg	201f
	stb.w	r2, [r0]+, #1		@ 1
201:
	stb.w	r2, [r0]+, #1		@ 1
	add	r1, r1, r3		@ 1 (r1 = r1 - (4 - r3))
/*
 * The pointer is now aligned and the length is adjusted.  Try doing the
 * memzero again.
 */

ENTRY(__memzero)
	mov	r2, #0			@ 1
	and.a	r3, r0, #3		@ 1 unaligned?
	bne	1b			@ 1
/*
 * r3 = 0, and we know that the pointer in r0 is aligned to a word boundary.
 */
	cmpsub.a	r1, #16		@ 1 we can skip this chunk if we
	bsl	4f			@ 1 have < 16 bytes

/*
 * We need an extra register for this loop - save the return address and
 * use the LR
 */
	mov	r14, r2			@ 1
	mov	r15, r2			@ 1

3:	sub.a	r1, r1, #64		@ 1 write 32 bytes out per loop
	bsl	201f
	stm.w	(r2, r3, r14, r15), [r0]+	@ 4
	stm.w	(r2, r3, r14, r15), [r0]+	@ 4
	stm.w	(r2, r3, r14, r15), [r0]+	@ 4
	stm.w	(r2, r3, r14, r15), [r0]+	@ 4
	bsg	3b			@ 1
	cmoveq	pc, lr			@ 1/2 quick exit
201:
/*
 * No need to correct the count; we're only testing bits from now on
 */
	cmpand.a	r1, #32			@ 1
	beq	201f
	stm.w	(r2, r3, r14, r15), [r0]+	@ 4
	stm.w	(r2, r3, r14, r15), [r0]+	@ 4
201:
	cmpand.a	r1, #16			@ 1 16 bytes or more?
	beq	4f
	stm.w	(r2, r3, r14, r15), [r0]+	@ 4

4:	cmpand.a	r1, #8			@ 1 8 bytes or more?
	beq	201f
	stm.w	(r2, r3), [r0]+		@ 2
201:
	cmpand.a	r1, #4		@ 1 4 bytes or more?
	beq	5f
	stw.w	r2, [r0]+, #4		@ 1
/*
 * When we get here, we've got less than 4 bytes to zero.  We
 * may have an unaligned pointer as well.
 */
5:	cmpand.a	r1, #2		@ 1 2 bytes or more?
	beq	201f
	stb.w	r2, [r0]+, #1		@ 1
	stb.w	r2, [r0]+, #1		@ 1
201:	
	cmpand.a	r1, #1		@ 1 a byte left over
	cmoveq	pc, lr
	stb.w	r2, [r0]+, #1		@ 1
	mov	pc, lr			@ 1
ENDPROC(__memzero)
