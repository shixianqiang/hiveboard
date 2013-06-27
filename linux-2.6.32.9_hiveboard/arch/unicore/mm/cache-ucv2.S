/*
 * linux/arch/unicore/mm/cache-ucv2.S
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
 *  This is the "shell" of the UniCore-v2 processor support.
 */
#include <linux/linkage.h>
#include <linux/init.h>
#include <asm/assembler.h>
#include <asm/page.h>

#include "proc-macros.S"

/*
 *	ucv2_flush_kern_cache_all()
 *	ucv2_flush_user_cache_all()
 *
 *	Flush the entire cache.
 */
ENTRY(ucv2_flush_kern_cache_all)
	/*FALLTHROUGH*/
ENTRY(ucv2_flush_user_cache_all)
	mov	r0, #0
	movc	p0.c5, r0, #14			@ Dcache flush all
	nop8

	mov	r0, #0
	movc	p0.c5, r0, #20			@ Icache invalidate all
	nop8

	mov	pc, lr

/*
 *	ucv2_flush_cache_range(start, end, flags)
 *
 *	Flush a range of TLB entries in the specified address space.
 *
 *	- start - start address (may not be aligned)
 *	- end   - end address (exclusive, may not be aligned)
 *	- flags	- vm_area_struct flags describing address space
 */
ENTRY(ucv2_flush_user_cache_range)
	cmpxor.a	r2, #0
	beq	ucv2_dma_flush_range
	
#ifndef CONFIG_CPU_DCACHE_LINE_DISABLE
	andn	r0, r0, #CACHE_LINESIZE - 1	@ Safety check
	sub	r1, r1, r0
	cmpsub.a	r1, #MAX_AREA_SIZE
	bsg	2f

	andn	r1, r1, #CACHE_LINESIZE - 1
	add	r1, r1, #CACHE_LINESIZE

101:	dcacheline_flush	r0, r11, r12
	
	add	r0, r0, #CACHE_LINESIZE
	sub.a	r1, r1, #CACHE_LINESIZE
	bns	101b
	b	3f
#endif
2:	mov	ip, #0
	movc	p0.c5, ip, #14			@ Dcache flush all
	nop8

3:	mov	ip, #0
	movc	p0.c5, ip, #20			@ Icache invalidate all
	nop8

	mov	pc, lr

/*
 *	ucv2_coherent_kern_range(start,end)
 *	ucv2_coherent_user_range(start,end)
 *
 *	Ensure that the I and D caches are coherent within specified
 *	region.  This is typically used when code has been written to
 *	a memory region, and will be executed.
 *
 *	- start   - virtual start address of region
 *	- end     - virtual end address of region
 */
ENTRY(ucv2_coherent_kern_range)
	/* FALLTHROUGH */
ENTRY(ucv2_coherent_user_range)
#ifndef CONFIG_CPU_DCACHE_LINE_DISABLE
	andn	r0, r0, #CACHE_LINESIZE - 1	@ Safety check
	sub	r1, r1, r0
	cmpsub.a	r1, #MAX_AREA_SIZE
	bsg	2f

	andn	r1, r1, #CACHE_LINESIZE - 1
	add	r1, r1, #CACHE_LINESIZE

	@ r0 va2pa r10
	mov	r9, #PAGE_SZ
	sub	r9, r9, #1			@ PAGE_MASK
101:	va2pa	r0, r10, r11, r12, r13, 2f	@ r10 is PA
	b	103f
102:	cmpand.a	r0, r9
	beq	101b
	
103:	movc	p0.c5, r10, #11			@ Dcache clean line of R10
	nop8

	add	r0, r0, #CACHE_LINESIZE
	add	r10, r10, #CACHE_LINESIZE
	sub.a	r1, r1, #CACHE_LINESIZE
	bns	102b
	b	3f
#endif
2:	mov	ip, #0
	movc	p0.c5, ip, #10			@ Dcache clean all
	nop8

3:	mov	ip, #0
	movc	p0.c5, ip, #20			@ Icache invalidate all
	nop8

	mov	pc, lr

/*
 *	ucv2_flush_kern_dcache_page(kaddr)
 *
 *	Ensure that the data held in the page kaddr is written back
 *	to the page in question.
 *
 *	- kaddr   - kernel address (guaranteed to be page aligned)
 */
ENTRY(ucv2_flush_kern_dcache_page)
	mov	ip, #0
	movc	p0.c5, ip, #14			@ Dcache flush all
	nop8
	mov	pc, lr

/*
 *	ucv2_dma_clean_range(start,end)
 *	- start   - virtual start address of region
 *	- end     - virtual end address of region
 */
ENTRY(ucv2_dma_clean_range)
#ifndef CONFIG_CPU_DCACHE_LINE_DISABLE
	andn	r0, r0, #CACHE_LINESIZE - 1
	sub	r1, r1, r0
	andn	r1, r1, #CACHE_LINESIZE - 1
	add	r1, r1, #CACHE_LINESIZE

	cmpsub.a	r1, #MAX_AREA_SIZE
	bsg	2f

	@ r0 va2pa r10
	mov	r9, #PAGE_SZ
	sub	r9, r9, #1			@ PAGE_MASK
101:	va2pa	r0, r10, r11, r12, r13, 2f	@ r10 is PA
	b	1f
102:	cmpand.a	r0, r9
	beq	101b
	
1:	movc	p0.c5, r10, #11			@ Dcache clean line of R10
	nop8
	add	r0, r0, #CACHE_LINESIZE
	add	r10, r10, #CACHE_LINESIZE
	sub.a	r1, r1, #CACHE_LINESIZE
	bns	102b
	mov	pc, lr
#endif
2:	mov	ip, #0
	movc	p0.c5, ip, #10			@ Dcache clean all
	nop8

	mov	pc, lr

/*
 *	ucv2_dma_inv_range(start,end)
 *	ucv2_dma_flush_range(start,end)
 *	- start   - virtual start address of region
 *	- end     - virtual end address of region
 */
ENTRY(ucv2_dma_inv_range)
	/* FALLTHROUGH */
ENTRY(ucv2_dma_flush_range)
#ifndef CONFIG_CPU_DCACHE_LINE_DISABLE
	andn	r0, r0, #CACHE_LINESIZE - 1
	sub	r1, r1, r0
	andn	r1, r1, #CACHE_LINESIZE - 1
	add	r1, r1, #CACHE_LINESIZE

	cmpsub.a	r1, #MAX_AREA_SIZE
	bsg	2f

	@ r0 va2pa r10
101:	dcacheline_flush	r0, r11, r12
	
	add	r0, r0, #CACHE_LINESIZE
	sub.a	r1, r1, #CACHE_LINESIZE
	bns	101b
	mov	pc, lr
#endif
2:	mov	ip, #0
	movc	p0.c5, ip, #14			@ Dcache flush all
	nop8

	mov	pc, lr

	__INITDATA

	.type	ucv2_cache_fns, #object
ENTRY(ucv2_cache_fns)
	.long	ucv2_flush_kern_cache_all
	.long	ucv2_flush_user_cache_all
	.long	ucv2_flush_user_cache_range
	.long	ucv2_coherent_kern_range
	.long	ucv2_coherent_user_range
	.long	ucv2_flush_kern_dcache_page
	.long	ucv2_dma_inv_range
	.long	ucv2_dma_clean_range
	.long	ucv2_dma_flush_range
	.size	ucv2_cache_fns, . - ucv2_cache_fns
