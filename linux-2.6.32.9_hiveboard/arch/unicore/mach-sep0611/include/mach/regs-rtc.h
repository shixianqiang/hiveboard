/* linux/arch/unicore/mach-sep0611/include/mach/regs-rtc.h
 *
 * Copyright(c) 2009-2011 SEUIC
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

#ifndef __ASM_ARCH_REGS_RTC_H
#define __ASM_ARCH_REGS_RTC_H

/*more details see sep0611 datasheet*/
#define RTC_STA_YMD             (0x00)
#define RTC_STA_HMS             (0x04)
#define RTC_ALARM               (0x08)
#define RTC_CTR                 (0x0c)
#define RTC_INT_EN              (0x10)
#define RTC_INT_STS             (0x14)
#define RTC_SAMP                (0x18)
#define RTC_WD_CNT              (0x1c)
#define RTC_CFG_CHK             (0x24)
#define RTC_POWERUP_CTRL        (0x28)

/*FOR GPS*/
#define TIC_COUNT               (0x40)
#define TIC_HMS                 (0x44)
#define TIC_YMD                	(0x48)

/*SEP0611_RTC_INT_EN*/
#define ALARM_INT_EN        	(1 << 4)
#define SEC_INT_EN				(1 << 2)
#define SAMP_INT_EN            	(1 << 0)

/*SEP0611_RTC_INT_STS*/
#define ALARM_FLAG            	(1 << 4)
#define SEC_FLAG				(1 << 2)
#define SAMP_FLAG             	(1 << 0)

#endif /* __ASM_ARCH_REGS_RTC_H */


