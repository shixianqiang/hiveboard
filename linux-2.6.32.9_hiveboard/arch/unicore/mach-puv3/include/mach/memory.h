/*
 * linux/arch/unicore/mach-puv3/include/mach/memory.h
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
#ifndef __MACH_PUV3_MEMORY_H__
#define __MACH_PUV3_MEMORY_H__

#include <mach/hardware.h>

/*
 * Physical DRAM offset.
 */
#define PHYS_OFFSET	UL(0x00000000)

#if !defined(__ASSEMBLY__) && defined(CONFIG_PCI)

void puv3_pci_adjust_zones(int node, unsigned long *size, unsigned long *holes);

#define arch_adjust_zones(node, size, holes) \
	puv3_pci_adjust_zones(node, size, holes)

#define ISA_DMA_THRESHOLD	(PHYS_OFFSET + SZ_128M - 1)
#define MAX_DMA_ADDRESS		(PAGE_OFFSET + SZ_128M)

#endif

#define is_pcibus_device(dev)	(dev && (strncmp(dev->bus->name, "pci", 3) == 0))

#define __virt_to_pcibus(x)     (__virt_to_phys(x) + PKUNITY_PCIAHB_BASE)
#define __pcibus_to_virt(x)     __phys_to_virt((x) - PKUNITY_PCIAHB_BASE) 

/* kuser area */
#define KUSER_VECPAGE_BASE	(CONFIG_KUSER_BASE + UL(0x3fff0000))
#define KUSER_UNIGFX_BASE	(CONFIG_KUSER_BASE + PKUNITY_UNIGFX_MMAP_BASE)
/* kuser_vecpage (0xbfff0000) is ro, and vectors page (0xffff0000) is rw */
#define kuser_vecpage_to_vectors(x)	((x) - KUSER_VECPAGE_BASE + CONFIG_VECTORS_BASE)

#endif
