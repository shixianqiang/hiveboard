/*
 * linux/arch/unicore/mm/alignment.c
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
/*
 * TODO:
 *  FPU ldm/stm not handling
 */
#include <linux/compiler.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

#include <asm/unaligned.h>

#include "fault.h"

#define SVC_SPACE	0xc0000000
#define CODING_BITS(i)	(i & 0xe0000120)

#define LDST_P_BIT(i)	(i & (1 << 28))		/* Preindex		*/
#define LDST_U_BIT(i)	(i & (1 << 27))		/* Add offset		*/
#define LDST_W_BIT(i)	(i & (1 << 25))		/* Writeback		*/
#define LDST_L_BIT(i)	(i & (1 << 24))		/* Load			*/

#define LDST_P_EQ_U(i)	((((i) ^ ((i) >> 1)) & (1 << 27)) == 0)

#define LDSTH_I_BIT(i)	(i & (1 << 26))		/* half-word immed	*/
#define LDM_S_BIT(i)	(i & (1 << 26))		/* write CPSR from SPSR	*/
#define LDM_H_BIT(i)	(i & (1 << 6))		/* select r0-r15 or r16-r31 */

#define RN_BITS(i)	((i >> 19) & 31)	/* Rn			*/
#define RD_BITS(i)	((i >> 14) & 31)	/* Rd			*/
#define RM_BITS(i)	(i & 31)		/* Rm			*/

#define REGMASK_BITS(i)	(((i & 0x7fe00) >> 3) | (i & 0x3f))
#define OFFSET_BITS(i)	(i & 0x03fff)

#define SHIFT_BITS(i)	((i >> 9) & 0x1f)
#define SHIFT_TYPE(i)	(i & 0xc0)
#define SHIFT_LSL	0x00
#define SHIFT_LSR	0x40
#define SHIFT_ASR	0x80
#define SHIFT_RORRRX	0xc0

static unsigned long ai_user;
static unsigned long ai_sys;
static unsigned long ai_skipped;
static unsigned long ai_half;
static unsigned long ai_word;
static unsigned long ai_multi;
static int ai_usermode;

#define UM_WARN		(1 << 0)
#define UM_FIXUP	(1 << 1)
#define UM_SIGNAL	(1 << 2)

#ifdef CONFIG_PROC_FS
static const char *usermode_action[] = {
	"ignored",
	"warn",
	"fixup",
	"fixup+warn",
	"signal",
	"signal+warn"
};

static int
proc_alignment_read(char *page, char **start, off_t off, int count, int *eof,
		    void *data)
{
	char *p = page;
	int len;

	p += sprintf(p, "User:\t\t%lu\n", ai_user);
	p += sprintf(p, "System:\t\t%lu\n", ai_sys);
	p += sprintf(p, "Skipped:\t%lu\n", ai_skipped);
	p += sprintf(p, "Half:\t\t%lu\n", ai_half);
	p += sprintf(p, "Word:\t\t%lu\n", ai_word);
	p += sprintf(p, "Multi:\t\t%lu\n", ai_multi);
	p += sprintf(p, "User faults:\t%i (%s)\n", ai_usermode,
			usermode_action[ai_usermode]);

	len = (p - page) - off;
	if (len < 0)
		len = 0;

	*eof = (len <= count) ? 1 : 0;
	*start = page + off;

	return len;
}

static int proc_alignment_write(struct file *file, const char __user *buffer,
				unsigned long count, void *data)
{
	char mode;

	if (count > 0) {
		if (get_user(mode, buffer))
			return -EFAULT;
		if (mode >= '0' && mode <= '5')
			ai_usermode = mode - '0';
	}
	return count;
}

#endif /* CONFIG_PROC_FS */

union offset_union {
	unsigned long un;
	  signed long sn;
};

#define TYPE_ERROR	0
#define TYPE_FAULT	1
#define TYPE_LDST	2
#define TYPE_DONE	3
#define TYPE_SWAP  4
#define TYPE_COLS  5   /* Coprocessor load/store */

