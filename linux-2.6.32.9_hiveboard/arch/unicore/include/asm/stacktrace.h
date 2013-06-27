#ifndef __ASM_STACKTRACE_H__
#define __ASM_STACKTRACE_H__

struct stackframe {
	unsigned long fp;
	unsigned long sp;
	unsigned long lr;
	unsigned long pc;
};

#ifdef CONFIG_FRAME_POINTER
extern int unwind_frame(struct stackframe *frame);
#else
#define unwind_frame(f) (-EINVAL)
#endif
extern void walk_stackframe(struct stackframe *frame,
			    int (*fn)(struct stackframe *, void *), void *data);

#endif	/* __ASM_STACKTRACE_H__ */
