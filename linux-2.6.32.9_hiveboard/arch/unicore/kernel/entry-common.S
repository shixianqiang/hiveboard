/*
 * linux/arch/unicore/kernel/entry-common.S
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

#include <asm/unistd.h>
#include <asm/ftrace.h>
#include <mach/entry-macro.S>

#include "entry-header.S"


	.align	5
/*
 * This is the fast syscall return path.  We do as little as
 * possible here, and this includes saving r0 back into the SVC
 * stack.
 */
ret_fast_syscall:
	disable_irq r1				@ disable interrupts
	ldw	r1, [tsk+], #TI_FLAGS
	cmpand.a	r1, #_TIF_WORK_MASK
	bne	fast_work_pending

	/* perform architecture specific actions before user return */
	arch_ret_to_user r1, lr

	@ fast_restore_user_regs
	restore_user_regs fast = 1, offset = S_OFF

/*
 * Ok, we need to do extra processing, enter the slow path.
 */
fast_work_pending:
	stw.w	r0, [sp+], #S_R0+S_OFF		@ returned r0
work_pending:
	cmpand.a	r1, #_TIF_NEED_RESCHED
	bne	work_resched
	cmpand.a	r1, #_TIF_SIGPENDING|_TIF_NOTIFY_RESUME
	beq	no_work_pending
	mov	r0, sp				@ 'regs'
	mov	r2, why				@ 'syscall'
	b.l	do_notify_resume
	b	ret_slow_syscall		@ Check work again

work_resched:
	b.l	schedule
/*
 * "slow" syscall return path.  "why" tells us if this was a real syscall.
 */
ENTRY(ret_to_user)
ret_slow_syscall:
	disable_irq r1				@ disable interrupts
	get_thread_info tsk			@ epip4d, one path error?!
	ldw	r1, [tsk+], #TI_FLAGS
	cmpand.a	r1, #_TIF_WORK_MASK
	bne	work_pending
no_work_pending:
	/* perform architecture specific actions before user return */
	arch_ret_to_user r1, lr

	@ slow_restore_user_regs
	restore_user_regs fast = 0, offset = 0
ENDPROC(ret_to_user)

/*
 * This is how we return from a fork.
 */
ENTRY(ret_from_fork)
	b.l	schedule_tail
	get_thread_info tsk
	ldw	r1, [tsk+], #TI_FLAGS		@ check for syscall tracing
	mov	why, #1
	cmpand.a	r1, #_TIF_SYSCALL_TRACE	@ are we tracing syscalls?
	beq	ret_slow_syscall
	mov	r1, sp
	mov	r0, #1				@ trace exit [IP = 1]
	b.l	syscall_trace
	b	ret_slow_syscall
ENDPROC(ret_from_fork)

	.equ NR_syscalls,0
#define CALL(x) .equ NR_syscalls,NR_syscalls+1
#include "calls.S"
#undef CALL
#define CALL(x) .long x

#ifdef CONFIG_FUNCTION_TRACER
#ifdef CONFIG_DYNAMIC_FTRACE
ENTRY(mcount)
	stm.w (lr), [sp-]
	stm.w (r0-r3), [sp-]
	mov r0, lr
	sub r0, r0, #MCOUNT_INSN_SIZE

	.globl mcount_call
mcount_call:
	b.l ftrace_stub
	ldw lr, [fp+], #-4			@ restore lr
	ldm.w (r0-r3), [sp]+
	ldm.w (pc), [sp]+

ENTRY(ftrace_caller)
	stm.w (lr), [sp-]
	stm.w (r0-r3), [sp-]
	ldw r1, [fp+], #-4
	mov r0, lr
	sub r0, r0, #MCOUNT_INSN_SIZE

	.globl ftrace_call
ftrace_call:
	b.l ftrace_stub
	ldw lr, [fp+], #-4			@ restore lr
	ldm.w (r0-r3), [sp]+
	ldm.w (pc), [sp]+

#else