#define get8_unaligned_check(val,addr,err)		\
	__asm__(					\
	"1:	ldb.u	%1, [%2], #1\n"			\
	"2:\n"						\
	"	.section .fixup,\"ax\"\n"		\
	"	.align	2\n"				\
	"3:	mov	%0, #1\n"			\
	"	b	2b\n"				\
	"	.previous\n"				\
	"	.section __ex_table,\"a\"\n"		\
	"	.align	3\n"				\
	"	.long	1b, 3b\n"			\
	"	.previous\n"				\
	: "=r" (err), "=&r" (val), "=r" (addr)		\
	: "0" (err), "2" (addr))

#define get8t_unaligned_check(val,addr,err)		\
	__asm__(					\
	"1:	ldb.u	%1, [%2], #1\n"			\
	"2:\n"						\
	"	.section .fixup,\"ax\"\n"		\
	"	.align	2\n"				\
	"3:	mov	%0, #1\n"			\
	"	b	2b\n"				\
	"	.previous\n"				\
	"	.section __ex_table,\"a\"\n"		\
	"	.align	3\n"				\
	"	.long	1b, 3b\n"			\
	"	.previous\n"				\
	: "=r" (err), "=&r" (val), "=r" (addr)		\
	: "0" (err), "2" (addr))

#define get16_unaligned_check(val,addr)				\
	do {							\
		unsigned int err = 0, v, a = addr;		\
		get8_unaligned_check(val,a,err);		\
		get8_unaligned_check(v,a,err);			\
		val |= v << 8;					\
		if (err)					\
			goto fault;				\
	} while (0)

#define put16_unaligned_check(val,addr)				\
	do {							\
		unsigned int err = 0, v = val, a = addr;	\
		__asm__(					\
		"1:	stb.u	%1, [%2], #1\n"			\
		"	mov	%1, %1 >> #8\n"			\
		"2:	stb.u	%1, [%2]\n"			\
		"3:\n"						\
		"	.section .fixup,\"ax\"\n"		\
		"	.align	2\n"				\
		"4:	mov	%0, #1\n"			\
		"	b	3b\n"				\
		"	.previous\n"				\
		"	.section __ex_table,\"a\"\n"		\
		"	.align	3\n"				\
		"	.long	1b, 4b\n"			\
		"	.long	2b, 4b\n"			\
		"	.previous\n"				\
		: "=r" (err), "=&r" (v), "=&r" (a)		\
		: "0" (err), "1" (v), "2" (a));			\
		if (err)					\
			goto fault;				\
	} while (0)

#define __put32_unaligned_check(ins,val,addr)			\
	do {							\
		unsigned int err = 0, v = val, a = addr;	\
		__asm__(					\
		"1:	"ins"	%1, [%2], #1\n"			\
		"	mov	%1, %1 >> #8\n"			\
		"2:	"ins"	%1, [%2], #1\n"			\
		"	mov	%1, %1 >> #8\n"			\
		"3:	"ins"	%1, [%2], #1\n"			\
		"	mov	%1, %1 >> #8\n"			\
		"4:	"ins"	%1, [%2]\n"			\
		"5:\n"						\
		"	.section .fixup,\"ax\"\n"		\
		"	.align	2\n"				\
		"6:	mov	%0, #1\n"			\
		"	b	5b\n"				\
		"	.previous\n"				\
		"	.section __ex_table,\"a\"\n"		\
		"	.align	3\n"				\
		"	.long	1b, 6b\n"			\
		"	.long	2b, 6b\n"			\
		"	.long	3b, 6b\n"			\
		"	.long	4b, 6b\n"			\
		"	.previous\n"				\
		: "=r" (err), "=&r" (v), "=&r" (a)		\
		: "0" (err), "1" (v), "2" (a));			\
		if (err)					\
			goto fault;				\
	} while (0)

#define get32_unaligned_check(val,addr)				\
	do {							\
		unsigned int err = 0, v, a = addr;		\
		get8_unaligned_check(val,a,err);		\
		get8_unaligned_check(v,a,err);			\
		val |= v << 8;					\
		get8_unaligned_check(v,a,err);			\
		val |= v << 16;					\
		get8_unaligned_check(v,a,err);			\
		val |= v << 24;					\
		if (err)					\
			goto fault;				\
	} while (0)

