/*
 * PKUNITY Power Manager (PM) Registers
 */
#define PM_PMCR			__REG(PKUNITY_PM_BASE + 0x0000) /* PM Control Reg. */
#define PM_PGCR			__REG(PKUNITY_PM_BASE + 0x0004) /* PM General Conf. Reg. */
#define PM_PPCR			__REG(PKUNITY_PM_BASE + 0x0008) /* PM PLL Conf. Reg. */
#define PM_PWER			__REG(PKUNITY_PM_BASE + 0x000C) /* PM Wakeup Enable Reg. */
#define PM_PGSR			__REG(PKUNITY_PM_BASE + 0x0010) /* PM GPIO Sleep Status Reg. */
#define PM_PCGR			__REG(PKUNITY_PM_BASE + 0x0014) /* PM Clock Gate Reg. */
#define PM_PLLSYSCFG		__REG(PKUNITY_PM_BASE + 0x0018) /* PM SYS PLL Conf. Reg. */
#define PM_PLLDDRCFG		__REG(PKUNITY_PM_BASE + 0x001C) /* PM DDR PLL Conf. Reg. */
#define PM_PLLVGACFG		__REG(PKUNITY_PM_BASE + 0x0020) /* PM VGA PLL Conf. Reg. */
#define PM_DIVCFG		__REG(PKUNITY_PM_BASE + 0x0024) /* PM Div Conf. Reg. */
#define PM_PLLSYSSTATUS		__REG(PKUNITY_PM_BASE + 0x0028) /* PM SYS PLL Status Reg. */
#define PM_PLLDDRSTATUS		__REG(PKUNITY_PM_BASE + 0x002C) /* PM DDR PLL Status Reg. */
#define PM_PLLVGASTATUS		__REG(PKUNITY_PM_BASE + 0x0030) /* PM VGA PLL Status Reg. */
#define PM_DIVSTATUS		__REG(PKUNITY_PM_BASE + 0x0034) /* PM Div Status Reg. */
#define PM_SWRESET		__REG(PKUNITY_PM_BASE + 0x0038) /* PM Software Reset Reg. */
#define PM_DDR2START		__REG(PKUNITY_PM_BASE + 0x003C) /* PM DDR2 PAD Start Reg. */
#define PM_DDR2CAL0		__REG(PKUNITY_PM_BASE + 0x0040) /* PM DDR2 PAD Status Reg. */
#define PM_PLLDFCDONE		__REG(PKUNITY_PM_BASE + 0x0044) /* PM PLL DFC Done Reg. */

#define PM_PMCR_SFB		FIELD(1, 1, 0)
#define PM_PMCR_IFB		FIELD(1, 1, 1)
#define PM_PMCR_CFBSYS		FIELD(1, 1, 2)
#define PM_PMCR_CFBDDR		FIELD(1, 1, 3)
#define PM_PMCR_CFBVGA		FIELD(1, 1, 4)
#define PM_PMCR_CFBDIVBCLK	FIELD(1, 1, 5)

#define PM_PWER_GPIOHIGH	FIELD(1, 1, 8) /* GPIO 8~27 wake-up enable */
#define PM_PWER_RTC		FIELD(1, 1, 31) /* RTC alarm wake-up enable */

#define PM_PCGR_VGACLK		FIELD(1, 1, 20)

#define PM_DIVCFG_VGACLK_MASK	FMASK(4, 20)
#define PM_DIVCFG_VGACLK(v)	FIELD((v), 4, 20) /* [23:20] */

#define PM_SWRESET_USB		FIELD(1, 1, 6)
#define PM_SWRESET_VGADIV	FIELD(1, 1, 26)
#define PM_SWRESET_GEDIV	FIELD(1, 1, 27)

#define PM_PLLDFCDONE_SYSDFC	FIELD(1, 1, 0)
#define PM_PLLDFCDONE_DDRDFC	FIELD(1, 1, 1)
#define PM_PLLDFCDONE_VGADFC	FIELD(1, 1, 2)
