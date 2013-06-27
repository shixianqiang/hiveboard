/*
 * linux/arch/unicore/lib/csumpartialcopy.S
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

/* Function: __u32 csum_partial_copy_nocheck(const char *src, char *dst, int len, __u32 sum)
 * Params  : r0 = src, r1 = dst, r2 = len, r3 = checksum
 * Returns : r0 = new checksum
 */

		.macro	save_regs
		mov	ip, sp
		stm.w	(fp, ip, lr, pc), [sp-]
		stm.w	(r1), [sp-]
		sub	fp, ip, #4
		.endm

		.macro	load_regs
		ldm.w	(r1), [sp]+
		ldm	(fp, sp, pc), [sp]+
		.endm

		.macro	load1b, reg1
		ldb.w	\reg1, [r0]+, #1
		.endm

		.macro	load2b, reg1, reg2
		ldb.w	\reg1, [r0]+, #1
		ldb.w	\reg2, [r0]+, #1
		.endm

		.macro	load1l, reg1
		ldw.w	\reg1, [r0]+, #4
		.endm

		.macro	load2l, reg1, reg2
		ldw.w	\reg1, [r0]+, #4
		ldw.w	\reg2, [r0]+, #4
		.endm

		.macro	load4l, reg1, reg2, reg3, reg4
		ldm.w	(\reg1, \reg2, \reg3, \reg4), [r0]+
		.endm

#define FN_ENTRY	ENTRY(csum_partial_copy_nocheck)
#define FN_EXIT		ENDPROC(csum_partial_copy_nocheck)

#include "csumpartialcopygeneric.S"
