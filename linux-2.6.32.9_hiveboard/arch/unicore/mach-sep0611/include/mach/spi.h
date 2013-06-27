/* linux/arch/unicore/mach-sep0611/include/mach/spi.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *  cgm <chenguangming@wxseuic.com>
 *
 * Southeast University ASIC SoC support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  05-31-2011	cgm initial version
 *
 */
 
#ifndef __ASM_ARCH_SPI_H
#define __ASM_ARCH_SPI_H

struct sep0611_spi_info {
	unsigned long pin_cs;	/* simple gpio cs */
	unsigned int num_cs;	/* total chipselects */
	int bus_num;       		/* bus number to use. */
	void (*set_cs)(struct sep0611_spi_info *spi, int cs, int pol);
};

#endif /* __ASM_ARCH_SPI_H */
