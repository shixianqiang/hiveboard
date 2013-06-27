/* linux/arch/unicore/mach-sep0611/include/mach/dma.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *
 * Southeast University ASIC SoC support
 *
 * SEP0611 DMA resource allocation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  05-31-2011	cgm initial version
 *
 */
 
#ifndef __ASM_ARCH_DMA_H
#define __ASM_ARCH_DMA_H 

/***************************************************************************************************************/
/******************************************** DMAC channel allocation ******************************************/
/***************************************************************************************************************/

/*
 * DMAC 1
 */
#define DMA_NAND_RDWR_CH 	0
#define DMA_SDIO1_CH		1
#define DMA_SDIO2_CH		2

/*
 * DMAC 2
 */
#define DMA_I2S_TX_CH 		0
#define DMA_I2S_RX_CH 		1
#define DMA_SSI1_TX_CH		2
#define DMA_SSI1_RX_CH		3
#define DMA_SSI2_TX_CH		4
#define DMA_SSI2_RX_CH		5
#define DMA_SSI3_TX_CH		6
#define DMA_SSI3_RX_CH		7

/***************************************************************************************************************/
/******************************************** DMAC hardware handshake number ***********************************/
/***************************************************************************************************************/

/*
 * DMAC 1 and 2 are together
 */
#define DMAC_HS_SDIO1_TX	0
#define DMAC_HS_SDIO1_RX	1
#define DMAC_HS_SDIO2_TX	2
#define DMAC_HS_SDIO2_RX	3
#define DMAC_HS_NAND_WR		4
#define DMAC_HS_NAND_RD		5
#define DMAC_HS_I2S_TX		6
#define DMAC_HS_I2S_RX		7


/***************************************************************************************************************/
/******************************************** DMAC served device id ********************************************/
/***************************************************************************************************************/

/*
 * DMAC 1 and 2 are together
 */
#define DMA_NAND_DEV_ID		0
#define DMA_SDIO1_DEV_ID	1
#define DMA_SDIO2_DEV_ID	2
#define DMA_ALSA_DEV_ID		3
#define DMA_SSI1_DEV_ID		4
#define DMA_SSI2_DEV_ID		5
#define DMA_SSI3_DEV_ID		6

/***************************************************************************************************************/
/******************************************** DMAC channel choose **********************************************/
/***************************************************************************************************************/

/*
 * Two levels MICRO replace to choose channel registers aotumaticly! you can use DMAC1_SAR_CH(0) to get DMAC1_SAR0_V,
 * easy to change the channel.
 */
#define DMAC1_SAR_CH(x)		_DMAC1_SAR_CH(x)
#define DMAC1_DAR_CH(x)		_DMAC1_DAR_CH(x)
#define DMAC1_LLP_CH(x)		_DMAC1_LLP_CH(x)
#define DMAC1_CTL_CH(x)		_DMAC1_CTL_CH(x)
#define DMAC1_SSTAT_CH(x)	_DMAC1_SSTAT_CH(x)
#define DMAC1_DSTAT_CH(x)	_DMAC1_DSTAT_CH(x)
#define DMAC1_SSTATR_CH(x)	_DMAC1_SSTATR_CH(x)
#define DMAC1_DSTATR_CH(x)	_DMAC1_DSTATR_CH(x)
#define DMAC1_CFG_CH(x)		_DMAC1_CFG_CH(x)
#define DMAC1_SGR_CH(x)		_DMAC1_SGR_CH(x)
#define DMAC1_DSR_CH(x)		_DMAC1_DSR_CH(x)

/* you should not care this when using */
#define _DMAC1_SAR_CH(x)	DMAC1_SAR##x##_V
#define _DMAC1_DAR_CH(x)	DMAC1_DAR##x##_V
#define _DMAC1_LLP_CH(x)	DMAC1_LLP##x##_V
#define _DMAC1_CTL_CH(x)	DMAC1_CTL##x##_V
#define _DMAC1_SSTAT_CH(x)	DMAC1_SSTAT##x##_V
#define _DMAC1_DSTAT_CH(x)	DMAC1_DSTAT##x##_V
#define _DMAC1_SSTATR_CH(x)	DMAC1_SSTATR##x##_V
#define _DMAC1_DSTATR_CH(x)	DMAC1_DSTATR##x##_V
#define _DMAC1_CFG_CH(x)	DMAC1_CFG##x##_V
#define _DMAC1_SGR_CH(x)	DMAC1_SGR##x##_V
#define _DMAC1_DSR_CH(x)	DMAC1_DSR##x##_V

