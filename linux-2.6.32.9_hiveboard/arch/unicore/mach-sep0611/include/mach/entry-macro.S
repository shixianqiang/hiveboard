/* linux/arch/unicore/mach-sep0611/include/mach/entry-macro.S
 *
 * Copyright (c) 2009-2011 SEUIC
 *
 * Southeast University ASIC SoC support
 *
 * Low-level IRQ helper macros for SEP0611
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  09-04-2010	Changelog initial version
 *
 */

#include <mach/hardware.h>


	.macro  get_irqnr_preamble, base, tmp
	.endm

	.macro	arch_ret_to_user, tmp1, tmp2
	.endm

 
	.macro disable_fiq	
	.endm

    .macro get_irqnr_and_base, irqnr, irqstat, base, base2
        ldw     \base,  =VIC_MSK_IRQ_STS_L_V
        ldw     \base,  [\base]

        ldw     \base2, =VIC_MSK_IRQ_STS_H_V
        ldw     \base2, [\base2]

        cmpsub.a    \base, #0
        beq    1003f
        bne   1004f

1003:
        cmpsub.a         \base2,#0
        beq     1002f

1004:
        ldw     \irqnr, =VIC_IRQ_VECTOR_NUM_V
        ldw     \irqnr, [\irqnr]
1002:
        .endm
        .macro irq_prio_table

        .endm

