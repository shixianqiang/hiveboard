/******************************************************************
*File name   :  "iic.h"
*Author      :  SixRoom
*Date        :  2011-02-15
*Description :  The code is wrote to configure the I2C.
*Version     :  1.03
*******************************************************************/
#ifndef IIC_H_
#define IIC_H_

#include	<asm/arch/vic.h>

#define IIC_CLK 50

//there is no defined I@C_TAR_CH in SEP0718v2_REG.h.
#define   I2C_TAR_CH(x)    (I2C_BASE_CH(x) + 0x004)

#define CODEC_ADDR       0x1A
#define I2C_TEST_ADDR    0x54
#define I2C_ADDR         0x254
#define I2C_EEPROM_ADDR  0x57
#define I2C_HDMI_ADDR    0x90
  
#define GEN_CALL_INT   (1<<11)
#define START_DET_INT  (1<<10)
#define STOP_DET_INT   (1<<9)
#define ACTIVITY_INT   (1<<8)
#define RX_DONE_INT    (1<<7)
#define TX_ABRT_INT    (1<<6)
#define RD_REQ_INT     (1<<5)
#define TX_EMPTY_INT   (1<<4)
#define TX_OVER_INT    (1<<3)
#define RX_FULL_INT    (1<<2)
#define RX_OVER_INT    (1<<1)
#define RX_UNDER_INT   (1<<0)

#define IIC_OT_ID		IIC_ID1
#define IIC_OT_ADDRLEN	ADDR_7BIT
#define IIC_OT_MS		MASTER
#define IIC_OT_MCode	0
#define IIC_OT_SAddr	0
#define IIC_OT_BCCall	0
#define IIC_OT_Restart	MODE_START

#define IIC_OT_SS_HCNT		(4 * 4)
#define IIC_OT_SS_LCNT		(4.7 * 4)

#define IIC_OT_FS_HCNT		0.6
#define IIC_OT_FS_LCNT		1.3

#define IIC_OT_HS_HCNT		0.12
#define IIC_OT_HS_LCNT		0.32

//extern volatile U32 Intsign;
extern void I2C_RxUnderIntr(IIC_ID pId);
extern void I2C_RxOverIntr(IIC_ID pId);
extern void I2C_RxFullInt(IIC_ID pId);
extern void I2C_TxOverIntr(IIC_ID pId);
extern void I2C_TxEmptyIntr(IIC_ID pId);
extern void I2C_RxReqIntr(IIC_ID pId);
extern void I2C_TxAbrtIntr(IIC_ID pId);
extern void I2C_RxDoneIntr(IIC_ID pId);
extern void I2C_ActivityIntr(IIC_ID pId);
extern void I2C_StopDetIntr(IIC_ID pId);
extern void I2C_StartDetIntr(IIC_ID pId);
extern void I2C_GenCallIntr(IIC_ID pId);

void I2C_InitCodec(void);
void I2C_WriteCodecReg(U32 pRegAddr, U32 pCfgData);

void I2C_Test(void);

#endif /*IIC_H_*/
