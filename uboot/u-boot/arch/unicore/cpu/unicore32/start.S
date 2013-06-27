/*
 *  unicoreboot - Startup Code for UNICORE-II CPU-core
 *
 *  Copyright (c) 2009	Tony Hook <suyonggang@pkunity.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *********************************************************************
 * Author: chinawrj chinawrj@gmail.com, SEUIC
 * TODO:fix dead_loop to display some information through the UART
 *      Add detect for booting from NORFLASH, Now we can boot from
 *      Nand and sdcard
 *********************************************************************
 */

#include <config.h>
#include <version.h>

.globl _start
_start:
	b	reset
	ldw	pc, _extend_handle
	ldw	pc, _swi_handle
	ldw	pc, _iabort_handle
	ldw	pc, _dabort_handle
	ldw	pc, _reserve_handle
	ldw	pc, _IRQ_handle
	ldw	pc, _FIQ_handle

_extend_handle:		.word extend_handle
_swi_handle: 		.word swi_handle
_iabort_handle: 	.word iabort_handle
_dabort_handle:		.word dabort_handle
_reserve_handle:	.word reserve_handle
_IRQ_handle:		.word IRQ_handle
_FIQ_handle:		.word FIQ_handle
	.balignl 16,0xdeadbeef


.global _TEXT_BASE
_TEXT_BASE:
	.word	CONFIG_SYS_TEXT_BASE	@ load address

.globl _unicoreboot_start
_unicoreboot_start:
	.word	_start			@ u-boot run address

.globl _bss_start
_bss_start:
	.word	__bss_start		@ load end address

.globl _bss_end
_bss_end:
	.word	_end

.globl IRQ_STACK_START
IRQ_STACK_START:
	.word	0x0badc0de

.globl FIQ_STACK_START
FIQ_STACK_START:
	.word	0x0badc0de
	

/*
 *************************************************************************
 *
 * Startup Code (called from the UNICORE reset exception vector)
 *
 * do important init only if we don't start from memory!
 * relocate unicoreboot to ram
 * setup stack
 * jump to second stage
 *
 *************************************************************************
 */

/*
 * the actual start code
 */



reset:
@Are we in the ESRAM?
@If not, skip copy uboot
	mov	r0, pc
	mov.a r0, r0 >> #16
	bne	skip_uboot_copy



lowlevel_init:

@CLK
@=======================config PMU===========================
@config of pll
     ldw     r0, =0xb0001004
@    ldw     r1, =0x0000C400     @apll 600MHz
@    ldw     r1, =0x00010816     @apll 100MHz    high_band 8div NR = 1 
     ldw     r1, =0x00010810     @apll 800MHz
@    ldw     r1, =0x00013c10     @apll 960MHz
@    ldw     r1, =0x00012810     @apll 900MHz
@    ldw     r1, =0x00010812     @apll 402M      high_band 2div NR = 1  
@    ldw     r1, =0x0000C412     @apll 300M      high_band 2div NR = 1 
@    ldw     r1, =0x00010414     @apll 198M      high_band 4div NR = 1 
@    ldw     r1, =0x0000a000     @apll 492M ligh_band 4div NR = 1 
@    ldw     r1, =0x0000a400      @apll 500M
     stw      r1, [r0]
    
     ldw     r0, =0xb0001008
@     ldw     r1, =0x00013c12     @mpll 480MHz
@    ldw     r1, =0x00006000     @mpll 300MHz
@    ldw     r1, =0x00012414     @mpll 220MHz
@    ldw     r1, =0x00010814     @mpll 200MHz
@    ldw     r1, =0x00010816     @mpll 100MHz
    ldw     r1, =0x0000EC14     @mpll 180MHz
@    ldw     r1, =0x0000F814     @mpll 189M      high_band 4div NR = 1
@    ldw     r1, =0x0000DC14     @mpll 168M      high_band 4div NR = 1
@    ldw     r1, =0x0000D014     @mpll 159M      high_band 4div NR = 1
@    ldw     r1, =0x0000c414     @mpll 150M      high_band 4div NR = 1
@    ldw     r1, =0x0000b414     @mpll 138M      high_band 4div NR = 1
@    ldw     r1, =0x0000a814     @mpll 129M      high_band 4div NR = 1
@    ldw     r1, =0x00009c14     @mpll 120M      high_band 4div NR = 1
@    ldw     r1, =0x00008c14     @mpll 108M      high_band 4div NR = 1 
     stw      r1, [r0]
    

     ldw     r0, =0xb000100c
@    ldw     r1, =0x00010816     @dpll 100M
@    ldw    r1, =0x00010814     @dpll 200M
@    ldw     r1, =0x00006000     @dpll 300M
@    ldw     r1, =0x0000f812     @dpll 378M
@    ldw     r1, =0x00010012     @dpll 390M
@    ldw     r1, =0x00010412     @dpll 396M
@    ldw     r1, =0x00010c12     @dpll 408M
     ldw     r1, =0x00010812     @dpll 402M
