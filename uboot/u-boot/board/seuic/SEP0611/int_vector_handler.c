/*********************************************************************
* Filename    :  int_vector_handler.c
* Author      :  Phoenix Liu
* Date        :  2010-03-10
* Description :  This file is used to define the interrupt handler which
*                will be used to search the interrupt processing entry
*                interrupt request
* Version  	  :  v1.01
**********************************************************************/
/*
|--------------------------------------------------------|
|    interrupt vector handler initialization             |
|--------------------------------------------------------|
|  the int_vector_handler is a pointer array set         |
|  which contain 64 pointers, each pointer index         |
|  to a function that has no argument and return         |
|  value									             |
|--------------------------------------------------------|
|  How to use the handler :							     |
|--------------------------------------------------------|
|  1. import the ISR, format as follow					 |
|     extern void uart_isr_handler(void); 				 |
|  2. locate the interrupt num that will be verified     |
|     or used, replace the 'NULL' with the specified     |
|     ISR, such as										 |
|     replace the No.47 'NULL' with the uart_isr_handler |
|     when the uart0 interrupt occurs, the vic will 	 |
|     get the address of the uart_isr_handler and set	 |
|     pc to that address								 |
|--------------------------------------------------------|
*/

#include <stdio.h>
#include "../include/typedef.h"
#include "../include/vic.h"

/*extern declare function list here*/
extern void SSI1_IRQHandler (void);
extern void SSI2_IRQHandler (void);
extern void SSI3_IRQHandler (void);

extern void DMAC1_IsrHandler(void);
extern void DMAC2_IsrHandler(void);

extern void I2C1_IntHandler(void);
extern void I2C2_IntHandler(void);
extern void I2C3_IntHandler(void);

extern void RTC_IRQHandle(void);

extern void PWM_IRQHandler(void);

extern void NFC_IsrHandler(void);

void (*int_vector_handler[64])(void) = {
	NULL,             /*  0*/
	NULL,             /*  1*/
	NULL,             /*  2*/
	NULL,             /*  3*/
    NULL,             /*  4*/
    NULL,             /*  5*/
    NULL,             /*  6*/
    NULL,             /*  7*/
    NULL,             /*  8*/
    NULL,             /*  9*/
    NULL,             /* 10*/
    NULL,             /* 11*/
    NULL,             /* 12*/
    NULL,             /* 13*/
    NULL,             /* 14*/
    NULL,             /* 15*/
	NULL,             /* 16*/
	NULL,             /* 17*/
	NULL,             /* 18*/
	NULL,             /* 19*/
	NULL,             /* 20*/
	NULL,             /* 21*/
	NULL,             /* 22*/
	NULL,             /* 23*/
	NULL,             /* 24*/
	NULL,             /* 25*/
	NULL,             /* 26*/
	NULL,             /* 27*/
	NULL,             /* 28*/
	NULL,             /* 29*/
	NULL,             /* 30*/
	NULL,             /* 31*/
	NULL,             /* 32*/
	RTC_IRQHandle,             /* 33*/
	NULL,             /* 34*/
	NULL,             /* 35*/
	NULL,             /* 36*/
	DMAC1_IsrHandler,             /* 37*/
	DMAC2_IsrHandler,             /* 38*/
	NULL,             /* 39*/
	NULL,             /* 40*/
	PWM_IRQHandler,             /* 41*/
	NULL,             /* 42*/
	NULL,             /* 43*/
	NULL,             /* 44*/
	NULL,             /* 45*/
	NULL,             /* 46*/
	NULL,             /* 47*/
	SSI1_IRQHandler,             /* 48*/
	SSI2_IRQHandler,             /* 49*/
	SSI3_IRQHandler,             /* 50*/
	I2C1_IntHandler,             /* 51*/
	I2C2_IntHandler,             /* 52*/
	I2C3_IntHandler,             /* 53*/
	NULL,             /* 54*/
	NFC_IsrHandler,             /* 55*/
	NULL,             /* 56*/
	NULL,             /* 57*/
	NULL,             /* 58*/
	NULL,             /* 59*/
	NULL,             /* 60*/
	NULL,             /* 61*/
	NULL,             /* 62*/
	NULL              /* 63*/
};


