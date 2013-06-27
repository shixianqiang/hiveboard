/*
 * linux/arch/unicore/lib/lib1funcs.S
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
 * Optimized UniCore division routines
 */

#include <linux/linkage.h>
#include <asm/assembler.h>


.macro UC_DIV_BODY dividend, divisor, result, curbit

	cntlz	\curbit, \divisor
	cntlz	\result, \dividend
	sub	\result, \curbit, \result
	mov	\curbit, #1
	mov	\divisor, \divisor << \result
	mov	\curbit, \curbit << \result
	mov	\result, #0
	
	@ Division loop
1:	cmpsub.a	\dividend, \divisor
	bub	201f
	sub	\dividend, \dividend, \divisor
	or	\result,   \result,   \curbit
201:
	cmpsub.a	\dividend, \divisor >> #1
	bub	201f
	sub	\dividend, \dividend, \divisor >> #1
	or	\result,   \result,   \curbit >> #1
201:
	cmpsub.a	\dividend, \divisor >> #2
	bub	201f
	sub	\dividend, \dividend, \divisor >> #2
	or	\result,   \result,   \curbit >> #2
201:
	cmpsub.a	\dividend, \divisor >> #3
	bub	201f
	sub	\dividend, \dividend, \divisor >> #3
	or	\result,   \result,   \curbit >> #3
201:
	cmpsub.a	\dividend, #0			@ Early termination?
	cmovne.a	\curbit,   \curbit >> #4	@ No, any more bits to do?
	cmovne	\divisor,  \divisor >> #4
	bne	1b

.endm


.macro UC_DIV2_ORDER divisor, order

	cntlz	\order, \divisor
	rsub	\order, \order, #31

.endm


.macro UC_MOD_BODY dividend, divisor, order, spare

	cntlz	\order, \divisor
	cntlz	\spare, \dividend
	sub	\order, \order, \spare
	mov	\divisor, \divisor << \order

	@ Perform all needed substractions to keep only the reminder.
	@ Do comparisons in batch of 4 first.
	sub.a	\order, \order, #3		@ yes, 3 is intended here
	bsl	2f

1:	cmpsub.a	\dividend, \divisor
	bub	201f
	sub	\dividend, \dividend, \divisor
201:
	cmpsub.a	\dividend, \divisor >> #1
	bub	201f
	sub	\dividend, \dividend, \divisor >> #1
201:
	cmpsub.a	\dividend, \divisor >> #2
	bub	201f
	sub	\dividend, \dividend, \divisor >> #2
201:
	cmpsub.a	\dividend, \divisor >> #3
	bub	201f
	sub	\dividend, \dividend, \divisor >> #3
201:
	cmpsub.a	\dividend, #1
	mov	\divisor, \divisor >> #4
	bsl	201f
	sub.a	\order, \order, #4
201:
	beg	1b

	cmpand.a	\order, #3
	beq	5f
	cmpxor.a	\dividend, #0
	beq	5f

	@ Either 1, 2 or 3 comparison/substractions are left.
2:	cmpadd.a	\order, #2
	bsl	4f
	beq	3f
	cmpsub.a	\dividend, \divisor
	bub	201f
	sub	\dividend, \dividend, \divisor
201:
	mov	\divisor,  \divisor >> #1
3:	cmpsub.a	\dividend, \divisor
	bub	201f
	sub	\dividend, \dividend, \divisor
201:
	mov	\divisor,  \divisor >> #1
4:	cmpsub.a	\dividend, \divisor
	bub	5f
	sub	\dividend, \dividend, \divisor
5:
.endm


ENTRY(__udivsi3)

	sub.a	r2, r1, #1
	cmoveq	pc, lr
	bub	Ldiv0
	cmpsub.a	r0, r1
	beb	11f
	cmpand.a	r1, r2
	beq	12f

	UC_DIV_BODY r0, r1, r2, r3

	mov	r0, r2
	mov	pc, lr

11:	cmoveq	r0, #1
	cmovne	r0, #0
	mov	pc, lr

12:	UC_DIV2_ORDER r1, r2

	mov	r0, r0 >> r2
	mov	pc, lr

ENDPROC(__udivsi3)

ENTRY(__umodsi3)

	sub.a	r2, r1, #1			@ compare divisor with 1
	bub	Ldiv0
	beq	201f
	cmpsub.a	r0, r1			@ compare dividend with divisor
201:
	cmoveq   r0, #0
	beb	201f
	cmpand.a	r1, r2			@ see if divisor is power of 2
201:
	bne	201f
	and	r0, r0, r2
201:
	cmoveb	pc, lr

	UC_MOD_BODY r0, r1, r2, r3

	mov	pc, lr

ENDPROC(__umodsi3)

ENTRY(__divsi3)

	cmpsub.a	r1, #0
	xor	ip, r0, r1			@ save the sign of the result.
	beq	Ldiv0
	bns	201f
	rsub	r1, r1, #0			@ loops below use unsigned.
201:
	sub.a	r2, r1, #1			@ division by 1 or -1 ?
	beq	10f
	mov.a	r3, r0
	bns	201f
	rsub	r3, r0, #0			@ positive dividend value
201:
	cmpsub.a	r3, r1
	beb	11f
	cmpand.a	r1, r2			@ divisor is power of 2 ?
	beq	12f

	UC_DIV_BODY r3, r1, r0, r2

	cmpsub.a	ip, #0
	cmovns	pc, lr
	rsub	r0, r0, #0
	mov	pc, lr

10:	cmpxor.a	ip, r0			@ same sign ?
	cmovns	pc, lr
	rsub	r0, r0, #0
	mov	pc, lr

11:	cmovub	r0, #0
	cmoveq	r0, ip |> #31
	cmovne	pc, lr
	or	r0, r0, #1
	mov	pc, lr

12:	UC_DIV2_ORDER r1, r2

	cmpsub.a	ip, #0
	mov	r0, r3 >> r2
	cmovns	pc, lr
	rsub	r0, r0, #0
	mov	pc, lr

ENDPROC(__divsi3)

ENTRY(__modsi3)

	cmpsub.a	r1, #0
	beq	Ldiv0
	bns	201f
	rsub	r1, r1, #0			@ loops below use unsigned.
201:
	mov.a	ip, r0				@ preserve sign of dividend
	bns	201f
	rsub	r0, r0, #0			@ if negative make positive
201:
	sub.a	r2, r1, #1			@ compare divisor with 1
	beq	201f
	cmpsub.a	r0, r1			@ compare dividend with divisor
201:
	cmoveq	r0, #0
	beb	201f
	cmpand.a	r1, r2			@ see if divisor is power of 2
201:
	bne	201f
	and	r0, r0, r2
201:
	beb	10f

	UC_MOD_BODY r0, r1, r2, r3

10:	cmpsub.a	ip, #0
	cmovns	pc, lr
	rsub	r0, r0, #0
	mov	pc, lr

Ldiv0:

	stw.w	lr, [sp+], #-8
	b.l	__div0
	mov	r0, #0			@ About as wrong as it could be.
	ldw.w	pc, [sp]+, #8

ENDPROC(__modsi3)

