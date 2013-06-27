/*
 * linux/arch/unicore/lib/lshrdi3.S
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

#define al r0
#define ah r1

ENTRY(__lshrdi3)

	sub.a	r3, r2, #32
	rsub	ip, r2, #32
	cmovfs	al, al >> r2
	cmovns	al, ah >> r3
	bns	201f
	or	al, al, ah << ip
201:
	mov	ah, ah >> r2
	mov	pc, lr

ENDPROC(__lshrdi3)
