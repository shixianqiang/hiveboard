/*
 * linux/arch/unicore/mach-v8/include/mach/debug-macro.S
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
 * Debugging macro include header
 */
#include <asm/asm-offsets.h>
#include <mach/hardware.h>

		.macro	put_word_ocd, rd, rx=r16
1001:		movc		\rx, p1.c0, #0
		cmpand.a	\rx, #2
		bne	1001b
		movc		p1.c1, \rd, #1
		.endm

#ifdef CONFIG_DEBUG_OCD
		/* debug using UniCore On-Chip-Debugger */
		.macro	addruart, rx
		.endm

		.macro	senduart, rd, rx
		put_word_ocd	\rd, \rx
		.endm

		.macro	busyuart, rd, rx
		.endm

		.macro	waituart, rd, rx
		.endm
#else	
#define UART_CLK_DEFAULT        3686400 * 20
	/* Uartclk = MCLK/ 2, The MCLK on my board is 3686400 * 40  */
#define BAUD_RATE_DEFAULT	115200
	/* The baud rate of the serial port */	

#define UART_DIVISOR_DEFAULT	(UART_CLK_DEFAULT / (16 * BAUD_RATE_DEFAULT) - 1)

		.macro	addruart,rx
		mrc	p0, #0, \rx, c1, c0
		tst	\rx, #1			@ MMU enabled?
		moveq	\rx, #0xee000000	@ physical base address
		movne	\rx, #0x6e000000	@ virtual address

		@ We probe for the active serial port here
		@ However, now we assume UART0 is active:	epip4d
		@ We assume r1 and r2 can be clobbered.

		movl 	r2, #UART_DIVISOR_DEFAULT
		mov 	r1, #0x80
		str	r1, [\rx, #UART_LCR_OFFSET]
		and	r1, r2, #0xff00
		mov	r1, r1, lsr #8
		str	r1, [\rx, #UART_DLH_OFFSET]
		and	r1, r2, #0xff
		str	r1, [\rx, #UART_DLL_OFFSET]
		mov 	r1, #0x7
		str	r1, [\rx, #UART_FCR_OFFSET]
		mov 	r1, #0x3
		str	r1, [\rx, #UART_LCR_OFFSET]
		mov 	r1, #0x0
		str	r1, [\rx, #UART_IER_OFFSET]
		.endm

		.macro	senduart,rd,rx
		str	\rd, [\rx, #UART_THR_OFFSET]
		.endm

		.macro	waituart,rd,rx
1001:		ldr	\rd, [\rx, #UART_LSR_OFFSET]
		tst	\rd, #UART_LSR_THRE
		beq	1001b
		.endm

		.macro	busyuart,rd,rx
1001:		ldr	\rd, [\rx, #UART_LSR_OFFSET]
		tst	\rd, #UART_LSR_TEMT
		bne	1001b
		.endm
#endif

#ifdef CONFIG_DEBUG_OCD_BREAKPOINT
	.macro pabt_breakpoint, t1, t2, taken=203f
	mrc	p0, #0, \t1, c3, c0, #1
	cmp	\t1, #0x00000000
	bne	203f

	movl	\t1, #0xbcbcbcbc
	put_word_ocd	\t1, \t2

	ldr	\t1, [sp, #S_PC]
	put_word_ocd	\t1, \t2

	@ print debug information
@	mov	r0, sp
@	bl	__show_regs
@	bl	dump_stack

	b	\taken
203:
	.endm

	.macro dabt_breakpoint, t1, t2, taken=203f
	mrc	p1, #0, \t1, c2, c0
	tst	\t1, #0x00000001
	beq	203f

	movl	\t2, #0x0
	mcr	p1, #0, \t2, c2, c0

	movl	\t1, #0xecececec
	put_word_ocd	\t1, \t2

	ldr	\t1, [sp, #S_PC]
	put_word_ocd	\t1, \t2

	@ print debug information
	mov	r0, sp
	bl	__show_regs
@	bl	dump_stack

	b	\taken
203:
	.endm
#else
	.macro pabt_breakpoint, t1, t2, taken=203f
	.endm
	.macro dabt_breakpoint, t1, t2, taken=203f
	.endm
#endif /* CONFIG_DEBUG_OCD_BREAKPOINT */

