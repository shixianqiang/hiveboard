#ifndef NFC_H_
#define NFC_H_

#include <asm/arch/vic.h>
#include <asm/arch/dmac.h>

// NFC configuration register
#define     NFC_ECCPASS(x)               (x << 29)   //0:unmask; 1:mask
#define     NFC_ECCMODE(x)               (x << 27)   //00: ecc16 mode; 01: ecc24 mode; 10: ecc30 mode 
#define     NFC_CS(x)                    (x << 25)   //chip select
#define     NFC_TACLS(x)                 (x << 18)   // time sequence x=0~63
#define     NFC_TWRPH0(x)                (x << 11)    // time sequencex=0~63
#define     NFC_TWRPH1(x)                (x << 4)    // time sequencex=0~63
#define     NFC_PAGESZ(x)                (x << 2)    // 00:2k/page nand ; 01:4k/page nand; 10:8k/page nand  
#define     NFC_CHIP_NUM(x)              (x << 1)    // 0: double chip;  1: single chip
#define     NFC_FLASHWIDTH(x)            (x)         // 0:8bit 1:16bit

//NFC DMA register
#define     NFC_TRAN_LEN(x)              (x << 10)   // DMA transfer data items, same with DMA config
#define     NFC_TRAN_DIR(x)              (x << 1)    // data tranfer direction. 0: Read data from NAND 1:Write data to NAND 
#define     NFC_TRAN_START(x)            (x)         //write 1 to start data tranfer


//NFC control register
#define     NFC_CE_CARE(x)               (x << 5)   // 0:CE don't care, controlled by hardware automatically; 1:CE care, controlled by software
#define     NFC_CE_REG(x)                (x << 4)   // 0: CE=0; 1:CE=1
#define     NFC_ENB_TRANSDONE_INT(x)     (x << 3)   // 0:interrupt disable, 1: interrupt enable, data tranfer completed interrupt enable        
#define     NFC_DECDONE_INT(x)           (x << 2)   // 0:ecc decoder completed interrupt disable, 1: ecc decoder completed interrupt enable
#define     NFC_ENB_RNB_INT(x)           (x << 1)   // 0:RnB edge trigger interrupt disable, 1:RnB edge trigger interrupt enable
#define     NFC_RNB_TRANSMODE(x)         (x)        // 0:RnB rising edge trigger, 1:RnB falling edge trigger



// for ECC decoding with Dummy Data
#define	    DUMMY_TACLS	                 (3)
#define	    DUMMY_TWRPH0	             (3)
#define	    DUMMY_TWRPH1	             (3)
// Default NAND Flash timing @HCLK 66MHz (tHCLK = 15ns)
#define	    TACLS	                     (7)	  // 1 HCLK (15ns)
#define	    TWRPH0	                     (7)	  // 2 HCLK (30ns)
#define	    TWRPH1	                     (7)	  // 2 HCLK (30ns)


#define     CMD_READID			        (0x90)	//  ReadID
#define     CMD_READ			        (0x00)	//  Read phase 1
#define     CMD_READ2			        (0x30)	//  Read phase 2
#define     CMD_READ_CB			        (0x35)	//  Read for copy back
#define     CMD_READ_O			        (0x50)	//  Read2
#define     CMD_RESET			        (0xff)	//  Reset
#define     CMD_ERASE			        (0x60)	//  Erase phase 1
#define     CMD_ERASE2			        (0xd0)	//  Erase phase 2
#define     CMD_WRITE			        (0x80)	//  Write phase 1
#define     CMD_WRITE2			        (0x10)	//  Write phase 2
#define     CMD_WRITE_TP		        (0x11)	//  Two Plane Write phase 1
#define     CMD_WRITE2_TP		        (0x81)	//  Two Plane Write phase 2
#define     CMD_STATUS			        (0x70)	//  STATUS
#define     CMD_STATUS_IL		        (0xF1)	//  STATUS Interleave
#define     CMD_STATUS2_IL		        (0xF2)	//  STATUS2 Interleave
#define     CMD_RDI				        (0x85)	//  Random Data Input
#define     CMD_RDO				        (0x05)	//  Random Data Output phase 1
#define     CMD_RDO2			        (0xE0)	//  Random Data Output phase 2


#define NFC_OT_CHIPTYPE	K9HCG08U1M
#define NFC_OT_ECCMASK	FALSE
#define NFC_OT_ECCMODE	ECC16_MODE
#define NFC_OT_CHIPSEL	NAND_CS0
#define NFC_OT_CHIPNUM	0
#define NFC_OT_BITWIDTH	NAND_WIDTH8
#define NFC_OT_PAGESIZE	PAGE8K	
#define NFC_OT_TRANSIZE	0
#define NFC_OT_TACLS	0
#define NFC_OT_TWRPH0	0
#define NFC_OT_TWRPH1	0
#define NFC_OT_CECARE	CE_NOT_CARE
#define NFC_OT_CEREG	CE_REG0
#define NFC_OT_INTTYPE	TRAN_DONE_INT
#define NFC_OT_INTEN	FALSE

void NFC_Test(void);

#endif /*NFC_H_*/
