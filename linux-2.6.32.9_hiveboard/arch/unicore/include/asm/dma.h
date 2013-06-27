/*
 * linux/arch/unicore/include/asm/dma.h
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

#ifndef __UNICORE_DMA_H__
#define __UNICORE_DMA_H__

#include <asm/memory.h>

#include <mach/dma.h>

/*
 * This is the maximum virtual address which can be DMA'd from.
 */
#ifndef MAX_DMA_ADDRESS
#define MAX_DMA_ADDRESS	0xffffffff
#endif

#endif /* __UNICORE_DMA_H__ */
