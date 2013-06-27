cmd_arch/unicore/kernel/entry-common.o := unicore32-linux-gcc -Wp,-MD,arch/unicore/kernel/.entry-common.o.d  -nostdinc -isystem /usr/unicore/gnu-toolchain-unicore/uc4-1.0-beta-hard-RHELAS5/lib/gcc/unicore32-linux/4.4.2/include -Iinclude  -I/home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include -include include/linux/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/unicore/mach-sep0611/include -D__ASSEMBLY__ -msoft-float       -c -o arch/unicore/kernel/entry-common.o arch/unicore/kernel/entry-common.S

deps_arch/unicore/kernel/entry-common.o := \
  arch/unicore/kernel/entry-common.S \
    $(wildcard include/config/function/tracer.h) \
    $(wildcard include/config/dynamic/ftrace.h) \
    $(wildcard include/config/syscall/trace.h) \
    $(wildcard include/config/alignment/trap.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/unistd.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/ftrace.h \
    $(wildcard include/config/frame/pointer.h) \
  arch/unicore/mach-sep0611/include/mach/entry-macro.S \
  arch/unicore/mach-sep0611/include/mach/hardware.h \
  arch/unicore/kernel/entry-header.S \
    $(wildcard include/config/debug/ll.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/linkage.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/linkage.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/assembler.h \
    $(wildcard include/config/trace/irqflags.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/ptrace.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/hwcap.h \
  include/asm/asm-offsets.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/thread_info.h \
    $(wildcard include/config/unicore/fpu/f64.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/fpstate.h \
  arch/unicore/kernel/calls.S \

arch/unicore/kernel/entry-common.o: $(deps_arch/unicore/kernel/entry-common.o)

$(deps_arch/unicore/kernel/entry-common.o):
