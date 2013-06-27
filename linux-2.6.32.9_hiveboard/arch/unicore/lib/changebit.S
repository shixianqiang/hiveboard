/*
 * linux/arch/unicore/lib/changebit.S
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
#include "bitops.h"
                .text

/* Purpose  : Function to change a bit
 * Prototype: int change_bit(int bit, void *addr)
 */
ENTRY(_change_bit_le)
	bitop	xor
ENDPROC(_change_bit_le)
