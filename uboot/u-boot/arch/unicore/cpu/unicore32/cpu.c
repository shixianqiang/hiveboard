/*
 * Code specific to PKUnity SoC and UniCore ISA
 * Fragments that appear the same as the files in arm or x86
 *
 * Copyright, 2008, GUAN XUE-tao
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

/*
 * CPU specific code
 */

#include <common.h>
#include <command.h>
#include <rtc.h>

#ifdef CONFIG_USE_IRQ
DECLARE_GLOBAL_DATA_PTR;
#endif

int cpu_init (void)
{
	/*
	 * setup up stacks if necessary
	 */
#ifdef CONFIG_USE_IRQ
	IRQ_STACK_START = _unicoreboot_start - CONFIG_SYS_MALLOC_LEN - CONFIG_SYS_GBL_DATA_SIZE - 4;
	FIQ_STACK_START = IRQ_STACK_START - CONFIG_STACKSIZE_IRQ;
#endif

	return 0;
}

int cleanup_before_linux (void)
{
	/*
	 * this function is called just before we call linux
	 * it prepares the processor for linux
	 *
	 * just disable everything that can disturb booting linux
	 */

	disable_interrupts ();

	dcache_flush();
	dcache_disable();

	icache_invalid();
	icache_disable();
        
	return (0);
}

int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	disable_interrupts ();
//	reset_cpu (0);
	/*NOTREACHED*/
	return (0);
}

/* taken from blob */
void icache_enable (void)
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

void icache_disable (void)
{
	icache_invalid();
	__asm__ volatile ( "movc r0, p0.c1, #0\n"
		       "andn r0, r0, #8\n"
		       "movc p0.c1, r0, #0\n"
		       :
		       :
		       :"memory", "cc");
}

int icache_status (void)
{
	int ret;
	
	__asm__ volatile ( "movc %0, p0.c1, #0\n"
		       "and %0, %0, #8\n"
		       : "=&r" (ret)
		       :
		       :"memory", "cc");

	return ret;
}

void icache_invalid (void)
{
	__asm__ volatile ( "mov r0, #0\n"
		       "movc p0.c5, r0, #20\n"
		       "nop; nop; nop; nop; nop; nop; nop; nop\n"
		       :
		       :
		       :"memory", "cc");	
}

/* It makes no sense to use the dcache if the MMU is not enabled */
void dcache_enable (void)
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

void dcache_disable (void)
{
	dcache_flush();
	__asm__ volatile ( "movc r0, p0.c1, #0\n"
		       "andn r0, r0, #20\n"
		       "movc p0.c1, r0, #0\n"
		       :
		       :
		       :"memory", "cc");
}

int dcache_status (void)
{
	int ret;
	
	__asm__ volatile ( "movc %0, p0.c1, #0\n"
		       "and %0, %0, #4\n"
		       : "=&r" (ret)
		       :
		       :"memory", "cc");

	return ret;
}

void dcache_clean (void)
{
	__asm__ volatile ( "mov ip, #0\n"
		       "movc p0.c5, ip, #10\n"
		       "nop; nop; nop; nop; nop; nop; nop; nop\n"
		       :
		       :
		       :"memory", "cc");
}

void dcache_invalid (void)
{
	__asm__ volatile ( "mov r0, #0\n"
		       "movc p0.c5, r0, #12\n"
		       "nop; nop; nop; nop; nop; nop; nop; nop\n"
		       :
		       :
		       :"memory", "cc");
}

void dcache_flush (void)
{
	__asm__ volatile ( "mov ip, #0\n"
		       "movc p0.c5, ip, #14\n"
		       "nop; nop; nop; nop; nop; nop; nop; nop\n"
		       : 
		       : 
		       : "ip" );
}



/*
	RTC stuff
*/
int rtc_get (struct rtc_time* tm)
{
	return 0;
}

int rtc_set (struct rtc_time* tm)
{
	return 0;
}

void rtc_reset (void)
{

}
