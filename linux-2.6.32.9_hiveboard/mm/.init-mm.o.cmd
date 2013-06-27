cmd_mm/init-mm.o := unicore32-linux-gcc -Wp,-MD,mm/.init-mm.o.d  -nostdinc -isystem /usr/unicore/gnu-toolchain-unicore/uc4-1.0-beta-hard-RHELAS5/lib/gcc/unicore32-linux/4.4.2/include -Iinclude  -I/home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include -include include/linux/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/unicore/mach-sep0611/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -msoft-float -Uunicore32 -Wframe-larger-than=1024 -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fno-dwarf2-cfi-asm -fconserve-stack   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(init_mm)"  -D"KBUILD_MODNAME=KBUILD_STR(init_mm)"  -c -o mm/.tmp_init-mm.o mm/init-mm.c

deps_mm/init-mm.o := \
  mm/init-mm.c \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mm/owner.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/mmu/notifier.h) \
  include/linux/auxvec.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/auxvec.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/posix_types.h \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
  include/linux/prefetch.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/processor.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/ptrace.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/hwcap.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/cache.h \
    $(wildcard include/config/cpu/l1/cache/shift.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/system.h \
  include/linux/linkage.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/linkage.h \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
    $(wildcard include/config/x86.h) \
  include/linux/typecheck.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/irqflags.h \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/cmpxchg.h \
    $(wildcard include/config/smp.h) \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/bitops.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/lock.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/thread_info.h \
    $(wildcard include/config/unicore/fpu/f64.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/fpstate.h \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /usr/unicore/gnu-toolchain-unicore/uc4-1.0-beta-hard-RHELAS5/lib/gcc/unicore32-linux/4.4.2/include/stdarg.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/ratelimit.h \
  include/linux/param.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  include/linux/dynamic_debug.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  include/linux/stringify.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/generic/hardirqs.h) \
  include/linux/spinlock_up.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/atomic.h \
  include/asm-generic/atomic-long.h \
  include/linux/spinlock_api_up.h \
  include/linux/prio_tree.h \
  include/linux/rbtree.h \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/completion.h \
  include/linux/wait.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/current.h \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/string.h \
  include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/page.h \
    $(wildcard include/config/cpu/ucv1.h) \
    $(wildcard include/config/cpu/ucv2.h) \
    $(wildcard include/config/sparsemem.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/glue.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/memory.h \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/discontigmem.h) \
  include/linux/const.h \
  arch/unicore/mach-sep0611/include/mach/memory.h \
    $(wildcard include/config/kuser/base.h) \
    $(wildcard include/config/vectors/base.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/sizes.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/asm-generic/getorder.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/mmu.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/pgtable.h \
    $(wildcard include/config/highpte.h) \
  include/asm-generic/pgtable-nopmd.h \
  include/asm-generic/pgtable-nopud.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/proc-fns.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/cpu-single.h \
  arch/unicore/mach-sep0611/include/mach/vmalloc.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/pgtable-hwdef.h \
  include/asm-generic/pgtable.h \

mm/init-mm.o: $(deps_mm/init-mm.o)

$(deps_mm/init-mm.o):
