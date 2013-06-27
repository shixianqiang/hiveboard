/*
 * linux/arch/unicore/kernel/entry-unicore.S
 *
 * Code specific to PKUnity SoC and UniCore ISA
 * Fragments that appear the same as the files in arm or x86
 *
 * Copyright (C) 2001-2008 GUAN Xue-tao
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Low-level vector interface routines
 */

#include <asm/memory.h>
#include <asm/glue.h>
#include <asm/thread_notify.h>
#include <mach/entry-macro.S>
#include <mach/debug-macro.S>
#include <asm/unistd.h>

#include "entry-header.S"

/*
 * Interrupt handling.  Preserves r17, r18, r19
 */
	.macro	irq_handler
	get_irqnr_preamble r5, lr
1:	get_irqnr_and_base r0, r6, r5, lr
	beq	2f
	mov	r1, sp
	@
	@ routine called with r0 = irq number, r1 = struct pt_regs *
	@
	adr	lr, 1b
	b	asm_do_IRQ
2:
	.endm

#ifdef CONFIG_KPROBES
	.section	.kprobes.text,"ax",%progbits
#else
	.text
#endif

/*
 * Invalid mode handlers
 */
	.macro	inv_entry, reason
	sub	sp, sp, #S_FRAME_SIZE
	stm	(r1 - r15), [sp+]
	add	r1, sp, #S_R16
	stm	(r16 - r28, sp, lr), [r1]+
	mov	r1, #\reason
	.endm

__pabt_invalid:
	inv_entry BAD_PREFETCH
	b	common_invalid
ENDPROC(__pabt_invalid)

__dabt_invalid:
	inv_entry BAD_DATA
	b	common_invalid
ENDPROC(__dabt_invalid)

__irq_invalid:
	inv_entry BAD_IRQ
	b	common_invalid
ENDPROC(__irq_invalid)

__und_invalid:
	inv_entry BAD_UNDEFINSTR

	@
	@ XXX fall through to common_invalid
	@

@
@ common_invalid - generic code for failed exception (re-entrant version of handlers)
@
common_invalid:
	zero_fp

	ldm	(r4 - r6), [r0]+
	add	r0, sp, #S_PC		@ here for interlock avoidance
	mov	r7, #-1			@  ""   ""    ""        ""
	stw	r4, [sp]		@ save preserved r0
	stm	(r5 - r7), [r0]+	@ lr_<exception>,
					@ cpsr_<exception>, "old_r0"

	mov	r0, sp
	b	bad_mode
ENDPROC(__und_invalid)

/*
 * SVC mode handlers
 */
	.macro	svc_entry, stack_hole=0
	sub	sp, sp, #(S_FRAME_SIZE + \stack_hole - 4)
	stm	(r1 - r15), [sp]+
	add	r5, sp, #S_R15
	stm	(r16 - r28), [r5]+

	ldm	(r1 - r3), [r0]+
	add	r5, sp, #S_SP - 4	@ here for interlock avoidance
	mov	r4, #-1			@  ""  ""      ""       ""
	add	r0, sp, #(S_FRAME_SIZE + \stack_hole - 4)
	stw.w	r1, [sp+], #-4		@ save the "real" r0 copied
					@ from the exception stack

	mov	r1, lr

	@
	@ We are now ready to fill in the remaining blanks on the stack:
	@
	@  r0 - sp_svc
	@  r1 - lr_svc
	@  r2 - lr_<exception>, already fixed up for correct return/restart
	@  r3 - spsr_<exception>
	@  r4 - orig_r0 (see pt_regs definition in ptrace.h)
	@
	stm	(r0 - r4), [r5]+

	asm_trace_hardirqs_off
	.endm

	.align	5
__dabt_svc:
	svc_entry

	dabt_breakpoint	r23, r24, 299f

	@
	@ get ready to re-enable interrupts if appropriate
	@
	mov	r17, asr
	cmpand.a	r3, #PSR_I_BIT
	bne	1f
	andn	r17, r17, #PSR_I_BIT
