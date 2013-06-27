#include "nand_common.h"
#include "nand_reg.h"

#define		BUS5_CLKFREQ	45000000
#define		BAUD_RATE		115200 

void uart0_init(void)
{
	unsigned long	baud, baudl, baudh; 

	/* config GPIO PD[11]、PD[10] sepcial function for uart0 */
	writel(readl(GPIO_PORTD_SEL) & ~(3 << 10), GPIO_PORTD_SEL);

	/* config baud divisor */
	baud =	(BUS5_CLKFREQ / 16 ) / BAUD_RATE;
	baudl = baud & 0xff;
	baudh = (baud >> 8) & 0xff;
	
	writel(readl(UART0_LCR) | (1 << 7), UART0_LCR);
	writel(baudl, UART0_DLBL);
	writel(baudh, UART0_DLBH);
	writel(readl(UART0_LCR) & ~(1 << 7), UART0_LCR);  

	/* config data length per frame, here 8 bits */
	writel(readl(UART0_LCR) | (3 << 0), UART0_LCR);
}

void uart0_send(const unsigned char *buf)
{
	/* judge buf whether NULL */
	if (buf != NULL) {
		/* judge whether to string end */
		while (*buf != '\0') {
			/* wait sending shift register empty */
			while (!(readl(UART0_LSR) & (1 << 5)));
			writel(*buf++, UART0_TXFIFO);
		}
	}
}
