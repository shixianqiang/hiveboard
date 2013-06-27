/*
 *  include/asm-unicore/proc-ucv2/processor.h
 */

#ifndef __ASM_PROC_PROCESSOR_H
#define __ASM_PROC_PROCESSOR_H

#include <asm/proc/domain.h>

#define KERNEL_STACK_SIZE	PAGE_SIZE

struct context_save_struct {
	unsigned long cpsr;
	unsigned long r4;
	unsigned long r5;
	unsigned long r6;
	unsigned long r7;
	unsigned long r8;
	unsigned long r9;
	unsigned long sl;
	unsigned long fp;
	unsigned long pc;
};

#define INIT_CSS (struct context_save_struct){ SVC_MODE, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

#define EXTRA_THREAD_STRUCT						\
	unsigned int	domain;

#define EXTRA_THREAD_STRUCT_INIT					\
	domain:	  domain_val(DOMAIN_USER, DOMAIN_CLIENT) |		\
		  domain_val(DOMAIN_KERNEL, DOMAIN_MANAGER) |		\
		  domain_val(DOMAIN_IO, DOMAIN_CLIENT)

#define start_thread(regs,pc,sp)					\
({									\
	unsigned long *stack = (unsigned long *)sp;			\
	set_fs(USER_DS);						\
	memzero(regs->uregs, sizeof(regs->uregs));			\
	if (current->personality & ADDR_LIMIT_32BIT)			\
		regs->UNICORE_cpsr = USR_MODE;				\
	else								\
		regs->UNICORE_cpsr = USR26_MODE;				\
	regs->UNICORE_pc = pc;		/* pc */			\
	regs->UNICORE_sp = sp;		/* sp */			\
	regs->UNICORE_r2 = stack[2];	/* r2 (envp) */			\
	regs->UNICORE_r1 = stack[1];	/* r1 (argv) */			\
	regs->UNICORE_r0 = stack[0];	/* r0 (argc) */			\
})

#define KSTK_EIP(tsk)	(((unsigned long *)(4096+(unsigned long)(tsk)))[1019])
#define KSTK_ESP(tsk)	(((unsigned long *)(4096+(unsigned long)(tsk)))[1017])

/* Allocation and freeing of basic task resources. */
/*
 * NOTE! The task struct and the stack go together
 */
#define ll_alloc_task_struct() ((struct task_struct *) __get_free_pages(GFP_KERNEL,1))
#define ll_free_task_struct(p) free_pages((unsigned long)(p),1)

#endif
