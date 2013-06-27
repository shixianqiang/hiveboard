/******************************************************************
*File name   :  "iis.c"
*Author      :  SixRoom
*Date        :  2011-02-15
*Description :  The code is wrote to configure the I2S.
*Version     :  1.06
*******************************************************************/

/*
|---------------|------------------|---------------|------------------------------------------|
|    version    |       date       |     Author    |           modify infomation              |
|---------------|------------------|---------------|------------------------------------------|
|    v1.00      |    2010-05-19    |  Yanghq       | 1) create this file   		              |
|    v1.01      |    2010-06-27    |  Yanghq       | 1) modify the coding style	              |
|				|				   |			   | 2) modify the interrupt handler function |                  
|    v1.02      |    2010-06-28    |  Yanghq       | 1) add the master mode of iis	          |
|				|				   |			   | 2) combine the master mode and slave     |
|               |                  |               |    mode in one project                   |
|    v1.03      |    2010-06-29    |  Yanghq       | 1) modify the dma transfer				  |
|    v1.04      |    2010-07-27    |  Yanghq       | 1) modify the coding style			      |
|    v1.05      |    2011-01-19    |  Yanghq       | 1) unify the coding style with workmates |
|    v1.06      |    2011-02-15    |  SixRoom      | 1) unify the coding style                |
|---------------|------------------|---------------|------------------------------------------|
*/

#include	"../include/iis.h"
#include	"../include/codec.h"
#include	"../include/dmac.h"
#include 	"../include/uart.h"

#define AUDIO_ADDR	0x45000000

AUDIO  gWaveAudio;		//gWaveAudio; /*audio informations:samplerate,datalength,datasize,audioaddr*/

volatile U32 flag[6] = {0,0,0,0,0,0};
                                 
volatile U32  gSrcupdata, gDstupdata;  

extern DMAC_CFG *gDmacCfg;

