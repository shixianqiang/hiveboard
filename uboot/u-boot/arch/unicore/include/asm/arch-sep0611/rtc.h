/*********************************************************************
* Filename    :  rtc.h
* Author      :  SixRoom
* Date        :  2011-01-16
* Description :  This file is used to define the rtc
* Version  	  :  v1.04
**********************************************************************/
/*
|---------------|------------------|---------------|-----------------------------------------|
|    version    |       date       |     Author    |           modify infomation             |
|---------------|------------------|---------------|-----------------------------------------|
|    v1.00      |    2010-02-25    |  YHQ		   | 1) create this typedef file    		 |
|    v1.01      |    2010-06-22    |  SixRoom      | 1) modify some type                     |
|    v1.02      |    2011-12-11    |  SixRoom      | 1) update and add some type             |
|    v1.03      |    2011-01-14    |  SixRoom      | 1) reduce some type				     |
|    v1.04      |    2011-01-16    |  SixRoom      | 1) reduce some type				     |
|---------------|------------------|---------------|-----------------------------------------|
*/
#ifndef RTC_H
#define RTC_H

#include <asm/arch/vic.h>

#define INT_SAMPLE 		(1<<0)
#define INT_WATCHDOG	(1<<1)
#define INT_SECOND		(1<<2)
#define INT_MINUTE		(1<<3)
#define	INT_ALARM		(1<<4)
#define INT_RESET		(1<<5)

#define    RTC_POWERUP_CTRL              (RTC_BASE + 0x028)

#define RTC_OT_CLKSEL	SYS_DIV_CLK
#define RTC_OT_CLKDIV	0
#define RTC_OT_RSTTYPE	ONCE_RESET
#define RTC_OT_WDCOUNT	0
#define RTC_OT_WDEN		TRUE
#define RTC_OT_SAMPLEVALUE	0
#define RTC_OT_SAMPLEEN	TRUE
#define RTC_OT_INTTYPE	INT_SECOND
#define RTC_OT_INTEN	TRUE
#define RTC_OT_TICVALUE	0

#define RTC_CUR_YEAR	2011
#define RTC_CUR_MONTH	Mar
#define RTC_CUR_DAY		Day17
#define RTC_CUR_HOUR	Hour7
#define RTC_CUR_MINUTE	Min29
#define RTC_CUR_SECOND	Sec20

#define RTC_ALARM_MONTH		Mar
#define RTC_ALARM_DAY		Day17
#define RTC_ALARM_HOUR		Hour7
#define RTC_ALARM_MINUTE	Min30

#define Wait_RTCCFG_DONE()		while((*(RP)RTC_CTR & (1<<31))!=0)	/*RTCÅäÖÃ×´Ì¬*/
#define Wait_RTCDI_DONE()		while((*(RP)RTC_CTR & (1<<7))!=0)	/*Domain_IsolationÅäÖÃ×´Ì¬*/
#define Wait_RTCWDP_DONE()		while((*(RP)RTC_CTR & (1<<5))!=0)	/*WatchDog PauseÅäÖÃ×´Ì¬*/
#define Wait_RTCWDEn_DONE()		while((*(RP)RTC_CTR & (1<<3))!=0)	/*WatchDog EnableÅäÖÃ×´Ì¬*/
#define Wait_RTCSamEn_DONE()	while((*(RP)RTC_CTR & (1<<1))!=0)	/*Sample EnableÅäÖÃ×´Ì¬*/

extern void RTC_Test(void);
#endif
