/*
 * (C) Copyright 2004
 */

#include <common.h>
#include <asm/arch/hardware.h>
#include <asm/u-boot-unicore.h>

//#define UART_CLK 	80000000UL //640M
//#define UART_CLK 	60000000UL //480M
//#define UART_CLK 	25000000UL //200M
#define UART_CLK 	60000000UL //180M (div 3)
#define BUAD_RATE 	115200


void serial_setbrg (void)
{
	DECLARE_GLOBAL_DATA_PTR;
	
	int baud = UART_CLK / BUAD_RATE / 16;

	/* get correct divisor */
	switch(gd->baudrate) {
		case 115200:
			UART0_LCR = 0x83;
			UART0_DLBH = baud >> 8;
			UART0_DLBL = baud & 0xFF;
			UART0_LCR = 0x03;
			break;
		default:
			UART0_LCR = 0x83;
			UART0_DLBH = 0x0;
			UART0_DLBL = 0x61;
			UART0_LCR = 0x03;
	}
}

/*
 * Initialise the serial port with the given baudrate. The settings
 * are always 8 data bits, no parity, 1 stop bit, no start bits.
 */
int serial_init (void)
{
	serial_setbrg ();
	return (0);
}


/*
 * Output a single byte to the serial port.
 */
void serial_putc (const char c)
{
	/* wait for room in the transmit FIFO */
	while(!(UART0_LSR & 0x40));

	if (c=='\n'){
		UART0_TXFIFO=(0x0a);
		UART0_TXFIFO=(0x0d);
	}
	else
		UART0_TXFIFO = (unsigned char)c;
}

void serial_puts (const char *s)
{
	while (*s) {
		serial_putc (*s++);
	}
}

/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise. When the function is succesfull, the character read is
 * written into its argument c.
 */
int serial_tstc (void)
{
	return (UART0_LSR & 0x01);
}

int serial_getc (void)
{
	int rv;

	for(;;) {
		rv = serial_tstc();
		if(rv > 0)
		{
			return UART0_RXFIFO;
		}
	}
}

