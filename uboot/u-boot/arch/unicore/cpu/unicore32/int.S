/*
 *  unicoreboot - Startup Code for UNICORE-II CPU-core
 *
 *  Copyright (c) 2009	Tony Hook <suyonggang@pkunity.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <version.h>

.globl _start
_start:
	bal	 reset
	bal	_extend_handle
	bal	_swi_handle
	bal	_iabort_handle
	bal	_dabort_handle
	bal	_reserve_handle
	bal	_IRQ_handle
	bal	_FIQ_handle


_extend_handle:		.word extend_handle
_swi_handle: 		.word swi_handle
_iabort_handle: 	.word iabort_handle
_dabort_handle:		.word dabort_handle
_reserve_handle:	.word reserve_handle
_IRQ_handle:		.word IRQ_handle
_FIQ_handle:		.word FIQ_handle
	.balignl 16,0xdeadbeef


/*
 *************************************************************************
 *
 * Startup Code (called from the UNICORE reset exception vector)
 *
 * do important init only if we don't start from memory!
 * relocate unicoreboot to ram
 * setup stack
 * jump to second stage
 *
 *************************************************************************
 */

/*
 * the actual start code
 */



reset:
/*The first thing is to remap ddr from 0x40000000 to 0x0
 *remember the code is move from the sd card to ddr
 */
@	ldw		r0, =0x30008000
@	ldw		r1, [r0]	
@	or		r1, r1, #0x80000000
@	stw		r1, [r0]


@disable the irq
    mov		r0, asr
	or		r0, r0, #0x80
	mov.a	asr, r0

	@irq stack
	mov 	r1, asr
	andn	r1, r1, #0x1f
	or		r1, r1, #0x12	@irq mode
	mov.a	asr, r1
	movl	sp, #0x3f0000   @403f0000
	sub		sp, sp, #0x4
	
	@svc stack
	mov 	r1, asr
	andn	r1, r1, #0x1f
	or		r1, r1, #0x13	@svc mode
	mov.a	asr, r1
	movl	sp,#0x310000
	@@ cmove stack top pointer to highest word of set58
@	movl	sp,#0x3f0000		@@ set58,in user program data segment
	movl 	r19, #0				@@ won't be dump out

@enalbe irq
    mov             r0, asr
    andn    r0, r0, #0x80
    mov.a   asr, r0


	nop
	nop
	nop
	ldw	r0, _TEXT_BASE			@ upper 128 KiB: relocated uboot
	sub	r0, r0, #CONFIG_SYS_MALLOC_LEN		@ malloc area
	sub	r0, r0, #CONFIG_SYS_GBL_DATA_SIZE	@ bdinfo
#ifdef CONFIG_USE_IRQ
	sub	r0, r0, #(CONFIG_STACKSIZE_IRQ+CONFIG_STACKSIZE_FIQ)
#endif
	sub	sp, r0, #12			@ leave 3 words for abort-stack

clear_bss:
	ldw	r0, _bss_start			@ find start of bss segment
	ldw	r1, _bss_end			@ stop here
	mov	r2, #0x00000000			@ clear

clbss_l:stw	r2, [r0]			@ clear loop...
	add	r0, r0, #4
	cmpsub.a	r0, r1
	bel	clbss_l

	ldw	pc, _start_unicoreboot

_start_unicoreboot:
	.word start_unicoreboot

	nop
	nop
	nop
	nop
	nop
	nop



/*
 * exception handlers
 */
extend_handle:
	mov.a	pc,lr
swi_handle:
	mov.a	pc,lr
iabort_handle:
	mov.a	pc,lr
dabort_handle:
	mov.a	pc,lr
reserve_handle:
	mov.a	pc,lr
IRQ_handle:

    stm.w (r0 - r10), [sp]-   
    stm.w (lr), [sp]-    

    ldw		r3, =0xb0000080
	ldw		r4, [r3]
	ldw		lr, =int_return
	ldw		pc, [r4]
int_return:
	nop
	nop
	nop

    ldm.w (lr), [sp+]
    ldm.w (r0 - r10), [sp+]
    mov.a pc, lr


FIQ_handle:
	mov.a	pc,lr


