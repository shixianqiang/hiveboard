/*
 * linux/arch/unicore/include/asm/ftrace.h
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
#ifndef __UNICORE_FTRACE_H__
#define __UNICORE_FTRACE_H__

#ifdef CONFIG_FUNCTION_TRACER
#define MCOUNT_ADDR		((long)(mcount))
#define MCOUNT_INSN_SIZE	4 /* sizeof mcount call */

#ifndef __ASSEMBLY__
extern void mcount(void);
extern void __gnu_mcount_nc(void);
#endif

#endif

#ifndef __ASSEMBLY__

#if defined(CONFIG_FRAME_POINTER)
/*
 * return_address uses walk_stackframe to do it's work.  If
 * CONFIG_FRAME_POINTER=y walk_stackframe uses unwind
 * information.  For this to work in the function tracer many functions would
 * have to be marked with __notrace.
 */

void *return_address(unsigned int);

#else

extern inline void *return_address(unsigned int level)
{
	return NULL;
}

#endif

#define HAVE_ARCH_CALLER_ADDR

#define CALLER_ADDR0 ((unsigned long)__builtin_return_address(0))
#define CALLER_ADDR1 ((unsigned long)return_address(1))
#define CALLER_ADDR2 ((unsigned long)return_address(2))
#define CALLER_ADDR3 ((unsigned long)return_address(3))
#define CALLER_ADDR4 ((unsigned long)return_address(4))
#define CALLER_ADDR5 ((unsigned long)return_address(5))
#define CALLER_ADDR6 ((unsigned long)return_address(6))

#endif /* ifndef __ASSEMBLY__ */

#endif /* __UNICORE_FTRACE_H__ */
