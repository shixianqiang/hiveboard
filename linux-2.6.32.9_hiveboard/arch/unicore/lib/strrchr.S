/*
 * linux/arch/unicore/lib/strrchr.S
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
ENTRY(strrchr)
		and	r1, r1, #0xff
		mov	r3, #0
1:		ldb.w	r2, [r0]+, #1
		cmpxor.a	r2, r1
		bne	201f
		sub	r3, r0, #1
201:		
		cmpxor.a	r2, #0
		bne	1b
		mov	r0, r3
		mov	pc, lr
ENDPROC(strrchr)