/* Two levels MICRO replace to choose channel registers aotumaticly!*/
#define DMAC2_SAR_CH(x)		_DMAC2_SAR_CH(x)
#define DMAC2_DAR_CH(x)		_DMAC2_DAR_CH(x)
#define DMAC2_LLP_CH(x)		_DMAC2_LLP_CH(x)
#define DMAC2_CTL_CH(x)		_DMAC2_CTL_CH(x)
#define DMAC2_SSTAT_CH(x)	_DMAC2_SSTAT_CH(x)
#define DMAC2_DSTAT_CH(x)	_DMAC2_DSTAT_CH(x)
#define DMAC2_SSTATR_CH(x)	_DMAC2_SSTATR_CH(x)
#define DMAC2_DSTATR_CH(x)	_DMAC2_DSTATR_CH(x)
#define DMAC2_CFG_CH(x)		_DMAC2_CFG_CH(x)
#define DMAC2_SGR_CH(x)		_DMAC2_SGR_CH(x)
#define DMAC2_DSR_CH(x)		_DMAC2_DSR_CH(x)

/* you should not care this when using */
#define _DMAC2_SAR_CH(x)	DMAC2_SAR##x##_V
#define _DMAC2_DAR_CH(x)	DMAC2_DAR##x##_V
#define _DMAC2_LLP_CH(x)	DMAC2_LLP##x##_V
#define _DMAC2_CTL_CH(x)	DMAC2_CTL##x##_V
#define _DMAC2_SSTAT_CH(x)	DMAC2_SSTAT##x##_V
#define _DMAC2_DSTAT_CH(x)	DMAC2_DSTAT##x##_V
#define _DMAC2_SSTATR_CH(x)	DMAC2_SSTATR##x##_V
#define _DMAC2_DSTATR_CH(x)	DMAC2_DSTATR##x##_V
#define _DMAC2_CFG_CH(x)	DMAC2_CFG##x##_V
#define _DMAC2_SGR_CH(x)	DMAC2_SGR##x##_V
#define _DMAC2_DSR_CH(x)	DMAC2_DSR##x##_V


/***************************************************************************************************************/
/************************************* registers' region declare ***********************************************/
/***************************************************************************************************************/
// DMAC Configuration Register
#define    DMAC_CFG_REG_EN(x)                  (x)             	//Enable DMAC

// DMAC Channel Enable Register
#define    DMAC_CH_EN(x)                       (x)           	//Enable the Channel
#define    DMAC_CH_WRITE_EN(x)                 (x<<8)       	//Channel Enable's Write Enable

//DMAC Channel Interrupt Mask Register                  
#define    DMAC_INT_MASK(x)                    (x)      
#define    DMAC_INT_WE(x)                      (x)      
                                                        

