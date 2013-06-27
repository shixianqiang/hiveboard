/*
 * linux/arch/unicore/include/asm/sigcontext.h
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
#ifndef __UNICORE_SIGCONTEXT_H__
#define __UNICORE_SIGCONTEXT_H__

/*
 * Signal context structure - contains all info to do with the state
 * before the signal handler was invoked.  Note: only add new entries
 * to the end of the structure.
 */
struct sigcontext {
	unsigned long trap_no;
	unsigned long error_code;
	unsigned long oldmask;
	unsigned long ucreg_00;
	unsigned long ucreg_01;
	unsigned long ucreg_02;
	unsigned long ucreg_03;
	unsigned long ucreg_04;
	unsigned long ucreg_05;
	unsigned long ucreg_06;
	unsigned long ucreg_07;
	unsigned long ucreg_08;
	unsigned long ucreg_09;
	unsigned long ucreg_10;
	unsigned long ucreg_11;
	unsigned long ucreg_12;
	unsigned long ucreg_13;
	unsigned long ucreg_14;
	unsigned long ucreg_15;
	unsigned long ucreg_16;
	unsigned long ucreg_17;
	unsigned long ucreg_18;
	unsigned long ucreg_19;
	unsigned long ucreg_20;
	unsigned long ucreg_21;
	unsigned long ucreg_22;
	unsigned long ucreg_23;
	unsigned long ucreg_24;
	unsigned long ucreg_25;
	unsigned long ucreg_26;
	unsigned long ucreg_fp;
	unsigned long ucreg_ip;
	unsigned long ucreg_sp;
	unsigned long ucreg_lr;
	unsigned long ucreg_pc;
	unsigned long ucreg_csr;
	unsigned long fault_address;
};


#endif