/********************************************************************
* Function    £º I2S_EnableModule
* Parameter   £º no para
* Return      £º no return
* Description £º Enable the i2s controller.
* *********************************************************************/
void I2S_Enable(BOOL pEn)
{   
    write_reg(I2S_IER , pEn);    
}
/********************************************************************
* Function    £º I2S_EnableTxBlock
* Parameter   £º no para
* Return      £º no return
* Description £º Enable the transmitter block.     
*********************************************************************/
void I2S_EnableBlock(IIS_DIR pDir, BOOL pEn)
{    
	if(pDir == IIS_IN)
	{
		write_reg(I2S_IRER, pEn);
	}
	else if(pDir == IIS_OUT)
	{
		write_reg(I2S_ITER , pEn); 
	}
}
/********************************************************************
* Function    £º I2S_EnableClock
* Parameter   £º no para
* Return      £º no return
* Description £º Enable the master mode clock.     
*********************************************************************/
void I2S_EnableClock(BOOL pEn)
{
    write_reg(I2S_CER , pEn);    
} 
/********************************************************************
* Function    £º I2S_SetClock
* Parameter   £º pWs_freq , pWsSclk , pGcSclk
* Return      £º no return
* Description £º Initialize the wordselect and gating of sclk cycles
*                reset value of the two parameters are zero.       
*********************************************************************/
void I2S_SetClock(U32 pWsFreq , WS_LEN pWsSclk , GC_LEN pGcSclk)
{
    write_reg(I2S_CCR, (pWsFreq<<8) + (pWsSclk << 3) + (pGcSclk));         
}
/********************************************************************
* Function    £º I2S_SetMasterClock
* Parameter   £º no para
* Return      £º no return
* Description £º Set Master Clock source                
*********************************************************************/
void I2S_SetMasterClock(CLOCK_SOURCE pSource, CLOCK_ATTRI pAttri)
{
	if(pSource == HARDWARE)
	{
	    write_reg(I2S_SCR , 0x0);
	}
	else if(pSource == SOFTWARE)
	{
		switch(pAttri)
		{
			case SAMPLE44_LENGTH16:
				write_reg(I2S_SCR , 0x321e1);
				break;
			case SAMPLE48_LENGTH16:
				write_reg(I2S_SCR , 0x321e1);
				break;
			case NOCARE:
				write_reg(I2S_SCR , 0x0);
				break;
			default:break;
		}
	}
}
/********************************************************************
* Function    £º I2S_FlushFifo(IIS_IN)
* Parameter   £º no para
* Return      £º no return
* Description £º Flush the receiver block FIFO                
*********************************************************************/
void I2S_FlushFifo(IIS_DIR pDir)
{
	if(pDir == IIS_IN)
	{
		write_reg(I2S_RXFFR , 0x01);
	}
	if(pDir == IIS_OUT)
	{
		write_reg(I2S_TXFFR , 0x01);
	}
}
/********************************************************************
* Function    £º I2S_EnableChannel(pNum, IIS_IN, TRUE)
* Parameter   £º pNum
* Return      £º no return
* Description £º Enable one of the receive channels of receiver block,
*                "uNum" is used to represents the different
*               channel number and valid value are 0,1,2,3.      
********************************************************************/
void I2S_EnableChannel(IIS_CH pNum, IIS_DIR pDir, BOOL pEn) 
{
	if(pDir == IIS_IN)
	{
		write_reg(I2S_RER(pNum), pEn);
	}
	else if(pDir == IIS_OUT)
	{
		write_reg(I2S_TER(pNum), pEn);
	}                                    
}
/********************************************************************
* Function    £º I2S_EnableDisableChannels
* Parameter   £º pState
* Return      £º no return
* Description £º Enable each channel of transmitter block.         
*********************************************************************/
void I2S_EnableDisableChannels(IIS_STATE pState)
{
	U32 uTemp;
	switch(pState)
	 {
	     case TxALLENABLE:
	    	 for(uTemp = 0 ; uTemp<4 ; uTemp++)
	    		 write_reg(I2S_TER(uTemp) , 0x1);
	    	 break;
	     case TxALLDISABLE:
	    	 for(uTemp = 0 ; uTemp<4 ; uTemp++)
	    		 write_reg(I2S_TER(uTemp) , 0x0);
	    	 break;
	     case RxALLENABLE:
	    	 for(uTemp = 0 ; uTemp<4 ; uTemp++)
	    		 write_reg(I2S_RER(uTemp) , 0x1);
	    	 break;
	     case RxALLDISABLE:
	    	 for(uTemp = 0 ; uTemp<4 ; uTemp++)
	    		 write_reg(I2S_RER(uTemp) , 0x0);
	    	 break;
	     default: break;
	 }
}
/********************************************************************
* Function    £º I2S_SetDataLen
* Parameter   £º pLength , pNum
* Return      £º no return
* Description £º Set each channel's date length of receiver block,
*                set the date length to ignore/12/16/20bits 
*               (default value is 24bits).        
*********************************************************************/
void I2S_SetDataLen(IIS_CH pNum, IIS_DIR pDir, IIS_DATALEN pLen)
{
	if(pDir == IIS_IN)
	{
		write_reg(I2S_RCR(pNum), pLen);
	}
	else if(pDir == IIS_OUT)
	{
		write_reg(I2S_TCR(pNum), pLen); 
	}
}
/********************************************************************
* Function    £º I2S_MaskInt
* Parameter   £º pNum , pInttype
* Return      £º no return
* Description £º Mask each or all interrupt of i2s.        
*********************************************************************/
void I2S_MaskInt(IIS_CH pNum, U32 pIntNum, BOOL pMask)
{
    U32 uTemp;    
    uTemp = read_reg(I2S_IMR(pNum)); 
	if(pMask == TRUE)
	{
		uTemp |= (pIntNum);
	}
	else if(pMask == FALSE)
	{
		uTemp &= ~(pIntNum); 
	}
	write_reg(I2S_IMR(pNum) , uTemp); 
}
/********************************************************************
* Function    £º I2S_SetFifoLevel
* Parameter   £º pNum , pDirection , pLevel
* Return      £º no return
* Description £º pLevel's default value is 3 , direction: RECEIVE 1 , TRANSMIT  0.        
*********************************************************************/ 
void I2S_SetFifoLevel(IIS_CH pNum, IIS_DIR pDir, U32 pLevel)
{	  	
    if(pDir == IIS_IN)
	{
        write_reg(I2S_RFCR(pNum) , pLevel);  	    
	}    	 
    else if(pDir == IIS_OUT)
	{
	    write_reg(I2S_TFCR(pNum) , pLevel);
   	}
}
/********************************************************************
* Function    £º I2S_ResetDma
* Parameter   £º pDirection 
* Return      £º no return
* Description £º Configue dma operation reset      
********************************************************************/ 
void I2S_ResetDma(IIS_DIR pDir)
{
    if(pDir == IIS_IN)
	{
	    write_reg(I2S_RRXDMA , 1);
	}    
    else if(pDir == IIS_OUT)
	{
	    write_reg(I2S_RTXDMA , 1);
	}    	
}
/********************************************************************
* Function    £º I2S_ResetDma
* Parameter   £º no para
* Return      £º no return
* Description £º Set the fast mode in dma playback.      
*********************************************************************/
void I2S_SetFastMode(void)
{ 
    write_reg(I2S_ITER , read_reg(I2S_ITER) | (1 << 1));
}
/********************************************************************
* Function    £º I2S_GetAudioInfo
* Parameter   £º pAddr , info
* Return      £º no return
* Description £º Get the audio information from the loaded audio data in memory     
*********************************************************************/
void I2S_GetAudioInfo(AUDIO *info , RP32 pAddr)
{	
	info->samplerate = *(pAddr+0x18); /*audio sample rate value stored in memory*/
	info->datalength = *(pAddr+0x20); /*audio date length value stored in memory*/   
	info->audioaddr  =   pAddr+0x2c ; /*the first address where stores the aduio date*/
	info->datasize   = *(pAddr+0x28); /*audio date size stored in memory*/	
}
/********************************************************************
* Function    £º I2S_InitDmaTxRx
* Parameter   £º para , pDir
* Return      £º no return
* Description £º Initialize the dma transmit corresponding parameters.      
*********************************************************************/
void I2S_InitDmaRtxPara(DMAC_CFG *para , IIS_DIR pDir)
{
//	switch(pDir)
//	{
//		case IIS_OUT:
//			para->blocksize = 64; 
//			para->dstburst  = 2;  
//			para->srcburst  = 3;
//			para->dstaddr   = 0x3100a1c8;
//			para->srcaddr   = 0x41000034;
//			para->dstlength = read_reg(I2S_ITER)==0x3?2:1;
//			para->srclength = 3;
//			break;
//		case IIS_IN:
//			para->blocksize = 64; 
//			para->dstburst  = 2;  
//			para->srcburst  = 3;
//			para->dstaddr   = 0x4100002c;
//			para->srcaddr   = 0x3100a1c0;
//			para->dstlength = 2;
//			para->srclength = 2;
//			break;
//		case IIS_DUPLEX:
//			para->blocksize = 1024; 
//			para->dstburst  = 2;  
//			para->srcburst  = 2;
//			para->dstaddr   = 0x3100a1c8;
//			para->srcaddr   = 0x3100a1c0;
//			para->dstlength = read_reg(I2S_ITER)==0x3?2:1;
//			para->srclength = 1;			
//		default:break;
//	}
}
/********************************************************************
* Function    £º I2S_ConfigDmaInt
* Parameter   £º no para 
* Return      £º no return
* Description £º Unmask and Clear the dma interrput.      
*********************************************************************/
void I2S_ConfigDmaInt(void)
{
	/*unmask the dma inerrupt*/
    *((RP32)(DMAC2_MASKTRF+4  )) = 0;
    *((RP32)(DMAC2_MASKTRF    )) = 1<<8 | 1 ;
    *((RP32)(DMAC2_MASKBLK+4  )) = 0;
    *((RP32)(DMAC2_MASKBLK    )) = 1<<8 | 1 ;
    *((RP32)(DMAC2_MASKSRCTR+4)) = 0 ;
    *((RP32)(DMAC2_MASKSRCTR  )) = 0<<8 | 0 ;
    *((RP32)(DMAC2_MASKDSTTR+4)) = 0 ;
    *((RP32)(DMAC2_MASKDSTTR  )) = 0<<8 | 0 ;
    *((RP32)(DMAC2_MASKERR+4  )) = 0 ;
    *((RP32)(DMAC2_MASKERR    )) = 1<<8 | 1 ;
    /*clear any pending interrupts*/ 
    *((RP32)(DMAC2_CLRTFR+4)  ) |=  0 ;
    *((RP32)(DMAC2_CLRTFR)    ) |=  0x1 ; 
    *((RP32)(DMAC2_CLRBLK+4)  ) |=  0 ;
    *((RP32)(DMAC2_CLRBLK)    ) |=  0x1 ; 
    *((RP32)(DMAC2_CLRSRCTR+4)) |=  0 ;
    *((RP32)(DMAC2_CLRSRCTR)  ) |=  0x1 ; 
    *((RP32)(DMAC2_CLRDSTTR+4)) |=  0 ;
    *((RP32)(DMAC2_CLRDSTTR)  ) |=  0x1 ; 
    *((RP32)(DMAC2_CLRERR+4)  ) |=  0 ;
    *((RP32)(DMAC2_CLRERR)    ) |=  0x1 ; 
}
/********************************************************************
* Function    £º I2S_SetPort
* Parameter   £º no para
* Return      £º no return
* Description £º Configure the gpio of the i2s and 12c.     
*************************************************************/
void I2S_SetPort(void)
{
	write_reg(SYS_CTRL , read_reg(SYS_CTRL)|0x10000000);   	    
    write_reg(GPIO_PORTE_SEL , read_reg(GPIO_PORTE_SEL)&(~(0x3<<20)));    
    write_reg(GPIO_PORTC_SEL ,read_reg(GPIO_PORTC_SEL)&0xfffffff0); 
}
/********************************************************************
* Function    £º I2S_InitTx
* Parameter   £º 
* Return      £º 
* Description £º Initialize the transmit or recevie of i2s.      
*************************************************************/
void I2S_Init(IIS_DIR pRWDir, CLOCK_STATE pClkState, IIS_DATALEN pDataLen, U32 pSampleRate, WS_LEN pWslength , GC_LEN pSclkg)
{
    I2S_Enable(FALSE);
    I2S_MaskInt(IIS_CH0, RX_AVALIABLE_INT|RX_OVERRUN_INT|TX_EMPTY_INT|TX_OVERRUN_INT, FALSE);     
    I2S_FlushFifo(pRWDir);
    if(pRWDir == IIS_OUT)
    {
    	I2S_SetFifoLevel(IIS_CH0, pRWDir, 8);
    }
    else if(pRWDir == IIS_IN) 
    {
    	I2S_SetFifoLevel(IIS_CH0, pRWDir, 7); 
    }
    I2S_SetDataLen(IIS_CH0, pRWDir, pDataLen);  
    I2S_Enable(TRUE);    
    I2S_EnableBlock(pRWDir, TRUE);    
    I2S_EnableDisableChannels(TxALLDISABLE);   
    I2S_EnableChannel(IIS_CH0, pRWDir, TRUE);           
    I2S_EnableClock(FALSE);   
    if(pClkState == MASTER_CLK)
    {    	    	
    	I2S_SetMasterClock(HARDWARE, NOCARE); 
    	I2S_SetClock(pSampleRate, pWslength, pSclkg);
        I2S_EnableClock(TRUE);
    }
}
/***********************************************************
*	Function	£º	IIS_InitDMACPara
*	Parameter	£º	
*	Return		£º  
*	Description	£º	
***********************************************************/
void IIS_InitDMACPara(DMAC_CFG *pDmacCfg, RW_DIR pDir, DMAC_TRAN_TYPE pType, U32 pSrcAddr , U32 pDstAddr)
{  
    /*INT MASK Register*/
	pDmacCfg->MskTfr           = TRUE; //true´ú±í²»ÆÁ±Î
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
/********************************************************************
* Function    £º I2S_ConfigDmaTx
* Parameter   £º pSrcupdata , pDstupdata
* Return      £º no return
* Description £º Configure the dma when playback the audio for single 
*                block transmit      
*********************************************************************/
void I2S_ConfigDmaTx(U32 pSrcupdata , U32 pDstupdata)
{
//	I2S_ConfigDmaInt();  	   	                   
//    /*the register is used to enable DW_ahb_dmac*/
//    *((RP32)(DMAC2_CFG+4)  )  =  0x0 ;
//    *((RP32)(DMAC2_CFG)  )    =  DMAC_CFG_REG_EN(1)  ;                   
//    /*Write the starting source address in the SARx register for channel 0*/
//    *((RP32)(DMAC2_SAR0+4))   =  0 ; 
//    *((RP32)(DMAC2_SAR0)  )   =  pSrcupdata;                                                                                                                    
//    /*Write the starting destination address in the SARx register for channel 0*/
//    *((RP32)(DMAC2_DAR0+4))   =  0 ; 
//    *((RP32)(DMAC2_DAR0)  )   =  pDstupdata;        
//    /*Programe CTLx and CFGx*/ 
//    *((RP32)(DMAC2_CTL0+4))   =  DMAC_DONE          (0) | DMAC_BLK_TRAN_SZ(gDmacPara.blocksize);  
//    *((RP32)(DMAC2_CTL0)  )   =  DMAC_SRC_LLP_EN    (0) | DMAC_DST_LLP_EN        (0) | DMAC_SRC_MASTER_SEL   (0) \
//     	                       | DMAC_DST_MASTER_SEL(1) | DMAC_TRAN_TYPE_FLOW_CTL(1) | DMAC_DST_SCAT_EN      (0) \
//     	                       | DMAC_SRC_GATH_EN   (0) | DMAC_SRC_BST_SZ (gDmacPara.srcburst) | DMAC_DST_BST_SZ(gDmacPara.dstburst) \
//     	                       | DMAC_SRC_INCR      (0) | DMAC_DST_INCR          (3) | DMAC_SRC_TRAN_WIDTH (gDmacPara.srclength) \
//                               | DMAC_DST_TRAN_WIDTH(gDmacPara.dstlength) | DMAC_INT_EN(1);
//         
//    /*Channel 0 Configuration Register*/
//    *((RP32)(DMAC2_CFG0+4))   =  DMAC_DST_PER        (7) | DMAC_SRC_PER  (6) | DMAC_SRC_STAT_UPD_EN(0) \
//     	                       | DMAC_DST_STAT_UPD_EN(0) | DMAC_PROT_CTL (1) | DMAC_FIFO_MODE      (0) \
//     	                       | DMAC_FLOW_CTL_MODE  (0); 
//    *((RP32)(DMAC2_CFG0)  )   =  DMAC_AUTO_RELOAD_DST(0) | DMAC_AUTO_RELOAD_SRC(0) | DMAC_MAX_AMBA_BST_LEN(0) \
//     	                       | DMAC_SRC_HS_POL     (0) | DMAC_DST_HS_POL     (0) | DMAC_BUS_LOCK        (0) \
//     	                       | DMAC_CH_LOCK        (0) | DMAC_BUS_LOCK_LEVEL (1) | DMAC_HW_SW_SEL_SRC   (1) \
//     	                       | DMAC_HW_SW_SEL_DST  (0) | DMAC_CH_SUSP        (0) | DMAC_CH_PRIOR        (0);                 
//    *((RP32)(DMAC2_CHENA+4)) =   0 ; 
//    *((RP32)(DMAC2_CHENA  )) =   DMAC_CH_WRITE_EN(1) | DMAC_CH_EN(1);         
}
/********************************************************************
* Function    £º I2S_ConfigDmaFDX
* Parameter   £º srcupdata , dstupdata
* Return      £º no return
* Description £º Configure the dma when record the audio.
*************************************************************/
void I2S_ConfigDmaRx(U32 srcupdata , U32 dstupdata)		                                
{
//	I2S_ConfigDmaInt();   	   	                   
//    *((RP32)(DMAC2_CFG+4)  )    =  0x0 ;
//    *((RP32)(DMAC2_CFG)  )      =  DMAC_CFG_REG_EN(1)  ;                   
//    *((RP32)(DMAC2_SAR0+4))   =   0 ; 
//    *((RP32)(DMAC2_SAR0)  )   =   srcupdata;                                                                                                                
//    *((RP32)(DMAC2_DAR0+4))   =   0 ; 
//    *((RP32)(DMAC2_DAR0)  )   =   dstupdata;
//    *((RP32)(DMAC2_CTL0+4))   =   DMAC_DONE          (0) | DMAC_BLK_TRAN_SZ(gDmacPara.blocksize);  
//    *((RP32)(DMAC2_CTL0)  )   =   DMAC_SRC_LLP_EN    (0) | DMAC_DST_LLP_EN        (0) | DMAC_SRC_MASTER_SEL   (1) \
//     	                      | DMAC_DST_MASTER_SEL(0) | DMAC_TRAN_TYPE_FLOW_CTL(2) | DMAC_DST_SCAT_EN      (0) \
//     	                      | DMAC_SRC_GATH_EN   (0) | DMAC_SRC_BST_SZ (gDmacPara.srcburst) | DMAC_DST_BST_SZ(gDmacPara.dstburst) \
//     	                      | DMAC_SRC_INCR      (3) | DMAC_DST_INCR          (0) | DMAC_SRC_TRAN_WIDTH (gDmacPara.srclength) \
//                              | DMAC_DST_TRAN_WIDTH(gDmacPara.dstlength) | DMAC_INT_EN(1);
//    *((RP32)(DMAC2_CFG0+4))  =   DMAC_DST_PER        (7) | DMAC_SRC_PER  (6) | DMAC_SRC_STAT_UPD_EN(0) \
//     	                     | DMAC_DST_STAT_UPD_EN(0) | DMAC_PROT_CTL (1) | DMAC_FIFO_MODE      (0) \
//     	                     | DMAC_FLOW_CTL_MODE  (0); 
//    *((RP32)(DMAC2_CFG0)  )  =   DMAC_AUTO_RELOAD_DST(0) | DMAC_AUTO_RELOAD_SRC(0) | DMAC_MAX_AMBA_BST_LEN(0) \
//     	                     | DMAC_SRC_HS_POL     (0) | DMAC_DST_HS_POL     (0) | DMAC_BUS_LOCK        (0) \
//     	                     | DMAC_CH_LOCK        (0) | DMAC_BUS_LOCK_LEVEL (1) | DMAC_HW_SW_SEL_SRC   (0) \
//     	                     | DMAC_HW_SW_SEL_DST  (1) | DMAC_CH_SUSP        (0) | DMAC_CH_PRIOR        (0);                
//    *((RP32)(DMAC2_CHENA+4)) =   0 ; 
//    *((RP32)(DMAC2_CHENA  )) =   DMAC_CH_WRITE_EN(1) | DMAC_CH_EN(1);          
}
/********************************************************************
* Function    £º I2S_CreatListChain
* Parameter   £º phead
* Return      £º phead
* Description £º The struct of linked_list DMA transmit.  
*************************************************************/
volatile LLI *I2S_CreatListChain(volatile LLI *phead)
{   
//	U32  i , uPnew_temp;
//	LLI *pnew , *pend;	
//	for (i=0 ; i<300 ; i++)
//	{    
//		pnew = (LLI *)malloc(sizeof(LLI)); 
//        pnew->SrcAddr = gDmacPara.srcaddr;	 
//	    pnew->DstAddr = gDmacPara.dstaddr;	      
//	    pnew-> CtlLow32  = DMAC_SRC_LLP_EN(1) | DMAC_DST_LLP_EN(1) | DMAC_SRC_MASTER_SEL(0) 
//	                      | DMAC_DST_MASTER_SEL(1)  | DMAC_TRAN_TYPE_FLOW_CTL(1) | DMAC_DST_SCAT_EN(0) \
//	                      | DMAC_SRC_GATH_EN   (0) | DMAC_SRC_BST_SZ(gDmacPara.srcburst) | DMAC_DST_BST_SZ (gDmacPara.dstburst) \
//	                      | DMAC_SRC_INCR(0) | DMAC_DST_INCR(3) | DMAC_SRC_TRAN_WIDTH(gDmacPara.srclength) \
//	                      | DMAC_DST_TRAN_WIDTH(gDmacPara.dstlength) | DMAC_INT_EN(0);         
//        pnew-> CtlHigh32 = DMAC_DONE (0) | DMAC_BLK_TRAN_SZ(gDmacPara.blocksize);
//        gDmacPara.srcaddr = gDmacPara.srcaddr + gDmacPara.blocksize*4;
//        gDmacPara.dstaddr = gDmacPara.dstaddr;	    	    
//	    if (phead == NULL)
//	    {
//		    phead = pnew;
//	    }
//	    else if ((U32)pnew > 0x40000000 && (U32)pnew <0x50000000)
//		{
//	    	uPnew_temp = (U32)pnew  + 0x18;
//	    	pend->Next = (LLI*)(uPnew_temp);
//		}
//	    else if ((U32)pnew > 0x32000000 && (U32)pnew <0x32018000)
//	    {
//	    	uPnew_temp = (U32)pnew  | 0x1;
//	    	pend->Next = (LLI*)(uPnew_temp);
//	    }	
//	    pend = pnew;  	
//	}	
//	pend->Next = NULL;
//	return phead;
}
/********************************************************************
* Function    £º I2S_ConfigDmaLinkedlist
* Parameter   £º pChannel_num
* Return      £º no return
* Description £º Configure the dma when playback the audio 
*                for multibolock transmit.      
*********************************************************************/
void I2S_ConfigDmaLinkedlist(U32 pChannel_num)
{  
//	U32 uTemp;
//    volatile LLI *head = NULL;
//    volatile LLI *head_temp;       
//    I2S_ConfigDmaInt();        
//    *((RP32)(DMAC2_CFG+4      )) =  0x0 ; 
//    *((RP32)(DMAC2_CFG        )) =  DMAC_CFG_REG_EN(1);    
//    head_temp = I2S_CreatListChain(head);    
//    if ((U32)head_temp >0x40000000 &&  (U32)head_temp < 0x50000000)
//    {
//        *((RP32)(DMAC_LLP(2,pChannel_num)+4))    = 0x0;	
//        *((RP32)(DMAC_LLP(2,pChannel_num)  ))    = (U32)head_temp | 0x0; 
//    }    
//    else 
//    {
//    	*((RP32)(DMAC_LLP(2,pChannel_num)+4))    = 0x0;	
//    	*((RP32)(DMAC_LLP(2,pChannel_num)  ))    = (U32)head_temp | 0x1; 
//    }
//    *((RP32)(DMAC2_DAR0+4))   =   0 ; 
//    *((RP32)(DMAC2_DAR0)  )   =   0x3100a1c8;    
//     /*Programe CTLx and CFGx */
//    *((RP32)(DMAC_CTL(2,pChannel_num)+4))   = DMAC_DONE  (0)        | DMAC_BLK_TRAN_SZ (gDmacPara.blocksize);
//    *((RP32)(DMAC_CTL(2,pChannel_num)  ))   = DMAC_SRC_LLP_EN   (1) | DMAC_DST_LLP_EN        (0) |DMAC_SRC_MASTER_SEL(0) 
//                                             |DMAC_DST_MASTER_SEL(1) | DMAC_TRAN_TYPE_FLOW_CTL(1)\
//                                             |DMAC_DST_SCAT_EN  (0) | DMAC_SRC_GATH_EN       (0) | DMAC_SRC_BST_SZ (gDmacPara.srcburst)\
//                                             |DMAC_DST_BST_SZ   (gDmacPara.dstburst) | DMAC_SRC_INCR (0) | DMAC_DST_INCR(3)\
//                                             |DMAC_SRC_TRAN_WIDTH  (gDmacPara.srclength) | DMAC_DST_TRAN_WIDTH(gDmacPara.srclength) | DMAC_INT_EN(0);        			    			
//    /*Channel 0 Configuration Register*/
//    *((RP32)(DMAC_CH_CFG(2,pChannel_num)+4))= DMAC_DST_PER        (7)|DMAC_SRC_PER (6)|DMAC_SRC_STAT_UPD_EN(0) \
//        	                                 |DMAC_DST_STAT_UPD_EN(0)|DMAC_PROT_CTL(1)|DMAC_FIFO_MODE      (0) \
//        	                                 |DMAC_FLOW_CTL_MODE  (0); 
//    *((RP32)(DMAC_CH_CFG(2,pChannel_num)  ))= DMAC_AUTO_RELOAD_DST(1U)|DMAC_AUTO_RELOAD_SRC(0)|DMAC_MAX_AMBA_BST_LEN(0) \
//        	                                 |DMAC_SRC_HS_POL     (0)|DMAC_DST_HS_POL     (0)|DMAC_BUS_LOCK        (0) \
//        	                                 |DMAC_CH_LOCK        (0)|DMAC_BUS_LOCK_LEVEL (1)|DMAC_HW_SW_SEL_SRC   (1) \
//        	                                 |DMAC_HW_SW_SEL_DST  (0)|DMAC_CH_SUSP        (0)|DMAC_CH_PRIOR        (0);
//    *((RP32)(DMAC2_CHENA+4)) = 0; 
//    *((RP32)(DMAC2_CHENA))   = 0x101;   
//	uTemp = (*(RP32)DMAC2_RAWTFR>> pChannel_num) & 0x1;
//	write_reg(0x40100000, (*(RP32)DMAC2_RAWTFR >> pChannel_num) & 0x1);
//    while ( uTemp != 0x1 )
//    uTemp = (*(RP32)DMAC2_RAWTFR >> pChannel_num) & 0x1;
}
/********************************************************************
* Function    £º I2S_ConfigDmaAutoreload
* Parameter   £º pChannel_num
* Return      £º no return
* Description £º Configure the dma when playback the audio 
*                for multibolock transmit.     
*********************************************************************/
void I2S_ConfigDmaAutoreload(U32 pChannel_num)
{
//	I2S_ConfigDmaInt(); 
//    *((RP32)(DMAC2_CFG+4      )) =  0x0 ;
//    *((RP32)(DMAC2_CFG        )) =  DMAC_CFG_REG_EN(1);    
//    *((RP32)(DMAC_LLP(2,pChannel_num)+4)) = 0x0;	
//    *((RP32)(DMAC_LLP(2,pChannel_num)  )) = 0x0; 
//    *((RP32)(DMAC2_SAR0+4)) = 0 ; 
//    *((RP32)(DMAC2_SAR0)  ) = 0x4200002c;                                                                                                                    
//    *((RP32)(DMAC2_DAR0+4)) = 0 ; 
//    *((RP32)(DMAC2_DAR0)  ) = 0x3100a1c8;    
//    /* Programe CTLx and CFGx */
//   *((RP32)(DMAC_CTL(2,pChannel_num)+4)) = DMAC_DONE  (0)        | DMAC_BLK_TRAN_SZ (gDmacPara.blocksize);
//   *((RP32)(DMAC_CTL(2,pChannel_num)  )) = DMAC_SRC_LLP_EN   (0) | DMAC_DST_LLP_EN        (0) |DMAC_SRC_MASTER_SEL(0) 
//                                          |DMAC_DST_MASTER_SEL(1)| DMAC_TRAN_TYPE_FLOW_CTL(1)\
//                                          |DMAC_DST_SCAT_EN  (0) | DMAC_SRC_GATH_EN       (0) | DMAC_SRC_BST_SZ (gDmacPara.srcburst)\
//                                          |DMAC_DST_BST_SZ   (gDmacPara.dstburst) | DMAC_SRC_INCR (0) | DMAC_DST_INCR(3)\
//                                          |DMAC_SRC_TRAN_WIDTH  (gDmacPara.srclength) | DMAC_DST_TRAN_WIDTH(gDmacPara.srclength) | DMAC_INT_EN(1);       			    			
//   /*Channel 0 Configuration Register*/
//   *((RP32)(DMAC_CH_CFG(2,pChannel_num)+4)) = DMAC_DST_PER    (7)|DMAC_SRC_PER (6)|DMAC_SRC_STAT_UPD_EN(0) \
//       	                                     |DMAC_DST_STAT_UPD_EN(0)|DMAC_PROT_CTL(1)|DMAC_FIFO_MODE      (0) \
//       	                                     |DMAC_FLOW_CTL_MODE  (0); 
//   *((RP32)(DMAC_CH_CFG(2,pChannel_num)  )) =(DMAC_AUTO_RELOAD_DST(1U)|DMAC_AUTO_RELOAD_SRC(0)|DMAC_MAX_AMBA_BST_LEN(0) 
//       	                                     |DMAC_SRC_HS_POL     (0)|DMAC_DST_HS_POL     (0)|DMAC_BUS_LOCK        (0) \
//       	                                     |DMAC_CH_LOCK        (0)|DMAC_BUS_LOCK_LEVEL (1)|DMAC_HW_SW_SEL_SRC   (1) \
//       	                                     |DMAC_HW_SW_SEL_DST  (0)|DMAC_CH_SUSP        (0)|DMAC_CH_PRIOR        (0));   
//   *((RP32)(DMAC2_CHENA+4)) =   0 ; 
//   *((RP32)(DMAC2_CHENA  )) =   DMAC_CH_WRITE_EN(1) | DMAC_CH_EN(1);    
}
/********************************************************************
* Function    £º I2S_ConfigDmaFDX
* Parameter   £º pChannel_num
* Return      £º no return
* Description £º Configure the dma when playback the audio 
*               for multibolock transmit      
********************************************************************/
void I2S_ConfigDmaFDX(U32 pChannel_num)
{
//	I2S_ConfigDmaInt(); 
//    *((RP32)(DMAC2_CFG+4      )) =  0x0 ;
//    *((RP32)(DMAC2_CFG        )) =  DMAC_CFG_REG_EN(1);
//    *((RP32)(DMAC2_SAR0+4))   =   0 ; 
//    *((RP32)(DMAC2_SAR0)  )   =   0x3100a1c0;                                                                                        
//    *((RP32)(DMAC2_DAR0+4))   =   0 ; 
//    *((RP32)(DMAC2_DAR0)  )   =   0x3100a1c8;    
//    *((RP32)(DMAC_CTL(2,pChannel_num)+4))   = DMAC_DONE  (0)        | DMAC_BLK_TRAN_SZ (gDmacPara.blocksize);
//    *((RP32)(DMAC_CTL(2,pChannel_num)  ))   = DMAC_SRC_LLP_EN   (0) | DMAC_DST_LLP_EN        (0) |DMAC_SRC_MASTER_SEL(1) 
//                                         |DMAC_DST_MASTER_SEL(1) | DMAC_TRAN_TYPE_FLOW_CTL(3)\
//                                         |DMAC_DST_SCAT_EN  (0) | DMAC_SRC_GATH_EN       (0) | DMAC_SRC_BST_SZ (gDmacPara.srcburst)\
//                                         |DMAC_DST_BST_SZ   (gDmacPara.dstburst) | DMAC_SRC_INCR (3) | DMAC_DST_INCR(3)\
//                                         |DMAC_SRC_TRAN_WIDTH  (gDmacPara.srclength) | DMAC_DST_TRAN_WIDTH(gDmacPara.dstlength) | DMAC_INT_EN(1);
//    *((RP32)(DMAC_CH_CFG(2,pChannel_num)+4))  = DMAC_DST_PER        (7)|DMAC_SRC_PER (6)|DMAC_SRC_STAT_UPD_EN(0) \
//       	                             |DMAC_DST_STAT_UPD_EN(0)|DMAC_PROT_CTL(1)|DMAC_FIFO_MODE      (0) \
//       	                             |DMAC_FLOW_CTL_MODE  (0); 
//    *((RP32)(DMAC_CH_CFG(2,pChannel_num)  ))  =(DMAC_AUTO_RELOAD_DST(0)|DMAC_AUTO_RELOAD_SRC(0)|DMAC_MAX_AMBA_BST_LEN(0) 
//       	                             |DMAC_SRC_HS_POL     (0)|DMAC_DST_HS_POL     (0)|DMAC_BUS_LOCK        (0) \
//       	                             |DMAC_CH_LOCK        (0)|DMAC_BUS_LOCK_LEVEL (1)|DMAC_HW_SW_SEL_SRC   (0) \
//       	                             |DMAC_HW_SW_SEL_DST  (0)|DMAC_CH_SUSP        (0)|DMAC_CH_PRIOR        (0));   
//    *((RP32)(DMAC2_CHENA+4)) =   0 ; 
//    *((RP32)(DMAC2_CHENA  )) =  0x101;   
}
	 
/********************************************************************
* Function    £º I2S_VerifyReg
* Parameter   £º no para
* Return      £º no return
* Description £º Verify the initialize value of registers in i2s.
*********************************************************************/
void I2S_VerifyReg(void) 
{
	U32 i;
	/*verify the public usage registers of the four channels*/ 
    if(read_reg(I2S_IER ) != 0x0)       
    	printf("The wrong initialize value of I2S_IER is 0x%x!\n", *(U32*)(I2S_IER ));
    if(read_reg(I2S_IRER) != 0x0)       
    	printf("The wrong initialize value of I2S_IRER is 0x%x!\n",*(U32*)(I2S_IRER));
    if(read_reg(I2S_ITER) != 0x0)       
    	printf("The wrong initialize value of I2S_ITER is 0x%x!\n",*(U32*)(I2S_ITER));
    if(read_reg(I2S_CER ) != 0x01)      
    	printf("The wrong initialize value of I2S_CER is 0x%x!\n", *(U32*)(I2S_CER ));
    if(read_reg(I2S_CCR ) != 0xac4414)  
    	printf("The wrong initialize value of I2S_CCR is 0x%x!\n", *(U32*)(I2S_CCR ));   
    /*verify the registers about each of the four channels*/
    for(i=0 ; i<4 ;i++)
    {
    	if(*(U32*)(I2S_LRBR(i)) != 0x0 )  
    		printf("The wrong initialize value of I2S_LRBR(%d) is 0x%x!\n",i, *(U32*)(I2S_LRBR(i)));
    	if(*(U32*)(I2S_LTHR(i)) != 0x0 )  
    		printf("The wrong initialize value of I2S_LTHR(%d) is 0x%x!\n",i, *(U32*)(I2S_LTHR(i)));
    	if(*(U32*)(I2S_RRBR(i)) != 0x0 )  
    		printf("The wrong initialize value of I2S_RRBR(%d) is 0x%x!\n",i, *(U32*)(I2S_RRBR(i)));
    	if(*(U32*)(I2S_RTHR(i)) != 0x0 )  
    		printf("The wrong initialize value of I2S_RTHR(%d) is 0x%x!\n",i, *(U32*)(I2S_RTHR(i)));
    	if(*(U32*)(I2S_RER(i))  != 0x01)  
    		printf("The wrong initialize value of I2S_RER(%d)  is 0x%x!\n",i, *(U32*)(I2S_RER(i) ));
    	if(*(U32*)(I2S_TER(i))  != 0x01)  
    		printf("The wrong initialize value of I2S_TER(%d)  is 0x%x!\n",i, *(U32*)(I2S_TER(i) ));
    	if(*(U32*)(I2S_RCR(i))  != 0x04)  
    		printf("The wrong initialize value of I2S_RCR(%d)  is 0x%x!\n",i, *(U32*)(I2S_RCR(i) ));
    	if(*(U32*)(I2S_TCR(i))  != 0x05)  
    		printf("The wrong initialize value of I2S_TCR(%d)  is 0x%x!\n",i, *(U32*)(I2S_TCR(i) ));
    	if(*(U32*)(I2S_ISR(i))  != 0x10)  
    		printf("The wrong initialize value of I2S_ISR(%d)  is 0x%x!\n",i, *(U32*)(I2S_ISR(i) ));
    	if(*(U32*)(I2S_IMR(i))  != 0x33)  
    		printf("The wrong initialize value of I2S_IMR(%d)  is 0x%x!\n",i, *(U32*)(I2S_IMR(i) ));
    	if(*(U32*)(I2S_ROR(i))  != 0x0 )  
    		printf("The wrong initialize value of I2S_ROR(%d)  is 0x%x!\n",i, *(U32*)(I2S_ROR(i) ));
    	if(*(U32*)(I2S_TOR(i))  != 0x0 )  
    		printf("The wrong initialize value of I2S_TOR(%d)  is 0x%x!\n",i, *(U32*)(I2S_TOR(i) ));
    	if(*(U32*)(I2S_RFCR(i)) != 0x03)  
    		printf("The wrong initialize value of I2S_RFCR(%d) is 0x%x!\n",i, *(U32*)(I2S_RFCR(i)));
    	if(*(U32*)(I2S_TFCR(i)) != 0x03)  
    		printf("The wrong initialize value of I2S_TFCR(%d) is 0x%x!\n",i, *(U32*)(I2S_TFCR(i)));
    	if(*(U32*)(I2S_RFF(i))  != 0x0 )  
    		printf("The wrong initialize value of I2S_RFF(%d)  is 0x%x!\n",i, *(U32*)(I2S_RFF(i) ));
    	if(*(U32*)(I2S_TFF(i))  != 0x0 )  
    		printf("The wrong initialize value of I2S_TFF(%d)  is 0x%x!\n",i, *(U32*)(I2S_TFF(i) ));
    }   
}
/********************************************************************
* Function    £º I2S_ReadandWriteReg
* Parameter   £º no para
* Return      £º no return
* Description £º Verify the i2s registers value after writing.
*********************************************************************/
void I2S_ReadandWriteReg(void)
{
	U32 i;
	/*verify the public usage registers of the four channels*/
	write_reg(I2S_IER , 0xffffffff);
    if(read_reg(I2S_IER )  != 0x1)       
    	printf("The wrong  value of I2S_IER in writeread verification is 0x%x!\n", *(U32*)(I2S_IER ));
    write_reg(I2S_IRER ,0xffffffff);
    if(read_reg(I2S_IRER ) != 0x1)       
    	printf("The wrong  value of I2S_IRER in writeread verification is 0x%x!\n",*(U32*)(I2S_IRER));
    write_reg(I2S_ITER ,0xffffffff);
    if(read_reg(I2S_ITER ) != 0x1)       
    	printf("The wrong  value of I2S_ITER in writeread verification is 0x%x!\n",*(U32*)(I2S_ITER));
            
    write_reg(I2S_CER ,0xffffffff);
    if(read_reg(I2S_CER )  != 0x1)       
    	printf("The wrong  value of I2S_CER in writeread verification is 0x%x!\n", *(U32*)(I2S_CER ));
    
    I2S_EnableClock(FALSE);;
    write_reg(I2S_CCR ,0xffffffff);
    if(read_reg(I2S_CCR )  != 0x3FFFFFF) 
    	printf("The wrong  value of I2S_CCR in writeread verification is 0x%x!\n", *(U32*)(I2S_CCR ));
    
    /*verify the registers about each of the four channels*/
    for(i=0 ; i<4 ; i++)
    {
    	*(U32*)(I2S_RER(i)) = 0xffffffff;
    	if(*(U32*)(I2S_RER(i))  != 0x1)  
    		printf("The wrong  value of I2S_RER(%d) in writeread verification is 0x%x!\n", i, *(U32*)(I2S_RER(i)));
    	*(U32*)(I2S_RER(i)) = 0xffffffff;
    	if(*(U32*)(I2S_TER(i))  != 0x1)  
    		printf("The wrong  value of I2S_TER(%d) in writeread verification is 0x%x!\n", i,  *(U32*)(I2S_TER(i)));
    	*(U32*)(I2S_RER(i)) = 0xffffffff;
    	if(*(U32*)(I2S_RCR(i))  != 0x04) 
    		printf("The wrong  value of I2S_RCR(%d) in writeread verification is 0x%x!\n", i,  *(U32*)(I2S_RCR(i)));
    	*(U32*)(I2S_RER(i)) = 0xffffffff;
    	if(*(U32*)(I2S_TCR(i))  != 0x05) 
    		printf("The wrong  value of I2S_TCR(%d) in writeread verification is 0x%x!\n", i,  *(U32*)(I2S_TCR(i)));
    	*(U32*)(I2S_IMR(i)) = 0xffffffff;
    	if(*(U32*)(I2S_IMR(i))  != 0x33) 
    		printf("The wrong  value of I2S_IMR(%d) in writeread verification is 0x%x!\n", i,  *(U32*)(I2S_IMR(i)));
    	
    	I2S_EnableChannel(IIS_CH0, IIS_IN, TRUE);
    	*(U32*)(I2S_RFCR(i))= 0xffffffff;
    	if(*(U32*)(I2S_RFCR(i)) != 0x0f) 
    		printf("The wrong  value of I2S_RFCR(%d) in writeread verification is 0x%x!\n", i, *(U32*)(I2S_RFCR(i)));
    	I2S_EnableChannel(IIS_CH0, IIS_IN, FALSE);
    	
    	I2S_EnableChannel(IIS_CH0, IIS_OUT, TRUE);
    	*(U32*)(I2S_TFCR(i))= 0xffffffff;
    	if(*(U32*)(I2S_TFCR(i)) != 0x0f) 
    		printf("The wrong  value of I2S_TFCR(%d) in writeread verification is 0x%x!\n", i, *(U32*)(I2S_TFCR(i)));
    	I2S_EnableChannel(IIS_CH0, IIS_OUT, FALSE);
    }
}
/********************************************************************
* Function    £º I2S_CpuPlayback
* Parameter   £º no para
* Return      £º no return
* Description £º i2s playback in cpu transmit mode     
*********************************************************************/
void I2S_CpuPlayback(void)
{
    U32 i;  
	I2S_SetPort();    
	I2S_GetAudioInfo(&gWaveAudio , (RP32)(0x41000000));  
	I2S_Wm8976Playback(0x000);   
	I2S_Init(IIS_OUT, MASTER_CLK, gWaveAudio.datalength>>16, gWaveAudio.samplerate, WS_SCLK16, GC_SCLK0);     
    if((read_reg(I2S_ISR0) & 0x10) != 0 )    
     {    	 
    	if((gWaveAudio.datalength>>16) == 16)   /*decode  16bits wave audio*/ 
    	{    
    		for(i=0 ; i<gWaveAudio.datasize ; i=i+4)
    		{
    	        write_reg(I2S_LTHR0 , *gWaveAudio.audioaddr);          
                write_reg(I2S_RTHR0 , *gWaveAudio.audioaddr>>16); 
                gWaveAudio.audioaddr = gWaveAudio.audioaddr + 1;
    		}                
    	}    	
    	if((gWaveAudio.datalength>>16) == 24)   /*decode  24bits wave audio*/
    	{          
            for(i=0 ; i<gWaveAudio.datasize; i=i+12)
               { 
             	    write_reg(I2S_LTHR0, *gWaveAudio.audioaddr);             	   
                    write_reg(I2S_RTHR0,(*gWaveAudio.audioaddr++>>24)|(*gWaveAudio.audioaddr<<8));                    
                    write_reg(I2S_LTHR0,(*gWaveAudio.audioaddr++>>16)|(*gWaveAudio.audioaddr<<16));                  
                    write_reg(I2S_RTHR0,(*gWaveAudio.audioaddr++>>8));
               }
    	 }    	
    	 if((gWaveAudio.datalength>>16) == 32)   /*decode  32bits wave audio*/
    	 {
            for(i=0 ; i<=gWaveAudio.datasize; i=i+8)  
             {
                   write_reg(I2S_LTHR0 , *gWaveAudio.audioaddr);
                   gWaveAudio.audioaddr = gWaveAudio.audioaddr + 1;                   
                   write_reg(I2S_RTHR0 , *gWaveAudio.audioaddr);
                   gWaveAudio.audioaddr = gWaveAudio.audioaddr + 1;  
             } 
    	 }    	
     }            
    if((read_reg(I2S_ISR0) & 0x20) != 0 )
     {      	  
         read_reg(I2S_TOR0);	          
         DebugPrintf("The transmitter overrun interrupt occurs!\n");
     }  
}
/********************************************************************
* Function    £º I2S_CpuRecord
* Parameter   £º no para
* Return      £º no return
* Description £º i2s record in cpu receive mode     
*********************************************************************/
void I2S_CpuRecord(void)
{
    U32 i;
    I2S_SetPort(); 
    I2S_Wm8976record(0x010 , 0x001);  
    I2S_Init(IIS_IN, MASTER_CLK, BIT16, 0xbb80, WS_SCLK16, GC_SCLK0);       		                               
/*Please set a breakpoint hereby!*/
    for(i=0 ; i<100; i++);   
    if((read_reg(I2S_ISR0) & 0x01) != 0 )
      {                 
          for(i=0 ; i<800000 ; i++)  
          {        	  
              *gWaveAudio.audioaddr = read_reg(I2S_LRBR0);          
              gWaveAudio.audioaddr = gWaveAudio.audioaddr + 1;          
              *gWaveAudio.audioaddr = read_reg(I2S_RRBR0);          
              gWaveAudio.audioaddr = gWaveAudio.audioaddr + 1;
          }                 
     }    
    if((read_reg(I2S_ISR0) & 0x02) != 0 )
      {     		      		  
          read_reg(I2S_ROR0);	     		
          DebugPrintf("The receiver overrun interupt occurs!\n");
      }       
}
/********************************************************************
* Function    £º I2S_CpuFDX
* Parameter   £º no para
* Return      £º no return
* Description £º CPU full duplex      
*********************************************************************/
void I2S_CpuFDX(void)
{
	I2S_SetPort(); 
	I2S_GetAudioInfo(&gWaveAudio , (RP32)(0x41000000));               
	I2S_Wm8976RecordPlaybak(0x010 , 0x001);
	I2S_Init(IIS_IN, MASTER_CLK, BIT16, 0xac44, WS_SCLK16, GC_SCLK0);  
	I2S_Init(IIS_OUT, MASTER_CLK, gWaveAudio.datalength>>16, gWaveAudio.samplerate, WS_SCLK16, GC_SCLK0);
    while(1)
    {
        *(RP32)(I2S_LTHR0) = *(RP32)(I2S_LRBR0);        
        *(RP32)(I2S_RTHR0) = *(RP32)(I2S_RRBR0);
    }    
}
/********************************************************************
* Function    £º I2S_DmaPlayback
* Parameter   £º no para
* Return      £º no return
* Description £º Playback the wave audio in dma transmit mode.     
*********************************************************************/
void I2S_DmaPlayback(void)
{
	I2S_SetPort();    
	I2S_GetAudioInfo(&gWaveAudio , (RP32)(0x41000000));  
	I2S_Wm8976Playback(0x000);  
	I2S_Init(IIS_OUT, MASTER_CLK, gWaveAudio.datalength>>16, gWaveAudio.samplerate, WS_SCLK16, GC_SCLK0);  
    I2S_SetFastMode();      
    //I2S_InitDmaRtxPara(gDmacCfg , IIS_OUT);    
    VIC_EnableInt(INTSRC_DMAC2);      
    //I2S_ConfigDmaTx(gDmacPara.srcaddr , gDmacPara.dstaddr);    
    //gSrcupdata = gDmacPara.srcaddr;
    while(gWaveAudio.datasize > 0)
    {    	
	    while(flag[2]==1)
	    {
		    gWaveAudio.datasize = gWaveAudio.datasize - 512;  	        
	        flag[2] = 0;	       
	        gSrcupdata = gSrcupdata + 512;                	       
	        I2S_ConfigDmaTx(gSrcupdata , gDstupdata);                	                               
	    }            
    }         
}
/********************************************************************
* Function    £º I2S_DmaLinkedlistPlayback
* Parameter   £º no para
* Return      £º no return
* Description £º Playback the wave audio in dma transmit mode     
*********************************************************************/
void I2S_DmaLinkedlistPlayback(void)
{
	I2S_SetPort();     
    I2S_GetAudioInfo(&gWaveAudio , (RP32)(0x41000000));
    I2S_Wm8976Playback(0x000);      
    I2S_Init(IIS_OUT, MASTER_CLK, gWaveAudio.datalength>>16, gWaveAudio.samplerate, WS_SCLK16, GC_SCLK0);
    I2S_SetFastMode();          
    //I2S_InitDmaRtxPara(&gDmacCfg , IIS_OUT);    
    VIC_EnableInt(INTSRC_DMAC2);
    I2S_ConfigDmaLinkedlist(IIS_CH0);  
}
/********************************************************************
* Function    £º I2S_DmaLinkedlistPlayback
* Parameter   £º no para
* Return      £º no return
* Description £º Playback the wave audio in dma transmit mode     
********************************************************************/
void I2S_DmaAutoreloadPlayback(void)
{
	I2S_SetPort();     
    I2S_GetAudioInfo(&gWaveAudio , (RP32)(0x41000000));
    I2S_Wm8976Playback(0x000);      
    I2S_Init(IIS_OUT, MASTER_CLK, gWaveAudio.datalength>>16, gWaveAudio.samplerate, WS_SCLK16, GC_SCLK0);
    I2S_SetFastMode();          
    //I2S_InitDmaRtxPara(&gDmacCfg , IIS_OUT);    
    VIC_EnableInt(INTSRC_DMAC2);    
    I2S_ConfigDmaAutoreload(IIS_CH0);
}
/********************************************************************
* Function    £º I2S_DmaRecord
* Parameter   £º no para
* Return      £º no return
* Description £º Record the wave audio in dma receive mode     
*********************************************************************/
void I2S_DmaRecord(void)
{    
    S32 recordlength  = 8000000;   /*in BYTE , you can change it if you want*/    
    I2S_SetPort(); 
    I2S_Wm8976record(0x010 , 0x001); 
    I2S_Init(IIS_IN, MASTER_CLK, BIT16, 0xbb80, WS_SCLK16, GC_SCLK0);    
    write_reg(0x41000028 , recordlength);   
    //I2S_InitDmaRtxPara(&gDmacPara , IIS_IN);    
    //gSrcupdata = gDmacPara.srcaddr;   
    //gDstupdata = gDmacPara.dstaddr;    
    VIC_EnableInt(INTSRC_DMAC2);     
    I2S_ConfigDmaRx(gSrcupdata , gDstupdata);		                                     
    while(recordlength > 0)
    {
        while(flag[2] == 1)
        { 
    	    recordlength = recordlength - 128;    	
    	    flag[2] = 0;   	    
    	    gDstupdata = gDstupdata + 128;    
    	    I2S_ConfigDmaRx(gSrcupdata , gDstupdata);   	
         }
    }
}

/********************************************************************
* Function    £º I2S_DmaFDX
* Parameter   £º no para
* Return      £º no return
* Description £º DMA full duplex      
*********************************************************************/
void I2S_DmaFDX(void)
{
	I2S_SetPort(); 
	I2S_GetAudioInfo(&gWaveAudio , (RP32)(0x41000000));               
	I2S_Wm8976RecordPlaybak(0x010 , 0x001);
	I2S_Init(IIS_IN, MASTER_CLK, BIT16, 0xac44, WS_SCLK16, GC_SCLK0);  
	I2S_Init(IIS_OUT, MASTER_CLK, gWaveAudio.datalength>>16, gWaveAudio.samplerate, WS_SCLK16, GC_SCLK0);
    I2S_SetFastMode();
    VIC_EnableInt(INTSRC_DMAC2);    
    //I2S_InitDmaRtxPara(&gDmacCfg , IIS_DUPLEX);  
    I2S_ConfigDmaFDX(IIS_CH0);        
    while(gWaveAudio.datasize > 0)
    {
           while(flag[2]==1)
           {
        	   gWaveAudio.datasize = gWaveAudio.datasize - 1;                 
               flag[2] = 0;                                           
               I2S_ConfigDmaFDX(IIS_CH0);                 	                               
           }            
   }     
}
/********************************************************************
* Function    £º I2S_IntHandler(
* Parameter   £º no para
* Return      £º no return
* Description £º To handle i2s interrupt      
*********************************************************************/
void I2S_IntHandler(void)
{
   volatile U32 uTemp;   
   uTemp = read_reg(I2S_ISR0);	
   /* execute when empty inerrupt occurs*/
	if((uTemp&0x10) != 0)
	{
		I2S_MaskInt(IIS_CH0, TX_EMPTY_INT, TRUE);		
		flag[4] = 1;  		  
	}	
	/* execute when transmitter fifo overrun interrupt occurs*/
	if((uTemp&0x20) != 0)
	{
		I2S_MaskInt(IIS_CH0, TX_OVERRUN_INT, TRUE);		
		flag[5] = 1;
	}	  
	/*  execute when date available interrupt occurs*/
    if((uTemp&0x01) != 0)
    {
	    I2S_MaskInt(IIS_CH0, RX_AVALIABLE_INT, TRUE);	    
        flag[0] = 1;
    }
    /*   execute when receiver fifo overrun interrupt occurs*/   
	 if((uTemp&0x02) != 0)
     { 
     	  I2S_MaskInt(IIS_CH0, RX_OVERRUN_INT, TRUE);     	      		
          flag[1] = 1;            		
     }	
} 

/********************************************************************
* Function    £º I2S_DmaIntHandler
* Parameter   £º 
* Return      £º 
* Description £ºto handle dma interrupt       
*********************************************************************/
void I2S_DmaIntHandler(void)
{	
    *((RP32)(DMAC2_MASKTRF+4  )) = 0;
    *((RP32)(DMAC2_MASKTRF    )) = 1<<8 | 1 ;
    *((RP32)(DMAC2_MASKBLK+4  )) = 0;
    *((RP32)(DMAC2_MASKBLK    )) = 1<<8 | 1 ;
    *((RP32)(DMAC2_MASKSRCTR+4)) = 0 ;
    *((RP32)(DMAC2_MASKSRCTR  )) = 0<<8 | 0 ;
    *((RP32)(DMAC2_MASKDSTTR+4)) = 0 ;
    *((RP32)(DMAC2_MASKDSTTR  )) = 0<<8 | 0 ;
    *((RP32)(DMAC2_MASKERR+4  )) = 0 ;
    *((RP32)(DMAC2_MASKERR    )) = 1<<8 | 1 ;    
	/* Clear any pending interrupts on the channel 0*/
    *((RP32)(DMAC2_CLRTFR+4)  ) |=  0 ;
    *((RP32)(DMAC2_CLRTFR)    ) |=  0x1 ; 
    *((RP32)(DMAC2_CLRBLK+4)  ) |=  0 ;
    *((RP32)(DMAC2_CLRBLK)    ) |=  0x1 ; 
    *((RP32)(DMAC2_CLRSRCTR+4)) |=  0 ;
    *((RP32)(DMAC2_CLRSRCTR)  ) |=  0x1 ; 
    *((RP32)(DMAC2_CLRDSTTR+4)) |=  0 ;
    *((RP32)(DMAC2_CLRDSTTR)  ) |=  0x1 ; 
    *((RP32)(DMAC2_CLRERR+4)  ) |=  0 ;
    *((RP32)(DMAC2_CLRERR)    ) |=  0x1 ;     
    flag[2] = 1;
}

/********************************************************************
* Function    £º I2S_DmaIntHandler
* Parameter   £º 
* Return      £º 
* Description £ºto handle dma interrupt       
*********************************************************************/
void I2S_RegisterTest(void)
{
	I2S_VerifyReg();
	I2S_ReadandWriteReg();
}

void I2S_RecordTest(void)
{
	U32 uSel;
	DebugPrintf("\n\nIIS Record verify: \r\n");
	DebugPrintf("\t0 : CPU Mode\r\n");
	DebugPrintf("\t1 : DMA Mode\r\n");
	DebugPrintf("\t2 : return\r\n");
	DebugPrintf("\n\nSelect the Mode to test : ");
	uSel = DebugGetNum();
	DebugPrintf("\n\n");
	
	switch(uSel)
	{
		case 0:
			//i2s_cpu_record();
			break;
		case 1:
			//i2s_dma_record();
			break;
		default:
			return;
	}
}

void I2S_PlayTest(void)
{
	U32 uSel;
	DebugPrintf("\n\nIIS play verify: \r\n");
	DebugPrintf("\t0 : CPU Mode\r\n");
	DebugPrintf("\t1 : DMA Mode\r\n");
	DebugPrintf("\t2 : return\r\n");
	DebugPrintf("\n\nSelect the Mode to test : ");
	uSel = DebugGetNum();
	DebugPrintf("\n\n");
	
	switch(uSel)
	{
		case 0:
			I2S_CpuPlayback();
			break;
		case 1:
			//i2s_dma_monotransmit_playback();
			//i2s_dma_linkedlist_playback();
			//i2s_dma_autoreload_playback();	  
			break;
		default:
			return;
	}
}

void I2S_FullDuplexTest(void)
{
	U32 uSel;
	DebugPrintf("\n\nIIS full duplex verify: \r\n");
	DebugPrintf("\t0 : CPU Mode\r\n");
	DebugPrintf("\t1 : DMA Mode\r\n");
	DebugPrintf("\t2 : return\r\n");
	DebugPrintf("\n\nSelect the Mode to test : ");
	uSel = DebugGetNum();
	DebugPrintf("\n\n");
	
	switch(uSel)
	{
		case 0:
			//cpu_full_duplex();
			break;
		case 1:
			//dma_full_duplex();  
			break;
		default:
			return;
	}
}

void I2S_InterruptTest(void)
{
	
}

const testFuncMenu IIS_Menu[] =
{
	I2S_RegisterTest,			"IIS Reg Verify",
	I2S_RecordTest,				"IIS record verify",
	I2S_PlayTest,				"IIS play verify",
	I2S_FullDuplexTest,			"IIS full duplex verify",
	I2S_InterruptTest,			"IIS interrupt verify"
};

void IIS_Test(void)
{
	volatile U32 i,uSel,uMenuNum=(sizeof(IIS_Menu)/8);
	char temp[10] = "return";
	gDmacCfg = (DMAC_CFG *)malloc(sizeof(DMAC_CFG));
	while(1)
	{
		DebugPrintf("\n\n");
		DebugPrintf("******************************************\n");
		DebugPrintf("*       IIS - Test firmware v0.1         *\n");
		DebugPrintf("******************************************\n");
		
		for (i=0; i<uMenuNum; i++)
		{
			DebugPrintf("\t%2d: %s  \r\n", i, IIS_Menu[i].desc);
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
			(IIS_Menu[uSel].func) ();
		else
			break;
	}
}
