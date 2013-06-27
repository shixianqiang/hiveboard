#ifndef __TYPEDEF_H_
#define __TYPEDEF_H_

/******************************************  
* General-Purpose Data Types
*******************************************/
typedef char            		S8;      /* signed 8-bit integer */
typedef short           		S16;      /* signed 16-bit integer */
typedef long            		S32;      /* signed 32-bit integer */
typedef unsigned char   	  U8;     /* unsigned 8-bit integer */
typedef unsigned short  	  U16;     /* unsigned 16-bit integer */
typedef unsigned long   	  U32;     /* unsigned 32-bit integer */
typedef unsigned long long    U64;

typedef	volatile U32 *		  RP;
typedef	volatile U16 *		  RP16;
typedef	volatile U8  *		  RP8;

#ifndef _BOOL_TYPE_
#define _BOOL_TYPE_
typedef int             		BOOL;	/* Boolean value.  TRUE (1) or FALSE (0). */
#endif  

typedef int             		ER;     /* Error code.  A signed integer. */


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

#define	  E_HA		    (-99)    // HARD WARE ERROR

/****************************************
	macros  for write and read registers
****************************************/
#define write_reg(reg, data) \
	*(RP)(reg) = data

#define read_reg(reg) \
	*(RP)(reg)
////////////////////////////////////////////////
#define delay(j)                                 \
{                                                \
  U16 i;                                         \
  for(i=0;i<j;i++){};                            \
}

////////////////////////////////////////////////
#endif

