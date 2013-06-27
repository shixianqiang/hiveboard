/*
 * linux/arch/unicore/kernel/head-common.S
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

#define ATAG_CORE 0x54410001
#define ATAG_CORE_SIZE ((2*4 + 3*4) >> 2)
#define ATAG_CORE_SIZE_EMPTY ((2*4) >> 2)

	.align	2
	.type	__switch_data, %object
__switch_data:
	.long	__mmap_switched
	.long	__data_loc			@ r4
	.long	_data				@ r5
	.long	__bss_start			@ r6
	.long	_end				@ r7
	.long	processor_id			@ r4
	.long	__machine_arch_type		@ r5
	.long	__atags_pointer			@ r6
	.long	cr_alignment			@ r7
	.long	init_thread_union + THREAD_START_SP @ sp
/*
 * The following fragment of code is executed with the MMU on in MMU mode,
 * and uses absolute addresses; this is not position independent.
 *
 *  r0  = cp#0 control register
 *  r1  = machine ID
 *  r2  = atags pointer
 *  r9  = processor ID
 */
__mmap_switched:
	adr	r3, __switch_data + 4

	ldm.w	(r4, r5, r6, r7), [r3]+
	cmpsub.a	r4, r5			@ Copy data segment if needed
	beq	2f
1:	cmpsub.a	r5, r6
	beq	2f
	ldw.w	fp, [r4]+, #4
	stw.w	fp, [r5]+, #4
	b	1b
2:
	mov	fp, #0				@ Clear BSS (and zero fp)
1:	cmpsub.a	r6, r7
	bea	2f
	stw.w	fp, [r6]+,#4
	b	1b
2:
	ldm	(r4, r5, r6, r7), [r3]+
	ldw	sp, [r3+], #16
	stw	r9, [r4]			@ Save processor ID
	stw	r1, [r5]			@ Save machine type
	stw	r2, [r6]			@ Save atags pointer
	andn	r4, r0, #CR_A			@ Clear 'A' bit
	stm	(r0, r4), [r7]+			@ Save control register values
	b	start_kernel
ENDPROC(__mmap_switched)


/*
 * Exception handling.  Something went wrong and we can't proceed.  We
 * ought to tell the user, but since we don't have any guarantee that
 * we're even running on the right architecture, we do virtually nothing.
 *
 * If CONFIG_DEBUG_LL is set we try to print out something about the error
 * and hope for the best (useful if bootloader fails to pass a proper
 * machine ID for example).
 */
__error_p:
#ifdef CONFIG_DEBUG_LL
	adr	r0, str_p1
	b.l	printascii
	mov	r0, r9
	b.l	printhex8
	adr	r0, str_p2
	b.l	printascii
	b	__error
str_p1:	.asciz	"\nError: unrecognized/unsupported processor variant (0x"
str_p2:	.asciz	").\n"
	.align
#endif
ENDPROC(__error_p)

__error_a:
#ifdef CONFIG_DEBUG_LL
	mov	r4, r1				@ preserve machine ID
	adr	r0, str_a1
	b.l	printascii
	mov	r0, r4
	b.l	printhex8
	adr	r0, str_a2
	b.l	printascii
	adr	r3, 4f
	ldm	(r4, r5, r6), [r3]+		@ get machine desc list
	sub	r4, r3, r4			@ get offset between virt&phys
	add	r5, r5, r4			@ convert virt addresses to
	add	r6, r6, r4			@ physical address space
1:	ldw	r0, [r5+], #MACHINFO_TYPE	@ get machine type
	b.l	printhex8
	mov	r0, #'\t'
	b.l	printch
	ldw     r0, [r5+], #MACHINFO_NAME	@ get machine name
	add	r0, r0, r4
	b.l	printascii
	mov	r0, #'\n'
	b.l	printch
	add	r5, r5, #SIZEOF_MACHINE_DESC	@ next machine_desc
	cmpsub.a	r5, r6
	bub	1b
	adr	r0, str_a3
	b.l	printascii
	b	__error
ENDPROC(__error_a)

str_a1:	.asciz	"\nError: unrecognized/unsupported machine ID (r1 = 0x"
str_a2:	.asciz	").\n\nAvailable machine support:\n\nID (hex)\tNAME\n"
str_a3:	.asciz	"\nPlease check your kernel config and/or bootloader.\n"
	.align
#endif

__error:
1:	mov	r0, r0
	b	1b
ENDPROC(__error)


