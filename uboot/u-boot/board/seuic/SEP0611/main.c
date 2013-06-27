/*********************************************************************
* Filename    :  main.c
* Author      :  SixRoom
* Date        :  2011-01-12
* Description :  
* Version  	  :  v1.00
**********************************************************************/
/*
|---------------|------------------|---------------|-----------------------------------------|
|    version    |       date       |     Author    |           modify infomation             |
|---------------|------------------|---------------|-----------------------------------------|
|    v1.00      |    2011-01-12    |  SixRoom      | 1) create this file    			     |
|---------------|------------------|---------------|-----------------------------------------|
*/
#include  	<asm/arch/delay.h>
//#include  	<asm/arch/lcdc.h>
#include  	<asm/arch/uart.h>
#include  	<asm/arch/ssi.h>
//#include  	<asm/arch/dmac.h>
//#include  	<asm/arch/rtc.h>
//#include  	<asm/arch/timer.h>
#include  	<asm/arch/gpio.h>
#include  	<asm/arch/sdio.h>
#include  	<asm/arch/iis.h>
//#include  	<asm/arch/iic.h>
#include	<asm/arch/codec.h>
#include	<asm/arch/pwm.h>
//#include	<asm/arch/nfc.h>
//#include 	<asm/arch/sram.h"
//#include <asm/arch/TIMER.h>
#include <vic/vic.h>
#include <asm/arch/hardware.h>
#include	<vic/typedef.h>
#include <asm/u-boot-unicore.h>
const testFuncMenu menu[] =
{
	//GPIO_Test,				"KEY Test        ",
//	LCDC_Test,				"LCD Test        ",
	//SDIO_Test,				"SD/MMC Test     ",
//	NFC_Test,				"NAND Test       ",
//	DMAC_Test,				"DMAC Test       ",	
//	RTC_Test,				"RTC Test        ",
	//PWM_Test,				"PWM_Test        ",
	//Timer_Test,				"TIMER Test      ",
	//UART_Test,				"UART_Test       ",
//	I2C_Test,				"I2C Test        ",
	//SSI_Test,				"SSI Test        ",
	//IIS_Test,				"IIS Test        "	
};
int modules_test(void)
{
	printf("now start=============\n");
	/*
	U32 i,uSel,uMenuNum=(sizeof(menu)/8);
	DebugOpenConsole();
	while(1)
	{
		DebugPrintf("\n\n");
	
		DebugPrintf("***************************************************\n");
		DebugPrintf("*       SEP0611 - Test firmware v0.1              *\n");
		DebugPrintf("***************************************************\n\n");
		DebugPrintf("ARMCLK: %d MHz  HCLK: %d MHz  PCLK: %d MHz\r\n\r\n",SYSCLK/1000000,SYSCLK/1000000,SYSCLK/1000000);

		for (i=0; i<uMenuNum; i++)
		{
			DebugPrintf("%2d: %s  \r\n", i, menu[i].desc);
		}

		DebugPrintf("\n\nSelect the function to test : ");

		uSel = DebugGetNum();
		
		DebugPrintf("\n\n");
		DebugPrintf("Function Begin ... ... \n");
		if (uSel<(sizeof(menu)/8-1))
			(menu[uSel].func) ();
	}
	*/
}
