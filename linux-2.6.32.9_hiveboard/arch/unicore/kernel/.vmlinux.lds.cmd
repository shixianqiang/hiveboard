cmd_arch/unicore/kernel/vmlinux.lds := unicore32-linux-gcc -E -Wp,-MD,arch/unicore/kernel/.vmlinux.lds.d  -nostdinc -isystem /usr/unicore/gnu-toolchain-unicore/uc4-1.0-beta-hard-RHELAS5/lib/gcc/unicore32-linux/4.4.2/include -Iinclude  -I/home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include -include include/linux/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/unicore/mach-sep0611/include   -DTEXT_OFFSET=0x00008000 -P -C -Uunicore -D__ASSEMBLY__ -DLINKER_SCRIPT -o arch/unicore/kernel/vmlinux.lds arch/unicore/kernel/vmlinux.lds.S

deps_arch/unicore/kernel/vmlinux.lds := \
  arch/unicore/kernel/vmlinux.lds.S \
    $(wildcard include/config/blk/dev/initrd.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/hotplug.h) \
  include/asm-generic/vmlinux.lds.h \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/ftrace/syscalls.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/pm/trace.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/thread_info.h \
    $(wildcard include/config/unicore/fpu/f64.h) \
  include/linux/compiler.h \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/fpstate.h \
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
    $(wildcard include/config/sparsemem.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/page.h \
    $(wildcard include/config/cpu/ucv1.h) \
    $(wildcard include/config/cpu/ucv2.h) \
  include/asm-generic/getorder.h \

arch/unicore/kernel/vmlinux.lds: $(deps_arch/unicore/kernel/vmlinux.lds)

$(deps_arch/unicore/kernel/vmlinux.lds):
