/*********************************************************************
* Filename    :  sdio.h
* Author      :  SixRoom
* Date        :  2011-01-19
* Description :  This file is used to define the sdio
* Version  	  :  v1.03
**********************************************************************/
/*
|---------------|------------------|---------------|-----------------------------------------|
|    version    |       date       |     Author    |           modify infomation             |
|---------------|------------------|---------------|-----------------------------------------|
|    v1.00      |    2010-03-22    |  chenxiaoshan | 1) create this typedef file    		 |
|    v1.01      |    2010-09-10    |  SixRoom      | 1) modify some content                  |
|    v1.02      |    2010-12-15    |  SixRoom      | 1) update and add some content          |
|    v1.03      |    2011-01-19    |  SixRoom      | 1) add some new content				 |
|---------------|------------------|---------------|-----------------------------------------|
*/
#include "../include/sdio.h"
#include "../include/sdio_protocol.h"
#include "../include/dmac.h"
#include "../include/uart.h"

/***********************************************************
*	Function	：	SD_reset
*	Parameter	：	
*	Return		：  
*	Description	：	SD卡复位操作
***********************************************************/
void SDIO_Reset(SDIO_ID pId, U32 pRstNum)
{
	*(RP)SDIO_CTRL_CH(pId) |=((pRstNum) & 0x07);
}
/***********************************************************
*	Function	：	SD_dma_enable
*	Parameter	：	
*	Return		：  
*	Description	：	SD卡DMA传输使能
***********************************************************/
void SDIO_EnableDma(SDIO_ID pId, BOOL pSta)
{
	if(pSta == TRUE)
	{
		*(RP)SDIO_CTRL_CH(pId)	|=	(1<<5);
	}
	else if(pSta == FALSE)
	{
		*(RP)SDIO_CTRL_CH(pId)	&=	~(1<<5);
	}
}
/***********************************************************
*	Function	：	SD_int_enable
*	Parameter	：	
*	Return		：  
*	Description	：	SD卡中断使能
***********************************************************/
void SDIO_EnableInt(SDIO_ID pId, BOOL pSta)
{
	if(pSta == TRUE)
	{
		*(RP)SDIO_CTRL_CH(pId)	|=	(1<<4);
	}
	else if(pSta == FALSE)
	{
		*(RP)SDIO_CTRL_CH(pId)	&=	~(1<<4);
	}
}
/***********************************************************
 *	Function	：	SD_enable_power
 *	Parameter	：	
 *	Return		：  
 *	Description	：	指定的SD卡上电
 ***********************************************************/
void SDIO_EnablePower(SDIO_ID pId, BOOL pSta)
{
	if(pSta == TRUE)
	{
		*(RP)SDIO_PWREN_CH(pId)	|=	1;
	}
	else if(pSta == FALSE)
	{
		*(RP)SDIO_PWREN_CH(pId)	&=	0;
	}
}
/***********************************************************
 *	Function	：	SD_rawint_clr
 *	Parameter	：	
 *	Return		：  
 *	Description	：	中断清除
 ***********************************************************/
