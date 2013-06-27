/* linux/arch/unicore/mach-sep0611/include/mach/regs-sdio.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *  fjj <fanjianjun@wxseuic.com>
 *
 * Southeast University ASIC SoC support
 *
 * SD Driver for SEP0611 Board
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  09-08-2011 fjj initial version
 */

#ifndef __ASM_ARCH_REGS_SDIO_H
#define __ASM_ARCH_REGS_SDIO_H

/* macro define for SDIO_CTRL */
#define		SDIO_CTRL_RST(x)            		((x) << 0)
#define    	SDIO_FIFO_RST(x)          			((x) << 1)
#define    	SDIO_DMA_RST(x)             		((x) << 2)
#define    	SDIO_INT_EN(x)              		((x) << 4)
#define    	SDIO_DMA_EN(x)              		((x) << 5)
#define    	SDIO_READ_WAIT(x)           		((x) << 6)
#define    	SDIO_SEND_IRQ_RESP(x)       		((x) << 7)
#define    	SDIO_ABT_RD_DAT(x)       			((x) << 8)
#define		SDIO_BIG_ENDIAN_EN(x)				((x) << 28)
#define		SDIO_DAT3_DETECT_EN(x)				((x) << 29)

/* macro define for SDIO_CLKENA */
#define     SDIO_CCLK_EN(x)              		(x << 0 )
#define     SDIO_CCLK_LW_PWR(x)         		(x << 16)
 
/* macro define for SDIO_FIFOTH */
#define   	TX_WMARK(x)                   		((x) << 0)  
#define   	RX_WMARK(x)                   		((x) << 16)
#define   	MSIZE8                        		(2 << 28)  

/* macro define for SDIO_RINTSTS, SDIO_INTMASK, SDIO_MINTSTS */
#define    	SDIO_INT_CDT(x)           			((x) << 0)
#define    	SDIO_INT_RE(x)           			((x) << 1)
#define    	SDIO_INT_CD(x)           			((x) << 2)
#define    	SDIO_INT_DTO(x)          			((x) << 3)
#define    	SDIO_INT_TXDR(x)         			((x) << 4)
#define    	SDIO_INT_RXDR(x)         			((x) << 5)
#define    	SDIO_INT_RCRC(x)        	 		((x) << 6)
#define    	SDIO_INT_DCRC(x)         			((x) << 7)
#define    	SDIO_INT_RTO(x)          			((x) << 8)
#define    	SDIO_INT_DRTO(x)          			((x) << 9)
#define    	SDIO_INT_HTO(x)          			((x) << 10)
#define    	SDIO_INT_FRUN(x)         			((x) << 11)
#define    	SDIO_INT_HLE(x)          			((x) << 12)
#define    	SDIO_INT_SBE(x)          			((x) << 13)
#define    	SDIO_INT_ACD(x)          			((x) << 14)
#define    	SDIO_INT_EBE(x)         	 		((x) << 15)
#define   	SDIO_INT_SDIO(x)           			((x) << 16)

/* macro define for SDIO_CMD */
#define   	CMD0     							(0	<<  0)
#define    	SDIO_CMD_RESP_EXPE(x)        		((x) << 6)
#define    	SDIO_CMD_LONG_RESP(x)        		((x) << 7)
#define    	SDIO_CMD_CHK_RESP_CRC(x)     		((x) << 8)
#define    	SDIO_CMD_HAVE_DAT_TRAN(x)    		((x) << 9)
#define    	SDIO_CMD_WRITE(x)            		((x) << 10)
#define    	SDIO_CMD_STR_TRAN(x)         		((x) << 11) 
#define    	SDIO_CMD_SET_STOP(x)         		((x) << 12)
#define    	SDIO_CMD_WAIT_DAT(x)         		((x) << 13)
#define    	SDIO_CMD_STP_ABT(x)          		((x) << 14)
#define    	SDIO_CMD_SEND_INIT(x)        		((x) << 15)
#define    	SDIO_CMD_CARD_NUM(x)         		((x) << 16)
#define    	SDIO_CMD_UPDATE_CLK_REG_ONLY(x) 	((x) << 21)   	 
#define    	SDIO_CMD_START              		(1 << 31)

