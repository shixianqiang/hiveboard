#
# Makefile for the linux unicore-specific parts of the memory manager.
#

obj-y				:= dma-mapping.o extable.o fault.o init.o \
				   iomap.o

obj-$(CONFIG_MMU)		+= fault-unicore.o flush.o ioremap.o \
				   pgd.o mmu.o

obj-$(CONFIG_MODULES)		+= proc-syms.o

obj-$(CONFIG_ALIGNMENT_TRAP)	+= alignment.o
obj-$(CONFIG_HIGHMEM)		+= highmem.o

obj-$(CONFIG_CPU_UCV2)		+= abort-ucv2.o cache-ucv2.o copypage-ucv2.o \
				   tlb-ucv2.o proc-ucv2.o

obj-$(CONFIG_CPU_PABRT_LEGACY)	+= pabort-legacy.o

