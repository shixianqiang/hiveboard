#include <linux/rtc.h>
unsigned int *esram_iomem = 0xc2000000;

struct rtc_time		pmu_rtc_time;
struct rtc_time		pmu_rtc_time1;//read time value to this struct
struct rtc_wkalrm	pmu_rtc_wkalrm;
struct rtc_wkalrm	pmu_rtc_wkalrm1;//read alarm value to this struct

extern int sep_pm_enter(suspend_state_t state);

typedef unsigned long   	  U32;     /* unsigned 32-bit integer */

typedef	volatile U32 *		  RP;

#define    write_reg(reg, data) \
	       *(RP)(reg) = data
#define    read_reg(reg) \
	       *(RP)(reg)
