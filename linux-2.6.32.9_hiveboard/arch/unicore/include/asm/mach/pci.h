/*
+ * linux/arch/unicore/include/asm/mach/pci.h
+ *
+ * Code specific to PKUnity SoC and UniCore ISA
+ * Fragments that appear the same as the files in arm or x86
+ *
+ * Copyright (C) 2001-2008 GUAN Xue-tao
+ *
+ * This program is free software; you can redistribute it and/or modify
+ * it under the terms of the GNU General Public License version 2 as
+ * published by the Free Software Foundation.
+ */

struct pci_sys_data;
struct pci_bus;

struct hw_pci {
	struct list_head buses;
	int		nr_controllers;
	int		(*setup)(int nr, struct pci_sys_data *);
	struct pci_bus *(*scan)(int nr, struct pci_sys_data *);
	void		(*preinit)(void);
	void		(*postinit)(void);
	u8		(*swizzle)(struct pci_dev *dev, u8 *pin);
	int		(*map_irq)(struct pci_dev *dev, u8 slot, u8 pin);
};

/*
+ * Per-controller structure
+ */
struct pci_sys_data {
	struct list_head node;
	int		busnr;		/* primary bus number			*/
	u64		mem_offset;	/* bus->cpu memory mapping offset	*/
	unsigned long	io_offset;	/* bus->cpu IO mapping offset		*/
	struct pci_bus	*bus;		/* PCI bus				*/
	struct resource *resource[3];	/* Primary PCI bus resources		*/
					/* Bridge swizzling			*/
	u8		(*swizzle)(struct pci_dev *, u8 *);
					/* IRQ mapping				*/
	int		(*map_irq)(struct pci_dev *, u8, u8);
	struct hw_pci	*hw;
};

/*
+ * This is the standard PCI-PCI bridge swizzling algorithm.
+ */
#define pci_std_swizzle pci_common_swizzle

/*
+ * Call this with your hw_pci struct to initialise the PCI system.
+ */
void pci_common_init(struct hw_pci *);
