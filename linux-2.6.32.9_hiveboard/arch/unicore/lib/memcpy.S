/*
 * linux/arch/unicore/lib/memcpy.S
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

	.macro ldr1w ptr reg abort
	ldw.w \reg, [\ptr]+, #4
	.endm

	.macro ldr4w ptr reg1 reg2 reg3 reg4 abort
	ldm.w (\reg1, \reg2, \reg3, \reg4), [\ptr]+
	.endm

	.macro ldr8w ptr reg1 reg2 reg3 reg4 reg5 reg6 reg7 reg8 abort
	ldm.w (\reg1, \reg2, \reg3, \reg4, \reg5, \reg6, \reg7, \reg8), [\ptr]+
	.endm

	.macro ldr1b ptr reg cond=al abort
	.ifnc	\cond, al
	b\cond	201f
	b	202f
	.endif
201:	ldb.w \reg, [\ptr]+, #1
202:
	.endm

	.macro str1w ptr reg abort
	stw.w \reg, [\ptr]+, #4
	.endm

	.macro str8w ptr reg1 reg2 reg3 reg4 reg5 reg6 reg7 reg8 abort
	stm.w (\reg1, \reg2, \reg3, \reg4, \reg5, \reg6, \reg7, \reg8), [\ptr]+
	.endm

	.macro str1b ptr reg cond=al abort
	.ifnc	\cond, al
	b\cond	201f
	b	202f
	.endif
201:	stb.w \reg, [\ptr]+, #1
202:
	.endm

	.macro enter
	stm.w (r0), [sp-]
	.endm

	.macro exit
	ldm.w (r0), [sp]+
	mov	pc, lr
	.endm

	.text

/* Prototype: void *memcpy(void *dest, const void *src, size_t n); */

ENTRY(memcpy)

#include "copy_template.S"

ENDPROC(memcpy)
