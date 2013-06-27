/*********************************************************************
* Filename    :  typedef.h
* Author      :  SixRoom
* Date        :  2011-02-24
* Description :  This file is used to define the interrupt source number
*                and some macros that will be used to processing the 
*                interrupt request
* Version  	  :  v1.06
**********************************************************************/
/*
|---------------|------------------|---------------|-----------------------------------------|
|    version    |       date       |     Author    |           modify infomation             |
|---------------|------------------|---------------|-----------------------------------------|
|    v1.00      |    2010-02-25    |  Phoenix Liu  | 1) create this typedef file    		 |
|    v1.01      |    2011-01-10    |  SixRoom      | 1) add new type for every model	     |
|				|				   |			   | 2) modify some type                     |
|    v1.02      |    2011-01-11    |  SixRoom      | 1) add some header file for debug	     |
|				|				   |			   | 2) update and add some type             |
|    v1.03      |    2011-01-12    |  SixRoom      | 1) add some new type				     |
|    v1.04      |    2011-01-14    |  SixRoom      | 1) modify RTC an SSI					 |
|    v1.05      |    2011-01-29    |  SixRoom      | 1) modify DMAC and SDIO				 |
|    v1.06      |    2011-02-24    |  SixRoom      | 1) modify IIC、PMU and NOR				 |
|---------------|------------------|---------------|-----------------------------------------|
*/
#ifndef __TYPEDEF_H_
#define __TYPEDEF_H_
#include <common.h>
#include <command.h>
/*


#include    <stdio.h>
#include    <string.h>
#include    <math.h>
#include 	<stdlib.h>
#include 	<stdarg.h>
#include 	<ctype.h>
#include	<time.h>

typedef enum {FPGA_V1=1, FPGA_V2=2, FPGA_V3=3, FPGA_V4=4, FPGA_V5=5} FPGA_VER;
typedef enum {BRANCH1=1, BRANCH2=2, BRANCH3=3} BRANCH_ID;

#define 	RTL_V(x)					(x)

#define SYSCLK  50000000
*/
/******************************************  
* new General-Purpose Data Types
*******************************************/
#define TYPE(x,y)				x##y

typedef char            		S8;      /* signed 8-bit integer */
typedef short           		S16;     /* signed 16-bit integer */
typedef long            		S32;     /* signed 32-bit integer */
typedef unsigned char   	  	U8;      /* unsigned 8-bit integer */
typedef unsigned short  	  	U16;     /* unsigned 16-bit integer */
typedef unsigned long   	  	U32;     /* unsigned 32-bit integer */
typedef unsigned long long 	  	U64;     /* unsigned 32-bit integer */

typedef volatile U64 *		  	RP64;
typedef volatile U32 *		  	RP32;
typedef volatile U32 *		  	RP;
typedef	volatile U16 *		  	RP16;
typedef	volatile U8  *		  	RP8;

typedef void            		*VP;     /* pointer to an unpredictable data type */
typedef void            		(*FP)(); /* program start address */

typedef int             		ER;     /* Error code.  A signed integer. */

typedef U32						STATUS;

#define NULL					0

/*a new verify function description type*/
typedef struct {
	void (*func)(void); 
	const char *desc;
} testFuncMenu;

typedef enum STATUS {FALSE=0, TRUE=1} BOOL;

#define    ESRAM_BEGIN          0x32000000                 //esram begin address
#define    ESRAM_END            0x32018000                 //esram end address
#define    DDR2_BEGIN           0x40000000                 //ddr2 begin address
#define    DDR2_END             0x50000000                 //ddr2 end address

/*******************************************************************
*  Error Codes               
*           IF SUCCESS RETURN 0, ELSE RETURN OTHER ERROR CODE,
*           parameter error return (-33)/E_PAR, hardware error reture (-99)/E_HA
********************************************************************/
#define 	E_OK       	0        /* Normal completion */
#define 	E_SYS      	(-5)     /* System error */
#define 	E_NOMEM    	(-10)    /* Insufficient memory */
#define 	E_NOSPT    	(-17)    /* Feature not supported */
#define 	E_INOSPT  	(-18)    /* Feature not supported 
                               		by ITRON/FILE specification */
#define 	E_RSFN     	(-20)    /* Reserved function code number */
#define 	E_RSATR    	(-24)    /* Reserved attribute */
#define 	E_PAR      	(-33)    /* Parameter error */
#define 	E_ID       	(-35)    /* Invalid ID number */
#define 	E_NOEXS    	(-52)    /* Object does not exist */
#define 	E_OBJ      	(-63)    /* Invalid object state */
#define 	E_MACV     	(-65)    /* Memory access disabled or memory access 
                               		violation */
#define 	E_OACV     	(-66)    /* Object access violation */
#define 	E_CTX      	(-69)    /* Context error */
#define 	E_QOVR     	(-73)    /* Queuing or nesting overflow */
#define 	E_DLT      	(-81)    /* Object being waited for was deleted */
#define 	E_TMOUT    	(-85)    /* Polling failure or timeout exceeded */
#define 	E_RLWAI    	(-86)    /* WAIT state was forcibly released */ 

#define	    E_HA		(-99)    // HARD WARE ERROR

/*******************************************************************           
*  define the Common function 
********************************************************************/
#define    write_reg(reg, data) 		*(RP32)(reg) = (data)
#define    read_reg(reg) 				*(RP32)(reg)
#define    orr_reg(addr, data)			*(RP32)(addr) |= (data)
#define    and_reg(addr, data)			*(RP32)(addr) &= (data)
/*向一个寄存器中的连续某几位写值,其它位保持不变*/
#define    write_Nbit(addr,bits,value)	*(RP32)(addr) &= ~(bits);*(RP32)(addr) |= (value)

#define delay(j)                                 \
{                                                \
  volatile U32 i;                                \
  for(i=0;i<j;i++){};                            \
}

/*******************************************************************           
*  define the new type for every model
********************************************************************/

	
#endif
//endif
