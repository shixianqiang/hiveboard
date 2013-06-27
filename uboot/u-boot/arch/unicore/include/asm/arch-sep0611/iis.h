/******************************************************************
*File name   :  "i2s.h"
*Author      :  Yanghq
*Date        :  2011-1-19
*Description :  The code is wrote to define the I2S's registers , functions and so on.
*Version     :  1.05
*******************************************************************/
/*
|---------------|------------------|---------------|------------------------------------------|
|    version    |       date       |     Author    |           modify infomation              |
|---------------|------------------|---------------|------------------------------------------|
|    v1.00      |    2010-05-19    |  Yanghq       | 1) create this file   		              |
|    v1.01      |    2010-06-27    |  Yanghq       | 1) modify the coding style	              |
|				|				   |			   | 2) modify the interrupt handler function |                  
|    v1.02      |    2010-06-28    |  Yanghq       | 1) add the master mode of iis	          |
|				|				   |			   | 2) combine the master mode and slave     |
|               |                  |               |    mode in one project                   |
|    v1.03      |    2010-06-29    |  Yanghq       | 1) modify the dma transfer				  |
|    v1.04      |    2010-07-27    |  Yanghq       | 1) modify the coding style			      |
|    v1.05      |    2011-01-19    |  Yanghq       | 1) unify the coding style with workmates |
|---------------|------------------|---------------|------------------------------------------|
*/
#ifndef IIS_H_
#define IIS_H_

#include 	<asm/arch/vic.h>

/*To distinguish the registers in one expression*/
#define    I2S_LRBR(x)    x==0?I2S_LRBR0:(x==1?I2S_LRBR1:(x==2?I2S_LRBR2:I2S_LRBR3))
#define    I2S_LTHR(x)    x==0?I2S_LTHR0:(x==1?I2S_LTHR1:(x==2?I2S_LTHR2:I2S_LTHR3))
#define    I2S_RRBR(x)    x==0?I2S_RRBR0:(x==1?I2S_RRBR1:(x==2?I2S_RRBR2:I2S_RRBR3))
#define    I2S_RTHR(x)    x==0?I2S_RTHR0:(x==1?I2S_RTHR1:(x==2?I2S_RTHR2:I2S_RTHR3))
#define    I2S_RER(x)     x==0?I2S_RER0 :(x==1?I2S_RER1 :(x==2?I2S_RER2 :I2S_RER3 ))
#define    I2S_TER(x)     x==0?I2S_TER0 :(x==1?I2S_TER1 :(x==2?I2S_TER2 :I2S_TER3 ))
#define    I2S_RCR(x)     x==0?I2S_RCR0 :(x==1?I2S_RCR1 :(x==2?I2S_RCR2 :I2S_RCR3 ))
#define    I2S_TCR(x)     x==0?I2S_TCR0 :(x==1?I2S_TCR1 :(x==2?I2S_TCR2 :I2S_TCR3 ))
#define    I2S_ISR(x)     x==0?I2S_ISR0 :(x==1?I2S_ISR1 :(x==2?I2S_ISR2 :I2S_ISR3 ))
#define    I2S_IMR(x)     x==0?I2S_IMR0 :(x==1?I2S_IMR1 :(x==2?I2S_IMR2 :I2S_IMR3 ))
#define    I2S_ROR(x)     x==0?I2S_ROR0 :(x==1?I2S_ROR1 :(x==2?I2S_ROR2 :I2S_ROR3 ))
#define    I2S_TOR(x)     x==0?I2S_TOR0 :(x==1?I2S_TOR1 :(x==2?I2S_TOR2 :I2S_TOR3 ))
#define    I2S_RFCR(x)    x==0?I2S_RFCR0:(x==1?I2S_RFCR1:(x==2?I2S_RFCR2:I2S_RFCR3))
#define    I2S_TFCR(x)    x==0?I2S_TFCR0:(x==1?I2S_TFCR1:(x==2?I2S_TFCR2:I2S_TFCR3))
#define    I2S_RFF(x)     x==0?I2S_RFF0 :(x==1?I2S_RFF1 :(x==2?I2S_RFF2 :I2S_RFF3 ))
#define    I2S_TFF(x)     x==0?I2S_TFF0 :(x==1?I2S_TFF1 :(x==2?I2S_TFF2 :I2S_TFF3 ))


/*enumerate the i2s data interrupt type*/
#define RX_AVALIABLE_INT		1
#define RX_OVERRUN_INT			(1<<1)
#define TX_EMPTY_INT			(1<<4)
#define TX_OVERRUN_INT			(1<<5)
		
#define IIS_OT_ID			IIS_CH0
#define IIS_OT_HSCLK		MASTER_CLK
#define IIS_OT_CLKSRC		HARDWARE
#define IIS_OT_DATALEN		BIT16
#define IIS_OT_SAMPLERATE	0
#define IIS_OT_SAMPLEATTRI	SAMPLE48_LENGTH16
#define IIS_OT_WSLEN		WS_SCLK16
#define IIS_OT_GCLEN		GC_SCLK0
#define IIS_OT_DATADIR		IIS_OUT
#define IIS_OT_DMAEN		TRUE

/*base functions*/


/*module functions*/


/*verification functions*/
extern void I2S_VerifyReg(void) ;
extern void I2S_ReadandWriteReg(void);
extern void I2S_DmaPlayback(void);              	                               
extern void I2S_DmaLinkedlistPlayback(void);
extern void I2S_DmaAutoreloadPlayback(void);  
extern void I2S_DmaRecord(void);
extern void I2S_CpuPlayback(void);
extern void I2S_CpuRecord(void);
extern void I2S_CpuFDX(void);
extern void I2S_DmaFDX(void);

void IIS_Test(void);

#endif /*IIS_H_*/
