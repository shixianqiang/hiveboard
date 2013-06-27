/*
 * linux/arch/unicore/kernel/vmlinux.lds.S
 *
 * Code specific to PKUnity SoC and UniCore ISA
 * Fragments that appear the same as the files in arm or x86
 *
 * Copyright (C) 2001-2008 GUAN Xue-tao
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <asm-generic/vmlinux.lds.h>
#include <asm/thread_info.h>
#include <asm/memory.h>
#include <asm/page.h>
	
OUTPUT_ARCH(unicore32)
ENTRY(stext)

jiffies = jiffies_64;

SECTIONS
{
	. = PAGE_OFFSET + TEXT_OFFSET;
	.text.head : {
		_stext = .;
		_sinittext = .;
		*(.text.head)
	}

	.init : {			/* Init code and data		*/
			INIT_TEXT
		_einittext = .;
		__proc_info_begin = .;
			*(.proc.info.init)
		__proc_info_end = .;
		__arch_info_begin = .;
			*(.arch.info.init)
		__arch_info_end = .;
		__tagtable_begin = .;
			*(.taglist.init)
		__tagtable_end = .;
		. = ALIGN(16);
		__setup_start = .;
			*(.init.setup)
		__setup_end = .;
		__early_begin = .;
			*(.early_param.init)
		__early_end = .;
		__initcall_start = .;
			INITCALLS
		__initcall_end = .;
		__con_initcall_start = .;
			*(.con_initcall.init)
		__con_initcall_end = .;
		__security_initcall_start = .;
			*(.security_initcall.init)
		__security_initcall_end = .;
#ifdef CONFIG_BLK_DEV_INITRD
		. = ALIGN(32);
		__initramfs_start = .;
			usr/built-in.o(.init.ramfs)
		__initramfs_end = .;
#endif
		. = ALIGN(PAGE_SIZE);
		__per_cpu_load = .;
		__per_cpu_start = .;
			*(.data.percpu.page_aligned)
			*(.data.percpu)
			*(.data.percpu.shared_aligned)
		__per_cpu_end = .;
		__init_begin = _stext;
		INIT_DATA
		. = ALIGN(PAGE_SIZE);
		__init_end = .;
	}

	/DISCARD/ : {			/* Exit code and data		*/
		EXIT_TEXT
		EXIT_DATA
		*(.exitcall.exit)
		*(.discard)
		*(.UNICORE.exidx.exit.text)
		*(.UNICORE.extab.exit.text)
#ifndef CONFIG_HOTPLUG_CPU
		*(.UNICORE.exidx.cpuexit.text)
		*(.UNICORE.extab.cpuexit.text)
#endif
#ifndef CONFIG_HOTPLUG
		*(.UNICORE.exidx.devexit.text)
		*(.UNICORE.extab.devexit.text)
#endif
	}

	.text : {			/* Real text segment		*/
		_text = .;		/* Text and read-only data	*/
			__exception_text_start = .;
			*(.exception.text)
			__exception_text_end = .;
			TEXT_TEXT
			SCHED_TEXT
			LOCK_TEXT
			KPROBES_TEXT
			*(.fixup)
			*(.gnu.warning)
			*(.rodata)
			*(.rodata.*)
			*(.glue_7)
			*(.glue_7t)
		*(.got)			/* Global offset table		*/
	}

	RO_DATA(PAGE_SIZE)

	_etext = .;			/* End of text and rodata section */

	. = ALIGN(THREAD_SIZE);
	__data_loc = .;

	.data : AT(__data_loc) {
		_data = .;		/* address in memory */
		_sdata = .;

		/*
		 * first, the init task union, aligned
		 * to an 8192 byte boundary.
		 */
		*(.data.init_task)

		. = ALIGN(PAGE_SIZE);
		__nosave_begin = .;
		*(.data.nosave)
		. = ALIGN(PAGE_SIZE);
		__nosave_end = .;

		/*
		 * then the cacheline aligned data
		 */
		. = ALIGN(32);
		*(.data.cacheline_aligned)

		/*
		 * The exception fixup table (might need resorting at runtime)
		 */
		. = ALIGN(32);
		__start___ex_table = .;
		*(__ex_table)
		__stop___ex_table = .;

		/*
		 * and the usual data section
		 */
		DATA_DATA
		CONSTRUCTORS

		_edata = .;
	}
	_edata_loc = __data_loc + SIZEOF(.data);

	.bss : {
		__bss_start = .;	/* BSS				*/
		*(.bss)
		*(COMMON)
		__bss_stop = .;
		_end = .;
	}
					/* Stabs debugging sections.	*/
	.stab 0 : { *(.stab) }
	.stabstr 0 : { *(.stabstr) }
	.stab.excl 0 : { *(.stab.excl) }
	.stab.exclstr 0 : { *(.stab.exclstr) }
	.stab.index 0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment 0 : { *(.comment) }
}

/*
 * These must never be empty
 * If you have to comment these two assert statements out, your
 * binutils is too old (for other reasons as well)
 */
ASSERT((__proc_info_end - __proc_info_begin), "missing CPU support")
ASSERT((__arch_info_end - __arch_info_begin), "no machine record defined")
