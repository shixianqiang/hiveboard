/*
 * linux/arch/unicore/include/asm/system.h
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
#ifndef __UNICORE_SYSTEM_H__
#define __UNICORE_SYSTEM_H__

#ifdef __KERNEL__

#define CPU_ARCH_UNKNOWN		0
#define CPU_ARCH_UCV1			1
#define CPU_ARCH_UCV2			2

/*
 * CR1 bits (CP#0 CR1)
 */
#define CR_M	(1 << 0)	/* MMU enable				*/
#define CR_A	(1 << 1)	/* Alignment abort enable		*/
#define CR_D	(1 << 2)	/* Dcache enable			*/
#define CR_I	(1 << 3)	/* Icache enable			*/
#define CR_B	(1 << 4)	/* Dcache write mechanism: write back	*/
#define CR_T	(1 << 5)	/* Burst enable				*/
#define CR_V	(1 << 13)	/* Vectors relocated to 0xffff0000	*/

/*
 * This is used to ensure the compiler did actually allocate the register we
 * asked it for some inline assembly sequences.  Apparently we can't trust
 * the compiler from one version to another so a bit of paranoia won't hurt.
 * This string is meant to be concatenated with the inline asm string and
 * will cause compilation to stop on mismatch.
 * (for details, see gcc PR 15089)
 */
#define __asmeq(x, y)  ".ifnc " x "," y " ; .err ; .endif\n\t"

#ifndef __ASSEMBLY__

#include <linux/linkage.h>
#include <linux/irqflags.h>

#define __exception	__attribute__((section(".exception.text")))

struct thread_info;
struct task_struct;

/* information about the system we're running on */
extern unsigned int system_rev;
extern unsigned int system_serial_low;
extern unsigned int system_serial_high;
extern unsigned int mem_fclk_21285;

struct pt_regs;

void die(const char *msg, struct pt_regs *regs, int err)
		__attribute__((noreturn));

struct siginfo;
void unicore_notify_die(const char *str, struct pt_regs *regs, struct siginfo *info,
		unsigned long err, unsigned long trap);

void hook_fault_code(int nr, int (*fn)(unsigned long, unsigned int,
				       struct pt_regs *),
		     int sig, const char *name);

#define xchg(ptr,x) \
	((__typeof__(*(ptr)))__xchg((unsigned long)(x),(ptr),sizeof(*(ptr))))

extern asmlinkage void __backtrace(void);
extern asmlinkage void c_backtrace(unsigned long fp, int pmode);

struct mm_struct;
extern void show_pte(struct mm_struct *mm, unsigned long addr);
extern void __show_regs(struct pt_regs *);

extern int cpu_architecture(void);
extern void cpu_init(void);

void unicore_machine_restart(char mode, const char *cmd);
extern void (*unicore_pm_restart)(char str, const char *cmd);

#define UDBG_UNDEFINED	(1 << 0)
#define UDBG_SYSCALL	(1 << 1)
#define UDBG_BADABORT	(1 << 2)
#define UDBG_SEGV	(1 << 3)
#define UDBG_BUS	(1 << 4)

extern unsigned int user_debug;

#define vectors_high()	(cr_alignment & CR_V)

#define isb() __asm__ __volatile__ ("" : : : "memory")
#define dsb() __asm__ __volatile__ ("" : : : "memory")
#define dmb() __asm__ __volatile__ ("" : : : "memory")

#define mb()	do { if (arch_is_coherent()) dmb(); else barrier(); } while (0)
#define rmb()	do { if (arch_is_coherent()) dmb(); else barrier(); } while (0)
#define wmb()	do { if (arch_is_coherent()) dmb(); else barrier(); } while (0)
#define smp_mb()	barrier()
#define smp_rmb()	barrier()
#define smp_wmb()	barrier()
#define read_barrier_depends()		do { } while(0)
#define smp_read_barrier_depends()	do { } while(0)

#define set_mb(var, value)	do { var = value; smp_mb(); } while (0)
#define nop() __asm__ __volatile__("mov\tr0,r0\t@ nop\n\t");

extern unsigned long cr_no_alignment;	/* defined in entry-unicore.S */
extern unsigned long cr_alignment;	/* defined in entry-unicore.S */

static inline unsigned int get_cr(void)
{
	unsigned int val;
	asm("movc %0, p0.c1, #0" : "=r" (val) : : "cc");
	return val;
}

static inline void set_cr(unsigned int val)
{
	asm volatile("movc p0.c1, %0, #0	@set CR"
	  : : "r" (val) : "cc");
	isb();
}

extern void adjust_cr(unsigned long mask, unsigned long set);

#define CPACC_FULL(n)		(3 << (n * 2))
#define CPACC_SVC(n)		(1 << (n * 2))
#define CPACC_DISABLE(n)	(0 << (n * 2))

/*
 * switch_to(prev, next) should switch from task `prev' to `next'
 * `prev' will never be the same as `next'.  schedule() itself
 * contains the memory barrier to tell GCC not to cache `current'.
 */
extern struct task_struct *__switch_to(struct task_struct *, struct thread_info *, struct thread_info *);

#define switch_to(prev,next,last)					\
do {									\
	last = __switch_to(prev,task_thread_info(prev), task_thread_info(next));	\
} while (0)

static inline unsigned long __xchg(unsigned long x, volatile void *ptr, int size)
{
	extern void __bad_xchg(volatile void *, int);
	unsigned long ret;

	switch (size) {
	case 1:
		asm volatile("@	__xchg1\n"
		"	swapb	%0, %1, [%2]"
			: "=&r" (ret)
			: "r" (x), "r" (ptr)
			: "memory", "cc");
		break;
	case 4:
		asm volatile("@	__xchg4\n"
		"	swapw	%0, %1, [%2]"
			: "=&r" (ret)
			: "r" (x), "r" (ptr)
			: "memory", "cc");
		break;
	default:
		__bad_xchg(ptr, size), ret = 0;
		break;
	}

	return ret;
}

extern void disable_hlt(void);
extern void enable_hlt(void);

#include <asm-generic/cmpxchg-local.h>

/*
 * cmpxchg_local and cmpxchg64_local are atomic wrt current CPU. Always make
 * them available.
 */
#define cmpxchg_local(ptr, o, n)				  	       \
	((__typeof__(*(ptr)))__cmpxchg_local_generic((ptr), (unsigned long)(o),\
			(unsigned long)(n), sizeof(*(ptr))))
#define cmpxchg64_local(ptr, o, n) __cmpxchg64_local_generic((ptr), (o), (n))

#include <asm-generic/cmpxchg.h>

#endif /* __ASSEMBLY__ */

#define arch_align_stack(x) (x)

#endif /* __KERNEL__ */

#endif
