/* linux/arch/unicore/mach-sep0611/include/mach/nand.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *  fjj <fanjianjun@wxseuic.com>
 *
 * Southeast University ASIC SoC support
 *
 * NAND Driver for SEP0611 Board
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  06-08-2011 fjj initial version
 *  07-15-2011 fjj second version
 */

#ifndef	__ASM_ARCH_NAND_H_
#define	__ASM_ARCH_NAND_H_

#include <mtd/mtd-abi.h>

/* NFC_CFG */
#define     NFC_ECC_PASS(x)              	((x) << 29)   			
#define     NFC_ECC_MODE(x)              	((x) << 27)   			 
#define     NFC_CS(x)                    	((x) << 25)   			
#define     NFC_TACLS(x)                 	((x) << 18)   			
#define     NFC_TWRPH0(x)                	((x) << 11)   			
#define     NFC_TWRPH1(x)                	((x) << 4)    			
#define     NFC_PAGESZ(x)                	((x) << 2)    			
#define   	NFC_CHIP_NUM(x)				  	((x) << 1) 				
#define     NFC_FLASH_WIDTH(x)           	(x)         			

/* NFC_CTL */
#define		NFC_CE_CARE(x)					((x) << 5)
#define		NFC_CE_REG(x)					((x) << 4)
#define		NFC_EN_TRANS_DONE_INT(x)		((x) << 3)
#define		NFC_EN_DEC_DONE_INT(x)			((x) << 2)
#define		NFC_EN_RnB_INT(x)				((x) << 1)
#define		NFC_RnB_TRAN_MODE(x)			(x)

/* NFC_DMA */
#define     NFC_TRAN_LEN(x)              	((x) << 10)   			
#define     NFC_TRAN_DIR(x)              	((x) << 1)    			
#define     NFC_TRAN_START(x)            	(x)         			

/*==================================================================================================================================*/
enum EccMode {
	ECC_MODE_16,
	ECC_MODE_24
};

enum BusType {
	SINGLE_CHAN_8BITS,	
	DOUBLE_CHANS_16BITS,	
	SINGLE_CHAN_16BITS
};

/* below for ftl */
enum FailOperate {
	PREVIOUS,
	CURRENT,
	PRE_CUR,
	NO_OPERATE_FAIL
};

enum FtlState {
	FTL_READY,
	FTL_READING,
	FTL_WRITING,
	FTL_DUMMY_WRITING,
	FTL_ERASING
};
/* above for ftl */

enum SupportedNandMaker {
	SAMSUNG,
	HYNIX,
	MICRON,
	TOSHIBA,
	ST,
	MAX_SUPPORTED_NAND_MAKER
};

#define 	A_08BIT		0x01	/* 8 bit bus */
#define		A_16BIT		0x02	/* 16 bit bus */
typedef struct {  
	uint8_t ID[6];
	uint8_t *name;
	uint16_t ChipSize;	/* unit "MBytes" */
	uint16_t BlockSize;	/* unit "KBytes" */
	uint16_t PageSize;	/* unit "Bytes" */
	uint16_t SpareSize;	/* unit "Bytes" */
	uint8_t ColCycle;	/* column address cycle */
	uint8_t RowCycle;	/* row address cycle */
	uint8_t tCLS;	/* CLE Setup Time */
	uint8_t tALS;	/* ALE Setup Time */
	uint8_t tWC;	/* Write Cycle Time */
	uint8_t tWP;	/* WEn Pulse Width */
	uint8_t tWH;	/* WEn High Hold Time  */
	uint8_t tRC;	/* Read Cycle Time */
	uint8_t tRP;	/* REn Pulse Width */
	uint8_t tREH;	/* REn High Hold Time */
	uint8_t attribute;
} NAND_FEATURES;

typedef struct {
	uint16_t MakerCode[MAX_SUPPORTED_NAND_MAKER];
	uint16_t NandNum[MAX_SUPPORTED_NAND_MAKER];
	NAND_FEATURES *flash[MAX_SUPPORTED_NAND_MAKER];
} DEV_INFO;

typedef struct {
	NAND_FEATURES *nand; 
	enum BusType bus_type;
	enum EccMode ecc_mode;
	
	/* timming */
	uint8_t TACLS;
	uint8_t TWRPH0;
	uint8_t TWRPH1;

	uint8_t eccSteps;
	uint8_t ecc_failed;
	uint8_t ecc_corrected;
	
	/* record the selected chip number */	
	uint8_t cs;
		
	/* for double dma buffer opration */
	uint8_t *doubleDma_virAddr;
	uint8_t *doubleDma_virAddr_0;
	uint8_t *doubleDma_virAddr_1;
	dma_addr_t doubleDma_busAddr;
	dma_addr_t doubleDma_busAddr_0;
	dma_addr_t doubleDma_busAddr_1;

	struct nand_ecclayout nand_ooblayout;

	struct completion nand_ready_request;
	struct mutex mutex_sepnfc;

	/* below for ftl */
	uint16_t eccUnitDatalen;
	uint16_t ftlSysDataStartPos;
	
	enum FtlState state;
	enum FtlState interleave_state[2];
	enum FailOperate failOp;

	uint8_t *eccAlign_virAddr;
	uint8_t *eccAlign_virAddr_0;
	uint8_t *eccAlign_virAddr_1;
	dma_addr_t eccAlign_busAddr;
	dma_addr_t eccAlign_busAddr_0;
	dma_addr_t eccAlign_busAddr_1;
	/* above for ftl */

	struct clk *clk;
} SEP_NFC_INFO;

#define MTD_SIZE_1MiB 	(1ull << 20)
typedef struct {
	struct mtd_partition *table;
	int num;
} SEP_MTD_PARTS;

extern SEP_NFC_INFO gNfcInfo;
extern SEP_MTD_PARTS sep0611_partitions;

#endif /* __ASM_ARCH_NAND_H_ */
