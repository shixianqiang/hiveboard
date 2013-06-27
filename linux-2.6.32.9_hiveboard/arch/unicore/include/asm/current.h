#ifndef __UNICORE_CURRENT_H__
#define __UNICORE_CURRENT_H__

#include <linux/thread_info.h>

static inline struct task_struct *get_current(void) __attribute_const__;

static inline struct task_struct *get_current(void)
{
	return current_thread_info()->task;
}

#define current (get_current())

#endif /* __UNICORE_CURRENT_H__ */
