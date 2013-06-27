cmd_arch/unicore/boot/compressed/head.o := unicore32-linux-gcc -Wp,-MD,arch/unicore/boot/compressed/.head.o.d  -nostdinc -isystem /usr/unicore/gnu-toolchain-unicore/uc4-1.0-beta-hard-RHELAS5/lib/gcc/unicore32-linux/4.4.2/include -Iinclude  -I/home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include -include include/linux/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/unicore/mach-sep0611/include -D__ASSEMBLY__ -msoft-float     -Wa,-march=all   -c -o arch/unicore/boot/compressed/head.o arch/unicore/boot/compressed/head.S

deps_arch/unicore/boot/compressed/head.o := \
  arch/unicore/boot/compressed/head.S \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include/asm/linkage.h \

arch/unicore/boot/compressed/head.o: $(deps_arch/unicore/boot/compressed/head.o)

$(deps_arch/unicore/boot/compressed/head.o):
