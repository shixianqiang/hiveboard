/*********************************************************************
* Filename    :  vic.h
* Description :  This file is used to define the interrupt source number
*                and some macros that will be used to processing the 
*                interrupt request
* Date        :  2010-05-21
* Author      :  Phoenix Liu
* Current Ver :  v1.01
*
**********************************************************************/
//
// -------------------------------------------------------------------
//                        version log                                
//   version      date         Author              modify infomation
//    v1.00    2010-02-25   Phoenix Liu   1) create this header file
//    v1.01    2010-03-08   Phoenix Liu   1) modify the interrupt source number 
//                                           accroding to the r36 rtl code
//    v1.02    2010-05-08   Phoenix Liu   1) modify the interrupt source number
//                                           accroding to the r46 rtl code
//    v1.03    2010-09-02   Phoenix Liu   1) modify the interrupt source number
//                                           accroding to the r78 rtl code
// ---------------------------------------------------------------------
//                the usage of the header file for fpga version 
//   headerfile version                     corresponding fpga version
//          v1.00                            fpga_ddr2v1 ~ fpga_top_v2_r12
//          v1.01                         fpga_top_v3_r1 ~ fpga_top_v3_r7
//          v1.02                       0718(0611)_b1_r1 ~ 

#ifndef __VIC_H_
#define __VIC_H_

#include "typedef.h"

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
#define set_vic_plevel(plevel)          \
    *(RP)VIC_PROIRTY_MASK = plevel

#define set_vector_priority(num, plevel) \
    *(RP)(VIC_VECTOR_PROIRTY00 + (num << 2)) = plevel

#define set_int_force(intnum)                         \
do{                                                   \
      if(intnum < 32)                                 \
          *(RP)VIC_SFT_INT_L |= (1 << intnum);         \
      else                                            \
          *(RP)VIC_SFT_INT_H |= (1 << (intnum - 32));  \
  }while(0)                                           

#define clr_int_force(intnum)                              \
do{                                                        \
      if(intnum < 32)                                      \
          *(RP)VIC_SFT_INT_CLR_L &= ~(1 << intnum);        \
      else                                                 \
          *(RP)VIC_SFT_INT_CLR_H &= ~(1 << (intnum - 32)); \
  }while(0)                                           

#define int_enable(intnum)                            \
do{                                                   \
      if(intnum < 32)                                 \
          *(RP)VIC_INT_EN_L |= (1 << intnum);          \
      else                                            \
          *(RP)VIC_INT_EN_H |= (1 << (intnum - 32));   \
  }while(0)                                           

#define int_disable(intnum)                           \
do{                                                   \
      if(intnum < 32)                                 \
          *(RP)VIC_INT_EN_L &= ~(1 << intnum);        \
      else                                            \
          *(RP)VIC_INT_EN_H &= ~(1 << (intnum - 32)); \
  }while(0)                                           

#define int_enable_clr(intnum)                        \
do{                                                   \
      if(intnum < 32)                                 \
          *(RP)VIC_INT_EN_CLR_L = (1 << intnum);      \
      else                                            \
          *(RP)VIC_INT_EN_CLR_H = (1 << intnum - 32); \
  }while(0)

#define int_enable_all()                              \
do{                                                   \
      *(RP)VIC_INT_EN_L = 0xffffffff;                 \
      *(RP)VIC_INT_EN_H = 0xffffffff;                 \
  }while(0)

#define int_disable_all()                             \
do{                                                   \
      *(RP)VIC_INT_EN_L = 0x0;                        \
      *(RP)VIC_INT_EN_H = 0x0;                        \
  }while(0)

#define mask_all_int(int_type)                        \
do{                                                   \
      if(int_type == IRQ)                             \
          *(RP)VIC_INT_MSK_ALL = 0x1;                 \
      else if(int_type == FIQ)                        \
          *(RP)VIC_INT_MSK_ALL = 0x2;                 \
  }while(0)                                           

#define unmask_all_int(int_type)                      \
do{                                                   \
      if(int_type == IRQ)                             \
          *(RP)VIC_INT_MSK_ALL &= 0xfffffffe;        \
      else if(int_type == FIQ)                        \
          *(RP)VIC_INT_MSK_ALL &= 0xfffffffd;        \
  }while(0)

/* declaration of function */
//extern void (*int_vector_handler[64])(void);

extern void vic_reset_value_check(void);
extern void vic_rw_value_check(void);
extern void vic_status_clear_register_check(void);
extern void vic_int_type_select(U32 type_h ,U32 type_l);
extern void vic_soft_int_check1(void);
extern void vic_soft_int_check2(void);
extern void vic_priority(void);
extern void vic_soft_int_check3(void);
extern void vic_soft_int_nested(void);
extern void vic_fiq_vs_irq_verify(void);
extern void vic_soft_int_reentry(void);
extern void vic_irq_int_handler(void);
extern void vic_fiq_int_handler(void);
extern void vic_single_interrupt(void);
extern void vic_whole_interrupt(void);

/* definition of vector struct for interrrupt */
typedef struct int_vector{
	  U8		IntNum;
	  void	(*handler)(void);
}INT_VECTOR;

/* declaration of function */
extern void (*int_vector_handler[64])(void);

#endif

