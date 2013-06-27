/* linux/drivers/mtd/nand/sep0611.c
* Copyright (c) 2011 Seuic Semiconductor Co.,Ltd
*
* sepxxxx nand driver.
* chip program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*/
#include <common.h>

#include <errno.h>

#include <linux/string.h>
#include <asm/io.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <asm/sizes.h>
#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/errno.h>
#include "sep0611.h"

#define		BUS_CLK_FREQ	240000000

/* This is the most important data structure on SEPXXXX nand driver */
struct nfc_info nfc;

#define MAX_SUPPORTED_SAMSUNG_NAND 8
const struct nand_features SAMSUNG_NAND_DevInfo[] = {
 	//*================================================================================================================================================================
    //*[             ID            ][ Name ][                     Size                      ][   Address Cycle  ][           Timing Sequence             ][ Attribute ]
    //*----------------------------------------------------------------------------------------------------------------------------------------------------------------
    //* 1st, 2nd, 3rd, 4th, 5th, 6th, name, ChipSize(MB), BlockSize(KB), PageSize, SpareSize, ColCycle, RowCycle, tCLS, tALS, tWC, tWP, tWH, tRC, tRP, tREH 
    //*================================================================================================================================================================
	// [256M] K9K2G16U0M	
	{{0xEC, 0xCA, 0x00, 0x55, 0x00, 0x00},  "Samsung NAND 256M 3.3V 8-bit",  256, 128, 2048, 64,  2, 3,  25, 25, 45, 25, 15, 50, 25, 15,  A_16BIT},
	// [512MB] K9G4G08U0B
    {{0xEC, 0xDC, 0x14, 0xA5, 0x54, 0x00},  "Samsung NAND 512M 3.3V 8-bit",  512, 256, 2048, 64,  2, 3,  15, 15, 25, 15, 10, 25, 15, 10,  A_08BIT},
    // [1GB] K9G8G08UOA 
    {{0xEC, 0xD3, 0x14, 0xA5, 0x64, 0x00},  "Samsung NAND 1G 3.3V 8-bit",  1024, 256, 2048, 64,  2, 3,  30, 30, 50, 30, 20, 50, 30, 20,  A_08BIT},
    // [2GB] K9GAG08UOM
    {{0xEC, 0xD5, 0x14, 0xB6, 0x74, 0x00},  "Samsung NAND 2G 3.3V 8-bit",  2048, 512, 4096, 128,  2, 3,  12, 12, 25, 12, 10, 25, 12, 10,  A_08BIT},
    // [4GB] K9LBG08UOM	 [8GB] K9HCG08U1M 
	{{0xEC, 0xD7, 0x55, 0xB6, 0x78, 0x00},  "Samsung NAND 4G 3.3V 8-bit",  4096, 512, 4096, 128,  2, 3,  12, 12, 25, 12, 10, 25, 12, 10,  A_08BIT},
  	// [2GB] K9GAG08UOD
    {{0xEC, 0xD5, 0x94, 0x29, 0x34, 0x41},  "Samsung NAND 2G 3.3V 8-bit",  2048, 512, 4096, 218,  2, 3,  15, 15, 30, 15, 10, 30, 15, 10,  A_08BIT},
    // [8GB] K9HCG08U1D	 [16G] K9MDG08U5D
    {{0xEC, 0xD7, 0xD5, 0x29, 0x38, 0x41},  "Samsung NAND 8G 3.3V 8-bit",  4096, 512, 4096, 218,  2, 3,  25, 25, 45, 25, 15, 50, 25, 15,  A_08BIT},	
    // [2GB] K9GAG08UOE		
    {{0xEC, 0xD5, 0x84, 0x72, 0x50, 0x42},  "Samsung NAND 2G 3.3V 8-bit",  2048, 1024, 8192, 436,  2, 3,  15, 15, 30, 15, 10, 30, 15, 10,  A_08BIT}
}; 

#define MAX_SUPPORTED_HYNIX_NAND 4
const struct nand_features HYNIX_NAND_DevInfo[] = {
 	//*================================================================================================================================================================
    //*[             ID            ][ Name ][                     Size                      ][   Address Cycle  ][           Timing Sequence             ][ Attribute ]
    //*----------------------------------------------------------------------------------------------------------------------------------------------------------------
    //* 1st, 2nd, 3rd, 4th, 5th, 6th, name, ChipSize(MB), BlockSize(KB), PageSize, SpareSize, ColCycle, RowCycle, tCLS, tALS, tWC, tWP, tWH, tRC, tRP, tREH 
    //*================================================================================================================================================================
   	// [128MB] HY27UF081G2A 
    {{0xAD, 0xF1, 0x80, 0x1D, 0x00, 0x00},  "Hynix NAND 128M 3.3V 8-bit",  128, 128, 2048, 64,  2, 2,  15, 15, 30, 15, 10, 30, 15, 10, A_08BIT},
    // [512MB] HY27UF084G2B
    {{0xAD, 0xDC, 0x10, 0x95, 0x00, 0x00},  "Hynix NAND 512M 3.3V 8-bit",  512, 128, 2048, 64,  2, 3,  12, 12, 25, 12, 10, 25, 12, 10,  A_08BIT},
    // [1GB] HY27UT088G2A
    {{0xAD, 0xD3, 0x14, 0xA5, 0x34, 0x00},  "Hynix NAND 1G 3.3V 8-bit",  1024, 256, 2048, 64,  2, 3,  12, 12, 25, 12, 10, 25, 12, 10,  A_08BIT},
    // [4GB] H27UBG8T2A
	{{0xAD, 0xD7, 0x94, 0x9A, 0x74, 0x42},  "Hynix NAND 4G 3.3V 8-bit",  2048, 2048, 8192, 448,  2, 3,  12, 12, 25, 12, 10, 25, 12, 10,  A_08BIT}
};

