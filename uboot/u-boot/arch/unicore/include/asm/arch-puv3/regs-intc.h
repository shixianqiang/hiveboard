/*
 * PKUNITY Interrupt Controller (INTC) Registers
 */
#define INTC_ICLR	__REG(PKUNITY_INTC_BASE + 0x0000) /* INTC Level Reg. */
#define INTC_ICMR	__REG(PKUNITY_INTC_BASE + 0x0004) /* INTC Mask Reg. */
#define INTC_ICPR	__REG(PKUNITY_INTC_BASE + 0x0008) /* INTC Pending Reg. */
#define INTC_ICIP	__REG(PKUNITY_INTC_BASE + 0x000C) /* INTC IRQ Pending Reg. */
#define INTC_ICFP	__REG(PKUNITY_INTC_BASE + 0x0010) /* INTC FIQ Pending Reg. */
#define INTC_ICCR	__REG(PKUNITY_INTC_BASE + 0x0014) /* INTC Control Reg. */

