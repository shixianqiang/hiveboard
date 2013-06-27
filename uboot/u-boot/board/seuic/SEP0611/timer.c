/*********************************************************************
* Filename    :  timer.c
* Author      :  SixRoom
* Date        :  2011-01-10
* Description :  This file is used to define timer
* Version  	  :  v1.00
**********************************************************************/
#include "../include/timer.h"
#include "../include/uart.h"

U32 gTimerFlag[4];
COUNT_MODE gTimerCountMode = MODE_RESTATR;

/***********************************************************
*	Function	：	TIMER_ClrInt
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TIMER_ClrInt(TIMER_ID pId)
{
	*(RP)TxISCR(pId);
}
/***********************************************************
*	Function	：	TIMER_MaskInt
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TIMER_MaskInt(TIMER_ID pId, INT_MASK_SET pMask)
{
	*(RP)TxCR(pId) &= ~(1<<4);
	*(RP)TxCR(pId) |= (pMask<<4);
}
/***********************************************************
*	Function	：	TIMER_EnableInt
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TIMER_EnableInt(TIMER_ID pId, INT_SET pSet)
{
	switch(pId)
	{
		case TIMER_ID1:
		case TIMER_ID2:
			if(pSet == INT_ENABLE)
			{
				VIC_EnableInt(INTSRC_TIMER1);
			}
			else
			{
				VIC_DisableInt(INTSRC_TIMER1);
			}
			break;
		case TIMER_ID3:
		case TIMER_ID4:
		case TIMER_ID5:
		case TIMER_ID6:
			if(pSet == INT_ENABLE)
			{
				VIC_EnableInt(INTSRC_TIMER2);
			}
			else
			{
				VIC_DisableInt(INTSRC_TIMER2);
			}
			break;
		case TIMER_ID7:
		case TIMER_ID8:
		case TIMER_ID9:
		case TIMER_ID10:
			if(pSet == INT_ENABLE)
			{
				VIC_EnableInt(INTSRC_TIMER3);
			}
			else
			{
				VIC_DisableInt(INTSRC_TIMER3);
			}
			break;
		default:
			break;	
	}
}
/***********************************************************
*	Function	：	TIMER_Enable
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TIMER_Enable(TIMER_ID pId, BOOL pSta)
{
	*(RP)TxCR(pId) &= 0xfffffffe;
	*(RP)TxCR(pId) |= pSta;
}
/***********************************************************
*	Function	：	TIMER_Start
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TIMER_Start(TIMER_ID pId, BOOL pSta)
{
	*(RP)TxCR(pId) &= 0xfffffffd;
	*(RP)TxCR(pId) |= ((pSta)<<1);
}
/***********************************************************
*	Function	：	TIMER_SetCountMode
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TIMER_SetCountMode(TIMER_ID pId, COUNT_MODE pMode)
{
	*(RP)TxCR(pId) &= 0xfffffffc;
	*(RP)TxCR(pId) |= ((pMode)<<2);
}
/***********************************************************
*	Function	：	TIMER_SetMatchMode
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TIMER_SetMatchMode(TIMER_ID pId, MATCH_MODE pMode)
{
	*(RP)TxCR(pId) &= 0xffffff9f;
	*(RP)TxCR(pId) |= ((pMode)<<5);
}
/***********************************************************
*	Function	：	TIMER_SetPreLoadValue
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TIMER_SetPreLoadValue(TIMER_ID pId, U32 pValue)
{
	if(pId < TIMER_ID7)
	{
		*(RP)TxLCR(pId) = pValue;
	}
	else
	{
		*(RP)TxLCR(pId) = pValue & 0x0000ffff;
	}
}
/***********************************************************
*	Function	：	TIMER_ReadCurCountValue
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
U32 TIMER_ReadCurCountValue(TIMER_ID pId)
{
	if(pId < TIMER_ID7)
	{
		return *(RP)TxCCR(pId);
	}
	else
	{
		return (*(RP)TxCCR(pId) & 0x0000ffff);
	}
}
/***********************************************************
*	Function	：	TIMER_ReadIntStatus
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
INT_STATUS TIMER_ReadIntStatus(TIMER_ID pId)
{
	return (INT_STATUS)(((*(RP)TIMER_TISR)>>(pId))&0x01);
}
/***********************************************************
*	Function	：	Delay
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TIMER_Delay(TIMER_ID pId, U32 pDelay, DELAY_UNIT pType)
{
	TIMER_Start(pId, FALSE);
	TIMER_Enable(pId, FALSE);
	/*enable int for timer*/
	TIMER_EnableInt(pId, INT_DISABLE);
	/*set Timer's count mode*/
	TIMER_SetCountMode(pId, MODE_ONESHOT);
	TIMER_SetPreLoadValue(pId, pDelay);
	TIMER_Start(pId, TRUE);
	TIMER_Enable(pId, TRUE);
	while(TIMER_ReadIntStatus(pId)==INT_FALSE);
}
/***********************************************************
*	Function	：	Timer_Init
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TIMER_Init(TIMER_CFG *pTimerPara)
{
	/*enable int for timer*/
	TIMER_EnableInt(pTimerPara->TimerId, INT_DISABLE);
	/*unmask correspond int*/
	TIMER_MaskInt(pTimerPara->TimerId, pTimerPara->IntMask);
	/*set Timer's count mode*/
	TIMER_SetCountMode(pTimerPara->TimerId, pTimerPara->CountMode);
	/*set match out mode*/
	TIMER_SetMatchMode(pTimerPara->TimerId, pTimerPara->MatchoutMode);
	if(pTimerPara->CountMode != MODE_FREE)
	{
		TIMER_SetPreLoadValue(pTimerPara->TimerId, pTimerPara->pPreLoadValue);
	}
	if(pTimerPara->IntMask == INT_UNMASK)
	{
		VIC_UnmaskAllInt(IRQ);
		TIMER_EnableInt(pTimerPara->TimerId, INT_ENABLE);
	}
	TIMER_Start(pTimerPara->TimerId, TRUE);
	TIMER_Enable(pTimerPara->TimerId, TRUE);
}
/***********************************************************
*	Function	：	TIMER1_IRQHandle
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TIMER1_IRQHandle(void)
{
	TIMER_ClrInt(TIMER_ID1);
	TIMER_ClrInt(TIMER_ID2);
	gTimerFlag[1]=1;
}
/***********************************************************
*	Function	：	TIMER2_IRQHandle
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TIMER2_IRQHandle(void)
{
	TIMER_ClrInt(TIMER_ID3);
	TIMER_ClrInt(TIMER_ID4);
	TIMER_ClrInt(TIMER_ID5);
	TIMER_ClrInt(TIMER_ID6);
	gTimerFlag[2]=1;
}
/***********************************************************
*	Function	：	TIMER3_IRQHandle
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TIMER3_IRQHandle(void)
{
	TIMER_ClrInt(TIMER_ID7);
	TIMER_ClrInt(TIMER_ID8);
	TIMER_ClrInt(TIMER_ID9);
	TIMER_ClrInt(TIMER_ID10);
	gTimerFlag[3]=1;
}
/***********************************************************
*	Function	：	Timer_mode_select
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void TIMER_CountModeTest(void)
{
	U32 uSel;
	DebugPrintf("\n\nCount Mode list : \r\n");
	DebugPrintf("\t0 : 自由计数模式\r\n");
	DebugPrintf("\t1 : 重启计数模式\r\n");
	DebugPrintf("\t2 : OneShot计数模式\r\n");
	DebugPrintf("\n\nSelect the Count Mode to test : ");
	uSel = DebugGetNum();
	DebugPrintf("\n\n");
	switch(uSel)
	{
		case 0:
			DebugPrintf("select 自由计数模式!\r\n");
			gTimerCountMode = MODE_FREE;
			break;
		case 1:
			DebugPrintf("select 重启计数模式!\r\n");
			gTimerCountMode = MODE_RESTATR;
			break;
		case 2:
			DebugPrintf("select OneShot计数模式!\r\n");
			gTimerCountMode = MODE_ONESHOT;
			break;
		default:
			DebugPrintf("Invilid input,reset to 重启计数模式!\r\n");
			gTimerCountMode = MODE_RESTATR;
			break;
	}
}

void TIMER_IntTest(void)
{
	TIMER_CFG uTimerPara={
			TIMER_ID1,
			MODE_RESTATR,
			50000*1000,
			OUT_NULL,
			INT_UNMASK
	};
	U32 i;
	uTimerPara.CountMode = gTimerCountMode;
	TIMER_Init(&uTimerPara);
	while(i++<5)
	{
		while(gTimerFlag[uTimerPara.TimerId]==0);
		gTimerFlag[uTimerPara.TimerId]=0;
		DebugPrintf("\tThe %ds interrupt happened!\r\n",i);
	}
}

void TIMER_OneTest(void)
{
	TIMER_CFG *uOneTestPara;
	uOneTestPara = (TIMER_CFG *)malloc(sizeof(TIMER_CFG));
	uOneTestPara->TimerId = TIMER_OT_ID;
	uOneTestPara->pPreLoadValue = TIMER_OT_PREVLU;
	uOneTestPara->CountMode = TIMER_OT_MODE;
	uOneTestPara->MatchoutMode = OUT_NULL;
	uOneTestPara->IntMask = TIMER_OT_INT;
	TIMER_Init(uOneTestPara);
	while(gTimerFlag[uOneTestPara->TimerId]==0);
	gTimerFlag[uOneTestPara->TimerId]=0;
	DebugPrintf("\tTimer interrupt happened!\r\n");	
}

const testFuncMenu Timer_Menu[] =
{
	TIMER_CountModeTest,	"Timer 计数模式选择",
	TIMER_IntTest,			"Timer 中断测试",
	TIMER_OneTest,			"Timer 一次验证"
};

void Timer_Test(void)
{
	U32 i,uSel,uMenuNum=(sizeof(Timer_Menu)/8);
	char temp[10] = "return";
	while(1)
	{
		DebugPrintf("\n\n");
		DebugPrintf("******************************************\n");
		DebugPrintf("*        Timer - Test firmware v0.1      *\n");
		DebugPrintf("******************************************\n");

		for (i=0; i<uMenuNum; i++)
		{
			DebugPrintf("\t%2d: %s  \r\n", i, Timer_Menu[i].desc);
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
			(Timer_Menu[uSel].func) ();
		else
			break;
	}
}