1:

	@
	@ Call the processor-specific abort handler:
	@
	@  r2 - aborted context pc
	@  r3 - aborted context cpsr
	@
	@ The abort handler must return the aborted address in r0, and
	@ the fault status register in r1.  r9 must be preserved.
	@
	b.l	CPU_DABORT_HANDLER

	@
	@ set desired IRQ state, then call main handler
	@
	mov.a	asr, r17
	mov	r2, sp
	b.l	do_DataAbort

	@
	@ IRQs off again before pulling preserved data off the stack
	@
	disable_irq r0
299:
	@
	@ restore SPSR and restart the instruction
	@
	ldw	r2, [sp+], #S_PSR
	svc_exit r2				@ return from exception
ENDPROC(__dabt_svc)

	.align	5
__irq_svc:
	svc_entry
@	nop
@	nop
@	ldw		r2, =0x31000000
@l111:
@	ldw		r1, =65
@	stw		r1, [r2]
@	bal		l111
	irq_handler
	
	ldw	r4, [sp+], #S_PSR		@ irqs are already disabled
#ifdef CONFIG_TRACE_IRQFLAGS
	cmpand.a	r4, #PSR_I_BIT
	belq	trace_hardirqs_on
#endif
	svc_exit r4				@ return from exception
ENDPROC(__irq_svc)

	.ltorg

	.align	5
__und_svc:
#ifdef CONFIG_KPROBES
	@ If a kprobe is about to simulate a "stm sp..." instruction,
	@ it obviously needs free stack space which then will belong to
	@ the saved context.
	svc_entry 64
#else
	svc_entry
#endif

	mov	r0, sp				@ struct pt_regs *regs
	b.l	do_undefinstr

	@
	@ IRQs off again before pulling preserved data off the stack
	@
1:	disable_irq r0

	@
	@ restore SPSR and restart the instruction
	@
	ldw	r2, [sp+], #S_PSR		@ Get SVC cpsr
	svc_exit r2				@ return from exception
ENDPROC(__und_svc)

	.align	5
__pabt_svc:
	svc_entry

	pabt_breakpoint	r23, r24, 299f

	@
	@ re-enable interrupts if appropriate
	@
	mov	r17, asr
	cmpand.a	r3, #PSR_I_BIT
	bne	1f
	andn	r17, r17, #PSR_I_BIT
1:

	@
	@ set args, then call main handler
	@
	@  r0 - address of faulting instruction
	@  r1 - pointer to registers on stack
	@
	mov	r0, r2				@ pass address of aborted instruction.
	b.l	CPU_PABORT_HANDLER
	mov.a	asr, r17
	mov	r2, sp				@ regs
	b.l	do_PrefetchAbort		@ call abort handler

	@
	@ IRQs off again before pulling preserved data off the stack
	@
	disable_irq r0
299:
	@
	@ restore SPSR and restart the instruction
	@
	ldw	r2, [sp+], #S_PSR
	svc_exit r2				@ return from exception
ENDPROC(__pabt_svc)

	.align	5
.LCcralign:
	.word	cr_alignment

/*
 * User mode handlers
 *
 */
	.macro	usr_entry
	sub	sp, sp, #S_FRAME_SIZE
	stm	(r1 - r15), [sp+]
	add	r4, sp, #S_R16
	stm	(r16 - r28), [r4]+

	ldm	(r1 - r3), [r0]+
	add	r0, sp, #S_PC		@ here for interlock avoidance
	mov	r4, #-1			@  ""  ""     ""        ""

	stw	r1, [sp]		@ save the "real" r0 copied
					@ from the exception stack

	@
	@ We are now ready to fill in the remaining blanks on the stack:
	@
	@  r2 - lr_<exception>, already fixed up for correct return/restart
	@  r3 - spsr_<exception>
	@  r4 - orig_r0 (see pt_regs definition in ptrace.h)
	@
	@ Also, separately save sp_usr and lr_usr
	@
	stm	(r2 - r4), [r0]+
	stur	(sp, lr), [r0-]

	@
	@ Enable the alignment trap while in kernel mode
	@
	alignment_trap r0

	@
	@ Clear FP to mark the first stack frame
	@
	zero_fp

	asm_trace_hardirqs_off
	.endm

	.align	5
