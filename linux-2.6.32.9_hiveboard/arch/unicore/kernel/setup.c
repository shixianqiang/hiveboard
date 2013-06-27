/*
 * linux/arch/unicore/kernel/setup.c
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
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <linux/utsname.h>
#include <linux/initrd.h>
#include <linux/console.h>
#include <linux/bootmem.h>
#include <linux/seq_file.h>
#include <linux/screen_info.h>
#include <linux/init.h>
#include <linux/root_dev.h>
#include <linux/cpu.h>
#include <linux/interrupt.h>
#include <linux/smp.h>
#include <linux/fs.h>

#include <asm/cpu.h>
#include <asm/cputype.h>
#include <asm/elf.h>
#include <asm/procinfo.h>
#include <asm/sections.h>
#include <asm/setup.h>
#include <asm/cacheflush.h>
#include <asm/tlbflush.h>

#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>
#include <asm/traps.h>
#include "compat.h"

#ifndef MEM_SIZE
#define MEM_SIZE	(16*1024*1024)
#endif

extern void paging_init(struct machine_desc *desc);
extern void reboot_setup(char *str);

unsigned int processor_id;
EXPORT_SYMBOL(processor_id);
unsigned int __machine_arch_type;
EXPORT_SYMBOL(__machine_arch_type);

unsigned int __atags_pointer __initdata;

unsigned int system_rev;
EXPORT_SYMBOL(system_rev);

unsigned int system_serial_low;
EXPORT_SYMBOL(system_serial_low);

unsigned int system_serial_high;
EXPORT_SYMBOL(system_serial_high);

unsigned int elf_hwcap;
EXPORT_SYMBOL(elf_hwcap);

struct stack {
	u32 irq[3];
	u32 abt[3];
	u32 und[3];
} ____cacheline_aligned;

static struct stack stacks[NR_CPUS];

char elf_platform[ELF_PLATFORM_SIZE];
EXPORT_SYMBOL(elf_platform);

static const char *cpu_name;
static const char *machine_name;
static char __initdata command_line[COMMAND_LINE_SIZE];

static char default_command_line[COMMAND_LINE_SIZE] __initdata = CONFIG_CMDLINE;

DEFINE_PER_CPU(struct cpuinfo_unicore, cpu_data);

/*
 * Standard memory resources
 */
static struct resource mem_res[] = {
	{
		.name = "Video RAM",
		.start = 0,
		.end = 0,
		.flags = IORESOURCE_MEM
	},
	{
		.name = "Kernel text",
		.start = 0,
		.end = 0,
		.flags = IORESOURCE_MEM
	},
	{
		.name = "Kernel data",
		.start = 0,
		.end = 0,
		.flags = IORESOURCE_MEM
	}
};

#define video_ram   mem_res[0]
#define kernel_code mem_res[1]
#define kernel_data mem_res[2]

static struct resource io_res[] = {
	{
		.name = "reserved",
		.start = 0x3bc,
		.end = 0x3be,
		.flags = IORESOURCE_IO | IORESOURCE_BUSY
	},
	{
		.name = "reserved",
		.start = 0x378,
		.end = 0x37f,
		.flags = IORESOURCE_IO | IORESOURCE_BUSY
	},
	{
		.name = "reserved",
		.start = 0x278,
		.end = 0x27f,
		.flags = IORESOURCE_IO | IORESOURCE_BUSY
	}
};

#define lp0 io_res[0]
#define lp1 io_res[1]
#define lp2 io_res[2]

static const char *proc_arch[] = {
	"undefined/unknown",
	"1",
	"2",
};

int cpu_architecture(void)
{
	int cpu_arch;

	if ((processor_id & 0x00f00000) == 0x00200000) {
		cpu_arch = CPU_ARCH_UCV2;
	} else if ((processor_id & 0x00f00000) == 0x00100000) {
                if ((processor_id & 0x000f0000) == 0x00080000)
                        cpu_arch = CPU_ARCH_UCV2;
                else
                        cpu_arch = CPU_ARCH_UCV1;
	} else {
		cpu_arch = CPU_ARCH_UNKNOWN;
	}

	return cpu_arch;
}

/*
 * These functions re-use the assembly code in head.S, which
 * already provide the required functionality.
 */
extern struct proc_info_list *lookup_processor_type(unsigned int);
extern struct machine_desc *lookup_machine_type(unsigned int);

