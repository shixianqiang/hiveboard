/* linux/arch/unicore/mach-sep0611/irq.c
 *
 * Copyright (c) 2009-2011 SEUIC
 *
 * Southeast University ASIC SoC support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  09-04-2010	Changelog initial version
 *  05-31-2011	cgm Updated for release version
 *
 */
 
#include <linux/init.h>
#include <linux/interrupt.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/mach/irq.h>
#include <mach/hardware.h>

#define SEP0611_IRQ_TYPE 0
#define SEP0611_FIQ_TYPE 1

#define int_enable_all()									\
do{															\
	*(volatile unsigned long*)VIC_INT_EN_L_V = 0xffffffff;	\
	*(volatile unsigned long*)VIC_INT_EN_H_V = 0xffffffff;	\
}while(0)

#define int_disable_all()                         			\
do{                                               			\
	*(volatile unsigned long*)VIC_INT_EN_L_V = 0x0;  		\
 	*(volatile unsigned long*)VIC_INT_EN_H_V = 0x0;      	\
}while(0)

#define mask_all_int(int_type)                     			\
do{                                                			\
	if(int_type == SEP0611_IRQ_TYPE)              			\
    	*(volatile unsigned long*)VIC_INT_MSK_ALL_V = 0x1;	\
  	else if(int_type == SEP0611_FIQ_TYPE)           		\
      	*(volatile unsigned long*)VIC_INT_MSK_ALL_V = 0x2;	\
}while(0)

#define unmask_all_int(int_type)                     		\
do{                                                  		\
	if(int_type == SEP0611_IRQ_TYPE)                		\
    	*(volatile unsigned long*)VIC_INT_MSK_ALL_V &= 0xfffffffe;	\
 	else if(int_type == SEP0611_FIQ_TYPE)            		\
     	*(volatile unsigned long*)VIC_INT_MSK_ALL_V &= 0xfffffffd;	\
}while(0)

static void sep0611_irq_enable(unsigned int irq)
{
	SEP0611_INT_ENABLE(irq);
}

static void sep0611_irq_disable(unsigned int irq)
{
    SEP0611_INT_DISABLE(irq);
}

static void sep0611_irq_mask(unsigned int irq)
{
	SEP0611_INT_DISABLE(irq);
}

static void sep0611_irq_ack(unsigned int irq)
{
    SEP0611_INT_DISABLE(irq);
}

static void sep0611_irq_unmask(unsigned int irq)
{
	SEP0611_INT_ENABLE(irq);  
} 

static struct irq_chip sep0611_chip = {
	.ack    = sep0611_irq_ack,
	.mask   = sep0611_irq_mask,
	.unmask = sep0611_irq_unmask,
	.disable= sep0611_irq_disable,
	.enable	= sep0611_irq_enable,
};
 
void __init sep0611_init_irq(void)
{
	unsigned int i;
	unsigned long flags;
	
	local_save_flags(flags);
	
	int_disable_all();
	mask_all_int(SEP0611_FIQ_TYPE);
	
    int_enable_all();
	unmask_all_int(SEP0611_IRQ_TYPE);
    
	local_irq_restore(flags);	
 
 	for(i = 0; i < NR_IRQS; i++){
		set_irq_chip(i, &sep0611_chip);
		set_irq_handler(i, handle_level_irq);
	 	set_irq_flags(i, IRQF_VALID);
	}
}
