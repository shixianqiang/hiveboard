/*
 * linux/arch/unicore/include/asm/moudle.h
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
#ifndef __UNICORE_MODULE_H__
#define __UNICORE_MODULE_H__

#define Elf_Shdr	Elf32_Shdr
#define Elf_Sym		Elf32_Sym
#define Elf_Ehdr	Elf32_Ehdr

struct mod_arch_specific
{
	int foo;
};

/*
 * Include the UNICORE architecture version.
 */
#define MODULE_ARCH_VERMAGIC	"UCV2" __stringify(__LINUX_UNICORE_ARCH__) " "

#endif /* __UNICORE_MODULE_H__ */
