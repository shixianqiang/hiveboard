/*
 * linux/arch/unicore/include/asm/checksum.h
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
 * IP checksum routines
 */
#ifndef __UNICORE_CHECKSUM_H__
#define __UNICORE_CHECKSUM_H__

#include <linux/in6.h>

/*
 * computes the checksum of a memory block at buff, length len,
 * and adds in "sum" (32-bit)
 *
 * returns a 32-bit number suitable for feeding into itself
 * or csum_tcpudp_magic
 *
 * this function must be called with even lengths, except
 * for the last fragment, which may be odd
 *
 * it's best to have buff aligned on a 32-bit boundary
 */
__wsum csum_partial(const void *buff, int len, __wsum sum);

/*
 * the same as csum_partial, but copies from src while it
 * checksums, and handles user-space pointer exceptions correctly, when needed.
 *
 * here even more important to align src and dst on a 32-bit (or even
 * better 64-bit) boundary
 */

__wsum
csum_partial_copy_nocheck(const void *src, void *dst, int len, __wsum sum);

__wsum
csum_partial_copy_from_user(const void __user *src, void *dst, int len, __wsum sum, int *err_ptr);

/*
 * 	Fold a partial checksum without adding pseudo headers
 */
static inline __sum16 csum_fold(__wsum sum)
{
	__asm__(
	"add	%0, %1, %1 <> #16	@ csum_fold"
	: "=r" (sum)
	: "r" (sum)
	: "cc");
	return (__force __sum16)(~(__force u32)sum >> 16);
}

/*
 *	This is a version of ip_compute_csum() optimized for IP headers,
 *	which always checksum on 4 octet boundaries.
 */
static inline __sum16
ip_fast_csum(const void *iph, unsigned int ihl)
{
	unsigned int tmp1;
	__wsum sum;

	__asm__ __volatile__(
	"ldw.w	%0, [%1]+, #4		@ ip_fast_csum		\n\
	ldw.w	%3, [%1]+, #4					\n\
	sub	%2, %2, #5					\n\
	add.a	%0, %0, %3					\n\
	ldw.w	%3, [%1]+, #4					\n\
	addc.a	%0, %0, %3					\n\
	ldw.w	%3, [%1]+, #4					\n\
1:	addc.a	%0, %0, %3					\n\
	ldw.w	%3, [%1]+, #4					\n\
	cmpand.a	%2, #15		@ do this carefully	\n\
	beq	2f				        	\n\
	sub	%2, %2, #1		@ without destroying	\n\
	bne	1b			@ the carry flag	\n\
2:	addc.a	%0, %0, %3					\n\
	addc	%0, %0, #0"
	: "=r" (sum), "=r" (iph), "=r" (ihl), "=r" (tmp1)
	: "1" (iph), "2" (ihl)
	: "cc", "memory");
	return csum_fold(sum);
}

static inline __wsum
csum_tcpudp_nofold(__be32 saddr, __be32 daddr, unsigned short len,
		   unsigned short proto, __wsum sum)
{
	__asm__(
	"add.a	%0, %1, %2		@ csum_tcpudp_nofold	\n\
	addc.a	%0, %0, %3					\n\
	addc.a	%0, %0, %4 << #8				\n\
	addc.a	%0, %0, %5					\n\
	addc	%0, %0, #0"
	: "=&r"(sum)
	: "r" (sum), "r" (daddr), "r" (saddr), "r" (len), "Ir" (htons(proto))
	: "cc");
	return sum;
}	
/*
 * computes the checksum of the TCP/UDP pseudo-header
 * returns a 16-bit checksum, already complemented
 */
static inline __sum16
csum_tcpudp_magic(__be32 saddr, __be32 daddr, unsigned short len,
		  unsigned short proto, __wsum sum)
{
	return csum_fold(csum_tcpudp_nofold(saddr, daddr, len, proto, sum));
}


/*
 * this routine is used for miscellaneous IP-like checksums, mainly
 * in icmp.c
 */
static inline __sum16
ip_compute_csum(const void *buff, int len)
{
	return csum_fold(csum_partial(buff, len, 0));
}

#define _HAVE_ARCH_IPV6_CSUM
extern __wsum
__csum_ipv6_magic(const struct in6_addr *saddr, const struct in6_addr *daddr, __be32 len,
		__be32 proto, __wsum sum);

static inline __sum16
csum_ipv6_magic(const struct in6_addr *saddr, const struct in6_addr *daddr, __u32 len,
		unsigned short proto, __wsum sum)
{
	return csum_fold(__csum_ipv6_magic(saddr, daddr, htonl(len),
					   htonl(proto), sum));
}
#endif
