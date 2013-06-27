/* linux/arch/unicore/mach-sep0611/gpiolib.c
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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/sysdev.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/gpio.h>

#include <mach/hardware.h>
#include <mach/gpio.h>

static inline unsigned long sep0611_gpio_to_reg(struct gpio_chip *chip)
{
	unsigned long gpio_base = 0;
	
	switch(chip->base){
	case PORT_A:
		gpio_base = GPIO_PORTA_V;
		break;
	
	case PORT_B:
		gpio_base = GPIO_PORTB_V;
		break;
		
	case PORT_C:
		gpio_base = GPIO_PORTC_V;
		break;
		
	case PORT_D:
		gpio_base = GPIO_PORTD_V;
		break;
		
	case PORT_E:
		gpio_base = GPIO_PORTE_V;
		break;
		
	case PORT_F:
		gpio_base = GPIO_PORTF_V;
		break;
		
	case PORT_I:
		gpio_base = GPIO_PORTI_V;
		break;
		
	case PORT_AO:
		gpio_base = GPIO_AO_V;
		break;
	}
	
	return gpio_base;
}

static int sep0611_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	unsigned long gpio_data;
	
	gpio_data = sep0611_gpio_to_reg(chip) + GP_DAT;

	return (readl(gpio_data) >> offset) & 1;
}

static void sep0611_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
	unsigned long gpio_data;
	
	gpio_data = sep0611_gpio_to_reg(chip) + GP_DAT;
	
	if (value)
		writel(readl(gpio_data) | (1 << offset), gpio_data);
	else
		writel(readl(gpio_data) & ~(1 << offset), gpio_data);
}

static int sep0611_gpio_direction_input(struct gpio_chip *chip, unsigned offset)
{
	unsigned long gpio_dir;
	
	gpio_dir = sep0611_gpio_to_reg(chip) + GP_DIR;

	writel(readl(gpio_dir) | (1 << offset), gpio_dir);

	return 0;
}

static int sep0611_gpio_direction_output(struct gpio_chip *chip, unsigned offset, int value)
{
	unsigned long gpio_dir;
	
	gpio_dir = sep0611_gpio_to_reg(chip) + GP_DIR;

	writel(readl(gpio_dir) & ~(1 << offset), gpio_dir);
	
	return 0;
}

static int sep0611_gpioi_to_irq(struct gpio_chip *chip, unsigned offset)
{
	if(likely(offset >= INTSRC_EXT4 && offset <= INTSRC_EXT15))
		return offset;
	else
		return -EINVAL;
}

static int sep0611_gpioao_to_irq(struct gpio_chip *chip, unsigned offset)
{
	if(likely(offset <= INTSRC_AO_EXT3))
		return offset;
	else
		return -EINVAL;

	return 0;
}

static struct gpio_chip sep0611_gpio_chips[] = {
	{
		.label	= "PORTA",
		.base	= PORT_A,
		.ngpio	= 17,
	},
	
	{
		.label	= "PORTB",
		.base 	= PORT_B,
		.ngpio	= 16,
	},
	
	{
		.label	= "PORTC",
		.base 	= PORT_C,
		.ngpio	= 16,
	},
	
	{
		.label 	= "PORTD",
		.base	= PORT_D,
		.ngpio	= 19,
	},
	
	{
		.label	= "PORTE",
		.base	= PORT_E,
		.ngpio	= 22,
	},
	
	{
		.label	= "PORTF",
		.base	= PORT_F,
		.ngpio	= 29,
	},
	
	{
		.label	= "reserve-g",
		.base	= PORT_G,
		.ngpio	= 0,
	},
	
	{
		.label	= "reserve-h",
		.base	= PORT_H,
		.ngpio	= 0,
	},
	
	{
		.label	= "PORTI",
		.base	= PORT_I,
		.ngpio	= 16,
		.to_irq = sep0611_gpioi_to_irq,
	},
	
	{
		.label	= "PORT_AO",
		.base	= PORT_AO,
		.ngpio	= 8,
		.to_irq = sep0611_gpioao_to_irq,
	},
};

int __init sep0611_gpio_init(void)
{
	struct gpio_chip *chip;
	int i;

	for (i = 0; i < ARRAY_SIZE(sep0611_gpio_chips); i++) {
		chip = &sep0611_gpio_chips[i];

		chip->direction_input = sep0611_gpio_direction_input;
		chip->direction_output = sep0611_gpio_direction_output;
		chip->set = sep0611_gpio_set;
		chip->get = sep0611_gpio_get;

		gpiochip_add(chip);
	}
	return 0;
}

postcore_initcall(sep0611_gpio_init);