/*
 * Read processor ID register (CP#0, CR0), and look up in the linker-built
 * supported processor list.  Note that we can't use the absolute addresses
 * for the __proc_info lists since we aren't running with the MMU on
 * (and therefore, we are not in the correct address space).  We have to
 * calculate the offset.
 *
 *	r9 = cpuid
 * Returns:
 *	r3, r4, r6 corrupted
 *	r5 = proc_info pointer in physical address space
 *	r9 = cpuid (preserved)
 */
__lookup_processor_type:
	adr	r3, 3f
	ldm	(r5 - r7), [r3]+
	add	r3, r3, #8
	sub	r3, r3, r7			@ get offset between virt&phys
	add	r5, r5, r3			@ convert virt addresses to
	add	r6, r6, r3			@ physical address space
1:	ldm	(r3, r4), [r5]+			@ value, mask
	and	r4, r4, r9			@ mask wanted bits
	cmpxor.a	r3, r4
	beq	2f
	add	r5, r5, #PROC_INFO_SZ		@ sizeof(proc_info_list)
	cmpsub.a	r5, r6
	bub	1b
	mov	r5, #0				@ unknown processor
2:	mov	pc, lr
ENDPROC(__lookup_processor_type)

/*
 * This provides a C-API version of the above function.
 */
ENTRY(lookup_processor_type)
	stm.w	(lr), [sp-]
	stm.w	(r4 - r7, r9), [sp-]
	mov	r9, r0
	b.l	__lookup_processor_type
	mov	r0, r5
	ldm.w	(r4 - r7, r9), [sp]+
	ldm.w	(pc), [sp]+
ENDPROC(lookup_processor_type)

/*
 * Look in <asm/procinfo.h> and arch/unicore/kernel/arch.[ch] for
 * more information about the __proc_info and __arch_info structures.
 */
	.align	2
3:	.long	__proc_info_begin
	.long	__proc_info_end
4:	.long	.
	.long	__arch_info_begin
	.long	__arch_info_end

/*
 * Lookup machine architecture in the linker-build list of architectures.
 * Note that we can't use the absolute addresses for the __arch_info
 * lists since we aren't running with the MMU on (and therefore, we are
 * not in the correct address space).  We have to calculate the offset.
 *
 *  r1 = machine architecture number
 * Returns:
 *  r3, r4, r6 corrupted
 *  r5 = mach_info pointer in physical address space
 */
__lookup_machine_type:
	adr	r3, 4b
	ldm	(r4, r5, r6), [r3]+
	sub	r3, r3, r4			@ get offset between virt&phys
	add	r5, r5, r3			@ convert virt addresses to
	add	r6, r6, r3			@ physical address space
1:	ldw	r3, [r5+], #MACHINFO_TYPE	@ get machine type
	cmpxor.a	r3, r1			@ matches loader number?
	beq	2f				@ found
	add	r5, r5, #SIZEOF_MACHINE_DESC	@ next machine_desc
	cmpsub.a	r5, r6
	bub	1b
	mov	r5, #0				@ unknown machine
2:	mov	pc, lr
ENDPROC(__lookup_machine_type)

/*
 * This provides a C-API version of the above function.
 */
ENTRY(lookup_machine_type)
	stm.w	(lr), [sp-]
	stm.w	(r4 - r6), [sp-]
	mov	r1, r0
	b.l	__lookup_machine_type
	mov	r0, r5
	ldm.w	(r4 - r6), [sp]+
	ldm.w	(pc), [sp]+
ENDPROC(lookup_machine_type)

/* Determine validity of the r2 atags pointer.  The heuristic requires
 * that the pointer be aligned, in the first 16k of physical RAM and
 * that the ATAG_CORE marker is first and present.  Future revisions
 * of this function may be more lenient with the physical address and
 * may also be able to move the ATAGS block if necessary.
 *
 * r8  = machinfo
 *
 * Returns:
 *  r2 either valid atags pointer, or zero
 *  r5, r6 corrupted
 */
__vet_atags:
	cmpand.a	r2, #0x3		@ aligned?
	bne	1f

	ldw	r5, [r2+], #0			@ is first tag ATAG_CORE?
	cmpsub.a	r5, #ATAG_CORE_SIZE
	beq	201f
	cmpsub.a	r5, #ATAG_CORE_SIZE_EMPTY
201:	bne	1f
	ldw	r5, [r2+], #4
	movl	r6, #ATAG_CORE			@ ldr	r6, =ATAG_CORE
	cmpsub.a	r5, r6
	bne	1f

	mov	pc, lr				@ atag pointer is ok

1:	mov	r2, #0
	mov	pc, lr
ENDPROC(__vet_atags)
