/*********************************************************************
* Filename    :  pwm.c
* Author      :  SixRoom
* Date        :  2011-01-14
* Description :  
* Version  	  :  v1.01
**********************************************************************/
/*
|---------------|------------------|---------------|-----------------------------------------|
|    version    |       date       |     Author    |           modify infomation             |
|---------------|------------------|---------------|-----------------------------------------|
|    v1.00      |    2010-06-22    |  SixRoom      | 1) create this file    				 |
|    v1.01      |    2011-01-14    |  SixRoom      | 1) Sorting type, modify functions	     |
|---------------|------------------|---------------|-----------------------------------------|
*/
#include 	"../include/pwm.h"
#include	"../include/uart.h"

U32 PWM_IntFlag[2][4] = {{0,0,0,0},{0,0,0,0}}; 

/***********************************************************
*	Function	：	PWM_RegVerifyTest
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void PWM_RegVerifyTest(void)
{
	
}
/***********************************************************
*	Function	：	PWM_SetPort
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void PWM_SetPort(PWM_CH pId)
{
	*(RP)GPIO_PORTE_DIR |= (1<<(pId+3));			 //PE4、PE5
	*(RP)GPIO_PORTE_SEL &= ~(1<<(pId+3));
}

/***********************************************************
*	Function	：	PWM_InitPwmMode
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void PWM_InitPwmMode(PWM_PWM_CFG *pPwmPara)
{
	/*disable INTSRC_PWM*/
	VIC_DisableInt(INTSRC_PWM); 
	PWM_Enable(pPwmPara->PwmId, FALSE);
	/*set pwm's mode*/
	PWM_SetMode(pPwmPara->PwmId, MODE_PWM);
	/*set PWM prescale*/
	PWM_SetPreScale(pPwmPara->PwmId, pPwmPara->Prescale);
	/*set period register*/
	PWM_SetPeriod(pPwmPara->PwmId, pPwmPara->Period);
	/*set duty cycle*/
	PWM_SetDutyCycle(pPwmPara->PwmId, pPwmPara->DutyCycle);
	if(pPwmPara->PwmMask == INT_UNMASK)
	{
		/*configure int for pwm*/
		PWM_MaskInt(pPwmPara->PwmId, pPwmPara->PwmMask);
		VIC_EnableInt(INTSRC_PWM);
	}
	PWM_Enable(pPwmPara->PwmId, TRUE);
}
/***********************************************************
*	Function	：	PWM_InitTimerMode
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void PWM_InitTimerMode(PWM_TIMER_CFG *pPwmPara)
{
	/*disable INTSRC_PWM*/
	VIC_DisableInt(INTSRC_PWM);
	PWM_Enable(pPwmPara->PwmId, FALSE);
	/*set pwm's mode*/
	PWM_SetMode(pPwmPara->PwmId, pPwmPara->TimerMode);
	/*set PWM prescale*/
	PWM_SetPreScale(pPwmPara->PwmId, pPwmPara->Prescale);
	/*set period value*/
	PWM_SetPeriod(pPwmPara->PwmId, pPwmPara->Period);
	/*set match mode,when count == period ,the output's ele level*/
	PWM_SetMatchMode(pPwmPara->PwmId, pPwmPara->MatchMode);
	/*configure int for pwm*/
	PWM_MaskInt(pPwmPara->PwmId, pPwmPara->PwmMask);
	if(pPwmPara->PwmMask == INT_UNMASK)
	{
		VIC_EnableInt(INTSRC_PWM);
	}
	PWM_Enable(pPwmPara->PwmId, TRUE);
}
/***********************************************************
*	Function	：	PWM_InitHGpioMode
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void PWM_InitHGpioMode(PWM_GPIO_CFG *pPwmPara)
{
	/*disable INTSRC_PWM*/
	VIC_DisableInt(INTSRC_PWM); 
	PWM_Enable(pPwmPara->PwmId, FALSE);
	/*set pwm's mode*/
	PWM_SetMode(pPwmPara->PwmId, MODE_HGPIO);
	/*set PWM prescale*/
	PWM_SetPreScale(pPwmPara->PwmId, pPwmPara->Prescale);
	/*set gpio dir*/
	PWM_SetHGDir(pPwmPara->PwmId, pPwmPara->PwmDir);
	/*configure int for pwm*/
	PWM_MaskInt(pPwmPara->PwmId, pPwmPara->PwmMask);
	if(pPwmPara->PwmMask == INT_UNMASK)
	{
		VIC_EnableInt(INTSRC_PWM);
	}
	PWM_Enable(pPwmPara->PwmId, TRUE);
}
/***********************************************************
*	Function	：	PWM_IRQHandler
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void PWM_IRQHandler(void)
{
	int temp1,temp2;
	int i;   
	temp1 = *(RP)PWM_INT;
	*(RP)PWM_INT = temp1;  //Clear int
	if(temp1 & 0x01)		//PWM1
	{
		temp2 = *(RP)PWM1_STATUS;
		for(i=0;i<4;i++)
		{
			if((temp2>>i)&0x01)
			{
				PWM_IntFlag[0][i] = 1;
			}
		}
	}
	if(temp1 & 0x02)		//PWM2
	{
		temp2 = *(RP)PWM2_STATUS;
		for(i=0;i<4;i++)
		{
			if((temp2>>i)&0x01)
			{
				PWM_IntFlag[1][i] = 1;
			}
		}
	}
}

/***********************************************************
*	Function	：	PWM_ModeTest
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void PWM_PWMModeTest(void)
{
	volatile U32 i, uSel, uNum;
	PWM_PWM_CFG uPwmPara = {
		PWM_CH1,
		0,				//prescale
		50*1000,		//cycle  1000us
		50*1000/2,		//duty
		INT_UNMASK		//INT_mask
	};
	while(1)
	{
		DebugPrintf("\t0 : 设置PWM周期\r\n");
		DebugPrintf("\t1 : 设置PWM占空比\r\n");
		DebugPrintf("\t2 : 运行\r\n");
		DebugPrintf("\t3 : return\r\n");
		DebugPrintf("\n\nSelect the serial num to test : ");
		uSel = DebugGetNum();
		DebugPrintf("\n\n");
		switch(uSel)
		{
			case 0:
				DebugPrintf("\nset pwm period : ");
				uNum = DebugGetNum();
				DebugPrintf("\n\n");
				uPwmPara.Period = uNum;	
				break;
			case 1:
				DebugPrintf("\nset pwm duty cycle : ");
				uNum = DebugGetNum();
				DebugPrintf("\n\n");
				uPwmPara.DutyCycle = uNum;	
				break;
			case 2:
				/*configure port for PWM*/
				PWM_SetPort(uPwmPara.PwmId);
				PWM_InitPwmMode(&uPwmPara);
				delay(100);
				for(i=0;i<4;i++)
				{
					if(PWM_IntFlag[uPwmPara.PwmId-1][i]==1)
					{
						DebugPrintf("\tInt %d happen!\r\n");
						PWM_IntFlag[uPwmPara.PwmId-1][i] = 0;
					}
				}
				break;
			default:
				return;	
		}
	}
}
/***********************************************************
*	Function	：	PWM_TimerModeTest
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void PWM_TimerModeTest(void)
{
	volatile U32 i, uSel, uNum;
	PWM_TIMER_CFG uPwmPara = {
		PWM_CH1,
		MODE_CYCLE,
		0,				//prescale
		PWMOUT_TOGGLE,
		50*1000,		//cycle  1000us
		INT_UNMASK		//INT_mask
	};
	while(1)
	{
		DebugPrintf("\t0 : 选择定时器计数模式\r\n");
		DebugPrintf("\t1 : 选择匹配输出方式\r\n");
		DebugPrintf("\t2 : 设置匹配值\r\n");
		DebugPrintf("\t3 : 运行\r\n");
		DebugPrintf("\t4 : return\r\n");
		DebugPrintf("\n\nSelect the serial num to test : ");
		uSel = DebugGetNum();
		DebugPrintf("\n\n");
		switch(uSel)
		{
			case 0:
				DebugPrintf("\t2 : MODE_SHOT\r\n");
				DebugPrintf("\t3 : MODE_CYCLE\r\n");			
				DebugPrintf("\nselect timer mode:");
				uNum = DebugGetNum();
				DebugPrintf("\n\n");
				uPwmPara.TimerMode = (PWM_MODE)uNum;
				break;
			case 1:
				DebugPrintf("\t0 : PWMOUT_CLEAR\r\n");
				DebugPrintf("\t1 : PWMOUT_SET\r\n");
				DebugPrintf("\t0 : PWMOUT_HOLD\r\n");
				DebugPrintf("\t1 : PWMOUT_TOGGLE\r\n");
				DebugPrintf("\nset matchout mode:");
				uNum = DebugGetNum();
				DebugPrintf("\n\n");
				uPwmPara.MatchMode = (PWM_MATCH_MODE)uNum;
				break;
			case 2:
				DebugPrintf("\nset period value:");
				uNum = DebugGetNum();
				DebugPrintf("\n\n");
				uPwmPara.Period = uNum;
				break;
			case 3:
				/*configure port for PWM*/
				PWM_SetPort(uPwmPara.PwmId);
				PWM_InitTimerMode(&uPwmPara);
				delay(1000);
				for(i=0;i<4;i++)
				{
					if(PWM_IntFlag[uPwmPara.PwmId-1][i]==1)
					{
						DebugPrintf("\tInt %d happen!\r\n");
						PWM_IntFlag[uPwmPara.PwmId-1][i] = 0;
					}
				}
				break;
			default:
				return;
		}
	}
}
	
