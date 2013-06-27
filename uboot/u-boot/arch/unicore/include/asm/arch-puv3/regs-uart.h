/*
 * PKUnity Universal Asynchronous Receiver/Transmitter (UART) Registers
 */
#define UART_RBR_OFFSET			0x0000 /* Data receive reg. */
#define UART_THR_OFFSET			0x0000 /* Data transfer reg. */
#define UART_DLL_OFFSET			0x0000 /* Divisor Latch LSB */
#define UART_DLH_OFFSET			0x0004 /* Divisor Latch MSB */
#define UART_IER_OFFSET			0x0004 /* Interrupt enable reg. */
#define UART_IIR_OFFSET			0x0008 /* Interrupt indentification reg. */
#define UART_FCR_OFFSET			0x0008 /* FIFO control reg. */
#define UART_LCR_OFFSET			0x000C /* Line control reg. */
#define UART_MCR_OFFSET			0x0010 /* Modem control reg. */
#define UART_LSR_OFFSET			0x0014 /* Line status reg. */
#define UART_MSR_OFFSET			0x0018 /* Modem status reg. */
#define UART_SRR_OFFSET			0x0088 /* Software Reset reg. */

#define UART_IER_ERBFI			(1 << 0) /* Enable Received Data Available Interrupt */
#define UART_IER_ETBEI			(1 << 1) /* Enable Transmit Holding Reg. Empty Interrupt */
#define UART_IER_ELSI			(1 << 2) /* Enable Receiver Line Status Interrupt */
#define UART_IER_EDSSI			(1 << 3) /* Enable Modem Status Interrupt */

#define UART_IIR_FIFOSE_MASK		(0b11 << 6) /* Enable FIFO mask */
#define UART_IIR_FIFOSE_DISABLE		(0b00 << 6) /* Disable */
#define UART_IIR_FIFOSE_ENABLE		(0b11 << 6) /* Enable */
#define UART_IIR_IID_MASK		(0b1111 << 0) /* Interrupt ID mask */
#define UART_IIR_IID_MS			(0b0000 << 0) /* modem status */
#define UART_IIR_IID_NIP		(0b0001 << 0) /* no interrupt pending */
#define UART_IIR_IID_THRE		(0b0010 << 0) /* THR empty */
#define UART_IIR_IID_RDA		(0b0100 << 0) /* received data available */
#define UART_IIR_IID_RLS		(0b0110 << 0) /* receiver line status */
#define UART_IIR_IID_CTO		(0b1100 << 0) /* character timeout */

#define UART_FCR_FE			(1 << 0) /* FIFO Enable */
#define UART_FCR_RFR			(1 << 1) /* RCVR FIFO Reset */
#define UART_FCR_XFR			(1 << 2) /* XMIT FIFO Reset */
#define UART_FCR_DMAM			(1 << 3) /* DMA Mode */
#define UART_FCR_RT_MASK		(0b11 << 6) /* RCVR Trigger */
#define UART_FCR_RT_1B			(0b00 << 6) /* received one byte */
#define UART_FCR_RT_4B			(0b01 << 6) /* 1/4 full of FIFO */
#define UART_FCR_RT_8B			(0b10 << 6) /* 1/2 full of FIFO */
#define UART_FCR_RT_16B			(0b11 << 6) /* two bytes left */

#define UART_LCR_DLS_MASK		(0b11 << 0)
#define UART_LCR_DLS_5B			(0b00 << 0)
#define UART_LCR_DLS_6B			(0b01 << 0)
#define UART_LCR_DLS_7B			(0b10 << 0)
#define UART_LCR_DLS_8B			(0b11 << 0)
#define UART_LCR_STOP			(1 << 2) /* Config. Stop Bit */
#define UART_LCR_PEN			(1 << 3) /* Enable Parity Bit */
#define UART_LCR_EPS			(1 << 4) /* Config. Parity Bit */
#define UART_LCR_BREAK			(1 << 6) /* Break Control Bit */
#define UART_LCR_DLAB			(1 << 7) /* Divisor Latch Access Bit */

#define UART_MCR_PTR			(1 << 0)
#define UART_MCR_RTS			(1 << 1)
#define UART_MCR_OUT1			(1 << 2)
#define UART_MCR_OUT2			(1 << 3)
#define UART_MCR_LP			(1 << 4) /* LoopBack */

#define UART_LSR_DR			(1 << 0) /* receive data ready */
#define UART_LSR_OE			(1 << 1) /* Overrun error */
#define UART_LSR_PE			(1 << 2) /* Parity error */
#define UART_LSR_FE			(1 << 3) /* Framing error */
#define UART_LSR_BI			(1 << 4) /* Break interrupt */
#define UART_LSR_THRE			(1 << 5) /* Tx buffer empty */
#define UART_LSR_TEMT			(1 << 6) /* Transmitter empty */
#define UART_LSR_RFE			(1 << 7) /* Receiced FIFO error */

#define UART_MSR_DCTS			(1 << 0) /* Delta clear to send */
#define UART_MSR_DDSR			(1 << 1) /* Delta data set ready */
#define UART_MSR_TERI			(1 << 2) /* Trailing edge of ring indicator */
#define UART_MSR_DDCD			(1 << 3) /* Delta data carrier detect */
#define UART_MSR_CTS			(1 << 4) /* Clear to send */
#define UART_MSR_DSR			(1 << 5) /* Data set ready */
#define UART_MSR_RI			(1 << 6) /* Ring indicator */
#define UART_MSR_DCD			(1 << 7) /* Data carrier detect */

