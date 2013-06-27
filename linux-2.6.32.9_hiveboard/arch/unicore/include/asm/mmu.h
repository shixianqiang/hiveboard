/*
 * linux/arch/unicore/include/asm/mmu.h
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
#ifndef __UNICORE_MMU_H__
#define __UNICORE_MMU_H__

typedef struct {
	unsigned int kvm_seq;
} mm_context_t;

#define ASID(mm)	(0)

#endif
