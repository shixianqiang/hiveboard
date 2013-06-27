/*
 * PKUnity Reset Controller (RC) Registers
 */
#define RESETC_SWRR	__REG(PKUNITY_RESETC_BASE + 0x0000) /* Software Reset Register */
#define RESETC_RSSR	__REG(PKUNITY_RESETC_BASE + 0x0004) /* Reset Status Register */

#define RESETC_SWRR_SRB		FIELD(1, 1, 0) /* Software Reset Bit */

#define RESETC_RSSR_HWR		FIELD(1, 1, 0) /* Hardware Reset */
#define RESETC_RSSR_SWR		FIELD(1, 1, 1) /* Software Reset */
#define RESETC_RSSR_WDR		FIELD(1, 1, 2) /* Watchdog Reset */
#define RESETC_RSSR_SMR		FIELD(1, 1, 3) /* Sleep Mode Reset */

