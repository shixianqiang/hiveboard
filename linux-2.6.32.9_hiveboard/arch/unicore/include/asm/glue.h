/*
 * linux/arch/unicore/include/asm/glue.h
 *
 * Code specific to PKUnity SoC and UniCore ISA
 * Fragments that appear the same as the files in arm or x86
 *
 * Copyright (C) 2001-2008 GUAN Xue-tao
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  This file provides the glue to stick the processor-specific bits
 *  into the kernel in an efficient manner.  The idea is to use branches
 *  when we're only targetting one class of TLB, or indirect calls
 *  when we're targetting multiple classes of TLBs.
 */
#ifdef __KERNEL__


#ifdef __STDC__
#define ____glue(name,fn)	name##fn
#else
#define ____glue(name,fn)	name/**/fn
#endif
#define __glue(name,fn)		____glue(name,fn)



/*
 *	Data Abort Model
 *	================
 *
 *	We have the following to choose from:
 *	  UniCore-v1 style
 *	  UniCore-v2 style
 * Now, we have only implemented V2 style, and perhaps V1 style need implement later.
 */
#undef CPU_DABORT_HANDLER

#ifdef CONFIG_CPU_UCV1
//#  define CPU_DABORT_HANDLER unicore_early_abort
#endif

#ifdef CONFIG_CPU_UCV2
#  define CPU_DABORT_HANDLER ucv2_early_abort
#endif

#ifndef CPU_DABORT_HANDLER
#error Unknown data abort handler type
#endif

/*
 * Prefetch abort handler. 
 */
#undef CPU_PABORT_HANDLER

#  define CPU_PABORT_HANDLER legacy_pabort

#ifndef CPU_PABORT_HANDLER
#error Unknown prefetch abort handler type
#endif

#endif
