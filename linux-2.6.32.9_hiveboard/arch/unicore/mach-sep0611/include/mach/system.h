/* linux/arch/unicore/mach-sep0611/include/mach/system.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *
 * Southeast University ASIC SoC support
 *
 * This file contains the system task definitions of the SEP0611
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  09-04-2010	initial version
 *
 */
 
#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

#include <asm/io.h>
#include <mach/hardware.h>
extern void sep0611_reboot(void);
extern void sep0611_poweroff(void);
void sep0611_idle(void);

static void arch_idle(void)
{
	sep0611_idle();
}


static __inline__ void arch_reset(char mode, const char *cmd)
{
	sep0611_reboot();
}

static __inline__ void arch_poweroff()
{
	sep0611_poweroff();
}
#endif /* __ASM_ARCH_SYSTEM_H */

