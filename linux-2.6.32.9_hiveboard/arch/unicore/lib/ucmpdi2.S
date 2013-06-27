/*
 * linux/arch/unicore/lib/ucmpdi2.S
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

#define xl r0
#define xh r1
#define yl r2
#define yh r3

ENTRY(__ucmpdi2)

	cmpsub.a	xh, yh
	bne	201f
	cmpsub.a	xl, yl
201:	cmovub	r0, #0
	cmoveq	r0, #1
	cmovua	r0, #2
	mov	pc, lr

ENDPROC(__ucmpdi2)