static void __init setup_processor(void)
{
	struct proc_info_list *list;

	/*
	 * locate processor in the list of supported processor
	 * types.  The linker builds this table for us from the
	 * entries in arch/unicore/mm/proc-*.S
	 */
	list = lookup_processor_type(read_cpuid_id());
	if (!list) {
		printk("CPU configuration botched (ID %08x), unable "
		       "to continue.\n", read_cpuid_id());
		while (1);
	}

	cpu_name = list->cpu_name;

	printk("CPU: %s [%08x] revision %d (UniCore v%s), cr=%08lx\n",
	       cpu_name, read_cpuid_id(), (int)(read_cpuid_id() >> 16) & 15,
	       proc_arch[cpu_architecture()], cr_alignment);

	sprintf(init_utsname()->machine, "%s", list->arch_name);
	sprintf(elf_platform, "%s", list->elf_name);
	elf_hwcap = list->elf_hwcap;

	cpu_proc_init();
}

/*
 * cpu_init - initialise one CPU.
 *
 * cpu_init sets up the per-CPU stacks.
 */
void cpu_init(void)
{
	unsigned int cpu = smp_processor_id();
	struct stack *stk = &stacks[cpu];

	if (cpu >= NR_CPUS) {
		printk(KERN_CRIT "CPU%u: bad primary CPU number\n", cpu);
		BUG();
	}

	/*
	 * setup stacks for re-entrant exception handlers
	 */
	__asm__ (
	"mov.a	asr, %1\n\t"
	"add	sp, %0, %2\n\t"
	"mov.a	asr, %3\n\t"
	"add	sp, %0, %4\n\t"
	"mov.a	asr, %5\n\t"
	"add	sp, %0, %6\n\t"
	"mov.a	asr, %7"
	    :
	    : "r" (stk),
	      "r" (PSR_R_BIT | PSR_I_BIT | INTR_MODE),
	      "I" (offsetof(struct stack, irq[0])),
	      "r" (PSR_R_BIT | PSR_I_BIT | ABRT_MODE),
	      "I" (offsetof(struct stack, abt[0])),
	      "r" (PSR_R_BIT | PSR_I_BIT | EXTN_MODE),
	      "I" (offsetof(struct stack, und[0])),
	      "r" (PSR_R_BIT | PSR_I_BIT | PRIV_MODE)
        : "r30", "cc");
}

static struct machine_desc * __init setup_machine(unsigned int nr)
{
	struct machine_desc *list;

	/*
	 * locate machine in the list of supported machines.
	 */
	list = lookup_machine_type(nr);
	if (!list) {
		printk("Machine configuration botched (nr %d), unable "
		       "to continue.\n", nr);
		while (1);
	}

	printk("Machine: %s\n", list->name);

	return list;
}

static int __init unicore_add_memory(unsigned long start, unsigned long size)
{
	struct membank *bank = &meminfo.bank[meminfo.nr_banks];

	if (meminfo.nr_banks >= NR_BANKS) {
		printk(KERN_CRIT "NR_BANKS too low, "
			"ignoring memory at %#lx\n", start);
		return -EINVAL;
        }

	/*
	 * Ensure that start/size are aligned to a page boundary.
	 * Size is appropriately rounded down, start is rounded up.
	 */
	size -= start & ~PAGE_MASK;

	bank->start = PAGE_ALIGN(start);
	bank->size  = size & PAGE_MASK;
	bank->node  = PHYS_TO_NID(start);

	/*
	 * Check whether this memory region has non-zero size or
	 * invalid node number.
	 */
	if (bank->size == 0 || bank->node >= MAX_NUMNODES)
		return -EINVAL;

	meminfo.nr_banks++;
	return 0;
}

/*
 * Pick out the memory size.  We look for mem=size@start,
 * where start and size are "size[KkMm]"
 */
static void __init early_mem(char **p)
{
	static int usermem __initdata = 0;
	unsigned long size, start;

	/*
	 * If the user specifies memory size, we
	 * blow away any automatically generated
	 * size.
	 */
	if (usermem == 0) {
		usermem = 1;
		meminfo.nr_banks = 0;
	}

	start = PHYS_OFFSET;
	size  = memparse(*p, p);
	if (**p == '@')
		start = memparse(*p + 1, p);

	unicore_add_memory(start, size);
}
__early_param("mem=", early_mem);

/*
 * Initial parsing of the command line.
 */