__dabt_usr:
	usr_entry

	dabt_breakpoint	r23, r24, ret_from_exception

#ifdef CONFIG_UNICORE_FPU_F64
	cff	ip, s31  
	cmpand.a	ip, #0x08000000		@ is there a execption trapped in FPU
	beq	209f
	
	ldw	ip, [sp+], #S_PC
	add	ip, ip, #4
	stw	ip, [sp+], #S_PC
	@
	@ fall through to the emulation code, which returns using r19 if
	@ it has emulated the instruction, or the more conventional lr
	@ if we are to treat this as a real undefined instruction
	@
	@  r0 - instruction
	@
1:	ldw.u	r0, [r2]
	adr	r19, ret_from_exception
	adr	lr, 209f
	@
	@ fallthrough to call_fpe
	@
/*
 * Check whether the instruction is a co-processor instruction.
 * If yes, we need to call the relevant co-processor handler.
 *
 * Note that we don't do a full check here for the co-processor
 * instructions; all instructions with bit 27 set are well
 * defined.  The only instructions that should fault are the
 * co-processor instructions.
 *
 * Emulators may wish to make use of the following registers:
 *  r0  = instruction opcode.
 *  r2  = PC
 *  r19 = normal "successful" return address
 *  r20 = this threads thread_info structure.
 *  lr  = unrecognised instruction return address
 */
	get_thread_info r20			@ get current thread
	and	r8, r0, #0x00003c00		@ mask out CP number
	mov	r7, #1
	stb	r7, [r20+], #TI_USED_CP + 2	@ set appropriate used_cp[]

@ commented by zhangyang 2010-06-26
@	b	do_vfp
209:
#endif
	@
	@ Call the processor-specific abort handler:
	@
	@  r2 - aborted context pc
	@  r3 - aborted context cpsr
	@
	@ The abort handler must return the aborted address in r0, and
	@ the fault status register in r1.
	@
	b.l	CPU_DABORT_HANDLER

	@
	@ IRQs on, then call the main handler
	@
	enable_irq r2
	mov	r2, sp
	adr	lr, ret_from_exception
	b	do_DataAbort
ENDPROC(__dabt_usr)

	.align	5
__irq_usr:
	usr_entry
	get_thread_info tsk

	irq_handler
#ifdef CONFIG_TRACE_IRQFLAGS
	b.l	trace_hardirqs_on
#endif

	mov	why, #0
	b	ret_to_user
ENDPROC(__irq_usr)

	.ltorg

	.align	5
__und_usr:
	usr_entry

	mov	r0, sp
	adr	lr, ret_from_exception
	b	do_undefinstr
ENDPROC(__und_usr)

	.align	5
__pabt_usr:
	usr_entry

	pabt_breakpoint	r23, r24, ret_from_exception

	mov	r0, r2				@ pass address of aborted instruction.
	b.l	CPU_PABORT_HANDLER
	enable_irq r1				@ Enable interrupts
	mov	r2, sp				@ regs
	b.l	do_PrefetchAbort		@ call abort handler
	/* fall through */
/*
 * This is the return code to user mode for abort handlers
 */
ENTRY(ret_from_exception)
	get_thread_info tsk
	mov	why, #0
	b	ret_to_user
ENDPROC(__pabt_usr)
ENDPROC(ret_from_exception)

/*
 * Register switch for UniCore V2 processors
 * r0 = previous task_struct, r1 = previous thread_info, r2 = next thread_info
 * previous and next are guaranteed not to be the same.
 */
ENTRY(__switch_to)
	add	ip, r1, #TI_CPU_SAVE
	stm.w	(r4 - r15), [ip]+
	stm.w	(r16 - r27, sp, lr), [ip]+

