/*
 * PKUnity Real-Time Clock (RTC) control registers
 */
#define RTC_RTAR	__REG(PKUNITY_RTC_BASE + 0x0000) /* RTC Alarm Reg. */
#define RTC_RCNR	__REG(PKUNITY_RTC_BASE + 0x0004) /* RTC Count Reg. */
#define RTC_RTTR	__REG(PKUNITY_RTC_BASE + 0x0008) /* RTC Trim Reg. */
#define RTC_RTSR	__REG(PKUNITY_RTC_BASE + 0x0010) /* RTC Status Reg. */

#define RTC_RTSR_AL		FIELD(1, 1, 0) /* ALarm detected */
#define RTC_RTSR_HZ		FIELD(1, 1, 1) /* 1 Hz clock detected */
#define RTC_RTSR_ALE		FIELD(1, 1, 2) /* ALarm interrupt Enable */
#define RTC_RTSR_HZE		FIELD(1, 1, 3) /* 1 Hz clock interrupt Enable */

