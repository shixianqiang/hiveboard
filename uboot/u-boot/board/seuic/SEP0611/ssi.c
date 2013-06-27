/*********************************************************************
* Filename    :  ssi.c
* Author      :  SixRoom
* Date        :  2011-01-12
* Description :  This file is used to define SSI
* Version  	  :  v1.00
**********************************************************************/
#include	"../include/ssi.h"
#include	"../include/dmac.h"
#include	"../include/gpio.h"
#include	"../include/uart.h"

volatile U32 gTouchPortIntFlag;
volatile U32 gSsiIntFlag[6];

SSI_CFG  gSsiPara;
extern DMAC_CFG *gDmacCfg;

/***********************************************************
*	Function	：	RTC_Delay
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_Delay(U32 dly)
{
	U32 i;
	while(dly--)
		for(i=1000;i>0;i--);
}
/***********************************************************
*	Function	：	SPI_ClrFifo
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_ClrFifo(SSI_ID pId)
{
	SSI_Enable(pId, FALSE);
	SSI_Enable(pId, TRUE);
}
/***********************************************************
*	Function	：	SSI_RegisterVerify
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_RegisterVerify(void)
{
	U32 ssiRegisterResetValue[]={
		0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x16,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	};
	int i,j;
	for(i=0;i<3;i++)
	{
		for(i=0;i<22;i++)
		{
			if(ssiRegisterResetValue[j]==read_reg((SSI_BASE+j*(0x04))+i*0x1000))
				DebugPrintf("SSI%d_Reset_Register %x 's value is right!\r\n",i,j*(0x04));
			else
				DebugPrintf("\tSSI%d_Reset_Register %x 's value is error!\r\n",i,j*(0x04));
		}
	}
}
/***********************************************************
*	Function	：	SSI_InitPara
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_InitPara(SSI_CFG *pSsiPara)
{
	pSsiPara->SsiId = SSI_ID2;
	pSsiPara->SsiSlaDev = SlaveDev1;
	pSsiPara->OperateMode = NORMAL_MODE;
	pSsiPara->Tmod = TRAN_RTX;
	pSsiPara->Baud = BAUD(2000); 
	pSsiPara->Scpol = SCPOL(0);
	pSsiPara->Scph = SCPH(1);
	pSsiPara->FrameLen = FRAME_LEN(7); 
	pSsiPara->FrameNum = FRAME_NUM(0);
	pSsiPara->IntNum = 0;
	pSsiPara->RxtFt = RECEIV_FT(0);
	pSsiPara->TxtFt = TRANS_FT(0);
	pSsiPara->DmaRxEn = FALSE; 
	pSsiPara->DmaRdl = DMA_RDL(0); 
	pSsiPara->DmaTxEn = FALSE;  
	pSsiPara->DmaTdl = DMA_TDL(0); 
}
/***********************************************************
*	Function	：	SSI_SetPort
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_SetPort(void)
{
	*(RP)GPIO_PORTE_DIR |= (0xff<<8);
	*(RP)GPIO_PORTE_SEL &= ~(0xff<<8);//PE8~15
}
/***********************************************************
*	Function	：	spi_init
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_Init(SSI_CFG *pSsiPara)
{
	VIC_DisableInt(INTSRC_SSI1 + pSsiPara->SsiId -1);
	/*disable ssi for configure*/
	SSI_Enable(pSsiPara->SsiId, FALSE);
	/*select slave device*/
	SSI_SelSlaveDev(pSsiPara->SsiId, SlaveDev1);
	/*select operate mode*/
	SSI_SetOperateMode(pSsiPara->SsiId, pSsiPara->OperateMode);
	/*set ssi send and recevie mode*/
	SSI_SetMode(pSsiPara->SsiId, pSsiPara->Tmod);
	/*set baud*/
	SSI_SetBaud(pSsiPara->SsiId, pSsiPara->Baud);
	/*set polor*/
	SSI_SetPolar(pSsiPara->SsiId, pSsiPara->Scpol, pSsiPara->Scph);
	/*set frame para*/
	SSI_SetFrameLen(pSsiPara->SsiId, pSsiPara->FrameLen);
	SSI_SetFrameNum(pSsiPara->SsiId, pSsiPara->FrameNum);
	/*configure dma*/
	SSI_SetRxDMA(pSsiPara->SsiId, pSsiPara->DmaRxEn, pSsiPara->DmaRdl);
	SSI_SetTxDMA(pSsiPara->SsiId, pSsiPara->DmaTxEn, pSsiPara->DmaTdl);
	/*mask all int, because the reset value of IMR is 0x1f, so must clear them*/
	SSI_MaskInt(pSsiPara->SsiId, 0xff, INT_MASK);			
    if(pSsiPara->IntNum != 0)
    {
    	SSI_SetRxFifoLevel(pSsiPara->SsiId, pSsiPara->RxtFt);
    	SSI_SetTxFifoLevel(pSsiPara->SsiId, pSsiPara->TxtFt);
    	
    	SSI_MaskInt(pSsiPara->SsiId, pSsiPara->IntNum, INT_UNMASK);
		VIC_UnmaskAllInt(IRQ);
		VIC_EnableInt(INTSRC_SSI1 + pSsiPara->SsiId -1);
    }
    SSI_Enable(pSsiPara->SsiId, TRUE);
}
/***********************************************************
*	Function	：	SSI_PutChar
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_PutChar(SSI_ID pId, U32 pData)
{
	write_reg(SSI_DR_CH(pId), pData);
	/*等待当前传输完成*/
	while(*(RP)SSI_SR_CH(pId) & 0x1);
}
/***********************************************************
*	Function	：	SSI_GetChar
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
U32 SSI_GetChar(SSI_ID pId)
{
	/*等待接收FIFO不空*/
	while(!(*(RP)SSI_SR_CH(pId) & (1<<3)));
	return read_reg(SSI_DR_CH(pId))&0xffff;
}
/***********************************************************
*	Function	：	SSI_RWChar
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
U32 SSI_RWChar(SSI_ID pId, U32 pData)
{
	U32 uTemp;
	SSI_PutChar(pId, pData);
	do{
		uTemp = read_reg(SSI_DR_CH(pId));
	}while((*(RP)SSI_SR_CH(pId) & 0x08));	//如果fifo有多于一个数据，就一直读，只取最后一个数据
	while(*(RP)SSI_SR_CH(pId) & 0x01);
	return uTemp;
}
/***********************************************************
*	Function	：	SSI_SendString
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_SendString(SSI_ID pId, char *pBuffer)
{
	char *uString = pBuffer;
	while(*uString != NULL)
	{
		SSI_PutChar(pId, *uString++);
	}
}
//------------------------------------------------------------------
//			===	===	===	===	===	===	===
//				 TOUCH  相关函数   //需要将时钟极性和相位设置为01
//			===	===	===	===	===	===	===
//------------------------------------------------------------------
/***********************************************************
*	Function	：	touch_portint_init
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TOUCH_SetPort(void)
{
	GPIO_CFG uTouchPort={
			GPIOI,
			PIN(11),
			PIN_COMMON,
			PIN_IN,
			PIN_LOW,
			INT_ENABLE,
			PIN_INT_RISE
	};
	GPIO_InitPort(&uTouchPort);
}
/***********************************************************
*	Function	：	touch_port_isr_handler
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TOUCH_IRQHandle(void)
{
	gTouchPortIntFlag = 1;
	*(RP)GPIO_PORTI_INTCLR |= (1<<11);
}
/***********************************************************
*	Function	：	touch_enable
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
TOUCH_FEEDBACK TOUCH_Enable(SSI_ID pId)
{
	U32 j=0;
	U32 uRxBuf[4]={0,0,0,0};
	SSI_RWChar(pId, 0x55);
	SSI_RWChar(pId, 0x01);
	SSI_RWChar(pId, 0x12);
	while(gTouchPortIntFlag==0);
	gTouchPortIntFlag=0;
	for(j = 0; j < 4; j++)
	{
		uRxBuf[j] = SSI_RWChar(pId,0x00); 
	}
	if((uRxBuf[0]!=0x55)||(uRxBuf[1]!=0x02)||(uRxBuf[3]!=0x12))	
	{
		DebugPrintf("\tEnable Touch Error.\r\n");
		return TF_ERROR;
	}
	DebugPrintf("\tEnable Touch Success.\r\n");
	return ((TOUCH_FEEDBACK)uRxBuf[2]);		
}
/***********************************************************
*	Function	：	toupId_disable
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
S8 TOUCH_Disable(SSI_ID pId)
{
	U32 j=0;
	U32 uRxBuf[4]={0,0,0,0};
	SSI_RWChar(pId,0x55);
	SSI_RWChar(pId,0x01);
	SSI_RWChar(pId,0x13);
	while(gTouchPortIntFlag==0);
	gTouchPortIntFlag=0;
	for(j = 0; j < 4; j++)
	{
		uRxBuf[j] = SSI_RWChar(pId,0x00); 
	}
	if((uRxBuf[0]!=0x55)||(uRxBuf[1]!=0x02)||(uRxBuf[3]!=0x13))	
	{
		DebugPrintf("\tDisable Touch Error.\r\n");
		return TF_ERROR;
	}
	DebugPrintf("\tDisable Touch Success.\r\n");
	return ((TOUCH_FEEDBACK)uRxBuf[2]);		
}
/***********************************************************
*	Function	：	toupId_disable
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
TOUCH_FEEDBACK TOUCH_ReadReg(SSI_ID pId,U32 pAddr,U32 pNum,S8 *pRxBuffer)
{
	U32 j=0,temp;
	U32 uRxBuf[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	temp = 4 + pNum;
	while(TOUCH_Enable(pId)!=TF_SUCCESS);
	SSI_RWChar(pId,0x55);
	SSI_RWChar(pId,0x04);
	SSI_RWChar(pId,0x20);
	SSI_RWChar(pId,0x00);
	SSI_RWChar(pId,(pAddr));
	SSI_RWChar(pId,(pNum));
	while(gTouchPortIntFlag==0);
	gTouchPortIntFlag=0;
	//delay(10);
	for(j = 0; j < temp; j++)
	{
		uRxBuf[j] = SSI_RWChar(pId,0x00); 
	}
	while(TOUCH_Disable(pId)!=TF_SUCCESS);
	if((uRxBuf[0]!=0x55)||(uRxBuf[1]!=(0x02+(pNum)))||(uRxBuf[3]!=0x20))	
	{
		DebugPrintf("\tRead Reg%d Error.\r\n",pAddr);
		return TF_ERROR;
	}
	if(uRxBuf[2]==TF_SUCCESS)
	{
		for(j=0;j<pNum;j++)
			pRxBuffer[j] = uRxBuf[j+6];
	}
	return ((TOUCH_FEEDBACK)uRxBuf[2]);
}
/***********************************************************
*	Function	：	toupId_write_register
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
TOUCH_FEEDBACK TOUCH_WriteReg(SSI_ID pId,S8 pAddr,S8 pNum,S8 *pTxBuffer)
{
	U32 j=0;
	S8 uRxBuf[4]={0,0,0,0};
	while(TOUCH_Enable(pId)!=TF_SUCCESS);
	SSI_RWChar(pId,0x55);
	SSI_RWChar(pId,0x04+(pNum));
	SSI_RWChar(pId,0x21);
	SSI_RWChar(pId,0x00);
	SSI_RWChar(pId,(pAddr));
	SSI_RWChar(pId,(pNum));
	for(j=0;j<pNum;j++)
	{
		SSI_PutChar(pId,(pTxBuffer[j]));
	}
	while(gTouchPortIntFlag==0);
	gTouchPortIntFlag=0;
	//delay(10);
	for(j = 0; j < 4; j++)
	{
		uRxBuf[j] = SSI_RWChar(pId,0x00); 
	}
	while(TOUCH_Disable(pId)!=TF_SUCCESS);
	if((uRxBuf[0]!=0x55)||(uRxBuf[1]!=0x02)||(uRxBuf[3]!=0x21))	
	{
		DebugPrintf("\Write Reg%d Error.\r\n",pAddr);
		return TF_ERROR;
	}
	return ((TOUCH_FEEDBACK)uRxBuf[2]);
}
/***********************************************************
*	Function	：	toupId_startaddr_request
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
TOUCH_FEEDBACK TOUCH_ReqStartAddr(SSI_ID pId,S8 *pRequestAddr)
{
	U32 j=0;
	S8 uRxBuf[5]={0,0,0,0,0};
	while(TOUCH_Enable(pId)!=TF_SUCCESS);
	SSI_RWChar(pId,0x55);
	SSI_RWChar(pId,0x01);
	SSI_RWChar(pId,0x22);
	while(gTouchPortIntFlag==0);
	gTouchPortIntFlag=0;
	//delay(10);
	for(j = 0; j < 5; j++)
	{
		uRxBuf[j] = SSI_RWChar(pId,0x00); 
	}
	while(TOUCH_Disable(pId)!=TF_SUCCESS);
	if((uRxBuf[0]!=0x55)||(uRxBuf[1]!=0x03)||(uRxBuf[3]!=0x22))
	{
		DebugPrintf("\ReqStartAddr Error.\r\n");
		return TF_ERROR;
	}
	if(uRxBuf[2]==TF_SUCCESS)
	{
		*pRequestAddr = uRxBuf[4];
	}
	return ((TOUCH_FEEDBACK)uRxBuf[2]);
}
/***********************************************************
*	Function	：	toupId_save_to_EEPROM
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
TOUCH_FEEDBACK TOUCH_SaveToEEPROM(SSI_ID pId)
{
	U32 j=0;
	S8 uRxBuf[4]={0,0,0,0};
	while(TOUCH_Enable(pId)!=TF_SUCCESS);
	SSI_RWChar(pId,0x55);
	SSI_RWChar(pId,0x01);
	SSI_RWChar(pId,0x23);
	while(gTouchPortIntFlag==0);
	gTouchPortIntFlag=0;
	//delay(10);
	for(j = 0; j < 4; j++)
	{
		uRxBuf[j] = SSI_RWChar(pId,0x00); 
	}
	while(TOUCH_Disable(pId)!=TF_SUCCESS);
	if((uRxBuf[0]!=0x55)||(uRxBuf[1]!=0x02)||(uRxBuf[3]!=0x23))	
	{
		DebugPrintf("\SaveToEEPROM Error.\r\n");
		return TF_ERROR;
	}
	return ((TOUCH_FEEDBACK)uRxBuf[2]);		
}
/***********************************************************
*	Function	：	toupId_restore_to_REGISTER
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
TOUCH_FEEDBACK TOUCH_RevFromEEPROM(SSI_ID pId)
{
	U32 j=0;
	S8 uRxBuf[4]={0,0,0,0};
	while(TOUCH_Enable(pId)!=TF_SUCCESS);
	SSI_RWChar(pId,0x55);
	SSI_RWChar(pId,0x01);
	SSI_RWChar(pId,0x2b);
	while(gTouchPortIntFlag==0);
	gTouchPortIntFlag=0;
	//delay(10);
	for(j = 0; j < 4; j++)
	{
		uRxBuf[j] = SSI_RWChar(pId,0x00); 
	}
	while(TOUCH_Disable(pId)!=TF_SUCCESS);
	if((uRxBuf[0]!=0x55)||(uRxBuf[1]!=0x02)||(uRxBuf[3]!=0x2b))	
	{
		DebugPrintf("\RevFromEEPROM Error.\r\n");
		return TF_ERROR;
	}
	return ((TOUCH_FEEDBACK)uRxBuf[2]);		
}
/***********************************************************
*	Function	：	toupId_read_EEPROM
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
TOUCH_FEEDBACK TOUCH_ReadEEPROM(SSI_ID pId, S8 pAddr, S8 pNum, S8 *pRxBuffer)
{
	U32 j=0,uTemp;
	S8 uRxBuf[13]={0,0,0,0,0,0,0,0,0,0,0,0,0};
	uTemp = 5 + pNum;
	while(TOUCH_Enable(pId)!=TF_SUCCESS);
	SSI_RWChar(pId,0x55);
	SSI_RWChar(pId,0x04);
	SSI_RWChar(pId,0x28);
	SSI_RWChar(pId,0x00);
	SSI_RWChar(pId,(pAddr));
	SSI_RWChar(pId,(pNum));
	while(gTouchPortIntFlag==0);
	gTouchPortIntFlag=0;
	//delay(10);
	for(j = 0; j < uTemp; j++)
	{
		uRxBuf[j] = SSI_RWChar(pId,0x00); 
	}
	while(TOUCH_Disable(pId)!=TF_SUCCESS);
	if((uRxBuf[0]!=0x55)||(uRxBuf[1]!=0x02)||(uRxBuf[2]!=(pNum))||(uRxBuf[4]!=0x28))	
	{
		DebugPrintf("\ReadEEPROM Error.\r\n");
		return TF_ERROR;
	}
	if(uRxBuf[3]==TF_SUCCESS)
	{
		for(j=0;j<pNum;j++)
			pRxBuffer[j] = uRxBuf[j+5];
	}
	return ((TOUCH_FEEDBACK)uRxBuf[3]);	
}
/***********************************************************
*	Function	：	toupId_read_EEPROM
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
TOUCH_FEEDBACK TOUCH_WriteEEPROM(SSI_ID pId, S8 pAddr, S8 pNum, S8 *pTxBuffer)
{
	U32 j=0;
	S8 uRxBuf[4]={0,0,0,0};
	while(TOUCH_Enable(pId)!=TF_SUCCESS);
	SSI_RWChar(pId,0x55);
	SSI_RWChar(pId,0x04+(pNum));
	SSI_RWChar(pId,0x29);
	SSI_RWChar(pId,0x00);
	SSI_RWChar(pId,(pAddr));
	SSI_RWChar(pId,(pNum));
	for(j=0;j<pNum;j++)
	{
		SSI_PutChar(pId,(pTxBuffer[j]));
	}
	while(gTouchPortIntFlag==0);
	gTouchPortIntFlag=0;
	//delay(10);
	for(j = 0; j < 4; j++)
	{
		uRxBuf[j] = SSI_RWChar(pId,0x00); 
	}
	while(TOUCH_Disable(pId)!=TF_SUCCESS);
	if((uRxBuf[0]!=0x55)||(uRxBuf[1]!=0x02)||(uRxBuf[3]!=0x29))	
	{
		DebugPrintf("\WriteEEPROM Error.\r\n");
		return TF_ERROR;
	}
	return ((TOUCH_FEEDBACK)uRxBuf[2]);	
}
/***********************************************************
*	Function	：	spi_toupId_read_coordinate
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
TOUCH_FEEDBACK TOUCH_GetCoord(SSI_ID pId, Coordinate *pCoord)
{
	U32 j=0,temp;
	S8 uRxBuf[5]={0,0,0,0,0};
	while(gTouchPortIntFlag==0);
	gTouchPortIntFlag=0;
	//delay(10);
	for(j = 0; j < 5; j++)
	{
		uRxBuf[j] = SSI_RWChar(pId,0x00); 
	}
	if(uRxBuf[0]==0x81)	//Pen_up
	{
		temp = (uRxBuf[1]&0x7f)+((uRxBuf[2]&0x1f)<<7);
		pCoord->X = (temp*800)/4096;
		temp = (uRxBuf[3]&0x7f)+((uRxBuf[4]&0x1f)<<7);
		pCoord->Y = (temp*480)/4096;
		DebugPrintf("\tPen up\t:  X_Coord = %d  Y_Coord = %d\r\n",pCoord->X,pCoord->Y);
	}
	else if(uRxBuf[0]==0x80)	//pen_down
	{
		temp = (uRxBuf[1]&0x7f)+((uRxBuf[2]&0x1f)<<7);
		pCoord->X = (temp*800)/4096;
		temp = (uRxBuf[3]&0x7f)+((uRxBuf[4]&0x1f)<<7);
		pCoord->Y = (temp*480)/4096;
		DebugPrintf("\tPen down:  X_Coord = %d  Y_Coord = %d\r\n",pCoord->X,pCoord->Y);
	}
	else
	{
		DebugPrintf("\GetCoord Error.\r\n");
		return TF_ERROR;
	}
	return TF_SUCCESS;
}
/***********************************************************
*	Function	：	SSI_InitDMACPara
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_InitDMACPara(DMAC_CFG *pDmacCfg, RW_DIR pDir, DMAC_TRAN_TYPE pType)
{  
    /*INT MASK Register*/
	pDmacCfg->MskTfr           = TRUE; //true代表不屏蔽
    pDmacCfg->MskBlk           = TRUE;
    pDmacCfg->MskSrcTran       = FALSE;
    pDmacCfg->MskDstTran       = FALSE;
    pDmacCfg->MskErr           = TRUE;
	/*INT CLR Register*/
    pDmacCfg->ClrTfr           = TRUE;
    pDmacCfg->ClrBlk           = TRUE;
    pDmacCfg->ClrSrcTran       = TRUE;
    pDmacCfg->ClrDstTran       = TRUE;
    pDmacCfg->ClrErr           = TRUE;
    /*Global*/
	pDmacCfg->DmacId           = DMA_ID2;
	pDmacCfg->ChId             = DMA_CH0;
	if(pDir == WRITE)
	{
		pDmacCfg->SrcAddr          = SSI2_DR;
		pDmacCfg->DstAddr          = 0X44000000;
		pDmacCfg->TranTypeFlowCtrl = MEM_TO_PERI_DMAC;
	    pDmacCfg->SrcHs            = SOFTWARE_HS;
	    pDmacCfg->DstHs            = HARDWARE_HS;
		pDmacCfg->SrcIncr          = INCR;
		pDmacCfg->DstIncr          = FIXED;
	}
	else if(pDir == READ)
	{
		pDmacCfg->SrcAddr          = 0X43000000;
		pDmacCfg->DstAddr          = SSI2_DR;
		pDmacCfg->TranTypeFlowCtrl = PERI_TO_MEM_DMAC;
	    pDmacCfg->SrcHs            = HARDWARE_HS;
	    pDmacCfg->DstHs            = SOFTWARE_HS;
		pDmacCfg->SrcIncr          = FIXED;
		pDmacCfg->DstIncr          = INCR;
	}
	/*CTL Register*/
	pDmacCfg->BlkTranSz        = 5;
	pDmacCfg->IntEn            = TRUE;
	pDmacCfg->SrcWidth         = WIDTH32;
	pDmacCfg->DstWidth         = WIDTH32;

    pDmacCfg->SrcBurst         = BUSRT4;
    pDmacCfg->DstBurst         = BUSRT4;
    pDmacCfg->SrcGather        = FALSE;
    pDmacCfg->DstScatter       = FALSE;
    switch(pType)
    {
	    case SRC_CON_DST_LINK_TRAN:
	    case SRC_RELOAD_DST_LINK_TRAN:
	        pDmacCfg->SrcLlpEn         = FALSE;
	        pDmacCfg->DstLlpEn         = TRUE;
	    	break;
	    case SRC_LINK_DST_CON_TRAN:
	    case SRC_LINK_DST_RELOAD_TRAN:
	        pDmacCfg->SrcLlpEn         = TRUE;
	        pDmacCfg->DstLlpEn         = FALSE;
	    	break;
	    case SRC_LINK_DST_LINK_TRAN:
	        pDmacCfg->SrcLlpEn         = TRUE;
	        pDmacCfg->DstLlpEn         = TRUE;
	    	break;
	    default:
	        pDmacCfg->SrcLlpEn         = FALSE;
	        pDmacCfg->DstLlpEn         = FALSE;
	    	break;
    }
    pDmacCfg->tmpHeadLLI = NULL;
    /*CFG Register*/
    pDmacCfg->ChPrio           = CH_PRIO0;

    switch(pType)
    {
	    case SRC_LINK_DST_RELOAD_TRAN:
	    case SRC_CON_DST_RELOAD_TRAN:
	        pDmacCfg->SrcReload        = FALSE;
	        pDmacCfg->DstReload        = TRUE;
	    	break;
	    case SRC_RELOAD_DST_CON_TRAN:
	    case SRC_RELOAD_DST_LINK_TRAN:
	        pDmacCfg->SrcReload        = TRUE;
	        pDmacCfg->DstReload        = FALSE;
	    	break;
	    case SRC_RELOAD_DST_RELOAD_TRAN:
	        pDmacCfg->SrcReload        = TRUE;
	        pDmacCfg->DstReload        = TRUE;
	    	break;
	    default:
	        pDmacCfg->SrcReload        = FALSE;
	        pDmacCfg->DstReload        = FALSE;
	    	break;
    }
    pDmacCfg->FifoMode         = HALF_FIFO;
    pDmacCfg->SrcHsInte        = SSI2_OUT; 
    pDmacCfg->DstHsInte        = SSI2_IN; 
}
/***********************************************************
*	Function	：	spi_toupId_dmac_get_coordinate
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
TOUCH_FEEDBACK TOUCH_DMAGetCoord(SSI_ID pId,Coordinate *pCoord)
{
	U32 j=0,temp;
	S8 uRxBuf[5]={0,0,0,0,0};
	static U32 addr = 0x41000000;;
	while(gTouchPortIntFlag==0);
	gTouchPortIntFlag=0;
	for(j = 0; j < 5; j++)
	{
		SSI_PutChar(pId,0x00); 
	}
	//Configure the DMA
	write_reg(DMAC2_CTL0,(1| (0<<1) | (0<<4) | (0<<7) | (3<<9) | (3<<11) | (3<<14) | (0<<17) | (2<<20) | (0<<23) | (1<<25) | (0<<27)));     //设置传输的位宽为32bits
	write_reg(DMAC2_CTL0 + 0x4,5);    //要传输的单元数
	write_reg(DMAC2_CFG0,0x00000200);        //目的硬件握手
	write_reg(DMAC2_CFG0 + 0x4,0x1906);		//设置硬件握手接口
	write_reg(DMAC2_LLP0,0x00000000); 
	write_reg(DMAC2_LLP0 + 0x4,0x00000000);
	write_reg(DMAC2_SAR0,SSI2_DR);//设置源为DR
	write_reg(DMAC2_SAR0 + 0x4,0x00000000);
	write_reg(DMAC2_DAR0,addr);//设置目的
	write_reg(DMAC2_DAR0 + 0x4,0x00000000);
	write_reg(DMAC2_MASKTRF,0x00000101);
	write_reg(DMAC2_MASKBLK,0x00000101);
	write_reg(DMAC2_MASKSRCTR,0X00000101);
	write_reg(DMAC2_MASKDSTTR,0X00000101);	
	write_reg(DMAC2_CHENA,0x00000101); //每个DMA有8个通道
	for(j=0;j<5;j++)
	{
		uRxBuf[j] = *(RP8)(addr++);
	}
	if(uRxBuf[0]==0x81)	//Pen_up
	{
		temp = (uRxBuf[1]&0x7f)+((uRxBuf[2]&0x1f)<<7);
		pCoord->X = (temp*800)/4096;
		temp = (uRxBuf[3]&0x7f)+((uRxBuf[4]&0x1f)<<7);
		pCoord->Y = (temp*480)/4096;
		DebugPrintf("\tPen up\t:  X_Coord = %d  Y_Coord = %d\r\n",pCoord->X,pCoord->Y);
	}
	else if(uRxBuf[0]==0x80)	//pen_down
	{
		temp = (uRxBuf[1]&0x7f)+((uRxBuf[2]&0x1f)<<7);
		pCoord->X = (temp*800)/4096;
		temp = (uRxBuf[3]&0x7f)+((uRxBuf[4]&0x1f)<<7);
		pCoord->Y = (temp*480)/4096;
		DebugPrintf("\tPen down:  X_Coord = %d  Y_Coord = %d\r\n",pCoord->X,pCoord->Y);
	}
	else
	{
		DebugPrintf("\GetCoord Error from DMA.\r\n");
		return TF_ERROR;
	}
	return TF_SUCCESS;
}
/***********************************************************
*	Function	：	spi_toupId_calibrate_set
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
TOUCH_FEEDBACK TOUCH_SetCalibrate(SSI_ID pId,float pInset)
{
	U32 j=0;
	S8 uRxBuf[4]={0,0,0,0};
	S8 uTouchBaseAddr,uCalibInset;
	while(TOUCH_ReqStartAddr(pId,&uTouchBaseAddr));
	uTouchBaseAddr += 0x0e;
	while(TOUCH_Enable(pId)!=TF_SUCCESS);
	uCalibInset = 2 * 12.5;
	//通过将所需值写入CalibrationInset寄存器来设置校准内移值
	SSI_RWChar(pId,0x55);
	SSI_RWChar(pId,0x05);
	SSI_RWChar(pId,0x21);
	SSI_RWChar(pId,0x00);
	SSI_RWChar(pId,uTouchBaseAddr);
	SSI_RWChar(pId,0x01);
	SSI_RWChar(pId,uCalibInset);
	while(gTouchPortIntFlag==0);
	gTouchPortIntFlag=0;
	for(j = 0; j < 4; j++)
	{
		uRxBuf[j] = SSI_RWChar(pId,0x00); 
	}
	//发出 <校准模式> 命令
	SSI_RWChar(pId,0x55);
	SSI_RWChar(pId,0x02);
	SSI_RWChar(pId,0x14);
	SSI_RWChar(pId,0x04); 		//4 point
	while(gTouchPortIntFlag==0);
	gTouchPortIntFlag=0;
	for(j = 0; j < 4; j++)
	{
		uRxBuf[j] = SSI_RWChar(pId,0x00); 
	}
	if((uRxBuf[0]!=0x55)||(uRxBuf[1]!=0x02)||(uRxBuf[3]!=0x14))	
	{
		return TF_ERROR;
	}
	if(uRxBuf[2]==TF_SUCCESS)		
	{
		//软件在显示屏左上象限显示第一个校准点目标，并提示用户触摸并释放目标。
		//等待用户触摸并释放第一个校准点目标
		DebugPrintf("Begin to calibrate the screen!\n");
		DebugPrintf("Please hit the screen firstly!\n");
		while(gTouchPortIntFlag==0);
		gTouchPortIntFlag=0;
		for(j = 0; j < 4; j++)
		{
			uRxBuf[j] = SSI_RWChar(pId,0x00); 
		}
		if((uRxBuf[0]!=0x55)||(uRxBuf[1]!=0x02)||(uRxBuf[3]!=0x14))
		{
			return TF_ERROR;
		}
		if(uRxBuf[2]==TF_SUCCESS)		
		{
			//软件在显示屏右上象限显示第二个校准点目标，并提示用户触摸并释放目标。
			DebugPrintf("Please hit the screen secondly!\n");
			while(gTouchPortIntFlag==0);
			gTouchPortIntFlag=0;
			for(j = 0; j < 4; j++)
			{
				uRxBuf[j] = SSI_RWChar(pId,0x00); 
			}
			if((uRxBuf[0]!=0x55)||(uRxBuf[1]!=0x02)||(uRxBuf[3]!=0x14))
			{
				return TF_ERROR;
			}
			if(uRxBuf[2]==TF_SUCCESS)		
			{
				//等待用户触摸并释放第三个校准点目标
				DebugPrintf("Please hit the screen thirdly!\n");
				while(gTouchPortIntFlag==0);
				gTouchPortIntFlag=0;
				for(j = 0; j < 4; j++)
				{
					uRxBuf[j] = SSI_RWChar(pId,0x00); 
				}
				if((uRxBuf[0]!=0x55)||(uRxBuf[1]!=0x02)||(uRxBuf[3]!=0x14))
				{
					return TF_ERROR;
				}
				if(uRxBuf[2]==TF_SUCCESS)		
				{
					//等待用户触摸并释放第四个校准点目标
					DebugPrintf("Please hit the screen forthly!\n");
					while(gTouchPortIntFlag==0);
					gTouchPortIntFlag=0;
					for(j = 0; j < 4; j++)
					{
						uRxBuf[j] = SSI_RWChar(pId,0x00); 
					}
					if((uRxBuf[0]!=0x55)||(uRxBuf[1]!=0x02)||(uRxBuf[3]!=0x14))
					{
						return TF_ERROR;
					}
					if(uRxBuf[2]==TF_SUCCESS)		
					{
						while(TOUCH_Enable(pId));	//enable toupId
						return (TOUCH_FEEDBACK)uRxBuf[2];
					}
				}
			}
		}
	}
	return TF_SUCCESS;
}
/***********************************************************
*	Function	：	spi_toupId_register_rw_verify
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TOUCH_VerifyReg(void)
{
	S8 uTouchBaseAddr=0;
	S8 Buffer_1[20]="\0",Buffer_2[20]="\0";
	while(TOUCH_ReqStartAddr(SSI_ID2,&uTouchBaseAddr));
	while(TOUCH_WriteReg(SSI_ID2,uTouchBaseAddr+0x02,6,Buffer_1));
	while(TOUCH_ReadReg(SSI_ID2,uTouchBaseAddr+0x02,6,Buffer_2));
}
/***********************************************************
*	Function	：	spi_toupId_reset
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TOUCH_Reset(void)
{
	S8 data = 0xff;
	TOUCH_Enable(SSI_ID2);
	while(TOUCH_WriteEEPROM(SSI_ID2,0x00,1,&data));
	//然后断电重启，之后toupId恢复到原始状态
}
/***********************************************************
*	Function	：	SSI_mode_select
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_SelModeTest(void)
{
	U32 uSel;
	DebugPrintf("\n\nSSI_Mode list : \r\n");
	DebugPrintf("\t0 : 00模式\r\n");
	DebugPrintf("\t1 : 01模式\r\n");
	DebugPrintf("\t2 : 10模式\r\n");
	DebugPrintf("\t3 : 11模式\r\n");
	DebugPrintf("\n\nSelect the SSI_Mode to test : ");
	uSel = DebugGetNum();
	DebugPrintf("\n\n");
	switch(uSel)
	{
		case 0:
			DebugPrintf("select 00模式!\r\n");
			gSsiPara.Scpol = SCPOL(0);
			gSsiPara.Scph = SCPH(0);
			break;
		case 1:
			DebugPrintf("select 01模式!\r\n");
			gSsiPara.Scpol = SCPOL(0);
			gSsiPara.Scph = SCPH(1);
			break;
		case 2:
			DebugPrintf("select 10模式!\r\n");
			gSsiPara.Scpol = SCPOL(1);
			gSsiPara.Scph = SCPH(0);
			break;
		case 3:
			DebugPrintf("select 11模式!\r\n");
			gSsiPara.Scpol = SCPOL(1);
			gSsiPara.Scph = SCPH(1);
			break;
		default:
			DebugPrintf("Invilid input,reset to 01模式!\r\n");
			gSsiPara.Scpol = SCPOL(0);
			gSsiPara.Scph = SCPH(1);
			break;
	}
}
/***********************************************************
*	Function	：	SSI_InterruptTest
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_InterruptTest(void)
{
	U32 i,j,uSel;
	DebugPrintf("\n\nSSI中断验证 list : \r\n");
	DebugPrintf("\t0 : 中断验证\r\n");
	DebugPrintf("\t1 : 中断屏蔽验证\r\n");
	DebugPrintf("\t2 : return\r\n");
	DebugPrintf("\n\nSelect the Int Mode to test : ");
	uSel = DebugGetNum();
	DebugPrintf("\n\n");
	switch(uSel)
	{
		case 0:
			//接收 FIFO满中断
			DebugPrintf("RXFIM Test\r\n");
			i = j = 0;
			for(i=0;i<8;i++)
			{
				SSI_InitPara(&gSsiPara);
				gSsiPara.SsiId = SSI_ID2;
				gSsiPara.OperateMode = TEST_MODE;
				gSsiPara.RxtFt = RECEIV_FT(i);
				gSsiPara.IntNum = (RXF_INT);
				SSI_Init(&gSsiPara);
				for(j=0; j<(gSsiPara.RxtFt+1); j++)
				{
					SSI_PutChar(gSsiPara.SsiId, 0xf0+j);
				}
				while(gSsiIntFlag[5] == 0);
				gSsiIntFlag[5] = 0;
				DebugPrintf("\tRXFTL = %d ; RXF Interrupt happen!\r\n",i);
			}
			//接收 FIFO上溢中断
			DebugPrintf("RXOIM Test\r\n");
			i = j = 0;
			SSI_InitPara(&gSsiPara);
			gSsiPara.OperateMode = TEST_MODE;
			gSsiPara.RxtFt = RECEIV_FT(7);
			gSsiPara.IntNum = (RXO_INT);
			SSI_Init(&gSsiPara);
			do{
				SSI_PutChar(gSsiPara.SsiId,j++);
			}while(gSsiIntFlag[4] == 0);
			gSsiIntFlag[4] = 0;
			DebugPrintf("\tRXFTL = 8 ; RXO Interrupt happen when has send %d bytes!\r\n",j);
			//接收 FIFO下溢中断
			DebugPrintf("RXUIM Test\r\n");
			i = j = 0;
			SSI_InitPara(&gSsiPara);
			gSsiPara.OperateMode = TEST_MODE;
			gSsiPara.RxtFt = RECEIV_FT(7);
			gSsiPara.IntNum = (RXU_INT);
			SSI_Init(&gSsiPara);
			read_reg(SSI_DR_CH(gSsiPara.SsiId));
			while(gSsiIntFlag[3] == 0);
			gSsiIntFlag[3] = 0;
			DebugPrintf("\tRXFTL = 8 ; RXU Interrupt happen!\r\n");
			//发送 FIFO上溢中断
			DebugPrintf("TXOIM Test\r\n");
			i = j = 0;
			SSI_InitPara(&gSsiPara);
			gSsiPara.OperateMode = TEST_MODE;
			gSsiPara.TxtFt = TRANS_FT(7);
			gSsiPara.IntNum = (TXO_INT);
			gSsiPara.Baud = BAUD(20000);
			SSI_Init(&gSsiPara);
			do{
				write_reg(SSI_DR_CH(gSsiPara.SsiId), j++);
			}while(gSsiIntFlag[2] == 0);
			gSsiIntFlag[2] = 0;
			DebugPrintf("\tRXFTL = 8 ; TXO Interrupt happen when has send %d bytes!\r\n",j);
			//发送 FIFO空中断
			DebugPrintf("TXEIM Test\r\n");
			i = j = 0;
			for(i=0;i<8;i++)
			{
				SSI_InitPara(&gSsiPara);
				gSsiPara.OperateMode = TEST_MODE;
				gSsiPara.TxtFt = TRANS_FT(i);
				gSsiPara.IntNum = (TXE_INT);
				SSI_Init(&gSsiPara);
				for(j=0; j<gSsiPara.TxtFt; j++)
				{
					SSI_PutChar(gSsiPara.SsiId, 0xf0+j);
				}
				while(gSsiIntFlag[1] == 0);
				gSsiIntFlag[1] = 0;
				DebugPrintf("\tTXETL = %d ; TXE Interrupt happen!\r\n",i);
			}
			break;
		case 1:
			//接收 FIFO满中断
			DebugPrintf("RXFIM IMR Test\r\n");
			i = j = 0;
			for(i=0;i<8;i++)
			{
				SSI_InitPara(&gSsiPara);
				gSsiPara.OperateMode = TEST_MODE;
				gSsiPara.RxtFt = RECEIV_FT(i);
				gSsiPara.IntNum = (RXF_INT);
				SSI_Init(&gSsiPara);
				for(j=0;j<(gSsiPara.RxtFt+1);j++)
					SSI_PutChar(gSsiPara.SsiId,0xf0+j);
				SSI_Delay(100);
				if(gSsiIntFlag[5] == 0)
				{
					DebugPrintf("\tRXFTL = %d ; RFF IMR Success!\r\n",i);
				}
				else
				{
					DebugPrintf("\tRXFTL = %d ; RFF IMR Failure!\r\n",i);
				}
			}
			//接收 FIFO上溢中断
			DebugPrintf("RXOIM IMR Test\r\n");
			i = j = 0;
			SSI_InitPara(&gSsiPara);
			gSsiPara.OperateMode = TEST_MODE;
			gSsiPara.RxtFt = RECEIV_FT(7);
			gSsiPara.IntNum = (RXO_INT);
			SSI_Init(&gSsiPara);
			do{
				SSI_PutChar(gSsiPara.SsiId,j++);
			}while(j<1000);
			SSI_Delay(100);
			if(gSsiIntFlag[4] == 0)
			{
				DebugPrintf("\tRXFTL = 8 ; RXO IMR Success!\r\n");
			}
			else
			{
				DebugPrintf("\tRXFTL = 8 ; RXO IMR Failure!\r\n");
			}
			//接收 FIFO下溢中断
			DebugPrintf("RXUIM IMR Test\r\n");
			i = j = 0;
			SSI_InitPara(&gSsiPara);
			gSsiPara.OperateMode = TEST_MODE;
			gSsiPara.RxtFt = RECEIV_FT(7);
			gSsiPara.IntNum = (RXU_INT);
			SSI_Init(&gSsiPara);
			for(i=0;i<10;i++)
				read_reg(SSI_DR_CH(gSsiPara.SsiId));
			if(gSsiIntFlag[3] == 0)
			{
				DebugPrintf("\tRXFTL = 8 ; RXU IMR Success!\r\n");
			}
			else
			{
				DebugPrintf("\tRXFTL = 8 ; RXU IMR Failure!\r\n");
			}
			//发送 FIFO上溢中断
			DebugPrintf("TXOIM IMR Test\r\n");
			i = j = 0;
			SSI_InitPara(&gSsiPara);
			gSsiPara.OperateMode = TEST_MODE;
			gSsiPara.RxtFt = RECEIV_FT(7);
			gSsiPara.IntNum = (TXO_INT);
			gSsiPara.Baud = BAUD(20000);
			SSI_Init(&gSsiPara);
			do{
				write_reg(SSI_DR_CH(gSsiPara.SsiId), j++);
			}while(j<1000);
			if(gSsiIntFlag[2] == 0)
			{
				DebugPrintf("\tRXFTL = 8 ; TXO IMR Success!\r\n");
			}
			else
			{
				DebugPrintf("\tRXFTL = 8 ; TXO IMR Failure!\r\n");
			}
			//发送 FIFO空中断
			DebugPrintf("TXEIM IMR Test\r\n");
			i = j = 0;
			for(i=0;i<8;i++)
			{
				SSI_InitPara(&gSsiPara);
				gSsiPara.OperateMode = TEST_MODE;
				gSsiPara.RxtFt = RECEIV_FT(i);
				gSsiPara.IntNum = (TXE_INT);
				SSI_Init(&gSsiPara);
				for(j=0;j<TRANS_FT(i);j++)
					SSI_PutChar(gSsiPara.SsiId,0xf0+j);
				SSI_Delay(100);
				if(gSsiIntFlag[1] == 0)
				{
					DebugPrintf("\tTXFTL = %d ; TXE IMR Success!\r\n",i);
				}
				else
				{
					DebugPrintf("\tTXFTL = %d ; TXE IMR Failure!\r\n",i);
				}
			}
			break;
		default:
			return;
	}
}
/***********************************************************
*	Function	：	SSI_BasicFunTest
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_BasicFunTest(void)
{
	U32 i,j,uSel;
	DebugPrintf("\n\nSSI 基础功能验证 list : \r\n");
	DebugPrintf("\t0 : 只发送模式验证\r\n");
	DebugPrintf("\t1 : 数据帧长度验证\r\n");
	DebugPrintf("\t2 : 波特率验证\r\n");
	DebugPrintf("\t3 : return\r\n");
	DebugPrintf("\n\nSelect the Int Mode to test : ");
	uSel = DebugGetNum();
	DebugPrintf("\n\n");
	switch(uSel)
	{
		case 0:
			DebugPrintf("Tx only mode verify begin!\r\n");
			SSI_InitPara(&gSsiPara);
			gSsiPara.OperateMode = TEST_MODE;
			gSsiPara.Tmod = TRAN_TO;
			SSI_Init(&gSsiPara);
			SSI_PutChar(gSsiPara.SsiId, 0x5a);
			delay(100);
			if(read_reg(SSI_DR_CH(gSsiPara.SsiId)) != 0x5a)
			{
				DebugPrintf("\tTx only mode verify success!\r\n",i,j);
			}
			break;
		case 1://改变数据帧长度下的数据传输
			DebugPrintf("frame_len change verify begin!\r\n");
			for(i=3;i<15;i++)
			{
				SSI_InitPara(&gSsiPara);
				gSsiPara.OperateMode = TEST_MODE;
				gSsiPara.FrameLen = FRAME_LEN(i);
				SSI_Init(&gSsiPara);
				for(j=0;j<8;j++)
				{
					SSI_PutChar(gSsiPara.SsiId, j);
					while(*(RP)SSI_SR_CH(gSsiPara.SsiId) & 0x08 == 0);
					if(read_reg(SSI_DR_CH(gSsiPara.SsiId)) != j)
					{
						DebugPrintf("\tframe_len %d change verify error when tran %d byte!\r\n",i,j);
						return ;
					}
				}
				DebugPrintf("\tframe_len %d change verify success!\r\n",i);
			}
			break;
		case 2:
			//不同波特率下的数据传输
			DebugPrintf("baud change verify begin!\r\n");
			//范围是2~65534
			for(i=2;i<65534;i+=200)
			{
				SSI_InitPara(&gSsiPara);
				gSsiPara.OperateMode = TEST_MODE;
				gSsiPara.Baud = BAUD(i);
				SSI_Init(&gSsiPara);
				for(j=0;j<8;j++)
				{
					SSI_PutChar(gSsiPara.SsiId, j);
					if(read_reg(SSI_DR_CH(gSsiPara.SsiId)) != j)
					{
						DebugPrintf("\tbaud %d change verify error when tran %d byte!\r\n",i,j);
						return ;
					}
				}
				DebugPrintf("\tbaud %d change verify success!\r\n",i);
			}
			break;
		default:
			break;
	}
}
/***********************************************************
*	Function	：	SSI_DMATranVerify
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_DMATranVerify(U32 baud,U32 burst_length,U32 burst_width,U32 block_size,U32 dmatdl,U32 dmardl)
{
	int i,j=0;
	U32 tx = 0x41000000,rx = 0x42000000;
	U32 width = (burst_width+1)*8;		
	DebugPrintf("SSI2 Para - baud : %d,framelen : %d\r\n",baud,width);
	DebugPrintf("SSI2 DMAC Para - burst_length : %d,burst_width : %d,block_size : %d,dmatdl : %d,dmardl : %d\r\n",burst_length,(burst_width+1)*8,block_size,dmatdl,dmardl);				
	for(i=0;i<256;i++)
	{
		*(RP8)(tx+i)=i;
	}
	for(i=0;i<256;i++)
	{
		*(RP8)(rx+i)=1;
	}
	SSI_Enable(2,FALSE);
	*(RP)GPIO_PORTE_SEL &=~(0xff00);//设置SSI为的接口为专用用途
	*(RP)SSI2_SSI2ENR = 0x0; //设置前紧禁止能SSI
	*(RP)SSI2_CONTROL0 = 0x840+width-1;
	*(RP)SSI2_CONTROL1 = 0x1; //数据帧数量为1
	*(RP)SSI2_BAUDR  = baud; //设置波特率
	*(RP)SSI2_TXFTLR = 0x1;  //发送阈值为1
	*(RP)SSI2_RXFTLR = 0x0;  //接受阈值为1
    *(RP)SSI2_TXFLR =0x1;
    *(RP)SSI2_RXFLR =0x1;
	*(RP)SSI2_IMR	= 0x0;  //屏蔽中断 
	*(RP)SSI2_DMACR  =0x3;//使能DMA
	*(RP)SSI2_DMATDLR =dmatdl;
	*(RP)SSI2_DMARDLR =dmardl;
	*(RP)SSI2_SER =0x1;
	*(RP)SSI2_SSI2ENR = 0x1; //设置完使能ssi
	SSI_Enable(2,TRUE);
	
	//配置发送DMAC
	write_reg(DMAC2_CFG ,0x00000001);     //dmac enable
	write_reg(DMAC2_CFG  + 0x4,0x00000000);
	write_reg(DMAC2_CTL0,(1| burst_width<<1 | burst_width<<4 | 3<<7 | 0<<9 |burst_length<<11 | burst_length<<14 | 0<<17 | 1<<20 | 1<<23 | 0<<25 | 0<<27));     //设置传输的位宽为32bits
	write_reg(DMAC2_CTL0 + 0x4,block_size);    //要传输的单元数
	write_reg(DMAC2_CFG0,0x00000200);        //目的硬件握手
	write_reg(DMAC2_CFG0 + 0x4,0x1906);//设置硬件握手接口
	write_reg(DMAC2_LLP0,0x00000000); 
	write_reg(DMAC2_LLP0 + 0x4,0x00000000);
	write_reg(DMAC2_SAR0,tx);//设置源为DR
	write_reg(DMAC2_SAR0 + 0x4,0x00000000);
	write_reg(DMAC2_DAR0,SSI2_DR);//设置目的
	write_reg(DMAC2_DAR0 + 0x4,0x00000000);
	write_reg(DMAC2_MASKTRF,0x00000101);
	write_reg(DMAC2_MASKBLK,0x00000101);
	write_reg(DMAC2_MASKSRCTR,0X00000101);
	write_reg(DMAC2_MASKDSTTR,0X00000101);
	
	//配置接收DMAC
	write_reg(DMAC2_CFG ,0x00000001);     //dmac enable
	write_reg(DMAC2_CFG  + 0x4,0x00000000);
	write_reg(DMAC2_CTL1,(1| burst_width<<1 | burst_width<<4 | 0<<7 | 3<<9 |burst_length<<11 | burst_length<<14 | 0<<17 | 2<<20 | 0<<23 | 1<<25 | 0<<27));     //设置传输的位宽为32bits
	write_reg(DMAC2_CTL1 + 0x4,block_size);    //要传输的单元数
	write_reg(DMAC2_CFG1,0x00000200);        //目的硬件握手
	write_reg(DMAC2_CFG1 + 0x4,0x1906);//设置硬件握手接口
	write_reg(DMAC2_LLP1,0x00000000); 
	write_reg(DMAC2_LLP1 + 0x4,0x00000000);
	write_reg(DMAC2_SAR1,SSI2_DR);//设置源为DR
	write_reg(DMAC2_SAR1 + 0x4,0x00000000);
	write_reg(DMAC2_DAR1,rx);//设置目的
	write_reg(DMAC2_DAR1 + 0x4,0x00000000);
	write_reg(DMAC2_MASKTRF,0x00000202);
	write_reg(DMAC2_MASKBLK,0x00000202);
	write_reg(DMAC2_MASKSRCTR,0X00000202);
	write_reg(DMAC2_MASKDSTTR,0X00000202);
	//同时使能两个通道
	write_reg(DMAC2_CHENA,0x00000303); //每个DMA有8个通道
	
	for(i=0;i<10000;i++);//for delay		
	for(i=0;i<block_size;i++)
	{	
		if (*(RP8)(tx+i) != *(RP8)(rx+i))
		{
			DebugPrintf("\tSSI DMA Test error when send/rev %d byte!\r\n\r\n",i);
			j++;
		}
	}
	if(j==0)
		DebugPrintf("\tDMAC test Success!\r\n\r\n");
}
/***********************************************************
*	Function	：	SSI_DMAFunTest
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_DMAFunTest(void)
{
	U32 buad = 0x300,burst_length,burst_width,block_size,dmatdl,dmardl;
	U32 n;
	//DMAburst == 1
	DebugPrintf("\r\n\r\nDMAC DMAburst = 1 test begin!\r\n\r\n");
	burst_width = 0;
	burst_length = 0;
	dmardl = 0;
	for(n=20;n<21;n++)
	{
		block_size = n;
		for(dmatdl=0;dmatdl<=20;dmatdl++)
		{
			SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
		}
	}
	DebugPrintf("\r\n\r\nDMAC DMAburst = 4 test begin!\r\n\r\n");
	//DMAburst == 4
	burst_width = 0;
	burst_length = 1;
	for(n=20;n<21;n++)
	{
		block_size = 4*n;
		dmardl = 3;
		for(dmatdl=0;dmatdl<=17;dmatdl++)
		{
			SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
		}
		
		block_size = 4*n+1;
		for(dmatdl=0;dmatdl<=17;dmatdl++)
		{
			for(dmardl=3;dmardl<5;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 4*n+2;
		for(dmatdl=0;dmatdl<=17;dmatdl++)
		{
			for(dmardl=3;dmardl<6;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 4*n+3;
		for(dmatdl=0;dmatdl<=17;dmatdl++)
		{
			for(dmardl=3;dmardl<7;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
	}
	DebugPrintf("\r\n\r\nDMAC DMAburst = 8 test begin!\r\n\r\n");
	//DMAburst == 8
	burst_width = 0;
	burst_length = 2;
	for(n=10;n<11;n++)
	{
		block_size = 8*n;
		for(dmatdl=0;dmatdl<=13;dmatdl++)
		{
			for(dmardl=7;dmardl<8;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 8*n+1;
		for(dmatdl=0;dmatdl<=13;dmatdl++)
		{
			for(dmardl=7;dmardl<9;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 8*n+2;
		for(dmatdl=0;dmatdl<=13;dmatdl++)
		{
			for(dmardl=7;dmardl<10;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 8*n+3;
		for(dmatdl=0;dmatdl<=13;dmatdl++)
		{
			for(dmardl=7;dmardl<11;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 8*n+4;
		for(dmatdl=0;dmatdl<=13;dmatdl++)
		{
			for(dmardl=7;dmardl<12;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 8*n+5;
		for(dmatdl=0;dmatdl<=13;dmatdl++)
		{
			for(dmardl=7;dmardl<13;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 8*n+6;
		for(dmatdl=0;dmatdl<=13;dmatdl++)
		{
			for(dmardl=7;dmardl<14;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 8*n+7;
		for(dmatdl=0;dmatdl<=13;dmatdl++)
		{
			for(dmardl=7;dmardl<15;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
	}
	//DMAburst == 16
	DebugPrintf("\r\n\r\nDMAC DMAburst = 16 test begin!\r\n\r\n");
	burst_width = 0;
	burst_length = 3;
	dmardl = 0;
	for(n=5;n<6;n++)
	{
		block_size = 16*n;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<16;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 16*n+1;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<17;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 16*n+2;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<18;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 16*n+3;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<19;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 16*n+4;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<20;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 16*n+5;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<21;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 16*n+6;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<22;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 16*n+7;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<22;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 16*n+8;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<22;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 16*n+9;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<22;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 16*n+10;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<22;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 16*n+11;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<22;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 16*n+12;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<22;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}

		block_size = 16*n+13;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<22;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 16*n+14;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<22;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
		
		block_size = 16*n+15;
		for(dmatdl=0;dmatdl<=5;dmatdl++)
		{
			for(dmardl=15;dmardl<22;dmardl++)
			{
				SSI_DMATranVerify(buad,burst_length,burst_width,block_size,dmatdl,dmardl);
			}
		}
	}
}
/***********************************************************
*	Function	：	SSI_ADCTest
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_ADCTest(void)  //adc_pId : 0 1 2 3
{
	U32 pId = 1,j = 0,i = 0;
	U32 adc_ch,m;
	float vef = 3.3;
	S8 uTxBuf[16] = {0x01,0x00,0xff};
	S8 uRxBuf[16] = {0};
	float temp=0;
	DebugPrintf("\n\nSelect the ADpIdannel (<4) : ");
	adc_ch = DebugGetNum();
	DebugPrintf("\n\n");
	gSsiPara.SsiId = SSI_ID1;
	gSsiPara.Scpol = SCPOL(0);
	gSsiPara.Scph = SCPH(1);
	SSI_Init(&gSsiPara);
	while(m<10)
	{
		m++;
		uTxBuf[1] = (1<<7)|(adc_ch<<4);
		SSI_PutChar(gSsiPara.SsiId,uTxBuf[0]);
		SSI_PutChar(gSsiPara.SsiId,uTxBuf[1]);
		SSI_PutChar(gSsiPara.SsiId,uTxBuf[2]);
	    for(j = 0; j < 3; j++)
	    {
	    	uRxBuf[j] = read_reg(SSI_DR_CH(gSsiPara.SsiId)); 
	    }
	    i=((uRxBuf[1]&0x03)<<8)|uRxBuf[2];
	    temp=((float)(i*vef))/1024;
	    DebugPrintf("\tthe adc value is %f!\r\n",temp);
	}
}
/***********************************************************
*	Function	：	SSI_TouchTest
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_TOUCHTest(void)
{
	Coordinate uCoord = {0,0};
	U32 i=0,mode = 0;
	TOUCH_SetPort();
	gSsiPara.SsiId = SSI_ID2;
	gSsiPara.Scpol = SCPOL(0);
	gSsiPara.Scph = SCPH(1);
	SSI_Init(&gSsiPara);
	DebugPrintf("\n\nSelect the Operate Mode : \r\n");
	DebugPrintf("\t0 : Normal mode\r\n");
	DebugPrintf("\t1 : DMA mode\r\n");
	DebugPrintf("\t2 : Change Baud Test\r\n\n");
	mode = DebugGetNum();
	DebugPrintf("\n\n");
	if(mode == 0)
	{
		DebugPrintf("Normal mode begin!\r\n");
		gSsiPara.OperateMode = NORMAL_MODE;
		gSsiPara.Baud = BAUD(2000);
		gSsiPara.FrameLen = FRAME_LEN(7);
		gSsiPara.TxtFt = TRANS_FT(1);
		gSsiPara.RxtFt = RECEIV_FT(1);
		SSI_Init(&gSsiPara);
		TOUCH_Enable(gSsiPara.SsiId);
		while(i<20)
		{
			i++;
			//读取触摸坐标
			TOUCH_GetCoord(gSsiPara.SsiId, &uCoord);
			SSI_Delay(100);
		}
	}
	else if(mode == 1)
	{
		DebugPrintf("DMA mode begin!\r\n");
		gSsiPara.OperateMode = NORMAL_MODE;
		gSsiPara.Baud = BAUD(2000);
		gSsiPara.FrameLen = FRAME_LEN(7);
		gSsiPara.TxtFt = TRANS_FT(1);
		gSsiPara.RxtFt = RECEIV_FT(1);
		gSsiPara.DmaRxEn = TRUE;
		gSsiPara.DmaRdl = 5;
		gSsiPara.DmaTxEn = TRUE;
		gSsiPara.DmaTdl = 1;
		SSI_Init(&gSsiPara);
		TOUCH_Enable(gSsiPara.SsiId);
		while(i<20)
		{
			i++;
			//读取触摸坐标
			TOUCH_DMAGetCoord(2,&uCoord);
			SSI_Delay(100);
		}
	}
	else if(mode == 2)
	{
		gSsiPara.OperateMode = NORMAL_MODE;
		gSsiPara.Baud = BAUD(0x300);
		gSsiPara.FrameLen = FRAME_LEN(7);
		gSsiPara.TxtFt = TRANS_FT(1);
		gSsiPara.RxtFt = RECEIV_FT(1);
		SSI_Init(&gSsiPara);
		TOUCH_Enable(gSsiPara.SsiId);
		//SPIDV=0x300改变到SPIDV=0x400
		DebugPrintf("SPIDV=0x300改变到SPIDV=0x400\r\n");
		SSI_SetBaud(2,0x300);
		TOUCH_GetCoord(gSsiPara.SsiId, &uCoord);
		SSI_SetBaud(2,0x400);
		TOUCH_GetCoord(gSsiPara.SsiId, &uCoord);
		//SPIDV=0x300改变到SPIDV=0x500
		DebugPrintf("SPIDV=0x300改变到SPIDV=0x500\r\n");
		SSI_SetBaud(2,0x300);
		TOUCH_GetCoord(gSsiPara.SsiId, &uCoord);
		SSI_SetBaud(2,0x500);
		TOUCH_GetCoord(gSsiPara.SsiId, &uCoord);
		//SPIDV=0x400改变到SPIDV=0x300
		DebugPrintf("SPIDV=0x400改变到SPIDV=0x300\r\n");
		SSI_SetBaud(2,0x400);
		TOUCH_GetCoord(gSsiPara.SsiId, &uCoord);
		SSI_SetBaud(2,0x300);
		TOUCH_GetCoord(gSsiPara.SsiId, &uCoord);
		//SPIDV=0x400改变到SPIDV=0x500
		DebugPrintf("SPIDV=0x400改变到SPIDV=0x500\r\n");
		SSI_SetBaud(2,0x400);
		TOUCH_GetCoord(gSsiPara.SsiId, &uCoord);
		SSI_SetBaud(2,0x500);
		TOUCH_GetCoord(gSsiPara.SsiId, &uCoord);
		//SPIDV=0x500改变到SPIDV=0x300
		DebugPrintf("SPIDV=0x500改变到SPIDV=0x300\r\n");
		SSI_SetBaud(2,0x500);
		TOUCH_GetCoord(gSsiPara.SsiId, &uCoord);
		SSI_SetBaud(2,0x300);
		TOUCH_GetCoord(gSsiPara.SsiId, &uCoord);
		//SPIDV=0x500改变到SPIDV=0x400
		DebugPrintf("SPIDV=0x500改变到SPIDV=0x400\r\n");
		SSI_SetBaud(2,0x500);
		TOUCH_GetCoord(gSsiPara.SsiId, &uCoord);
		SSI_SetBaud(2,0x400);
		TOUCH_GetCoord(gSsiPara.SsiId, &uCoord);
	}
}

const testFuncMenu SSI_Menu[] =
{
	SSI_SelModeTest,	"SSI 模式选择	",
	SSI_InterruptTest,	"SSI 中断功能测试",
	SSI_BasicFunTest,	"SSI 基本传输功能测试",
	SSI_DMAFunTest,		"SSI DMA传输测试",
	SSI_ADCTest,		"SSI 外接AD(MCP3004)",
	SSI_TOUCHTest,		"SSI 外接TOUCH(AR1020)"
};
/***********************************************************
*	Function	：	SSI_Test
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void SSI_Test(void)
{
	U32 i,uSel,uMenuNum=(sizeof(SSI_Menu)/8);
	char temp[10] = "return";
	gDmacCfg = (DMAC_CFG *)malloc(sizeof(DMAC_CFG));
	while(1)
	{
		DebugPrintf("\n\n");
		DebugPrintf("******************************************\n");
		DebugPrintf("*        SSI - Test firmware v0.1        *\n");
		DebugPrintf("******************************************\n");
		
		SSI_InitPara(&gSsiPara);
		SSI_SetPort();
		
		for (i=0; i<uMenuNum; i++)
		{
			DebugPrintf("\t%2d: %s  \r\n", i, SSI_Menu[i].desc);
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
			(SSI_Menu[uSel].func) ();
		else
			break;
	}
}
/***********************************************************
*	Function	：	SSI_IRQHandle
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void txe_intr(SSI_ID pId)
{
	gSsiIntFlag[1]= 1;  
    //write_reg(SSI_DR_CH(pId), 0x0);
    *(RP)SSI_ICR_CH(pId);
}
void txo_intr(SSI_ID pId)
{ 
	gSsiIntFlag[2]=1; 	
	*(RP) SSI_TXOICR_CH(pId);// clear transfer overflow interruption
}
void rxu_intr(SSI_ID pId)
{
	gSsiIntFlag[3] = 1;
     *(RP)SSI_RXUICR_CH(pId); //clear receive fifo underflow interruption
}
void rxo_intr(SSI_ID pId)
{
	gSsiIntFlag[4] = 1;
    *(RP)SSI_RXOICR_CH(pId);  //clear receive fifo overflow interruption
}
void rxf_intr(SSI_ID pId)
{
	U32 i;
	gSsiIntFlag[5] =1;
	for(i=0; i<=gSsiPara.RxtFt; i++)
		*(RP)SSI_DR_CH(pId);
	*(RP)SSI_ICR_CH(pId);
}
void (*isr_handler[])(SSI_ID uId) = {
										txe_intr, 
										txo_intr, 
										rxu_intr, 
										rxo_intr, 
										rxf_intr
									};
void SSI1_IRQHandler (void)
{
	U32 int_src = 0;
	U32 int_num = 0;
	U32 i;
	int_src = *(RP)SSI_ISR_CH(SSI_ID1);
	for(i=0; i<5; i++)
	{
		int_num = int_src & (0x1<<i);
		if(int_num == (0x1<<i))
		{
		    isr_handler[i](SSI_ID1);  
		    break;
		}
	}
}	
void SSI2_IRQHandler (void)
{	
	U32 int_src = 0;
	U32 int_num = 0;
	U32 i;
	int_src = *(RP)SSI_ISR_CH(SSI_ID2);
	VIC_MaskAllInt(IRQ);
	for(i=0; i<5; i++)
	{
		int_num = int_src & (0x1<<i);
		if(int_num == (0x1<<i))
		{
		    isr_handler[i](SSI_ID2);  
		    break;
		}
	}
	//VIC_UnmaskAllInt(IRQ);
}
void SSI3_IRQHandler (void)
{
    return;
}