static void __init parse_cmdline(char **cmdline_p, char *from)
{
	char c = ' ', *to = command_line;
	int len = 0;

	for (;;) {
		if (c == ' ') {
			extern struct early_params __early_begin, __early_end;
			struct early_params *p;

			for (p = &__early_begin; p < &__early_end; p++) {
				int arglen = strlen(p->arg);

				if (memcmp(from, p->arg, arglen) == 0) {
					if (to != command_line)
						to -= 1;
					from += arglen;
					p->fn(&from);

					while (*from != ' ' && *from != '\0')
						from++;
					break;
				}
			}
		}
		c = *from++;
		if (!c)
			break;
		if (COMMAND_LINE_SIZE <= ++len)
			break;
		*to++ = c;
	}
	*to = '\0';
	*cmdline_p = command_line;
}

static void __init
setup_ramdisk(int doload, int prompt, int image_start, unsigned int rd_sz)
{
#ifdef CONFIG_BLK_DEV_RAM
	extern int rd_size, rd_image_start, rd_prompt, rd_doload;

	rd_image_start = image_start;
	rd_prompt = prompt;
	rd_doload = doload;

	if (rd_sz)
		rd_size = rd_sz;
#endif
}

static void __init
request_standard_resources(struct meminfo *mi, struct machine_desc *mdesc)
{
	struct resource *res;
	int i;

	kernel_code.start   = virt_to_phys(_text);
	kernel_code.end     = virt_to_phys(_etext - 1);
	kernel_data.start   = virt_to_phys(_data);
	kernel_data.end     = virt_to_phys(_end - 1);

	for (i = 0; i < mi->nr_banks; i++) {
		if (mi->bank[i].size == 0)
			continue;

		res = alloc_bootmem_low(sizeof(*res));
		res->name  = "System RAM";
		res->start = mi->bank[i].start;
		res->end   = mi->bank[i].start + mi->bank[i].size - 1;
		res->flags = IORESOURCE_MEM | IORESOURCE_BUSY;

		request_resource(&iomem_resource, res);

		if (kernel_code.start >= res->start &&
		    kernel_code.end <= res->end)
			request_resource(res, &kernel_code);
		if (kernel_data.start >= res->start &&
		    kernel_data.end <= res->end)
			request_resource(res, &kernel_data);
	}

	if (mdesc->video_start) {
		video_ram.start = mdesc->video_start;
		video_ram.end   = mdesc->video_end;
		request_resource(&iomem_resource, &video_ram);
	}

	/*
	 * Some machines don't have the possibility of ever
	 * possessing lp0, lp1 or lp2
	 */
	if (mdesc->reserve_lp0)
		request_resource(&ioport_resource, &lp0);
	if (mdesc->reserve_lp1)
		request_resource(&ioport_resource, &lp1);
	if (mdesc->reserve_lp2)
		request_resource(&ioport_resource, &lp2);
}

struct eth_addr {
	u8 addr[6];
};

struct eth_addr hw_addr[1] = {
	{ {0x00, 0x25, 0x9b, 0xff, 0x00, 0x00} }
};

EXPORT_SYMBOL(hw_addr);
/*
 *  Tag parsing.
 *
 * This is the new way of passing data to the kernel at boot time.  Rather
 * than passing a fixed inflexible structure to the kernel, we pass a list
 * of variable-sized tags to the kernel.  The first tag must be a ATAG_CORE
 * tag for the list to be recognised (to distinguish the tagged list from
 * a param_struct).  The list is terminated with a zero-length tag (this tag
 * is not parsed in any way).
 */
static int __init parse_tag_core(const struct tag *tag)
{
	if (tag->hdr.size > 2) {
		if ((tag->u.core.flags & 1) == 0)
			root_mountflags &= ~MS_RDONLY;
		ROOT_DEV = old_decode_dev(tag->u.core.rootdev);
	}
	return 0;
}

__tagtable(ATAG_CORE, parse_tag_core);

static int __init parse_tag_mem32(const struct tag *tag)
{
	return unicore_add_memory(tag->u.mem.start, tag->u.mem.size);
}

__tagtable(ATAG_MEM, parse_tag_mem32);

#if defined(CONFIG_VGA_CONSOLE) || defined(CONFIG_DUMMY_CONSOLE)
struct screen_info screen_info = {
 .orig_video_lines	= 30,
 .orig_video_cols	= 80,
 .orig_video_mode	= 0,
 .orig_video_ega_bx	= 0,
 .orig_video_isVGA	= 1,
 .orig_video_points	= 8
};

