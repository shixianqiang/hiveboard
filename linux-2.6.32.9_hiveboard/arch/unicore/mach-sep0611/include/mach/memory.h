/* linux/arch/unicore/mach-sep0611/include/mach/memory.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *
 * Southeast University ASIC SoC support
 *
 * This file contains the memory physical definitions of the SEP0611
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  09-04-2010	Changelog initial version
 *
 */
 
#ifndef __ASM_ARCH_MMU_H
#define __ASM_ARCH_MMU_H



/*
 * This decides where the kernel will search for a free chunk of vm
 * space during mmap's.
 */

/*
 * Page offset: 3GB
 */
#define PHYS_OFFSET	UL(0x40000000)
#define CONSISTENT_DMA_SIZE (SZ_4M+SZ_8M)
/* kuser area */
#define KUSER_VECPAGE_BASE      (CONFIG_KUSER_BASE + UL(0x3fff0000))
#define KUSER_UNIGFX_BASE       (CONFIG_KUSER_BASE + PKUNITY_UNIGFX_MMAP_BASE)
/* kuser_vecpage (0xbfff0000) is ro, and vectors page (0xffff0000) is rw */
#define kuser_vecpage_to_vectors(x)     ((x) - KUSER_VECPAGE_BASE + CONFIG_VECTORS_BASE)


#endif

