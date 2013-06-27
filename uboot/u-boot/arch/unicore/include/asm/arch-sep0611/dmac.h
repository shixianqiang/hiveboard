// |--------|-----------|--------|-----------|
// the list followed is the naming convention
// |--------|-----------|--------|-----------|
// |  name  | meaning   |  name  | meaning   |
// |--------|-----------|--------|-----------|
// |  ADDR  | address   |  VECT  | vector    |
// |  CTL   | controller|  COMP  | component |
// |  INT   | interrupt |  RX    | receive   |
// |  MEM   | memory    |  TX    | transmit  |
// |  EN    | enable    |  STAT  | status    |
// |  MSK   | mask      |  CTRL  | controller|
// |  SLT   | select    |  SAMP  | sample    |
// |  SFT   | soft      |  CH    | channel   |
// |  STS   | status    |  CNT   | count(er) |
// |  PRIOR | priority  |  SUSP  | suspend   |
// |  HW    | hardware  |  SW    | software  | 
// |  DST   | dstination|  SRC   | source    |
// |  SEL   | select    |  POL   | polarity  |
// |  MAX   | maximum   |  BST   | burst     |
// |  ATUO  | automatic |  PROT  | protection|
// |  UPD   | update    |  PER   | peripheral|
// |  TRAN  | transfer  |  INCR  | increment |
// |  BLK   | block     |  SCAT  | scatter   |
// |  GATH  | gather    |  SZ    | size      |
// |  LLP   | linked list pointer            |
// |--------|-----------|--------|-----------|
//		    define Function
// |		CTL
// |			============================
// |		  	0: MEM 	to 	MEM,	DMAC; 
// |			1: MEM 	to 	PER, 	DMAC; 
// |			2: PER 	to 	MEM, 	DMAC; 
// |			3: PER 	to 	PER, 	DMAC; 
// |			4: PER 	to 	MEM, 	PER; 
// |			5: PER 	to 	PER, 	SRC PER; 
// |			6: MEM 	to 	PER,	PER; 
// |			7: PER 	to 	PER, 	DST PER;
// |			=============================
// |		
// |		DMAC1 Handshaking Interface Number£º
// |			0: sdio1_in
// |			1: sdio1_out
// |			2: sdio2_in
// |			3: sdio2_out
// |			4: nand_in
// |			5: nand_out
// |		DMAC2 Handshaking Interface Number£º
// |			0: ssi1_out
// |			1: ssi1_in
// |			2: ssi2_out
// |			3: ssi2_in
// |			4: ssi3_out
// |			5: ssi3_in
// |			6: i2s_out
// |			7: i2s_in
// |--------|-----------|--------|-----------|

#ifndef __DMAC_H_
#define __DMAC_H_

#include <asm/arch/vic.h>

#define DMAC_OT_ID			DMA_ID1
#define DMAC_OT_CH			DMA_CH0
#define DMAC_OT_MSCLK		MasterCLK	          
#define DMAC_OT_SRCADDR		0x45000000
#define DMAC_OT_DSTADDR		0x46000000
#define DMAC_OT_BLKSZ		1024
#define DMAC_OT_FIFOSIZE	NO_FIFO
#define DMAC_OT_SRCBITWIDTH	WIDTH16
#define DMAC_OT_DSTBITWIDTH	WIDTH16
#define DMAC_OT_SRCINCR		INCR
#define DMAC_OT_DSTINCR		INCR
#define DMAC_OT_SRCBURST	BUSRT4
#define DMAC_OT_DSTBURST	BUSRT4
#define DMAC_OT_SRCGATHER	FALSE
#define DMAC_OT_DSTSCATTER	FALSE
#define DMAC_OT_FLOWTYPE	MEM_TO_MEM_DMAC
#define DMAC_OT_SRCLLP		FALSE
#define DMAC_OT_DSTLLP		FALSE
#define DMAC_OT_CHPRIOR		CH_PRIO0
#define DMAC_OT_SRCHS		SDIO1_IN
#define DMAC_OT_DSTHS		SDIO1_OUT
#define DMAC_OT_SRCHsInte	SOFTWARE_HS
#define DMAC_OT_DSTHsInte	SOFTWARE_HS
#define DMAC_OT_SRCReLoad	SRC_DST_RELOAD_DIS
#define DMAC_OT_DSTReLoad	SRC_DST_RELOAD_DIS
#define DMAC_OT_INTEN		TRUE
#define DMAC_OT_DMAEN		TRUE		          

void DMAC_Enable(DMAC_ID pDmacId, BOOL pDmacEn);
void DMAC_SetBasicCfg(DMAC_CFG *pDmacCfg);
void DMAC_EnableChannel(DMAC_ID pDmacId, DMAC_CH pChId, BOOL pChEn);
void DMAC_Test(void);

#endif