#define MAX_SUPPORTED_MICRON_NAND 1
const struct nand_features MICRON_NAND_DevInfo[] = {
 	//*================================================================================================================================================================
    //*[             ID            ][ Name ][                     Size                      ][   Address Cycle  ][           Timing Sequence             ][ Attribute ]
    //*----------------------------------------------------------------------------------------------------------------------------------------------------------------
    //* 1st, 2nd, 3rd, 4th, 5th, 6th, name, ChipSize(MB), BlockSize(KB), PageSize, SpareSize, ColCycle, RowCycle, tCLS, tALS, tWC, tWP, tWH, tRC, tRP, tREH 
    //*================================================================================================================================================================
  	// [4GB] 29F32G08QAA 
	{{0x2C, 0xD5, 0x94, 0x3E, 0x74, 0x00},  "Micron NAND 4G 3.3V 8-bit",  4096, 512, 4096, 218,  2, 3,  10, 10, 20, 10, 7, 25, 12, 10, A_08BIT}
};

#define MAX_SUPPORTED_TOSHIBA_NAND 2
const struct nand_features TOSHIBA_NAND_DevInfo[] = {
 	//*================================================================================================================================================================
    //*[             ID            ][ Name ][                     Size                      ][   Address Cycle  ][           Timing Sequence             ][ Attribute ]
    //*----------------------------------------------------------------------------------------------------------------------------------------------------------------
    //* 1st, 2nd, 3rd, 4th, 5th, 6th, name, ChipSize(MB), BlockSize(KB), PageSize, SpareSize, ColCycle, RowCycle, tCLS, tALS, tWC, tWP, tWH, tRC, tRP, tREH 
    //*================================================================================================================================================================
  	// [2GB] TC58NVG4D2FTA00
	{{0x98, 0xD5, 0x94, 0x32, 0x76, 0x55},  "Toshiba NAND 2G 3.3V 8-bit",  2048, 1024, 8192, 448,  2, 3,  12, 12, 25, 12, 10, 25, 12, 10, A_08BIT},
	// [4GB] TC58NVG5D2FTA00  [8GB] TH58NVG6D2FTA20
	{{0x98, 0xD7, 0x94, 0x32, 0x76, 0x55},  "Toshiba NAND 4G 3.3V 8-bit",  4096, 1024, 8192, 448,  2, 3,  12, 12, 25, 12, 10, 25, 12, 10, A_08BIT}
};

#define MAX_SUPPORTED_STMICRO_NAND 2
const struct nand_features STMICRO_NAND_DevInfo[] = {
 	//*================================================================================================================================================================
    //*[             ID            ][ Name ][                     Size                      ][   Address Cycle  ][           Timing Sequence             ][ Attribute ]
    //*----------------------------------------------------------------------------------------------------------------------------------------------------------------
    //* 1st, 2nd, 3rd, 4th, 5th, 6th, name, ChipSize(MB), BlockSize(KB), PageSize, SpareSize, ColCycle, RowCycle, tCLS, tALS, tWC, tWP, tWH, tRC, tRP, tREH 
    //*================================================================================================================================================================
   	// [512MB] NAND04GW3B2D
    {{0x20, 0xDC, 0x10, 0x95, 0x54, 0x00},  "Stmicro NAND 512M 3.3V 8-bit", 512, 128, 2048, 64,  2, 3,  15, 15, 25, 15, 10, 25, 15, 10,  A_08BIT},
	// [1GB] NAND08GW3B2CN6
    {{0x20, 0xD3, 0x51, 0x95, 0x58, 0x00},  "Stmicro NAND 1G 3.3V 8-bit", 1024, 128, 2048, 64,  2, 3,  15, 15, 25, 15, 10, 25, 15, 10,  A_08BIT},
};

struct dev_info DevSupported = {
	/* nand maker code */
	{NAND_MFR_SAMSUNG,
	NAND_MFR_HYNIX,
	NAND_MFR_MICRON,
	NAND_MFR_TOSHIBA,
	NAND_MFR_STMICRO},
	
	/* max supported nand flash of each maker */
	{MAX_SUPPORTED_SAMSUNG_NAND,
	MAX_SUPPORTED_HYNIX_NAND,
	MAX_SUPPORTED_MICRON_NAND,
	MAX_SUPPORTED_TOSHIBA_NAND,
	MAX_SUPPORTED_STMICRO_NAND},

	/* pointer of nand flash information */
	{(struct nand_features *)SAMSUNG_NAND_DevInfo,
	(struct nand_features *)HYNIX_NAND_DevInfo,
	(struct nand_features *)MICRON_NAND_DevInfo,
	(struct nand_features *)TOSHIBA_NAND_DevInfo,
	(struct nand_features *)STMICRO_NAND_DevInfo}
};

static uint8_t sep_nand_readbyte(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;
	return readb(chip->IO_ADDR_R);
}

static u16 sep_nand_readword(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;
	return readw(chip->IO_ADDR_R);
}

static void sep_nand_select_chip(struct mtd_info *mtd, int chipnr)
{
	int old, new;
	
	if (chipnr == -1) 
		return;

	/* remember the selected chip */
	nfc.cs = chipnr;
	
	old = readl(NAND_CFG);
	new = (old & (~(0x3 << 25))) | (nfc.cs << 25);
	writel(new, NAND_CFG);	
}

static int sep_nand_dev_ready(struct mtd_info *mtd)
{	
	return (readl(NAND_STATUS) & (1 << nfc.cs));
}

