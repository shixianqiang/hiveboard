/*
 * linux/arch/unicore/boot/compressed/head.S
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



		.section ".start", #alloc, #execinstr
		.align
start:
		.type	start,#function
		.rept	8
		mov	r0, r0
		.endr

		mov	r1, r1			@ MACH_TYPE_ARCH_PUV3 epip4d
		mov	r2, r2			@ r1 & r2 should come from bootloader

		mov	r7, r1			@ save architecture ID
		mov	r8, r2			@ save atags pointer
        /*
         * EMI
         */
 /*       movl r3, #0x32020074
  *      movl r4, #0xa0000000
  *      stw r4, [r3]
 */
		mov	r2, #0xD3		@ turn off interrupts
		mov.a	asr, r2

		/*
		 * Note that some cache flushing and other stuff may
		 * be needed here
		 */

		/*
		 * some architecture specific code can be inserted
		 * by the linker here, but it should preserve r7, r8, and r9.
		 */

		.text
		adr	r0, LC0
		ldm	(r1, r2, r3, r4, r5, r6), [r0]+
		ldw	ip, [r0+], #24
		ldw	sp, [r0+], #28
		sub.a	r0, r0, r1		@ calculate the delta offset

						@ if delta is zero, we are
		beq	not_relocated		@ running at the address we
						@ were linked at.

		/*
		 * We're running at a different address.  We need to fix
		 * up various pointers:
		 *   r5 - zImage base address
		 *   r6 - GOT start
		 *   ip - GOT end
		 */
		add	r5, r5, r0
		add	r6, r6, r0
		add	ip, ip, r0

		/*
		 * we need to fix up pointers into the BSS region.
		 *   r2 - BSS start
		 *   r3 - BSS end
		 *   sp - stack pointer
		 */
		add	r2, r2, r0
		add	r3, r3, r0
		add	sp, sp, r0

		/*
		 * Relocate all entries in the GOT table.
		 */
1:		ldw	r1, [r6+], #0		@ relocate entries in the GOT
		add	r1, r1, r0		@ table.  This fixes up the
		stw.w	r1, [r6]+, #4		@ C references.
		cmpsub.a	r6, ip
		bub	1b

not_relocated:	mov	r0, #0
1:		stw.w	r0, [r2]+, #4		@ clear bss
		stw.w	r0, [r2]+, #4
		stw.w	r0, [r2]+, #4
		stw.w	r0, [r2]+, #4
		cmpsub.a	r2, r3
		bub	1b

		/*
		 * The C runtime environment should now be setup
		 * sufficiently.  Turn the cache on, set up some
		 * pointers, and start decompressing.
		 */
		b.l	cache_on

		mov	r1, sp			@ malloc space above stack
		add	r2, sp, #0x10000	@ 64k max

/*
 * Check to see if we will overwrite ourselves.
 *   r4 = final kernel address
 *   r5 = start of this image
 *   r2 = end of malloc space (and therefore this image)
 * We basically want:
 *   r4 >= r2 -> OK
 *   r4 + image length <= r5 -> OK
 */
		cmpsub.a	r4, r2
		bea	wont_overwrite
		sub	r3, sp, r5		@ > compressed kernel size
		add	r0, r4, r3 << #1	@ allow for 4x expansion
		cmpsub.a	r0, r5
		beb	wont_overwrite

		mov	r5, r2			@ decompress after malloc space
		mov	r0, r5
		mov	r3, r7
		b.l	decompress_kernel

		add	r0, r0, #127 + 128	@ alignment + stack
		andn	r0, r0, #127		@ align the kernel length
/*
 * r0     = decompressed kernel length
 * r1-r3  = unused
 * r4     = kernel execution address
 * r5     = decompressed kernel start
 * r6     = processor ID
 * r7     = architecture ID
 * r8     = atags pointer
 * r9-r14 = corrupted
 */
		add	r1, r5, r0		@ end of decompressed kernel
		adr	r2, reloc_start
		ldw	r3, LC1
		add	r3, r2, r3
1:		ldm.w	(r9 - r14), [r2]+	@ copy relocation code
		stm.w	(r9 - r14), [r1]+
		ldm.w	(r9 - r14), [r2]+
		stm.w	(r9 - r14), [r1]+
		cmpsub.a	r2, r3
		bub	1b
		add	sp, r1, #128		@ relocate the stack

		b.l	cache_clean_flush
		add	pc, r5, r0		@ call relocation code

/*
 * We're not in danger of overwriting ourselves.  Do this the simple way.
 *
 * r4     = kernel execution address
 * r7     = architecture ID
 */
wont_overwrite:	adr	r0, LC0
		stm	(r4, r7, r8), [r0]+
		mov	r0, r4
		mov	r3, r7
		b.l	decompress_kernel
		adr	r0, LC0
		ldm	(r4, r7, r8), [r0]+
		b	call_kernel

		.align	2
		.type	LC0, #object
LC0:		.word	LC0			@ r1
		.word	__bss_start		@ r2
		.word	_end			@ r3
		.word	zreladdr		@ r4
		.word	_start			@ r5
		.word	_got_start		@ r6
		.word	_got_end		@ ip
		.word	user_stack+4096		@ sp
LC1:		.word	reloc_end - reloc_start
		.size	LC0, . - LC0

