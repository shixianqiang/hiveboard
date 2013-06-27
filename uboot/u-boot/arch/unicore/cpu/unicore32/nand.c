/*
 * arch/unicore/cpu/unicore32/nand.c
 * Code specific to ASIC center of SouthEast University
 * 
 * Maintained by SEUIC
 * 
 * Copyright (C) 2001-2011 ASIC & SouthEast University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Contributors & Additions/Fixes:
 * V1.1 2011-6-8 chinawrj
 * 	  add nand read function. The origin source come from
 * 	  NANDCopy written by fjj@seuic
 * 
 * TODO:
 */
#include "nand_common.h"
#include "nand_reg.h"
#include "nand.h"

#define	 BUS2_CLK_FREQ	240000000	//180MHz

static struct nfc_info *nfc = (void *)(0x32010000);

static inline void hang(void)
{
	while(1);
}		

static inline void send_colrow_addr(int col, int row)
{
	int i;

#define 	COL_CYCLE	2
#define 	ROW_CYCLE	3
	/* Write column address */
	if (col != -1) {
		for (i = 0; i < COL_CYCLE; i++) {
			writel(col & 0xff, NAND_ADDR);
			col >>= 8;
		}
	}	
	/* Write row address */
	if (row != -1) {			
		for (i = 0; i < ROW_CYCLE; i++) {
			writel(row & 0xff, NAND_ADDR);
			row >>= 8;
		}
	}
}

static inline void read_dummy_data_per_step(int step)
{
	int i;


	if (nfc->bus_type == SINGLE_CHAN_8BITS) {
		for (i = 0; i < 4; i++)
			readb(NAND_SDATA);
	} else {
		for (i = 0; i < 2; i++)
			readw(NAND_SDATA);
	}
}

static inline void read_ecc_code_per_step(int step)
{
	int i;

	switch (nfc->ecc_mode) {
		case ECC_MODE16: {
			if (nfc->bus_type == SINGLE_CHAN_8BITS) {
				for (i = 0; i < 28; i++) 
					readb(NAND_SDATA);	
			} else {
				for (i = 0; i < 14; i++) 
					readw(NAND_SDATA);	
			}
			break;
		}

		case ECC_MODE24: {
			if (nfc->bus_type == SINGLE_CHAN_8BITS) {
				for (i = 0; i < 42; i++) 
					readb(NAND_SDATA);	
			} else {
				for (i = 0; i < 21; i++) 
					readw(NAND_SDATA);	
			}
			break;
		}
		
		default: {	//nfc.ecc_mode = ECC_MODE30
			if (nfc->bus_type == SINGLE_CHAN_8BITS) {
				for (i = 0; i < 52; i++) 
					readb(NAND_SDATA);	
			} else {
				for (i = 0; i < 26; i++) 
					readw(NAND_SDATA);	
			}
			break;
		}			
	}
}

static inline void ecc_correct_by_step(unsigned char *buf)
{
	unsigned short *databuf = (unsigned short *)buf;
	unsigned long error_sum, error_reg, error_word, error_bit;
	int i;

	if (readl(NAND_STATUS) & (1 << 10)) {				
		hang();
	} else {
  		error_sum = (readl(NAND_STATUS) >> 12) & 0x1f;
		if (error_sum) {					
			for (i = 0; i < error_sum; i++) {
				error_reg = readl(NAND_ERR_ADDR0 + (i << 2));
				error_word = error_reg & 0x3ff; 
				error_bit = (error_reg >> 10) & 0xffff;
				if (error_word < 512) 				
					databuf[error_word] ^= error_bit;  			
			}		
		}
	}
}

