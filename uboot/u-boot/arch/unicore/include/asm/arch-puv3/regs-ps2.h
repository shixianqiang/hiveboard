/*
 * PKUnity PS2 Controller Registers
 */
#define PS2_DATA	__REG(PKUNITY_PS2_BASE + 0x0060) /* the same as I8042_DATA_REG */
#define PS2_COMMAND	__REG(PKUNITY_PS2_BASE + 0x0064) /* the same as I8042_COMMAND_REG */
#define PS2_STATUS	__REG(PKUNITY_PS2_BASE + 0x0064) /* the same as I8042_STATUS_REG */
#define PS2_CNT		__REG(PKUNITY_PS2_BASE + 0x0068) /* counter reg. */

