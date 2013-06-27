/* linux/arch/unicore/mach-sep0611/include/mach/gpio.h
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

#ifndef __ASM_MACH_GPIO_H
#define __ASM_MACH_GPIO_H

#define GPIO_HIGH	1
#define GPIO_LOW	0

#define PORT_A  	(0<<5)
#define PORT_B  	(1<<5)
#define PORT_C   	(2<<5)
#define PORT_D  	(3<<5)
#define PORT_E   	(4<<5)
#define PORT_F   	(5<<5)
#define PORT_G   	(6<<5)
#define PORT_H   	(7<<5)
#define PORT_I   	(8<<5)
#define PORT_AO  	(9<<5)

/* the gpio regs offset from base */
#define GP_DIR		0x00
#define GP_SEL		0x04
#define GP_DAT 		0x08
#define GP_INTSEL 	0x0C
#define GP_INTLEL 	0x10
#define GP_INTPOL 	0x14
#define GP_INTCLR 	0x18
#define GP_SECFUN 	0x1C

enum gpio_irq_trig{
	UP_TRIG = 0,
	DOWN_TRIG,
	HIGH_TRIG,
	LOW_TRIG,
};

/*
 * the parameter 'pin', is a line number defined in <mach/regs-gpio.h>
 */

/*
 * set the extern interrupt
 */
extern void sep0611_gpio_setirq(unsigned int pin, enum gpio_irq_trig trig);

/*
 * get the extern interrupt irq num, it is not usually use
 */
extern int sep0611_gpio_getirq(unsigned int pin);

/*
 * clear the pin's irq flag
 */
extern int sep0611_gpio_clrirq(unsigned int pin);

/*
 * get the pin's value, 0 is low level, 1 is high level
 */
extern int sep0611_gpio_getpin(unsigned int pin);

/*
 * set the pin's value, 0 is low level, 1 is high level
 */
extern void sep0611_gpio_setpin(unsigned int pin, unsigned int val);

/*
 * set the pin's direction, 0 is outupt, 1 is input
 */
extern void sep0611_gpio_dirpin(unsigned int pin, unsigned int in);

/*
 * config the pin's function, 0 is IO, 1 is special function 1, 2 is special function 2,
 * recommend to use SEP0611_GPIO_IO, SEP0611_GPIO_SFN, SEP0611_GPIO_SFN2
 */
extern void sep0611_gpio_cfgpin(unsigned int pin, unsigned int function);


/*
 * the gpio's group operations, all the pins in group are in the same
 */
 
/*
 * config the group pin's function, 0 is IO, 1 is special function 1, 2 is special function 2,
 * recommend to use SEP0611_GPIO_IO, SEP0611_GPIO_SFN, SEP0611_GPIO_SFN2
 */
extern int sep0611_gpio_cfggrp(unsigned int start_pin, unsigned int num, unsigned int function);

/*
 * set the group pin's value, 0 is low level, 1 is high level
 */
extern int sep0611_gpio_setgrp(unsigned int start_pin, unsigned int num, unsigned int val);

/*
 * get the group pin's value, 0 is low level, 1 is high level
 */
extern int sep0611_gpio_getgrp(unsigned int start_pin, unsigned int num);

/*
 * set the group pin's direction, 0 is outupt, 1 is input
 */
extern int sep0611_gpio_dirgrp(unsigned int start_pin, unsigned int num, unsigned int in);


/* this gpio means a pin */
#define ARCH_NR_GPIOS (32 * 10)
#define gpio_to_irq(gpio) __gpio_to_irq(gpio)
#define gpio_get_value(gpio) __gpio_get_value(gpio)
#define gpio_set_value(gpio, value) __gpio_set_value(gpio, value)

#include <asm-generic/gpio.h>

#endif /* __ASM_MACH_GPIO_H */

