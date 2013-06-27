/*
 * linux/arch/unicore/lib/csumpartialcopygeneric.S
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

/*
 * unsigned int
 * csum_partial_copy_xxx(const char *src, char *dst, int len, int sum, )
 *  r0 = src, r1 = dst, r2 = len, r3 = sum
 *  Returns : r0 = checksum
 *
 * Note that 'tst' and 'teq' preserve the carry flag.
 */

src	.req	r0
dst	.req	r1
len	.req	r2
sum	.req	r3

.Lzero:		mov	r0, sum
		load_regs

		/*
		 * Align an unaligned destination pointer.  We know that
		 * we have >= 8 bytes here, so we don't need to check
		 * the length.  Note that the source pointer hasn't been
		 * aligned yet.
		 */
.Ldst_unaligned:
		cmpand.a	dst, #1
		beq	.Ldst_16bit

		load1b	ip
		sub	len, len, #1
		addc.a	sum, sum, ip put_byte_1		@ update checksum
		stb.w	ip, [dst]+, #1
		cmpand.a	dst, #2
		cmoveq	pc, lr				@ dst is now 32bit aligned

.Ldst_16bit:	load2b	r8, ip
		sub	len, len, #2
		addc.a	sum, sum, r8 put_byte_0
		stb.w	r8, [dst]+, #1
		addc.a	sum, sum, ip put_byte_1
		stb.w	ip, [dst]+, #1
		mov	pc, lr				@ dst is now 32bit aligned

		/*
		 * Handle 0 to 7 bytes, with any alignment of source and
		 * destination pointers.  Note that when we get here, C = 0
		 */
.Lless8:	cmpxor.a	len, #0			@ check for zero count
		beq	.Lzero

		/* we must have at least one byte. */
		cmpand.a	dst, #1			@ dst 16-bit aligned
		beq	.Lless8_aligned

		/* Align dst */
		load1b	ip
		sub	len, len, #1
		addc.a	sum, sum, ip put_byte_1		@ update checksum
		stb.w	ip, [dst]+, #1
		cmpand.a	len, #6
		beq	.Lless8_byteonly

1:		load2b	r8, ip
		sub	len, len, #2
		addc.a	sum, sum, r8 put_byte_0
		stb.w	r8, [dst]+, #1
		addc.a	sum, sum, ip put_byte_1
		stb.w	ip, [dst]+, #1
.Lless8_aligned:
		cmpand.a	len, #6
		bne	1b
.Lless8_byteonly:
		cmpand.a	len, #1
		beq	.Ldone
		load1b	r8
		addc.a	sum, sum, r8 put_byte_0		@ update checksum
		stb.w	r8, [dst]+, #1
		b	.Ldone

FN_ENTRY
		save_regs

		cmpsub.a	len, #8			@ Ensure that we have at least
		bub	.Lless8				@ 8 bytes to copy.

		add.a	sum, sum, #0			@ C = 0
		cmpand.a	dst, #3			@ Test destination alignment
		bne.l	.Ldst_unaligned			@ align destination, return here

		/*
		 * Ok, the dst pointer is now 32bit aligned, and we know
		 * that we must have more than 4 bytes to copy.  Note
		 * that C contains the carry from the dst alignment above.
		 */

		cmpand.a	src, #3			@ Test source alignment
		bne	.Lsrc_not_aligned

		/* Routine for src & dst aligned */

		andn.a	ip, len, #15
		beq	2f

1:		load4l	r4, r5, r6, r7
		stm.w	(r4, r5, r6, r7), [dst]+
		addc.a	sum, sum, r4
		addc.a	sum, sum, r5
		addc.a	sum, sum, r6
		addc.a	sum, sum, r7
		sub	ip, ip, #16
		cmpxor.a	ip, #0
		bne	1b

2:		and.a	ip, len, #12
		beq	4f
		cmpand.a	ip, #8
		beq	3f
		load2l	r4, r5
		stm.w	(r4, r5), [dst]+
		addc.a	sum, sum, r4
		addc.a	sum, sum, r5
		cmpand.a	ip, #4
		beq	4f

3:		load1l	r4
		stw.w	r4, [dst]+, #4
		addc.a	sum, sum, r4

4:		and.a	len, len, #3
		beq	.Ldone
		load1l	r4
		cmpand.a	len, #2
		mov	r5, r4 get_byte_0
		beq	.Lexit
		addc.a	sum, sum, r4 push #16
		stb.w	r5, [dst]+, #1
		mov	r5, r4 get_byte_1
		stb.w	r5, [dst]+, #1
		mov	r5, r4 get_byte_2
.Lexit:		cmpand.a	len, #1
		beq	.Ldone
		stb.w	r5, [dst]+, #1
		and	r5, r5, #255
		addc.a	sum, sum, r5 put_byte_0

		/*
		 * If the dst pointer was not 16-bit aligned, we
		 * need to rotate the checksum here to get around
		 * the inefficient byte manipulations in the
		 * architecture independent code.
		 */
.Ldone:		addc	r0, sum, #0
		ldw	sum, [sp+], #0		@ dst
		cmpand.a	sum, #1
		cmovne	r0, r0 <> #8
		load_regs

.Lsrc_not_aligned:
		addc	sum, sum, #0		@ include C from dst alignment
		and	ip, src, #3
		andn	src, src, #3
		load1l	r5
		cmpsub.a	ip, #2
		beq	.Lsrc2_aligned
		bua	.Lsrc3_aligned
		mov	r4, r5 pull #8		@ C = 0
		andn.a	ip, len, #15
		beq	2f