#ifdef	CONFIG_UNICORE_FPU_F64
	add	ip, r1, #TI_FPSTATE
	sfm.w	(f0  - f7 ), [ip]+
	sfm.w	(f8  - f15), [ip]+
	sfm.w	(f16 - f23), [ip]+
	sfm.w	(f24 - f31), [ip]+
	cff	r4, s31
	stw	r4, [ip]

	add	ip, r2, #TI_FPSTATE
	lfm.w	(f0  - f7 ), [ip]+
	lfm.w	(f8  - f15), [ip]+
	lfm.w	(f16 - f23), [ip]+
	lfm.w	(f24 - f31), [ip]+
	ldw	r4, [ip]
	ctf	r4, s31
#endif
	mov	r25, r0
	add	r24, r2, #TI_CPU_SAVE
	ldw	r0, =thread_notify_head
	mov	r1, #THREAD_NOTIFY_SWITCH
	b.l	atomic_notifier_call_chain
	mov	r0, r25
	mov	ip, r24
	ldm.w	(r4 - r15), [ip]+
	ldm	(r16 - r27, sp, pc), [ip]+	@ Load all regs saved previously
ENDPROC(__switch_to)

	__INIT

/*
 * Vector stubs.
 *
 * This code is copied to 0xffff0200 so we can use branches in the
 * vectors, rather than ldr's.  Note that this code must not
 * exceed 0x300 bytes.
 *
 * Common stub entry macro:
 *   Enter in IRQ mode, spsr = SVC/USR CPSR, lr = SVC/USR PC
 *
 * SP points to a minimal amount of processor-private memory, the address
 * of which is copied into r0 for the mode specific abort handler.
 */
	.macro	vector_stub, name, mode
	.align	5

vector_\name:
	@
	@ Save r0, lr_<exception> (parent PC) and spsr_<exception>
	@ (parent CPSR)
	@
	stw	r0, [sp]
	stw	lr, [sp+], #4		@ save r0, lr
#ifdef	CONFIG_VECTORS_TRACE
	adr	lr, 201f
	ldw	r0, [lr]
	add	lr, pc, #8
	mov	pc, r0
201:	.word	vectors_trace_entry
#endif
	mov	lr, bsr
	stw	lr, [sp+], #8		@ save spsr

	@
	@ Prepare for SVC32 mode.  IRQs remain disabled.
	@
	mov	r0, asr
	xor	r0, r0, #(\mode ^ PRIV_MODE)
	mov.a	bsr, r0

	@
	@ the branch table must immediately follow this code
	@
	and	lr, lr, #0x0f
	add	lr, lr, #1
	mov	r0, sp
	ldw	lr, [pc+], lr << #2
	mov.a	pc, lr			@ branch to handler in SVC mode
ENDPROC(vector_\name)
	.align	2
	@ handler addresses follow this label
	.endm

	.globl	__stubs_start
__stubs_start:
/*
 * Interrupt dispatcher
 */
	vector_stub	irq, INTR_MODE

	.long	__irq_usr			@  0  (USR_32)
	.long	__irq_invalid			@  1  (FIQ_32)
	.long	__irq_invalid			@  2  (IRQ_32)
	.long	__irq_svc			@  3  (SVC_32)
	.long	__irq_invalid			@  4
	.long	__irq_invalid			@  5
	.long	__irq_invalid			@  6
	.long	__irq_invalid			@  7
	.long	__irq_invalid			@  8
	.long	__irq_invalid			@  9
	.long	__irq_invalid			@  a
	.long	__irq_invalid			@  b
	.long	__irq_invalid			@  c
	.long	__irq_invalid			@  d
	.long	__irq_invalid			@  e
	.long	__irq_invalid			@  f

/*
 * Data abort dispatcher
 * Enter in ABT mode, spsr = USR CPSR, lr = USR PC
 */
	vector_stub	dabt, ABRT_MODE

	.long	__dabt_usr			@  0  (USR_32)
	.long	__dabt_invalid			@  1  (FIQ_32)
	.long	__dabt_invalid			@  2  (IRQ_32)
	.long	__dabt_svc			@  3  (SVC_32)
	.long	__dabt_invalid			@  4
	.long	__dabt_invalid			@  5
	.long	__dabt_invalid			@  6
	.long	__dabt_invalid			@  7
	.long	__dabt_invalid			@  8
	.long	__dabt_invalid			@  9
	.long	__dabt_invalid			@  a
	.long	__dabt_invalid			@  b
	.long	__dabt_invalid			@  c
	.long	__dabt_invalid			@  d
	.long	__dabt_invalid			@  e
	.long	__dabt_invalid			@  f

