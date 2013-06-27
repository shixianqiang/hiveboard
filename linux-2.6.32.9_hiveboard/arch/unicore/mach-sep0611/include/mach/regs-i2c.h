/* linux/arch/unicore/mach-sep0611/include/mach/regs-i2c.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *  cgm <chenguangming@wxseuic.com>
 *
 * Southeast University ASIC SoC support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  05-31-2011	cgm initial version
 *
 */

#ifndef __ASM_ARCH_REGS_I2C_H
#define __ASM_ARCH_REGS_I2C_H 

#define STATUS_IDLE					0x0
#define STATUS_WRITE				0x1
#define STATUS_READ					0x2

#define I2C_ERR_TX_ABRT 			0x1

/*more details see sep0611 datasheet*/
#define I2C_CON                        	(0x00)
#define I2C_TAR                        	(0x04)
#define I2C_SLA                        	(0x08)
#define I2C_HS_MASDDR                  	(0x0c)
#define I2C_DATA_CMD                   	(0x10)
#define I2C_SS_SCL_HCNT                	(0x14)
#define I2C_SS_SCL_LCNT                	(0x18)
#define I2C_FS_SCL_HCNT                	(0x1c)
#define I2C_FS_SCL_LCNT                	(0x20)
#define I2C_HS_SCL_HCNT                	(0x24)
#define I2C_HS_SCL_LCNT             	(0x28)
#define I2C_INTR_STAT                  	(0x2c)
#define I2C_INTR_MASK                  	(0x30)
#define I2C_RAW_INTR_STAT              	(0x34)
#define I2C_RX_TL                      	(0x38)
#define I2C_TX_TL                      	(0x3c)
#define I2C_CLR_INT                    	(0x40)
#define I2C_CLR_RX_UNDER               	(0x44)
#define I2C_CLR_RX_OVER                	(0x48)
#define I2C_CLR_TX_OVER                	(0x4c)
#define I2C_CLR_RD_REQ                 	(0x50)
#define I2C_CLR_TX_ABRT                	(0x54)
#define I2C_CLR_RX_DONE                	(0x58)
#define I2C_CLR_ACTIVITY               	(0x5c)
#define I2C_CLR_STOP_DET               	(0x60)
#define I2C_CLR_START_DET              	(0x64)
#define I2C_CLR_GEN_CALL               	(0x68)
#define I2C_ENABLE                     	(0x6c)
#define I2C_STATUS                     	(0x70)
#define I2C_TXFLR                      	(0x74)
#define I2C_RXFLR                      	(0x78)
#define I2C_TX_ABRT_SOURCE             	(0x80)
#define I2C_COMP_PARAM_1               	(0xf4)
#define I2C_COMP_VERSION               	(0xf8)
#define I2C_COMP_TYPE             		(0xfc)

/*I2C_CON*/
#define I2C_CON_SLAVE_DISABLE 			(1 << 6)
#define I2C_CON_RESTART_EN 				(1 << 5)
#define I2C_CON_10ADD_M 				(1 << 4)
#define I2C_CON_10ADD_S 				(1 << 3)
#define I2C_CON_S_SPEED 				(1 << 1)
#define I2C_CON_F_SPEED 				(2 << 1)
#define I2C_CON_H_SPEED 				(3 << 1)
#define I2C_CON_MASTER_MODE 			(1 << 0)

/*I2C_TAR*/
#define I2C_DATA_CMD_RW                	(1 << 8)


#define I2C_STATUS_ACTIVITY            	(1 << 0)

#define ABRT_7B_ADDR_NOACK				0
#define ABRT_10ADDR1_NOACK				1
#define ABRT_10ADDR2_NOACK				2
#define ABRT_TXDATA_NOACK				3
#define ABRT_GCALL_NOACK				4
#define ABRT_GCALL_READ					5
#define ABRT_SBYTE_ACKDET				7
#define ABRT_SBYTE_NORSTRT				9
#define ABRT_10B_RD_NORSTRT				10
#define ARB_MASTER_DIS					11
#define ARB_LOST						12

#define I2C_TX_ABRT_NOACK				(ABRT_7B_ADDR_NOACK | \
					    					ABRT_10ADDR1_NOACK | ABRT_10ADDR2_NOACK | \
					    					ABRT_TXDATA_NOACK | ABRT_GCALL_NOACK)

/*I2C_INT_MASK*/
#define I2C_INTR_RX_UNDER   			(1 << 0)
#define I2C_INTR_RX_OVER    			(1 << 1)
#define I2C_INTR_RX_FULL    			(1 << 2)
#define I2C_INTR_TX_OVER    			(1 << 3)
#define I2C_INTR_TX_EMPTY   			(1 << 4)
#define I2C_INTR_RD_REQ     			(1 << 5)
#define I2C_INTR_TX_ABRT    			(1 << 6)
#define I2C_INTR_RX_DONE    			(1 << 7)
#define I2C_INTR_ACTIVITY   			(1 << 8)
#define I2C_INTR_STOP_DET   			(1 << 9)
#define I2C_INTR_START_DET  			(1 << 10)
#define I2C_INTR_GEN_CALL   			(1 << 11)

#define I2C_INTR_DEFAULT_MASK			(I2C_INTR_RX_FULL | \
					 						I2C_INTR_TX_EMPTY | \
					 						I2C_INTR_TX_ABRT | \
					 						I2C_INTR_STOP_DET)

#endif /* __ASM_ARCH_REGS_I2C_H */

