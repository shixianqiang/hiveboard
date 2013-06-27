/*
 * linux/arch/unicore/mm/flush.c
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
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/pagemap.h>

#include <asm/cacheflush.h>
#include <asm/system.h>
#include <asm/tlbflush.h>

void flush_ptrace_access(struct vm_area_struct *vma, struct page *page,
			 unsigned long uaddr, void *kaddr,
			 unsigned long len, int write)
{
	/* VIPT non-aliasing cache */
	if (cpumask_test_cpu(smp_processor_id(), mm_cpumask(vma->vm_mm)) &&
	    vma->vm_flags & VM_EXEC) {
		unsigned long addr = (unsigned long)kaddr;
		/* only flushing the kernel mapping on non-aliasing VIPT */
		__cpuc_coherent_kern_range(addr, addr + len);
	}
}

void __flush_dcache_page(struct address_space *mapping, struct page *page)
{
	/*
	 * Writeback any data associated with the kernel mapping of this
	 * page.  This ensures that data in the physical page is mutually
	 * coherent with the kernels mapping.
	 */
#ifdef CONFIG_HIGHMEM
	/*
	 * kmap_atomic() doesn't set the page virtual address, and
	 * kunmap_atomic() takes care of cache flushing already.
	 */
	if (page_address(page))
#endif
		__cpuc_flush_dcache_page(page_address(page));
}

/*
 * Ensure cache coherency between kernel mapping and userspace mapping
 * of this page.
 */
void flush_dcache_page(struct page *page)
{
	struct address_space *mapping = page_mapping(page);

	if (!PageHighMem(page) && mapping && !mapping_mapped(mapping))
		set_bit(PG_dcache_dirty, &page->flags);
	else
	{
		__flush_dcache_page(mapping, page);
		if (mapping)
			__flush_icache_all();
	}
}
EXPORT_SYMBOL(flush_dcache_page);
