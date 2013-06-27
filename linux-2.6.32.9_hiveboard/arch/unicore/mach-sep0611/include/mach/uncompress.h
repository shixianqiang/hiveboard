/* linux/arch/unicore/mach-sep0611/include/mach/uncompress.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *
 * Southeast University ASIC SoC support
 *
 * This file descript uncompress of the SEP0611
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  09-04-2010	initial version
 *
 */
 
#include <mach/hardware.h>

/*
 * This does not append a newline
 */
static inline void putc(int c)
{
	/* wait for room in the transmit FIFO */
	while(!((*(volatile unsigned char *)UART0_LSR)& 0x40))
		; 

	if (c == '\n'){
		*(volatile unsigned long*)UART0_TXFIFO = (0x0A);
		*(volatile unsigned long*)UART0_TXFIFO = (0x0D);
	}
	else
		*(volatile unsigned long*)UART0_TXFIFO = (int)c;
}

static inline void flush(void)
{

}


/*
 * nothing to do
 */
#define arch_decomp_setup()

#define arch_decomp_wdog()