static inline void read_page(unsigned char *read_buf, int pagenr)
{	
	int i, step, offset;
	unsigned short *p = (unsigned short *)read_buf;

	writeb(0x00, NAND_CMD);	
	send_colrow_addr(0, pagenr);
	writeb(0x30, NAND_CMD);	

   	/* Wait for data transfer from cell to register */
	while (!(readl(NAND_STATUS) & 0x1));

	for (step = 0; step < nfc->ecc_steps; step++) {	
		/* Open ecc module */
		writel(readl(NAND_CFG) & (~(1 << 29)), NAND_CFG);
		/* Init ecc module */				
		writel(0x1, NAND_INIT);
					
		/* Read 1024 bytes data */
		if (nfc->bus_type == SINGLE_CHAN_8BITS) {
			offset = step << 10;
			for (i = 0; i < 1024; i++) 
				read_buf[offset + i] = readb(NAND_SDATA);
		} else {
			offset = step << 9;
			for (i = 0; i < 512; i++) 
				p[offset + i] = readw(NAND_SDATA);
		}
	
		/* Read 4 bytes dummy data */
		read_dummy_data_per_step(step);			

		/* Read ecc code */
		read_ecc_code_per_step(step);				

		/* Wait ecc decoder completed */
		while (!(readl(NAND_STATUS) & (1 << 8)));
		writel(readl(NAND_STATUS) | (1 << 8), NAND_STATUS);
		/* Close ecc module */
		writel(readl(NAND_CFG) | (1 << 29), NAND_CFG);

		/* Correct data */
		ecc_correct_by_step(&read_buf[step << 10]);	
	}
}

