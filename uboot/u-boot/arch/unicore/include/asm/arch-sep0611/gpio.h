 /* 
 *
 * Copyright (c) 2009-2011 SEUIC
 *  chinawrj <chinawrj@gmail.com>
 *
 * Southeast University ASIC SoC support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  06-15-2011	chinawrj chinawrj@gmail.com
 *
 */

#ifndef __ASM_MACH_GPIO_H
#define __ASM_MACH_GPIO_H

#define PORT_A  	0x1010
#define PORT_B  	0x1030
#define PORT_C   	0x1050
#define PORT_D  	0x1070
#define PORT_E   	0x1090
#define PORT_F   	0x10b0
#define PORT_I   	0x1110
#define PORT_AO  	0x0110

/* the gpio regs offset from base */
#define GP_DIR		0x00
#define GP_SEL		0x04
#define GP_DAT 		0x08
#define GP_INTSEL 	0x0C
#define GP_INTLEL 	0x10
#define GP_INTPOL 	0x14
#define GP_INTCLR 	0x18
#define GP_SECFUN 	0x1C

#define GPIO_SET3(group, offs, func) \
	({\
		*(volatile unsigned int*)(GPIO_AO_BASE + group + func) |= (1<<offs);\
	})

#define GPIO_CLEAR3(group, offs, func) \
	({\
		*(volatile unsigned int*)(GPIO_AO_BASE + group + func) &= ~(1<<offs);\
	})

#define GPIO_SET(num, func) \
	({\
		GPIO_SET3(num, func); \
	})

#define GPIO_CLEAR(num, func) \
	({\
		GPIO_CLEAR3(num, func); \
	})

#define GPIO_OUT_HIGH(num) \
	({\
		GPIO_CLEAR3(num, GP_DIR); \
		GPIO_SET3(num, GP_SEL); \
		GPIO_SET3(num, GP_DAT); \
	})

#define GPIO_OUT_LOW(num) \
	({\
		GPIO_CLEAR3(num, GP_DIR); \
		GPIO_SET3(num, GP_SEL); \
		GPIO_CLEAR3(num, GP_DAT); \
	})

#endif
