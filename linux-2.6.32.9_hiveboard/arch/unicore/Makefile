#
# arch/unicore/Makefile
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies.
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 2008 by Guan Xue-tao
LOCALVERSION	:= -uc2

#CROSS_COMPILE	:= unicore32-linux-

INSTALL_MOD_PATH := $(obj)/usr

LDFLAGS_vmlinux	:=-p --no-undefined -X

OBJCOPYFLAGS	:=-O binary -R .note -R .note.gnu.build-id -R .comment -S
GZFLAGS		:=-9
#KBUILD_CFLAGS	+=-pipe
# Explicitly specifiy 32-bit UniCore ISA:
KBUILD_CFLAGS	+=$(call cc-option,-municore,)

ifeq ($(CONFIG_FRAME_POINTER),y)
KBUILD_CFLAGS	+=-fno-omit-frame-pointer -mno-sched-prolog
endif

KBUILD_CPPFLAGS	+= -mlittle-endian
AS		+= -EL
LD		+= -EL

comma = ,

# Need -Uunicore32 for gcc < 3.x
# delete -mstructure-size-boundary=32, and default being 8
# delete -funsigned-char, and default being signed-char
KBUILD_CFLAGS	+=$(call cc-option,-mshort-load-bytes,$(call cc-option,-malignment-traps,)) -msoft-float -Uunicore32
KBUILD_AFLAGS	+=-msoft-float

CHECKFLAGS	+= -D__unicore32__

#Default value
head-y		:= arch/unicore/kernel/head.o arch/unicore/kernel/init_task.o
textofs-y	:= 0x00008000

machine-$(CONFIG_ARCH_PUV3)	:= puv3
############ add by zhangyang 2010-05-13-15:17
machine-$(CONFIG_ARCH_SEP0611)  := sep0611

# The byte offset of the kernel image in RAM from the start of RAM.
TEXT_OFFSET := $(textofs-y)

# The first directory contains additional information for the boot setup code
ifneq ($(machine-y),)
MACHINE  := arch/unicore/mach-$(word 1,$(machine-y))/
else
MACHINE  :=
endif

machdirs := $(patsubst %,arch/unicore/mach-%/,$(machine-y))

ifeq ($(KBUILD_SRC),)
KBUILD_CPPFLAGS += $(patsubst %,-I%include,$(machdirs))
else
KBUILD_CPPFLAGS += $(patsubst %,-I$(srctree)/%include,$(machdirs))
endif

export	TEXT_OFFSET GZFLAGS

# If we have a machine-specific directory, then include it in the build.
core-y				+= arch/unicore/kernel/ arch/unicore/mm/ 
core-y				+= $(machdirs)
########## commented by zhangyang 2010-05-13 16:06
#core-$(CONFIG_UNICORE_FPU_F64)	+= arch/unicore/uc-f64/

########## comment by zhangyang 2010-05-13 15:20
#drivers-$(CONFIG_UNICORE)	+= drivers/pkunity/
#drivers-$(CONFIG_OPROFILE)      += arch/unicore/oprofile/

libs-y				:= arch/unicore/lib/ $(libs-y)

# Default target when executing plain make
KBUILD_IMAGE := zImage

all:	$(KBUILD_IMAGE)

boot := arch/unicore/boot

ASM_GENERIC_HEADERS	:= arch/unicore/include/asm/bitsperlong.h		\
			   arch/unicore/include/asm/cputime.h			\
			   arch/unicore/include/asm/emergency-restart.h		\
			   arch/unicore/include/asm/errno.h			\
			   arch/unicore/include/asm/fcntl.h			\
			   arch/unicore/include/asm/ioctl.h			\
			   arch/unicore/include/asm/irq_regs.h			\
			   arch/unicore/include/asm/kdebug.h			\
			   arch/unicore/include/asm/local.h			\
			   arch/unicore/include/asm/percpu.h			\
			   arch/unicore/include/asm/poll.h			\
			   arch/unicore/include/asm/resource.h			\
			   arch/unicore/include/asm/sections.h			\
			   arch/unicore/include/asm/siginfo.h			\
			   arch/unicore/include/asm/topology.h

%.h:
	$(Q)echo '#include <asm-generic/$(notdir $@)>' > $@

archprepare: include/linux/version.h $(ASM_GENERIC_HEADERS) FORCE

# Convert bzImage to zImage
bzImage: zImage

zImage Image uImage: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $(boot)/$@

zinstall install: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $@

CLEAN_DIRS += $(INSTALL_MOD_PATH)/lib
CLEAN_FILES += $(ASM_GENERIC_HEADERS)

# We use MRPROPER_FILES and CLEAN_FILES now
archclean:
	$(Q)$(MAKE) $(clean)=$(boot)

define archhelp
  echo  '* zImage        - Compressed kernel image (arch/$(ARCH)/boot/zImage)'
  echo  '  Image         - Uncompressed kernel image (arch/$(ARCH)/boot/Image)'
  echo  '  uImage        - U-Boot wrapped zImage'
  echo  '  install       - Install uncompressed kernel'
  echo  '  zinstall      - Install compressed kernel'
  echo  '                  Install using (your) ~/bin/$(INSTALLKERNEL) or'
  echo  '                  (distribution) /sbin/$(INSTALLKERNEL) or'
  echo  '                  install to $$(INSTALL_PATH) and run lilo'
endef
