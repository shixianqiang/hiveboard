/*
 * linux/arch/unicore/include/asm/cpu.h
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
#ifndef __UNICORE_CPU_H__
#define __UNICORE_CPU_H__

#include <linux/percpu.h>

struct cpuinfo_unicore {
	struct cpu	cpu;
};

DECLARE_PER_CPU(struct cpuinfo_unicore, cpu_data);

#endif
