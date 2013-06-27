/*
 * PKUnity Serial Peripheral Interface (SPI) Registers
 */
#define SPI_CR0		__REG(PKUNITY_SPI_BASE + 0x0000) /* Control reg. 0 */
#define SPI_CR1		__REG(PKUNITY_SPI_BASE + 0x0004) /* Control reg. 1 */
#define SPI_SSIENR	__REG(PKUNITY_SPI_BASE + 0x0008)/* Enable reg. */
#define SPI_SR		__REG(PKUNITY_SPI_BASE + 0x0028) /* Status reg. */
#define SPI_IMR		__REG(PKUNITY_SPI_BASE + 0x002C) /* Interrupt Mask reg. */
#define SPI_ISR		__REG(PKUNITY_SPI_BASE + 0x0030) /* Interrupt Status reg. */

#define SPI_SSIENR_EN		FIELD(1, 1, 0) /* Enable SPI Controller */

#define SPI_SR_BUSY		FIELD(1, 1, 0) /* SPI Busy */
#define SPI_SR_TFNF		FIELD(1, 1, 1) /* Transmit FIFO Not Full */
#define SPI_SR_TFE		FIELD(1, 1, 2) /* Transmit FIFO Empty */
#define SPI_SR_RFNE		FIELD(1, 1, 3) /* Receive FIFO Not Empty */
#define SPI_SR_RFF		FIELD(1, 1, 4) /* Receive FIFO Full */

#define SPI_ISR_TXEIS		FIELD(1, 1, 0) /* Trans. FIFO Empty Interrupt Status */
#define SPI_ISR_TXOIS		FIELD(1, 1, 1) /* Trans. FIFO Overflow Interrupt Status */
#define SPI_ISR_RXUIS		FIELD(1, 1, 2) /* Receiv. FIFO Underflow Interrupt Status */
#define SPI_ISR_RXOIS		FIELD(1, 1, 3) /* Receiv. FIFO Overflow Interrupt Status */
#define SPI_ISR_RXFIS		FIELD(1, 1, 4) /* Receiv. FIFO Full Interrupt Status */
#define SPI_ISR_MSTIS		FIELD(1, 1, 5)

#define SPI_IMR_TXEIM		FIELD(1, 1, 0) /* Trans. FIFO Empty Interrupt Mask */
#define SPI_IMR_TXOIM		FIELD(1, 1, 1) /* Trans. FIFO Overflow Interrupt Mask */
#define SPI_IMR_RXUIM		FIELD(1, 1, 2) /* Receiv. FIFO Underflow Interrupt Mask */
#define SPI_IMR_RXOIM		FIELD(1, 1, 3) /* Receiv. FIFO Overflow Interrupt Mask */
#define SPI_IMR_RXFIM		FIELD(1, 1, 4) /* Receiv. FIFO Full Interrupt Mask */
#define SPI_IMR_MSTIM		FIELD(1, 1, 5)

