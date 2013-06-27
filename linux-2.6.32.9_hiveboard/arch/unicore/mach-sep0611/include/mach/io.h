/* linux/arch/unicore/mach-sep0611/include/mach/io.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *
 * Southeast University ASIC SoC support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
 
#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

#ifdef __CHECKER__
# define __user		__attribute__((noderef, address_space(1)))
# define __kernel	/* default address space */
# define __safe		__attribute__((safe))
# define __force	__attribute__((force))
# define __nocast	__attribute__((nocast))
# define __iomem	__attribute__((noderef, address_space(2)))
# define __acquires(x)	__attribute__((context(0,1)))
# define __releases(x)	__attribute__((context(1,0)))
# define __acquire(x)	__context__(1)
# define __release(x)	__context__(-1)
extern void __chk_user_ptr(void __user *);
extern void __chk_io_ptr(void __iomem *);
#else
# define __user
# define __kernel
# define __safe
# define __force
# define __nocast
# define __iomem
# define __chk_user_ptr(x) (void)0
# define __chk_io_ptr(x) (void)0
# define __builtin_warning(x, y...) (1)
# define __acquires(x)
# define __releases(x)
# define __acquire(x) (void)0
# define __release(x) (void)0
#endif


#define IO_SPACE_LIMIT 0xffffffff

#define __io(a)			((void __iomem *)(a))
#define __mem_pci(a)		(a)
#define __mem_isa(a)		(a)

/*
 * We don't support ins[lb]/outs[lb].  Make them fault.
 */
#define __raw_readsb(p,d,l)	do { *(int *)0 = 0; } while (0)
#define __raw_readsl(p,d,l)	do { *(int *)0 = 0; } while (0)
#define __raw_writesb(p,d,l)	do { *(int *)0 = 0; } while (0)
#define __raw_writesl(p,d,l)	do { *(int *)0 = 0; } while (0)

#endif
