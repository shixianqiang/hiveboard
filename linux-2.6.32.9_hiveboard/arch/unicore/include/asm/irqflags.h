/*
 * linux/arch/unicore/include/asm/irqflags.h
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
#ifndef __UNICORE_IRQFLAGS_H__
#define __UNICORE_IRQFLAGS_H__

#ifdef __KERNEL__

#include <asm/ptrace.h>

/*
 * CPU interrupt mask handling.
 */

/*
 * Save the current interrupt enable state & disable IRQs
 */
#define raw_local_irq_save(x)					\
	({							\
		unsigned long temp;				\
		(void) (&temp == &x);				\
	__asm__ __volatile__(					\
	"mov	%0, asr			@ local_irq_save\n"	\
"	or	%1, %0, #128\n"					\
"	mov.a	asr, %1"					\
	: "=r" (x), "=r" (temp)					\
	:							\
	: "memory", "cc");					\
	})
	
/*
 * Enable IRQs
 */
#define raw_local_irq_enable()					\
	({							\
		unsigned long temp;				\
	__asm__ __volatile__(					\
	"mov	%0, asr			@ local_irq_enable\n"	\
"	andn	%0, %0, #128\n"					\
"	mov.a	asr, %0"					\
	: "=r" (temp)						\
	:							\
	: "memory", "cc");					\
	})

/*
 * Disable IRQs
 */
#define raw_local_irq_disable()					\
	({							\
		unsigned long temp;				\
	__asm__ __volatile__(					\
	"mov	%0, asr			@ local_irq_disable\n"	\
"	or	%0, %0, #128\n"					\
"	mov.a	asr, %0"					\
	: "=r" (temp)						\
	:							\
	: "memory", "cc");					\
	})

/*
 * Enable FIQs
 */
#define local_fiq_enable()					\
	({							\
		unsigned long temp;				\
	__asm__ __volatile__(					\
	"mov	%0, asr			@ stf\n"		\
"	andn	%0, %0, #64\n"					\
"	mov.a	asr, %0"					\
	: "=r" (temp)						\
	:							\
	: "memory", "cc");					\
	})

/*
 * Disable FIQs
 */
#define local_fiq_disable()					\
	({							\
		unsigned long temp;				\
	__asm__ __volatile__(					\
	"mov	%0, asr			@ clf\n"		\
"	or	%0, %0, #64\n"					\
"	mov.a	asr, %0"					\
	: "=r" (temp)						\
	:							\
	: "memory", "cc");					\
	})

/*
 * Save the current interrupt enable state.
 */
#define raw_local_save_flags(x)					\
	({							\
	__asm__ __volatile__(					\
	"mov	%0, asr			@ local_save_flags"	\
	: "=r" (x) : : "memory", "cc");				\
	})

/*
 * restore saved IRQ & FIQ state
 */
#define raw_local_irq_restore(x)				\
	({							\
		unsigned long temp;				\
	__asm__ __volatile__(					\
	"mov	%0, asr\n"					\
	"mov.a	asr, %1			@ local_irq_restore\n"	\
	"mov.f	asr, %0"					\
	: "=&r" (temp)                                          \
	: "r" (x)						\
	: "memory", "cc");                                      \
	})

#define raw_irqs_disabled_flags(flags)	\
({					\
	(int)((flags) & PSR_I_BIT);	\
})

#endif
#endif
