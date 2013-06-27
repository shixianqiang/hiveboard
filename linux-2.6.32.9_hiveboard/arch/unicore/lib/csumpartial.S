/*
 * linux/arch/unicore/lib/csumpartial.S
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
 * Function: __u32 csum_partial(const char *src, int len, __u32 sum)
 * Params  : r0 = buffer, r1 = len, r2 = checksum
 * Returns : r0 = new checksum
 */

buf	.req	r0
len	.req	r1
sum	.req	r2
td0	.req	r3
td1	.req	r4
td2	.req	r5
td3	.req	r6

.Lzero:		mov	r0, sum
		add	sp, sp, #4
		ldw.w	pc, [sp]+, #4

		/*
		 * Handle 0 to 7 bytes, with any alignment of source and
		 * destination pointers.  Note that when we get here, C = 0
		 */
.Lless8:		cmpxor.a	len, #0		@ check for zero count
		beq	.Lzero

		/* we must have at least one byte. */
		cmpand.a	buf, #1			@ odd address?
		beq	.Lless4
		mov	sum, sum <> #8
		ldb.w	td0, [buf]+, #1
		sub	len, len, #1
		addc.a	sum, sum, td0 put_byte_1

.Lless4:		cmpand.a	len, #6
		beq	.Lless8_byte

		/* we are now half-word aligned */

.Lless8_wordlp:
		ldh.w	td0, [buf]+, #2
		sub	len, len, #2
		addc.a	sum, sum, td0
		cmpand.a	len, #6
		bne	.Lless8_wordlp

.Lless8_byte:	cmpand.a	len, #1			@ odd number of bytes
		beq	.Ldone
		ldb.w	td0, [buf]+, #1			@ include last byte
		addc.a	sum, sum, td0 put_byte_0	@ update checksum

.Ldone:		addc	r0, sum, #0			@ collect up the last carry
		ldw.w	td0, [sp]+, #4
		cmpand.a	td0, #1			@ check buffer alignment
		cmovne	r0, r0 <> #8			@ rotate checksum by 8 bits
		ldw.w	pc, [sp]+, #4			@ return

.Lnot_aligned:	cmpand.a	buf, #1			@ odd address
		beq	201f
		ldb.w	td0, [buf]+, #1			@ make even
		sub	len, len, #1
		addc.a	sum, sum, td0 put_byte_1	@ update checksum
	201:
		cmpand.a	buf, #2			@ 32-bit aligned?
		beq	201f
		ldh.w	td0, [buf]+, #2			@ make 32-bit aligned
		sub	len, len, #2
		addc.a	sum, sum, td0			@ update checksum
	201:
		mov	pc, lr

ENTRY(csum_partial)
		stm.w	(lr), [sp-]
		stm.w	(buf), [sp-]
		cmpsub.a	len, #8			@ Ensure that we have at least
		bub	.Lless8				@ 8 bytes to copy.

		cmpand.a	buf, #1
		cmovne	sum, sum <> #8

		add.a	sum, sum, #0			@ C = 0
		cmpand.a	buf, #3			@ Test destination alignment
		bne.l	.Lnot_aligned			@ align destination, return here

1:		andn.a	ip, len, #31
		beq	3f

2:		ldm.w	(td0, td1, td2, td3), [buf]+
		addc.a	sum, sum, td0
		addc.a	sum, sum, td1
		addc.a	sum, sum, td2
		addc.a	sum, sum, td3
		ldm.w	(td0, td1, td2, td3), [buf]+
		addc.a	sum, sum, td0
		addc.a	sum, sum, td1
		addc.a	sum, sum, td2
		addc.a	sum, sum, td3
		sub	ip, ip, #32
		cmpxor.a	ip, #0
		bne	2b

3:		cmpand.a	len, #0x1c		@ should not change C
		beq	.Lless4

4:		ldw.w	td0, [buf]+, #4
		sub	len, len, #4
		addc.a	sum, sum, td0
		cmpand.a	len, #0x1c
		bne	4b
		b	.Lless4
ENDPROC(csum_partial)
