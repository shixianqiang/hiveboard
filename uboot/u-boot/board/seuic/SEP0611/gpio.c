/*********************************************************************
* Filename    :  mem.c
* Author      :  SixRoom
* Date        :  2011-01-12
* Description :  This file is used to define gpio
* Version  	  :  v1.00
**********************************************************************/
#include	"..\include\gpio.h"
#include	"..\include\uart.h"

volatile U32  gpio_flag[16];
volatile PIN_INT_TYPE IntType = PIN_INT_DOWN;

/***********************************************************
*	Function	：	GPIO_SetDbclkDiv
*	Parameter	：	
*	Return		：  
*	Description	：	去毛刺采样时钟分频比配置寄存器
***********************************************************/
void GPIO_SetDbclkDiv(U32 pDiv)
{
	write_reg(GPIO_DBCLK, pDiv);
}

/***********************************************************
*	Function	：	GPIO_InitPortSelFun
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void GPIO_SetPinSelFun(GPIO_ID pId, U32 pPinNum, PIN_SEL pSelFun)
{
	if(pSelFun == PIN_COMMON)
	{
		*(RP)GPIO_PORT_SEL(pId) |= (pPinNum);
	}
	else if(pSelFun == PIN_SPECIAL)
	{
		*(RP)GPIO_PORT_SEL(pId) &= ~(pPinNum);
	}
}
/***********************************************************
*	Function	：	GPIO_SetPinDir
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void GPIO_SetPinDir(GPIO_ID pId, U32 pPinNum, PIN_DIR pDir)
{
	if(pDir)
	{	//Output
		*(RP)GPIO_PORT_DIR(pId) |= (pPinNum);
	}
	else
	{
		*(RP)GPIO_PORT_DIR(pId) &= ~(pPinNum);
	}
}
/***********************************************************
*	Function	：	GPIO_SetPinValue
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void GPIO_SetPinValue(GPIO_ID pId, U32 pPinNum, PIN_VALUE pValue)
{
	if(pValue)
	{	
		*(RP)GPIO_PORT_DATA(pId) |=	(pPinNum);
	}
	else
	{
		*(RP)GPIO_PORT_DATA(pId) &= ~(pPinNum);
	}
}
/***********************************************************
*	Function	：	GPIO_SetIntType
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void GPIO_SetIntType(GPIO_ID pId, U32 pPinNum, PIN_INT_TYPE pType)
{
	switch(pType)
	{
		case PIN_INT_RISE:
			*(RP)GPIO_PORT_INTLEL(pId) &= ~(pPinNum);
			*(RP)GPIO_PORT_INTPOL(pId) &= ~(pPinNum);
			break;
		case PIN_INT_DOWN:
			*(RP)GPIO_PORT_INTLEL(pId) &= ~(pPinNum);
			*(RP)GPIO_PORT_INTPOL(pId) |= (pPinNum);
			break;				
		case PIN_INT_HIGH:
			*(RP)GPIO_PORT_INTLEL(pId) |= (pPinNum);
			*(RP)GPIO_PORT_INTPOL(pId) &= ~(pPinNum);
			break;	
		case PIN_INT_LOW:
			*(RP)GPIO_PORT_INTLEL(pId) |= (pPinNum);
			*(RP)GPIO_PORT_INTPOL(pId) |= (pPinNum);
			break;	
	}
}
/***********************************************************
*	Function	：	GPIO_SetPinInputType
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void GPIO_SetPinInputType(GPIO_ID pId, U32 pPinNum, PIN_INPUT_TYPE pType)
{
	if(pType == PIN_INT_INPUT)//int input
	{	
		*(RP)GPIO_PORT_INTSEL(pId) |= (pPinNum);
	}
	else
	{
		*(RP)GPIO_PORT_INTSEL(pId) &= ~(pPinNum);
	}
}
/***********************************************************
*	Function	：	GPIO_SetPinInputType
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void GPIO_ClrInt(GPIO_ID pId, U32 pPinNum)
{
	*(RP)GPIO_PORT_INTCLR(pId) = pPinNum;
}
/***********************************************************
*	Function	：	GPIO_InitPort
*	Parameter	：	pId			(GPIO端口)
* 					pGpioCfg	(GPIO配置参数)	
*	Return		：  
*	Description	：	initial gpio
***********************************************************/
void GPIO_InitPort(GPIO_CFG *pGpioCfg)
{
	//set frequency division register
	GPIO_SetDbclkDiv(7);
	//set funciton selection register
	GPIO_SetPinSelFun(pGpioCfg->PORT_ID, pGpioCfg->GPIO_NUM, pGpioCfg->SEL);
	//set DIR register
	GPIO_SetPinDir(pGpioCfg->PORT_ID, pGpioCfg->GPIO_NUM, pGpioCfg->DIR);
	//set Data register
	if(pGpioCfg->DIR == PIN_OUT)
	{
		GPIO_SetPinValue(pGpioCfg->PORT_ID, pGpioCfg->GPIO_NUM, pGpioCfg->VALUE);
	}
	//set INT register
	if((pGpioCfg->INT_STA)&&(pGpioCfg->DIR==0))
	{
		GPIO_ClrInt(pGpioCfg->PORT_ID, 0xffff);
		GPIO_SetPinInputType(pGpioCfg->PORT_ID, pGpioCfg->GPIO_NUM, PIN_INT_INPUT);
		GPIO_SetIntType(pGpioCfg->PORT_ID, pGpioCfg->GPIO_NUM, pGpioCfg->INT_TYPE);
		VIC_UnmaskAllInt(IRQ);
		VIC_EnableInt(pGpioCfg->GPIO_NUM);
	}
}
/***********************************************************
*	Function	：	EXTx_ISR_HANDLER
*	Parameter	：		
*	Return		：  
*	Description	：	
***********************************************************/
void EXT0_IRQRandle(void)
{
	gpio_flag[0] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<0; 
}
void EXT1_IRQRandle(void)
{
	gpio_flag[1] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<1;
}
void EXT2_IRQRandle(void)
{
	gpio_flag[2] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<2;
}
void EXT3_IRQRandle(void)
{
	gpio_flag[3] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<3;
}
void EXT4_IRQRandle(void)
{
	gpio_flag[4] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<4;
}
void EXT5_IRQRandle(void)
{
	gpio_flag[5] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<5;
}
void EXT6_IRQRandle(void)
{
	gpio_flag[6] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<6;
}
void EXT7_IRQRandle(void)
{
	gpio_flag[7] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<7;
}
void EXT8_IRQRandle(void)
{
	gpio_flag[8] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<8;
}
void EXT9_IRQRandle(void)
{
	gpio_flag[9] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<9;
}
void EXT10_IRQRandle(void)
{
	gpio_flag[10] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<10;
}
void EXT11_IRQRandle(void)
{
	gpio_flag[11] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<11;
}
void EXT12_IRQRandle(void)
{
	gpio_flag[12] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<12;
}
void EXT13_IRQRandle(void)
{
	gpio_flag[13] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<13;
}
void EXT14_IRQRandle(void)
{
	gpio_flag[14] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<14;
}
void EXT15_IRQRandle(void)
{
	gpio_flag[15] = 1;
	*(RP)GPIO_PORTI_INTCLR = 1<<15;
}
//***********************************************************
//		Function	：	GPIO_OriginalTest
//		Parameter	：	（无）
//		Description	：	GPIO寄存器复位值测试
//***********************************************************
void GPIO_OriginalTest(void)
{
	RegRstValueVerify(GPIO_PORTAO_DIV, 0x0, REG_WIDTH3);
	RegRstValueVerify(GPIO_PORTAO_DIR, 0xff, REG_WIDTH8);
    RegRstValueVerify(GPIO_PORTAO_SEL, 0x7f, REG_WIDTH7);
    RegRstValueVerify(GPIO_PORTAO_DATA, 0x0, REG_WIDTH32);
    RegRstValueVerify(GPIO_PORTAO_INTSEL, 0x0, REG_WIDTH32);
    RegRstValueVerify(GPIO_PORTAO_INTLEL, 0x0, REG_WIDTH32);
    RegRstValueVerify(GPIO_PORTAO_INTPOL, 0x0, REG_WIDTH32);
    RegRstValueVerify(GPIO_PORTAO_INTCLR, 0x0, REG_WIDTH32);
    RegRstValueVerify(GPIO_DBCLK, 0x0, REG_WIDTH32);
	RegRstValueVerify(GPIO_PORTA_SECFUN, 0x0, REG_WIDTH32);
	RegRstValueVerify(GPIO_PORTA_DIR, 0x1ffff, REG_WIDTH32);
	//NOR_boot this register value is 0x0,else is 0x1ffff
	if(CUR_BOOT_TYPE == NOR_BOOT)
	{
		RegRstValueVerify(GPIO_PORTA_SEL, 0x1ffff, REG_WIDTH32);
	}
	else
	{
		RegRstValueVerify(GPIO_PORTA_SEL, 0x0, REG_WIDTH32);
	}
	RegRstValueVerify(GPIO_PORTA_DATA, 0x0, REG_WIDTH32);
	RegRstValueVerify(GPIO_PORTB_SECFUN, 0x0, REG_WIDTH32);
	RegRstValueVerify(GPIO_PORTB_DIR, 0xffff, REG_WIDTH32);	
	RegRstValueVerify(GPIO_PORTB_SEL, 0x0, REG_WIDTH32);	
	RegRstValueVerify(GPIO_PORTB_DATA, 0x0, REG_WIDTH32);	
	RegRstValueVerify(GPIO_PORTC_DIR, 0xffff, REG_WIDTH16);
    RegRstValueVerify(GPIO_PORTC_SEL, 0xf, REG_WIDTH32); 
    RegRstValueVerify(GPIO_PORTC_DATA, 0x0, REG_WIDTH32);
    RegRstValueVerify(GPIO_PORTC_INTSEL, 0x0, REG_WIDTH32);
    RegRstValueVerify(GPIO_PORTC_INTLEL, 0x0, REG_WIDTH32);
    RegRstValueVerify(GPIO_PORTC_INTPOL, 0x0, REG_WIDTH32);
    RegRstValueVerify(GPIO_PORTC_INTCLR, 0x0, REG_WIDTH32);
    RegRstValueVerify(GPIO_PORTD_DIR, 0xfff, REG_WIDTH32);
    RegRstValueVerify(GPIO_PORTD_SEL, 0xfff, REG_WIDTH32);  
    RegRstValueVerify(GPIO_PORTD_DATA, 0x3E000, REG_WIDTH32);   
    RegRstValueVerify(GPIO_PORTD_INTSEL, 0x0, REG_WIDTH32);   
    RegRstValueVerify(GPIO_PORTD_INTLEL, 0x0, REG_WIDTH32);   
    RegRstValueVerify(GPIO_PORTD_INTPOL, 0x0, REG_WIDTH32);   
    RegRstValueVerify(GPIO_PORTD_INTCLR, 0x0, REG_WIDTH32);  
    RegRstValueVerify(GPIO_PORTE_DIR, 0x3fffff, REG_WIDTH32);
    RegRstValueVerify(GPIO_PORTE_SEL, 0x3ffff0, REG_WIDTH32);      
    RegRstValueVerify(GPIO_PORTE_DATA, 0x0, REG_WIDTH32);       
    RegRstValueVerify(GPIO_PORTE_INTSEL, 0x0, REG_WIDTH32);      
    RegRstValueVerify(GPIO_PORTE_INTLEL, 0x0, REG_WIDTH32);       
    RegRstValueVerify(GPIO_PORTE_INTPOL, 0x0, REG_WIDTH32);        
    RegRstValueVerify(GPIO_PORTE_INTCLR, 0x0, REG_WIDTH32);       
    RegRstValueVerify(GPIO_PORTE_SECFUN, 0x0, REG_WIDTH32);
	RegRstValueVerify(GPIO_PORTF_DIR, 0x1fffffff, REG_WIDTH32);
	RegRstValueVerify(GPIO_PORTF_SEL, 0x1fffffff, REG_WIDTH32);		
	RegRstValueVerify(GPIO_PORTF_DATA, 0x0, REG_WIDTH32);		
	RegRstValueVerify(GPIO_PORTI_DIR, 0x0, REG_WIDTH32);	
	RegRstValueVerify(GPIO_PORTI_SEL, 0xffff, REG_WIDTH32);           
    RegRstValueVerify(GPIO_PORTI_DATA, 0x0, REG_WIDTH32);          
    RegRstValueVerify(GPIO_PORTI_INTSEL, 0x0, REG_WIDTH32);          
    RegRstValueVerify(GPIO_PORTI_INTLEL, 0x0, REG_WIDTH32);          
    RegRstValueVerify(GPIO_PORTI_INTPOL, 0x0, REG_WIDTH32);          
    RegRstValueVerify(GPIO_PORTI_INTCLR, 0x0, REG_WIDTH32);         
}
//***********************************************************
//		Function	：	GPIO_WriteReadTest
//		Parameter	：	（无）
//		Description	：	GPIO寄存器读写测试
//***********************************************************
void GPIO_WriteReadTest(void) 
{
	RegRWVerify(GPIO_DBCLK, REG_WIDTH3);
	RegRWVerify(GPIO_PORTAO_DIV, REG_WIDTH3);
	RegRWVerify(GPIO_PORTAO_SEL, REG_WIDTH8);
	RegRWVerify(GPIO_PORTAO_DIR, REG_WIDTH8);
	RegRWVerify(GPIO_PORTAO_DATA, REG_WIDTH8); 
	RegRWVerify(GPIO_PORTAO_INTSEL, REG_WIDTH4);
	RegRWVerify(GPIO_PORTAO_INTLEL, REG_WIDTH4);
	RegRWVerify(GPIO_PORTAO_INTPOL, REG_WIDTH4);
	RegRWVerify(GPIO_PORTAO_INTCLR, REG_WIDTH4);
    RegRWVerify(GPIO_PORTA_SEL, REG_WIDTH17); 
    RegRWVerify(GPIO_PORTA_DIR, REG_WIDTH17); 
    RegRWVerify(GPIO_PORTA_DATA, REG_WIDTH17); 
    RegRWVerify(GPIO_PORTB_SEL, REG_WIDTH16);
    RegRWVerify(GPIO_PORTB_DIR, REG_WIDTH16);
    RegRWVerify(GPIO_PORTB_DATA, REG_WIDTH16);
    RegRWVerify(GPIO_PORTC_SEL, REG_WIDTH16);
    RegRWVerify(GPIO_PORTC_DIR, REG_WIDTH16);
    RegRWVerify(GPIO_PORTC_DATA, REG_WIDTH16);   
    RegRWVerify(GPIO_PORTC_INTSEL, REG_WIDTH4);
    RegRWVerify(GPIO_PORTC_INTLEL, REG_WIDTH4);
    RegRWVerify(GPIO_PORTC_INTPOL, REG_WIDTH4);
    RegRWVerify(GPIO_PORTC_INTCLR, REG_WIDTH4);

    RegRWVerify(GPIO_PORTD_SEL, REG_WIDTH19);
    RegRWVerify(GPIO_PORTD_DIR, REG_WIDTH19);
    RegRWVerify(GPIO_PORTD_DATA, REG_WIDTH19);
    RegRWVerify(GPIO_PORTD_DATA, REG_WIDTH19);
    RegRWVerify(GPIO_PORTD_INTSEL, 0x30f);
    RegRWVerify(GPIO_PORTD_INTLEL, 0x30f);
    RegRWVerify(GPIO_PORTD_INTPOL, 0x30f); 
    RegRWVerify(GPIO_PORTD_INTCLR, 0x30f);
    
    RegRWVerify(GPIO_PORTE_SEL, REG_WIDTH16);
    RegRWVerify(GPIO_PORTE_SEL, REG_WIDTH16);
    RegRWVerify(GPIO_PORTE_DIR, REG_WIDTH16);
    RegRWVerify(GPIO_PORTE_DATA, REG_WIDTH16);
    RegRWVerify(GPIO_PORTE_DATA, REG_WIDTH16);
    RegRWVerify(GPIO_PORTE_INTSEL, 0x1e30f);
    RegRWVerify(GPIO_PORTE_INTLEL, 0x1e30f);
    RegRWVerify(GPIO_PORTE_INTPOL, 0x1e30f); 
    RegRWVerify(GPIO_PORTE_INTCLR, 0x1e30f);

    RegRWVerify(GPIO_PORTF_SEL, REG_WIDTH29);
    RegRWVerify(GPIO_PORTF_DIR, REG_WIDTH29);
    RegRWVerify(GPIO_PORTF_DATA, REG_WIDTH29);

    RegRWVerify(GPIO_PORTI_SEL, REG_WIDTH16);
    RegRWVerify(GPIO_PORTI_DIR, REG_WIDTH16);
    RegRWVerify(GPIO_PORTI_DATA, REG_WIDTH16);
    RegRWVerify(GPIO_PORTI_INTSEL, 0xfff0);
    RegRWVerify(GPIO_PORTI_INTLEL, 0xfff0);
    RegRWVerify(GPIO_PORTI_INTPOL, 0xfff0); 
    RegRWVerify(GPIO_PORTI_INTCLR, 0xfff0);

    RegRWVerify(GPIO_PORTA_SECFUN, REG_WIDTH8);
    RegRWVerify(GPIO_PORTB_SECFUN, REG_WIDTH16);
    RegRWVerify(GPIO_PORTE_SECFUN, 0xfff0);
}
void GPIO_IntMode_Set(void)
{
	U32 uSel;
	DebugPrintf("\n\nInt_Type list : \r\n");
	DebugPrintf("\t0 : RISE模式\r\n");
	DebugPrintf("\t1 : DOWN模式\r\n");
	DebugPrintf("\t2 : HIGH模式\r\n");
	DebugPrintf("\t3 : LOW模式\r\n");
	DebugPrintf("\n\nSelect the SSI_Mode to test : ");
	uSel = DebugGetNum();
	DebugPrintf("\n\n");
	IntType = (PIN_INT_TYPE)uSel;
}
void GPIO_KEY_Test(void)
{
	U32 i,j=0;
	GPIO_CFG key_l={
			GPIOF,
			PIN(15)|PIN(14)|PIN(13),
			PIN_COMMON,
			PIN_IN,
			PIN_HIGH,				//Init_DATA
			INT_ENABLE,
			PIN_INT_DOWN
	};
	GPIO_CFG key_r={
			GPIOF,
			PIN(8)|PIN(7)|PIN(6),
			PIN_COMMON,
			PIN_OUT,
			PIN_LOW,
			INT_DISABLE,
			PIN_INT_DOWN
	};
	DebugPrintf("Key Test begin!\r\n");
	
	key_l.INT_TYPE = IntType;
	key_r.INT_TYPE = IntType;
	
	GPIO_InitPort(&key_r);
	GPIO_InitPort(&key_l);
	while(j<10)
	{
		j++;
		for(i = 0; i < 16; i++)
		{
			if(gpio_flag[i])
			{
				DebugPrintf("\tExtern irq %d happened\n", i);
				gpio_flag[i]=0;
			}
		}
		DebugPrintf("You have %d chance to hit buttons!\r\n",(10-j));
		delay(100000);
	}
	DebugPrintf("Key Test end!\r\n");
}
void GPIO_OneTest(void)
{
	GPIO_CFG *uOneTestPara;
	uOneTestPara = (GPIO_CFG *)malloc(sizeof(GPIO_CFG));
	uOneTestPara->PORT_ID = GPIO_OT_PORT;
	uOneTestPara->GPIO_NUM = GPIO_OT_PIN;
	uOneTestPara->DIR = GPIO_OT_DIR;
	uOneTestPara->SEL = GPIO_OT_SEL;
	uOneTestPara->VALUE = GPIO_OT_VALUE;
	if(GPIO_OT_INT != NO_PIN_INT)
	{
		uOneTestPara->INT_STA = PIN_INT_INPUT;
		uOneTestPara->INT_TYPE = GPIO_OT_INT;
	}
	GPIO_InitPort(uOneTestPara);
}

const testFuncMenu GPIO_Menu[] =
{
	{GPIO_IntMode_Set,			"GPIO 中断模式选择"},
	{GPIO_KEY_Test,				"GPIO 按键测试"},
	{GPIO_OneTest,				"GPIO 单次验证"}
};
//***********************************************************
//		Function	：	GPIO_Test
//		Parameter	：	（无）
//		Description	：	GPIO测试程序
//***********************************************************
void GPIO_Test(void)
{
	U32 i,uSel,uMenuNum=(sizeof(GPIO_Menu)/8);
	char temp[10] = "return";
	while(1)
	{
		DebugPrintf("\n\n");
		DebugPrintf("******************************************\n");
		DebugPrintf("*       GPIO - Test firmware v0.1        *\n");
		DebugPrintf("******************************************\n");

		for (i=0; i<uMenuNum; i++)
		{
			DebugPrintf("\t%2d: %s  \r\n", i, GPIO_Menu[i].desc);
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
			(GPIO_Menu[uSel].func) ();
		else
			break;
	}
}