/* for sdio1 */
#define		DMA_CH_FOR_SDIO1_CTL_L_WR	\
	     	( DMAC_SRC_LLP_EN    (1) | DMAC_DST_LLP_EN        (0) | DMAC_SRC_MASTER_SEL (0) \
   	  	 	| DMAC_DST_MASTER_SEL(1) | DMAC_TRAN_TYPE_FLOW_CTL(1) | DMAC_DST_SCAT_EN    (0) \
   	  	 	| DMAC_SRC_GATH_EN   (0) | DMAC_SRC_BST_SZ        (2) | DMAC_DST_BST_SZ     (2) \
   	  	 	| DMAC_SRC_INCR      (0) | DMAC_DST_INCR          (2) | DMAC_SRC_TRAN_WIDTH (2) \
         	| DMAC_DST_TRAN_WIDTH(2) | DMAC_INT_EN            (1) )

#define		DMA_CH_FOR_SDIO1_CTL_L_RD	\
	     	( DMAC_SRC_LLP_EN    (0) | DMAC_DST_LLP_EN        (1) | DMAC_SRC_MASTER_SEL (1) \
   	  	 	| DMAC_DST_MASTER_SEL(0) | DMAC_TRAN_TYPE_FLOW_CTL(2) | DMAC_DST_SCAT_EN    (0) \
   	  	 	| DMAC_SRC_GATH_EN   (0) | DMAC_SRC_BST_SZ        (2) | DMAC_DST_BST_SZ     (2) \
   	  	 	| DMAC_SRC_INCR      (2) | DMAC_DST_INCR          (0) | DMAC_SRC_TRAN_WIDTH (2) \
         	| DMAC_DST_TRAN_WIDTH(2) | DMAC_INT_EN            (1) )

#define 	DMA_CH_FOR_SDIO1_CFG_L_WR	\
		 	( DMAC_AUTO_RELOAD_DST(0) | DMAC_AUTO_RELOAD_SRC(0) | DMAC_MAX_AMBA_BST_LEN(0)	\
   	  	 	| DMAC_SRC_HS_POL     (0) | DMAC_DST_HS_POL     (0) | DMAC_BUS_LOCK        (0)	\
   	  	 	| DMAC_CH_LOCK        (0) | DMAC_BUS_LOCK_LEVEL (0) | DMAC_CH_LOCK_LEVEL   (0)	\
   	  	 	| DMAC_HW_SW_SEL_SRC  (1) | DMAC_HW_SW_SEL_DST  (0) | DMAC_FIFO_EMPTY      (1)	\
		 	| DMAC_CH_SUSP        (0) | DMAC_CH_PRIOR       (0) )

#define 	DMA_CH_FOR_SDIO1_CFG_L_RD	\
		 	( DMAC_AUTO_RELOAD_DST(0) | DMAC_AUTO_RELOAD_SRC(0) | DMAC_MAX_AMBA_BST_LEN(0)	\
   	  	 	| DMAC_SRC_HS_POL     (0) | DMAC_DST_HS_POL     (0) | DMAC_BUS_LOCK        (0)	\
   	  	 	| DMAC_CH_LOCK        (0) | DMAC_BUS_LOCK_LEVEL (0) | DMAC_CH_LOCK_LEVEL   (0)	\
   	  	 	| DMAC_HW_SW_SEL_SRC  (0) | DMAC_HW_SW_SEL_DST  (1) | DMAC_FIFO_EMPTY      (1)	\
		 	| DMAC_CH_SUSP        (0) | DMAC_CH_PRIOR       (0) )

#define 	DMA_CH_FOR_SDIO1_CFG_H_WR	\
		 	( DMAC_DST_PER        (1) | DMAC_SRC_PER (0) | DMAC_SRC_STAT_UPD_EN(0) \
   	  		| DMAC_DST_STAT_UPD_EN(0) | DMAC_PROT_CTL(1) | DMAC_FIFO_MODE      (1) \
   	  	 	| DMAC_FLOW_CTL_MODE  (0) )

#define 	DMA_CH_FOR_SDIO1_CFG_H_RD	\
		 	( DMAC_DST_PER        (1) | DMAC_SRC_PER (0) | DMAC_SRC_STAT_UPD_EN(0) \
   	  		| DMAC_DST_STAT_UPD_EN(0) | DMAC_PROT_CTL(1) | DMAC_FIFO_MODE      (1) \
   	  	 	| DMAC_FLOW_CTL_MODE  (0) )

