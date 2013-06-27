/*
 * linux/arch/unicore/include/asm/ucontext.h
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
#ifndef __UNICORE_UCONTEXT_H__
#define __UNICORE_UCONTEXT_H__

#include <asm/fpstate.h>

/*
 * struct sigcontext only has room for the basic registers, but struct
 * ucontext now has room for all registers which need to be saved and
 * restored.  Coprocessor registers are stored in uc_regspace.  Each
 * coprocessor's saved state should start with a documented 32-bit magic
 * number, followed by a 32-bit word giving the coproccesor's saved size.
 * uc_regspace may be expanded if necessary, although this takes some
 * coordination with glibc.
 */

struct ucontext {
	unsigned long	  uc_flags;
	struct ucontext  *uc_link;
	stack_t		  uc_stack;
	struct sigcontext uc_mcontext;
	sigset_t	  uc_sigmask;
	/* Allow for uc_sigmask growth.  Glibc uses a 1024-bit sigset_t.  */
	int		  __unused[32 - (sizeof (sigset_t) / sizeof (int))];
};

#endif /* __UNICORE_UCONTEXT_H__ */
