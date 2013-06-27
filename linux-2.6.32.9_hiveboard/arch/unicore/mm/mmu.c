/*
 * linux/arch/unicore/mm/mmu.c
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
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/bootmem.h>
#include <linux/mman.h>
#include <linux/nodemask.h>

#include <asm/cputype.h>
#include <asm/sections.h>
#include <asm/setup.h>
#include <asm/sizes.h>
#include <asm/tlb.h>
#include <asm/highmem.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include "mm.h"

DEFINE_PER_CPU(struct mmu_gather, mmu_gathers);

/*
 * empty_zero_page is a special page that is used for
 * zero-initialized data and COW.
 */
struct page *empty_zero_page;
EXPORT_SYMBOL(empty_zero_page);

/*
 * The pmd table for the upper-most set of pages.
 */
pmd_t *top_pmd;

pgprot_t pgprot_user;
pgprot_t pgprot_kernel;

EXPORT_SYMBOL(pgprot_user);
EXPORT_SYMBOL(pgprot_kernel);

static int __init noalign_setup(char *__unused)
{
	cr_alignment &= ~CR_A;
	cr_no_alignment &= ~CR_A;
	set_cr(cr_alignment);
	return 1;
}
__setup("noalign", noalign_setup);

void adjust_cr(unsigned long mask, unsigned long set)
{
	unsigned long flags;

	mask &= ~CR_A;

	set &= mask;

	local_irq_save(flags);

	cr_no_alignment = (cr_no_alignment & ~mask) | set;
	cr_alignment = (cr_alignment & ~mask) | set;

	set_cr((get_cr() & ~mask) | set);

	local_irq_restore(flags);
}

#define PROT_PTE_DEVICE		L_PTE_PRESENT|L_PTE_YOUNG|L_PTE_DIRTY|L_PTE_USER|L_PTE_WRITE
#define PROT_SECT_DEVICE	PMD_TYPE_SECT|PMD_PRESENT|PMD_SECT_READ|PMD_SECT_WRITE

static struct mem_type mem_types[] = {
	[MT_DEVICE] = {		  /* Strongly ordered */
		.prot_pte	= PROT_PTE_DEVICE,
		.prot_l1	= PMD_TYPE_TABLE | PMD_PRESENT,
		.prot_sect	= PROT_SECT_DEVICE,
	},
	/* MT_KUSER: pte for vecpage -- cacheable, and sect for unigfx mmap -- noncacheable */
	[MT_KUSER] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY | L_PTE_CACHEABLE |
				L_PTE_USER | L_PTE_EXEC,
		.prot_l1   = PMD_TYPE_TABLE | PMD_PRESENT,
		.prot_sect = PROT_SECT_DEVICE,
	},
	[MT_HIGH_VECTORS] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY | L_PTE_CACHEABLE |
				L_PTE_USER | L_PTE_WRITE | L_PTE_EXEC,
		.prot_l1   = PMD_TYPE_TABLE | PMD_PRESENT,
	},
	[MT_MEMORY] = {
		.prot_sect = PMD_TYPE_SECT | PMD_PRESENT | PMD_SECT_CACHEABLE |
                             PMD_SECT_READ | PMD_SECT_WRITE | PMD_SECT_EXEC,
	},
	[MT_ROM] = {
		.prot_sect = PMD_TYPE_SECT | PMD_PRESENT | PMD_SECT_CACHEABLE | PMD_SECT_READ,
	},
};

const struct mem_type *get_mem_type(unsigned int type)
{
	return type < ARRAY_SIZE(mem_types) ? &mem_types[type] : NULL;
}
EXPORT_SYMBOL(get_mem_type);

/*
 * Adjust the PMD section entries according to the CPU in use.
 */
static void __init build_mem_type_table(void)
{
	pgprot_user   = __pgprot(L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_CACHEABLE);
	pgprot_kernel = __pgprot(L_PTE_PRESENT | L_PTE_YOUNG |
				 L_PTE_DIRTY | L_PTE_USER | L_PTE_WRITE |
				 L_PTE_EXEC | L_PTE_CACHEABLE);
}

#define vectors_base()	(vectors_high() ? 0xffff0000 : 0)

static void __init alloc_init_pte(pmd_t *pmd, unsigned long addr,
				  unsigned long end, unsigned long pfn,
				  const struct mem_type *type)
{
	pte_t *pte;

	if (pmd_none(*pmd)) {
		pte = alloc_bootmem_low_pages(PTRS_PER_PTE * sizeof(pte_t));
		__pmd_populate(pmd, __pa(pte) | type->prot_l1);
	}

	pte = pte_offset_kernel(pmd, addr);
	do {
		set_pte(pte, pfn_pte(pfn, __pgprot(type->prot_pte)));
		pfn++;
	} while (pte++, addr += PAGE_SIZE, addr != end);
}

