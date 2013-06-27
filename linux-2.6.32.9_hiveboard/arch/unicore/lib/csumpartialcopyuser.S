/*
 * linux/arch/unicore/lib/csumpartialcopyuser.S
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
 * 27/03/03 Ian Molton Clean up CONFIG_CPU
 *
 */
#include <linux/linkage.h>
#include <asm/assembler.h>
#include <asm/errno.h>
#include <asm/asm-offsets.h>

		.text

		.macro	save_regs
		mov	ip, sp
		stm.w	(fp, ip, lr, pc), [sp-]
		stm.w	(r1 - r2), [sp-]
		sub	fp, ip, #4
		.endm

		.macro	load_regs
		ldm.w	(r1 - r2), [sp]+
		ldm	(fp, sp, pc), [sp]+
		.endm

		.macro	load1b,	reg1
		ldrusr	\reg1, r0, 1
		.endm

		.macro	load2b, reg1, reg2
		ldrusr	\reg1, r0, 1
		ldrusr	\reg2, r0, 1
		.endm

		.macro	load1l, reg1
		ldrusr	\reg1, r0, 4
		.endm

		.macro	load2l, reg1, reg2
		ldrusr	\reg1, r0, 4
		ldrusr	\reg2, r0, 4
		.endm

		.macro	load4l, reg1, reg2, reg3, reg4
		ldrusr	\reg1, r0, 4
		ldrusr	\reg2, r0, 4
		ldrusr	\reg3, r0, 4
		ldrusr	\reg4, r0, 4
		.endm

/*
 * unsigned int
 * csum_partial_copy_from_user(const char *src, char *dst, int len, int sum, int *err_ptr)
 *  r0 = src, r1 = dst, r2 = len, r3 = sum, [sp] = *err_ptr
 *  Returns : r0 = checksum, [[sp, #0], #0] = 0 or -EFAULT
 */

#define FN_ENTRY	ENTRY(csum_partial_copy_from_user)
#define FN_EXIT		ENDPROC(csum_partial_copy_from_user)

#include "csumpartialcopygeneric.S"

/*
 * FIXME: minor buglet here
 * We don't return the checksum for the data present in the buffer.  To do
 * so properly, we would have to add in whatever registers were loaded before
 * the fault, which, with the current asm above is not predictable.
 */
		.section .fixup,"ax"
		.align	4
9001:		mov	r4, #-EFAULT
		ldw	r5, [fp+], #4		@ *err_ptr
		stw	r4, [r5]
		ldm	(r1, r2), [sp]+		@ retrieve dst, len
		add	r2, r2, r1
		mov	r0, #0			@ zero the buffer
9002:		cmpxor.a	r2, r1
		beq	201f
		stb.w	r0, [r1]+, #1
		b	9002b
201:
		load_regs
		.previous
