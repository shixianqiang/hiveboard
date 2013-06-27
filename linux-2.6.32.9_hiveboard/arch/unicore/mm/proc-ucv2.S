/*
 * linux/arch/unicore/mm/proc-ucv2.S
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
#include <asm/assembler.h>
#include <asm/asm-offsets.h>
#include <asm/hwcap.h>
#include <asm/pgtable-hwdef.h>
#include <asm/pgtable.h>

#include "proc-macros.S"

ENTRY(cpu_ucv2_proc_init)
	mov	pc, lr

ENTRY(cpu_ucv2_proc_fin)
	stm.w	(lr), [sp-]
	mov	ip, #PSR_R_BIT | PSR_I_BIT | PRIV_MODE
	mov.a	asr, ip
	b.l	ucv2_flush_kern_cache_all
	ldm.w	(pc), [sp]+

/*
 *	cpu_ucv2_reset(loc)
 *
 *	Perform a soft reset of the system.  Put the CPU into the
 *	same state as it would be if it had been reset, and branch
 *	to what would be the reset vector.
 *
 *	- loc   - location to jump to for soft reset
 */
	.align	5
ENTRY(cpu_ucv2_reset)
	mov	ip, #0
	movc	p0.c5, ip, #28			@ Cache invalidate all
	nop8

	movc	p0.c6, ip, #6			@ TLB invalidate all
	nop8

	movc	ip, p0.c1, #0			@ ctrl register
	or	ip, ip, #0x2000			@ vector base address
	andn	ip, ip, #0x000f			@ ............idam
	movc	p0.c1, ip, #0			@ disable caches and mmu
	nop
	mov	pc, r0				@ jump to loc
	nop8

/*
 *	cpu_ucv2_do_idle()
 *
 *	Idle the processor (eg, wait for interrupt).
 *
 *	IRQs are already disabled.
 */
ENTRY(cpu_ucv2_do_idle)
	mov	r0, #0				@ PCI address
	.rept	8
	ldw	r1, [r0]
	.endr
	mov	pc, lr

ENTRY(cpu_ucv2_dcache_clean_area)
#ifndef CONFIG_CPU_DCACHE_LINE_DISABLE
	cmpsub.a	r1, #MAX_AREA_SIZE
	bsg	101f
	mov	r9, #PAGE_SZ
	sub	r9, r9, #1			@ PAGE_MASK
1:	va2pa	r0, r10, r11, r12, r13		@ r10 is PA
	b	3f
2:	cmpand.a	r0, r9
	beq	1b
3:	movc	p0.c5, r10, #11			@ clean D entry
	nop8
	add	r0, r0, #CACHE_LINESIZE
	add	r10, r10, #CACHE_LINESIZE
	sub.a	r1, r1, #CACHE_LINESIZE
	bua	2b
	mov	pc, lr
#endif
101:	mov	ip, #0
	movc	p0.c5, ip, #10			@ Dcache clean all
	nop8

	mov	pc, lr

/*
 *	cpu_ucv2_switch_mm(pgd_phys)
 *
 *	Set the translation table base pointer to be pgd_phys
 *
 *	- pgd_phys - physical address of new pgd
 *
 *	It is assumed that:
 *	- we are not using split page tables
 */
	.align	5
ENTRY(cpu_ucv2_switch_mm)
	movc	p0.c2, r0, #0			@ update page table ptr
	nop8
	
	movc	p0.c6, ip, #6			@ TLB invalidate all
	nop8

	mov	pc, lr

/*
 *	cpu_ucv2_set_pte(ptep, pte)
 *
 *	Set a level 2 translation table entry.
 *
 *	- ptep  - pointer to level 2 translation table entry
 *	- pte   - PTE value to store
 */
	.align	5
ENTRY(cpu_ucv2_set_pte)
	stw	r1, [r0]
#ifndef CONFIG_CPU_DCACHE_LINE_DISABLE
	sub	r2, r0, #CONFIG_PAGE_OFFSET
	movc	p0.c5, r2, #11				@ Dcache clean line
	nop8
#else
	mov	ip, #0
	movc	p0.c5, ip, #10				@ Dcache clean all
	nop8
	@dcacheline_flush	r0, r2, ip
#endif
	mov	pc, lr


cpu_ucv2_name:
	.asciz	"UniCore II 32-bit processor"
	.align

	__INIT
/*
 *	__ucv2_setup
 *
 *	Initialise TLB, Caches, and MMU state ready to switch the MMU
 *	on.  Return in r0 the new CP0 C1 control register setting.
 */
__ucv2_setup:
	mov	ip, #0
	movc	p0.c5, ip, #28			@ cache invalidate all
	nop8
	movc	p0.c6, ip, #6			@ TLB invalidate all
	nop8

	adr	r5, ucv2_crval
	ldm	(r5, r6), [r5]+
	movc	r0, p0.c1, #0			@ read control register
	andn	r0, r0, r5			@ clear bits them
	or	r0, r0, r6			@ set them
	mov	pc, lr				@ return to head.S:__ret

	/*
	 * ..V. .... ..TB IDAM
	 * ..1. .... ..01 1111
	 */
	.type	ucv2_crval, #object
ucv2_crval:
	crval	clear=0x203f, mmuset=0x201f, ucset=0x0000

	.type	ucv2_processor_functions, #object
ENTRY(ucv2_processor_functions)
	.word	ucv2_early_abort
	.word	legacy_pabort
	.word	cpu_ucv2_proc_init
	.word	cpu_ucv2_proc_fin
	.word	cpu_ucv2_reset
	.word	cpu_ucv2_do_idle
	.word	cpu_ucv2_dcache_clean_area
	.word	cpu_ucv2_switch_mm
	.word	cpu_ucv2_set_pte
	.size	ucv2_processor_functions, . - ucv2_processor_functions

	.type	cpu_arch_name, #object
cpu_arch_name:
	.asciz	"UniCore-II"
	.size	cpu_arch_name, . - cpu_arch_name

	.type	cpu_elf_name, #object
cpu_elf_name:
	.asciz	"ucv2"
	.size	cpu_elf_name, . - cpu_elf_name
	.align

	.section ".proc.info.init", #alloc, #execinstr

	/*
	 * Match any UniCore-II processor core.
	 */
	.type	__ucv2_proc_info, #object
__ucv2_proc_info:
	.long	0x4d000863
	.long	0xff00ffff
	.long   PMD_TYPE_SECT | \
		PMD_PRESENT | \
		PMD_SECT_CACHEABLE | \
		PMD_SECT_READ | \
		PMD_SECT_WRITE | \
		PMD_SECT_EXEC
	.long   PMD_TYPE_SECT | \
		PMD_PRESENT | \
		PMD_SECT_READ | \
		PMD_SECT_WRITE
	b	__ucv2_setup
	.long	cpu_arch_name
	.long	cpu_elf_name
	.long	HWCAP_CMOV
	.long	cpu_ucv2_name
	.long	ucv2_processor_functions
	.long	ucv2_tlb_fns
	.long	ucv2_user_fns
	.long	ucv2_cache_fns
	.size	__ucv2_proc_info, . - __ucv2_proc_info