#define put32_unaligned_check(val,addr)	 \
	__put32_unaligned_check("stb.u", val, addr)

#define get32t_unaligned_check(val,addr)			\
	do {							\
		unsigned int err = 0, v, a = addr;		\
		get8t_unaligned_check(val,a,err);		\
		get8t_unaligned_check(v,a,err);			\
		val |= v << 8;					\
		get8t_unaligned_check(v,a,err);			\
		val |= v << 16;					\
		get8t_unaligned_check(v,a,err);			\
		val |= v << 24;					\
		if (err)					\
			goto fault;				\
	} while (0)

#define put32t_unaligned_check(val,addr) \
	__put32_unaligned_check("stb.u", val, addr)

static void
do_alignment_finish_ldst(unsigned long addr, unsigned long instr, struct pt_regs *regs, union offset_union offset)
{
	if (!LDST_U_BIT(instr))
		offset.un = -offset.un;

	if (!LDST_P_BIT(instr))
		addr += offset.un;

	if (!LDST_P_BIT(instr) || LDST_W_BIT(instr))
		regs->uregs[RN_BITS(instr)] = addr;
}

static int
do_alignment_ldrhstrh(unsigned long addr, unsigned long instr, struct pt_regs *regs)
{
	unsigned int rd = RD_BITS(instr);

	if ((instr & 0x4b003fe0) == 0x40000120)    /* old value 0x40002120, can't judge swap instr correctly */
		goto swp;

	ai_half += 1;

	if (LDST_L_BIT(instr)) {
		unsigned long val;
		get16_unaligned_check(val, addr);

		/* signed half-word? */
		if (instr & 0x80)
			val = (signed long)((signed short) val);

		regs->uregs[rd] = val;
	} else
		put16_unaligned_check(regs->uregs[rd], addr);

	return TYPE_LDST;

swp:
        /* only handle swap word, for swap byte should not active this alignment exception */
        get32_unaligned_check(regs->uregs[RD_BITS(instr)], addr);
        put32_unaligned_check(regs->uregs[RM_BITS(instr)], addr);
        return TYPE_SWAP;

fault:
	return TYPE_FAULT;
}

static int
do_alignment_ldrstr(unsigned long addr, unsigned long instr, struct pt_regs *regs)
{
	unsigned int rd = RD_BITS(instr);

	ai_word += 1;

	if (!LDST_P_BIT(instr) && LDST_W_BIT(instr))
		goto trans;

	if (LDST_L_BIT(instr))
		get32_unaligned_check(regs->uregs[rd], addr);
	else
		put32_unaligned_check(regs->uregs[rd], addr);
	return TYPE_LDST;

trans:
	if (LDST_L_BIT(instr))
		get32t_unaligned_check(regs->uregs[rd], addr);
	else
		put32t_unaligned_check(regs->uregs[rd], addr);
	return TYPE_LDST;

fault:
	return TYPE_FAULT;
}

/*
 * LDM/STM alignment handler.
 *
 * There are 4 variants of this instruction:
 *
 * B = rn pointer before instruction, A = rn pointer after instruction
 *              ------ increasing address ----->
 *	        |    | r0 | r1 | ... | rx |    |
 * PU = 01             B                    A
 * PU = 11        B                    A
 * PU = 00        A                    B
 * PU = 10             A                    B
 */
