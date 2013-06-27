/*
 * (C) Copyright 2011
 */

#include <common.h>
#include <command.h>
#include <asm/arch/hardware.h>
#include <asm/proc-ucv2/ptrace.h>
#include <asm/u-boot-unicore.h>
#include <asm/vic/typedef.h>

/* we always count down the max. */
#define TIMER_LOAD_VAL 0xffffffff

extern void do_irq(void);
extern void udc_irq(void);
extern void timer1_isr_handler(void);
extern void timer2_isr_handler(void);
extern void timer3_isr_handler(void);
extern void EXT7_IRQRandle(void);
extern void EXT11_IRQRandle(void);

void timer1_isr_handler(void)
{
	U32 chann_int_num;
	U32 i;
	
	chann_int_num = *(RP)(TIMER_TISR);  
	read_reg(TxISCR(1));
	read_reg(TxISCR(2));
	for(i=1; i<3; i++)
	{
		if((chann_int_num & 0x1) == 0x1)
		{
			UART0_TXFIFO = 'T';
	  		while(!(UART0_LSR & 0x40));
			UART0_TXFIFO = i|0x30;
	  		while(!(UART0_LSR & 0x40));
			UART0_TXFIFO = '|';
		}
		chann_int_num = chann_int_num>>1;
	}
}

void timer2_isr_handler(void)
{
	U32 chann_int_num;
	U32 i;
	
	chann_int_num = *(RP)(TIMER_TISR);  
	read_reg(TxISCR(3));
	read_reg(TxISCR(4));
	read_reg(TxISCR(5));
	read_reg(TxISCR(6));
	chann_int_num = chann_int_num>>2;
	for(i=3; i<7; i++)
	{
		if(( chann_int_num & 0x1) == 0x1)
		{
			UART0_TXFIFO = 'T';
	  		while(!(UART0_LSR & 0x40));
			UART0_TXFIFO = i|0x30;
	  		while(!(UART0_LSR & 0x40));
			UART0_TXFIFO = '|';
		}
		chann_int_num = chann_int_num>>1;
	}
}

void timer3_isr_handler(void)
{
	U32 chann_int_num;
	U32 i;
	
	chann_int_num = *(RP)(TIMER_TISR);  
	read_reg(TxISCR(6));
	read_reg(TxISCR(8));
	read_reg(TxISCR(9));
	read_reg(TxISCR(10));
	chann_int_num = chann_int_num>>6;
	for(i=7; i<11; i++)
	{
		if(( chann_int_num & 0x1) == 0x1)
		{
			UART0_TXFIFO = 'T';
	  		while(!(UART0_LSR & 0x40));
			UART0_TXFIFO = i|0x30;
	  		while(!(UART0_LSR & 0x40));
			UART0_TXFIFO = '|';
		}
		chann_int_num = chann_int_num>>1;
	}
}

void (*int_vector_handler[64])(void) = {
		/* handler */
		    NULL			,//0
		    NULL			,//1
		    NULL			,//2
		    NULL			,//3
		    NULL			,//4
		    NULL			,//5
		    NULL			,//6
		    NULL		    ,//7
		    NULL			,//8
		    NULL			,//9
		    NULL			,//10
		    NULL		    ,//11
		    NULL			,//12
		    NULL			,//13
		    NULL			,//14
		    NULL			,//15
			NULL			,//16
			NULL			,//17
		    NULL			,//18				
		    NULL			,//19
		    NULL			,//20
		    NULL			,//21
		    NULL			,//22
		    NULL			,//23
		    NULL			,//24
		    NULL			,//25
		    NULL			,//26
		    NULL			,//27
		    NULL			,//28
		    NULL			,//29
		    NULL			,//30
		    NULL			,//31
			NULL			,//32
			NULL			,//33
			NULL			,//34
			NULL			,//35
			NULL			,//36
			NULL			,//37
			NULL			,//38
			NULL			,//39
			udc_irq			,//40
			NULL			,//41
			NULL			,//42
			NULL			,//43
			NULL			,//44
			NULL			,//45
			NULL			,//46
			NULL			,//47
			NULL			,//48
			NULL			,//49
			NULL			,//50
			NULL			,//51
			NULL			,//52
			NULL			,//53
			NULL			,//54
			NULL			,//55
			timer1_isr_handler	,//56
			timer2_isr_handler	,//57
			timer3_isr_handler	,//58
			NULL			,//59
			NULL			,//60
			NULL			,//61
			NULL			,//62
			NULL			,//63
};

void do_irq (void)
{
	unsigned int intnum;	
	intnum = *(volatile unsigned int *)(VIC_IRQ_VECTOR_NUM);
	/*Something may be wrong*/
	if( intnum == 64 ||  *int_vector_handler[intnum] ==NULL )
		return;
	(*int_vector_handler[intnum])();
	return;
}

static ulong timestamp;
static ulong lastdec;

int arch_interrupt_init (void)
{
	/*Just For Display,the ACTURE Stack Address is build in start.s*/
	FIQ_STACK_START = _unicoreboot_start-CONFIG_SYS_MALLOC_LEN-CONFIG_SYS_GBL_DATA_SIZE-12;
	IRQ_STACK_START = _unicoreboot_start-CONFIG_SYS_MALLOC_LEN-CONFIG_SYS_GBL_DATA_SIZE-CONFIG_STACKSIZE_IRQ-12;
	*(RP)TIMER_T1LCR = TIMER_LOAD_VAL;
	*(RP)TIMER_T1CR = 0x17;
	lastdec = *(RP)TIMER_T1LCR = TIMER_LOAD_VAL;
	timestamp = 0;
	return 0;
}

/* timer without interrupts */
void reset_timer (void)
{
	reset_timer_masked ();
}

ulong get_timer (ulong base)
{
	return (get_timer_masked () - base)>>16;
}

void set_timer (ulong t)
{
	timestamp = t;
}

void __udelay (unsigned long usec)
{
	ulong now = *(RP)TIMER_T1CCR;
	ulong next = now - usec*240;
	
	while(*(RP)TIMER_T1CCR > next);
}

void reset_timer_masked (void)
{
	/* reset time */
	lastdec = *(RP)TIMER_T1CCR;
	timestamp = 0;
}

ulong get_timer_masked (void)
{
	ulong now = *(RP)TIMER_T1CCR;

	if (lastdec >= now) {
		// normal mode
		timestamp += lastdec - now;
	} else {
		//  have an overflow ...
		timestamp += lastdec + TIMER_LOAD_VAL - now;
	}
	lastdec = now;

	return timestamp;
}

void udelay_masked (unsigned long usec)
{
	ulong tmo;
	ulong endtime;
	signed long diff;

	if (usec >= 1000) {
		tmo = usec / 1000;
		tmo *= CONFIG_SYS_HZ;
		tmo /= 8;
	} else {
		tmo = usec * CONFIG_SYS_HZ;
		tmo /= (1000*8);
	}

	endtime = get_timer(0) + tmo;

	do {
		ulong now = get_timer_masked ();
		diff = endtime - now;
	} while (diff >= 0);
}
