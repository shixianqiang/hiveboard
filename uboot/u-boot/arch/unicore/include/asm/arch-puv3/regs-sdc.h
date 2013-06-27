/*
 * PKUnity Multi-Media Card and Security Digital Card (MMC/SD) Registers
 */
#define SDC_CCR		__REG(PKUNITY_SDC_BASE + 0x0000) /* Clock Control Reg. */
#define SDC_SRR		__REG(PKUNITY_SDC_BASE + 0x0004) /* Software Reset Reg. */
#define SDC_ARGUMENT	__REG(PKUNITY_SDC_BASE + 0x0008) /* Argument Reg. */
#define SDC_COMMAND	__REG(PKUNITY_SDC_BASE + 0x000C) /* Command Reg. */
#define SDC_BLOCKSIZE	__REG(PKUNITY_SDC_BASE + 0x0010) /* Block Size Reg. */
#define SDC_BLOCKCOUNT	__REG(PKUNITY_SDC_BASE + 0x0014) /* Block Cound Reg. */
#define SDC_TMR		__REG(PKUNITY_SDC_BASE + 0x0018) /* Transfer Mode Reg. */
#define SDC_RES0	__REG(PKUNITY_SDC_BASE + 0x001C) /* Response Reg. 0 */
#define SDC_RES1	__REG(PKUNITY_SDC_BASE + 0x0020) /* Response Reg. 1 */
#define SDC_RES2	__REG(PKUNITY_SDC_BASE + 0x0024) /* Response Reg. 2 */
#define SDC_RES3	__REG(PKUNITY_SDC_BASE + 0x0028) /* Response Reg. 3 */
#define SDC_RTCR	__REG(PKUNITY_SDC_BASE + 0x002C) /* Read Timeout Control Reg. */
#define SDC_ISR		__REG(PKUNITY_SDC_BASE + 0x0030) /* Interrupt Status Reg. */
#define SDC_ISMR	__REG(PKUNITY_SDC_BASE + 0x0034) /* Interrupt Status Mask Reg. */
#define SDC_RXFIFO	__REG(PKUNITY_SDC_BASE + 0x0038) /* RX FIFO */
#define SDC_TXFIFO	__REG(PKUNITY_SDC_BASE + 0x003C) /* TX FIFO */

#define SDC_CCR_CLKEN			FIELD(1, 1, 2) /* SD Clock Enable */
#define SDC_CCR_PDIV(v)			FIELD((v), 8, 8) /* [15:8] */

#define SDC_SRR_ENABLE			FIELD(0, 1, 0) /* Software reset enable */
#define SDC_SRR_DISABLE			FIELD(1, 1, 0) /* Software reset disable */

#define SDC_COMMAND_RESTYPE_MASK	FMASK(2, 0) /* Response type */
#define SDC_COMMAND_RESTYPE_NONE	FIELD(0, 2, 0) /* No response */
#define SDC_COMMAND_RESTYPE_LONG	FIELD(1, 2, 0) /* 136-bit long response */
#define SDC_COMMAND_RESTYPE_SHORT	FIELD(2, 2, 0) /* 48-bit short response */
#define SDC_COMMAND_RESTYPE_SHORTBUSY	FIELD(3, 2, 0) /* 48-bit short and test if busy response */
#define SDC_COMMAND_DATAREADY		FIELD(1, 1, 2) /* data ready */
#define SDC_COMMAND_CMDEN		FIELD(1, 1, 3)
#define SDC_COMMAND_CMDINDEX(v)		FIELD((v), 6, 5) /* [10:5] */

#define SDC_BLOCKSIZE_BSMASK(v)		FIELD((v), 11, 0) /* [10:0] */
#define SDC_BLOCKCOUNT_BCMASK(v)	FIELD((v), 12, 0) /* [11:0] */

#define SDC_TMR_WTH_1BIT		FIELD(0, 1, 0) /* Data Width 1bit */
#define SDC_TMR_WTH_4BIT		FIELD(1, 1, 0) /* Data Width 4bit */
#define SDC_TMR_DIR_READ		FIELD(0, 1, 1) /* Read */
#define SDC_TMR_DIR_WRITE		FIELD(1, 1, 1) /* Write */

#define SDC_IR_MASK			FMASK(13, 0)
#define SDC_IR_RESTIMEOUT		FIELD(1, 1, 0)
#define SDC_IR_WRITECRC			FIELD(1, 1, 1)
#define SDC_IR_READCRC			FIELD(1, 1, 2)
#define SDC_IR_TXFIFOREAD		FIELD(1, 1, 3)
#define SDC_IR_RXFIFOWRITE		FIELD(1, 1, 4)
#define SDC_IR_READTIMEOUT        	FIELD(1, 1, 5)
#define SDC_IR_DATACOMPLETE		FIELD(1, 1, 6)
#define SDC_IR_CMDCOMPLETE		FIELD(1, 1, 7)
#define SDC_IR_RXFIFOFULL		FIELD(1, 1, 8)
#define SDC_IR_RXFIFOEMPTY		FIELD(1, 1, 9)
#define SDC_IR_TXFIFOFULL		FIELD(1, 1, 10)
#define SDC_IR_TXFIFOEMPTY		FIELD(1, 1, 11)
#define SDC_IR_ENDCMDWITHRES		FIELD(1, 1, 12)
