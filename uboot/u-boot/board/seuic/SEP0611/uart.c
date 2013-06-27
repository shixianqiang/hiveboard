/*********************************************************************
* Filename    :  uart.h
* Author      :  SixRoom
* Date        :  2011-01-10
* Description :  This file is used to define Uart
* Version  	  :  v1.01
**********************************************************************/
/*
|---------------|------------------|---------------|-----------------------------------------|
|    version    |       date       |     Author    |           modify infomation             |
|---------------|------------------|---------------|-----------------------------------------|
|    v1.00      |    2010-02-25    |  zhang shuli  | 1) create this typedef file    		 |
|    v1.01      |    2011-01-10    |  SixRoom      | 1) modify some function                 |
|---------------|------------------|---------------|-----------------------------------------|
*/
#include "../include/uart.h"
#include "../include/gpio.h"

static U32 gUartIntFlag[4][5];
static U32 gUartRevStrComplete[4];
static char pRxBuffer[4][256];

UART_CFG *gUartPara;

/***********************************************************
*	Function	：	UART_InitPort
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART_InitPort(UART_ID pId)
{
	switch (pId)
	{
		case UART_ID0:										//UART0:PD10/11
			GPIO_SetPinSelFun(GPIOD, PIN(11)|PIN(10), PIN_SPECIAL);
			break;
		case UART_ID1:										//UART0:PD8/9
			*(RP32)GPIO_PORTD_SEL &= ~((1<<9)|(1<<8));
			break;
		case UART_ID2:										//UART0:PD6/7
			*(RP32)GPIO_PORTD_SEL &= ~((1<<7)|(1<<6));
			break;
		case UART_ID3:										//UART0:PD2/3
			*(RP32)GPIO_PORTD_SEL &= ~((1<<3)|(1<<2));
			break;
		default:
			break;
	}
}
/***********************************************************
*	Function	：	UART_EnableInt
*	Parameter	：	
*	Return		：  
*	Description	：	UART中断配置
***********************************************************/
void UART_EnableInt(UART_ID pId, U32 pNum, INT_SET pIntEn)
{
	if(pIntEn)
	{
		*(RP32)UART_IER_CH(pId) |= (pNum);
	}
	else
	{
		*(RP32)UART_IER_CH(pId) &= ~(pNum);
	}
}
/***********************************************************
*	Function	：	UART_ReadIIR
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
U32 UART_ReadIIR(UART_ID pId)
{
	return *(RP)UART_IIR_CH(pId);
}
/***********************************************************
*	Function	：	UART_SetBaud
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART_SetBaud(UART_ID pId, U32 pBaud)
{
	*(RP32)UART_LCR_CH(pId) |=	(1<<7);
	*(RP32)UART_DLBH_CH(pId)=	(pBaud >> 8)&0xff;
	*(RP32)UART_DLBL_CH(pId)=	(pBaud & 0xff);
 	*(RP32)UART_LCR_CH(pId)	&=	(~(1<<7));
}
/***********************************************************
*	Function	：	UART_SetDataLen
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART_SetDataLen(UART_ID pId, UART_DATALEN pDataLen)
{
	*(RP)UART_LCR_CH(pId) &= ~(0x03);
	*(RP)UART_LCR_CH(pId) |= pDataLen;
}
/***********************************************************
*	Function	：	UART_SetStopBit
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART_SetStopBit(UART_ID pId, UART_STOPBIT pStopBit)
{
	*(RP)UART_LCR_CH(pId) &= ~(1<<2);
	*(RP)UART_LCR_CH(pId) |= ((pStopBit)<<2);
}
/***********************************************************
*	Function	：	UART_SetVerifyBit
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART_SetVerifyBit(UART_ID pId, UART_VERIFY pVerifyBit)
{
	*(RP)UART_LCR_CH(pId) &= ~(7<<3);
    switch (pVerifyBit)  
    {
    	case ODD:
    		*(RP)UART_LCR_CH(pId) |= (1<<5)|((pVerifyBit)<<4)|(1<<3);
     		break;
    	case EVEN:
    		*(RP)UART_LCR_CH(pId) |= (0<<5)|((pVerifyBit)<<4)|(1<<3);
    		break;
    	default:
    		*(RP)UART_LCR_CH(pId) |= (0<<5)|(0<<4)|(0<<3);
    		break;
    }
}
/***********************************************************
*	Function	：	UART_SetFIFOTrig
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART_SetFIFOTrig(UART_ID pId, UART_TXFIFO pTxFIFO, UART_RXFIFO pRxFIFO)
{
	*(RP)UART_FCR_CH(pId) = (pRxFIFO<<6)|(pTxFIFO<<4);
}
/***********************************************************
*	Function	：	UART_ClrFIFO
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART_ClrFIFO(UART_ID pId, U32 pTxFIFO, U32 pRxFIFO)
{
	*(RP)UART_FCR_CH(pId) |= (pTxFIFO<<2)|(pRxFIFO<<1);
}
/***********************************************************
*	Function	：	UART_SetMode
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART_SetMode(UART_ID pId, UART_MODE pMode)
{
	switch (pMode)
	{
		case COMMON_TRAN:
			*(RP)UART_MCR_CH(pId) |= ((0<<4)|(0<<5)|(0<<6));
			break;
		case IRDA_TRAN:
			*(RP)UART_MCR_CH(pId) |= ((0<<4)|(0<<5)|(1<<6));
			break;
		case AUTOCON_TRAN:
			*(RP)UART_MCR_CH(pId) |= ((0<<4)|(1<<5)|(0<<6));
			break;
		case LOOPBACK_TRAN:
			*(RP)UART_MCR_CH(pId) |= ((1<<4)|(0<<5)|(0<<6));
			break;
		default:
			break;
	}
}
/***********************************************************
*	Function	：	UART_Init
*	Parameter	：	
*	Return		：  
*	Description	：	串口初始化函数(PD10/11,PD8/9,PD6/7,PD2/3)
***********************************************************/
void UART_Init(UART_CFG *pUartPara)
{
	U32 uBaud;
	uBaud = (SYS_CLK) / 16;
	uBaud = uBaud / (pUartPara->UartBaud);
	/*set baud*/
	UART_SetBaud(pUartPara->UartId, uBaud);
	/*set verify bit*/
	UART_SetVerifyBit(pUartPara->UartId, pUartPara->UartVerify);
	/*set data length*/
	UART_SetDataLen(pUartPara->UartId, pUartPara->UartDataLen);
	/*set stop bit */
	UART_SetStopBit(pUartPara->UartId, pUartPara->UartStopBit);
	/*set fifo trig level*/
	UART_SetFIFOTrig(pUartPara->UartId, pUartPara->UartTxFifo, pUartPara->UartRxFifo);
	/*enable int for uart*/
	UART_EnableInt(pUartPara->UartId, pUartPara->UartIntNum, INT_ENABLE);
}
/***********************************************************
*	Function	：	UART_PutChar
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART_PutChar(UART_ID pId, char pChar)
{
	if(pChar=='\n')
	{
		while(((*(RP)UART_LSR_CH(pId))&0x00000020)==0);
		*(RP)UART_TXFIFO_CH(pId)='\r';
	}
	while(((*(RP)UART_LSR_CH(pId))&0x00000020)==0);
	*(RP)UART_TXFIFO_CH(pId)=pChar;
}
/***********************************************************
*	Function	：	UART_GetChar
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
char UART_GetChar(UART_ID pId)
{
	while(((*(RP)UART_LSR_CH(pId))&0x00000001)==0);
	return *(RP)UART_RXFIFO_CH(pId);
}
/***********************************************************
*	Function	：	UART_PutNChar
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART_PutNChar(UART_ID pId, char *pString,U32 pLen)
{
	U32 i;
	for(i=0;i<pLen;i++)
	{
		UART_PutChar(pId, *(pString+i));
	}
}
/***********************************************************
*	Function	：	UART_PutString
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART_PutString(UART_ID pId, char *pString)
{
    while(*pString)
    {
    	UART_PutChar(pId,*pString++);
    }
}
/***********************************************************
*	Function	：	UART_GetLine
*	Parameter	：	
*	Return		：  
*	Description	：	以enter为结束符,返回值个数不包括‘\0’
***********************************************************/
U32 UART_GetLine(UART_ID pId, char *pStr)
{
    char *uStr, uChar;
    uStr = pStr;
    while ((uChar= UART_GetChar(pId))!='\r')
    {
 		if (uChar=='\b') 
 		{
 		    if ((int)pStr < (int)uStr) 
 		    {
 				UART_PutString(pId, "\b \b");
 				uStr--;
 		    }
 		}
 		else 
 		{
 		   *uStr++ = uChar;
 		   UART_PutChar(pId, uChar);
 		}
    }
    *uStr = '\0';
    if ((int)pStr < (int)uStr) //如果没有输入任何有效字符，即使输入enter也无效
    {
    	UART_PutChar(pId, '\n');
    }
    return (strlen(pStr));
}
/***********************************************************
*	Function	：	Str2Int
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
static S32 Str2Int(char *string)
{
	S32 base     = 10;
	S32 minus    = 0;
	S32 result   = 0;
	S32 lastIndex;
	U32 i;
    if(string[0]=='-')
    {
        minus = 1;
        string++;
    }
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
    {
        base    = 16;
        string += 2;
    }
    lastIndex = strlen((const char *)string) - 1;
    if(lastIndex<0)
        return -1;
    if(string[lastIndex]=='h' || string[lastIndex]=='H' )
    {
        base = 16;
        string[lastIndex] = 0;
        lastIndex--;
    }
    if(base==10)
    {
        result = 0;
        for(i = 0; i<=lastIndex; i++)
        {
			result = result * 10 + string[i] - 0x30;
       	}
        result = minus ? (-1*result):result;
    }
    else
    {
        for(i=0;i<=lastIndex;i++)
        {
            if(isalpha(string[i]))
            {
                if(isupper(string[i]))
                    result = (result<<4) + string[i] - 'A' + 10;
                else
                    result = (result<<4) + string[i] - 'a' + 10;
            }
            else
                result = (result<<4) + string[i] - '0';
        }
        result = minus ? (-1*result):result;
    }
    return result;
}
/***********************************************************
*	Function	：	UART_GetIntNum
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
U32 UART_GetInputNum(UART_ID pId)
{
    char uStr[32];
    UART_GetLine(pId,uStr);
    return Str2Int(uStr);
}
/***********************************************************
*	Function	：	UART_VerifyInt
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
U32 UART_VerifyInt(UART_ID pId)
{
	return ((*(RP)UART_IER_CH(pId)&0x00000001) ? ((*(RP)UART_IER_CH(pId)&0x0000000e)>>1):NONE_INT);
}
/***********************************************************
*	Function	：	DebugOpenConsole
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void DebugOpenConsole(void)
{
	UART_CFG *uUartPara;
	uUartPara = (UART_CFG *)malloc(sizeof(UART_CFG));
	uUartPara->UartId = UART_ID0;
	uUartPara->UartBaud = UART_DB_BAUD;
	uUartPara->UartDataLen = UART_DB_DATALEN;
	uUartPara->UartStopBit = UART_DB_STOPBIT;
	uUartPara->UartVerify = UART_DB_VERIFY;
	uUartPara->UartTxFifo = TXLEVEL0;
	uUartPara->UartRxFifo = RXLEVEL1;
	uUartPara->UartIntNum = NONE_INT;
	UART_InitPort(UART_ID0);
	UART_Init(uUartPara);
}
/***********************************************************
*	Function	：	DebugClrConsoleScreen
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void DebugClrConsoleScreen(void)
{
	UART_PutChar(UART_ID0,0x0c);
}
/***********************************************************
*	Function	：	DebugWaitForKey
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void DebugWaitForKey(void)
{
	UART_GetChar(UART_ID0);
}
/***********************************************************
*	Function	：	DebugGetNum
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
U32 DebugGetNum(void)
{
	return UART_GetInputNum(UART_ID0);
}
/***********************************************************
*	Function	：	DebugPrintf
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void DebugPrintf(const char *fmt,...)
{
    va_list ap;
    char string[256];
    va_start(ap, fmt);
    vsprintf(string, fmt, ap);
    UART_PutString(UART_ID0, string);
    va_end(ap);
}
/***********************************************************
*	Function	：	UART0_IRQ_Handler
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART0_IRQ_Handler(void)
{
    static U32 i=0;
    U32 uTemp = UART_ReadIIR(0);//read the IIR
	switch((uTemp >> 1) & 0x07)
    {
        case 3 : 										//receive data status interrupt
        	uTemp=*(RP)UART_LSR_CH(0);
        	gUartIntFlag[0][0] = 1;
            break;
        case 2 : 										//receive fifo interrupt
        	pRxBuffer[0][i++] = *(RP)UART_RXFIFO_CH(0);
        	gUartIntFlag[0][1] = 1;
            break;
        case 6 : 										//timeout interrupt
        	pRxBuffer[0][i++] = *(RP)UART_RXFIFO_CH(0);
        	gUartIntFlag[0][2] = 1;
            break;
        case 1 : 										//send fifo interrupt
        	gUartIntFlag[0][3] = 1;
            break;
        case 0 : 										//modem change interrupt
        	gUartIntFlag[0][4] = 1;
            break;
    }
	if(pRxBuffer[0][i-1]=='#')
	{
		i=0;	//静态变量清零
		gUartRevStrComplete[0] = 1;  //如果接收字符时遇到“#”，代表字符串结束符（在超级终端中很难模拟以'\0'作为结束符的情况）
	}
}
/***********************************************************
*	Function	：	UART1_IRQ_Handler
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART1_IRQ_Handler(void)
{
	
}
/***********************************************************
*	Function	：	UART2_IRQ_Handler
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART2_IRQ_Handler(void)
{
	
}
/***********************************************************
*	Function	：	UART3_IRQ_Handler
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART3_IRQ_Handler(void)
{
	
}
/***********************************************************
*	Function	：	UART_Test
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void UART_RwTest(void)
{
	
}

void UART_ParaTest(void)
{
	U32 uId,uBaud,uDataLen,uStopBit,m,n;
	char uTemp;
	U32 uBaudList[5]={300,9600,19200,38400,115200};
	for(uId=UART_ID0;uId<=UART_ID3;uId++)
	{
		UART_InitPort(uId);
		//VIC_EnableInt(INTSRC_UART1 + uId);  
		//设置为LoopBack模式
		UART_SetMode(uId, LOOPBACK_TRAN);
		gUartPara->UartId = uId;
		for(uBaud=0;uBaud<sizeof(uBaudList)/4;uBaud++)
		{
			gUartPara->UartBaud = uBaudList[uBaud];
			for(uDataLen=DATA_LEN5;uDataLen<DATA_LEN8;uDataLen++)
			{
				gUartPara->UartDataLen = uDataLen;
				for(uStopBit=STOP_BIT1;uStopBit<STOP_BIT2;uStopBit++)
				{
					gUartPara->UartStopBit = uStopBit;
					gUartPara->UartVerify = NOVERIFY;
					gUartPara->UartTxFifo = TXLEVEL0;
					gUartPara->UartRxFifo = RXLEVEL1;
					gUartPara->UartIntNum = NONE_INT;
					
					UART_Init(gUartPara);
					for(m=0;m<0xff;m++)
					{
						UART_PutChar(gUartPara->UartId, m);
						uTemp = UART_GetChar(gUartPara->UartId);
						if(uTemp != m)
						{
							DebugPrintf("\tUART LoopBack Tran Error %d, %d!\r\n", m, uTemp);
						}
					}
				}
			}
		}
		//VIC_DisableInt(INTSRC_UART1 + uId);
	}
}

void UART_IntTest(void)
{
	U32 i,j,uNum;
	gUartPara->UartId = UART_OT_ID;
	UART_SetMode(gUartPara->UartId, LOOPBACK_TRAN);
	gUartPara->UartBaud = UART_OT_BAUD;
	gUartPara->UartDataLen = UART_OT_DATALEN;
	gUartPara->UartStopBit = UART_OT_STOPBIT;
	gUartPara->UartVerify = UART_OT_VERIFY;
	VIC_EnableInt(INTSRC_UART0 + gUartPara->UartId);  
	UART_InitPort(gUartPara->UartId);
	//发送FIFO空中断
	for(j=TXLEVEL0;j<TXLEVEL8;j++)
	{
		gUartPara->UartRxFifo = RXLEVEL1;
		gUartPara->UartTxFifo = j;
		UART_Init(gUartPara);
		//*(RP)UART_TXFIFO_CH(gUartPara->UartId) = 0xff;
		while(((*(RP)UART_LSR_CH(gUartPara->UartId))&0x00000020)==0);
		DebugPrintf("\tUART FIFO Tx Int happen!\r\n");
	}
	//接收FIFO中断
	for(i=RXLEVEL1;i<RXLEVEL14;i++)
	{
		gUartPara->UartTxFifo = TXLEVEL0;
		gUartPara->UartRxFifo = i;
		UART_Init(gUartPara);
		do{
			*(RP)UART_TXFIFO_CH(gUartPara->UartId) = 0xff;
		}while((*(RP)UART_LSR_CH(gUartPara->UartId))&0x02==0);
		DebugPrintf("\tUART FIFO Rx Int happen!\r\n");
	}
}

void UART_OneTest(void)
{
	gUartPara->UartId = UART_OT_ID;
	UART_SetMode(gUartPara->UartId, UART_OT_MODE);
	gUartPara->UartBaud = UART_OT_BAUD;
	gUartPara->UartDataLen = UART_OT_DATALEN;
	gUartPara->UartStopBit = UART_OT_STOPBIT;
	gUartPara->UartVerify = UART_OT_VERIFY;
	gUartPara->UartTxFifo = TXLEVEL0;
	gUartPara->UartRxFifo = RXLEVEL1;
	gUartPara->UartIntNum = RX_FIFO_INT;
	UART_InitPort(gUartPara->UartId);
	UART_Init(gUartPara);
	VIC_EnableInt(INTSRC_UART1 + gUartPara->UartId);  
	while(gUartRevStrComplete[gUartPara->UartId]==0);	/*等待接收字符串完毕*/
	UART_PutString(gUartPara->UartId, pRxBuffer[gUartPara->UartId]);
	gUartRevStrComplete[gUartPara->UartId]=0;
}

const testFuncMenu UART_Menu[] =
{
	{UART_RwTest,				"UART 读写验证"},
	{UART_ParaTest,				"UART 基本参数测试"},
	{UART_IntTest,				"UART 中断测试"},
	{UART_OneTest,				"UART 单次验证"}
};

void UART_Test(void)
{
	U32 i,uSel,uMenuNum=(sizeof(UART_Menu)/8);
	char temp[10] = "return";
	while(1)
	{
		DebugPrintf("\n\n");
		DebugPrintf("******************************************\n");
		DebugPrintf("*       UART - Test firmware v0.1        *\n");
		DebugPrintf("******************************************\n");

		for (i=0; i<uMenuNum; i++)
		{
			DebugPrintf("\t%2d: %s  \r\n", i, UART_Menu[i].desc);
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
			(UART_Menu[uSel].func) ();
		else
			break;
	}
}


