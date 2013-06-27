/*
 * linux/arch/unicore/include/asm/proc-fns.h
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
#ifndef __UNICORE_PROC_FNS_H__
#define __UNICORE_PROC_FNS_H__

#ifdef __KERNEL__

#undef CPU_NAME

/*
 * CPU_NAME - the prefix for CPU related functions
 */

#ifdef CONFIG_CPU_UCV1
//#   define CPU_NAME cpu_ucv1
#endif
#ifdef CONFIG_CPU_UCV2
#   define CPU_NAME cpu_ucv2
#endif

#ifndef __ASSEMBLY__

#include <asm/cpu-single.h>
#include <asm/memory.h>

#define cpu_switch_mm(pgd,mm) cpu_do_switch_mm(virt_to_phys(pgd),mm)

#define cpu_get_pgd()	\
	({						\
		unsigned long pg;			\
		__asm__("movc	%0, p0.c2, #0"  	\
			 : "=r" (pg) : : "cc");		\
		pg &= ~0x0fff;				\
		(pgd_t *)phys_to_virt(pg);		\
	})

#endif /* __ASSEMBLY__ */
#endif /* __KERNEL__ */
#endif /* __UNICORE_PROC_FNS_H__ */
