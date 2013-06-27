/*
 * linux/arch/unicore/include/asm/page.h
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
#ifndef __UNICORE_PAGE_H__
#define __UNICORE_PAGE_H__

/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT		12
#define PAGE_SIZE		(_AC(1,UL) << PAGE_SHIFT)
#define PAGE_MASK		(~(PAGE_SIZE-1))

#ifndef __ASSEMBLY__

#include <asm/glue.h>

/*
 *	User Space Model
 *	================
 *
 *	This section selects the correct set of functions for dealing with
 *	page-based copying and clearing for user space for the particular
 *	processor(s) we're building for.
 *
 *	We have the following to choose from:
 *	  ucv1		- UNICOREv1
 *	  ucv2		- UNICOREv2
 */
#undef _USER

#ifdef CONFIG_CPU_UCV1
//#  define _USER ucv1
#endif

#ifdef CONFIG_CPU_UCV2
#  define _USER ucv2
#endif

#ifndef _USER
#error Unknown user operations model
#endif

struct page;

struct cpu_user_fns {
	void (*cpu_clear_user_highpage)(struct page *page, unsigned long vaddr);
	void (*cpu_copy_user_highpage)(struct page *to, struct page *from,
			unsigned long vaddr);
};

#define __cpu_clear_user_highpage	__glue(_USER,_clear_user_highpage)
#define __cpu_copy_user_highpage	__glue(_USER,_copy_user_highpage)

extern void __cpu_clear_user_highpage(struct page *page, unsigned long vaddr);
extern void __cpu_copy_user_highpage(struct page *to, struct page *from,
			unsigned long vaddr);

#define clear_user_highpage(page,vaddr)		\
	 __cpu_clear_user_highpage(page, vaddr)

#define __HAVE_ARCH_COPY_USER_HIGHPAGE
#define copy_user_highpage(to,from,vaddr,vma)	\
	__cpu_copy_user_highpage(to, from, vaddr)

#define clear_page(page)	memset((void *)(page), 0, PAGE_SIZE)
extern void copy_page(void *to, const void *from);

#undef STRICT_MM_TYPECHECKS

#ifdef STRICT_MM_TYPECHECKS
/*
 * These are used to make use of C type-checking..
 */
typedef struct { unsigned long pte; } pte_t;
typedef struct { unsigned long pgd; } pgd_t;
typedef struct { unsigned long pgprot; } pgprot_t;

#define pte_val(x)      ((x).pte)
#define pgd_val(x)	((x).pgd)
#define pgprot_val(x)   ((x).pgprot)

#define __pte(x)        ((pte_t) { (x) } )
#define __pgd(x)	((pgd_t) { (x) } )
#define __pgprot(x)     ((pgprot_t) { (x) } )

#else
/*
 * .. while these make it easier on the compiler
 */
typedef unsigned long pte_t;
typedef unsigned long pgd_t;
typedef unsigned long pgprot_t;

#define pte_val(x)      (x)
#define pgd_val(x)      (x)
#define pgprot_val(x)   (x)

#define __pte(x)        (x)
#define __pgd(x)	(x)
#define __pgprot(x)     (x)

#endif /* STRICT_MM_TYPECHECKS */

typedef struct page *pgtable_t;

#ifndef CONFIG_SPARSEMEM
extern int pfn_valid(unsigned long);
#endif

#include <asm/memory.h>

#endif /* !__ASSEMBLY__ */

#define VM_DATA_DEFAULT_FLAGS \
	(VM_READ | VM_WRITE | VM_EXEC | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC)

#include <asm-generic/getorder.h>

#endif
