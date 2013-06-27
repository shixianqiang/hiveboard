/*
 * linux/arch/unicore/include/asm/thread_notify.h
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
#ifndef __UNICORE_THREAD_NOTIFY_H__
#define __UNICORE_THREAD_NOTIFY_H__

#ifdef __KERNEL__

#ifndef __ASSEMBLY__

#include <linux/notifier.h>
#include <asm/thread_info.h>

static inline int thread_register_notifier(struct notifier_block *n)
{
	extern struct atomic_notifier_head thread_notify_head;
	return atomic_notifier_chain_register(&thread_notify_head, n);
}

static inline void thread_unregister_notifier(struct notifier_block *n)
{
	extern struct atomic_notifier_head thread_notify_head;
	atomic_notifier_chain_unregister(&thread_notify_head, n);
}

static inline void thread_notify(unsigned long rc, struct thread_info *thread)
{
	extern struct atomic_notifier_head thread_notify_head;
	atomic_notifier_call_chain(&thread_notify_head, rc, thread);
}

#endif

/*
 * These are the reason codes for the thread notifier.
 */
#define THREAD_NOTIFY_FLUSH	0
#define THREAD_NOTIFY_RELEASE	1
#define THREAD_NOTIFY_SWITCH	2

#endif
#endif