static int
do_alignment_ldmstm(unsigned long addr, unsigned long instr, struct pt_regs *regs)
{
	unsigned int rd, rn, pc_correction, reg_correction, nr_regs, regbits;
	unsigned long eaddr, newaddr;

	if (LDM_S_BIT(instr))
		goto bad;

	pc_correction = 4; /* processor implementation defined */
	ai_multi += 1;

	/* count the number of registers in the mask to be transferred */
	nr_regs = hweight16(REGMASK_BITS(instr)) * 4;

	rn = RN_BITS(instr);
	newaddr = eaddr = regs->uregs[rn];

	if (!LDST_U_BIT(instr))
		nr_regs = -nr_regs;
	newaddr += nr_regs;
	if (!LDST_U_BIT(instr))
		eaddr = newaddr;

	if (LDST_P_EQ_U(instr))	/* U = P */
		eaddr += 4;

	/*
	 * This is a "hint" - we already have eaddr worked out by the
	 * processor for us.
	 */
	if (addr != eaddr) {
		printk(KERN_ERR "LDMSTM: PC = %08lx, instr = %08lx, "
			"addr = %08lx, eaddr = %08lx\n",
			 instruction_pointer(regs), instr, addr, eaddr);
		show_regs(regs);
	}

	if (LDM_H_BIT(instr))
		reg_correction = 0x10;
	else
		reg_correction = 0x00;	

	for (regbits = REGMASK_BITS(instr), rd = 0; regbits; regbits >>= 1, rd += 1)
		if (regbits & 1) {
			if (LDST_L_BIT(instr))
				get32_unaligned_check(regs->uregs[rd + reg_correction], eaddr);
			else
				put32_unaligned_check(regs->uregs[rd + reg_correction], eaddr);
			eaddr += 4;
		}

	if (LDST_W_BIT(instr))
		regs->uregs[rn] = newaddr;
	return TYPE_DONE;

fault:
	regs->UCreg_pc -= pc_correction;
	return TYPE_FAULT;

bad:
	printk(KERN_ERR "Alignment trap: not handling ldm with s-bit set\n");
	return TYPE_ERROR;
}

