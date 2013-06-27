cmd_arch/unicore/boot/compressed/piggy.o := unicore32-linux-gcc -Wp,-MD,arch/unicore/boot/compressed/.piggy.o.d  -nostdinc -isystem /usr/unicore/gnu-toolchain-unicore/uc4-1.0-beta-hard-RHELAS5/lib/gcc/unicore32-linux/4.4.2/include -Iinclude  -I/home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include -include include/linux/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/unicore/mach-sep0611/include -D__ASSEMBLY__ -msoft-float     -Wa,-march=all   -c -o arch/unicore/boot/compressed/piggy.o arch/unicore/boot/compressed/piggy.S

deps_arch/unicore/boot/compressed/piggy.o := \
  arch/unicore/boot/compressed/piggy.S \

arch/unicore/boot/compressed/piggy.o: $(deps_arch/unicore/boot/compressed/piggy.o)

$(deps_arch/unicore/boot/compressed/piggy.o):
