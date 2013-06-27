#ifndef DELAY_H_
#define DELAY_H_

#include 	<asm/arch/vic.h>
//#include 	"../include/timer.h"

//=====================================================================
//			=============================================
//							   ��ʱ����
//			=============================================
//=====================================================================

#define delay_20ns()	__asm("NOP")

//=====================================================================
//			=============================================
//						ʹ�ö�ʱ��������ȷ��ʱ����
//			=============================================
//=====================================================================

//**********************************************
//		Function	��	timer_delay_s
//		Parameter	��	ch��ͨ���ţ�
//						dly����ʱ������
//		Description	��	����ʱ
//**********************************************
#define timer_delay_s(ch,dly)			\
do{										\
	*(RP)TxLCR(ch)=dly*50000000-20;		\
	*(RP)TxCR(ch)=0x0000001a;			\
	*(RP)TxCR(ch)|=0x01;				\
	while((*(RP)TIMER_TISR&0x01)==0);	\
}while(0)
//**********************************************
//		Function	��	timer_delay_ms
//		Parameter	��	ch��ͨ���ţ�
//						dly����ʱ��������
//		Description	��	������ʱ
//**********************************************
#define timer_delay_ms(ch,dly)			\
do{										\
	*(RP)TxLCR(ch)=dly*50000-20;		\
	*(RP)TxCR(ch)=0x0000001a;			\
	*(RP)TxCR(ch)|=0x01;				\
	while((*(RP)TIMER_TISR&0x01)==0);	\
}while(0)
//**********************************************
//		Function	��	timer_delay_us
//		Parameter	��	ch��ͨ���ţ�
//						dly����ʱ΢������
//		Description	��	΢����ʱ
//**********************************************
#define timer_delay_us(ch,dly)			\
do{										\
	*(RP)TxLCR(ch)=dly*50-20;			\
	*(RP)TxCR(ch)=0x0000001a;			\
	*(RP)TxCR(ch)|=0x01;				\
	while((*(RP)TIMER_TISR&0x01)==0);	\
}while(0)



#endif /*DELAY_H_*/