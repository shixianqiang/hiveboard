cmd_arch/unicore/kernel/entry-unicore.o := unicore32-linux-gcc -Wp,-MD,arch/unicore/kernel/.entry-unicore.o.d  -nostdinc -isystem /usr/unicore/gnu-toolchain-unicore/uc4-1.0-beta-hard-RHELAS5/lib/gcc/unicore32-linux/4.4.2/include -Iinclude  -I/home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include -include include/linux/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/unicore/mach-sep0611/include -D__ASSEMBLY__ -msoft-float       -c -o arch/unicore/kernel/entry-unicore.o arch/unicore/kernel/entry-unicore.S

deps_arch/unicore/kernel/entry-unicore.o := \
  arch/unicore/kernel/entry-unicore.S \
    $(wildcard include/config/kprobes.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/unicore/fpu/f64.h) \
    $(wildcard include/config/vectors/trace.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/memory.h \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/discontigmem.h) \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/const.h \
  arch/unicore/mach-sep0611/include/mach/memory.h \
    $(wildcard include/config/kuser/base.h) \
    $(wildcard include/config/vectors/base.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/sizes.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/glue.h \
    $(wildcard include/config/cpu/ucv1.h) \
    $(wildcard include/config/cpu/ucv2.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/thread_notify.h \
  arch/unicore/mach-sep0611/include/mach/entry-macro.S \
  arch/unicore/mach-sep0611/include/mach/hardware.h \
  arch/unicore/mach-sep0611/include/mach/debug-macro.S \
    $(wildcard include/config/debug/ocd.h) \
    $(wildcard include/config/debug/ocd/breakpoint.h) \
  include/asm/asm-offsets.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/unistd.h \
  arch/unicore/kernel/entry-header.S \
    $(wildcard include/config/frame/pointer.h) \
    $(wildcard include/config/alignment/trap.h) \
    $(wildcard include/config/debug/ll.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/linkage.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/linkage.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/assembler.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/ptrace.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/hwcap.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/thread_info.h \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/fpstate.h \

arch/unicore/kernel/entry-unicore.o: $(deps_arch/unicore/kernel/entry-unicore.o)

$(deps_arch/unicore/kernel/entry-unicore.o):
