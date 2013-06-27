/*
 * (C) Copyright 2003
 * Texas Instruments <www.ti.com>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
 *
 * (C) Copyright 2002-2004
 * Gary Jennejohn, DENX Software Engineering, <garyj@denx.de>
 *
 * (C) Copyright 2004
 * Philippe Robin, unicore Ltd. <philippe.robin@unicore.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/ptrace.h>

#ifdef CONFIG_USE_IRQ
DECLARE_GLOBAL_DATA_PTR;

int interrupt_init (void)
{
	/*
	 * setup up stacks if necessary
	 */
	IRQ_STACK_START = _unicoreboot_start - CONFIG_SYS_MALLOC_LEN - CONFIG_SYS_GBL_DATA_SIZE - 4;
	FIQ_STACK_START = IRQ_STACK_START - CONFIG_STACKSIZE_IRQ;
	return arch_interrupt_init();
}

/* enable IRQ interrupts */
void enable_interrupts (void)
{
	unsigned long temp;
	__asm__ __volatile__("mov %0, asr\n"
			     "andn %0, %0, #0x80\n"
			     "mov.a asr, %0"
			     : "=r" (temp)
			     :
			     : "memory", "cc");
}


/*
 * disable IRQ/FIQ interrupts
 * returns true if interrupts had been enabled before we disabled them
 */
int disable_interrupts (void)
{
	unsigned long old,temp;
	__asm__ __volatile__("mov %0, asr\n"
			     "or %1, %0, #0xc0\n"
			     "mov.a asr, %1"
			     : "=r" (old), "=r" (temp)
			     :
			     : "memory", "cc");
	return (old & 0x80) == 0;
}
#else
void enable_interrupts (void)
{
	return;
}
int disable_interrupts (void)
{
	return 0;
}
#endif


void bad_mode (void)
{
	panic ("Resetting CPU ...\n");
	//reset_cpu (0);
}

void show_regs (struct pt_regs *regs)
{
unsigned long flags;

	static const char *processor_modes[] = {
		"UK00" , "UK01" , "UK02" , "UK03" , "UK04" , "UK05" , "UK06" , "UK07" ,
		"UK08" , "UK09" , "UK0A" , "UK0B" , "UK0C" , "UK0D" , "UK0E" , "UK0F" ,
		"USER" , "REAL" , "INTR" , "PRIV" , "UK14" , "UK15" , "UK16" , "ABRT" ,
		"UK18" , "UK19" , "UK1A" , "EXTN" , "UK1C" , "UK1D" , "UK1E" , "SUSR"
	};

	flags = condition_codes (regs);

	printf ("pc : [<%08lx>]	   lr : [<%08lx>]\n"
		"sp : %08lx  ip : %08lx	 fp : %08lx\n",
		instruction_pointer (regs),
		regs->UCreg_lr, regs->UCreg_sp, regs->UCreg_ip, regs->UCreg_fp);
	printf ("r10: %08lx  r9 : %08lx	 r8 : %08lx\n",
		regs->UCreg_10, regs->UCreg_09, regs->UCreg_08);
	printf ("r7 : %08lx  r6 : %08lx	 r5 : %08lx  r4 : %08lx\n",
		regs->UCreg_07, regs->UCreg_06, regs->UCreg_05, regs->UCreg_04);
	printf ("r3 : %08lx  r2 : %08lx	 r1 : %08lx  r0 : %08lx\n",
		regs->UCreg_03, regs->UCreg_02, regs->UCreg_01, regs->UCreg_00);
	printf ("Flags: %c%c%c%c",
		flags & PSR_S_BIT ? 'S' : 's',
		flags & PSR_Z_BIT ? 'Z' : 'z',
		flags & PSR_C_BIT ? 'C' : 'c', flags & PSR_V_BIT ? 'V' : 'v');
	printf ("  IRQs %s  FIQs %s  Mode %s\n",
		interrupts_enabled (regs) ? "on" : "off",
		fast_interrupts_enabled (regs) ? "on" : "off",
		processor_modes[processor_mode (regs)]);
}

void do_undefined_instruction (struct pt_regs *pt_regs)
{
	printf ("undefined instruction\n");
	show_regs (pt_regs);
	bad_mode ();
}

void do_software_interrupt (struct pt_regs *pt_regs)
{
	printf ("software interrupt\n");
	show_regs (pt_regs);
	bad_mode ();
}

void do_prefetch_abort (struct pt_regs *pt_regs)
{
	printf ("prefetch abort\n");
	show_regs (pt_regs);
	bad_mode ();
}

void do_data_abort (struct pt_regs *pt_regs)
{
	printf ("data abort\n");
	show_regs (pt_regs);
	bad_mode ();
}

void do_not_used (struct pt_regs *pt_regs)
{
	printf ("not used\n");
	show_regs (pt_regs);
	bad_mode ();
}

void do_fiq (struct pt_regs *pt_regs)
{
	printf ("fast interrupt request\n");
	show_regs (pt_regs);
	bad_mode ();
}

#ifndef CONFIG_USE_IRQ
void do_irq (struct pt_regs *pt_regs)
{
	printf ("interrupt request\n");
	show_regs (pt_regs);
	bad_mode ();
}
#endif