ENTRY(__gnu_mcount_nc)
	stm.w (lr), [sp-]
	stm.w (r0-r3), [sp-]
	ldw r0, =ftrace_trace_function
	ldw r2, [r0]
	adr r0, ftrace_stub
	cmpsub.a r0, r2
	bne gnu_trace
	ldm.w (r0-r3), [sp]+
	ldm.w (ip, lr), [sp]+
	mov pc, ip

gnu_trace:
	ldw r1, [sp+], #20			@ lr of instrumented routine
	mov r0, lr
	sub r0, r0, #MCOUNT_INSN_SIZE
	mov lr, pc
	mov pc, r2
	ldm.w (r0-r3), [sp]+
	ldm.w (ip, lr), [sp]+
	mov pc, ip

ENTRY(mcount)
	stm.w (lr), [sp-]
	stm.w (r0-r3), [sp-]
	ldw r0, =ftrace_trace_function
	ldw r2, [r0]
	adr r0, ftrace_stub
	cmpsub.a r0, r2
	bne trace
	ldw lr, [fp+], #-4			@ restore lr
	ldm.w (r0-r3), [sp]+
	ldm.w (pc), [sp]+

trace:
	ldw r1, [fp+], #-4			@ lr of instrumented routine
	mov r0, lr
	sub r0, r0, #MCOUNT_INSN_SIZE
	mov lr, pc
	mov pc, r2
	ldw lr, [fp+], #-4			@ restore lr
	ldm.w (r0-r3), [sp]+
	ldm.w (pc), [sp]+

#endif /* CONFIG_DYNAMIC_FTRACE */

	.globl ftrace_stub
ftrace_stub:
	mov pc, lr

#endif /* CONFIG_FUNCTION_TRACER */

/*=============================================================================
 * SWI handler
 *-----------------------------------------------------------------------------
 */
	.align	5
ENTRY(vector_swi)
	sub	sp, sp, #S_FRAME_SIZE
	stm	(r0 - r15), [sp]+		@ Calling r0 - r15
	add	r8, sp, #S_R16
	stm	(r16 - r28), [r8]+		@ Calling r16 - r28
	add	r8, sp, #S_PC
	stur	(sp, lr), [r8-]			@ Calling sp, lr
	mov	r8, bsr				@ called from non-FIQ mode, so ok.
	stw	lr, [sp+], #S_PC		@ Save calling PC
	stw	r8, [sp+], #S_PSR		@ Save CPSR
	stw	r0, [sp+], #S_OLD_R0		@ Save OLD_R0
	zero_fp
#ifdef	CONFIG_SYSCALL_TRACE
	mov	r8, r0
	mov	r7, lr
	b.l	vectors_entry_trace
	mov	lr, r7
	mov	r0, r8
#endif

	/*
	 * Get the system call number.
	 */
	sub	ip, lr, #4
	ldw.u	scno, [ip]			@ get SWI instruction
#ifdef	CONFIG_SYSCALL_TRACE
	printreg	scno, ip		@ print syscall number
#endif

#ifdef CONFIG_ALIGNMENT_TRAP
	ldw	ip, __cr_alignment
	ldw	ip, [ip]
	movc	p0.c1, ip, #0                   @ update control register
#endif
	enable_irq ip

	get_thread_info tsk
	adr	tbl, sys_call_table		@ load syscall table pointer
	ldw	ip, [tsk+], #TI_FLAGS		@ check for syscall tracing

	andn	scno, scno, #0xff000000		@ mask off SWI op-code
	xor	scno, scno, #__NR_SYSCALL_BASE	@ check OS number

	stm.w	(r4, r5), [sp-]			@ push fifth and sixth args
	cmpand.a	ip, #_TIF_SYSCALL_TRACE	@ are we tracing syscalls?
	bne	__sys_trace

	cmpsub.a	scno, #NR_syscalls	@ check upper syscall limit
	adr	lr, ret_fast_syscall		@ return address
	bea	1f
	ldw	pc, [tbl+], scno << #2		@ call sys_* routine
