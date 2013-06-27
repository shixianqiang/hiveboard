/*
 * linux/arch/unicore/mach-v8/include/mach/hardware.h
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
 * This file contains the hardware definitions for PKUnity architecture
 */

#ifndef __MACH_V8_HARDWARE_H__
#define __MACH_V8_HARDWARE_H__

#include "PKUnity.h"

#define io_p2v( x )     ( (x) - PKUNITY_IOSPACE_BASE )
#define io_v2p( x )	( (x) + PKUNITY_IOSPACE_BASE )

#ifndef __ASSEMBLY__

# define __REG(x)	(*((volatile unsigned long *)io_p2v(x)))
# define __PREG(x)	(io_v2p((unsigned long)&(x)))

#else

# define __REG(x)	io_p2v(x)
# define __PREG(x)	io_v2p(x)

#endif

#define PCIBIOS_MIN_IO			0x4000 /* should lower than 64KB */
#define PCIBIOS_MIN_MEM			PKUNITY_PCIMEM_BASE

/*
 * We override the standard dma-mask routines for bouncing.
 */
#define	HAVE_ARCH_PCI_SET_DMA_MASK

#define pcibios_assign_all_busses()	1

#endif  /* __MACH_V8_HARDWARE_H__ */
