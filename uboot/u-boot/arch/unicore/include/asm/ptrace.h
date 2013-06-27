#ifndef __ASM_UNICORE_PTRACE_H
#define __ASM_UNICORE_PTRACE_H

#define PTRACE_GETREGS		12
#define PTRACE_SETREGS		13
#define PTRACE_GETFPREGS	14
#define PTRACE_SETFPREGS	15

#define PTRACE_SETOPTIONS	21
#define PTRACE_GET_THREAD_AREA	22
#define PTRACE_SET_SYSCALL	23

/* options set using PTRACE_SETOPTIONS */
#define PTRACE_O_TRACESYSGOOD	0x00000001

#include <asm/proc-ucv2/ptrace.h>

#ifndef __ASSEMBLY__
#define pc_pointer(v) \
	((v) & ~PCMASK)

#define instruction_pointer(regs) \
	(pc_pointer((regs)->UCreg_pc))

#ifdef __KERNEL__
extern void show_regs(struct pt_regs *);

#define predicate(x)	(x & 0xf0000000)
#define PREDICATE_ALWAYS	0xe0000000

#endif

#endif /* __ASSEMBLY__ */

#endif
