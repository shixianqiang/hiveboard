/** linux/drivers/mtd/nand/sep0611.h
* Copyright (c) 2011 Seuic Semiconductor Co.,Ltd
*
* sepxxxx nand driver.
* chip program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*/

#ifndef	_LINUX_SEP_NFC_H_
#define	_LINUX_SEP_NFC_H_


/*===========================================NFC==================================================*/
//NFC_CFG
#define     NFC_ECC_PASS(x)              	((x) << 29)   			//0:unmask; 1: mask.
#define     NFC_ECC_MODE(x)              	((x) << 27)   			//00:ecc16 mode; 01:ecc24 mode; 10:ecc30 mode. 
#define     NFC_CS(x)                    	((x) << 25)   			//chip select
#define     NFC_TACLS(x)                 	((x) << 18)   			//time sequence x=0~63. 
#define     NFC_TWRPH0(x)                	((x) << 11)   			//time sequence x=0~63.
#define     NFC_TWRPH1(x)                	((x) << 4)    			//time sequence x=0~63.
#define     NFC_PAGESZ(x)                	((x) << 2)    			//0:2k/page; 1: 4k/page; 2: 8k/page. 
#define   	NFC_CHIP_NUM(x)				  	((x) << 1) 				//0:two 8 bit nand flash; 1: one 16 bit nand flash.
#define     NFC_FLASH_WIDTH(x)           	(x)         			//0:8bit; 1:16bit.

//NFC_CTL
#define		NFC_CE_CARE(x)					((x) << 5)
#define		NFC_CE_REG(x)					((x) << 4)
#define		NFC_EN_TRANS_DONE_INT(x)		((x) << 3)
#define		NFC_EN_DEC_DONE_INT(x)			((x) << 2)
#define		NFC_EN_RnB_INT(x)				((x) << 1)
#define		NFC_RnB_TRAN_MODE(x)			(x)

//NFC_DMA 
#define     NFC_TRAN_LEN(x)              	((x) << 10)   			//DMA transfer data items, same with DMA config.
#define     NFC_TRAN_DIR(x)              	((x) << 1)    			//data tranfer direction. 0:Read data from NAND; 1:Write data to NAND. 
#define     NFC_TRAN_START(x)            	(x)         			//write 1 to start data tranfer.

//DmaCfgReg
#define    DMAC_DMA_EN(x)               	(x)						//Enable DMAC. x=0, disable DMAC; x=1, enable DMAC.

                
//ChEnReg
#define    DMAC_CH_EN(x)                	(x)             		//Enable/Disable the Channel. x=0, disable channel; x=1, enable channel.
#define    DMAC_CH_EN_WE(x)             	((x) << 8)          	//Channel enable write enable. x=0, disable; x=1, enable.

/*=============================================================================================================*/
enum EccMode {
	ECC_MODE16,
	ECC_MODE24,
	ECC_MODE30,
};

enum BusType {
	SINGLE_CHAN_8BITS,	
	DOUBLE_CHANS_16BITS,	
	SINGLE_CHAN_16BITS
};

enum SupportedNandMaker {
	SAMSUNG,
	HYNIX,
	MICRON,
	TOSHIBA,
	ST,
	MAX_SUPPORTED_NAND_MAKER
};

#define 	A_08BIT		0x00000001	//8 bit bus
#define		A_16BIT		0x00000002	//16 bit bus

struct nand_features {  
	uint8_t ID[6];
	char *name;
	int ChipSize;	//unit "MBytes"
	int BlockSize;	//unit "KBytes"
	int PageSize;	//unit "Bytes"
	int SpareSize;	//unit "Bytes"
	int ColCycle;	//column address cycle
	int RowCycle;	//row address cycle
	int tCLS;	//CLE Setup Time
	int tALS;	//ALE Setup Time
	int tWC;	//Write Cycle Time
	int tWP;	//WEn Pulse Width
	int tWH;	//WEn High Hold Time 
	int tRC;	//Read Cycle Time
	int tRP;	//REn Pulse Width
	int tREH;	//REn High Hold Time
	int attribute;
};

struct dev_info {
	int	MakerCode[MAX_SUPPORTED_NAND_MAKER];
	int	NandNum[MAX_SUPPORTED_NAND_MAKER];
	struct nand_features *flash[MAX_SUPPORTED_NAND_MAKER];
};

struct partition_name {
	char string[6];
};

struct nfc_info {
	enum BusType bus_type;
	
	enum EccMode ecc_mode;
	int ecc_steps;
	int ecc_failed;
	int ecc_corrected;

	int write_size;
	
	struct nand_ecclayout nand_ooblayout;
	/* the selected chip number */	
	int cs;
	
	int TACLS;
	int TWRPH0;
	int TWRPH1;	
	
	struct nand_features *nand;
};

/* exported to nand_base.c */
extern struct nfc_info nfc;

#endif