/* for sdio2 */
#define		DMA_CH_FOR_SDIO2_CTL_L_WR	\
	     	( DMAC_SRC_LLP_EN    (1) | DMAC_DST_LLP_EN        (0) | DMAC_SRC_MASTER_SEL (0) \
   	  	 	| DMAC_DST_MASTER_SEL(1) | DMAC_TRAN_TYPE_FLOW_CTL(1) | DMAC_DST_SCAT_EN    (0) \
   	  	 	| DMAC_SRC_GATH_EN   (0) | DMAC_SRC_BST_SZ        (2) | DMAC_DST_BST_SZ     (2) \
   	  	 	| DMAC_SRC_INCR      (0) | DMAC_DST_INCR          (2) | DMAC_SRC_TRAN_WIDTH (2) \
         	| DMAC_DST_TRAN_WIDTH(2) | DMAC_INT_EN            (1) )

#define		DMA_CH_FOR_SDIO2_CTL_L_RD	\
	     	( DMAC_SRC_LLP_EN    (0) | DMAC_DST_LLP_EN        (1) | DMAC_SRC_MASTER_SEL (1) \
   	  	 	| DMAC_DST_MASTER_SEL(0) | DMAC_TRAN_TYPE_FLOW_CTL(2) | DMAC_DST_SCAT_EN    (0) \
   	  	 	| DMAC_SRC_GATH_EN   (0) | DMAC_SRC_BST_SZ        (2) | DMAC_DST_BST_SZ     (2) \
   	  	 	| DMAC_SRC_INCR      (2) | DMAC_DST_INCR          (0) | DMAC_SRC_TRAN_WIDTH (2) \
         	| DMAC_DST_TRAN_WIDTH(2) | DMAC_INT_EN            (1) )

#define 	DMA_CH_FOR_SDIO2_CFG_L_WR	\
		 	( DMAC_AUTO_RELOAD_DST(0) | DMAC_AUTO_RELOAD_SRC(0) | DMAC_MAX_AMBA_BST_LEN(0)	\
   	  	 	| DMAC_SRC_HS_POL     (0) | DMAC_DST_HS_POL     (0) | DMAC_BUS_LOCK        (0)	\
   	  	 	| DMAC_CH_LOCK        (0) | DMAC_BUS_LOCK_LEVEL (0) | DMAC_CH_LOCK_LEVEL   (0)	\
   	  	 	| DMAC_HW_SW_SEL_SRC  (1) | DMAC_HW_SW_SEL_DST  (0) | DMAC_FIFO_EMPTY      (1)	\
		 	| DMAC_CH_SUSP        (0) | DMAC_CH_PRIOR       (0) )

#define 	DMA_CH_FOR_SDIO2_CFG_L_RD	\
		 	( DMAC_AUTO_RELOAD_DST(0) | DMAC_AUTO_RELOAD_SRC(0) | DMAC_MAX_AMBA_BST_LEN(0)	\
   	  	 	| DMAC_SRC_HS_POL     (0) | DMAC_DST_HS_POL     (0) | DMAC_BUS_LOCK        (0)	\
   	  	 	| DMAC_CH_LOCK        (0) | DMAC_BUS_LOCK_LEVEL (0) | DMAC_CH_LOCK_LEVEL   (0)	\
   	  	 	| DMAC_HW_SW_SEL_SRC  (0) | DMAC_HW_SW_SEL_DST  (1) | DMAC_FIFO_EMPTY      (1)	\
		 	| DMAC_CH_SUSP        (0) | DMAC_CH_PRIOR       (0) )

#define 	DMA_CH_FOR_SDIO2_CFG_H_WR	\
		 	( DMAC_DST_PER        (3) | DMAC_SRC_PER (2) | DMAC_SRC_STAT_UPD_EN(0) \
   	  		| DMAC_DST_STAT_UPD_EN(0) | DMAC_PROT_CTL(1) | DMAC_FIFO_MODE      (1) \
   	  	 	| DMAC_FLOW_CTL_MODE  (0) )

#define 	DMA_CH_FOR_SDIO2_CFG_H_RD	\
		 	( DMAC_DST_PER        (3) | DMAC_SRC_PER (2) | DMAC_SRC_STAT_UPD_EN(0) \
   	  		| DMAC_DST_STAT_UPD_EN(0) | DMAC_PROT_CTL(1) | DMAC_FIFO_MODE      (1) \
   	  	 	| DMAC_FLOW_CTL_MODE  (0) )

#endif /* __ASM_ARCH_REGS_SDIO_H */
