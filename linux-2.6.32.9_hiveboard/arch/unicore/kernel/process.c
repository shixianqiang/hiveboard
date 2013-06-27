/*
 * linux/arch/unicore/kernel/process.c
 *
 * Code specific to PKUnity SoC and UniCore ISA
 * Fragments that appear the same as the files in arm or x86
 *
 * Copyright (C) 2001-2008 GUAN Xue-tao
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <stdarg.h>

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/stddef.h>
#include <linux/unistd.h>
#include <linux/slab.h>
#include <linux/user.h>
#include <linux/delay.h>
#include <linux/reboot.h>
#include <linux/interrupt.h>
#include <linux/kallsyms.h>
#include <linux/init.h>
#include <linux/cpu.h>
#include <linux/elfcore.h>
#include <linux/pm.h>
#include <linux/tick.h>
#include <linux/utsname.h>
#include <linux/uaccess.h>

//##########commented by zhangyang 2010-05-13 16:58
//#include <asm/gpio.h>
//#include <asm/leds.h>
#include <asm/processor.h>
#include <asm/system.h>
#include <asm/thread_notify.h>
#include <asm/stacktrace.h>
#include <asm/mach/time.h>

extern void misc_write(void);

static const char *processor_modes[] = {
  "UK00" , "UK01" , "UK02" , "UK03" , "UK04" , "UK05" , "UK06" , "UK07" ,
  "UK08" , "UK09" , "UK0A" , "UK0B" , "UK0C" , "UK0D" , "UK0E" , "UK0F" ,
  "USER" , "REAL" , "INTR" , "PRIV" , "UK14" , "UK15" , "UK16" , "ABRT" ,
  "UK18" , "UK19" , "UK1A" , "EXTN" , "UK1C" , "UK1D" , "UK1E" , "SUSR"
};

static const char *isa_modes[] = {
  "UniCore-II" , "UniCore-16"
};

extern void setup_mm_for_reboot(char mode);

static volatile int hlt_counter;

#include <mach/system.h>

void disable_hlt(void)
{
	hlt_counter++;
}

EXPORT_SYMBOL(disable_hlt);

void enable_hlt(void)
{
	hlt_counter--;
}

EXPORT_SYMBOL(enable_hlt);

static int __init nohlt_setup(char *__unused)
{
	hlt_counter = 1;
	return 1;
}

static int __init hlt_setup(char *__unused)
{
	hlt_counter = 0;
	return 1;
}

__setup("nohlt", nohlt_setup);
__setup("hlt", hlt_setup);

void unicore_machine_poweroff(void)
{
	arch_poweroff();	
}

void unicore_machine_restart(char mode, const char *cmd)
{      
	if(!strcmp(cmd, "recovery")){
    	misc_write();
        printk("Misc write BCB\n");     
    }
      
	/*
	 * Clean dcache, and turn off interrupts
	 */
	cpu_proc_fin();

	/*
	 * Tell the mm system that we are going to reboot -
	 * we may need it to insert some 1:1 mappings so that
	 * soft boot works.
	 */
	setup_mm_for_reboot(mode);

	/*
	 * Now call the architecture specific reboot code.
	 */
	arch_reset(mode, cmd);

	/*
	 * Whoops - the architecture was unable to reboot.
	 * Tell the user!
	 */
	mdelay(1000);
	printk("Reboot failed -- System halted\n");
	while (1);
}

/*
 * Function pointers to optional machine specific functions
 */
void (*pm_power_off)(void) = unicore_machine_poweroff;
EXPORT_SYMBOL(pm_power_off);

void (*unicore_pm_restart)(char str, const char *cmd) = unicore_machine_restart;
EXPORT_SYMBOL_GPL(unicore_pm_restart);


/*
 * This is our default idle handler.  We need to disable
 * interrupts here to ensure we don't miss a wakeup call.
 */
static void default_idle(void)
{
	if (!need_resched())
		arch_idle();
	local_irq_enable();
}

void (*pm_idle)(void) = default_idle;
EXPORT_SYMBOL(pm_idle);

/*
 * The idle thread, has rather strange semantics for calling pm_idle,
 * but this is what x86 does and we need to do the same, so that
 * things like cpuidle get called in the same way.  The only difference
 * is that we always respect 'hlt_counter' to prevent low power idle.
 */