static int
do_alignment(unsigned long addr, unsigned int error_code, struct pt_regs *regs)
{
	union offset_union offset;
	unsigned long instr, instrptr;
	int (*handler)(unsigned long addr, unsigned long instr, struct pt_regs *regs);
	unsigned int type;

	if (user_mode(regs))
		ai_user += 1;
	else
		ai_sys += 1;

	instrptr = instruction_pointer(regs);
	if (instrptr >= SVC_SPACE)
		instr = *(unsigned long *)instrptr;
	else
	{
		__asm__ __volatile__(
			"ldw.u	%0, [%1]\n"
			:"=&r"(instr)
			:"r"(instrptr));
	}


	regs->UCreg_pc += 4;

	switch (CODING_BITS(instr)) {
	case 0x40000120:	/* ldrh or strh */
		if (LDSTH_I_BIT(instr))
			offset.un = (instr & 0x3e00) >> 4 | (instr & 31);
		else
			offset.un = regs->uregs[RM_BITS(instr)];
		handler = do_alignment_ldrhstrh;
		break;

	case 0x60000000:	/* ldr or str immediate */
	case 0x60000100:	/* ldr or str immediate */
	case 0x60000020:	/* ldr or str immediate */
	case 0x60000120:	/* ldr or str immediate */
		offset.un = OFFSET_BITS(instr);
		handler = do_alignment_ldrstr;
		break;

	case 0x40000000:	/* ldr or str register */
		offset.un = regs->uregs[RM_BITS(instr)];
		{
			unsigned int shiftval = SHIFT_BITS(instr);

			switch(SHIFT_TYPE(instr)) {
			case SHIFT_LSL:
				offset.un <<= shiftval;
				break;

			case SHIFT_LSR:
				offset.un >>= shiftval;
				break;

			case SHIFT_ASR:
				offset.sn >>= shiftval;
				break;

			case SHIFT_RORRRX:
				if (shiftval == 0) {
					offset.un >>= 1;
					if (regs->UCreg_csr & PSR_C_BIT)
						offset.un |= 1 << 31;
				} else
					offset.un = offset.un >> shiftval |
							  offset.un << (32 - shiftval);
				break;
			}
		}
		handler = do_alignment_ldrstr;
		break;

	case 0x80000000:	/* ldm or stm */
	case 0x80000020:	/* ldm or stm */
		handler = do_alignment_ldmstm;
		break;

	default:
		goto bad;
	}

	type = handler(addr, instr, regs);

	if (type == TYPE_ERROR || type == TYPE_FAULT)
		goto bad_or_fault;

	if (type == TYPE_LDST)
		do_alignment_finish_ldst(addr, instr, regs, offset);

	return 0;

bad_or_fault:
	if (type == TYPE_ERROR)
		goto bad;
	regs->UCreg_pc -= 4;
	/*
	 * We got a fault - fix it up, or die.
	 */
	do_bad_area(addr, error_code, regs);
	return 0;

bad:
	/*
	 * Oops, we didn't handle the instruction.
         * However, we must handle fpu instr firstly.
	 */
#ifdef CONFIG_UNICORE_FPU_F64
	/* handle co.load/store */
#define CODING_COLS                0xc0000000
#define COLS_OFFSET_BITS(i)	(i & 0x1FF)
#define COLS_L_BITS(i)		(i & (1<<24))
#define COLS_FN_BITS(i)		((i>>14) & 31)
	if((instr & 0xe0000000) == CODING_COLS)
	{
		unsigned int fn = COLS_FN_BITS(instr);
		unsigned long val=0;
		if(COLS_L_BITS(instr))
		{
			get32t_unaligned_check(val,addr);
			switch(fn)
			{
				case 0: __asm__ __volatile__("MTF %0, F0": :"r"(val)); break;
				case 1: __asm__ __volatile__("MTF %0, F1": :"r"(val)); break;
				case 2: __asm__ __volatile__("MTF %0, F2": :"r"(val)); break;
				case 3: __asm__ __volatile__("MTF %0, F3": :"r"(val)); break;
				case 4: __asm__ __volatile__("MTF %0, F4": :"r"(val)); break;
				case 5: __asm__ __volatile__("MTF %0, F5": :"r"(val)); break;
				case 6: __asm__ __volatile__("MTF %0, F6": :"r"(val)); break;
				case 7: __asm__ __volatile__("MTF %0, F7": :"r"(val)); break;
				case 8: __asm__ __volatile__("MTF %0, F8": :"r"(val)); break;
				case 9: __asm__ __volatile__("MTF %0, F9": :"r"(val)); break;
				case 10: __asm__ __volatile__("MTF %0, F10": :"r"(val)); break;
				case 11: __asm__ __volatile__("MTF %0, F11": :"r"(val)); break;
				case 12: __asm__ __volatile__("MTF %0, F12": :"r"(val)); break;
				case 13: __asm__ __volatile__("MTF %0, F13": :"r"(val)); break;
				case 14: __asm__ __volatile__("MTF %0, F14": :"r"(val)); break;
				case 15: __asm__ __volatile__("MTF %0, F15": :"r"(val)); break;
				case 16: __asm__ __volatile__("MTF %0, F16": :"r"(val)); break;
				case 17: __asm__ __volatile__("MTF %0, F17": :"r"(val)); break;
				case 18: __asm__ __volatile__("MTF %0, F18": :"r"(val)); break;
				case 19: __asm__ __volatile__("MTF %0, F19": :"r"(val)); break;
				case 20: __asm__ __volatile__("MTF %0, F20": :"r"(val)); break;
				case 21: __asm__ __volatile__("MTF %0, F21": :"r"(val)); break;
				case 22: __asm__ __volatile__("MTF %0, F22": :"r"(val)); break;
				case 23: __asm__ __volatile__("MTF %0, F23": :"r"(val)); break;
				case 24: __asm__ __volatile__("MTF %0, F24": :"r"(val)); break;
				case 25: __asm__ __volatile__("MTF %0, F25": :"r"(val)); break;
				case 26: __asm__ __volatile__("MTF %0, F26": :"r"(val)); break;
				case 27: __asm__ __volatile__("MTF %0, F27": :"r"(val)); break;
				case 28: __asm__ __volatile__("MTF %0, F28": :"r"(val)); break;
				case 29: __asm__ __volatile__("MTF %0, F29": :"r"(val)); break;
				case 30: __asm__ __volatile__("MTF %0, F30": :"r"(val)); break;
				case 31: __asm__ __volatile__("MTF %0, F31": :"r"(val)); break;
			}				
		} 
		else 
		{	
			switch(fn)
			{
				case 0: __asm__ __volatile__("MFF %0, F0": :"r"(val)); break;
				case 1: __asm__ __volatile__("MFF %0, F1": :"r"(val)); break;
				case 2: __asm__ __volatile__("MFF %0, F2": :"r"(val)); break;
				case 3: __asm__ __volatile__("MFF %0, F3": :"r"(val)); break;
				case 4: __asm__ __volatile__("MFF %0, F4": :"r"(val)); break;
				case 5: __asm__ __volatile__("MFF %0, F5": :"r"(val)); break;
				case 6: __asm__ __volatile__("MFF %0, F6": :"r"(val)); break;
				case 7: __asm__ __volatile__("MFF %0, F7": :"r"(val)); break;
				case 8: __asm__ __volatile__("MFF %0, F8": :"r"(val)); break;
				case 9: __asm__ __volatile__("MFF %0, F9": :"r"(val)); break;
				case 10: __asm__ __volatile__("MFF %0, F10": :"r"(val)); break;
				case 11: __asm__ __volatile__("MFF %0, F11": :"r"(val)); break;
				case 12: __asm__ __volatile__("MFF %0, F12": :"r"(val)); break;
				case 13: __asm__ __volatile__("MFF %0, F13": :"r"(val)); break;
				case 14: __asm__ __volatile__("MFF %0, F14": :"r"(val)); break;
				case 15: __asm__ __volatile__("MFF %0, F15": :"r"(val)); break;
				case 16: __asm__ __volatile__("MFF %0, F16": :"r"(val)); break;
				case 17: __asm__ __volatile__("MFF %0, F17": :"r"(val)); break;
				case 18: __asm__ __volatile__("MFF %0, F18": :"r"(val)); break;
				case 19: __asm__ __volatile__("MFF %0, F19": :"r"(val)); break;
				case 20: __asm__ __volatile__("MFF %0, F20": :"r"(val)); break;
				case 21: __asm__ __volatile__("MFF %0, F21": :"r"(val)); break;
				case 22: __asm__ __volatile__("MFF %0, F22": :"r"(val)); break;
				case 23: __asm__ __volatile__("MFF %0, F23": :"r"(val)); break;
				case 24: __asm__ __volatile__("MFF %0, F24": :"r"(val)); break;
				case 25: __asm__ __volatile__("MFF %0, F25": :"r"(val)); break;
				case 26: __asm__ __volatile__("MFF %0, F26": :"r"(val)); break;
				case 27: __asm__ __volatile__("MFF %0, F27": :"r"(val)); break;
				case 28: __asm__ __volatile__("MFF %0, F28": :"r"(val)); break;
				case 29: __asm__ __volatile__("MFF %0, F29": :"r"(val)); break;
				case 30: __asm__ __volatile__("MFF %0, F30": :"r"(val)); break;
				case 31: __asm__ __volatile__("MFF %0, F31": :"r"(val)); break;
			}				
			put32t_unaligned_check(val, addr);
		}
		return TYPE_COLS;	
	}
fault:
        return TYPE_FAULT;
#endif	
	printk(KERN_ERR "Alignment trap: not handling instruction "
		"%08lx at [<%08lx>]\n", instr, instrptr);
	ai_skipped += 1;
	return 1;
}

/*
 * This needs to be done after sysctl_init, otherwise sys/ will be
 * overwritten.  Actually, this shouldn't be in sys/ at all since
 * it isn't a sysctl, and it doesn't contain sysctl information.
 * We now locate it in /proc/cpu/alignment instead.
 */
static int __init alignment_init(void)
{
#ifdef CONFIG_PROC_FS
	struct proc_dir_entry *res;

	res = proc_mkdir("cpu", NULL);
	if (!res)
		return -ENOMEM;

	res = create_proc_entry("alignment", S_IWUSR | S_IRUGO, res);
	if (!res)
		return -ENOMEM;

	res->read_proc = proc_alignment_read;
	res->write_proc = proc_alignment_write;
#endif

	hook_fault_code(1, do_alignment, SIGILL, "alignment exception");

	return 0;
}

fs_initcall(alignment_init);
