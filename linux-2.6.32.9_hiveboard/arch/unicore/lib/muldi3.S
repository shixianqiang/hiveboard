/*
 * linux/arch/unicore/lib/muldi3.S
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

ENTRY(__muldi3)

	mul	xh, yl, xh
	mula	xh, xl, yh, xh
	mov	ip, xl >> #16
	mov	yh, yl >> #16
	andn	xl, xl, ip << #16
	andn	yl, yl, yh << #16
	mula	xh, yh, ip, xh
	mul	yh, xl, yh
	mul	xl, yl, xl
	mul	ip, yl, ip
	add.a	xl, xl, yh << #16
	addc	xh, xh, yh >> #16
	add.a	xl, xl, ip << #16
	addc	xh, xh, ip >> #16
	mov	pc, lr

ENDPROC(__muldi3)