static void __init alloc_init_section(pgd_t *pgd, unsigned long addr,
				      unsigned long end, unsigned long phys,
				      const struct mem_type *type)
{
	pmd_t *pmd = pmd_offset((pud_t *)pgd, addr);

	/*
	 * Try a section mapping - end, addr and phys must all be aligned
	 * to a section boundary.
	 */
	if (((addr | end | phys) & ~SECTION_MASK) == 0) {
		pmd_t *p = pmd;

		do {
			set_pmd(pmd, __pmd(phys | type->prot_sect));
			phys += SECTION_SIZE;
		} while (pmd++, addr += SECTION_SIZE, addr != end);

		flush_pmd_entry(p);
	} else {
		/*
		 * No need to loop; pte's aren't interested in the
		 * individual L1 entries.
		 */
		alloc_init_pte(pmd, addr, end, __phys_to_pfn(phys), type);
	}
}

/*
 * Create the page directory entries and any necessary
 * page tables for the mapping specified by `md'.  We
 * are able to cope here with varying sizes and address
 * offsets, and we take full advantage of sections.
 */
void __init create_mapping(struct map_desc *md)
{
	unsigned long phys, addr, length, end;
	const struct mem_type *type;
	pgd_t *pgd;

	if (md->virtual != vectors_base() && md->virtual < TASK_SIZE) {
		printk(KERN_WARNING "BUG: not creating mapping for "
		       "0x%08llx at 0x%08lx in user region\n",
		       __pfn_to_phys((u64)md->pfn), md->virtual);
		return;
	}

	if ((md->type == MT_DEVICE || md->type == MT_ROM) &&
	    md->virtual >= PAGE_OFFSET && md->virtual < VMALLOC_END) {
		printk(KERN_WARNING "BUG: mapping for 0x%08llx at 0x%08lx "
		       "overlaps vmalloc space\n",
		       __pfn_to_phys((u64)md->pfn), md->virtual);
	}

	type = &mem_types[md->type];

	addr = md->virtual & PAGE_MASK;
	phys = (unsigned long)__pfn_to_phys(md->pfn);
	length = PAGE_ALIGN(md->length + (md->virtual & ~PAGE_MASK));

	if (type->prot_l1 == 0 && ((addr | phys | length) & ~SECTION_MASK)) {
		printk(KERN_WARNING "BUG: map for 0x%08lx at 0x%08lx can not "
		       "be mapped using pages, ignoring.\n",
		       __pfn_to_phys(md->pfn), addr);
		return;
	}

	pgd = pgd_offset_k(addr);
	end = addr + length;
	do {
		unsigned long next = pgd_addr_end(addr, end);

		alloc_init_section(pgd, addr, next, phys, type);

		phys += next - addr;
		addr = next;
	} while (pgd++, addr != end);
}

/*
 * Create the architecture specific mappings
 */
void __init iotable_init(struct map_desc *io_desc, int nr)
{
	int i;

	for (i = 0; i < nr; i++)
		create_mapping(io_desc + i);
}

static unsigned long __initdata vmalloc_reserve = SZ_128M;

/*
 * vmalloc=size forces the vmalloc area to be exactly 'size'
 * bytes. This can be used to increase (or decrease) the vmalloc
 * area - the default is 128m.
 */
static void __init early_vmalloc(char **arg)
{
	vmalloc_reserve = memparse(*arg, arg);

	if (vmalloc_reserve < SZ_16M) {
		vmalloc_reserve = SZ_16M;
		printk(KERN_WARNING
			"vmalloc area too small, limiting to %luMB\n",
			vmalloc_reserve >> 20);
	}

	if (vmalloc_reserve > VMALLOC_END - (PAGE_OFFSET + SZ_32M)) {
		vmalloc_reserve = VMALLOC_END - (PAGE_OFFSET + SZ_32M);
		printk(KERN_WARNING
			"vmalloc area is too big, limiting to %luMB\n",
			vmalloc_reserve >> 20);
	}
}
__early_param("vmalloc=", early_vmalloc);

#define VMALLOC_MIN	(void *)(VMALLOC_END - vmalloc_reserve)

