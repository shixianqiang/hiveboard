/*
 * linux/arch/unicore/lib/strchr.S
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
ENTRY(strchr)
		and	r1, r1, #0xff
1:		ldb.w	r2, [r0]+, #1
		cmpxor.a	r2, r1
		beq	201f
		cmpxor.a	r2, #0
		bne	1b
201:		
		cmpxor.a	r2, r1
		cmovne	r0, #0
		cmovne	pc, lr
		sub	r0, r0, #1
		mov	pc, lr
ENDPROC(strchr)