static void sep_nand_cmdfunc(struct mtd_info *mtd, unsigned int command, int column, int page_addr)
{
	int i;

	/* Emulate NAND_CMD_READOOB */
	if (command == NAND_CMD_READOOB) {
		column += mtd->writesize;
		command = NAND_CMD_READ0;
	}

	/* write command */
	writeb(command, NAND_CMD);

	/* write column address */
	if (likely(command != NAND_CMD_READID)) {
		if (column != -1) {
			if (nfc.bus_type != SINGLE_CHAN_8BITS)
				column >>= 1;
			for (i = 0; i < nfc.nand->ColCycle; i++) {
				writeb(column & 0xff, NAND_ADDR);
				column >>= 8;
			}
		}	
		/* write row address */
		if (page_addr != -1) {			
			for (i = 0; i < nfc.nand->RowCycle; i++) {
				writeb(page_addr & 0xff, NAND_ADDR);
				page_addr >>= 8;
			}
		}
	}

	switch (command) {
	/* Block Erase */
	case NAND_CMD_ERASE1:
	case NAND_CMD_ERASE2:
		return;
	
	/* Page Program */
	case NAND_CMD_SEQIN:
	case NAND_CMD_PAGEPROG:
		return;

	/* Read */
	case NAND_CMD_READ0:
		writeb(NAND_CMD_READSTART, NAND_CMD);
		while (!(readl(NAND_STATUS) & (1 << nfc.cs)));
		return;
	
	/* Reset */
	case NAND_CMD_RESET:
		while (!(readl(NAND_STATUS) & (1 << nfc.cs)));
		return;

	/* Read ID, wait for tWHR */
	case NAND_CMD_READID:
		writeb(0x00, NAND_ADDR);
		ndelay(100);
		return;

	/* Read Status, Wait for tWHR*/
	case NAND_CMD_STATUS:
		ndelay(100);
		return;
		
	default:
		printf("Not Supported Command!!!\n");
		BUG();
	}
}

static int sep_nand_waitfunc(struct mtd_info *mtd, struct nand_chip *this)
{	
	int status;		

	while (!(readl(NAND_STATUS) & (1 << nfc.cs)));

	this->cmdfunc(mtd, NAND_CMD_STATUS, -1, -1);

	status = readl(NAND_SDATA);	
	
	if (nfc.bus_type == DOUBLE_CHANS_16BITS) {  
		if (status & 0x0100)
		 	status |= 0x01;
		status &= ~0xff00;
	}
	
	if (status & 0x01) {
		if (this->state == FL_ERASING)
			printf("Erase fail!\n");
		else
			printf("Program fail!\n");
	}
		
	return status;	
}

static inline void random_data_input(int column)
{
	if (nfc.bus_type != SINGLE_CHAN_8BITS)
		column >>= 1;
	
	writeb(NAND_CMD_RNDIN, NAND_CMD); 
	writeb(column & 0xff, NAND_ADDR);
	writeb((column >> 8) & 0xff, NAND_ADDR);	 
}

static inline void random_data_output(int column)
{
	if (nfc.bus_type != SINGLE_CHAN_8BITS)
		column >>= 1;
	
	writeb(NAND_CMD_RNDOUT, NAND_CMD);
	writeb(column & 0xff, NAND_ADDR);
	writeb((column >> 8) & 0xff, NAND_ADDR);
	writeb(NAND_CMD_RNDOUTSTART, NAND_CMD);	 
}

static inline void write_fs_info_per_step(struct mtd_info *mtd, int step)
{
	struct nand_chip *this = mtd->priv;
	int offset;
	int i;

	random_data_input(mtd->writesize + (step << 2));

	if (nfc.bus_type == SINGLE_CHAN_8BITS) {
		offset = step << 2;
		for (i = 0; i < 4; i++)
			writeb(this->oob_poi[offset + i], NAND_SDATA);
	} else {
		uint16_t *oobbuf = (uint16_t *)this->oob_poi;

		offset = step << 1;
		for (i = 0; i < 2; i++)
			writew(oobbuf[offset + i], NAND_SDATA);
	}
}

static inline void read_fs_info_per_step(struct mtd_info *mtd, int step)
{
	struct nand_chip *this = mtd->priv;
	int offset;
	int i;

	random_data_output(mtd->writesize + (step << 2));

	if (nfc.bus_type == SINGLE_CHAN_8BITS) {
		offset = step << 2;
		for (i = 0; i < 4; i++)
			this->oob_poi[offset + i] = readb(NAND_SDATA);
	} else {
		uint16_t *oobbuf = (uint16_t *)this->oob_poi;

		offset = step << 1;
		for (i = 0; i < 2; i++)
			oobbuf[offset + i] = readw(NAND_SDATA);
	}
}

