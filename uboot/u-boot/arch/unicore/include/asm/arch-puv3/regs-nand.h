/*
 * PKUnity NAND Controller Registers
 */
#define NAND_IDR0	__REG(PKUNITY_NAND_BASE + 0x0000) /* ID Reg. 0 */
#define NAND_IDR1	__REG(PKUNITY_NAND_BASE + 0x0004) /* ID Reg. 1 */
#define NAND_IDR2	__REG(PKUNITY_NAND_BASE + 0x0008) /* ID Reg. 2 */
#define NAND_IDR3	__REG(PKUNITY_NAND_BASE + 0x000C) /* ID Reg. 3 */
#define NAND_PAR0	__REG(PKUNITY_NAND_BASE + 0x0010) /* Page Address Reg. 0 */
#define NAND_PAR1	__REG(PKUNITY_NAND_BASE + 0x0014) /* Page Address Reg. 1 */
#define NAND_PAR2	__REG(PKUNITY_NAND_BASE + 0x0018) /* Page Address Reg. 2 */
#define NAND_ECCEN	__REG(PKUNITY_NAND_BASE + 0x001C) /* ECC Enable Reg. */
#define NAND_BUF	__REG(PKUNITY_NAND_BASE + 0x0020) /* Buffer Reg. */
#define NAND_ECCSR	__REG(PKUNITY_NAND_BASE + 0x0024) /* ECC Status Reg. */
#define NAND_CMD 	__REG(PKUNITY_NAND_BASE + 0x0028) /* Command Reg. */
#define NAND_DMACR	__REG(PKUNITY_NAND_BASE + 0x002C) /* DMA Configure Reg. */
#define NAND_IR		__REG(PKUNITY_NAND_BASE + 0x0030) /* Interrupt Reg. */
#define NAND_IMR	__REG(PKUNITY_NAND_BASE + 0x0034) /* Interrupt Mask Reg. */
#define NAND_CHIPEN	__REG(PKUNITY_NAND_BASE + 0x0038) /* Chip Enable Reg. */
#define NAND_ADDR	__REG(PKUNITY_NAND_BASE + 0x003C) /* Address Reg. */

#define NAND_CMD_CMD_MASK		FMASK(4, 4) /* Command bits */
#define NAND_CMD_CMD_READPAGE		FIELD(0x0, 4, 4)
#define NAND_CMD_CMD_ERASEBLOCK		FIELD(0x6, 4, 4)
#define NAND_CMD_CMD_READSTATUS		FIELD(0x7, 4, 4)
#define NAND_CMD_CMD_WRITEPAGE		FIELD(0x8, 4, 4)
#define NAND_CMD_CMD_READID		FIELD(0x9, 4, 4)
#define NAND_CMD_CMD_RESET		FIELD(0xf, 4, 4)