static void __init sanity_check_meminfo(void)
{
	int i, j, highmem = 0;

	for (i = 0, j = 0; i < meminfo.nr_banks; i++) {
		struct membank *bank = &meminfo.bank[j];
		*bank = meminfo.bank[i];

#ifdef CONFIG_HIGHMEM
		if (__va(bank->start) > VMALLOC_MIN ||
		    __va(bank->start) < (void *)PAGE_OFFSET)
			highmem = 1;

		bank->highmem = highmem;

		/*
		 * Split those memory banks which are partially overlapping
		 * the vmalloc area greatly simplifying things later.
		 */
		if (__va(bank->start) < VMALLOC_MIN &&
		    bank->size > VMALLOC_MIN - __va(bank->start)) {
			if (meminfo.nr_banks >= NR_BANKS) {
				printk(KERN_CRIT "NR_BANKS too low, "
						 "ignoring high memory\n");
			} else {
				memmove(bank + 1, bank,
					(meminfo.nr_banks - i) * sizeof(*bank));
				meminfo.nr_banks++;
				i++;
				bank[1].size -= VMALLOC_MIN - __va(bank->start);
				bank[1].start = __pa(VMALLOC_MIN - 1) + 1;
				bank[1].highmem = highmem = 1;
				j++;
			}
			bank->size = VMALLOC_MIN - __va(bank->start);
		}
#else
		bank->highmem = highmem;

		/*
		 * Check whether this memory bank would entirely overlap
		 * the vmalloc area.
		 */
		if (__va(bank->start) >= VMALLOC_MIN ||
		    __va(bank->start) < (void *)PAGE_OFFSET) {
			printk(KERN_NOTICE "Ignoring RAM at %.8lx-%.8lx "
			       "(vmalloc region overlap).\n",
			       bank->start, bank->start + bank->size - 1);
			continue;
		}

		/*
		 * Check whether this memory bank would partially overlap
		 * the vmalloc area.
		 */
		if (__va(bank->start + bank->size) > VMALLOC_MIN ||
		    __va(bank->start + bank->size) < __va(bank->start)) {
			unsigned long newsize = VMALLOC_MIN - __va(bank->start);
			printk(KERN_NOTICE "Truncating RAM at %.8lx-%.8lx "
			       "to -%.8lx (vmalloc region overlap).\n",
			       bank->start, bank->start + bank->size - 1,
			       bank->start + newsize - 1);
			bank->size = newsize;
		}
#endif
		j++;
	}
#ifdef CONFIG_HIGHMEM
	if (highmem) {
		const char *reason = NULL;

		if (reason) {
			printk(KERN_CRIT "HIGHMEM is not supported %s, ignoring high memory\n",
				reason);
			while (j > 0 && meminfo.bank[j - 1].highmem)
				j--;
		}
	}
#endif
	meminfo.nr_banks = j;
}

static inline void prepare_page_table(void)
{
	unsigned long addr;

	/*
	 * Clear out all the mappings below the kernel image.
	 */
	for (addr = 0; addr < MODULES_VADDR; addr += PGDIR_SIZE)
		pmd_clear(pmd_off_k(addr));

	for ( ; addr < PAGE_OFFSET; addr += PGDIR_SIZE)
		pmd_clear(pmd_off_k(addr));

	/*
	 * Clear out all the kernel space mappings, except for the first
	 * memory bank, up to the end of the vmalloc region.
	 */
	for (addr = __phys_to_virt(bank_phys_end(&meminfo.bank[0]));
	     addr < VMALLOC_END; addr += PGDIR_SIZE)
		pmd_clear(pmd_off_k(addr));
}

/*
 * Reserve the various regions of node 0
 */
void __init reserve_node_zero(pg_data_t *pgdat)
{
	unsigned long res_size = 0;

	/*
	 * Register the kernel text and data with bootmem.
	 * Note that this can only be in node 0.
	 */
	reserve_bootmem_node(pgdat, __pa(_stext), _end - _stext,
			BOOTMEM_DEFAULT);

	/*
	 * Reserve the page tables.  These are already in use,
	 * and can only be in node 0.
	 */
	reserve_bootmem_node(pgdat, __pa(swapper_pg_dir),
			     PTRS_PER_PGD * sizeof(pgd_t), BOOTMEM_DEFAULT);

#ifdef CONFIG_PUV3_UNIGFX
	/*
	 * These should likewise go elsewhere.  They pre-reserve the
	 * screen/video memory region at the 48M~64M of main system memory.
	 */
        reserve_bootmem_node(pgdat, PKUNITY_UNIGFX_MMAP_BASE,
                             PKUNITY_UNIGFX_MMAP_SIZE, BOOTMEM_DEFAULT);
        reserve_bootmem_node(pgdat, PKUNITY_UVC_MMAP_BASE,
                             PKUNITY_UVC_MMAP_SIZE, BOOTMEM_DEFAULT);
#endif

	if (res_size)
		reserve_bootmem_node(pgdat, PHYS_OFFSET, res_size,
				BOOTMEM_DEFAULT);
}

