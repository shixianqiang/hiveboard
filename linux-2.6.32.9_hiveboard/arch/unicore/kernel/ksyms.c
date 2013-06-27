/*
 * linux/arch/unicore/kernel/ksyms.c
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
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/cryptohash.h>
#include <linux/delay.h>
#include <linux/in6.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#include <asm/checksum.h>
#include <asm/system.h>
#include <asm/ftrace.h>

/*
 * libgcc functions - functions that are used internally by the
 * compiler...  (prototypes are not correct though, but that
 * doesn't really matter since they're not versioned).
 */
extern void __ashldi3(void);
extern void __ashrdi3(void);
extern void __divsi3(void);
extern void __lshrdi3(void);
extern void __modsi3(void);
//extern void __muldi3(void);
extern void __ucmpdi2(void);
extern void __udivsi3(void);
extern void __umodsi3(void);
extern void __do_div64(void);

extern void fpundefinstr(void);
extern void fp_enter(void);

/*
 * This has a special calling convention; it doesn't
 * modify any of the usual registers, except for LR.
 */
#define EXPORT_CRC_ALIAS(sym) __CRC_SYMBOL(sym, "")

#define EXPORT_SYMBOL_ALIAS(sym,orig)		\
 EXPORT_CRC_ALIAS(sym)				\
 static const struct kernel_symbol __ksymtab_##sym	\
  __used __attribute__((section("__ksymtab"))) =	\
    { (unsigned long)&orig, #sym };

EXPORT_SYMBOL(__backtrace);

	/* platform dependent support */
EXPORT_SYMBOL(__udelay);
EXPORT_SYMBOL(__const_udelay);

	/* networking */
EXPORT_SYMBOL(csum_partial);
EXPORT_SYMBOL(csum_partial_copy_from_user);
EXPORT_SYMBOL(csum_partial_copy_nocheck);
EXPORT_SYMBOL(__csum_ipv6_magic);

	/* io */
#ifndef __raw_readsb
EXPORT_SYMBOL(__raw_readsb);
#endif
#ifndef __raw_readsw
EXPORT_SYMBOL(__raw_readsw);
#endif
#ifndef __raw_readsl
EXPORT_SYMBOL(__raw_readsl);
#endif
#ifndef __raw_writesb
EXPORT_SYMBOL(__raw_writesb);
#endif
#ifndef __raw_writesw
EXPORT_SYMBOL(__raw_writesw);
#endif
#ifndef __raw_writesl
EXPORT_SYMBOL(__raw_writesl);
#endif

	/* string / mem functions */
EXPORT_SYMBOL(strchr);
EXPORT_SYMBOL(strrchr);
EXPORT_SYMBOL(memset);
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memmove);
EXPORT_SYMBOL(memchr);
EXPORT_SYMBOL(__memzero);

	/* user mem (segment) */
EXPORT_SYMBOL(__strnlen_user);
EXPORT_SYMBOL(__strncpy_from_user);

EXPORT_SYMBOL(copy_page);

EXPORT_SYMBOL(__copy_from_user);
EXPORT_SYMBOL(__copy_to_user);
EXPORT_SYMBOL(__clear_user);

EXPORT_SYMBOL(__get_user_1);
EXPORT_SYMBOL(__get_user_2);
EXPORT_SYMBOL(__get_user_4);

EXPORT_SYMBOL(__put_user_1);
EXPORT_SYMBOL(__put_user_2);
EXPORT_SYMBOL(__put_user_4);
EXPORT_SYMBOL(__put_user_8);

	/* crypto hash */
EXPORT_SYMBOL(sha_transform);

	/* gcc lib functions */
EXPORT_SYMBOL(__ashldi3);
EXPORT_SYMBOL(__ashrdi3);
EXPORT_SYMBOL(__divsi3);
EXPORT_SYMBOL(__lshrdi3);
EXPORT_SYMBOL(__modsi3);
//EXPORT_SYMBOL(__muldi3);
EXPORT_SYMBOL(__ucmpdi2);
EXPORT_SYMBOL(__udivsi3);
EXPORT_SYMBOL(__umodsi3);
EXPORT_SYMBOL(__do_div64);

	/* bitops */
EXPORT_SYMBOL(_set_bit_le);
EXPORT_SYMBOL(_test_and_set_bit_le);
EXPORT_SYMBOL(_clear_bit_le);
EXPORT_SYMBOL(_test_and_clear_bit_le);
EXPORT_SYMBOL(_change_bit_le);
EXPORT_SYMBOL(_test_and_change_bit_le);
EXPORT_SYMBOL(_find_first_zero_bit_le);
EXPORT_SYMBOL(_find_next_zero_bit_le);
EXPORT_SYMBOL(_find_first_bit_le);
EXPORT_SYMBOL(_find_next_bit_le);

#ifdef CONFIG_FUNCTION_TRACER
EXPORT_SYMBOL(mcount);
EXPORT_SYMBOL(__gnu_mcount_nc);
#endif
