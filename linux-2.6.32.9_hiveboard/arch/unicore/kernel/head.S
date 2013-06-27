/*
 * linux/arch/unicore/kernel/head.S
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
#include <linux/linkage.h>
#include <linux/init.h>

#include <asm/assembler.h>
#include <asm/ptrace.h>
#include <asm/asm-offsets.h>
#include <asm/memory.h>
#include <asm/thread_info.h>
#include <asm/system.h>

#if (PHYS_OFFSET & 0x001fffff)
#error "PHYS_OFFSET must be at an even 2MiB boundary!"
#endif

#define KERNEL_RAM_VADDR	(PAGE_OFFSET + TEXT_OFFSET)
#define KERNEL_RAM_PADDR	(PHYS_OFFSET + TEXT_OFFSET)


/*
 * swapper_pg_dir is the virtual address of the initial page table.
 * We place the page tables 4K below KERNEL_RAM_VADDR.  Therefore, we must
 * make sure that KERNEL_RAM_VADDR is correctly set.  Currently, we expect
 * the least significant 16 bits to be 0x8000, but we could probably
 * relax this restriction to KERNEL_RAM_VADDR >= PAGE_OFFSET + 0x1000.
 */
#if (KERNEL_RAM_VADDR & 0xffff) != 0x8000
#error KERNEL_RAM_VADDR must start at 0xXXXX8000
#endif

	.globl	swapper_pg_dir
	.equ	swapper_pg_dir, KERNEL_RAM_VADDR - 0x1000

	.macro	pgtbl, rd
	ldw	\rd, =(KERNEL_RAM_PADDR - 0x1000)
	.endm

#define KERNEL_START	KERNEL_RAM_VADDR
#define KERNEL_END	_end

/*
 * Kernel startup entry point.
 * ---------------------------
 *
 * This is normally called from the decompressor code.  The requirements
 * are: MMU = off, D-cache = off, I-cache = dont care, r0 = 0,
 * r1 = machine nr, r2 = atags pointer.
 *
 * This code is mostly position independent, so if you link the kernel at
 * 0xc0008000, you call this at __pa(0xc0008000).
 *
 * We're trying to keep crap to a minimum; DO NOT add any machine specific
 * crap here - that's what the boot loader (or in extreme, well justified
 * circumstances, zImage) is for.
 */
	.section ".text.head", "ax"
ENTRY(stext)
	mov	r9, #PSR_R_BIT | PSR_I_BIT | PRIV_MODE	@ ensure priv mode
	mov.a	asr, r9				@ and irqs disabled
	movc	r9, p0.c0, #0
	b.l	__lookup_processor_type		@ r5=procinfo r9=cpuid
	mov.a	r10, r5				@ invalid processor (r5=0)?
	beq	__error_p			@ yes, error 'p'
	b.l	__lookup_machine_type		@ r5=machinfo
	mov.a	r8, r5				@ invalid machine (r5=0)?
	beq	__error_a			@ yes, error 'a'
	b.l	__vet_atags
	b.l	__create_page_tables

	/*
	 * The following calls CPU specific code in a position independent
	 * manner.  See arch/unicore/mm/proc-*.S for details.  r10 = base of
	 * xxx_proc_info stucture selected by __lookup_machine_type
	 * above.  On return, the CPU will be ready for the MMU to be
	 * turned on, and r0 will hold the CPU control register value.
	 */
	ldw	r13, __switch_data		@ address to jump to after
						@ mmu has been enabled
	adr	lr, __enable_mmu		@ return (PIC) address
	add	pc, r10, #PROCINFO_INITFUNC
ENDPROC(stext)

/*
 * Setup common bits before finally enabling the MMU.  Essentially
 * this is just loading the page table pointer and domain access
 * registers.
 */
__enable_mmu:
#ifndef CONFIG_ALIGNMENT_TRAP
	andn	r0, r0, #CR_A
#endif
#ifdef CONFIG_CPU_DCACHE_DISABLE
	andn	r0, r0, #CR_D
#endif
#ifdef CONFIG_CPU_DCACHE_WRITETHROUGH
	andn	r0, r0, #CR_B
