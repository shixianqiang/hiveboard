/*
 * linux/arch/unicore/lib/sha1.S
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
 *  The reference implementation for this code is linux/lib/sha1.c
 */

#include <linux/linkage.h>

	.text


/*
 * void sha_transform(__u32 *digest, const char *in, __u32 *W)
 *
 * Note: the "in" ptr may be unaligned.
 */

ENTRY(sha_transform)

	stm.w	(lr), [sp-]

	@ for (i = 0; i < 16; i++)
	@         W[i] = be32_to_cpu(in[i]);

	mov	r3, r2
	mov	lr, #16
1:	ldb.w	r4, [r1]+, #1
	ldb.w	r5, [r1]+, #1
	ldb.w	r6, [r1]+, #1
	ldb.w	r7, [r1]+, #1
	sub.a	lr, lr, #1
	or	r5, r5, r4 << #8
	or	r6, r6, r5 << #8
	or	r7, r7, r6 << #8
	stw.w	r7, [r3]+, #4
	bne	1b

	@ for (i = 0; i < 64; i++)
	@         W[i+16] = ror(W[i+13] ^ W[i+8] ^ W[i+2] ^ W[i], 31);

	sub	r3, r2, #4
	mov	lr, #64
2:	ldw.w	r4, [r3+], #4
	sub.a	lr, lr, #1
	ldw	r5, [r3+], #8
	ldw	r6, [r3+], #32
	ldw	r7, [r3+], #52
	xor	r4, r4, r5
	xor	r4, r4, r6
	xor	r4, r4, r7
	mov	r4, r4 <> #31
	stw	r4, [r3+], #64
	bne	2b

	/*
	 * The SHA functions are:
	 *
	 * f1(B,C,D) = (D ^ (B & (C ^ D)))
	 * f2(B,C,D) = (B ^ C ^ D)
	 * f3(B,C,D) = ((B & C) | (D & (B | C)))
	 *
	 * Then the sub-blocks are processed as follows:
	 *
	 * A' = ror(A, 27) + f(B,C,D) + E + K + *W++
	 * B' = A
	 * C' = ror(B, 2)
	 * D' = C
	 * E' = D
	 *
	 * We therefore unroll each loop 5 times to avoid register shuffling.
	 * Also the ror for C (and also D and E which are successivelyderived
	 * from it) is applied in place to cut on an additional mov insn for
	 * each round.
	 */

	.macro	sha_f1, A, B, C, D, E
	ldw.w	r3, [r2]+, #4
	xor	ip, \C, \D
	add	\E, r1, \E <> #2
	and	ip, \B, ip <> #2
	add	\E, \E, \A <> #27
	xor	ip, ip, \D <> #2
	add	\E, \E, r3
	add	\E, \E, ip
	.endm

	.macro	sha_f2, A, B, C, D, E
	ldw.w	r3, [r2]+, #4
	add	\E, r1, \E <> #2
	xor	ip, \B, \C <> #2
	add	\E, \E, \A <> #27
	xor	ip, ip, \D <> #2
	add	\E, \E, r3
	add	\E, \E, ip
	.endm

	.macro	sha_f3, A, B, C, D, E
	ldw.w	r3, [r2]+, #4
	add	\E, r1, \E <> #2
	or	ip, \B, \C <> #2
	add	\E, \E, \A <> #27
	and	ip, ip, \D <> #2
	add	\E, \E, r3
	and	r3, \B, \C <> #2
	or	ip, ip, r3
	add	\E, \E, ip
	.endm

	ldm	(r4 - r8), [r0]+

	mov	lr, #4
	ldw	r1, .L_sha_K + 0

	/* adjust initial values */
	mov	r6, r6 <> #30
	mov	r7, r7 <> #30
	mov	r8, r8 <> #30

3:	sub.a	lr, lr, #1
	sha_f1	r4, r5, r6, r7, r8
	sha_f1	r8, r4, r5, r6, r7
	sha_f1	r7, r8, r4, r5, r6
	sha_f1	r6, r7, r8, r4, r5
	sha_f1	r5, r6, r7, r8, r4
	bne	3b

	ldw	r1, .L_sha_K + 4
	mov	lr, #4

4:	sub.a	lr, lr, #1
	sha_f2	r4, r5, r6, r7, r8
	sha_f2	r8, r4, r5, r6, r7
	sha_f2	r7, r8, r4, r5, r6
	sha_f2	r6, r7, r8, r4, r5
	sha_f2	r5, r6, r7, r8, r4
	bne	4b

	ldw	r1, .L_sha_K + 8
	mov	lr, #4

5:	sub.a	lr, lr, #1
	sha_f3	r4, r5, r6, r7, r8
	sha_f3	r8, r4, r5, r6, r7
	sha_f3	r7, r8, r4, r5, r6
	sha_f3	r6, r7, r8, r4, r5
	sha_f3	r5, r6, r7, r8, r4
	bne	5b

	ldw	r1, .L_sha_K + 12
	mov	lr, #4

6:	sub.a	lr, lr, #1
	sha_f2	r4, r5, r6, r7, r8
	sha_f2	r8, r4, r5, r6, r7
	sha_f2	r7, r8, r4, r5, r6
	sha_f2	r6, r7, r8, r4, r5
	sha_f2	r5, r6, r7, r8, r4
	bne	6b

	ldm	(r1, r2, r3, r9, r10), [r0]+
	add	r4, r1, r4
	add	r5, r2, r5
	add	r6, r3, r6 <> #2
	add	r7, r9, r7 <> #2
	add	r8, r10, r8 <> #2
	stm	(r4 - r8), [r0]+

	ldm.w	(pc), [sp]+

ENDPROC(sha_transform)

	.align	2
.L_sha_K:
	.word	0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6


/*
 * void sha_init(__u32 *buf)
 */

	.align	2
.L_sha_initial_digest:
	.word	0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0

ENTRY(sha_init)

	adr	r1, .L_sha_initial_digest
	ldm	(r1 - r5), [r1]+
	stm	(r1 - r5), [r0]+
	mov	pc, lr

ENDPROC(sha_init)
