/*
 * linux/arch/unicore/include/asm/xor.h
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
#include <asm-generic/xor.h>

#define __XOR(a1, a2) a1 ^= a2

#define GET_BLOCK_2(dst) \
	__asm__("ldm	(%1, %2), [%0]+" \
		: "=r" (dst), "=r" (a1), "=r" (a2) \
		: "0" (dst))

#define GET_BLOCK_4(dst) \
	__asm__("ldm	(%1, %2, %3, %4), [%0]+" \
		: "=r" (dst), "=r" (a1), "=r" (a2), "=r" (a3), "=r" (a4) \
		: "0" (dst))

#define XOR_BLOCK_2(src) \
	__asm__("ldm.w	{%1, %2}, [%0]+" \
		: "=r" (src), "=r" (b1), "=r" (b2) \
		: "0" (src)); \
	__XOR(a1, b1); __XOR(a2, b2);

#define XOR_BLOCK_4(src) \
	__asm__("ldm.w	(%1, %2, %3, %4), [%0]+" \
		: "=r" (src), "=r" (b1), "=r" (b2), "=r" (b3), "=r" (b4) \
		: "0" (src)); \
	__XOR(a1, b1); __XOR(a2, b2); __XOR(a3, b3); __XOR(a4, b4)

#define PUT_BLOCK_2(dst) \
	__asm__ __volatile__("stm	(%2, %3), [%0]+" \
		: "=r" (dst) \
		: "0" (dst), "r" (a1), "r" (a2))

#define PUT_BLOCK_4(dst) \
	__asm__ __volatile__("stm	(%2, %3, %4, %5), [%0]+" \
		: "=r" (dst) \
		: "0" (dst), "r" (a1), "r" (a2), "r" (a3), "r" (a4))

static void
xor_uc4regs_2(unsigned long bytes, unsigned long *p1, unsigned long *p2)
{
	unsigned int lines = bytes / sizeof(unsigned long) / 4;
	register unsigned int a1 __asm__("v1");
	register unsigned int a2 __asm__("v2");
	register unsigned int a3 __asm__("v3");
	register unsigned int a4 __asm__("v4");
	register unsigned int b1 __asm__("v5");
	register unsigned int b2 __asm__("v6");
	register unsigned int b3 __asm__("v7");
	register unsigned int b4 __asm__("v8");

	do {
		GET_BLOCK_4(p1);
		XOR_BLOCK_4(p2);
		PUT_BLOCK_4(p1);
	} while (--lines);
}

static void
xor_uc4regs_3(unsigned long bytes, unsigned long *p1, unsigned long *p2,
		unsigned long *p3)
{
	unsigned int lines = bytes / sizeof(unsigned long) / 4;
	register unsigned int a1 __asm__("v1");
	register unsigned int a2 __asm__("v2");
	register unsigned int a3 __asm__("v3");
	register unsigned int a4 __asm__("v4");
	register unsigned int b1 __asm__("v5");
	register unsigned int b2 __asm__("v6");
	register unsigned int b3 __asm__("v7");
	register unsigned int b4 __asm__("v8");

	do {
		GET_BLOCK_4(p1);
		XOR_BLOCK_4(p2);
		XOR_BLOCK_4(p3);
		PUT_BLOCK_4(p1);
	} while (--lines);
}

static void
xor_uc4regs_4(unsigned long bytes, unsigned long *p1, unsigned long *p2,
		unsigned long *p3, unsigned long *p4)
{
	unsigned int lines = bytes / sizeof(unsigned long) / 2;
	register unsigned int a1 __asm__("v5");
	register unsigned int a2 __asm__("v6");
	register unsigned int b1 __asm__("v7");
	register unsigned int b2 __asm__("v8");

	do {
		GET_BLOCK_2(p1);
		XOR_BLOCK_2(p2);
		XOR_BLOCK_2(p3);
		XOR_BLOCK_2(p4);
		PUT_BLOCK_2(p1);
	} while (--lines);
}

static void
xor_uc4regs_5(unsigned long bytes, unsigned long *p1, unsigned long *p2,
		unsigned long *p3, unsigned long *p4, unsigned long *p5)
{
	unsigned int lines = bytes / sizeof(unsigned long) / 2;
	register unsigned int a1 __asm__("v5");
	register unsigned int a2 __asm__("v6");
	register unsigned int b1 __asm__("v7");
	register unsigned int b2 __asm__("v8");

	do {
		GET_BLOCK_2(p1);
		XOR_BLOCK_2(p2);
		XOR_BLOCK_2(p3);
		XOR_BLOCK_2(p4);
		XOR_BLOCK_2(p5);
		PUT_BLOCK_2(p1);
	} while (--lines);
}

static struct xor_block_template xor_block_uc4regs = {
	.name:	"uc4regs",
	.do_2:	xor_uc4regs_2,
	.do_3:	xor_uc4regs_3,
	.do_4:	xor_uc4regs_4,
	.do_5:	xor_uc4regs_5,
};

#undef XOR_TRY_TEMPLATES
#define XOR_TRY_TEMPLATES			\
	do {					\
		xor_speed(&xor_block_uc4regs);	\
		xor_speed(&xor_block_8regs);	\
		xor_speed(&xor_block_32regs);	\
	} while (0)