#endif
#ifdef CONFIG_CPU_ICACHE_DISABLE
	andn	r0, r0, #CR_I
#endif
	movc	p0.c2, r4, #0
	b	__turn_mmu_on
ENDPROC(__enable_mmu)

/*
 * Enable the MMU.  This completely changes the stucture of the visible
 * memory space.  You will not be able to trace execution through this.
 *
 *  r0  = cp#0 control register
 *  r13 = *virtual* address to jump to upon completion
 *
 * other registers depend on the function called upon completion
 */
	.align	5
__turn_mmu_on:
	mov	r0, r0
	movc	p0.c1, r0, #0			@ write control reg
	nop					@ fetch inst by phys addr
	mov	pc, r13
	nop; nop; nop; nop; nop; nop; nop; nop	@ fetch inst by phys addr
ENDPROC(__turn_mmu_on)

/*
 * Setup the initial page tables.  We only setup the barest
 * amount which are required to get the kernel running, which
 * generally means mapping in the kernel code.
 *
 * r8  = machinfo
 * r9  = cpuid
 * r10 = procinfo
 *
 * Returns:
 *  r0, r3, r6, r7 corrupted
 *  r4 = physical page table address
 */
__create_page_tables:
	pgtbl	r4				@ page table address

	/*
	 * Clear the 4K level 1 swapper page table
	 */
	mov	r0, r4
	mov	r3, #0
	add	r6, r0, #0x1000
1:	stw.w	r3, [r0]+, #4
	stw.w	r3, [r0]+, #4
	stw.w	r3, [r0]+, #4
	stw.w	r3, [r0]+, #4
	cmpxor.a	r0, r6
	bne	1b

	ldw	r7, [r10+], #PROCINFO_MM_MMUFLAGS @ mm_mmuflags

	/*
	 * Create identity mapping for first MB of kernel to
	 * cater for the MMU enable.  This identity mapping
	 * will be removed by paging_init().  We use our current program
	 * counter to determine corresponding section base address.
	 */
	mov	r6, pc
	mov	r6, r6 >> #22			@ start of kernel section
	or	r3, r7, r6 << #22		@ flags + kernel base
	stw	r3, [r4+], r6 << #2		@ identity mapping

	/*
	 * Now setup the pagetables for our kernel direct
	 * mapped region.
	 */
	add	r0, r4,  #(KERNEL_START & 0xff000000) >> 20
	stw.w	r3, [r0+], #(KERNEL_START & 0x00c00000) >> 20
	ldw	r6, =(KERNEL_END - 1)
	add	r0, r0, #4
	add	r6, r4, r6 >> #20
1:	cmpsub.a	r0, r6
	add	r3, r3, #1 << 22
	bua	2f
	stw.w	r3, [r0]+, #4
	b	1b
2:
	/*
	 * Then map first 4MB of ram in case it contains our boot params.
	 */
	add	r0, r4, #PAGE_OFFSET >> 20
	or	r6, r7, #(PHYS_OFFSET & 0xff000000)
	.if	(PHYS_OFFSET & 0x00c00000)
	or	r6, r6, #(PHYS_OFFSET & 0x00c00000)
	.endif
	stw	r6, [r0]

#ifdef CONFIG_DEBUG_LL
	ldw	r7, [r10+], #PROCINFO_IO_MMUFLAGS @ io_mmuflags
	/*
	 * Map in IO space for serial debugging.
	 * This allows debug messages to be output
	 * via a serial console before paging_init.
	 */
	ldw	r3, [r8+], #MACHINFO_PGOFFIO
	add	r0, r4, r3
	@rsb	r3, r3, #0x1000			@ PTRS_PER_PGD*sizeof(long)
	@cmp	r3, #0x0800			@ limit to 512MB
	@movhi	r3, #0x0800
	@add	r6, r0, r3
	ldw	r3, [r8+], #MACHINFO_PHYSIO
	or	r3, r3, r7
1:	stw.w	r3, [r0]+, #4
	@add	r3, r3, #1 << 22
	@teq	r0, r6
	@bne	1b
#endif
	mov	pc, lr
ENDPROC(__create_page_tables)
	.ltorg

#include "head-common.S"