/*
 * Prefetch abort dispatcher
 * Enter in ABT mode, spsr = USR CPSR, lr = USR PC
 */
	vector_stub	pabt, ABRT_MODE

	.long	__pabt_usr			@  0 (USR_32)
	.long	__pabt_invalid			@  1 (FIQ_32)
	.long	__pabt_invalid			@  2 (IRQ_32)
	.long	__pabt_svc			@  3 (SVC_32)
	.long	__pabt_invalid			@  4
	.long	__pabt_invalid			@  5
	.long	__pabt_invalid			@  6
	.long	__pabt_invalid			@  7
	.long	__pabt_invalid			@  8
	.long	__pabt_invalid			@  9
	.long	__pabt_invalid			@  a
	.long	__pabt_invalid			@  b
	.long	__pabt_invalid			@  c
	.long	__pabt_invalid			@  d
	.long	__pabt_invalid			@  e
	.long	__pabt_invalid			@  f

/*
 * Undef instr entry dispatcher
 * Enter in UND mode, spsr = SVC/USR CPSR, lr = SVC/USR PC
 */
	vector_stub	und, EXTN_MODE

	.long	__und_usr			@  0 (USR_32)
	.long	__und_invalid			@  1 (FIQ_32)
	.long	__und_invalid			@  2 (IRQ_32)
	.long	__und_svc			@  3 (SVC_32)
	.long	__und_invalid			@  4
	.long	__und_invalid			@  5
	.long	__und_invalid			@  6
	.long	__und_invalid			@  7
	.long	__und_invalid			@  8
	.long	__und_invalid			@  9
	.long	__und_invalid			@  a
	.long	__und_invalid			@  b
	.long	__und_invalid			@  c
	.long	__und_invalid			@  d
	.long	__und_invalid			@  e
	.long	__und_invalid			@  f

	.align	5

/*=============================================================================
 * Undefined FIQs
 *-----------------------------------------------------------------------------
 * Enter in FIQ mode, spsr = ANY CPSR, lr = ANY PC
 * MUST PRESERVE SVC SPSR, but need to switch to SVC mode to show our msg.
 * Basically to switch modes, we *HAVE* to clobber one register...  brain
 * damage alert!  I don't think that we can execute any code in here in any
 * other mode than FIQ...  Ok you can switch to another mode, but you can't
 * get out of that mode without clobbering one register.
 */
vector_fiq:
	disable_fiq
	mov.a	pc, lr

/*=============================================================================
 * Address exception handler
 *-----------------------------------------------------------------------------
 * These aren't too critical.
 * (they're not supposed to happen, and won't happen in 32-bit data mode).
 */

vector_addrexcptn:
	b	vector_addrexcptn

/*
 * We group all the following data together to optimise
 * for CPUs with separate I & D caches.
 */
	.align	5

.LCvswi:
	.word	vector_swi

	.globl	__stubs_end
__stubs_end:

	.equ	stubs_offset, __vectors_start + 0x200 - __stubs_start

	.globl	__vectors_start
__vectors_start:
	jepriv	SYS_ERROR0
@    b	l113
@	b   l113
@	b	l113
@	b	l113
@	b	l113
@   b	l113
@	b	l113
@l113:
@	ldw		r2, =0x31000000
@l112:
@	ldw		r1, =65
@	stw		r1, [r2]
@	bal		l112
    b	vector_und + stubs_offset
	ldw	pc, .LCvswi + stubs_offset
	b	vector_pabt + stubs_offset
	b	vector_dabt + stubs_offset
	b	vector_addrexcptn + stubs_offset
    b	vector_irq + stubs_offset
	b	vector_fiq + stubs_offset
	.globl	__vectors_end
__vectors_end:

	.data

	.globl	cr_alignment
	.globl	cr_no_alignment
cr_alignment:
	.space	4
cr_no_alignment:
	.space	4
