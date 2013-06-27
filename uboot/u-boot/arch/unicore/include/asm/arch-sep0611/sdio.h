/*********************************************************************
* Filename    :  sdio.h
* Author      :  SixRoom
* Date        :  2011-01-14
* Description :  This file is used to define the interrupt source number
*                and some macros that will be used to processing the 
*                interrupt request
* Version  	  :  v1.04
**********************************************************************/
#ifndef SDIO_H_
#define SDIO_H_

#include 	<asm/arch/vic.h>

//Macro define for SDIO1_CTRL
#define    RST_CTRL             (1 << 0)
#define    RST_FIFO             (1 << 1)
#define    RST_DMA              (1 << 2)

//Macro define for SDIO1_INTMASK
#define    INT_CD           (1)
#define    INT_RE           (1 << 1 )
#define    INT_CP           (1 << 2 )
#define    INT_DTP          (1 << 3 )
#define    INT_TXDR         (1 << 4 )
#define    INT_RXDR         (1 << 5 )
#define    INT_RCRC         (1 << 6 )
#define    INT_DCRC         (1 << 7 )
#define    INT_RTO          (1 << 8 )
#define    INT_DTO          (1 << 9 )
#define    INT_HTO          (1 << 10)
#define    INT_FRUN         (1 << 11)
#define    INT_HLE          (1 << 12)
#define    INT_SBE          (1 << 13)
#define    INT_ACD          (1 << 14)
#define    INT_EBE          (1 << 15)

#define	SDIO_OT_ID			SDIO_ID1
#define	SDIO_OT_RESPTYPE	SHORT_RESP
#define	SDIO_OT_CARDTYPE	FOUR_WIRE
#define	SDIO_OT_DATADIR		READ
#define	SDIO_OT_CLKSRC		DIV0
#define	SDIO_OT_BSTWIDTH	MSIZE4

/***********************************************************
*	Function	：	SD_wait_cmd_done
*	Parameter	：	
*	Return		：  
*	Description	：	等待命令执行完成
***********************************************************/
#define SDIO_WaitCmdDone(dev)				\
	while(*(RP)SDIO_RINTSTS_CH(dev) & 0x4 == 0)
/***********************************************************
*	Function	：	SDIO_WaitTranDone
*	Parameter	：	
*	Return		：  
*	Description	：	等待数据传输完成
***********************************************************/
#define SDIO_WaitTranDone(dev)				\
	while(*(RP)SDIO_RINTSTS_CH(dev) & 0x8 == 0)
/***********************************************************
*	Function	：	SD_wait_cmd_taken_by_CIU
*	Parameter	：	
*	Return		：  
*	Description	：	等待命令被CIU接收
***********************************************************/
#define SDIO_WaitCmdTakenByCIU(dev)				\
	while((*(RP)SDIO_CMD_CH(dev) & 0x80000000))

void SDIO_Test(void);

#endif /*SDIO_H_*/
