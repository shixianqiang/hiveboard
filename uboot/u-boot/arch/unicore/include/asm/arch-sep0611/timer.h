/*********************************************************************
* Filename    :  timer.h
* Author      :  SixRoom
* Date        :  2011-01-10
* Description :  This file is used to define timer
* Version  	  :  v1.01
**********************************************************************/
/*
|---------------|------------------|---------------|-----------------------------------------|
|    version    |       date       |     Author    |           modify infomation             |
|---------------|------------------|---------------|-----------------------------------------|
|    v1.00      |    2010-02-25    |  WuPeng       | 1) create this typedef file    		 |
|    v1.01      |    2011-01-10    |  SixRoom      | 1) modify some function         	     |
|---------------|------------------|---------------|-----------------------------------------|
*/
#ifndef TIMER_H_
#define TIMER_H_

#include 	<asm/arch/vic.h>

#define TIMER_OT_ID		TIMER_ID1
#define TIMER_OT_MODE	MODE_RESTATR
#define TIMER_OT_PREVLU	0
#define TIMER_OT_INT	INT_ENABLE


extern void Timer_Test(void);

#endif /*TIMER_H_*/
