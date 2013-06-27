/*
 * linux/arch/unicore/lib/csumipv6.S
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

ENTRY(__csum_ipv6_magic)
		stw.w	lr, [sp+], #-4
		add.a	ip, r2, r3
		
		ldm   (r1 - r4), [r1]+
		addc.a	ip, ip, r1
		addc.a	ip, ip, r2
		addc.a	ip, ip, r3
		addc.a	ip, ip, r4
		ldm	(r0 - r3), [r0]+
		addc.a	r0, ip, r0
		addc.a	r0, r0, r1
		addc.a	r0, r0, r2
		ldw	r2, [sp+], #4
		addc.a	r0, r0, r3
		addc.a	r0, r0, r2
		addc.a	r0, r0, #0
		ldm.w	(pc), [sp]+
ENDPROC(__csum_ipv6_magic)

