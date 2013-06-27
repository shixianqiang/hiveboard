/*********************************************************************
* Filename    :  vic.h
* Author      :  SixRoom
* Date        :  2011-01-13 
* Description :  This file is used to define the interrupt source number
*                and some macros that will be used to processing the 
*                interrupt request
* Version  	  :  v1.04
**********************************************************************/
/*
|---------------|------------------|---------------|-----------------------------------------|
|    version    |       date       |     Author    |           modify infomation             |
|---------------|------------------|---------------|-----------------------------------------|
|    v1.00      |    2010-02-25    |  Phoenix Liu  | 1) create this header file    			 |
|				|				   |			   |    fpga_ddr2v1 ~ fpga_top_v2_r12	     |
|    v1.01      |    2010-03-08    |  Phoenix Liu  | 1) modify the interrupt source number 	 |
|				|				   |			   |    accroding to the r36 rtl code        |
|				|				   |			   |    fpga_top_v3_r1 ~ fpga_top_v3_r7      |
|    v1.03      |    2010-05-21    |  Phoenix Liu  |  	0718_b1_r26  ~ 					     |
|    v1.03      |    2011-01-13    |  SixRoom	   | 1) modify some function	             |
|---------------|------------------|---------------|-----------------------------------------|
*/

#ifndef __VIC_H_
#define __VIC_H_

#include	<asm/arch/typedef.h>
#include 	<asm/arch/hardware.h>
#include	<asm/arch/globalpara.h>
#include	<asm/arch/mem.h>

#define    IRQ    0
#define    FIQ    1

/* define the interrupt source number */
#define    INTSRC_PRESERVE2     63
#define    INTSRC_PRESERVE1     62
#define    INTSRC_LCDC          61
#define    INTSRC_GPU           60
#define    INTSRC_VPU           59
#define    INTSRC_TIMER3        58
#define    INTSRC_TIMER2        57
#define    INTSRC_TIMER1        56
#define    INTSRC_NAND          55
#define    INTSRC_I2S           54
#define    INTSRC_I2C3          53
#define    INTSRC_I2C2          52
#define    INTSRC_I2C1          51
#define    INTSRC_SSI3          50
#define    INTSRC_SSI2          49
#define    INTSRC_SSI1          48
#define    INTSRC_SDIO2         47
#define    INTSRC_SDIO1         46
#define    INTSRC_UART3         45
#define    INTSRC_UART2         44
#define    INTSRC_UART1         43
#define    INTSRC_UART0         42
#define    INTSRC_PWM           41
#define    INTSRC_USB           40
#define    INTSRC_USBDMA        39
#define    INTSRC_DMAC2         38
#define    INTSRC_DMAC1         37
#define    INTSRC_PMUIRQ_A11    36
#define    INTSRC_DMAIRQ_A11    35
#define    INTSRC_PMU           34
#define    INTSRC_RTC           33
#define    INTSRC_WATCHDOG      32
#define    INTSRC_PORTE12       31
#define    INTSRC_PORTE11       30
#define    INTSRC_PORTE10       29
#define    INTSRC_PORTE9        28
#define    INTSRC_PORTE5        27
#define    INTSRC_PORTE4        26
#define    INTSRC_PORTD9        25
#define    INTSRC_PORTD8        24
#define    INTSRC_PORTD3        23
#define    INTSRC_PORTD2        22
#define    INTSRC_PORTD1        21
#define    INTSRC_PORTD0        20
#define    INTSRC_PORTC3        19
#define    INTSRC_PORTC2        18
#define    INTSRC_PORTC1        17
#define    INTSRC_PORTC0        16
#define    INTSRC_EXT15         15
#define    INTSRC_EXT14         14
#define    INTSRC_EXT13         13
#define    INTSRC_EXT12         12
#define    INTSRC_EXT11         11
#define    INTSRC_EXT10         10
#define    INTSRC_EXT9          9
#define    INTSRC_EXT8          8
#define    INTSRC_EXT7          7
#define    INTSRC_EXT6          6
#define    INTSRC_EXT5          5
#define    INTSRC_EXT4          4
#define    INTSRC_AO_EXT3       3
#define    INTSRC_AO_EXT2       2
#define    INTSRC_AO_EXT1       1
#define    INTSRC_AO_EXT0       0

/*               vic based macros               */

/***********************************************************
*	Function	£º	VIC_SetPriorMask
*	Parameter	£º	
*	Return		£º  
*	Description	£º	
***********************************************************/
#define VIC_SetPriorMask(plevel)          \
    *(RP32)VIC_PROIRTY_MASK = plevel