@    ldw     r1, =0x00011012     @dpll 400M
@    ldw     r1, =0x0000a400      @dpll 500M

     stw      r1, [r0] 
    

    ldw     r0, =0xb0001000
    ldw     r1, =0x00000007     @clock Enable
    stw      r1, [r0] 

 @AHB APB div 
    ldw  r0,=0xb0001020
@    ldw	 r1,=0xC1     @ BUS5/4 AHB/2 
    ldw	 r1,=0x80     @ BUS5/3 AHB/1
    stw  r1,[r0]

    ldw  r0,=0xb0001034    @DIV trigger
    ldw	 r1,=0x1f
    stw  r1,[r0]

LOOP1: 
    ldw		r0,=0xb000105c
    ldw		r1,[r0]
    cmpsub.a	r1,#0x7
    bne		LOOP1

@open all the CLKGT
    ldw	r0, =0xb0001014
    ldw	r1, =0xffffffff
    stw	r1, [r0]
	
    ldw	r0, =0xb0001018
    ldw	r1, =0xdff
    stw	r1, [r0]

@DDR
@ddr EMR2
	ldw	r0,=0x330001f8
	ldw	r1,=0x80
	ldw	r2,[r0]
	or	r2,r2,r1
	stw	r2,[r0]

@ddr2 MR0  BL= quen  DLL_reset
    ldw	r0,=0x330001f0
	ldw	r1,=0x952
	ldw	r2,[r0]
	or	r2,r2,r1
    ldw r1,=0xfffff952
    and r2,r2,r1
	stw	r2,[r0]

@ddr2 MR0  BL = interleave  No_DLL_reset
@    ldw	r0,=0x330001f0
@	ldw	r1,=0x85a
@	ldw	r2,[r0]
@	or	r2,r2,r1
@    ldw r1,=0xfffff85a
@    and r2,r2,r1
@	stw	r2,[r0]


@ddr2 MR1  Enable_dll
    ldw	r0,=0x330001f4
	ldw	r1,=0xffffffc6
	ldw	r2,[r0]
    and r2,r2,r1
	stw	r2,[r0]
	
@ triggle init
    ldw	r0,=0x33000000
	ldw	r1,=0x80000000
	ldw	r2,[r0]
	or	r2,r2,r1
	stw	r2,[r0]

	ldw r0,=0xffff		
	loopij:				
	sub	r0,r0,#0x1		
	cmpsub.a r0,#0x0		
	bne	loopij	
	
	

ok:   @open lower power of DDR
   ldw r0,=0x30008014
   ldw r2,=0xff000000
   ldw r1,[r0]
   or r1,r1,r2
   stw r1,[r0]


@
ok2:
    ldw r0,=0x30008010
    ldw r1, [r0]
    ldw r2, =0xffffff00
    and r1, r1, r2
    or r1, r1, #0xfc
    stw r1,[r0]

@open ODT
    ldw    r0, =0xc000003
    ldw    r1, =0x33000008     @open ODT
    stw    r0, [r1]

    ldw    r0, =0x84218421
    ldw    r1, =0x33000098     @ODT write&read
   stw    r0, [r1]

b come_on
.word 0x00000000
.word 0x3c060101@0x1C0
.word 0x0ec6463e
.word 0x0a240000
.word 0x00000004
.word 0x3c832b41@0x1D0
.word 0x4142307e
.word 0x58a40011
.word 0x00000000
.word 0x00000000@0x1E0
.word 0x00000000
.word 0x00000000
.word 0x00000000
.word 0x00000000@0x1F0
.word 0x00000000
.word 0x00000000
.word 0xaa550000
come_on:


    ldw    r0, =0x100085ef
    ldw    r1, =0x330000a8     @ODT×èÖµ,N=1£¬×î¸ß
    stw    r0, [r1]


@ddr
@    ldw     r0, =0xb000801c
@    ldw     r1, =0x40
@    stw     r1, [r0]

     ldw     r0, =0xb3000000
     ldw     r1, =0x00020104
     stw     r1, [r0]

     ldw     r0, =0x33000004
@    ldw     r1, =0x000001b4		    @4*512Mbx8
@    ldw     r1, =0x000001c4		@4*1Gbx8
@    ldw     r1, =0x000001d4        @4*2Gbx8
@    ldw     r1, =0x000001b8		@2*512Mbx16
@    ldw     r1, =0x0000021c8		@2*1Gbx16
     ldw     r1, =0x0000020c8           @1*1Gbx16
@     ldw     r1, =0x000000c8		@2*1Gbx16
@    ldw     r1, =0x000009d8		@2*2Gbx16,2ranks
    stw     r1, [r0]

    ldw     r0, =0xb3000004
    ldw     r1, [r0]
    or      r1, r1, #0x1000000
    stw     r1, [r0]

