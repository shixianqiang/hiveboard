/*********************************************************************
* Filename    :  pwm.h
* Description :  This file is used to define the PWM 
* Date        :  2010-03-22
* Author      :  SixRoom
*
**********************************************************************/
#ifndef PWM_H_
#define PWM_H_

#include 	<asm/arch/vic.h>


/***********************************************************
*	Function	：	pwm_mode
*	Parameter	：	pwmnum	（PWM号）
* 					mode	（模式：PWM模式、高速GPIO、一次计数模式、周期技术模式）
*	Return		：  
*	Description	：	
***********************************************************/
#define PWM_SetMode(pwmnum,mode)  						\
do{                              					\
      	*(RP)PWM_CTRL_CH(pwmnum) &= 0xfffffffc; 	\
		*(RP)PWM_CTRL_CH(pwmnum) |= (mode);  		\
  }while(0) 
/***********************************************************
*	Function	：	pwm_prescale
*	Parameter	：	pwmnum	（PWM号）
* 					div		（分频值）
*	Return		：  
*	Description	：	
***********************************************************/
#define PWM_SetPreScale(pwmnum,div)    		\
do{                              			\
		*(RP)PWM_CTRL_CH(pwmnum) = (div); 	\
  }while(0) 

/***********************************************************
*	Function	：	pwm_prescale
*	Parameter	：	pwmnum	（PWM号）
*	Return		：  
*	Description	：	
***********************************************************/
#define PWM_Enable(pwmnum, sta)  				\
do{                              				\
	*(RP)PWM_ENABLE &= ~(1<<(pwmnum-1));  		\
    *(RP)PWM_ENABLE |= ((sta)<<(pwmnum-1));  	\
  }while(0)

/***********************************************************
*	Function	：	pwm_clr_int
*	Parameter	：	pwmnum	（PWM号）
*	Return		：  
*	Description	：	
***********************************************************/
#define PWM_ClrInt(pwmnum)  				\
do{                              			\
      *(RP)PWM_INT |= (1<<(pwmnum-1));  	\
  }while(0) 
/***********************************************************
*	Function	：	pwm_int_mask
*	Parameter	：	pwmnum	（PWM号）
*	Return		：  
*	Description	：	
***********************************************************/
#define PWM_MaskInt(pwmnum, sta)  				\
do{                              				\
	 *(RP)PWM_INTMASK &= ~(1<<(pwmnum-1));  	\
     *(RP)PWM_INTMASK |= (sta<<(pwmnum-1));  	\
  }while(0) 

//***********************************************************
//		Function	：	pwm_gpio_dir
//		Parameter	：	pwmnum		（PWM号）
//						direction	（方向）
//		Description	：	PWM作为高速GPIO口模式时的端口方向配置函数
//***********************************************************
#define PWM_SetHGDir(pwmnum,direction)    				\
do{                              						\
      	*(RP)PWM_CTRL_CH(pwmnum) &= 0xfffffff7;  		\
		*(RP)PWM_CTRL_CH(pwmnum) |= ((direction)<<3); 	\
  }while(0) 
//***********************************************************
//		Function	：	pwm_mode_gpio_read
//		Parameter	：	pwmnum		（PWM号）
//		Description	：	PWM作为高速GPIO口模式时读端口值
//***********************************************************
#define PWM_ReadHGPin(pwmnum)				\
		*(RP)PWM_DATA_CH(pwmnum)
//***********************************************************
//		Function	：	pwm_mode_gpio_write
//		Parameter	：	pwmnum		（PWM号）
//						data		（待写数据）
//		Description	：	PWM作为高速GPIO口模式时往指定端口写值
//***********************************************************
#define PWM_WriteHGPin(pwmnum,data)  		\
do{                              				\
		*(RP)PWM_DATA_CH(pwmnum)=(data);		\
}while(0) 		

//***********************************************************
//		Function	：	pwm_timer_matchout
//		Parameter	：	pwmnum		（PWM号）
//						mode		（待写数据）
//		Description	：	PWM配置匹配值
//***********************************************************  
#define PWM_SetMatchMode(pwmnum,mode)    				\
do{                              						\
      	*(RP)PWM_CTRL_CH(pwmnum) &= 0xffffffcf;  		\
		*(RP)PWM_CTRL_CH(pwmnum) |= ((mode)<<4); 		\
  }while(0) 

//***********************************************************
//		Function	：	pwm_period
//		Parameter	：	pwmnum		（PWM号）
//						period		（定时周期）
//		Description	：	PWM周期设置
//***********************************************************
#define PWM_SetPeriod(pwmnum,period)    		\
do{                              				\
		*(RP)PWM_PERIOD_CH(pwmnum) = (period); 	\
  }while(0)
//***********************************************************
//		Function	：	pwm_dutycycle
//		Parameter	：	pwmnum		（PWM号）
//						pulse		（占空比）
//		Description	：	PWM占空比配置
//***********************************************************
#define PWM_SetDutyCycle(pwmnum,pulse)    		\
do{                              				\
		*(RP)PWM_DATA_CH(pwmnum) = (pulse); 	\
  }while(0) 

#define PWM_SetHGData(pwmnum,data)    		\
do{                              			\
		*(RP)PWM_DATA_CH(pwmnum) = (data); 	\
//***********************************************************
//		Function	：	pwm_currentvalue
//		Parameter	：	pwmnum	（PWM号）
//		Description	：	PWM读取当前值
//***********************************************************
#define PWM_ReadCountValue(pwmnum)   				\
			*(RP)PWM_CNT_CH(pwmnum)	
//***********************************************************
//		Function	：	pwm_fifostatus
//		Parameter	：	pwmnum	（PWM号）
//		Description	：	PWM读取当前FIFO状态
//***********************************************************
#define PWM_ReadFIFOStatus(pwmnum)   				\
			*(RP)PWM_STATUS_CH(pwmnum)


extern void PWM_Test(void);

#endif /*PWM_H_*/
