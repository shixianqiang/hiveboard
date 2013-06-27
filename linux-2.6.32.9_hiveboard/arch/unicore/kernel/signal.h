/*
 * linux/arch/unicore/kernel/signal.h
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
#define KERN_SIGRETURN_CODE	(KUSER_VECPAGE_BASE + 0x00000500)
#define KERN_RESTART_CODE	(KERN_SIGRETURN_CODE + sizeof(sigreturn_codes))

extern const unsigned long sigreturn_codes[3];
extern const unsigned long syscall_restart_code[2];
