#
# Makefile for the linux kernel.
#

CPPFLAGS_vmlinux.lds := -DTEXT_OFFSET=$(TEXT_OFFSET)
AFLAGS_head.o        := -DTEXT_OFFSET=$(TEXT_OFFSET)

ifdef CONFIG_DYNAMIC_FTRACE
CFLAGS_REMOVE_ftrace.o = -pg
endif

CFLAGS_REMOVE_return_address.o = -pg

# Object file lists.

obj-y		:= compat.o elf.o entry-unicore.o entry-common.o io.o irq.o \
		   process.o ptrace.o return_address.o setup.o signal.o \
		   sys_unicore.o stacktrace.o time.o traps.o

obj-$(CONFIG_MODULES)		+= ksyms.o module.o
obj-$(CONFIG_PCI)		+= bios32.o
obj-$(CONFIG_DYNAMIC_FTRACE)	+= ftrace.o
obj-$(CONFIG_EARLY_PRINTK)	+= early_printk.o

head-y			:= head.o
obj-$(CONFIG_DEBUG_LL)	+= debug.o

extra-y := $(head-y) init_task.o vmlinux.lds