void cpu_idle(void)
{
	local_fiq_enable();

	/* endless idle loop with no priority at all */
	while (1) {
		tick_nohz_stop_sched_tick(1);
//########### commented by zhangyang 2010-05-13 17:00
//		leds_event(led_idle_start);
		while (!need_resched()) {

			local_irq_disable();
			if (hlt_counter) {
				local_irq_enable();
				cpu_relax();
			} else {
				stop_critical_timings();
				pm_idle();
				start_critical_timings();
				/*
				 * This will eventually be removed - pm_idle
				 * functions should always return with IRQs
				 * enabled.
				 */
				WARN_ON(irqs_disabled());
				local_irq_enable();
			}
		}
//########### commented by zhangyang 2010-05-13 17:00
//		leds_event(led_idle_end);
		tick_nohz_restart_sched_tick();
		preempt_enable_no_resched();
		schedule();
		preempt_disable();
	}
}

static char reboot_mode = 'h';

int __init reboot_setup(char *str)
{
	reboot_mode = str[0];
	return 1;
}

__setup("reboot=", reboot_setup);

void machine_halt(void)
{
//######### commented by zhangyang 2010-05-13 17:01
//        gpio_set_value(GPO_SOFT_OFF, 0);
}


void machine_power_off(void)
{
	if (pm_power_off)
		pm_power_off();
        machine_halt();
}

void machine_restart(char *cmd)
{
	unicore_pm_restart(reboot_mode, cmd);
}

void __show_regs(struct pt_regs *regs)
{
	unsigned long flags;
	char buf[64];

	printk("CPU: %d    %s  (%s %.*s)\n",
		smp_processor_id(), print_tainted(), init_utsname()->release,
		(int)strcspn(init_utsname()->version, " "),
		init_utsname()->version);
	print_symbol("PC is at %s\n", instruction_pointer(regs));
	print_symbol("LR is at %s\n", regs->UCreg_lr);
	printk("pc : [<%08lx>]    lr : [<%08lx>]    psr: %08lx\n"
	       "sp : %08lx  ip : %08lx  fp : %08lx\n",
		regs->UCreg_pc, regs->UCreg_lr, regs->UCreg_csr,
		regs->UCreg_sp, regs->UCreg_ip, regs->UCreg_fp);
	printk("r26: %08lx  r25: %08lx  r24: %08lx\n",
		regs->UCreg_26, regs->UCreg_25,
		regs->UCreg_24);
	printk("r23: %08lx  r22: %08lx  r21: %08lx  r20: %08lx\n",
		regs->UCreg_23, regs->UCreg_22,
		regs->UCreg_21, regs->UCreg_20);
	printk("r19: %08lx  r18: %08lx  r17: %08lx  r16: %08lx\n",
		regs->UCreg_19, regs->UCreg_18,
		regs->UCreg_17, regs->UCreg_16);
	printk("r15: %08lx  r14: %08lx  r13: %08lx  r12: %08lx\n",
		regs->UCreg_15, regs->UCreg_14,
		regs->UCreg_13, regs->UCreg_12);
	printk("r11: %08lx  r10: %08lx  r9 : %08lx  r8 : %08lx\n",
		regs->UCreg_11, regs->UCreg_10,
		regs->UCreg_09, regs->UCreg_08);
	printk("r7 : %08lx  r6 : %08lx  r5 : %08lx  r4 : %08lx\n",
		regs->UCreg_07, regs->UCreg_06,
		regs->UCreg_05, regs->UCreg_04);
	printk("r3 : %08lx  r2 : %08lx  r1 : %08lx  r0 : %08lx\n",
		regs->UCreg_03, regs->UCreg_02,
		regs->UCreg_01, regs->UCreg_00);

	flags = regs->UCreg_csr;
	buf[0] = flags & PSR_S_BIT ? 'S' : 's';
	buf[1] = flags & PSR_Z_BIT ? 'Z' : 'z';
	buf[2] = flags & PSR_C_BIT ? 'C' : 'c';
	buf[3] = flags & PSR_V_BIT ? 'V' : 'v';
	buf[4] = '\0';

	printk("Flags: %s  INTR o%s  REAL o%s  Mode %s  ISA %s  Segment %s\n",
		buf, interrupts_enabled(regs) ? "n" : "ff",
		fast_interrupts_enabled(regs) ? "n" : "ff",
		processor_modes[processor_mode(regs)],
		isa_modes[isa_mode(regs)],
		get_fs() == get_ds() ? "kernel" : "user");
	{
		unsigned int ctrl;

		buf[0] = '\0';
		{
			unsigned int transbase;
			asm("movc %0, p0.c2, #0\n"
			    : "=r" (transbase));
			snprintf(buf, sizeof(buf), "  Table: %08x",
			  	transbase);
		}
		asm("movc %0, p0.c1, #0\n" : "=r" (ctrl));

		printk("Control: %08x%s\n", ctrl, buf);
	}
}