/***********************************************************
*	Function	£º	VIC_SetVectPrior
*	Parameter	£º	
*	Return		£º  
*	Description	£º	
***********************************************************/
#define VIC_SetVectPrior(num, plevel) \
    *(RP32)(VIC_VECTOR_PROIRTY00 + (num << 2)) = plevel
/***********************************************************
*	Function	£º	VIC_SetIntForce
*	Parameter	£º	
*	Return		£º  
*	Description	£º	
***********************************************************/
#define VIC_SetIntForce(intnum)                         \
do{                                                   \
      if(intnum < 32)                                 \
          *(RP32)VIC_SFT_INT_L = (1 << intnum);       \
      else                                            \
          *(RP32)VIC_SFT_INT_H = (1 << (intnum - 32));\
  }while(0)                                           
/***********************************************************
*	Function	£º	VIC_ClrIntForce
*	Parameter	£º	
*	Return		£º  
*	Description	£º	
***********************************************************/
#define VIC_ClrIntForce(intnum)                           \
do{                                                     \
      if(intnum < 32)                                   \
          *(RP32)VIC_SFT_INT_L &= ~(1 << intnum);         \
      else                                              \
          *(RP32)VIC_SFT_INT_H &= ~(1 << (intnum - 32));  \
  }while(0)                                           
/***********************************************************
*	Function	£º	VIC_EnableInt
*	Parameter	£º	
*	Return		£º  
*	Description	£º	
***********************************************************/
#define VIC_EnableInt(intnum)                            \
do{                                                   \
      if(intnum < 32)                                 \
          *(RP32)VIC_INT_EN_L |= (1 << intnum);          \
      else                                            \
          *(RP32)VIC_INT_EN_H |= (1 << (intnum - 32));   \
  }while(0)                                           
/***********************************************************
*	Function	£º	VIC_DisableInt
*	Parameter	£º	
*	Return		£º  
*	Description	£º	
***********************************************************/
#define VIC_DisableInt(intnum)                           \
do{                                                   \
      if(intnum < 32)                                 \
          *(RP32)VIC_INT_EN_L &= ~(1 << intnum);        \
      else                                            \
          *(RP32)VIC_INT_EN_H &= ~(1 << (intnum - 32)); \
  }while(0)                                           
/***********************************************************
*	Function	£º	VIC_ClrInt
*	Parameter	£º	
*	Return		£º  
*	Description	£º	
***********************************************************/
#define VIC_ClrInt(intnum)                        \
do{                                                   \
      if(intnum < 32)                                 \
          *(RP32)VIC_INT_EN_CLR_L = (1 << intnum);      \
      else                                            \
          *(RP32)VIC_INT_EN_CLR_H = (1 << intnum - 32); \
  }while(0)
/***********************************************************
*	Function	£º	VIC_EnableAllInt
*	Parameter	£º	
*	Return		£º  
*	Description	£º	
***********************************************************/
#define VIC_EnableAllInt()                              \
do{                                                   \
      *(RP32)VIC_INT_EN_L = 0xffffffff;                 \
      *(RP32)VIC_INT_EN_H = 0xffffffff;                 \
  }while(0)
/***********************************************************
*	Function	£º	VIC_DisableAllInt
*	Parameter	£º	
*	Return		£º  
*	Description	£º	
***********************************************************/
#define VIC_DisableAllInt()                             \
do{                                                   \
      *(RP32)VIC_INT_EN_L = 0x0;                        \
      *(RP32)VIC_INT_EN_H = 0x0;                        \
  }while(0)
/***********************************************************
*	Function	£º	VIC_MaskAllInt
*	Parameter	£º	
*	Return		£º  
*	Description	£º	
***********************************************************/
#define VIC_MaskAllInt(int_type)                        \
do{                                                   \
      if(int_type == IRQ)                             \
          *(RP32)VIC_INT_MSK_ALL = 0x1;                 \
      else if(int_type == FIQ)                        \
          *(RP32)VIC_INT_MSK_ALL = 0x2;                 \
  }while(0)                                           
/***********************************************************
*	Function	£º	VIC_UnmaskAllInt
*	Parameter	£º	
*	Return		£º  
*	Description	£º	
***********************************************************/
#define VIC_UnmaskAllInt(int_type)                      \
do{                                                   \
      if(int_type == IRQ)                             \
          *(RP32)VIC_INT_MSK_ALL &= 0xfffffffe;        \
      else if(int_type == FIQ)                        \
          *(RP32)VIC_INT_MSK_ALL &= 0xfffffffd;        \
  }while(0)

/* declaration of function */
extern void (*int_vector_handler[64])(void);

#endif