static int __init parse_tag_videotext(const struct tag *tag)
{
	screen_info.orig_x            = tag->u.videotext.x;
	screen_info.orig_y            = tag->u.videotext.y;
	screen_info.orig_video_page   = tag->u.videotext.video_page;
	screen_info.orig_video_mode   = tag->u.videotext.video_mode;
	screen_info.orig_video_cols   = tag->u.videotext.video_cols;
	screen_info.orig_video_ega_bx = tag->u.videotext.video_ega_bx;
	screen_info.orig_video_lines  = tag->u.videotext.video_lines;
	screen_info.orig_video_isVGA  = tag->u.videotext.video_isvga;
	screen_info.orig_video_points = tag->u.videotext.video_points;
	return 0;
}

__tagtable(ATAG_VIDEOTEXT, parse_tag_videotext);
#endif

static int __init parse_tag_ramdisk(const struct tag *tag)
{
	setup_ramdisk((tag->u.ramdisk.flags & 1) == 0,
		      (tag->u.ramdisk.flags & 2) == 0,
		      tag->u.ramdisk.start, tag->u.ramdisk.size);
	return 0;
}

__tagtable(ATAG_RAMDISK, parse_tag_ramdisk);

static int __init parse_tag_serialnr(const struct tag *tag)
{
	system_serial_low = tag->u.serialnr.low;
	system_serial_high = tag->u.serialnr.high;
	return 0;
}

__tagtable(ATAG_SERIAL, parse_tag_serialnr);

static int __init parse_tag_revision(const struct tag *tag)
{
	system_rev = tag->u.revision.rev;
	return 0;
}

__tagtable(ATAG_REVISION, parse_tag_revision);

static int __init parse_tag_cmdline(const struct tag *tag)
{
#ifdef CONFIG_CMDLINE_FROM_U_BOOT
	strlcpy(default_command_line, tag->u.cmdline.cmdline, COMMAND_LINE_SIZE);
#endif
	return 0;
}

__tagtable(ATAG_CMDLINE, parse_tag_cmdline);

static int __init parse_tag_ethernet(const struct tag *tag)
{
	int i = tag->u.ethernet.mac_index;

	if (i < ARRAY_SIZE(hw_addr))
		memcpy(hw_addr[i].addr, tag->u.ethernet.hw_address,
		       sizeof(hw_addr[i].addr));

	return 0;
}

__tagtable(ATAG_ETHERNET, parse_tag_ethernet);

/*
 * Scan the tag table for this tag, and call its parse function.
 * The tag table is built by the linker from all the __tagtable
 * declarations.
 */
static int __init parse_tag(const struct tag *tag)
{
	extern struct tagtable __tagtable_begin, __tagtable_end;
	struct tagtable *t;

	for (t = &__tagtable_begin; t < &__tagtable_end; t++)
		if (tag->hdr.tag == t->tag) {
			t->parse(tag);
			break;
		}

	return t < &__tagtable_end;
}

/*
 * Parse all tags in the list, checking both the global and architecture
 * specific tag tables.
 */
static void __init parse_tags(const struct tag *t)
{
	for (; t->hdr.size; t = tag_next(t))
		if (!parse_tag(t))
			printk(KERN_WARNING
				"Ignoring unrecognised tag 0x%08x\n",
				t->hdr.tag);
}

/*
 * This holds our defaults.
 */
static struct init_tags {
	struct tag_header hdr1;
	struct tag_core   core;
	struct tag_header hdr2;
	struct tag_mem32  mem;
	struct tag_header hdr3;
} init_tags __initdata = {
	{ tag_size(tag_core), ATAG_CORE },
	{ 1, PAGE_SIZE, 0xff },
	{ tag_size(tag_mem32), ATAG_MEM },
	{ MEM_SIZE, PHYS_OFFSET },
	{ 0, ATAG_NONE }
};

static void (*init_machine)(void) __initdata;

static int __init customize_machine(void)
{
	/* customizes platform devices, or adds new ones */
	if (init_machine)
		init_machine();
	return 0;
}
arch_initcall(customize_machine);

