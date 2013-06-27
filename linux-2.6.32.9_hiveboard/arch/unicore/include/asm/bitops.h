/*
 * linux/arch/unicore/include/asm/bitops.h
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

#ifndef __UNICORE_BITOPS_H__
#define __UNICORE_BITOPS_H__

#ifdef __KERNEL__

#ifndef _LINUX_BITOPS_H
#error only <linux/bitops.h> can be included directly
#endif

#include <linux/compiler.h>
#include <asm/system.h>

#define smp_mb__before_clear_bit()	mb()
#define smp_mb__after_clear_bit()	mb()

/*
 * These functions are the basis of our bit ops.
 *
 * First, the atomic bitops. These use native endian.
 */
static inline void ____atomic_set_bit(unsigned int bit, volatile unsigned long *p)
{
	unsigned long flags;
	unsigned long mask = 1UL << (bit & 31);

	p += bit >> 5;

	raw_local_irq_save(flags);
	*p |= mask;
	raw_local_irq_restore(flags);
}

static inline void ____atomic_clear_bit(unsigned int bit, volatile unsigned long *p)
{
	unsigned long flags;
	unsigned long mask = 1UL << (bit & 31);

	p += bit >> 5;

	raw_local_irq_save(flags);
	*p &= ~mask;
	raw_local_irq_restore(flags);
}

static inline void ____atomic_change_bit(unsigned int bit, volatile unsigned long *p)
{
	unsigned long flags;
	unsigned long mask = 1UL << (bit & 31);

	p += bit >> 5;

	raw_local_irq_save(flags);
	*p ^= mask;
	raw_local_irq_restore(flags);
}

static inline int
____atomic_test_and_set_bit(unsigned int bit, volatile unsigned long *p)
{
	unsigned long flags;
	unsigned int res;
	unsigned long mask = 1UL << (bit & 31);

	p += bit >> 5;

	raw_local_irq_save(flags);
	res = *p;
	*p = res | mask;
	raw_local_irq_restore(flags);

	return (res & mask) != 0;
}

static inline int
____atomic_test_and_clear_bit(unsigned int bit, volatile unsigned long *p)
{
	unsigned long flags;
	unsigned int res;
	unsigned long mask = 1UL << (bit & 31);

	p += bit >> 5;

	raw_local_irq_save(flags);
	res = *p;
	*p = res & ~mask;
	raw_local_irq_restore(flags);

	return (res & mask) != 0;
}

static inline int
____atomic_test_and_change_bit(unsigned int bit, volatile unsigned long *p)
{
	unsigned long flags;
	unsigned int res;
	unsigned long mask = 1UL << (bit & 31);

	p += bit >> 5;

	raw_local_irq_save(flags);
	res = *p;
	*p = res ^ mask;
	raw_local_irq_restore(flags);

	return (res & mask) != 0;
}

#include <asm-generic/bitops/non-atomic.h>

/*
 *  Note: Linux-uc ONLY supports Little Endian-ness.
 */

/*
 * Little endian assembly bitops.  nr = 0 -> byte 0 bit 0.
 */
extern void _set_bit_le(int nr, volatile unsigned long * p);
extern void _clear_bit_le(int nr, volatile unsigned long * p);
extern void _change_bit_le(int nr, volatile unsigned long * p);
extern int _test_and_set_bit_le(int nr, volatile unsigned long * p);
extern int _test_and_clear_bit_le(int nr, volatile unsigned long * p);
extern int _test_and_change_bit_le(int nr, volatile unsigned long * p);
extern int _find_first_zero_bit_le(const void * p, unsigned size);
extern int _find_next_zero_bit_le(const void * p, int size, int offset);
extern int _find_first_bit_le(const unsigned long *p, unsigned size);
extern int _find_next_bit_le(const unsigned long *p, int size, int offset);

/*
 * The __* form of bitops are non-atomic and may be reordered.
 */