/*
 * Set up device the mappings.  Since we clear out the page tables for all
 * mappings above VMALLOC_END, we will remove any debug device mappings.
 * This means you have to be careful how you debug this function, or any
 * called function.  This means you can't use any function or debugging
 * method which may touch any device, otherwise the kernel _will_ crash.
 */
static void __init devicemaps_init(struct machine_desc *mdesc)
{
	struct map_desc map;
	unsigned long addr;
	void *vectors;

	/*
	 * Allocate the vector page early.
	 */
	vectors = alloc_bootmem_low_pages(PAGE_SIZE);

	for (addr = VMALLOC_END; addr; addr += PGDIR_SIZE)
		pmd_clear(pmd_off_k(addr));

	/*
	 * Create a mapping for UniGFX VRAM
	 */
#ifdef CONFIG_PUV3_UNIGFX
	map.pfn = __phys_to_pfn(mdesc->video_start);
	map.virtual = KUSER_UNIGFX_BASE;
	map.length = mdesc->video_end - mdesc->video_start;
	map.type = MT_KUSER;
	create_mapping(&map);
#endif

	/*
	 * Create a mapping for the machine vectors at the high-vectors
	 * location (0xffff0000).  If we aren't using high-vectors, also
	 * create a mapping at the low-vectors virtual address.
	 */
	map.pfn = __phys_to_pfn(virt_to_phys(vectors));
	map.virtual = CONFIG_VECTORS_BASE;
	map.length = PAGE_SIZE;
	map.type = MT_HIGH_VECTORS;
	create_mapping(&map);

	/*
	 * Create a mapping for the kuser page at the special
	 * location (0xbfff0000) to the same vectors location.
	 */
	map.pfn = __phys_to_pfn(virt_to_phys(vectors));
	map.virtual = KUSER_VECPAGE_BASE;
	map.length = PAGE_SIZE;
	map.type = MT_KUSER;
	create_mapping(&map);

	/*
	 * Ask the machine support to map in the statically mapped devices.
	 */
	if (mdesc->map_io)
		mdesc->map_io();

	/*
	 * Finally flush the caches and tlb to ensure that we're in a
	 * consistent state wrt the writebuffer.  This also ensures that
	 * any write-allocated cache lines in the vector page are written
	 * back.  After this point, we can start to touch devices again.
	 */
	local_flush_tlb_all();
	flush_cache_all();
}

static void __init kmap_init(void)
{
#ifdef CONFIG_HIGHMEM
	pmd_t *pmd = pmd_off_k(PKMAP_BASE);
	pte_t *pte = alloc_bootmem_low_pages(PTRS_PER_PTE * sizeof(pte_t));
	BUG_ON(!pmd_none(*pmd) || !pte);
	__pmd_populate(pmd, __pa(pte) | _PAGE_KERNEL_TABLE);
	pkmap_page_table = pte;
#endif
}

/*
 * paging_init() sets up the page tables, initialises the zone memory
 * maps, and sets up the zero page, bad page and bad page tables.
 */
void __init paging_init(struct machine_desc *mdesc)
{
	void *zero_page;

	build_mem_type_table();
	sanity_check_meminfo();
	prepare_page_table();
	bootmem_init();
	devicemaps_init(mdesc);
	kmap_init();

	top_pmd = pmd_off_k(0xffff0000);

	/*
	 * allocate the zero page.  Note that this always succeeds and
	 * returns a zeroed result.
	 */
	zero_page = alloc_bootmem_low_pages(PAGE_SIZE);
	empty_zero_page = virt_to_page(zero_page);
	flush_dcache_page(empty_zero_page);
}

/*
 * In order to soft-boot, we need to insert a 1:1 mapping in place of
 * the user-mode pages.  This will then ensure that we have predictable
 * results when turning the mmu off
 */
void setup_mm_for_reboot(char mode)
{
	unsigned long base_pmdval;
	pgd_t *pgd;
	int i;

	if (current->mm && current->mm->pgd)
		pgd = current->mm->pgd;
	else
		pgd = init_mm.pgd;

	base_pmdval = PMD_SECT_WRITE | PMD_SECT_READ | PMD_TYPE_SECT;

	for (i = 0; i < FIRST_USER_PGD_NR + USER_PTRS_PER_PGD; i++, pgd++) {
		unsigned long pmdval = (i << PGDIR_SHIFT) | base_pmdval;
		pmd_t *pmd;

		pmd = pmd_off(pgd, i << PGDIR_SHIFT);
		set_pmd(pmd, __pmd(pmdval));
		flush_pmd_entry(pmd);
	}
}
