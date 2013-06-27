/*
 * PKUnity Operating System Timer (OST) Registers
 */
#define OST_OSMR0	__REG(PKUNITY_OST_BASE + 0x0000) /* Match Reg. 0 */
#define OST_OSMR1	__REG(PKUNITY_OST_BASE + 0x0004) /* Match Reg. 1 */
#define OST_OSMR2	__REG(PKUNITY_OST_BASE + 0x0008) /* Match Reg. 2 */
#define OST_OSMR3	__REG(PKUNITY_OST_BASE + 0x000C) /* Match Reg. 3 */
#define OST_OSCR	__REG(PKUNITY_OST_BASE + 0x0010) /* Counter Reg. */
#define OST_OSSR	__REG(PKUNITY_OST_BASE + 0x0014) /* Status Reg. */
#define OST_OWER	__REG(PKUNITY_OST_BASE + 0x0018) /* Watchdog Enable Reg. */
#define OST_OIER	__REG(PKUNITY_OST_BASE + 0x001C) /* Interrupt Enable Reg. */

#define OST_OSSR_M0		FIELD(1, 1, 0) /* Match detected 0 */
#define OST_OSSR_M1		FIELD(1, 1, 1) /* Match detected 1 */
#define OST_OSSR_M2		FIELD(1, 1, 2) /* Match detected 2 */
#define OST_OSSR_M3		FIELD(1, 1, 3) /* Match detected 3 */

#define OST_OIER_E0		FIELD(1, 1, 0) /* Interrupt enable 0 */
#define OST_OIER_E1		FIELD(1, 1, 1) /* Interrupt enable 1 */
#define OST_OIER_E2		FIELD(1, 1, 2) /* Interrupt enable 2 */
#define OST_OIER_E3		FIELD(1, 1, 3) /* Interrupt enable 3 */

#define OST_OWER_WME		FIELD(1, 1, 0) /* Watchdog Match Enable */