void show_regs(struct pt_regs * regs)
{
	printk("\n");
	printk("Pid: %d, comm: %20s\n", task_pid_nr(current), current->comm);
	__show_regs(regs);
	__backtrace();
}

/*
 * Free current thread data structures etc..
 */
void exit_thread(void)
{
}

ATOMIC_NOTIFIER_HEAD(thread_notify_head);

EXPORT_SYMBOL_GPL(thread_notify_head);

void flush_thread(void)
{
	struct thread_info *thread = current_thread_info();
	struct task_struct *tsk = current;

	memset(thread->used_cp, 0, sizeof(thread->used_cp));
	memset(&tsk->thread.debug, 0, sizeof(struct debug_info));
#ifdef  CONFIG_UNICORE_FPU_F64	
	memset(&thread->fpstate, 0, sizeof(union fp_state));
#endif

	thread_notify(THREAD_NOTIFY_FLUSH, thread);
}

void release_thread(struct task_struct *dead_task)
{
	struct thread_info *thread = task_thread_info(dead_task);

	thread_notify(THREAD_NOTIFY_RELEASE, thread);
}

asmlinkage void ret_from_fork(void) __asm__("ret_from_fork");

int
copy_thread(unsigned long clone_flags, unsigned long stack_start,
	    unsigned long stk_sz, struct task_struct *p, struct pt_regs *regs)
{
	struct thread_info *thread = task_thread_info(p);
	struct pt_regs *childregs = task_pt_regs(p);

	*childregs = *regs;
	childregs->UCreg_00 = 0;
	childregs->UCreg_sp = stack_start;

	memset(&thread->cpu_context, 0, sizeof(struct cpu_context_save));
	thread->cpu_context.sp = (unsigned long)childregs;
	thread->cpu_context.pc = (unsigned long)ret_from_fork;

	if (clone_flags & CLONE_SETTLS)
		childregs->UCreg_16 = regs->UCreg_03;

	return 0;
}

/*
 * Fill in the task's elfregs structure for a core dump.
 */
int dump_task_regs(struct task_struct *t, elf_gregset_t *elfregs)
{
	elf_core_copy_regs(elfregs, task_pt_regs(t));
	return 1;
}

/*
 * fill in the fpe structure for a core dump...
 */
int dump_fpu (struct pt_regs *regs, struct user_fp *fp)
{
	struct thread_info *thread = current_thread_info();
	int used_math = thread->used_cp[1] | thread->used_cp[2];

#ifdef CONFIG_UNICORE_FPU_F64
	if (used_math)
		memcpy(fp, &thread->fpstate.soft, sizeof (*fp));
#endif
	return used_math != 0;
}
EXPORT_SYMBOL(dump_fpu);

/*
 * Shuffle the argument into the correct register before calling the
 * thread function.  r1 is the thread argument, r2 is the pointer to
 * the thread function, and r3 points to the exit function.
 */
extern void kernel_thread_helper(void);
asm(	".section .text\n"
"	.align\n"
"	.type	kernel_thread_helper, #function\n"
"kernel_thread_helper:\n"
"	mov	r0, r1\n"
"	mov	lr, r3\n"
"	mov	pc, r2\n"
"	.size	kernel_thread_helper, . - kernel_thread_helper\n"
"	.previous");

/*
 * Create a kernel thread.
 */
pid_t kernel_thread(int (*fn)(void *), void *arg, unsigned long flags)
{
	struct pt_regs regs;

	memset(&regs, 0, sizeof(regs));

	regs.UCreg_01 = (unsigned long)arg;
	regs.UCreg_02 = (unsigned long)fn;
	regs.UCreg_03 = (unsigned long)do_exit;
	regs.UCreg_pc = (unsigned long)kernel_thread_helper;
	regs.UCreg_csr = PRIV_MODE;

	return do_fork(flags|CLONE_VM|CLONE_UNTRACED, 0, &regs, 0, NULL, NULL);
}
EXPORT_SYMBOL(kernel_thread);

unsigned long get_wchan(struct task_struct *p)
{
	struct stackframe frame;
	int count = 0;
	if (!p || p == current || p->state == TASK_RUNNING)
		return 0;

	frame.fp = thread_saved_fp(p);
	frame.sp = thread_saved_sp(p);
	frame.lr = 0;			/* recovered from the stack */
	frame.pc = thread_saved_pc(p);
	do {
		int ret = unwind_frame(&frame);
		if (ret < 0)
			return 0;
		if (!in_sched_functions(frame.pc))
			return frame.pc;
	} while (count ++ < 16);
	return 0;
}
