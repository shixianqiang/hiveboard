/*
 *  include/asm-unicore/proc-ucv2/ptrace.h
 */

#ifndef __ASM_UNICORE_PROC_PTRACE_H
#define __ASM_UNICORE_PROC_PTRACE_H

#include <linux/config.h>

/*
 * PSR bits
 */
#define USER_MODE	0x00000010
#define REAL_MODE	0x00000011
#define INTR_MODE	0x00000012
#define PRIV_MODE	0x00000013
#define ABRT_MODE	0x00000017
#define EXTN_MODE	0x0000001b
#define SUSR_MODE	0x0000001f
#define MODE_MASK	0x0000001f
#define PSR_R_BIT	0x00000040
#define PSR_I_BIT	0x00000080
#define PSR_V_BIT	0x10000000
#define PSR_C_BIT	0x20000000
#define PSR_Z_BIT	0x40000000
#define PSR_S_BIT	0x80000000
#define PCMASK		0

/*
 * Groups of PSR bits
 */
#define PSR_f		0xff000000	/* Flags		*/
#define PSR_s		0x00ff0000	/* Status		*/
#define PSR_x		0x0000ff00	/* Extension		*/
#define PSR_c		0x000000ff	/* Control		*/

#ifndef __ASSEMBLY__

/* this struct defines the way the registers are stored on the
   stack during a system call. */

struct pt_regs {
	long uregs[34];
};

#define UCreg_csr	uregs[32]
#define UCreg_pc	uregs[31]
#define UCreg_lr	uregs[30]
#define UCreg_sp	uregs[29]
#define UCreg_ip	uregs[28]
#define UCreg_fp	uregs[27]
#define UCreg_26	uregs[26]
#define UCreg_25	uregs[25]
#define UCreg_24	uregs[24]
#define UCreg_23	uregs[23]
#define UCreg_22	uregs[22]
#define UCreg_21	uregs[21]
#define UCreg_20	uregs[20]
#define UCreg_19	uregs[19]
#define UCreg_18	uregs[18]
#define UCreg_17	uregs[17]
#define UCreg_16	uregs[16]
#define UCreg_15	uregs[15]
#define UCreg_14	uregs[14]
#define UCreg_13	uregs[13]
#define UCreg_12	uregs[12]
#define UCreg_11	uregs[11]
#define UCreg_10	uregs[10]
#define UCreg_09	uregs[9]
#define UCreg_08	uregs[8]
#define UCreg_07	uregs[7]
#define UCreg_06	uregs[6]
#define UCreg_05	uregs[5]
#define UCreg_04	uregs[4]
#define UCreg_03	uregs[3]
#define UCreg_02	uregs[2]
#define UCreg_01	uregs[1]
#define UCreg_00	uregs[0]
#define UCreg_ORIG_00	uregs[33]

#ifdef __KERNEL__

#define user_mode(regs)	\
	(processor_mode(regs) == USER_MODE)

#define processor_mode(regs) \
	((regs)->UCreg_csr & MODE_MASK)

#define interrupts_enabled(regs) \
	(!((regs)->UCreg_csr & PSR_I_BIT))

#define fast_interrupts_enabled(regs) \
	(!((regs)->UCreg_csr & PSR_R_BIT))

#define condition_codes(regs) \
	((regs)->UCreg_csr & (PSR_V_BIT|PSR_C_BIT|PSR_Z_BIT|PSR_S_BIT))

/* Are the current registers suitable for user mode?
 * (used to maintain security in signal handlers)
 */
static inline int valid_user_regs(struct pt_regs *regs)
{
	if ((regs->UCreg_csr & 0xf) == 0 &&
	    (regs->UCreg_csr & (PSR_R_BIT|PSR_I_BIT)) == 0)
		return 1;

	/*
	 * Force CSR to something logical...
	 */
	regs->UCreg_csr &= PSR_f | PSR_s | PSR_x | USER_MODE;

	return 0;
}

#endif

#endif /* __ASSEMBLY__ */

#endif