1:		load4l	r5, r6, r7, r8
		or	r4, r4, r5 push #24
		mov	r5, r5 pull #8
		or	r5, r5, r6 push #24
		mov	r6, r6 pull #8
		or	r6, r6, r7 push #24
		mov	r7, r7 pull #8
		or	r7, r7, r8 push #24
		stm.w	(r4, r5, r6, r7), [dst]+
		addc.a	sum, sum, r4
		addc.a	sum, sum, r5
		addc.a	sum, sum, r6
		addc.a	sum, sum, r7
		mov	r4, r8 pull #8
		sub	ip, ip, #16
		cmpxor.a	ip, #0
		bne	1b
2:		and.a	ip, len, #12
		beq	4f
		cmpand.a	ip, #8
		beq	3f
		load2l	r5, r6
		or	r4, r4, r5 push #24
		mov	r5, r5 pull #8
		or	r5, r5, r6 push #24
		stm.w	(r4, r5), [dst]+
		addc.a	sum, sum, r4
		addc.a	sum, sum, r5
		mov	r4, r6 pull #8
		cmpand.a	ip, #4
		beq	4f
3:		load1l	r5
		or	r4, r4, r5 push #24
		stw.w	r4, [dst]+, #4
		addc.a	sum, sum, r4
		mov	r4, r5 pull #8
4:		and.a	len, len, #3
		beq	.Ldone
		mov	r5, r4 get_byte_0
		cmpand.a	len, #2
		beq	.Lexit
		addc.a	sum, sum, r4 push #16
		stb.w	r5, [dst]+, #1
		mov	r5, r4 get_byte_1
		stb.w	r5, [dst]+, #1
		mov	r5, r4 get_byte_2
		b	.Lexit

.Lsrc2_aligned:	mov	r4, r5 pull #16
		add.a	sum, sum, #0
		andn.a	ip, len, #15
		beq	2f
1:		load4l	r5, r6, r7, r8
		or	r4, r4, r5 push #16
		mov	r5, r5 pull #16
		or	r5, r5, r6 push #16
		mov	r6, r6 pull #16
		or	r6, r6, r7 push #16
		mov	r7, r7 pull #16
		or	r7, r7, r8 push #16
		stm.w	(r4, r5, r6, r7), [dst]+
		addc.a	sum, sum, r4
		addc.a	sum, sum, r5
		addc.a	sum, sum, r6
		addc.a	sum, sum, r7
		mov	r4, r8 pull #16
		sub	ip, ip, #16
		cmpxor.a	ip, #0
		bne	1b
2:		and.a	ip, len, #12
		beq	4f
		cmpand.a	ip, #8
		beq	3f
		load2l	r5, r6
		or	r4, r4, r5 push #16
		mov	r5, r5 pull #16
		or	r5, r5, r6 push #16
		stm.w	(r4, r5), [dst]+
		addc.a	sum, sum, r4
		addc.a	sum, sum, r5
		mov	r4, r6 pull #16
		cmpand.a	ip, #4
		beq	4f
3:		load1l	r5
		or	r4, r4, r5 push #16
		stw.w	r4, [dst]+, #4
		addc.a	sum, sum, r4
		mov	r4, r5 pull #16
4:		and.a	len, len, #3
		beq	.Ldone
		mov	r5, r4 get_byte_0
		cmpand.a	len, #2
		beq	.Lexit
		addc.a	sum, sum, r4
		stb.w	r5, [dst]+, #1
		mov	r5, r4 get_byte_1
		stb.w	r5, [dst]+, #1
		cmpand.a	len, #1
		beq	.Ldone
		load1b	r5
		b	.Lexit

.Lsrc3_aligned:	mov	r4, r5 pull #24
		add.a	sum, sum, #0
		andn.a	ip, len, #15
		beq	2f
1:		load4l	r5, r6, r7, r8
		or	r4, r4, r5 push #8
		mov	r5, r5 pull #24
		or	r5, r5, r6 push #8
		mov	r6, r6 pull #24
		or	r6, r6, r7 push #8
		mov	r7, r7 pull #24
		or	r7, r7, r8 push #8
		stm.w	(r4, r5, r6, r7), [dst]+
		addc.a	sum, sum, r4
		addc.a	sum, sum, r5
		addc.a	sum, sum, r6
		addc.a	sum, sum, r7
		mov	r4, r8 pull #24
		sub	ip, ip, #16
		cmpxor.a	ip, #0
		bne	1b
2:		and.a	ip, len, #12
		beq	4f
		cmpand.a	ip, #8
		beq	3f
		load2l	r5, r6
		or	r4, r4, r5 push #8
		mov	r5, r5 pull #24
		or	r5, r5, r6 push #8
		stm.w	(r4, r5), [dst]+
		addc.a	sum, sum, r4
		addc.a	sum, sum, r5
		mov	r4, r6 pull #24
		cmpand.a	ip, #4
		beq	4f
3:		load1l	r5
		or	r4, r4, r5 push #8
		stw.w	r4, [dst]+, #4
		addc.a	sum, sum, r4
		mov	r4, r5 pull #24
4:		and.a	len, len, #3
		beq	.Ldone
		mov	r5, r4 get_byte_0
		cmpand.a	len, #2
		beq	.Lexit
		stb.w	r5, [dst]+, #1
		addc.a	sum, sum, r4
		load1l	r4
		mov	r5, r4 get_byte_0
		stb.w	r5, [dst]+, #1
		addc.a	sum, sum, r4 push #24
		mov	r5, r4 get_byte_1
		b	.Lexit
FN_EXIT
