/*
 * linux/arch/unicore/kernel/entry-header.S
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
#include <asm/errno.h>
#include <asm/thread_info.h>

@ Bad Abort numbers
@ -----------------
@
#define BAD_PREFETCH	0
#define BAD_DATA	1
#define BAD_ADDREXCPTN	2
#define BAD_IRQ		3
#define BAD_UNDEFINSTR	4

@
@ Most of the stack format comes from struct pt_regs, but with
@ the addition of 8 bytes for storing syscall args 5 and 6.
@
#define S_OFF		8

/* 
 * The SWI code relies on the fact that R0 is at the bottom of the stack
 * (due to slow/fast restore user regs).
 */
#if S_R0 != 0
#error "Please fix"
#endif

	.macro	zero_fp
#ifdef CONFIG_FRAME_POINTER
	mov	fp, #0
#endif
	.endm

	.macro	alignment_trap, rtemp
#ifdef CONFIG_ALIGNMENT_TRAP
	ldw	\rtemp, .LCcralign
	ldw	\rtemp, [\rtemp]
	movc	p0.c1, \rtemp, #0
#endif
	.endm

	.macro	load_user_sp_lr, rd, rtemp, offset = 0
	mov	\rtemp, asr
	xor	\rtemp, \rtemp, #(PRIV_MODE ^ SUSR_MODE)
	mov.a	asr, \rtemp			@ switch to the SUSR mode

	ldw	sp, [\rd+], #\offset		@ load sp_usr
	ldw	lr, [\rd+], #\offset + 4	@ load lr_usr

	xor	\rtemp, \rtemp, #(PRIV_MODE ^ SUSR_MODE)
	mov.a	asr, \rtemp			@ switch back to the PRIV mode
	.endm

	.macro	svc_exit, rpsr
	mov.a	bsr, \rpsr
	ldm.w	(r0 - r15), [sp]+
	ldm.b	(r16 - pc), [sp]+		@ load r0 - pc, cpsr
	.endm

	.macro	restore_user_regs, fast = 0, offset = 0
	ldw	r1, [sp+], #\offset + S_PSR	@ get calling cpsr
	ldw	lr, [sp+], #\offset + S_PC	@ get pc
	mov.a	bsr, r1			@ save in spsr_svc
	.if	\fast
	add	sp, sp, #\offset + S_R1		@ r0 contains the syscall return vaule
	ldm.w	(r1 - r15), [sp]+		@ get calling r1 - r15
	ldur	(r16 - lr), [sp]+		@ get calling r16 - lr
	.else
	ldm.w	(r0 - r15), [sp]+		@ get calling r0 - r15
	ldur	(r16 - lr), [sp]+		@ get calling r16 - lr
	.endif
	nop
	add	sp, sp, #S_FRAME_SIZE - S_R16
	mov.a	pc, lr				@ return & move spsr_svc into cpsr
	.endm

	.macro	get_thread_info, rd
	mov	\rd, sp >> #13
	mov	\rd, \rd << #13
	.endm

#ifdef CONFIG_DEBUG_LL
	.macro	printreg, reg, temp
		adr	\temp, 901f
		stm	(r0-r3), [\temp]+
		stw	lr, [\temp+], #0x10
		mov	r0, \reg
		b.l	printhex8
		mov	r0, #':'
		b.l	printch
		mov	r0, pc
		b.l	printhex8
		adr	r0, 902f
		b.l	printascii
		adr	\temp, 901f
		ldm	(r0-r3), [\temp]+
		ldw	lr, [\temp+], #0x10
		b	903f
901:	.word	0, 0, 0, 0, 0	@ r0-r3, lr
902:	.asciz	": epip4d\n"
	.align
903:
	.endm
#endif

/*
 * These are the registers used in the syscall handler, and allow us to
 * have in theory up to 7 arguments to a function - r0 to r6.
 *
 * Note that tbl == why is intentional.
 *
 * We must set at least "tsk" and "why" when calling ret_with_reschedule.
 */
scno	.req	r21		@ syscall number
tbl	.req	r22		@ syscall table pointer
why	.req	r22		@ Linux syscall (!= 0)
tsk	.req	r23		@ current thread_info
