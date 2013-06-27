/*
 * linux/arch/unicore/lib/io-readsb.S
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

.Linsb_align:	rsub	ip, ip, #4
		cmpsub.a	ip, r2
		cmovsg	ip, r2
		cmpsub.a	ip, #2
		ldb	r3, [r0]
		stb.w	r3, [r1]+, #1
		bsl	201f
		ldb	r3, [r0]
		stb.w	r3, [r1]+, #1
		bel	201f
		ldb	r3, [r0]
		stb.w	r3, [r1]+, #1
201:
		sub.a	r2, r2, ip
		bne	.Linsb_aligned

ENTRY(__raw_readsb)
		cmpxor.a	r2, #0	@ do we have to check for the zero len?
		cmoveq	pc, lr
		and.a	ip, r1, #3
		bne	.Linsb_align

.Linsb_aligned:

		sub.a	r2, r2, #16
		bfs	.Linsb_no_16

.Linsb_16_lp:	ldb	r3, [r0]
		ldb	r4, [r0]
		ldb	r5, [r0]
		mov	r3, r3 put_byte_0
		ldb	r6, [r0]
		or	r3, r3, r4 put_byte_1
		ldb	r4, [r0]
		or	r3, r3, r5 put_byte_2
		ldb	r5, [r0]
		or	r3, r3, r6 put_byte_3
		ldb	r6, [r0]
		mov	r4, r4 put_byte_0
		ldb	ip, [r0]
		or	r4, r4, r5 put_byte_1
		ldb	r5, [r0]
		or	r4, r4, r6 put_byte_2
		ldb	r6, [r0]
		or	r4, r4, ip put_byte_3
		ldb	ip, [r0]
		mov	r5, r5 put_byte_0
		ldb	r15, [r0]
		or	r5, r5, r6 put_byte_1
		ldb	r6, [r0]
		or	r5, r5, ip put_byte_2
		ldb	ip, [r0]
		or	r5, r5, r15 put_byte_3
		ldb	r15, [r0]
		mov	r6, r6 put_byte_0
		or	r6, r6, ip put_byte_1
		ldb	ip, [r0]
		or	r6, r6, r15 put_byte_2
		or	r6, r6, ip put_byte_3
		stm.w	(r3 - r6), [r1]+

		sub.a	r2, r2, #16
		bns	.Linsb_16_lp

		cmpand.a	r2, #15
		cmoveq	pc, lr

.Linsb_no_16:	cmpand.a	r2, #8
		beq	.Linsb_no_8

		ldb	r3, [r0]
		ldb	r4, [r0]
		ldb	r5, [r0]
		mov	r3, r3 put_byte_0
		ldb	r6, [r0]
		or	r3, r3, r4 put_byte_1
		ldb	r4, [r0]
		or	r3, r3, r5 put_byte_2
		ldb	r5, [r0]
		or	r3, r3, r6 put_byte_3
		ldb	r6, [r0]
		mov	r4, r4 put_byte_0
		ldb	ip, [r0]
		or	r4, r4, r5 put_byte_1
		or	r4, r4, r6 put_byte_2
		or	r4, r4, ip put_byte_3
		stm.w	(r3, r4), [r1]+

.Linsb_no_8:	cmpand.a	r2, #4
		beq	.Linsb_no_4

		ldb	r3, [r0]
		ldb	r4, [r0]
		ldb	r5, [r0]
		ldb	r6, [r0]
		mov	r3, r3 put_byte_0
		or	r3, r3, r4 put_byte_1
		or	r3, r3, r5 put_byte_2
		or	r3, r3, r6 put_byte_3
		stw.w	r3, [r1]+, #4

.Linsb_no_4:	and.a	r2, r2, #3
		cmoveq	pc, lr

		cmpsub.a	r2, #2
		ldb	r3, [r0]
		stb.w	r3, [r1]+, #1
		bsl	201f
		ldb	r3, [r0]
		stb.w	r3, [r1]+, #1
		bel	201f
		ldb	r3, [r0]
		stb	r3, [r1]
201:
		mov	pc, lr
ENDPROC(__raw_readsb)
