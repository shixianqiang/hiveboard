#ifndef __UNICORE_CPUTYPE_H__
#define __UNICORE_CPUTYPE_H__

#include <linux/stringify.h>

#define CPUID_ID	0
#define CPUID_CACHETYPE	1

#define read_cpuid(reg)							\
	({								\
		unsigned int __val;					\
		asm("movc	%0, p0.c0, #" __stringify(reg)		\
		    : "=r" (__val)					\
		    :							\
		    : "cc");						\
		__val;							\
	})

#define read_cpuid_ext(ext_reg)						\
	({								\
		unsigned int __val;					\
		asm("movc	%0, p15.c0, " ext_reg			\
		    : "=r" (__val)					\
		    :							\
		    : "cc");						\
		__val;							\
	})

/*
 * The CPU ID never changes at run time, so we might as well tell the
 * compiler that it's constant.  Use this function to read the CPU ID
 * rather than directly reading processor_id or read_cpuid() directly.
 */
static inline unsigned int __attribute_const__ read_cpuid_id(void)
{
	return read_cpuid(CPUID_ID);
}

static inline unsigned int __attribute_const__ read_cpuid_cachetype(void)
{
	return read_cpuid(CPUID_CACHETYPE);
}

#endif
