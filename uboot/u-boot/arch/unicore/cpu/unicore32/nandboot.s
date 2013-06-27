.text
.global boot
boot:
	movl sp, #0xb2002000

@CLK
@=======================config PMU===========================
@config of pll
    ldw r0, =0xb0001004
@   ldw r1, =0x0000C400 @apll 600MHz
@   ldw r1, =0x00010816 @apll 100MHz
    ldw r1, =0x00010810 @apll 800MHz
    stw    	r1, [r0]
    
    ldw r0, =0xb0001008
@   ldw r1, =0x00010814 @mpll 200MHz
@   ldw r1, =0x00010816 @mpll 100MHz
    ldw r1, =0x0000EC14 @mpll 180MHz
    stw   	r1, [r0]
    

    ldw r0, =0xb000100c
@   ldw r1, =0x00010814 @dpll 200M
@   ldw r1, =0x00006000 @dpll 300M
    ldw r1, =0x00010812 @dpll 400M
    stw   	r1, [r0]
    
    ldw r0, =0xb0001000
    ldw r1, =0x00000007 
    stw   	r1, [r0]
LOOP1: 
    ldw r0, =0xb000105c
    ldw r1, [r0]
    cmpsub.a	r1, #0x7
    bne LOOP1

@div bus5 to 45MHz
	ldw	r0,	=0xb0001020 
    ldw r1, =0xc0
    stw r1, [r0]
    
	ldw	r0,	=0xb0001034
    ldw r1, =0x02
    stw r1, [r0]
LOOP2: 
    ldw r0, =0xb0001034
    ldw r1, [r0]
    cmpsub.a	r1, #0x0
    bne LOOP2

@open all the CLKGT
    ldw	r0, =0xb0001014
    ldw	r1, =0xffffffff
    stw	r1, [r0]
	
    ldw	r0, =0xb0001018
    ldw	r1, =0x3ff
    stw	r1, [r0]

@DDR
@=======================config SYSYCTRL==========================
    ldw r0, =0xb0008014
    ldw r2, =0xff000000
	ldw r1, [r0]
	or 	r1, r1,r2
   	stw r1, [r0]

    ldw r0, =0xb0008010
    ldw r1, [r0]
    ldw r2, =0xffffff00
    and r1, r1, r2
    or 	r1, r1, #0xfc
    stw r1,[r0]
@=======================config DDR2CTRL==========================
    ldw	r0, =0xb3000008
    ldw	r1, =0x3
    stw	r1, [r0]

    ldw	r0, =0xb30000a8
    ldw	r1, =0x100085ef
    stw	r1, [r0]

    ldw	r0, =0xb3000098
    ldw	r1, =0x84218421
    stw	r1, [r0]

    ldw	r0, =0xb3000000
    ldw	r1, =0x00020104
    stw	r1, [r0]

    ldw	r0, =0x33000004
    ldw	r1, =0x000001c8 @2*1Gbx16
    stw	r1, [r0]

    ldw	r0, =0xb3000004
    ldw	r1, [r0]
    or	r1, r1, #0x1000000
    stw	r1, [r0]

ddr_loop3:
    ldw	r0, =0xb30003fc
    ldw	r1, [r0]
    and	r1, r1, #5
    cmpsub.a	r1, #4
    bne	ddr_loop3
    
    ldw	r0, =0xb30000a4
    ldw	r1, =0x4ffffff0

    ldw r0, =0x30008010
    ldw r1, [r0]
    ldw r2, =0xffe0ffff
    and r1, r1, r2
    stw r1, [r0]
    or 	r1, r1, #0x001f0000
    stw r1, [r0]

back_datatraining:
    ldw	r0, =0xb3000000
    ldw	r1, [r0]
    ldw	r2, =0x40020000
    or 	r1, r1, r2
    stw	r1, [r0]
    
    ldw	r3, =0x3f000
datatraining_loop:
    sub	r3, r3,  #1
    cmpsub.a	r3,  #1
    bne	datatraining_loop
    
    ldw	r0, =0xb300000c
    ldw	r1, [r0]
    mov.a	r3,  #0x100000
    and	r1, r1, r3
    cmpsub.a r1,  r3 
    bne	ddr_datatraining
    b 	error

ddr_datatraining:
    ldw	r0, =0xb30003fc
    ldw	r1, [r0]
    and	r1, r1, #5
    cmpsub.a	r1, #4
    bne	ddr_datatraining

	ldw	r0, =0xb3000000
    ldw	r1, [r0]
    or	r1, r1, #0x00000004
    stw	r1, [r0]

go_nfc:
	nop
	nop
	nop
	ldw lr, =go_main
	ldw pc, =nfc_init

go_main:
	nop
	nop
	nop
	nop
	@jump to C language main function
	ldw r0, =0x40400000
	ldw lr, go_uboot
	ldw pc, =read_uboot
go_uboot:
	nop
	nop
	nop
b go_uboot

error:
    ldw r1, =0x31000000
    mov.a	r0, #0x65
    stw r0, [r1]
    b	back_datatraining 
