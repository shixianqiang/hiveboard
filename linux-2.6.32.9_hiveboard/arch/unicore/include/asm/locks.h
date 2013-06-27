/*
 * linux/arch/unicore/include/asm/locks.h
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
 *  Interrupt safe locking assembler. 
 */
#ifndef __UNICORE_PROC_LOCKS_H__
#define __UNICORE_PROC_LOCKS_H__

#define __down_op(ptr,fail)			\
	({					\
	__asm__ __volatile__(			\
	"@ down_op\n"				\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr, lr\n"			\
"	ldr	lr, [%0]\n"			\
"	subs	lr, lr, %1\n"			\
"	str	lr, [%0]\n"			\
"	mrs	lr, cpsr\n"			\
"	msr	cpsr, ip\n"			\
"	msr	cpsr_flag, lr\n"		\
"	movmi	ip, %0\n"			\
"	blmi	" #fail				\
	:					\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	smp_mb();				\
	})

#define __down_op_ret(ptr,fail)			\
	({					\
		unsigned int ret;		\
	__asm__ __volatile__(			\
	"@ down_op_ret\n"			\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr, lr\n"			\
"	ldr	lr, [%1]\n"			\
"	subs	lr, lr, %2\n"			\
"	str	lr, [%1]\n"			\
"	mrs	lr, cpsr\n"			\
"	msr	cpsr, ip\n"			\
"	msr	cpsr_flag, lr\n"		\
"	movmi	ip, %1\n"			\
"	movpl	ip, #0\n"			\
"	blmi	" #fail "\n"			\
"	mov	%0, ip"				\
	: "=&r" (ret)				\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	smp_mb();				\
	ret;					\
	})

#define __up_op(ptr,wake)			\
	({					\
	smp_mb();				\
	__asm__ __volatile__(			\
	"@ up_op\n"				\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr, lr\n"			\
"	ldr	lr, [%0]\n"			\
"	adds	lr, lr, %1\n"			\
"	str	lr, [%0]\n"			\
"	mrs	lr, cpsr\n"			\
"	msr	cpsr, ip\n"			\
"	msr	cpsr_flag, lr\n"		\
"	movle	ip, %0\n"			\
"	blle	" #wake				\
	:					\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	})

/*
 * The value 0x01000000 supports up to 128 processors and
 * lots of processes.  BIAS must be chosen such that sub'ing
 * BIAS once per CPU will result in the long remaining
 * negative.
 */
#define RW_LOCK_BIAS      0x01000000
#define RW_LOCK_BIAS_STR "0x01000000"

#define __down_op_write(ptr,fail)		\
	({					\
	__asm__ __volatile__(			\
	"@ down_op_write\n"			\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr, lr\n"			\
"	ldr	lr, [%0]\n"			\
"	subs	lr, lr, %1\n"			\
"	str	lr, [%0]\n"			\
"	mrs	lr, cpsr\n"			\
"	msr	cpsr, ip\n"			\
"	msr	cpsr_flag, lr\n"		\
"	movne	ip, %0\n"			\
"	blne	" #fail				\
	:					\
	: "r" (ptr), "I" (RW_LOCK_BIAS)		\
	: "ip", "lr", "cc");			\
	smp_mb();				\
	})

#define __up_op_write(ptr,wake)			\
	({					\
	__asm__ __volatile__(			\
	"@ up_op_write\n"			\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr, lr\n"			\
"	ldr	lr, [%0]\n"			\
"	adds	lr, lr, %1\n"			\
"	str	lr, [%0]\n"			\
"	mrs	lr, cpsr\n"			\
"	msr	cpsr, ip\n"			\
"	msr	cpsr_flag, lr\n"		\
"	movcs	ip, %0\n"			\
"	blcs	" #wake				\
	:					\
	: "r" (ptr), "I" (RW_LOCK_BIAS)		\
	: "ip", "lr", "cc");			\
	smp_mb();				\
	})

#define __down_op_read(ptr,fail)		\
	__down_op(ptr, fail)

#define __up_op_read(ptr,wake)			\
	({					\
	smp_mb();				\
	__asm__ __volatile__(			\
	"@ up_op_read\n"			\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr, lr\n"			\
"	ldr	lr, [%0]\n"			\
"	adds	lr, lr, %1\n"			\
"	str	lr, [%0]\n"			\
"	mrs	lr, cpsr\n"			\
"	msr	cpsr, ip\n"			\
"	msr	cpsr_flag, lr\n"		\
"	moveq	ip, %0\n"			\
"	bleq	" #wake				\
	:					\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	})

#endif
