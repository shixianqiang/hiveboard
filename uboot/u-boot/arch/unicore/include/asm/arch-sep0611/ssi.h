/*********************************************************************
* Filename    :  ssi.h
* Author      :  SixRoom
* Date        :  2011-01-10
* Description :  This file is used to define timer
* Version  	  :  v1.00
**********************************************************************/
/*
|---------------|------------------|---------------|-----------------------------------------|
|    version    |       date       |     Author    |           modify infomation             |
|---------------|------------------|---------------|-----------------------------------------|
|    v1.00      |    2010-03-23    |  WuPeng       | 1) create this typedef file    		 |
|    v1.01      |    2011-01-10    |  SixRoom      | 1) modify some function         	     |
|---------------|------------------|---------------|-----------------------------------------|
*/
#ifndef   SSI_H_
#define   SSI_H_

#include 	<asm/arch/vic.h>

#define RXF_INT					(1<<4)
#define RXO_INT					(1<<3)
#define RXU_INT					(1<<2)
#define TXO_INT					(1<<1)
#define TXE_INT					(1)

#define BAUD(x)         		(x)     //x=2~65535, here range of x is 34~5000
#define FRAME_LEN(x)     		(x)     //x=3~15, here x=7 is effective
#define FRAME_NUM(x)     		(x)

#define TRANS_FT(x)       		(x)     //x=1~21  as the depth of the transfer fifo is 22.
#define RECEIV_FT(x)      		(x)     //x=1~21  as the depth of the receive fifo is 22

#define SCPOL(x)				(x)
#define SCPH(x)					(x)

//SSI DMA配置
#define DMA_TDL(x)       		(x) //dmatdl = FIFO_DEPTH - txtft + 1;
#define DMA_RDL(x)       		(x) //dmardl = txrft + 1;

/***********************************************************
*	Function	：	spi_fifo_clr
*	Parameter	：	
*	Return		：  
*	Description	：	SSI使能
***********************************************************/
#define SSI_Enable(ch,pSta)                            	\
do{                                               		\
	   *(RP)SSI_SSIENR_CH(ch) &= ~0x1;             		\
	   *(RP)SSI_SSIENR_CH(ch) |= pSta;             		\
  }while(0)
/***********************************************************
*	Function	：	ssi_int_unmask
*	Parameter	：	pSta  INT_MASK：屏蔽 INT_UNMASK：不屏蔽
*	Return		：  
*	Description	：	SSI中断屏蔽
***********************************************************/
#define SSI_MaskInt(ch, pIntNum, pSta)              \
do{                                               	\
	if(pSta == INT_UNMASK)							\
		*(RP)SSI_IMR_CH(ch) |= (pIntNum);			\
	else if(pSta == INT_MASK)						\
	    *(RP)SSI_IMR_CH(ch) &= ~(pIntNum);         	\
  }while(0)
/***********************************************************
*	Function	：	ssi_slavedevice_select
*	Parameter	：	
*	Return		：  
*	Description	：	SSI最多可以带两个从设备，CS0/CS1，此函数用于配置片选哪个设备
***********************************************************/
#define SSI_SelSlaveDev(ch, pSlaDev)      		\
do{                                         	\
	   *(RP)SSI_SER_CH(ch) = pSlaDev;           \
  }while(0)
/***********************************************************
*	Function	：	ssi_rtx_level
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
#define SSI_SetOperateMode(ch, pMode)   			\
do{                                               	\
	   *(RP)SSI_CONTROL0_CH(ch) &= ~(1<<11);		\
	   *(RP)SSI_CONTROL0_CH(ch) |= ((pMode)<<11);	\
}while(0)
/***********************************************************
*	Function	：	ssi_baud_set
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
#define SSI_SetBaud(ch,baud)		\
do{                                 \
	*(RP)SSI_BAUDR_CH(ch) = baud;   \
}while(0)
/***********************************************************
*	Function	：	ssi_rtx_level
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
#define SSI_SetMode(ch, pMode)   					\
do{                                               	\
	   *(RP)SSI_CONTROL0_CH(ch) &= ~(3<<8);			\
	   *(RP)SSI_CONTROL0_CH(ch) |= ((pMode)<<8);	\
}while(0)
/***********************************************************
*	Function	：	ssi_rtx_level
*	Parameter	：	clock_para	(时钟极性和相位配置：00模式、11模式)
*	Return		：  
*	Description	：	
***********************************************************/
#define SSI_SetPolar(ch, pSCPOL, pSCPH)   						\
do{                                               				\
	   *(RP)SSI_CONTROL0_CH(ch) &= ~(3<<6);						\
	   *(RP)SSI_CONTROL0_CH(ch) |= ((pSCPOL<<7)|((pSCPH)<<6));	\
}while(0)
/***********************************************************
*	Function	：	SSI_SetFrameLen
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
#define SSI_SetFrameLen(ch, pFrameLen)   			\
do{                                               	\
	   *(RP)SSI_CONTROL0_CH(ch) &= 0xfffffff0;		\
	   *(RP)SSI_CONTROL0_CH(ch) |= (pFrameLen&0x0f);\
}while(0)
/***********************************************************
*	Function	：	SSI_SetFrameNum
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
#define SSI_SetFrameNum(ch, pFrameNum)   			\
do{                                               	\
	   *(RP)SSI_CONTROL1_CH(ch) = pFrameNum;		\
}while(0)
/***********************************************************
*	Function	：	ssi_dma_config
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
#define SSI_SetRxDMA(ch,pRxDmaEn,pDmaLevel)  		\
 do{                                               	\
	   *(RP)SSI_DMARDLR_CH(ch) = pDmaLevel;         \
	   *(RP)SSI_DMACR_CH(ch) &= ~(1);				\
	   *(RP)SSI_DMACR_CH(ch) |= (pRxDmaEn);			\
   }while(0)
/***********************************************************
*	Function	：	ssi_dma_config
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
#define SSI_SetTxDMA(ch,pTxDmaEn,pDmaLevel)  		\
 do{                                               	\
	   *(RP)SSI_DMATDLR_CH(ch) = pDmaLevel;         \
	   *(RP)SSI_DMACR_CH(ch) &= ~(1<<1);			\
	   *(RP)SSI_DMACR_CH(ch) |= ((pTxDmaEn)<<1);	\
   }while(0)
/***********************************************************
*	Function	：	ssi_rtx_level
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
#define SSI_SetRxFifoLevel(ch, pRxFifoLevel)   			\
do{                                               		\
	   *(RP)SSI_RXFTLR_CH(ch)=(pRxFifoLevel-1);        	\
}while(0)
/***********************************************************
*	Function	：	ssi_rtx_level
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
#define SSI_SetTxFifoLevel(ch, pTxFifoLevel)   			\
do{                                               		\
	   *(RP)SSI_TXFTLR_CH(ch)=(pTxFifoLevel-1);			\
}while(0)

	  

extern void SSI_Test(void); 
#endif 

