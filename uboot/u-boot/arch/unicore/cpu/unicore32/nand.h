#ifndef _NAND_H_
#define _NAND_H_

//NFC_CFG
#define     	NFC_ECC_PASS(x)              	((x) << 29)   	//0:unmask; 1: mask.
#define     	NFC_ECC_MODE(x)              	((x) << 27)   	//00:ecc16 mode; 01:ecc24 mode; 10:ecc30 mode. 
#define     	NFC_CS(x)                    	((x) << 25)   	//chip select
#define     	NFC_TACLS(x)                 	((x) << 18)   	//time sequence x=0~63. 
#define     	NFC_TWRPH0(x)                	((x) << 11)     //time sequence x=0~63.
#define  			NFC_TWRPH1(x)                	((x) << 4)    	//time sequence x=0~63.
#define     	NFC_PAGESZ(x)                	((x) << 2)    	//0:2k/page; 1: 4k/page; 2: 8k/page. 
#define   			NFC_CHIP_NUM(x)				  					((x) << 1) 		//0:two 8 bits nand flash; 1: one 16 bits nand flash.
#define     	NFC_FLASH_WIDTH(x)           	(x)         	//0:8bit; 1:16bit.

//NFC_DMA
#define     	NFC_TRAN_LEN(x)              	((x) << 10)   	//dma transfer data items, same with DMA config.
#define     	NFC_TRAN_DIR(x)              	((x) << 1)    	//data tranfer direction. 0:Read data from NAND; 1:Write data to NAND. 
#define     	NFC_TRAN_START(x)            	(x)         	//write 1 to start data tranfer.

//NFC_CTL
#define			NFC_CE_CARE(x)									((x) << 5)
#define			NFC_CE_REG(x)										((x) << 4)
#define			NFC_EN_TRANS_DONE_INT(x)		((x) << 3)
#define			NFC_EN_DEC_DONE_INT(x)				((x) << 2)
#define			NFC_EN_RnB_INT(x)							((x) << 1)
#define			NFC_RnB_TRAN_MODE(x)					(x)

//NFC_STATUS
#define			NFC_DMA_TRANS_DONE						(1 << 9)
#define 		NFC_ECC_DEC_DONE								(1 << 8)
/*===========================================================================================================================================*/
enum EccMode {
	ECC_MODE16,
	ECC_MODE24,
	ECC_MODE30
};

enum BusType {
	SINGLE_CHAN_8BITS,	
	DOUBLE_CHANS_16BITS,	
};

struct nfc_info {
	int page_size;
	enum BusType bus_type;	
	enum EccMode ecc_mode;
	
	int write_size;
	int ecc_steps;
	int ecc_pos;	
};


extern void nfc_init(void);
extern void read_uboot(void);

#endif
