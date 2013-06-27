/*
 * linux/arch/unicore/mach-v8/include/mach/io.h
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
#ifndef __MACH_V8_IO_H__
#define __MACH_V8_IO_H__

#define IO_SPACE_LIMIT 0xffffffff

/*
 * We don't actually have real ISA buses, but there is so many 
 * drivers out there that might just work if we fake them...
 */
#define __io(a)		__typesafe_io(a)
#define __mem_pci(a)	(a)

#endif
