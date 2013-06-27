/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _U_BOOT_UNICORE_H_
#define _U_BOOT_UNICORE_H_	1

#include <asm/proc/ucv2.h>

#define in8(v)				in_8(v)
#define out8(v, p)			out_8(v, p)
/* for the following variables, see start.S */
extern ulong _unicoreboot_start;	/* code start */
extern ulong _bss_start;	/* code + data end == BSS start */
extern ulong _bss_end;		/* BSS end */
extern ulong IRQ_STACK_START;	/* top of IRQ stack */
extern ulong FIQ_STACK_START;	/* top of FIQ stack */

/* cpu/.../cpu.c */
int	cpu_init(void);
int	cleanup_before_linux(void);
void	dcache_clean(void);
void	dcache_invalid(void);
void	dcache_flush(void);
void	icache_invalid(void);

/* cpu/.../arch/cpu.c */
int	arch_cpu_init(void);
int	arch_misc_init(void);

/* board/.../... */
int	board_init(void);
int	modules_test(void);
int	dram_init (void);
int	board_eth_init(bd_t *bis);
/* ------------------------------------------------------------ */
/* Here is a list of some prototypes which are incompatible to	*/
/* the U-Boot implementation					*/
/* To be fixed!							*/
/* ------------------------------------------------------------ */
/* common/cmd_nvedit.c */
int	setenv		(char *, char *);

/* cpu/.../interrupt.c */
int	arch_interrupt_init	(void);
void	reset_timer_masked	(void);
ulong	get_timer_masked	(void);
void	udelay_masked		(unsigned long usec);

/* cpu/.../timer.c */
//int	timer_init		(void);

#endif	/* _U_BOOT_UNICORE_H_ */
