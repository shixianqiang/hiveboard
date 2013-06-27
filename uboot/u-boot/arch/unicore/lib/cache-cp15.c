/*
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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

#include <common.h>
#include <asm/system.h>

#if !(defined(CONFIG_SYS_NO_ICACHE) && defined(CONFIG_SYS_NO_DCACHE))
static void cp_delay (void)
{
	volatile int i;

	/* copro seems to need some delay between reading and writing */
	for (i = 0; i < 100; i++)
		nop();
}

/* cache_bit must be either CR_I or CR_C */
static void cache_enable(uint32_t cache_bit)
{
 	return;
}

/* cache_bit must be either CR_I or CR_C */
static void cache_disable(uint32_t cache_bit)
{
	return;
}
#endif

#ifdef CONFIG_SYS_NO_ICACHE
void icache_enable (void)
{
	return;
}

void icache_disable (void)
{
	return;
}

int icache_status (void)
{
	return 0;					/* always off */
}
#else
void icache_invalid (void)
{
	__asm__ volatile ( "mov r0, #0\n"
		       "movc p0.c5, r0, #20\n"
		       "nop; nop; nop; nop; nop; nop; nop; nop\n"
		       :
		       :
		       :"memory", "cc");	
}

void icache_enable(void)
{
	__asm__ volatile ( "mov r0, #0\n"
		       "movc p0.c5, r0, #20\n"
		       "nop; nop; nop; nop; nop; nop; nop; nop\n"
		       :
		       :
		       :"memory", "cc");

	__asm__ volatile ( "movc r0, p0.c1, #0\n"
		       "or r0, r0, #8\n"
		       "movc p0.c1, r0, #0\n"
		       :
		       :
		       :"memory", "cc");
}

void icache_disable(void)
{
	icache_invalid();
	__asm__ volatile ( "movc r0, p0.c1, #0\n"
		       "andn r0, r0, #8\n"
		       "movc p0.c1, r0, #0\n"
		       :
		       :
		       :"memory", "cc");
}

int icache_status(void)
{
	int ret;
	
	__asm__ volatile ( "movc %0, p0.c1, #0\n"
		       "and %0, %0, #8\n"
		       : "=&r" (ret)
		       :
		       :"memory", "cc");

	return ret;
}
#endif

#ifdef CONFIG_SYS_NO_DCACHE
void dcache_enable (void)
{
	return;
}

void dcache_disable (void)
{
	return;
}

int dcache_status (void)
{
	return 0;					/* always off */
}
#else
void dcache_flush (void)
{
	__asm__ volatile ( "mov ip, #0\n"
		       "movc p0.c5, ip, #14\n"
		       "nop; nop; nop; nop; nop; nop; nop; nop\n"
		       : 
		       : 
		       : "ip" );
}

void dcache_enable(void)
{
	__asm__ volatile ( "mov r0, #0\n"
		       "movc p0.c5, r0, #12\n"
		       "nop; nop; nop; nop; nop; nop; nop; nop\n"
		       :
		       :
		       :"memory", "cc");
	
	__asm__ volatile ( "movc r0, p0.c1, #0\n"
		       "or r0, r0, #20\n"
		       "movc p0.c1, r0, #0\n"
		       :
		       :
		       :"memory", "cc");
}

void dcache_disable(void)
{
	dcache_flush();
	__asm__ volatile ( "movc r0, p0.c1, #0\n"
		       "andn r0, r0, #20\n"
		       "movc p0.c1, r0, #0\n"
		       :
		       :
		       :"memory", "cc");
}

int dcache_status(void)
{
	int ret;
	
	__asm__ volatile ( "movc %0, p0.c1, #0\n"
		       "and %0, %0, #4\n"
		       : "=&r" (ret)
		       :
		       :"memory", "cc");

	return ret;
}
#endif