#define	ATOMIC_BITOP_LE(name,nr,p)		\
	(__builtin_constant_p(nr) ?		\
	 ____atomic_##name(nr, p) :		\
	 _##name##_le(nr,p))

#define NONATOMIC_BITOP(name,nr,p)		\
	(____nonatomic_##name(nr, p))

/*
 * These are the little endian, atomic definitions.
 */
#define set_bit(nr,p)			ATOMIC_BITOP_LE(set_bit,nr,p)
#define clear_bit(nr,p)			ATOMIC_BITOP_LE(clear_bit,nr,p)
#define change_bit(nr,p)		ATOMIC_BITOP_LE(change_bit,nr,p)
#define test_and_set_bit(nr,p)		ATOMIC_BITOP_LE(test_and_set_bit,nr,p)
#define test_and_clear_bit(nr,p)	ATOMIC_BITOP_LE(test_and_clear_bit,nr,p)
#define test_and_change_bit(nr,p)	ATOMIC_BITOP_LE(test_and_change_bit,nr,p)
#define find_first_zero_bit(p,sz)	_find_first_zero_bit_le(p,sz)
#define find_next_zero_bit(p,sz,off)	_find_next_zero_bit_le(p,sz,off)
#define find_first_bit(p,sz)		_find_first_bit_le(p,sz)
#define find_next_bit(p,sz,off)		_find_next_bit_le(p,sz,off)

#define WORD_BITOFF_TO_LE(x)		((x))

static inline int constant_fls(int x)
{
	int r = 32;

	if (!x)
		return 0;
	if (!(x & 0xffff0000u)) {
		x <<= 16;
		r -= 16;
	}
	if (!(x & 0xff000000u)) {
		x <<= 8;
		r -= 8;
	}
	if (!(x & 0xf0000000u)) {
		x <<= 4;
		r -= 4;
	}
	if (!(x & 0xc0000000u)) {
		x <<= 2;
		r -= 2;
	}
	if (!(x & 0x80000000u)) {
		x <<= 1;
		r -= 1;
	}
	return r;
}

/*
 * On UNICORE, those functions can be implemented around
 * the clz instruction for much better code efficiency.
 */

static inline int fls(int x)
{
	int ret;

	if (__builtin_constant_p(x))
	       return constant_fls(x);

	asm("cntlz\t%0, %1" : "=r" (ret) : "r" (x) : "cc");
       	ret = 32 - ret;
	return ret;
}

#define __fls(x) (fls(x) - 1)
#define ffs(x) ({ unsigned long __t = (x); fls(__t & -__t); })
#define __ffs(x) (ffs(x) - 1)
#define ffz(x) __ffs( ~(x) )


#include <asm-generic/bitops/fls64.h>

#include <asm-generic/bitops/sched.h>
#include <asm-generic/bitops/hweight.h>
#include <asm-generic/bitops/lock.h>

/*
 * Ext2 is defined to use little-endian byte ordering.
 * These do not need to be atomic.
 */
#define ext2_set_bit(nr,p)			\
		__test_and_set_bit(WORD_BITOFF_TO_LE(nr), (unsigned long *)(p))
#define ext2_set_bit_atomic(lock,nr,p)          \
                test_and_set_bit(WORD_BITOFF_TO_LE(nr), (unsigned long *)(p))
#define ext2_clear_bit(nr,p)			\
		__test_and_clear_bit(WORD_BITOFF_TO_LE(nr), (unsigned long *)(p))
#define ext2_clear_bit_atomic(lock,nr,p)        \
                test_and_clear_bit(WORD_BITOFF_TO_LE(nr), (unsigned long *)(p))
#define ext2_test_bit(nr,p)			\
		test_bit(WORD_BITOFF_TO_LE(nr), (unsigned long *)(p))
#define ext2_find_first_zero_bit(p,sz)		\
		_find_first_zero_bit_le(p,sz)
#define ext2_find_next_zero_bit(p,sz,off)	\
		_find_next_zero_bit_le(p,sz,off)
#define ext2_find_next_bit(p, sz, off) \
		_find_next_bit_le(p, sz, off)

/*
 * Minix is defined to use little-endian byte ordering.
 * These do not need to be atomic.
 */
#define minix_set_bit(nr,p)			\
		__set_bit(WORD_BITOFF_TO_LE(nr), (unsigned long *)(p))
#define minix_test_bit(nr,p)			\
		test_bit(WORD_BITOFF_TO_LE(nr), (unsigned long *)(p))
#define minix_test_and_set_bit(nr,p)		\
		__test_and_set_bit(WORD_BITOFF_TO_LE(nr), (unsigned long *)(p))
#define minix_test_and_clear_bit(nr,p)		\
		__test_and_clear_bit(WORD_BITOFF_TO_LE(nr), (unsigned long *)(p))
#define minix_find_first_zero_bit(p,sz)		\
		_find_first_zero_bit_le(p,sz)

#endif /* __KERNEL__ */

#endif /* __UNICORE_BITOPS_H__ */
