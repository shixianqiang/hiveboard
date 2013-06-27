/*
 * linux/arch/unicore/lib/memchr.S
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
ENTRY(memchr)
	and	r1, r1, #0xff
1:	sub.a	r2, r2, #1
	bfs	2f
	ldb.w	r3, [r0]+, #1
	cmpxor.a	r3, r1
	bne	1b
	sub	r0, r0, #1
2:	cmovne	r0, #0
	mov	pc, lr
ENDPROC(memchr)