void SDIO_ClrRawInt(SDIO_ID pId, U32 pNum)
{
	*(RP)SDIO_RINTSTS_CH(pId) |= pNum;
}
/***********************************************************
*	Function	：	SDIO_MaskInt
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_MaskInt(SDIO_ID pId, U32 pIntNum, INT_MASK_SET pSta)
{
	if(pSta == INT_MASK)
	{
		*(RP)SDIO_INTMASK_CH(pId) &= ~(pIntNum);
	}
	else if(pSta == INT_UNMASK)
	{
		*(RP)SDIO_INTMASK_CH(pId) |= (pIntNum);
	}
}
/***********************************************************
*	Function	：	SDIO_RstCMDReg
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_RstCMDReg(SDIO_ID pId)
{
	*(RP)SDIO_CMD_CH(pId) = 0;
}
/***********************************************************
*	Function	：	SDIO_StartCMD
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_StartCMD(SDIO_ID pId)
{
	*(RP)SDIO_CMD_CH(pId) |= (1<<31);
}
/***********************************************************
*	Function	：	SDIO_UpdateClock
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_UpdateClock(SDIO_ID pId, BOOL pSta)
{
	if(pSta == TRUE)
	{
		*(RP)SDIO_CMD_CH(pId) |= (1<<21);
	}
	else if(pSta == FALSE)
	{
		*(RP)SDIO_CMD_CH(pId) &= ~(1<<21);
	}
}
/***********************************************************
*	Function	：	SDIO_EnableInitArry
*	Parameter	：	
*	Return		：  
*	Description	：	only cmd0
***********************************************************/
void SDIO_EnableInitArry(SDIO_ID pId, BOOL pSta)
{
	if(pSta == TRUE)
	{
		*(RP)SDIO_CMD_CH(pId) |= (1<<15);
	}
	else if(pSta == FALSE)
	{
		*(RP)SDIO_CMD_CH(pId) &= ~(1<<15);
	}
}
/***********************************************************
*	Function	：	SDIO_EnableStopAbort
*	Parameter	：	
*	Return		：  
*	Description	：	only cmd15
***********************************************************/
void SDIO_EnableStopAbort(SDIO_ID pId, BOOL pSta)
{
	if(pSta == TRUE)
	{
		*(RP)SDIO_CMD_CH(pId) |= (1<<14);
	}
	else if(pSta == FALSE)
	{
		*(RP)SDIO_CMD_CH(pId) &= ~(1<<14);
	}
}
/***********************************************************
*	Function	：	SDIO_EnableWaitData
*	Parameter	：	
*	Return		：  
*	Description	：	it's useful when R/W sd sector
***********************************************************/
void SDIO_EnableWaitData(SDIO_ID pId, BOOL pSta)
{
	if(pSta == TRUE)
	{
		*(RP)SDIO_CMD_CH(pId) |= (1<<13);
	}
	else if(pSta == FALSE)
	{
		*(RP)SDIO_CMD_CH(pId) &= ~(1<<13);
	}
}
/***********************************************************
*	Function	：	SDIO_EnableStopCMD
*	Parameter	：	
*	Return		：  
*	Description	：	it's useful when R/W sd sector
***********************************************************/
void SDIO_EnableStopCMD(SDIO_ID pId, BOOL pSta)
{
	if(pSta == TRUE)
	{
		*(RP)SDIO_CMD_CH(pId) |= (1<<12);
	}
	else if(pSta == FALSE)
	{
		*(RP)SDIO_CMD_CH(pId) &= ~(1<<12);
	}
}
/***********************************************************
*	Function	：	SDIO_SetRWDir
*	Parameter	：	
*	Return		：  
*	Description	：	it's useful when R/W sd sector
***********************************************************/
void SDIO_SetRWDir(SDIO_ID pId, RW_DIR pSta)
{
	if(pSta == WRITE)
	{
		*(RP)SDIO_CMD_CH(pId) |= (1<<10);
	}
	else if(pSta == READ)
	{
		*(RP)SDIO_CMD_CH(pId) &= ~(1<<10);
	}
}
/***********************************************************
*	Function	：	SDIO_SetDataExChangeStatus
*	Parameter	：	
*	Return		：  
*	Description	：	it's useful only when tansfer data,
* 					indicate that it's exchanging data
***********************************************************/
void SDIO_SetDataExChangeStatus(SDIO_ID pId, BOOL pSta)
{
	if(pSta == TRUE)
	{
		*(RP)SDIO_CMD_CH(pId) |= (1<<9);
	}
	else if(pSta == FALSE)
	{
		*(RP)SDIO_CMD_CH(pId) &= ~(1<<9);
	}
}
/***********************************************************
*	Function	：	SDIO_SetCRC
*	Parameter	：	
*	Return		：  
*	Description	：	it's useful only when RW data,
* 					it's nouse for cmd
***********************************************************/
void SDIO_SetCRC(SDIO_ID pId, BOOL pEn)
{
	if(pEn == TRUE)
	{
		*(RP)SDIO_CMD_CH(pId) |= (1<<8);
	}
	else if(pEn == FALSE)
	{
		*(RP)SDIO_CMD_CH(pId) &= ~(1<<8);
	}
}
/***********************************************************
*	Function	：	SDIO_SetResp
*	Parameter	：	
*	Return		：  
*	Description	：	it's useful only when tansfer data,
* 					indicate that it's exchanging data
***********************************************************/
void SDIO_SetResp(SDIO_ID pId, BOOL pEn, RESP_LEN pLen)
{
	if(pEn == TRUE)
	{
		*(RP)SDIO_CMD_CH(pId) |= (1<<6)|(pLen<<7);
	}
	else if(pEn == FALSE)
	{
		*(RP)SDIO_CMD_CH(pId) &= ~(1<<6);
	}
}
/***********************************************************
*	Function	：	SDIO_GetRespValue
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_GetRespValue(SDIO_ID pId, U32 *pResp)
{
	*(pResp+0) = *(RP)SDIO_RESP0_CH(pId);
	*(pResp+1) = *(RP)SDIO_RESP1_CH(pId);
	*(pResp+2) = *(RP)SDIO_RESP2_CH(pId);
	*(pResp+3) = *(RP)SDIO_RESP3_CH(pId);
}
/***********************************************************
*	Function	：	SDIO_SetCmdSn
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SetCmdSn(SDIO_ID pId, U32 pCmdSn)
{
	*(RP)SDIO_CMD_CH(pId) &= ~(0x3f);
	*(RP)SDIO_CMD_CH(pId) |= (pCmdSn&0x3f);
}
/***********************************************************
*	Function	：	SDIO_SetCmdArg
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SetCmdArg(SDIO_ID pId, U32 pCmdArg)
{
	*(RP)SDIO_CMD_CH(pId) = pCmdArg;
}
/***********************************************************
*	Function	：	SDIO_SetCardType
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SetCardType(SDIO_ID pId, CARD_TYPE pType)
{
	*(RP)SDIO_CTYPE_CH(pId) = pType;
}
/***********************************************************
*	Function	：	SDIO_SetClock
*	Parameter	：	
*	Return		：  
*	Description	：	pLowPowerEn: 一般仅对 MMC 和 SD 存储卡设置；
* 								对SDIO 卡，如果需要检测中断，不应该停止时钟
***********************************************************/
void SDIO_SetClock(SDIO_ID pId, CLOCK_SRC pSrc, U32 pDiv, BOOL pClockEn, BOOL pLowPowerEn)
{
	*(RP)SDIO_CLKSRC_CH(pId) = pSrc;
	*(RP)SDIO_CLKDIV_CH(pId) &= ~(0xff << (pSrc*8));
	*(RP)SDIO_CLKDIV_CH(pId) |= (pDiv << (pSrc*8));
	*(RP)SDIO_CLKENA_CH(pId) =  (pClockEn) | (pLowPowerEn << 16); 
}
/***********************************************************
*	Function	：	SDIO_SetDataTimeOut
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SetDataTimeOut(SDIO_ID pId, U32 pTimeOut)
{
	*(RP)SDIO_TMOUT_CH(pId) &= ~(0xff);
	*(RP)SDIO_TMOUT_CH(pId) |= (pTimeOut&0xff);
}
/***********************************************************
*	Function	：	SDIO_SetRespTimeOut
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SetRespTimeOut(SDIO_ID pId, U32 pTimeOut)
{
	*(RP)SDIO_TMOUT_CH(pId) &= 0x000000ff;
	*(RP)SDIO_TMOUT_CH(pId) |= ((pTimeOut&0xff)<<8);
}
/***********************************************************
*	Function	：	SDIO_SetBlkSzAndNum
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SetBlkSizeAndNum(SDIO_ID pId, U32 pBlkSize, U32 pBlkNum)
{
	*(RP)SDIO_BLKSIZ_CH(pId) = pBlkSize;
	*(RP)SDIO_BYTCNT_CH(pId) = pBlkNum * pBlkSize;
}
/***********************************************************
*	Function	：	SDIO_SendCMD
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SendCMD(SDIO_ID pId, U32 pCMDSn, U32 pCMDArg, 
				  BOOL pUpdateClockEn, BOOL pSendInitEn, BOOL pStopAbortEn, 
				  BOOL pWaitDataEn, BOOL pAutoStop, RW_DIR pRWDir, BOOL pDataExchange, 
				  BOOL pCRCEn, BOOL pRespEn, RESP_LEN pLongSel)
{
	*(RP)SDIO_CMDARG_CH(pId) = pCMDArg;
	*(RP)SDIO_CMD_CH(pId) =  (1 << 31) | (pUpdateClockEn << 21) | (pSendInitEn << 15) \
						   | (pStopAbortEn<<14) | (pWaitDataEn << 13) | (pAutoStop << 12)\
		                   | (pRWDir << 10) | (pDataExchange << 9) | (pCRCEn << 8) 
		                   | (pLongSel << 7)  | (pRespEn << 6) | (pCMDSn & 0X3F);
}
/***********************************************************
*	Function	：	SDIO_SetRxFifo
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SetRxFifo(SDIO_ID pId, U32 pRxFifo)
{
	*(RP)SDIO_FIFOTH_CH(pId) &= 0xf000ffff;
	*(RP)SDIO_FIFOTH_CH(pId) |= ((pRxFifo & 0x0fff) << 16);
}
/***********************************************************
*	Function	：	SDIO_SetTxFifo
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SetTxFifo(SDIO_ID pId, U32 pTxFifo)
{
	*(RP)SDIO_FIFOTH_CH(pId) &= 0xfffff000;
	*(RP)SDIO_FIFOTH_CH(pId) |= (pTxFifo & 0x0fff);
}
/***********************************************************
*	Function	：	SDIO_SetBurstSize
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SetBurstSize(SDIO_ID pId, BST_MSIZE pBurstSize)
{
	*(RP)SDIO_FIFOTH_CH(pId) &= 0x8fffffff;
	*(RP)SDIO_FIFOTH_CH(pId) |= ((pBurstSize & 0x0f) << 28);
}
/***********************************************************
*	Function	：	SDIO_UpdateClockRegion
*	Parameter	：	
*	Return		：  
*	Description	：	更新时钟域
***********************************************************/
void SDIO_UpdateClockRegion(SDIO_ID pId, U32 pDiv)
{   
	DebugPrintf("\tUpdata clock begin !!!\n");
    //clock programming   
    SDIO_SetClock(pId, DIV0, 0, FALSE, FALSE);		//disable all clk and non_low_power mode
    //update_clock_register_only  0x80202000
    SDIO_SendCMD(pId, CMD0, 0x00, 
    			 TRUE, 			/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 TRUE, 			/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 FALSE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    delay(10);
    //wait the command taken by CIU
    SDIO_WaitCmdTakenByCIU(pId);   
    DebugPrintf("\tUpdata_Clock cmd0 done!!!\n");
    //clock programming,set the clock frequency 400K ,20000K/(2*400K)=25
    SDIO_SetClock(pId, DIV1, pDiv, FALSE, FALSE);
    //update_clock_register_only  0x80202000
    SDIO_SendCMD(pId, CMD0, 0x00, 
    			 TRUE, 			/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 TRUE, 			/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 FALSE,			/*pCRCEn*/
    			 FALSE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    delay(10);
    //wait the command taken by CIU
    SDIO_WaitCmdTakenByCIU(pId);    
    DebugPrintf("\tUpdata_Clock cmd0 done!!!\n");
    //clock programming   
    SDIO_SetClock(pId, DIV0, 0, TRUE, FALSE);			//enable clock
    //update_clock_register_only  0x80202000
    SDIO_SendCMD(pId, CMD0, 0x00, 
    			 TRUE, 			/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 TRUE, 			/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 FALSE,			/*pCRCEn*/
    			 FALSE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    //wait the command taken by CIU
    SDIO_WaitCmdTakenByCIU(pId);    
    DebugPrintf("\tUpdata_Clock cmd0 done!!!\n");
    DebugPrintf("Updata clock end !!!\n");
}
/***********************************************************
*	Function	：	SDIO_InitDev
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
U32 SDIO_InitDev(SDIO_ID pId)
{
    U32 uResp[4];
    DebugPrintf("Initial begin !!!\n");
    //reset SD controller,FIFO,DMA
    SDIO_Reset(pId, RST_CTRL|RST_FIFO|RST_DMA);
    //enable power to all cards, we have only one card
    SDIO_EnablePower(pId, TRUE);
    // set the interrupt,unmask all the interrupt we have only one card
    SDIO_MaskInt(pId, 
    			 INT_EBE|INT_SBE|INT_HLE|INT_FRUN|INT_HTO|INT_DTO|INT_RTO|INT_DCRC|INT_RCRC|INT_DTP|INT_CP|INT_RE|INT_CD, 
    			 INT_UNMASK);
    //clear int_status bits
    SDIO_ClrRawInt(pId, INT_CD);		//SD1,CARD0    
    //enable global interrput
    SDIO_EnableInt(pId, TRUE);
    //set the card width as a single bit
    SDIO_SetCardType(pId, ONE_WIRE);	//one wire mode       
    //降低时钟使其小于400k
    SDIO_UpdateClockRegion(pId, 0xc8);
    //set CMD0 with send_initialization 0x80008000
    SDIO_SendCMD(pId, CMD0, 0x00, 
    			 FALSE, 		/*pUpdateClockEn*/
    			 TRUE, 			/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 FALSE, 		/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 FALSE,			/*pCRCEn*/
    			 FALSE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    delay(10);       
    SDIO_WaitCmdDone(SDIO_ID1);
    DebugPrintf("\tInitial cmd0 done!!!\n");
    SDIO_ClrRawInt(pId, 0xffff);
    delay(5);                                //need to wait Ncc time at least 8 clock
    //CMD55 below,card in idle state after cmd0
    do{
    	// CMD55 (0x80000177)
        SDIO_SendCMD(pId, CMD55, 0x00, 
        			 FALSE, 		/*pUpdateClockEn*/
        			 TRUE, 			/*pSendInitEn*/
        			 FALSE, 		/*pStopAbortEn*/
        			 FALSE, 		/*pWaitDataEn*/
        			 FALSE,			/*pAutoStop*/
        			 READ, 			/*pRWDir*/
        			 FALSE, 		/*pDataExchange*/
        			 TRUE,			/*pCRCEn*/
        			 TRUE,			/*pRespEn*/
        			 SHORT_RESP);	/*pLongSel*/
		delay(1000);                        //need to wait Nrc time at least 8 clock 
		SDIO_WaitCmdDone(SDIO_ID1);
		DebugPrintf("\tInitial cmd55 done!!!\n");
		//write to bits clear status bit
		SDIO_ClrRawInt(pId, 0xffff); 
		SDIO_GetRespValue(pId, uResp);
		if((uResp[0] & 0x80000000) != 0x80000000)
		{
			continue;
		}
		//ACMD 41 below (0x80000069)
        SDIO_SendCMD(pId, ACMD41, 0x80ff8000, 
        			 FALSE, 		/*pUpdateClockEn*/
        			 FALSE, 		/*pSendInitEn*/
        			 FALSE, 		/*pStopAbortEn*/
        			 FALSE, 		/*pWaitDataEn*/
        			 FALSE,			/*pAutoStop*/
        			 READ, 			/*pRWDir*/
        			 FALSE, 		/*pDataExchange*/
        			 FALSE,			/*pCRCEn*/
        			 TRUE,			/*pRespEn*/
        			 SHORT_RESP);	/*pLongSel*/
        delay(1000);
        SDIO_WaitCmdDone(SDIO_ID1);
        SDIO_ClrRawInt(pId, 0xffff);              //write to bits clear status bit
        DebugPrintf("\tInitial acmd41 done!!!\n");
        SDIO_GetRespValue(pId, uResp);				//OCR with busy
    }while((uResp[0] & 0x80000000) != 0x80000000);
    delay(10);                            //need to wait Nrc time at least 8 clock  
    // CMD2 below , long response except from card  0x800001c2 
    SDIO_SendCMD(pId, CMD2, 0x00, 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 FALSE, 		/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 LONG_RESP);	/*pLongSel*/
    SDIO_WaitCmdDone(SDIO_ID1);
    SDIO_ClrRawInt(pId, 0xffff);              //write to bits clear status bit
    DebugPrintf("\tInitial cmd2 done!!!\n");
    SDIO_GetRespValue(pId, uResp);		
    delay(5);                                //need to wait Nrc time at least 8 clock    
    //CMD3 below, card is in ident state (0x80000143)
    SDIO_SendCMD(pId, CMD3, 0x00, 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 FALSE, 		/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
	SDIO_WaitCmdDone(SDIO_ID1);
	DebugPrintf("\tInitial cmd3 done!!!\n");
	SDIO_ClrRawInt(pId, 0xffff);               //write to bits clear status bit
	SDIO_GetRespValue(pId, uResp);		
    delay(5);                                //need to wait Nrc time at least 8 clock
    DebugPrintf("Initial end !!!\n");
    return (uResp[0] >> 16);
}
/***********************************************************
*	Function	：	SDIO_SwitchBitWidth
*	Parameter	：	
*	Return		：  
*	Description	：	切换SD卡位宽
***********************************************************/
void SDIO_SwitchBitWidth(SDIO_ID pId, U16 rca, U16 width)
{
    U32 temp;
    DebugPrintf("SD Width Switch begin !!!\n");
    // (0x80000177)
    SDIO_SendCMD(pId, CMD55, (rca << 16), 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 FALSE, 		/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    delay(10);   //need to wait Nrc time at least 8 clock 
    temp = *(RP)SDIO_RINTSTS_CH(pId) & 0x4;      //command done
    SDIO_WaitCmdDone(pId);
    //write to bits clear status bit
    SDIO_ClrRawInt(pId, 0xffff);   
    //ACMD6 below
    SDIO_SendCMD(pId, ACMD6, (width), 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 FALSE, 		/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 FALSE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    delay(1000);
    SDIO_WaitCmdDone(pId);
    //write to bits clear status bit
    SDIO_ClrRawInt(pId, 0xffff); 
    DebugPrintf("SD Width Switch end !!!\n");
}
/***********************************************************
*	Function	：	SDIO_ToggleStandbyTran
*	Parameter	：	
*	Return		：  
*	Description	：	切换SD卡位宽
***********************************************************/
void SDIO_ToggleStandbyTran(SDIO_ID pId, U32 pRca)
{
    U32 uRcaTemp = pRca << 16;
    DebugPrintf("Tranfer mode toggle begin !\n");
    // --send CMD13 first(Address card sends tis status register)
    SDIO_SendCMD(pId, CMD13, uRcaTemp, 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 TRUE, 			/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 FALSE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    SDIO_WaitCmdDone(pId);
    SDIO_ClrRawInt(pId, 0xffff); 
    // --send CMD7 (toggle the card between the stand_by and transfer states)
    SDIO_SendCMD(pId, CMD7, uRcaTemp, 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 TRUE, 			/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 FALSE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    SDIO_WaitCmdDone(pId);		// card is in standby state
    SDIO_ClrRawInt(pId, 0xffff); 
    // --send CMD13 (Address card sends tis status register)
    SDIO_SendCMD(pId, CMD13, uRcaTemp, 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 TRUE, 			/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 FALSE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    SDIO_WaitCmdDone(pId);		// card is in tran state
    SDIO_ClrRawInt(pId, 0xffff); 
    DebugPrintf("Tranfer mode toggle end !\n");
}
/***********************************************************
*	Function	：	SDIO_SendCSD
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SendCSD(SDIO_ID pId, U32 pRca)
{
    SDIO_SendCMD(pId, CMD9, (pRca << 16), 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 FALSE, 		/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 LONG_RESP);	/*pLongSel*/      
    delay(10);                        //need to wait Nrc time at least 8 clock 
    SDIO_WaitCmdDone(pId);
    SDIO_ClrRawInt(pId, (1 << 2));	//clr cmd complete int flag
}
/***********************************************************
*	Function	：	SDIO_SendCID
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SendCID(SDIO_ID pId, U32 pRca)
{
    SDIO_SendCMD(pId, CMD10, (pRca << 16), 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 FALSE, 		/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 LONG_RESP);	/*pLongSel*/      
    delay(10);                  //need to wait Nrc time at least 8 clock 
    SDIO_WaitCmdDone(pId);
    SDIO_ClrRawInt(pId, (1 << 2));	//clr cmd complete int flag
}
/***********************************************************
*	Function	：	SDIO_SetDSR
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SetDSR(SDIO_ID pId)
{
    SDIO_SendCMD(pId, CMD4, 0x0, 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 FALSE, 		/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 TRUE,			/*pCRCEn*/
    			 FALSE,			/*pRespEn*/
    			 LONG_RESP);	/*pLongSel*/      
    delay(10);                  //need to wait Nrc time at least 8 clock 
    SDIO_WaitCmdDone(pId);
    SDIO_ClrRawInt(pId, (1 << 2));	//clr cmd complete int flag
}
/***********************************************************
*	Function	：	SDIO_StopTran
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_StopTran(SDIO_ID pId)
{
    SDIO_SendCMD(pId, CMD12, 0x0, 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 FALSE, 		/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 LONG_RESP);	/*pLongSel*/      
    delay(10);                  //need to wait Nrc time at least 8 clock 
    SDIO_WaitCmdDone(pId);
    SDIO_ClrRawInt(pId, (1 << 2));	//clr cmd complete int flag
}
/***********************************************************
*	Function	：	SDIO_SetInactiveState
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SetInactiveState(SDIO_ID pId, U32 pRca)
{
    SDIO_SendCMD(pId, CMD15, (pRca << 16), 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 FALSE, 		/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 TRUE,			/*pCRCEn*/
    			 FALSE,			/*pRespEn*/
    			 LONG_RESP);	/*pLongSel*/      
    delay(10);                  //need to wait Nrc time at least 8 clock 
    SDIO_WaitCmdDone(pId);
    SDIO_ClrRawInt(pId, (1 << 2));	//clr cmd complete int flag
}
/***********************************************************
*	Function	：	SDIO_SendSCR
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SendSCR(SDIO_ID pId, U32 pRca)
{
    U32 i;
    // --set the translate parameter
	SDIO_SetCardType(pId, FOUR_WIRE);  
	SDIO_SetBlkSizeAndNum(pId, 8, 1);
	//MSize = 8,TX_Wmark =8,RX_WMark =7
	SDIO_SetBurstSize(pId, MSIZE8);         	
	SDIO_SetTxFifo(pId, 8);
	SDIO_SetRxFifo(pId, 7);
	SDIO_Reset(pId, RST_FIFO);
    //-- CMD55 (0x80000177)
    SDIO_SendCMD(pId, CMD55, (pRca << 16), 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 FALSE, 		/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/   
    delay(10); 
    SDIO_WaitCmdDone(pId);
    SDIO_ClrRawInt(pId, (1 << 2));	//clr cmd complete int flag
    //-- ACMD51 (0x80000177)
    SDIO_SendCMD(pId, ACMD51, 0x0, 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 TRUE, 			/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 TRUE, 			/*pDataExchange*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/   
    delay(100);
    SDIO_WaitCmdDone(pId);
    SDIO_ClrRawInt(pId, (1 << 2));	//clr cmd complete int flag   
    for (i=0;i<0x10;i++)
    {
        write_reg(0x48000000+i*4, read_reg(SDIO_DATA_CH(pId)));
    }
}
/***********************************************************
*	Function	：	SDIO_ProgramCSD
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_ProgramCSD(SDIO_ID pId)
{
    U32 i;
    // --set the translate parameter
	SDIO_SetCardType(pId, FOUR_WIRE);  
	SDIO_SetBlkSizeAndNum(pId, 16, 1);
	// -- MSize = 8,TX_Wmark =8,RX_WMark =7
	SDIO_SetBurstSize(pId, MSIZE8);         	
	SDIO_SetTxFifo(pId, 8);
	SDIO_SetRxFifo(pId, 7);
	SDIO_Reset(pId, RST_FIFO);
	/*CMD27*/
    SDIO_SendCMD(pId, 27, 0x0, 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 FALSE, 		/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 WRITE, 		/*pRWDir*/
    			 TRUE, 			/*pDataExchange / pHaveDataTran*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/  
    delay(10);                        //need to wait Nrc time at least 8 clock 
    SDIO_WaitCmdDone(pId);
    SDIO_ClrRawInt(pId, (1 << 2));	//clr cmd complete int flag   
    for (i=0;i<8;i++)
    {
        write_reg(0x48000000+i*4, read_reg(SDIO_DATA_CH(pId)));
    }
}
/***********************************************************
*	Function	：	SDIO_SetWriteProtect
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_SetWriteProtect(SDIO_ID pId)
{
    U32 i;
    // --set the translate parameter
	SDIO_SetCardType(pId, FOUR_WIRE);  
	SDIO_SetBlkSizeAndNum(pId, 4, 1);
	//MSize = 8,TX_Wmark =8,RX_WMark =7
	SDIO_SetBurstSize(pId, MSIZE8);         	
	SDIO_SetTxFifo(pId, 8);
	SDIO_SetRxFifo(pId, 7);
	SDIO_Reset(pId, RST_FIFO);
	//-- CMD30
    SDIO_SendCMD(pId, CMD30, 0x0, 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 FALSE, 		/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 TRUE, 			/*pDataExchange / pHaveDataTran*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/   
    delay(10);                        //need to wait Nrc time at least 8 clock 
    SDIO_WaitCmdDone(pId);
    SDIO_ClrRawInt(pId, (1 << 2));	//clr cmd complete int flag      
    for (i=0;i<8;i++)
    {
        write_reg(0x48000000+i*4, read_reg(SDIO_DATA_CH(pId)));
    }
}
/***********************************************************
*	Function	：	SDIO_ReadSDStatus
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SDIO_ReadSDStatus(SDIO_ID pId, U32 pRca)
{
    U32 i;
    // --set the translate parameter
	SDIO_SetCardType(pId, FOUR_WIRE);  
	SDIO_SetBlkSizeAndNum(pId, 64, 1);
	//MSize = 8,TX_Wmark =8,RX_WMark =7
	SDIO_SetBurstSize(pId, MSIZE8);         	
	SDIO_SetTxFifo(pId, 8);
	SDIO_SetRxFifo(pId, 7);
	SDIO_Reset(pId, RST_FIFO);
	//-- CMD55
    SDIO_SendCMD(pId, CMD55, (pRca << 16), 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 FALSE, 		/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange / pHaveDataTran*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/   
    delay(10);                        //need to wait Nrc time at least 8 clock 
    SDIO_WaitCmdDone(pId);
    SDIO_ClrRawInt(pId, (1 << 2));	//clr cmd complete int flag  
	//-- ACMD13
    SDIO_SendCMD(pId, ACMD13, 0x0, 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 TRUE, 			/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 TRUE, 			/*pDataExchange / pHaveDataTran*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    delay(100);                        //need to wait Nrc time at least 8 clock 
    SDIO_WaitCmdDone(pId);
    SDIO_ClrRawInt(pId, (1 << 2));		//clr cmd complete int flag  
    for (i=0;i<0x10;i++)
    {
        write_reg(0x48000000+i*4, read_reg(SDIO_DATA_CH(pId)));
    }
}
/***********************************************************
*	Function	：	SDIO_EraseSector
*	Parameter	：	
*	Return		：  
*	Description	：	插除SD
***********************************************************/
void SDIO_EraseSector(SDIO_ID pId, U32 pWriteSectorAddr, U32 pWriteSectorNum, U32 pSectorSize)
{
    U32  temp;
    U32  resp;
    DebugPrintf("Erase Sector begin !\r\n");
    // --send CMD32 define the start address of the erase range
    SDIO_SendCMD(pId, CMD32, pWriteSectorAddr, 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 TRUE, 			/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange / pHaveDataTran*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    SDIO_WaitCmdDone(pId);
    SDIO_ClrRawInt(pId, 0xffff); 
    delay(5); 
    // --send CMD33 define the last address of the erase range
    SDIO_SendCMD(pId, CMD33, pWriteSectorAddr + pWriteSectorNum, 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 TRUE, 			/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    SDIO_WaitCmdDone(pId);
    SDIO_ClrRawInt(pId, 0xffff);
    delay(5);
    // --send CMD38 the erase command
    SDIO_SendCMD(pId, CMD38, 0, 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 TRUE, 			/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 READ, 			/*pRWDir*/
    			 FALSE, 		/*pDataExchange*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    SDIO_WaitCmdDone(1);
    SDIO_ClrRawInt(pId, 0xffff); 
    delay(5);
    SDIO_ClrRawInt(pId, 0xffff);
    DebugPrintf("Erase Sector end !\r\n");
}
/***********************************************************
*	Function	：	SDIO_WriteSingleSector
*	Parameter	：	
*	Return		：  
*	Description	：	向SD中写入单块数据
***********************************************************/
void SDIO_WriteSingleSector(SDIO_ID pId, U32 pWriteSectorAddr, U32 pSectorSize)
{
    // --set the translate parameter
	SDIO_SetCardType(pId, FOUR_WIRE);  
	SDIO_SetBlkSizeAndNum(pId, pSectorSize, 1);
	//MSize = 8,TX_Wmark =8,RX_WMark =7
	SDIO_SetBurstSize(pId, MSIZE8);         	
	SDIO_SetTxFifo(pId, 8);
    //CMD24
    SDIO_SendCMD(pId, CMD24, (pWriteSectorAddr * pSectorSize), 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 TRUE, 			/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
    			 WRITE, 		/*pRWDir*/
    			 TRUE, 			/*pDataExchange*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    delay(100);
    SDIO_WaitTranDone(pId);
    SDIO_ClrRawInt(pId, 0xffff);   //write to bits clear status bit
}
/***********************************************************
*	Function	：	SDIO_WriteMultiSector
*	Parameter	：	
*	Return		：  
*	Description	：	向SD中写入多块数据
***********************************************************/
void SDIO_WriteMultiSector(SDIO_ID pId, U32 pWriteSectorAddr, U32 pWriteSectorNum, U32 pSectorSize)
{    
    // --set the translate parameter
	SDIO_SetCardType(pId, FOUR_WIRE);  
	SDIO_SetBlkSizeAndNum(pId, pSectorSize, pWriteSectorNum);
	//MSize = 8,TX_Wmark =8,RX_WMark =7
	SDIO_SetBurstSize(pId, MSIZE8);         	
	SDIO_SetTxFifo(pId, 8);
    // --send CMD25 (write mulitple blocks)
    SDIO_SendCMD(pId, CMD25, (pWriteSectorAddr * pSectorSize), 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 TRUE, 			/*pWaitDataEn*/
    			 TRUE,			/*pAutoStop*/
    			 WRITE, 		/*pRWDir*/
    			 TRUE, 			/*pDataExchange*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    delay(1000);
    SDIO_WaitTranDone(pId);
    SDIO_ClrRawInt(pId, 0xffff);   //write to bits clear status bit
}
/***********************************************************
*	Function	：	SDIO_ReadSingleSector
*	Parameter	：	
*	Return		：  
*	Description	：	从SD中读出单块数据
***********************************************************/
void SDIO_ReadSingleSector(SDIO_ID pId, U32 pReadSectorAddr, U32 pSectorSize)
{
    // --set the translate parameter
	SDIO_SetCardType(pId, FOUR_WIRE);  
	SDIO_SetBlkSizeAndNum(pId, pSectorSize, 1);
	//MSize = 8,TX_Wmark =8,RX_WMark =7
	SDIO_SetBurstSize(pId, MSIZE8);         	
	SDIO_SetRxFifo(pId, 7);
    //CMD17
	SDIO_SendCMD(pId, CMD17, (pReadSectorAddr * pSectorSize), 
 			 	 FALSE, 		/*pUpdateClockEn*/
 			 	 FALSE, 		/*pSendInitEn*/
 			 	 FALSE, 		/*pStopAbortEn*/
 			 	 TRUE, 			/*pWaitDataEn*/
    			 FALSE,			/*pAutoStop*/
 			 	 WRITE, 		/*pRWDir*/
 			 	 TRUE, 			/*pDataExchange*/
 			 	 TRUE,			/*pCRCEn*/
 			 	 TRUE,			/*pRespEn*/
 			 	 SHORT_RESP);	/*pLongSel*/
	SDIO_WaitTranDone(pId);
	SDIO_ClrRawInt(pId, 0xffff);   //write to bits clear status bit
}
/***********************************************************
*	Function	：	SDIO_ReadMultiSector
*	Parameter	：	
*	Return		：  
*	Description	：	从SD中读出多块数据
***********************************************************/
void SDIO_ReadMultiSector(SDIO_ID pId, U32 pReadSectorAddr, U32 pReadSectorNum, U32 pSectorSize)
{
    // --set the translate parameter
	SDIO_SetCardType(pId, FOUR_WIRE);  
	SDIO_SetBlkSizeAndNum(pId, pSectorSize, pReadSectorNum);
	//MSize = 8,TX_Wmark =8,RX_WMark =7
	SDIO_SetBurstSize(pId, MSIZE8);         	
	SDIO_SetRxFifo(pId, 7);
    // --send CMD25 (write mulitple blocks)
    SDIO_SendCMD(pId, CMD25, (pReadSectorAddr * pSectorSize), 
    			 FALSE, 		/*pUpdateClockEn*/
    			 FALSE, 		/*pSendInitEn*/
    			 FALSE, 		/*pStopAbortEn*/
    			 TRUE, 			/*pWaitDataEn*/
    			 TRUE,			/*pAutoStop*/
    			 READ,	 		/*pRWDir*/
    			 TRUE, 			/*pDataExchange*/
    			 TRUE,			/*pCRCEn*/
    			 TRUE,			/*pRespEn*/
    			 SHORT_RESP);	/*pLongSel*/
    delay(1000);
    SDIO_WaitTranDone(pId);
    SDIO_ClrRawInt(pId, 0xffff);   //write to bits clear status bit
}
/***********************************************************
*	Function	：	SDIO_WriteSD
*	Parameter	：	
*	Return		：  
*	Description	：	向SD中写入数据
* 					DMAC一次最大可以传输8K的数据，为了方便做一次DMAC配置就进行
* 					一次SD传输，我们规定多快传输时最大一次传输16块。
***********************************************************/
void SDIO_WriteSD(SDIO_ID pId, U32 pSrcAddr, U32 pWriteSectorAddr, U32 pWriteSectorNum)
{    
	U32 i, uSectorNum, uBlkNum;
	DebugPrintf("Write SD begin !\r\n");
	// calc the block num and the sector num of last block
	uBlkNum = pWriteSectorNum / 16;
	uSectorNum = pWriteSectorNum % 16;
	/*configure*/
	SDIO_Reset(pId, RST_FIFO);
	SDIO_EnableDma(pId, TRUE);
    for(i = 0;i < uBlkNum;i ++)
    {
    	// --enable the dma in the SDIO_CTRL_CH(SDIO_channel)
    	//DMAC_Write_Tran(DMAC_CHANNEL0, SRC_Peri, DST_Peri, ram_start, SDIO_DATA_CH(SDIO_channel), word_len);
    	SDIO_WriteMultiSector(pId, pWriteSectorAddr + i*16, 16, 512);
    }
	// --enable the dma in the SDIO_CTRL_CH(SDIO_channel)
	//DMAC_Write_Tran(DMAC_CHANNEL0, SRC_Peri, DST_Peri, ram_start, SDIO_DATA_CH(SDIO_channel), word_len);
	SDIO_WriteMultiSector(pId, pWriteSectorAddr + uBlkNum*16, uSectorNum, 512);
	DebugPrintf("Write SD End !\r\n");
}
/***********************************************************
*	Function	：	SDIO_ReadSD
*	Parameter	：	
*	Return		：  
*	Description	：	从SD读取数据
* 					DMAC一次最大可以传输8K的数据，为了方便做一次DMAC配置就进行
* 					一次SD传输，我们规定多快传输时最大一次传输16块。
***********************************************************/
void SDIO_ReadSD(SDIO_ID pId, U32 pDstAddr, U32 pReadSectorAddr, U32 pReadSectorNum)
{
	U32 i, uSectorNum, uBlkNum;
	DebugPrintf("Read SD begin !\r\n");
	// calc the block num and the sector num of last block
	uBlkNum = pReadSectorNum / 16;
	uSectorNum = pReadSectorNum % 16;
	SDIO_Reset(pId, RST_FIFO);
	SDIO_EnableDma(pId, TRUE);
    for(i = 0;i < uBlkNum;i ++)
    {
    	// --enable the dma in the SDIO_CTRL_CH(SDIO_channel)
    	//DMAC_Write_Tran(DMAC_CHANNEL0, SRC_Peri, DST_Peri, ram_start, SDIO_DATA_CH(SDIO_channel), word_len);
    	SDIO_ReadMultiSector(pId, pReadSectorAddr + i*16, 16, 512);
    }
	// --enable the dma in the SDIO_CTRL_CH(SDIO_channel)
	//DMAC_Write_Tran(DMAC_CHANNEL0, SRC_Peri, DST_Peri, ram_start, SDIO_DATA_CH(SDIO_channel), word_len);
	SDIO_WriteMultiSector(pId, pReadSectorAddr + uBlkNum*16, uSectorNum, 512);
	DebugPrintf("Read SingleSD end!\r\n");
}

void SDIO_SingleSectorTest(void)
{
	
}

void SDIO_MultiSectorTest(void)
{
	
}

const testFuncMenu SDIO_Menu[] =
{
	{SDIO_SingleSectorTest,		"SD卡单块传输验证"},
	{SDIO_MultiSectorTest,		"SD卡多块传输验证"}
};

void SDIO_Test(void)
{
	volatile U32 i,uSel,uMenuNum=(sizeof(SDIO_Menu)/8);
	char temp[10] = "return";
	while(1)
	{
		DebugPrintf("\n\n");
		DebugPrintf("******************************************\n");
		DebugPrintf("*       SDIO - Test firmware v0.1        *\n");
		DebugPrintf("******************************************\n");
		for (i=0; i<uMenuNum; i++)
		{
			DebugPrintf("\t%2d: %s  \r\n", i, SDIO_Menu[i].desc);
		}
		DebugPrintf("\t%2d: %s  \r\n", i, temp);

		DebugPrintf("\n\nSelect the function to test : ");
		uSel = DebugGetNum();
		DebugPrintf("\n\n");
		
		if(uSel == uMenuNum)
		{
			DebugPrintf("Return to MainMenu!\r\n");
		}
		else
		{
			DebugPrintf("Function Begin ... ... \n");
		}

		if (uSel<uMenuNum)
			(SDIO_Menu[uSel].func) ();
		else
			break;
	}
}