static inline void write_ecc_code_per_step(struct mtd_info *mtd, int step)
{
	struct nand_chip *this = mtd->priv;
	uint32_t *eccpos = this->ecc.layout->eccpos;	
	int i, ecc_code;

	switch (nfc.ecc_mode) {  
		case ECC_MODE16: {	
			random_data_input(mtd->writesize + eccpos[0] + step * 28);
			if (nfc.bus_type == SINGLE_CHAN_8BITS)	{
				for (i = 0; i < 7; i++)	{
					ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
					writeb(ecc_code & 0xff, NAND_SDATA);
					writeb((ecc_code >> 8) & 0xff, NAND_SDATA);
					writeb((ecc_code >> 16) & 0xff, NAND_SDATA);
					writeb((ecc_code >> 24) & 0xff, NAND_SDATA);
				}
			} else {
				for (i = 0; i < 7; i++)	{
					ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
					writew(ecc_code & 0xffff, NAND_SDATA);
					writew((ecc_code >> 16) & 0xffff, NAND_SDATA);
				}
			}	
			break;
		}
					
		case ECC_MODE24: {
			random_data_input(mtd->writesize + eccpos[0] + step * 42);
			if (nfc.bus_type == SINGLE_CHAN_8BITS)	{
				for (i = 0; i < 10; i++) {
					ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
					writeb(ecc_code & 0xff, NAND_SDATA);
					writeb((ecc_code >> 8) & 0xff, NAND_SDATA);
					writeb((ecc_code >> 16) & 0xff, NAND_SDATA);
					writeb((ecc_code >> 24) & 0xff, NAND_SDATA);
				}
	
				ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
				writeb(ecc_code & 0xff, NAND_SDATA);
				writeb((ecc_code >> 8) & 0xff, NAND_SDATA);	
			} else {
				for (i = 0; i < 10; i++) {
					ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
					writew(ecc_code & 0xffff, NAND_SDATA);
					writew((ecc_code >> 16) & 0xffff, NAND_SDATA);
				}
				ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
				writew(ecc_code & 0xffff, NAND_SDATA);
			}	
			break;
		}
		
		default: {	//nfc.ecc_mode = ECC_MODE30
			int offset = mtd->writesize + (step << 2) + 3;
			random_data_input(offset);

			if (nfc.bus_type == SINGLE_CHAN_8BITS)	{
				/* To rewrite the 1028th byte */
				writeb((((readl(NAND_ECC_PARITY0) << 4) | (this->oob_poi[(step << 2) + 3] & 0x0f)) & 0xff), NAND_SDATA);
				random_data_input(mtd->writesize + eccpos[0] + step * 52);
				for (i = 1; i < 14; i++) {
					ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
					writeb(ecc_code & 0xff, NAND_SDATA);
					writeb((ecc_code >> 8) & 0xff, NAND_SDATA);
					writeb((ecc_code >> 16) & 0xff, NAND_SDATA);
					writeb((ecc_code >> 24) & 0xff, NAND_SDATA);
				}
			} else {
				uint16_t *oobbuf = (uint16_t *)this->oob_poi;
				/* To rewrite the 514th half-word */
				writew((((readl(NAND_ECC_PARITY0) << 12) | (oobbuf[(step << 1) + 1] & 0x0fff)) & 0xffff), NAND_SDATA);
				random_data_input(mtd->writesize + eccpos[0] + step * 52);
				for (i = 1; i < 14; i++) {
					ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
					writew(ecc_code & 0xffff, NAND_SDATA);
					writew((ecc_code >> 16) & 0xffff, NAND_SDATA);
				}
			}	
			break;
		}		
	}
}