void __init setup_arch(char **cmdline_p)
{
	struct tag *tags = (struct tag *)&init_tags;
	struct machine_desc *mdesc;
	char *from = default_command_line;

	setup_processor();
	mdesc = setup_machine(__machine_arch_type);
	machine_name = mdesc->name;

	if (mdesc->soft_reboot)
		reboot_setup("s");

	if (__atags_pointer)
		tags = phys_to_virt(__atags_pointer);
	else if (mdesc->boot_params)
		tags = phys_to_virt(mdesc->boot_params);

	/*
	 * If we have the old style parameters, convert them to
	 * a tag list.
	 */
	if (tags->hdr.tag != ATAG_CORE)
		convert_to_tag_list(tags);
	if (tags->hdr.tag != ATAG_CORE)
		tags = (struct tag *)&init_tags;

	if (mdesc->fixup)
		mdesc->fixup(mdesc, tags, &from, &meminfo);

	if (tags->hdr.tag == ATAG_CORE) {
		if (meminfo.nr_banks != 0)
			squash_mem_tags(tags);
		parse_tags(tags);
	}

	init_mm.start_code = (unsigned long) _text;
	init_mm.end_code   = (unsigned long) _etext;
	init_mm.end_data   = (unsigned long) _edata;
	init_mm.brk	   = (unsigned long) _end;

	memcpy(boot_command_line, from, COMMAND_LINE_SIZE);
	boot_command_line[COMMAND_LINE_SIZE-1] = '\0';
	parse_cmdline(cmdline_p, from);
	paging_init(mdesc);
	request_standard_resources(&meminfo, mdesc);

	cpu_init();

	/*
	 * Set up various architecture-specific pointers
	 */
	init_arch_irq = mdesc->init_irq;
	system_timer = mdesc->timer;
	init_machine = mdesc->init_machine;
#ifdef CONFIG_VT
#if defined(CONFIG_VGA_CONSOLE)
	conswitchp = &vga_con;
#elif defined(CONFIG_DUMMY_CONSOLE)
	conswitchp = &dummy_con;
#endif
#endif
	early_trap_init();
}


static int __init topology_init(void)
{
	int cpu;

	for_each_possible_cpu(cpu) {
		struct cpuinfo_unicore *cpuinfo = &per_cpu(cpu_data, cpu);
		cpuinfo->cpu.hotpluggable = 1;
		register_cpu(&cpuinfo->cpu, cpu);
	}

	return 0;
}

subsys_initcall(topology_init);

static const char *hwcap_str[] = {
	"msp",
	"unicore16",
	"cmov",
        "unicore-f64",
	NULL
};

static int c_show(struct seq_file *m, void *v)
{
	int i;

	seq_printf(m, "Processor\t: %s rev %d (%s)\n",
		   cpu_name, (int)(read_cpuid_id() >> 16) & 15, elf_platform);

	seq_printf(m, "BogoMIPS\t: %lu.%02lu\n",
		   loops_per_jiffy / (500000/HZ),
		   (loops_per_jiffy / (5000/HZ)) % 100);

	/* dump out the processor features */
	seq_puts(m, "Features\t: ");

	for (i = 0; hwcap_str[i]; i++)
		if (elf_hwcap & (1 << i))
			seq_printf(m, "%s ", hwcap_str[i]);

	seq_printf(m, "\nCPU implementer\t: 0x%02x\n", read_cpuid_id() >> 24);
	seq_printf(m, "CPU architecture: %s\n", proc_arch[cpu_architecture()]);
	seq_printf(m, "CPU revision\t: %d\n", (read_cpuid_id() >> 16) & 15);

	seq_printf(m, "Cache type\t: write-back\n"
                   "Cache clean\t: cp0 c5 ops\n"
                   "Cache lockdown\t: not support\n"
                   "Cache format\t: Harvard\n");

	seq_puts(m, "\n");

	seq_printf(m, "Hardware\t: %s\n", machine_name);
	seq_printf(m, "Revision\t: %04x\n", system_rev);
	seq_printf(m, "Serial\t\t: %08x%08x\n",
		   system_serial_high, system_serial_low);

	return 0;
}

static void *c_start(struct seq_file *m, loff_t *pos)
{
	return *pos < 1 ? (void *)1 : NULL;
}

static void *c_next(struct seq_file *m, void *v, loff_t *pos)
{
	++*pos;
	return NULL;
}

static void c_stop(struct seq_file *m, void *v)
{
}

const struct seq_operations cpuinfo_op = {
	.start	= c_start,
	.next	= c_next,
	.stop	= c_stop,
	.show	= c_show
};