// DMAC Configuration Register for Channel x, x=0~7
#define    DMAC_CH_PRIOR(x)                    (x << 5)      	//Channel Priority
#define    DMAC_CH_SUSP(x)                     (x << 8)       	//Channel Suspend
#define    DMAC_FIFO_EMPTY(x)                  (x << 9)    		//read only
#define    DMAC_HW_SW_SEL_DST(x)               (x << 10)    	//Destination Software or Hardware Handshaking Select
#define    DMAC_HW_SW_SEL_SRC(x)               (x << 11)       	//Source Software or Hardware Handshaking Select
#define    DMAC_CH_LOCK_LEVEL(x)               (x << 12)     	//Channel Lock Level
#define    DMAC_BUS_LOCK_LEVEL(x)              (x << 14)        //Bus Lock Level
#define    DMAC_CH_LOCK(x)                     (x << 16)      	//Channel Lock Bit
#define    DMAC_BUS_LOCK(x)                    (x << 17)     	//Bus Lock Bit 
#define    DMAC_DST_HS_POL(x)                  (x << 18)      	//DST Handshaking Interface Polarity
#define    DMAC_SRC_HS_POL(x)                  (x << 19)        //SRC Handshaking Interface Polarity
#define    DMAC_MAX_AMBA_BST_LEN(x)            (x << 20)        //Maximum AMBA Burst Length
#define    DMAC_AUTO_RELOAD_SRC(x)             (x << 30)        //Automatic Source Reload
#define    DMAC_AUTO_RELOAD_DST(x)             (x << 31)        //Automatic Destination Reload
#define    DMAC_FLOW_CTL_MODE(x)               (x)              //Flow Control Mode
#define    DMAC_FIFO_MODE(x)                   (x << 1)         //FIFO Mode Select
#define    DMAC_PROT_CTL(x)                    (x << 2)         //Protection Control bits used to drive the AHB HPROT[3:1] bus
#define    DMAC_DST_STAT_UPD_EN(x)             (x << 5)         //Destination Status Update Enable
#define    DMAC_SRC_STAT_UPD_EN(x)             (x << 6)         //Source Status Update Enable
#define    DMAC_SRC_PER(x)                     (x << 7)         //x=0~15,Assigns a hardware handshaking interface to the source of channel of x 
#define    DMAC_DST_PER(x)                     (x << 11)        //x=0~15,Assigns a hardware handshaking interface to the destination of channel of x

// DMAC Control Register for Channel x
#define    DMAC_INT_EN(x)                      (x)              //Interrupt Enable Bit
#define    DMAC_DST_TRAN_WIDTH(x)              (x << 1)         //Destination Transfer Width
#define    DMAC_SRC_TRAN_WIDTH(x)              (x << 4)         //Source Transfer Width
#define    DMAC_DST_INCR(x)                    (x << 7)         //Destination Address Increment
#define    DMAC_SRC_INCR(x)                    (x << 9)         //Source Address Increment
#define    DMAC_DST_BST_SZ(x)                  (x << 11)        //Destination Burst Transaction Length
#define    DMAC_SRC_BST_SZ(x)                  (x << 14)        //Source Burst Transaction Length
#define    DMAC_SRC_GATH_EN(x)                 (x << 17)        //Source gather enable bit
#define    DMAC_DST_SCAT_EN(x)                 (x << 18)        //Destination Scatter enable bit
#define    DMAC_TRAN_TYPE_FLOW_CTL(x)          (x << 20)        //Transfer Type and Flow Control
#define    DMAC_DST_MASTER_SEL(x)              (x << 23)        //Destination Master Select
#define    DMAC_SRC_MASTER_SEL(x)              (x << 25)        //Source Master Select
#define    DMAC_DST_LLP_EN(x)                  (x << 27)        //Block chaining enable on the destination side 
#define    DMAC_SRC_LLP_EN(x)                  (x << 28)        //Block chaining enable on the source side
#define    DMAC_BLK_TRAN_SZ(x)                 (x)              //Block Tranfer Size
#define    DMAC_DONE(x)                        (x << 12 )       //Done bit

// DMAC Linked List Pointer Register for Channel x 
#define    DMAC_LIST_MASTER_SEL(x)             (x)              //List Master Select
#define    DMAC_LOC(x)                         (x << 2)       	//Start Address in Memory of next LLI 

// DMAC Destination Scatter Register for Channel x
#define    DMAC_DST_SCAT_INTERVAL(x)           (x)            	//Destination Scatter Interval
#define    DMAC_DST_SCAT_CNT(x)                (x << 20)    	//Destination Scatter Count

// DMAC Source Gather Register for Channel x
#define    DMAC_SRC_GATH_INTERVAL(x)           (x)          	//Source Gather Interval
#define    DMAC_SRC_GATH_CNT(x)                (x << 20)     	//Source Gather Count

#endif /* __ASM_ARCH_DMA_H */