static inline void read_ecc_code_per_step(struct mtd_info *mtd, int step)
{
	struct nand_chip *this = mtd->priv;
	uint32_t *eccpos = this->ecc.layout->eccpos;	
	int i;

	switch (nfc.ecc_mode) {
		case ECC_MODE16: {
			random_data_output(mtd->writesize + eccpos[0] + step * 28);
			if (nfc.bus_type == SINGLE_CHAN_8BITS) {
				for (i = 0; i < 28; i++) 
					readb(NAND_SDATA);	
			} else {
				for (i = 0; i < 14; i++) 
					readw(NAND_SDATA);	
			}
			break;
		}

		case ECC_MODE24: {
			random_data_output(mtd->writesize + eccpos[0] + step * 42);
			if (nfc.bus_type == SINGLE_CHAN_8BITS) {
				for (i = 0; i < 42; i++) 
					readb(NAND_SDATA);	
			} else {
				for (i = 0; i < 21; i++) 
					readw(NAND_SDATA);	
			}
			break;
		}
		
		default: {	//nfc.ecc_mode = ECC_MODE30
			random_data_output(mtd->writesize + eccpos[0] + step * 52);
			if (nfc.bus_type == SINGLE_CHAN_8BITS) {
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

static inline void ecc_correct_by_step(struct mtd_info *mtd, u_char *buf, int step)
{
	struct nand_chip *this = mtd->priv;
	uint16_t *oobbuf = (uint16_t *)this->oob_poi;
	uint16_t *databuf = (uint16_t *)buf;
	uint32_t error_sum, error_reg, error_word, error_bit;
	int i;

	if (!step) {
		nfc.ecc_failed = 0;
		nfc.ecc_corrected = 0;
	}

	if (readl(NAND_STATUS) & (1 << 10)) {				
		printf("step: %d, The number of error-bits exceeds correction range!\n", step);
		nfc.ecc_failed = -1;
	} else {
  		error_sum = (readl(NAND_STATUS) >> 12) & 0x1f;
		if (error_sum) {
			printf("step: %d, The number of error-bits is %d.\n", step, error_sum);					
			for (i = 0; i < error_sum; i++) {
				error_reg = readl(NAND_ERR_ADDR0 + (i << 2));
				error_word = error_reg & 0x3ff; 
				error_bit = (error_reg >> 10) & 0xffff;
				if (error_word < 512) {					
					databuf[error_word] ^= error_bit;  
					nfc.ecc_corrected++;
				} else if (512 <= error_word && error_word < 514) {
					oobbuf[(step << 1) + (error_word - 512)] ^= error_bit;
					nfc.ecc_corrected++;
				}					
			}		
		}
	}
}

static void sep_nand_writebuf(struct mtd_info *mtd, const u_char *buf, int len)
{
	int i;
	uint16_t *p = (uint16_t *)buf;

	if (len == mtd->writesize) {
		int step;
		int offset;
		
		for (step = 0; step < nfc.ecc_steps; step++) {
			/* Open ecc module */
			writel(readl(NAND_CFG) & (~(1 << 29)), NAND_CFG);
			/* Init ecc module */				
			writel(0x1, NAND_INIT);
			
			if (likely(step))
				random_data_input(step << 10);

			/* Write 1024 bytes data */
			if (nfc.bus_type == SINGLE_CHAN_8BITS) {
				offset = step << 10;
				for (i = 0; i < 1024; i++) 
					writeb(buf[offset + i], NAND_SDATA);	
			} else {
				offset = step << 9;
				for (i = 0; i < 512; i++) 
					writew(p[offset + i], NAND_SDATA);
			}

			/* Write 4 bytes file system information */
			write_fs_info_per_step(mtd, step);	
 
			/* Close ecc module */
			writel(readl(NAND_CFG) | (1 << 29), NAND_CFG);
			/* Write ecc code*/			
			write_ecc_code_per_step(mtd, step);		
		}		
	} else {	//len == mtd->oobsize	
		int size;	

		if (nfc.bus_type == SINGLE_CHAN_8BITS) {
			size = mtd->oobsize;
			for (i = 0; i < size; i++) 
				writeb(buf[i], NAND_SDATA);	
		} else {
			size = mtd->oobsize >> 1;
			for (i = 0; i < size; i++) 
				writew(p[i], NAND_SDATA);
		}
	}
}

static void sep_nand_readbuf(struct mtd_info *mtd, u_char *buf, int len)
{
	int i;
	uint16_t *p = (uint16_t *)buf;

	if (len == mtd->writesize) {
		int step;
		int offset;
		
		for (step = 0; step < nfc.ecc_steps; step++) {
			/* Open ecc module */
			writel(readl(NAND_CFG) & (~(1 << 29)), NAND_CFG);
			/* Init ecc module */				
			writel(0x1, NAND_INIT);
			
			if (likely(step))		
				random_data_output(step << 10);

			/* Read 1024 bytes data */
			if (nfc.bus_type == SINGLE_CHAN_8BITS) {
				offset = step << 10;
				for (i = 0; i < 1024; i++) 
					buf[offset + i] = readb(NAND_SDATA);
			} else {
				offset = step << 9;
				for (i = 0; i < 512; i++) 
					p[offset + i] = readw(NAND_SDATA);
			}

			/* Read 4 bytes file system information */
			read_fs_info_per_step(mtd, step);			

			/* Read ecc code */
			read_ecc_code_per_step(mtd, step);
			
			/* Wait ecc decoder completed */
			while (!(readl(NAND_STATUS) & (1 << 8)));
			writel(readl(NAND_STATUS) | (1 << 8), NAND_STATUS);
			/* Close ecc module */
			writel(readl(NAND_CFG) | (1 << 29), NAND_CFG);	

			/* Correct data */
			ecc_correct_by_step(mtd, &buf[step << 10], step);
		}				
	} else {	//len == mtd->oobsize
		int size;

		if (nfc.bus_type == SINGLE_CHAN_8BITS) {
			size = mtd->oobsize;
			for (i = 0; i < size; i++) 
				buf[i] = readb(NAND_SDATA);
		} else {
			size = mtd->oobsize >> 1;
			for (i = 0; i < size; i++) 
				p[i] = readw(NAND_SDATA);
		}
	}	
}

static void sep_nand_write_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf)
{
	chip->write_buf(mtd, buf, mtd->writesize);
}

static int sep_nand_read_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf, int page)
{	
	chip->read_buf(mtd, buf, mtd->writesize);

	if (nfc.ecc_failed < 0)
		mtd->ecc_stats.failed++;
	else
	/* 
	 * why driver force nfc.ecc_corrected equal zero? because if nfc.ecc_corrected too large( >= 2), file 
	 * system deals with the block a bad. 
	 */
		nfc.ecc_corrected = 0;
		mtd->ecc_stats.corrected += nfc.ecc_corrected;

	return 0;
}

static void set_nand_ecclayou(void)
{
	int i;
	int base = nfc.ecc_steps << 2;

	switch (nfc.ecc_mode) {
		case ECC_MODE16: {
			nfc.nand_ooblayout.eccbytes = nfc.ecc_steps * 28;
			for (i = 0; i < nfc.nand_ooblayout.eccbytes; i++)
				nfc.nand_ooblayout.eccpos[i] = base + i;
				
			nfc.nand_ooblayout.oobfree[0].offset = 2;
			nfc.nand_ooblayout.oobfree[0].length = base - 2;
			break;
		}
			
		case ECC_MODE24: {
			nfc.nand_ooblayout.eccbytes = nfc.ecc_steps * 42;
			for (i = 0; i < nfc.nand_ooblayout.eccbytes; i++)
				nfc.nand_ooblayout.eccpos[i] = base + i;
				
			nfc.nand_ooblayout.oobfree[0].offset = 2;
			nfc.nand_ooblayout.oobfree[0].length = base - 2;
			break;
		}
			
		/* NB: nfc.ecc_steps must less than or equal to 16 */
		default: {	//nfc.ecc_mode = ECC_MODE30 
			nfc.nand_ooblayout.eccbytes = nfc.ecc_steps * 52;
			for (i = 0; i < nfc.nand_ooblayout.eccbytes; i++)
				nfc.nand_ooblayout.eccpos[i] = base + i;
			
			for (i = 1; i < nfc.ecc_steps; i++) {
				nfc.nand_ooblayout.oobfree[i].offset = i << 2;
				nfc.nand_ooblayout.oobfree[i].length = 3;
			}
			break;			
		}	
	}
}

#define FORCE_MAX_ECC_MODE_24
static void calculate_ecc_mode(void)
{
	int avail_size;
		
	/* calculate available oob size per 1024 bytes */
	nfc.ecc_steps = nfc.nand->PageSize >> 10;
	avail_size = nfc.nand->SpareSize / nfc.ecc_steps;

#ifndef FORCE_MAX_ECC_MODE_24
	if (avail_size >= 56)
		nfc.ecc_mode = ECC_MODE30;
	else if (avail_size >= 46)
#else
	if (avail_size >= 46)
		nfc.ecc_mode = ECC_MODE24;
#endif
	else if (avail_size >= 32)
		nfc.ecc_mode = ECC_MODE16;
	else
		BUG();

	if (nfc.bus_type == DOUBLE_CHANS_16BITS)
		nfc.ecc_steps <<= 1;
}

static void calculate_timing_sequence(void)
{
	int HCLK, tacls, twrph0, twrph1, twr;

	HCLK = 1000000000 / BUS_CLK_FREQ;	//unit "ns"

	tacls = nfc.nand->tCLS - nfc.nand->tWP;
	if (tacls < nfc.nand->tALS - nfc.nand->tWP)
		tacls = nfc.nand->tALS - nfc.nand->tWP;	

	nfc.TACLS = tacls / HCLK;
	if (nfc.TACLS * HCLK < tacls)
		nfc.TACLS += 1;	

	twrph0 = (nfc.nand->tWP > nfc.nand->tRP)? nfc.nand->tWP : nfc.nand->tRP;
	twrph1 = (nfc.nand->tWH > nfc.nand->tREH)? nfc.nand->tWH : nfc.nand->tREH;
	twr = (nfc.nand->tWC > nfc.nand->tRC)? nfc.nand->tWC : nfc.nand->tRC;

	nfc.TWRPH0 = twrph0 / HCLK;
	if (nfc.TWRPH0 * HCLK < twrph0)	
		nfc.TWRPH0 += 1; 
	
	nfc.TWRPH1 = twrph1 / HCLK;
	if (nfc.TWRPH1 * HCLK < twrph1)
		nfc.TWRPH1 += 1;
	
	if (nfc.TWRPH0 * HCLK + nfc.TWRPH1 * HCLK < twr) {
		int i = 0;

		while (1) {
			if (!(i & 0x1)) //even
				nfc.TWRPH0++;
			else	//odd
				nfc.TWRPH1++;

			if (nfc.TWRPH0 * HCLK + nfc.TWRPH1 * HCLK >= twr)	
				break;
			
			i++;
		}		
	}

	nfc.TWRPH0 -= 1;
	nfc.TWRPH1 -= 1;

}

static inline void clear_nfc_register(void)
{
	/* Clear NFC CFG nand CTRL register */
	writel(0, NAND_CFG);
}

static int get_nand_dev_info(void)
{
	int i, j;
	uint16_t id_code[6];	
	enum SupportedNandMaker maker_num;
	int match_count;
	int found_nand;

	clear_nfc_register();

	writel(NFC_ECC_PASS(1)    | NFC_ECC_MODE(0)    | NFC_CS(0) 		\
		 | NFC_TACLS   (0x3f) | NFC_TWRPH0  (0x3f) | NFC_TWRPH1(0x3f) 	\
		 | NFC_PAGESZ  (0)    | NFC_CHIP_NUM(0)    | NFC_FLASH_WIDTH(0),  NAND_CFG);		
	
	/* Reset the chip, required by some chips (e.g. Micron MT29FxGxxxxx) after power-up */
	sep_nand_cmdfunc(NULL, NAND_CMD_RESET, -1, -1);

	/* Send the command for reading device ID */
	sep_nand_cmdfunc(NULL, NAND_CMD_READID, 0x00, -1);

	/* Read entire ID string */
	for (i = 0; i < 6; i++)
		id_code[i] = readb(NAND_SDATA);

	/* Check maker code */
	maker_num = MAX_SUPPORTED_NAND_MAKER;
	for (i = 0; i < MAX_SUPPORTED_NAND_MAKER; i++) {
		if ((id_code[0] & 0xff) == DevSupported.MakerCode[i]) {
			maker_num = (enum SupportedNandMaker)i;
			nfc.nand = DevSupported.flash[i];
			break;
		}			
	}
	
	if (maker_num == MAX_SUPPORTED_NAND_MAKER) {
		printf("No device found\n");
		return -ENODEV;
	}

	/* Check device code */
	found_nand = 0;
	for (i = 0; i < DevSupported.NandNum[maker_num]; i++) {
		match_count = 0;

		for (j = 1; j < 6; j++) {
			if (nfc.nand->ID[j] == (id_code[j] & 0xff)) 
				match_count++;
			else if (nfc.nand->ID[j] == 0x00)
				match_count++;
			else
				break;
		}

		if (match_count == 5) {
			found_nand = 1;
			break;
		}
		else
			nfc.nand++;	
	}

	if (found_nand == 0) {
		printf("No device found\n");
		return -ENODEV;
	}	
	
	if (nfc.nand->attribute & A_16BIT)
		nfc.bus_type = SINGLE_CHAN_16BITS;
	else {	
	#ifndef 	CONFIG_MTD_NAND_SINGLE_CHANNEL
		uint8_t LByte;
		uint8_t HByte;
	
		/* Check if compositin of nand is double channel 8 bits */
		nfc.bus_type = DOUBLE_CHANS_16BITS;
		writel(readl(NAND_CFG) | NFC_CHIP_NUM(0) | NFC_FLASH_WIDTH(1), NAND_CFG);

		sep_nand_cmdfunc(NULL, NAND_CMD_RESET, -1, -1);
		sep_nand_cmdfunc(NULL, NAND_CMD_READID, 0x00, -1);

		for (i = 0; i < 6; i++)
			id_code[i] = readw(NAND_SDATA);		
		 
  		for (i = 0; i < 6; i++)	{
  			if (nfc.nand->ID[i] == 0x00)
				continue;
			LByte = id_code[i] & 0xff;
			HByte = (id_code[i] >> 8) & 0xff;
			if (nfc.nand->ID[i] != LByte) {
				printf("No device found\n");
				return -ENODEV;
			} else if (LByte != HByte) {
				nfc.bus_type = SINGLE_CHAN_8BITS;
				break;
			}
		}
	#else
		nfc.bus_type = SINGLE_CHAN_8BITS;  
	#endif
	}	

	calculate_ecc_mode();

	calculate_timing_sequence();

	return 0;
}

static void nfc_init(void) 
{	
	int ps;	//pagesize	
	
#ifdef CONFIG_SEP0611_SB
	/* GPIO set, PI[13], WPn */
	writel(readl(GPIO_PORTI_SEL) | (1 << 13), GPIO_PORTI_SEL);	//general function
	writel(readl(GPIO_PORTI_DATA) | (1 << 13), GPIO_PORTI_DATA);	//high level
	writel(readl(GPIO_PORTI_DIR) & ~(1 << 13), GPIO_PORTI_DIR);	//output
#else
	/* GPIO set, PD[8], WPn */
	writel(readl(GPIO_PORTD_SEL) | (1 << 8), GPIO_PORTI_SEL);	//general function
	writel(readl(GPIO_PORTD_DATA) | (1 << 8), GPIO_PORTI_DATA);	//high level
	writel(readl(GPIO_PORTD_DIR) & ~(1 << 8), GPIO_PORTI_DIR);	//output
#endif
	
	clear_nfc_register();

	/* configure NFC_CFG register */
	writel(NFC_ECC_PASS(1) | NFC_CS(0) | NFC_TACLS(nfc.TACLS + 1) | NFC_TWRPH0(nfc.TWRPH0 + 1) | NFC_TWRPH1(nfc.TWRPH1 + 1) , NAND_CFG);
	switch (nfc.nand->PageSize) {
		case 2048: ps = 0; break;
		case 4096: ps = 1; break;
		default: ps = 2;	//PageSize = 8192
	}
	writel(readl(NAND_CFG) | NFC_PAGESZ(ps), NAND_CFG);
	switch (nfc.ecc_mode) {
		case ECC_MODE16: 
			writel(readl(NAND_CFG) | NFC_ECC_MODE(0), NAND_CFG);
			break;
 
		case ECC_MODE24: 
			writel(readl(NAND_CFG) | NFC_ECC_MODE(1), NAND_CFG);
			break; 

		default: 	//nfc.ecc_mod == ECC_MODE30
			writel(readl(NAND_CFG) | NFC_ECC_MODE(2), NAND_CFG);
	}
	switch (nfc.bus_type) {
		case SINGLE_CHAN_8BITS: 
			writel(readl(NAND_CFG) | NFC_CHIP_NUM(0) | NFC_FLASH_WIDTH(0), NAND_CFG);
			break;
 
		case DOUBLE_CHANS_16BITS: 
			writel(readl(NAND_CFG) | NFC_CHIP_NUM(0) | NFC_FLASH_WIDTH(1), NAND_CFG);
			break;

		default: 	//nfc.bus_type == SINGLE_CHAN_16BITS
			writel(readl(NAND_CFG) | NFC_CHIP_NUM(1) | NFC_FLASH_WIDTH(1), NAND_CFG);
	}	
}

int board_nand_init(struct nand_chip *this)
{
	this->IO_ADDR_R = (void __iomem *)(NAND_SDATA);
	this->IO_ADDR_W = (void __iomem *)(NAND_SDATA);
	this->read_byte = sep_nand_readbyte;
	this->read_word = sep_nand_readword;
	this->write_buf = sep_nand_writebuf;
	this->read_buf = sep_nand_readbuf;
	this->select_chip = sep_nand_select_chip;
	this->dev_ready = sep_nand_dev_ready;
	this->cmdfunc = sep_nand_cmdfunc;
	this->waitfunc = sep_nand_waitfunc;
	
	/* try to get nand info */
	if (get_nand_dev_info()) 
		return -ENXIO;

	/* must be below get_nand_dev_info() */
	set_nand_ecclayou();
	this->ecc.mode = NAND_ECC_HW;
	this->ecc.layout = &nfc.nand_ooblayout;
	this->ecc.write_page = sep_nand_write_page_hwecc;
	this->ecc.read_page = sep_nand_read_page_hwecc;		
	
	/* inital nfc */
	nfc_init();

	return 0;   
}

//=====================================================================================================================================

static void send_colrow_address(int col, int row)
{
	int i;

	/*write column address*/
	if (col != -1) {
		for (i = 0; i < 2; i++) {
			writeb(col & 0xff, NAND_ADDR);
			col >>= 8;
		}
	}	
	/*write row address*/
	if (row != -1) {			
		for (i = 0; i < 3; i++) {
			writeb(row & 0xff, NAND_ADDR);
			row >>= 8;
		}
	}
}

static int erase_first_block(void)
{
	writel(0x60, NAND_CMD);
	send_colrow_address(-1, 0);
	writel(0xd0, NAND_CMD);

	while (!(readl(NAND_STATUS) & 0x1));

	writel(0x70, NAND_CMD);
	switch (nfc.bus_type) {	
		case SINGLE_CHAN_8BITS:	
			if (readl(NAND_SDATA) & 0x1) {
				printk("Erase first block fail!\n");
				return -1;
			}
			break;
			
		case DOUBLE_CHANS_16BITS:
			if (readl(NAND_SDATA) & 0x0101) {
				printk("Erase first block fail!\n");
				return -1;
			}
			break;	
		 
		default: printk("Not supported boot ecc mode!\n"); return -1;				
	}

	return 0;
}

void write_4Bytes_dummy_data(void)
{
	int i;

	if (nfc.bus_type == SINGLE_CHAN_8BITS) {
		for (i = 0; i < 4; i++)
			writeb(0xff, NAND_SDATA);
	} else {
		for (i = 0; i < 2; i++)
			writew(0xffff, NAND_SDATA);
	}
}

#define RANDOM_DATA_INPUT(col) do { 			\
	writel(0x85, NAND_CMD);						\
	writel((col) & 0xff, NAND_ADDR);			\
	writel(((col) >> 8) & 0xff, NAND_ADDR);		\
	} while(0)

void write_ecc_code(int step)
{	
	int i, ecc_code;

	switch (nfc.ecc_mode) {  
		case ECC_MODE16: {	
			if (nfc.bus_type == SINGLE_CHAN_8BITS)	{
				for (i = 0; i < 7; i++)	{
					ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
					writeb(ecc_code & 0xff, NAND_SDATA);
					writeb((ecc_code >> 8) & 0xff, NAND_SDATA);
					writeb((ecc_code >> 16) & 0xff, NAND_SDATA);
					writeb((ecc_code >> 24) & 0xff, NAND_SDATA);
				}
			} else {
				for (i = 0; i < 7; i++)	{
					ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
					writew(ecc_code & 0xffff, NAND_SDATA);
					writew((ecc_code >> 16) & 0xffff, NAND_SDATA);
				}
			}	
			break;
		}
					
		case ECC_MODE24: {
			if (nfc.bus_type == SINGLE_CHAN_8BITS)	{
				for (i = 0; i < 10; i++) {
					ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
					writeb(ecc_code & 0xff, NAND_SDATA);
					writeb((ecc_code >> 8) & 0xff, NAND_SDATA);
					writeb((ecc_code >> 16) & 0xff, NAND_SDATA);
					writeb((ecc_code >> 24) & 0xff, NAND_SDATA);
				}
	
				ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
				writeb(ecc_code & 0xff, NAND_SDATA);
				writeb((ecc_code >> 8) & 0xff, NAND_SDATA);	
			} else {
				for (i = 0; i < 10; i++) {
					ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
					writew(ecc_code & 0xffff, NAND_SDATA);
					writew((ecc_code >> 16) & 0xffff, NAND_SDATA);
				}
				ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
				writew(ecc_code & 0xffff, NAND_SDATA);
			}	
			break;
		}
		
		default: {	//nfc.ecc_mode = ECC_MODE30
			if (nfc.bus_type == SINGLE_CHAN_8BITS)	{
				/* to rewrite the 1028th byte */
				RANDOM_DATA_INPUT(1080 * step + 1027);
				writeb(((readl(NAND_ECC_PARITY0) << 4) | 0x0f) & 0xff, NAND_SDATA);

				for (i = 1; i < 14; i++) {
					ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
					writeb(ecc_code & 0xff, NAND_SDATA);
					writeb((ecc_code >> 8) & 0xff, NAND_SDATA);
					writeb((ecc_code >> 16) & 0xff, NAND_SDATA);
					writeb((ecc_code >> 24) & 0xff, NAND_SDATA);
				}	
			} else {
				/* to rewrite the 514th word */
				RANDOM_DATA_INPUT(540 * step + 513);	
				writew(((readl(NAND_ECC_PARITY0) << 12) | 0x0fff) & 0xffff, NAND_SDATA);
	
				for (i = 1; i < 14; i++) {
					ecc_code = readl(NAND_ECC_PARITY0 + (i << 2));
					writew(ecc_code & 0xffff, NAND_SDATA);
					writew((ecc_code >> 16) & 0xffff, NAND_SDATA);
				}
			}	
			break;
		}		
	}
}

static int write_page(u_char *write_buf, int pagenr)
{
	int i, step, offset;
	uint16_t *p = (uint16_t *)write_buf;

	writel(0x80, NAND_CMD);

	send_colrow_address(0, pagenr);

	for (step = 0; step < nfc.ecc_steps; step++) {	
		/* Open ecc module */
		writel(readl(NAND_CFG) & (~(1 << 29)), NAND_CFG);
		/* Init ecc module */				
		writel(0x1, NAND_INIT);

		/* Write 1024 bytes data */
		if (nfc.bus_type == SINGLE_CHAN_8BITS) {
			offset = step << 10;
			for (i = 0; i < 1024; i++) 
				writeb(write_buf[offset + i], NAND_SDATA);	
		} else {
			offset = step << 9;
			for (i = 0; i < 512; i++) 
				writew(p[offset + i], NAND_SDATA);
		}
		
		/* Write 4 bytes dummy data */
		write_4Bytes_dummy_data();
 
		/* Close ecc module */
		writel(readl(NAND_CFG) | (1 << 29), NAND_CFG);

		/* Write ecc code*/			
		write_ecc_code(step);				
	}
	
	writel(0x10, NAND_CMD);
	
  	while (!(readl(NAND_STATUS) & 0x1));
   
	writel(0x70, NAND_CMD);
	ndelay(100);
	if (readl(NAND_SDATA) & 0x1)
		return -1;
	else
		return 0;	
}

int burn_nandboot(u_char *buf)
{
	int i;
	
	switch (nfc.nand->PageSize) {
		case 2048: printk("page_size=2KiB\n"); break;
		case 4096: printk("page_size=4KiB\n"); break;
		case 8192: printk("page_size=8KiB\n"); break;
		default: printk("Nandboot page_size not supported\n"); return -1;		
	}	

	switch (nfc.bus_type) {
		case 0: printk("bus_type=SINGLE_CHAN_8BITS\n"); break;
		case 1: printk("bus_type=DOUBLE_CHANS_16BITS\n"); break;
		default: printk("Nandboot bus_type not supported\n"); return -1;
	}

	switch (nfc.ecc_mode) {
		case 0: printk("ecc_mode=ECC_MODE16\n"); break;
		case 1: printk("ecc_mode=ECC_MODE24\n"); break;
		case 2: printk("ecc_mode=ECC_MODE30\n"); break;
		default: printk("Nandboot ecc_mode not supported\n"); return -1;
	}

	/* erase first block */
	erase_first_block();
	
	/* write 8K bytes data */
	switch (nfc.write_size) {
		case 2048:
			for (i = 0; i < 512; i++)
				if (write_page(&buf[2048 * i], i))
					return 1;
			break;
		case 4096:
			for (i = 0; i < 256; i++)
				if (write_page(&buf[4096 * i], i))
					return 1;
			break;
		case 8192:
			for (i = 0; i < 128; i++)
				if (write_page(&buf[8192 * i], i))
					return 1;
			break;
		case 16384:
			for (i = 0; i < 64; i++)
				if (write_page(&buf[16384 * i], i))
					return 1;
			break;
		default: printk("Not supported write size\n");	return 1;	
	}

	return 0;
}