/*
 * Turn on the cache.  We need to setup some page tables so that we
 * can have both the I and D caches on.
 *
 * We place the page tables 16k down from the kernel execution address,
 * and we hope that nothing else is using it.  If we're using it, we
 * will go pop!
 *
 * On entry,
 *  r4 = kernel execution address
 *  r6 = processor ID
 *  r7 = architecture number
 *  r8 = atags pointer
 *  r9 = run-time address of "start"  (???)
 * On exit,
 *  r1, r2, r3, r9, r10, r12 corrupted
 * This routine must preserve:
 *  r4, r5, r6, r7, r8
 */
		.align	5
cache_on:	mov	r3, #8			@ cache_on function
		b	call_cache_fn

__ucv2_mmu_cache_on:
                mov     r0, #0
                movc    p0.c5, r0, #28				@ cache invalidate all
                nop; nop; nop; nop; nop; nop; nop; nop
                movc    p0.c6, r0, #6			@ tlb invalidate all
                nop; nop; nop; nop; nop; nop; nop; nop

                mov     r0, #0x1c			@ enable icache and writeback dcache
                movc    p0.c1, r0, #0
                nop; nop; nop; nop; nop; nop; nop; nop

		mov	pc, lr

/*
 * All code following this line is relocatable.  It is relocated by
 * the above code to the end of the decompressed kernel image and
 * executed there.  During this time, we have no stacks.
 *
 * r0     = decompressed kernel length
 * r1-r3  = unused
 * r4     = kernel execution address
 * r5     = decompressed kernel start
 * r6     = processor ID
 * r7     = architecture ID
 * r8     = atags pointer
 * r9-r14 = corrupted
 */
		.align	5
reloc_start:	add	r9, r5, r0
		sub	r9, r9, #128		@ do not copy the stack
		mov	r1, r4
1:
		.rept	4
		ldm.w	(r0, r2, r3, r10 - r14), [r5]+	@ relocate kernel
		stm.w	(r0, r2, r3, r10 - r14), [r1]+
		.endr

		cmpsub.a	r5, r9
		bub	1b
		add	sp, r1, #128		@ relocate the stack

call_kernel:	b.l	cache_clean_flush
		b.l	cache_off
		mov	r0, #0			@ must be zero
		mov	r1, r7			@ restore architecture number
		mov	r2, r8			@ restore atags pointer
		mov	pc, r4			@ call kernel

/*
 * Here follow the relocatable cache support functions for the
 * various processors.  This is a generic hook for locating an
 * entry and jumping to an instruction at the specified offset
 * from the start of the block.  Please note this is all position
 * independent code.
 *
 *  r1  = corrupted
 *  r2  = corrupted
 *  r3  = block offset
 *  r6  = corrupted
 *  r12 = corrupted
 */

call_cache_fn:	adr	r12, proc_types

		movc	r6, p0.c0, #0		@ get processor ID

1:		ldw	r1, [r12+], #0		@ get value
		ldw	r2, [r12+], #4		@ get mask
		xor	r1, r1, r6		@ (real ^ match)
		cmpand.a	r1, r2		@       & mask
		bne	2f
		add	pc, r12, r3		@ call cache function
2:
		add	r12, r12, #4*5
		b	1b

/*
 * Table for cache operations.  This is basically:
 *   - CPU ID match
 *   - CPU ID mask
 *   - 'cache on' method instruction
 *   - 'cache off' method instruction
 *   - 'cache flush' method instruction
 *
 * We match an entry using: ((real_id ^ match) & mask) == 0
 *
 * Writethrough caches generally only need 'on' and 'off'
 * methods.  Writeback caches _must_ have the flush method
 * defined.
 */
		.align	2
		.type	proc_types,#object
proc_types:
		.word	0x4d000863		@ PKUnity
		.word	0xff00ffff
		b	__ucv2_mmu_cache_on
		b	__ucv2_mmu_cache_off
		b	__ucv2_mmu_cache_flush

		.word	0			@ unrecognised type
		.word	0
		mov	pc, lr
		mov	pc, lr
		mov	pc, lr

		.size	proc_types, . - proc_types

/*
 * Turn off the Cache and MMU.
 *
 * On entry,  r6 = processor ID
 * On exit,   r0, r1, r2, r3, r12 corrupted
 * This routine must preserve: r4, r6, r7
 */
		.align	5
cache_off:	mov	r3, #12			@ cache_off function
		b	call_cache_fn

__ucv2_mmu_cache_off:
		mov	r0, #0				@ disable icache, dcache and MMU
		movc	p0.c1, r0, #0
                nop; nop; nop; nop
	
                mov     r0, #0
                movc    p0.c5, r0, #28			@ cache invalidate all
                nop; nop; nop; nop
                movc    p0.c6, r0, #6			@ tlb invalidate all
                nop; nop; nop; nop

		mov	pc, lr


/*
 * Clean and flush the cache to maintain consistency.
 *
 * On entry,
 *  r6 = processor ID
 * On exit,
 *  r1, r2, r3, r11, r12 corrupted
 * This routine must preserve:
 *  r0, r4, r5, r6, r7
 */
		.align	5
cache_clean_flush:
		mov	r3, #16
		b	call_cache_fn

__ucv2_mmu_cache_flush:
		mov	r0, #0
                movc    p0.c5, r0, #14			@ flush dcache
		nop; nop; nop; nop
                movc    p0.c5, r0, #20			@ icache invalidate all
                nop; nop; nop; nop

		mov	pc, lr
		
		.ltorg
reloc_end:

		.align
		.section ".stack", "w"
user_stack:	.space	4096