void nfc_init(void)
{	
	unsigned long Value_Nfc_Cfg;

	if ((*(volatile unsigned int*)0xb000104c) == 0x1) 
	{
		int i;
		unsigned int *save2DDR;


//uart_init
		__asm__(
		"	  ldw r1,=0xB0006074		   	\r\n"                   
		"    ldw r0,=0xFFFFF000			\r\n"
		"    stw r0,[r1]	\r\n"  
		
		"    ldw r1,=0x3100000C			\r\n"  
		"    mov.a r0,#0x83				\r\n"  
		"    stw r0,[r1]					\r\n"  
		
		"    mov.a r1,#0x31000000		\r\n"  
//		"    mov.a r0,#0x18    @45M		\r\n" 
		"    mov.a r0,#0x20    @60M		\r\n"  
		"    stw r0,[r1]	\r\n"  
		
		"    ldw r1,=0x3100000C			\r\n"  
		"    mov.a r0,#0x03				\r\n" 
		"    stw  r0,[r1]				\r\n"


		"   ldw r1,=0x31000000			\r\n"
		"   ldw r0,=0x35					\r\n"
		"    stw r0,[r1]					\r\n"  
		"    stw r0,[r1]					\r\n"  
		"    stw r0,[r1]					\r\n"  
		"    stw r0,[r1]					\r\n"  
		"    stw r0,[r1]					\r\n"  
		"    stw r0,[r1]					\r\n"  
		"    stw r0,[r1]					\r\n"  
		"    stw r0,[r1]					\r\n"  
		"    stw r0,[r1]					\r\n"  
		"dead:    stw r0,[r1]			\r\n"  
	
   			 );	

		__asm__(
		"   ldw r1,=0x31000000		\r\n"
		"   ldw r0,=0x34				\r\n"
		"    stw r0,[r1]				\r\n"  
		"    stw r0,[r1]				\r\n"  
		"    stw r0,[r1]				\r\n"  
		"    stw r0,[r1]				\r\n"  
		"dead1:    stw r0,[r1]		\r\n"  		
				);

		save2DDR = 0x40505000;//reserve base changed to 0x40505000
		for (i = 0; i<0x150; i++)
		{
			*(volatile unsigned int *)((0xb0003000)+i*4) = *(volatile unsigned int *)(save2DDR+i*4) ;
		}

		__asm__(
		"ldw r2, =0x404ffffc		        \n" //ddr reserve address 40500000

//store asr                                      
      	"add r2, r2, #4		         	 \n"
      	"ldw r3, [r2+]	               \n"
		"mov.a asr, r3					 \n"
                                                 
//store r0-r30                                   
      	"add r2, r2, #4		        	 \n"
      	"ldm.w (r4-r15), [r2+]			 \n"
      	"ldm.w (r16-r28), [r2+]         \n"
                                                 
//Save MODE registers                          
      	"add r2, r2, #4                 \n"
		"ldw r0, =0xFFFFFFF0            \n"
		"and r3, r3, r0                 \n"
		"or r3, r3, #0x1                \n"
		"mov.a asr, r3		@real   	 \n"
		"ldm.w (r29-r30), [r2+]         \n"
		"or r3, r3, #0x2                \n"
		"mov.a asr, r3		@priv 		 \n"
		"ldm.w (r29-r30), [r2+]         \n"
		"or r3, r3, #0x4                \n"
		"mov.a asr, r3		@trap      \n"
		"ldm.w (r29-r30), [r2+]         \n"
		"or r3, r3, #0x8                \n"
		"mov.a asr, r3		@prot       \n"
		"ldm.w (r29-r30), [r2+]         \n"
		"and r3, r3, r0                 \n"
		"or r3, r3, #0x2                \n"
		"mov.a asr, r3		@int       \n"
		"ldm.w (r29-r30), [r2+]         \n"
		"or r3, r3, #0x9                \n"
		"mov.a asr, r3		@ext    	 \n"
		"ldm.w (r29-r30), [r2+]         \n"
             
// back-to PRIV mode    
		"and r3, r3, r0                 \n"
		"or r3, r3, #0x3                \n"
		"mov.a asr, r3                  \n"
		"add r2, r2, #4			\n"
		"ldm.w (r29-r30), [r2+]			 \n"

// prepare pc (virtual addr) r0 = pc
		"ldw r3, =0xb0001098				 \n"
		"ldw r0, [r3+]					 \n"
		"add r0, r0, #0x4				 \n"

//store  p0.c2                                   
      	"add r2, r2, #4					 \n"
      	"ldw r3, [r2+]          		 \n"
		"movc p0.c2, r3, #0				 \n"

// store  p0.c1 
      	"add r2, r2, #4					\n"
      	"ldw r1, [r2+]          		\n"
      	"movc p0.c1, r1, #0    			\n"

		"mov pc, r0						\n"
		"nop								\n"
		"nop								\n"
		"nop								\n"
		"nop								\n"
		"nop								\n"
		);
		__asm__(
		"ldw r1,=0xB0006074		\r\n"                   
       "ldw r0,=0xfff			\r\n"
		"stw r0,[r1]				\r\n"  
			);

	}





	Value_Nfc_Cfg = readl(NAND_CFG);

	nfc->page_size = (Value_Nfc_Cfg >> 2) & 0x3; 
	nfc->bus_type = Value_Nfc_Cfg & 0x1 ;
	nfc->ecc_mode = (Value_Nfc_Cfg >> 27) & 0x3;
	   
	nfc->write_size = 2048 << (nfc->page_size + nfc->bus_type);
	nfc->ecc_steps = nfc->write_size >> 10;
	nfc->ecc_pos = nfc->ecc_steps << 2;
	
#define		TACLS		3
#define		TWRPH0	6
#define		TWRPH1	6

	writel(Value_Nfc_Cfg | NFC_TACLS(TACLS) | NFC_TWRPH0(TWRPH0) | NFC_TWRPH1(TWRPH1), NAND_CFG);
	
	/* Reset chip */ 
	writeb(0xff, NAND_CMD);									  		 	
	while (!(readl(NAND_STATUS) & 0x1));			   	
}

void read_uboot(void)
{	
	int i, cnt;
	unsigned long load_addr = 0x40500000;
	/* Now, We read the second block, read 320KiB */
	if (nfc->page_size == 1) {
		/* K9HCG08U1M K9LBG08U1M K9HCG08U1D K9GAG08U0M  K9GAG08U0D K9MDG08U5D
		 * page_size = 4KiB PpB = 128 
		  */
		if (nfc->bus_type) 
			cnt = 40;
		else
			cnt = 80;
		for (i = 0; i < cnt; i++) 
			read_page((unsigned char *)load_addr + nfc->write_size * i, i);		
	} else if (nfc->page_size == 2) {
		/* H27UBG8ATR 
		 * page_size = 8KiB PpB = 256 
		  */
		if (nfc->bus_type) 
			cnt = 20;
		else
			cnt = 40;
		for (i = 0; i < cnt; i++) 
			read_page((unsigned char *)load_addr + nfc->write_size * i, i);
	}
}
