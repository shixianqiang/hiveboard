/*
 * linux/arch/unicore/mm/copypage-ucv2.c
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
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/highmem.h>

#include <asm/pgtable.h>
#include <asm/shmparam.h>

#if SHMLBA > 16384
#error FIX ME
#endif

/*
 * Copy the user page.  No aliasing to deal with so we can just
 * attack the kernel's existing mapping of these pages.
 */
void ucv2_copy_user_highpage(struct page *to,
	struct page *from, unsigned long vaddr)
{
	void *kto, *kfrom;

	kfrom = kmap_atomic(from, KM_USER0);
	kto = kmap_atomic(to, KM_USER1);
	copy_page(kto, kfrom);
	kunmap_atomic(kto, KM_USER1);
	kunmap_atomic(kfrom, KM_USER0);
}

/*
 * Clear the user page.  No aliasing to deal with so we can just
 * attack the kernel's existing mapping of this page.
 */
void ucv2_clear_user_highpage(struct page *page, unsigned long vaddr)
{
	void *kaddr = kmap_atomic(page, KM_USER0);
	clear_page(kaddr);
	kunmap_atomic(kaddr, KM_USER0);
}

struct cpu_user_fns ucv2_user_fns __initdata = {
	.cpu_clear_user_highpage	= ucv2_clear_user_highpage,
	.cpu_copy_user_highpage		= ucv2_copy_user_highpage,
};