@CL
    ldw r1,=0x3300001c    
    ldw r0,=0x19a1    
    stw r0,[r1]

@TPR0       tRP:8
@   ldw r1,=0x33000014    
@    ldw r0,[r1]
@    or  r0, r0, #0x800
@    ldw r2,=0xfffff8ff
@    and r0,r0,r2
@    stw r0,[r1]


@TPR1       
@    ldw r1,=0x33000018    
@    ldw r0,[r1]
@    or  r0, r0, #0x1
@    ldw r2,=0xfffffffd
@    and r0, r0,r2
@    stw r0,[r1]

@config DRR(DDR)
@	ldw	r0,=0x33000010
@	ldw	r1,=0xff0000ff
@	ldw	r2,[r0]
@	and	r2,r2,r1
@	ldw	r1,=0xff00
@	or	r2,r2,r1
@	stw	r2,[r0]


@@@@@@@@@@@@@@@@@@@@@@@
@config TPR0
@	@ldw  r0,=0x33000014		
@	@ldw  r1,=0x0000ffff		
@	@ldw  r2,[r0]			
@	@and   r2,r2,r1			
@	@ldw  r1,=0xd51f0000		
@	@or   r2,r2,r1			
@	@stw  r2,[r0]			
@@@@@@@@@@@@@@@@@@@@@

   ldw   r3,  =0x3f000
datatraining_loop_s:
    sub     r3,  r3,  #1
    cmpsub.a     r3,  #1
    bne    datatraining_loop_s


    ldw r0,=0x30008010    
    ldw r1, [r0]
   ldw r2, =0xffe0ffff
    and r1, r1, r2
    stw r1,[r0]
    ldw   r3,  =0x3f000
datatraining_loop_sf:
    sub     r3,  r3,  #1
    cmpsub.a     r3,  #1
    bne    datatraining_loop_sf

    or r1, r1, #0x001f0000
    stw r1, [r0]

    ldw   r3,  =0x3f000
datatraining_loop_sff:
    sub     r3,  r3,  #1
    cmpsub.a     r3,  #1
    bne    datatraining_loop_sff


back_datatraining:
    mov.a   r9,  #0
    ldw     r0, =0xb3000000
    ldw     r1, [r0]
    ldw     r2,=0x40020000
    or      r1, r1, r2
    stw     r1, [r0]
    
    ldw   r3,  =0x3f000
datatraining_loop:
    sub     r3,  r3,  #1
    cmpsub.a     r3,  #1
    bne    datatraining_loop
    
ddr_datatraining_3fc:
    ldw     r0, =0xb30003fc
    ldw     r1, [r0]
    and     r1, r1, #1
    cmpsub.a     r1, #1
    beq     ddr_datatraining_3fc

    ldw     r0, =0xb30003fc
    ldw     r1, [r0]
    and     r1, r1, #2
    cmpsub.a     r1, #2
    beq    fcwrong
    bne    training 
fcwrong:
    mov.a   r9,   #1	
    ldw r1,=0x31000000
    mov.a r0,#0x66
    stw r0,[r1]
training:   
    ldw     r0, =0xb300000c
    ldw     r1, [r0]
    mov.a   r3,  #0x100000
    and     r1,r1,r3
    cmpsub.a  r1,  r3 
    bne    print
    b 	error

print:
    
    cmpsub.a    r9,  #1
    beq  back_datatraining

    mov.a  r9,  #0
    ldw     r0, =0xb3000000
    ldw     r1, [r0]
    or      r1, r1, #0x00000004
    stw     r1, [r0]

@@gpio_back:
@    ldw r1,=0xB0006074    @gpio_back
@    ldw r0,=0x00000fff
@    stw r0,[r1]

@@POWER HOLD
@ldw r0 , =0xb0005114
@ldw r1 , =0x7f
@stw r1 , [r0]
@ldw r0 , =0xb0005110
@ldw r1 , =0xfb
@stw r1 , [r0]
@ldw r0 , =0xb0005118
@ldw r1 , =0x04
@stw r1 , [r0]

@	b.l		sd2ddr
@	nop
@    nop
@	nop
@	ldw		pc, =0xb2002000
@	nop
@	nop
@	nop
@	nop


/*************************************
*prepare stack point for C function
*************************************/
	movl sp, #0x32008000
	
go_sd:
	b.l sd2ddr 
	nop
	nop
	nop
	ldw pc, =0x40500000
/****************************************
*Dead loop! You should never reach here
****************************************/
dead_loop:
	nop
b dead_loop

error:
    ldw r1,=0x31000000
    mov.a r0,#0x69
    stw r0,[r1]
    b	back_datatraining 
