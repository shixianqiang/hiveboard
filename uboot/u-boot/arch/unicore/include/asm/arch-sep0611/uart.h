/*********************************************************************
* Filename    :  uart.h
* Author      :  SixRoom
* Date        :  2011-01-10
* Description :  This file is used to define Uart
* Version  	  :  v1.00
**********************************************************************/
#ifndef UART_H_
#define UART_H_

#include 	<asm/arch/vic.h>
 
//ÖÐ¶ÏÊ¹ÄÜ	
#define MODEM_INT		8
#define RX_STA_INT		4
#define TX_FIFO_INT		2
#define RX_FIFO_INT		1
#define NONE_INT 		0  

#define UART_DB_BAUD	B9600
#define UART_DB_DATALEN	DATA_LEN8
#define UART_DB_STOPBIT	STOP_BIT1
#define UART_DB_VERIFY	NOVERIFY

#define UART_OT_ID		UART_ID0
#define UART_OT_MODE	COMMON_TRAN
#define UART_OT_BAUD	B9600
#define UART_OT_DATALEN	DATA_LEN8
#define UART_OT_STOPBIT	STOP_BIT1
#define UART_OT_VERIFY	NOVERIFY


void DebugOpenConsole(void);
void DebugClrConsoleScreen(void);
void DebugWaitForKey(void);
U32  DebugGetNum(void);
void DebugPrintf(const char *fmt,...);

void UART_Test(void);
#endif /*UART_H_*/
