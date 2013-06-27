/*
 * linux/arch/unicore/mm/tlb-ucv2.S
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
#include <linux/linkage.h>
#include <asm/asm-offsets.h>
#include <asm/page.h>
#include <asm/tlbflush.h>
#include "proc-macros.S"

/*
 *	ucv2_flush_user_tlb_range(start, end, vma)
 *
 *	Invalidate a range of TLB entries in the specified address space.
 *
 *	- start - start address (may not be aligned)
 *	- end   - end address (exclusive, may not be aligned)
 *	- vma   - vma_struct describing address range
 */
ENTRY(ucv2_flush_user_tlb_range)
#ifndef	CONFIG_CPU_TLB_SINGLE_ENTRY_DISABLE
	mov	r0, r0 >> #PAGE_SHIFT		@ align address
	mov	r0, r0 << #PAGE_SHIFT
	vma_vm_flags r2, r2			@ get vma->vm_flags
1:
	movc	p0.c6, r0, #3
	nop8

	cmpand.a	r2, #VM_EXEC		@ Executable area ?
	beq	2f

	movc	p0.c6, r0, #5
	nop8
2:	
	add	r0, r0, #PAGE_SZ
	cmpsub.a	r0, r1
	beb	1b
#else
	movc	p0.c6, r0, #2
	nop8
	
	cmpand.a	r2, #VM_EXEC		@ Executable area ?
	beq	2f

	movc	p0.c6, r0, #4
	nop8
2:
#endif
	mov	pc, lr

/*
 *	ucv2_flush_kern_tlb_range(start,end)
 *
 *	Invalidate a range of kernel TLB entries
 *
 *	- start - start address (may not be aligned)
 *	- end   - end address (exclusive, may not be aligned)
 */
ENTRY(ucv2_flush_kern_tlb_range)
#ifndef	CONFIG_CPU_TLB_SINGLE_ENTRY_DISABLE
	mov	r0, r0 >> #PAGE_SHIFT		@ align address
	mov	r0, r0 << #PAGE_SHIFT
1:
	movc	p0.c6, r0, #3
	nop8

	movc	p0.c6, r0, #5
	nop8

	add	r0, r0, #PAGE_SZ
	cmpsub.a	r0, r1
	beb	1b
#else
	movc	p0.c6, r0, #2
	nop8
	
	movc	p0.c6, r0, #4
	nop8
#endif
	mov	pc, lr

	__INIT

	.type	ucv2_tlb_fns, #object
ENTRY(ucv2_tlb_fns)
	.long	ucv2_flush_user_tlb_range
	.long	ucv2_flush_kern_tlb_range
	.long	ucv2_tlb_flags
	.size	ucv2_tlb_fns, . - ucv2_tlb_fns