@@@@ldw	r0, =0xb0008000
@@@@ldw	r1, [r0]	
@@@@or	r1, r1, #0x80000000
@@@@stw	r1, [r0]

skip_uboot_copy:
	mov r0, pc
	mov r0, r0 >> #20
	cmpsub.a r0, #0x404
	beq skip_ram_relocate

	ldw	r12, =0x80000
	ldw	r0, = 0x40500000
	ldw	r1, = 0x40400000
ram_relocate:
	ldw	r2, [r0]
	stw	r2, [r1]
	add	r0, r0, #4
	add	r1, r1, #4
	sub	r12, r12, #4
	cmpsub.a	r12, #0
	bne	ram_relocate

	ldw pc, =0x40400000

	

skip_ram_relocate:
	ldw	r12, =0x100
	ldw	r0, = 0x40400000
	ldw	r1, = 0x00000000
copy_vetor:
	ldw	r2, [r0]
	stw	r2, [r1]
	add	r0, r0, #4
	add	r1, r1, #4
	sub	r12, r12, #4
	cmpsub.a	r12, #0
	bne	copy_vetor

@disable the irq
    mov	r0, asr
	or	r0, r0, #0x80
	mov.a	asr, r0

@irq stack
	mov	r1, asr
	andn	r1, r1, #0x1f
	or	r1, r1, #0x12	@irq mode
	mov.a	asr, r1
	movl	sp, #0x32008000
	sub	sp, sp, #0x4
	
@svc stack
	mov 	r1, asr
	andn	r1, r1, #0x1f
	or	r1, r1, #0x13	@svc mode
	mov.a	asr, r1
	movl	sp, #0x32017ff0

@enalbe irq
   	mov     r0, asr
   	andn    r0, r0, #0x80
   	mov.a   asr, r0


@	nop
@	nop
@	nop
@	ldw	r0, _TEXT_BASE			@ upper 128 KiB: relocated uboot
@	sub	r0, r0, #CONFIG_SYS_MALLOC_LEN		@ malloc area
@	sub	r0, r0, #CONFIG_SYS_GBL_DATA_SIZE	@ bdinfo
@#ifdef CONFIG_USE_IRQ
@	sub	r0, r0, #(CONFIG_STACKSIZE_IRQ+CONFIG_STACKSIZE_FIQ)
@#endif
@	sub	sp, r0, #12			@ leave 3 words for abort-stack

@@I/DCACHE INVALIDATE&ENABLE
    	b.l	invalidate_cache
	b.l	enable_cache

clear_bss:
	ldw	r0, _bss_start			@ find start of bss segment
	ldw	r1, _bss_end			@ stop here
	mov	r2, #0x00000000			@ clear

clbss_l:stw	r2, [r0]			@ clear loop...
	add	r0, r0, #4
	cmpsub.a	r0, r1
	bel	clbss_l

@    IMPORT	int_vector_handler
    	ldw	r0,=int_vector_handler
    	ldw	r1,=0x30000070
    	stw	r0,[r1]
    	stw	r0,[r1+],#4

	ldw	pc, _start_unicoreboot

_start_unicoreboot:
	.word start_unicoreboot

	nop
	nop
	nop
	nop
	nop
	nop



/*
 * exception handlers
 */
extend_handle:
	mov.a	pc,lr
swi_handle:
	mov.a	pc,lr
iabort_handle:
	mov.a	pc,lr
dabort_handle:
	mov.a	pc,lr
reserve_handle:
	mov.a	pc,lr
IRQ_handle:
	stm.w (r10 - r15), [sp]-   
	stm.w (lr), [sp]-   
	
	ldw	lr, =int_return
	ldw	pc, _do_irq
_do_irq:
	.word do_irq
int_return:
	nop
	nop
	nop

	ldm.w (lr), [sp+]
	ldm.w (r10 - r15), [sp+]
	mov.a pc, lr


FIQ_handle:
	mov.a	pc,lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@ Invalidate I&D Cache function call			@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
invalidate_cache:
	stm.w	(r1),[sp-]
	movc	p0.c5, r1, #28
	mov 	r1,r1
	mov 	r1,r1
	mov 	r1,r1
	mov 	r1,r1
	mov 	r1,r1
	mov 	r1,r1
	mov 	r1,r1
	ldm.w	(r1),[sp]+
	mov	pc,lr
	nop	
	nop

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@ Enable I&D Cache function call			@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
enable_cache:
	stm.w	(r1),[sp-]
	movc	r1, p0.c1, #0
	or	r1,r1,#0x0c               @#0x1C
	movc	p0.c1, r1, #0
	mov 	r1,r1
	mov 	r1,r1
	mov 	r1,r1
	mov 	r1,r1
	mov 	r1,r1
	mov 	r1,r1
	mov 	r1,r1
	ldm.w	(r1),[sp]+
	mov	pc,lr
	nop	
	nop

