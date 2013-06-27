/*
 * linux/arch/unicore/include/asm/assembler.h
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
 *  Do not include any C declarations in this file - it is included by
 *  assembler source.
 */
#ifndef __ASSEMBLY__
#error "Only include this from assembly code"
#endif

#include <asm/ptrace.h>

/*
 * Little Endian independent macros for shifting bytes within registers.
 */
#define pull            >>
#define push            <<
#define get_byte_0      << #0
#define get_byte_1	>> #8
#define get_byte_2	>> #16
#define get_byte_3	>> #24
#define put_byte_0      << #0
#define put_byte_1	<< #8
#define put_byte_2	<< #16
#define put_byte_3	<< #24

/*
 * Enable and disable interrupts
 */
	.macro	disable_irq_notrace, temp
	mov	\temp, asr
        andn     \temp, \temp, #0xFF
	or 	\temp, \temp, #PSR_I_BIT | PRIV_MODE
	mov.a	asr, \temp
	.endm

	.macro	enable_irq_notrace, temp
	mov	\temp, asr
        andn     \temp, \temp, #0xFF
	or 	\temp, \temp, #PRIV_MODE
	mov.a	asr, \temp
	.endm

	.macro asm_trace_hardirqs_off
#if defined(CONFIG_TRACE_IRQFLAGS)
	stm.w   (ip, lr), [sp-]
	stm.w   (r0-r3), [sp-]
	b.l	trace_hardirqs_off
	ldm.w	(r0-r3), [sp]+
	ldm.w	(ip, lr), [sp]+
#endif
	.endm

	.macro asm_trace_hardirqs_on_cond, cond
#if defined(CONFIG_TRACE_IRQFLAGS)
	/*
	 * actually the registers should be pushed and pop'd conditionally, but
	 * after bl the flags are certainly clobbered
	 */
	stm.w   (ip, lr), [sp-]
	stm.w   (r0-r3), [sp-]
	bl\cond	trace_hardirqs_on
	ldm.w	(r0-r3), [sp]+
	ldm.w	(ip, lr), [sp]+
#endif
	.endm

	.macro asm_trace_hardirqs_on
	asm_trace_hardirqs_on_cond al
	.endm

	.macro disable_irq, temp
	disable_irq_notrace \temp
	asm_trace_hardirqs_off
	.endm

	.macro enable_irq, temp
	asm_trace_hardirqs_on
	enable_irq_notrace \temp
	.endm
/*
 * Save the current IRQ state and disable IRQs.  Note that this macro
 * assumes FIQs are enabled, and that the processor is in SVC mode.
 */
	.macro	save_and_disable_irqs, oldcpsr, temp
	mov	\oldcpsr, asr
	disable_irq     \temp
	.endm

/*
 * Restore interrupt state previously stored in a register.  We don't
 * guarantee that this will preserve the flags.
 */
	.macro	restore_irqs_notrace, oldcpsr, temp
	mov	\temp, asr
	mov.a	asr, \oldcpsr
	mov.f	asr, \temp
	.endm

	.macro restore_irqs, oldcpsr, temp
	cmpand.a	\oldcpsr, #PSR_I_BIT
	asm_trace_hardirqs_on_cond eq
	restore_irqs_notrace \oldcpsr, \temp
	.endm

#define USER(x...)				\
9999:	x;					\
	.section __ex_table,"a";		\
	.align	3;				\
	.long	9999b,9001f;			\
	.previous

	.macro	notcond, cond, nexti=.+8
	.ifc	\cond, eq
		bne	\nexti
	.else;	.ifc	\cond, ne
		beq	\nexti
	.else;	.ifc	\cond, ea
		bub	\nexti
	.else;	.ifc	\cond, ub
		bea	\nexti
	.else;	.ifc	\cond, fs
		bns	\nexti
	.else;	.ifc	\cond, ns
		bfs	\nexti
	.else;	.ifc	\cond, fv
		bnv	\nexti
	.else;	.ifc	\cond, nv
		bfv	\nexti
	.else;	.ifc	\cond, ua
		beb	\nexti
	.else;	.ifc	\cond, eb
		bua	\nexti
	.else;	.ifc	\cond, eg
		bsl	\nexti
	.else;	.ifc	\cond, sl
		beg	\nexti
	.else;	.ifc	\cond, sg
		bel	\nexti
	.else;	.ifc	\cond, el
		bsg	\nexti
	.else;	.ifnc	\cond, al
		.error  "Unknown cond in notcond macro argument"
	.endif;	.endif;	.endif;	.endif;	.endif;	.endif;	.endif
	.endif;	.endif;	.endif;	.endif;	.endif;	.endif;	.endif
	.endif
	.endm

	.macro	usracc, instr, reg, ptr, inc, cond, rept, abort
	.rept	\rept
	notcond	\cond, .+8
9999:
	.if	\inc == 1
	\instr\()b.u \reg, [\ptr], #\inc
	.elseif	\inc == 4
	\instr\()w.u \reg, [\ptr], #\inc
	.else
	.error	"Unsupported inc macro argument"
	.endif

	.section __ex_table,"a"
	.align	3
	.long	9999b, \abort
	.previous
	.endr
	.endm

	.macro	strusr, reg, ptr, inc, cond=al, rept=1, abort=9001f
	usracc	st, \reg, \ptr, \inc, \cond, \rept, \abort
	.endm

	.macro	ldrusr, reg, ptr, inc, cond=al, rept=1, abort=9001f
	usracc	ld, \reg, \ptr, \inc, \cond, \rept, \abort
	.endm
