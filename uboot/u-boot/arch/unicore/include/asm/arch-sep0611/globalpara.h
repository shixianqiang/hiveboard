/*********************************************************************
* Filename    :  globalpara.h
* Author      :  SixRoom
* Date        :  2011-01-10
* Description :  This file is used to leave with common define
* Version  	  :  v1.00
**********************************************************************/
#ifndef GLOBALPARA_H_
#define GLOBALPARA_H_

#include <asm/arch/typedef.h>

//<首页>
#define SYS_CLK  		50000000
#define EXT_BOARD  		FPGA_V4
#define BRANCH_IMG  	BRANCH1

	#define GPIO_EN		0
	#define VIC_EN		0
	#define PMU_EN		0
	#define RTC_EN		0
	#define TIMER_EN	0
	#define PWM_EN		0
	#define UART_EN		0
	#define IIC_EN		0
	#define SSI_EN		0
	#define IIS_EN		0
	#define DMA_EN		0
	#define LCD_EN		1
	#define SDIO_EN		0
	#define NFC_EN		0
	#define SRAM_EN		0
	#define HDMI_EN		0
//</首页>

//<GPIO>
#if GPIO_EN
	#define KEY_PIN	
	#define TIMER_PIN
	#define PWM_PIN_OUT1
	#define PWM_PIN_IN1
	#define PWM_PIN_OUT2
	#define PWM_PIN_IN2
	#define LED_PIN
	#define UART0_PIN
	#define UART1_PIN
	#define UART2_PIN
	#define UART3_PIN
	#define IIC1_PIN
	#define IIC2_PIN
	#define IIC3_PIN
	#define SSI1_PIN
	#define SSI2_PIN
	#define SSI3_PIN
	#define LCD_PIN_EN
	#define LCD_PIN_BL_EN
	#define SD1_PIN
	#define SD2_PIN
	#define NAND_PIN_nCS0
	#define NAND_PIN_nCS1
	#define NAND_PIN_nCS2
	#define NAND_PIN_nCS3
	#define NAND_PIN_nFRE
	#define NAND_PIN_nFWE
	#define NAND_PIN_FCLE
	#define NAND_PIN_FALE
	#define NAND_PIN_R_B0
	#define NAND_PIN_R_B1
	#define NAND_PIN_R_B2
	#define NAND_PIN_R_B3
	#define SRAM_PIN_nCS0
	#define SRAM_PIN_nCS1
	#define SRAM_PIN_nWE
	#define SRAM_PIN_nRE
	#define SRAM_PIN_nOE
	#define SRAM_PIN_nBE0
	#define SRAM_PIN_nBE1
	#define DM9000_PIN_INT
	#define DDRII_PIN
	#define HDMI_PIN				IIC1_PIN
	#define HDMI_PIN_INT
	#define HDMI_PIN_RST
	#define CODEC_PIN				IIC1_PIN
	#define GPS_PIN
#endif
//</GPIO>

//<VIC>
#if VIC_EN
	#define RTL_VERSION 			RTL_V(91)
#endif
//</VIC>

//<PMU>
#if PMU_EN
	#define OPERATE_MODE 			NORMAL
#endif
//</PMU>

//<RTC>
#if RTC_EN

#endif
//</RTC>

//<TIMER>
#if TIMER_EN

#endif
//</TIMER>

//<PWM>
#if PWM_EN

#endif
//</PWM>

//<UART>
#if UART_EN

#endif
//</UART>

//<IIC>
#if IIC_EN

#endif
//</IIC>

//<SSI>
#if SSI_EN

#endif
//</SSI>

//<IIS>
#if IIS_EN

#endif
//</IIS>

//<DMA>
#if DMA_EN

#endif
//</DMA>

//<LCD>
#if LCD_EN
	#define		LCD_ASYNC_CLK	75000000

	//面板输出极性 配置参数
	#define		PIXPOL			(0 << 12)
	#define		FLMPOL			(1 << 11)
	#define		LPPOL			(1 << 10)
	#define		CLKPOL			(0 << 9)
	#define		OEPOL			(0 << 8)
	
	//Macro for Panel line control register(LCDC_PLCR)
	#define		H_WIDTH			(48 << 26)
	#define		H_WAIT1			(40 << 8)
	#define		H_WAIT2			(40)
	
	//Macro for Panel frame control register(LCDC_PFCR)
	#define		V_WIDTH			(3 << 26)
	#define		V_WAIT1			(29 << 8)
	#define		V_WAIT2			(13)

#endif
//</LCD>

//<SDIO>
#if SDIO_EN

#endif
//</SDIO>

//<NFC>
#if NFC_EN

#endif
//</NFC>

//<SRAM>
#if SRAM_EN

#endif
//</SRAM>

//<HDMI>
#if HDMI_EN

#endif
//</HDMI>

#endif /*GLOBALPARA_H_*/