1:
	add	r1, sp, #S_OFF
2:	mov	why, #0				@ no longer a real syscall
	cmpsub.a	scno, #(__UNICORE_NR_BASE - __NR_SYSCALL_BASE)
	xor	r0, scno, #__NR_SYSCALL_BASE	@ put OS number back
	bea	unicore_syscall
	b	sys_ni_syscall			@ not private func

	/*
	 * This is the really slow path.  We're going to be doing
	 * context switches, and waiting for our parent to respond.
	 */
__sys_trace:
	mov	r2, scno
	add	r1, sp, #S_OFF
	mov	r0, #0				@ trace entry [IP = 0]
	b.l	syscall_trace

	adr	lr, __sys_trace_return		@ return address
	mov	scno, r0			@ syscall number (possibly new)
	add	r1, sp, #S_R0 + S_OFF		@ pointer to regs
	cmpsub.a	scno, #NR_syscalls	@ check upper syscall limit
	bea	2b
	ldm	(r0 - r3), [r1]+		@ have to reload r0 - r3
	ldw	pc, [tbl+], scno << #2		@ call sys_* routine

__sys_trace_return:
	stw.w	r0, [sp+], #S_R0 + S_OFF	@ save returned r0
	mov	r2, scno
	mov	r1, sp
	mov	r0, #1				@ trace exit [IP = 1]
	b.l	syscall_trace
	b	ret_slow_syscall

	.align	5
#ifdef CONFIG_ALIGNMENT_TRAP
	.type	__cr_alignment, #object
__cr_alignment:
	.word	cr_alignment
#endif
	.ltorg

/*
 * This is the syscall table declaration for native ABI syscalls.
 */
	.type	sys_call_table, #object
ENTRY(sys_call_table)
#include "calls.S"

/*============================================================================
 * Special system call wrappers
 */
@ r0 = syscall number
@ r8 = syscall table
sys_syscall:
		andn	scno, r0, #__NR_SYSCALL_BASE
		cmpsub.a	scno, #__NR_syscall - __NR_SYSCALL_BASE
		beq	1f
		cmpsub.a	scno, #NR_syscalls	@ check range
1:
		bea	2f
		stm	(r5, r6), [sp]+		@ shuffle args
		mov	r0, r1
		mov	r1, r2
		mov	r2, r3
		mov	r3, r4
		ldw	pc, [tbl+], scno << #2
2:		b	sys_ni_syscall
ENDPROC(sys_syscall)

sys_fork_wrapper:
		add	r0, sp, #S_OFF
		b	sys_fork
ENDPROC(sys_fork_wrapper)

sys_vfork_wrapper:
		add	r0, sp, #S_OFF
		b	sys_vfork
ENDPROC(sys_vfork_wrapper)

sys_execve_wrapper:
		add	r3, sp, #S_OFF
		b	sys_execve
ENDPROC(sys_execve_wrapper)

sys_clone_wrapper:
		add	ip, sp, #S_OFF
		stw	ip, [sp+], #4
		b	sys_clone
ENDPROC(sys_clone_wrapper)

sys_sigreturn_wrapper:
		add	r0, sp, #S_OFF
		b	sys_sigreturn
ENDPROC(sys_sigreturn_wrapper)

sys_rt_sigreturn_wrapper:
		add	r0, sp, #S_OFF
		b	sys_rt_sigreturn
ENDPROC(sys_rt_sigreturn_wrapper)

sys_sigaltstack_wrapper:
		ldw	r2, [sp+], #S_OFF + S_SP
		b	do_sigaltstack
ENDPROC(sys_sigaltstack_wrapper)

sys_statfs64_wrapper:
		cmpxor.a	r1, #88
		cmoveq	r1, #84
		b	sys_statfs64
ENDPROC(sys_statfs64_wrapper)

sys_fstatfs64_wrapper:
		cmpxor.a	r1, #88
		cmoveq	r1, #84
		b	sys_fstatfs64
ENDPROC(sys_fstatfs64_wrapper)