/***********************************************************
*	Function	：	PWM_HGpioModeTest
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void PWM_HGpioModeTest(void)
{
	volatile U32 i, j, uSel, uNum;
	PWM_GPIO_CFG uPwmPara = {
		PWM_CH1,
		0,				//prescale
		PWM_OUT,
		0x0f,
		INT_MASK		//INT_mask
	};
	while(1)
	{
		DebugPrintf("\t0 : 选择High GPIO方向\r\n");
		DebugPrintf("\t1 : 运行\r\n");
		DebugPrintf("\t2 : return\r\n");
		DebugPrintf("\n\nSelect the serial num to test : ");
		uSel = DebugGetNum();
		DebugPrintf("\n\n");
		switch(uSel)
		{
			case 0:
				DebugPrintf("\t0 : PWM_OUT\r\n");
				DebugPrintf("\t1 : PWM_IN\r\n");			
				DebugPrintf("\nselect PWM DIR mode:");
				uNum = DebugGetNum();
				DebugPrintf("\n\n");
				uPwmPara.PwmDir = (PWM_DIR)uNum;
				break;
			case 1:
				/*configure port for PWM*/
				PWM_SetPort(uPwmPara.PwmId);
				PWM_InitHGpioMode(&uPwmPara);
				break;
			default:
				return;
		}
	}
}
/***********************************************************
*	Function	：	PWM_HGpioModeTest
*	Parameter	：	
*	Return		：  
*	Description	：	
***********************************************************/
void PWM_IntteruptTest(void)
{
	VIC_EnableInt(INTSRC_PWM);
}

const testFuncMenu PWM_Menu[] =
{
	{PWM_RegVerifyTest,			"PWM 寄存器验证"},
	{PWM_PWMModeTest,			"PWM PWM模式测试"},
	{PWM_TimerModeTest,			"PWM Timer模式测试"},
	{PWM_HGpioModeTest,			"PWM HGPIO模式测试"},
	{PWM_IntteruptTest,			"PWM 中断测试"}
};

//***********************************************************
//		Function	：	PWM_Test
//		Parameter	：	（无）
//		Description	：	PWM测试程序
//***********************************************************
void PWM_Test(void)
{
	U32 i,uSel,uMenuNum=(sizeof(PWM_Menu)/8);
	char temp[10] = "return";
	while(1)
	{
		DebugPrintf("\n\n");
		DebugPrintf("******************************************\n");
		DebugPrintf("*       PWM - Test firmware v0.1        *\n");
		DebugPrintf("******************************************\n");

		for (i=0; i<uMenuNum; i++)
		{
			DebugPrintf("\t%2d: %s  \r\n", i, PWM_Menu[i].desc);
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
			(PWM_Menu[uSel].func) ();
		else
			break;
	}
}

