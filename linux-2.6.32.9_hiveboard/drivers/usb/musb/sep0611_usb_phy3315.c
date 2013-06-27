/*
 * linux/drivers/usb/musb/sep0611_usb.c
 *
 * Code specific to ASIC center of SouthEast University
 nux-611-800-180-400$ *
*Maintained by Shi Gui-jun <yinfeng_best@163.com>
*	Copyright (C) 2001-2011 ASIC & SouthEast University
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
* Based on: drivers/usb/musb/omap2430.c
*
* Contributors & Additions/Fixes:
* 	First version by Shi Guijun & Zhou Xin & Chen Minmin
* 
* TODO:
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/clk.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <mach/irqs.h>
#include <mach/gpio.h>
#include <board/board.h>
#include <linux/usb/otg.h>

#include "musb_core.h"

#define MUSB_TIMEOUT_A_WAIT_BCON	1100

/* Disable double buffer packet */
#define RX_DPKTBUFDIS           0x0340
#define TX_DPKTBUFDIS           0x0342

static struct timer_list musb_idle_timer;

static void musb_do_idle(unsigned long _musb)
{
    struct musb	*musb = (void *)_musb;
    unsigned long	flags;
    u8	power;
    u8	devctl;

    spin_lock_irqsave(&musb->lock, flags);

    devctl = musb_readb(musb->mregs, MUSB_DEVCTL);
  switch (musb->xceiv->state) {
    case OTG_STATE_A_IDLE:
    case OTG_STATE_A_WAIT_BCON:
    devctl &= ~MUSB_DEVCTL_SESSION;            
    musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);  
    devctl = musb_readb(musb->mregs, MUSB_DEVCTL); 
    devctl |= MUSB_DEVCTL_SESSION;
    musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);
    devctl = musb_readb(musb->mregs, MUSB_DEVCTL);
  if (devctl & MUSB_DEVCTL_BDEVICE) {
    musb->xceiv->state = OTG_STATE_B_IDLE;
    MUSB_DEV_MODE(musb);
    } else {
    musb->xceiv->state = OTG_STATE_A_HOST;                       
    MUSB_HST_MODE(musb);
    }
  if (!musb->is_active)  {                                          
    mod_timer(&musb_idle_timer, jiffies + msecs_to_jiffies(300));
    }
    break;
#ifdef CONFIG_USB_MUSB_HDRC_HCD
  case OTG_STATE_A_SUSPEND:
/* finish RESUME signaling? */
    if (musb->port1_status & MUSB_PORT_STAT_RESUME) {
    power = musb_readb(musb->mregs, MUSB_POWER);
    power &= ~MUSB_POWER_RESUME;
    DBG(1, "root port resume stopped, power %02x\n", power);
    musb_writeb(musb->mregs, MUSB_POWER, power);
    musb->is_active = 1;
    musb->port1_status &= ~(USB_PORT_STAT_SUSPEND
    | MUSB_PORT_STAT_RESUME);
    musb->port1_status |= USB_PORT_STAT_C_SUSPEND << 16;
    usb_hcd_poll_rh_status(musb_to_hcd(musb));
/* NOTE: it might really be A_WAIT_BCON ... */
    musb->xceiv->state = OTG_STATE_A_HOST;
}
    break;
#endif
#ifdef CONFIG_USB_MUSB_HDRC_HCD
  case OTG_STATE_A_HOST:
    devctl = musb_readb(musb->mregs, MUSB_DEVCTL);
    if (devctl &  MUSB_DEVCTL_BDEVICE)
    musb->xceiv->state = OTG_STATE_B_IDLE;
    else
    musb->xceiv->state = OTG_STATE_A_WAIT_BCON;
#endif
   default:
    devctl |= MUSB_DEVCTL_SESSION;                                
    musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);                 
    devctl = musb_readb(musb->mregs, MUSB_DEVCTL);                
    if (devctl & MUSB_DEVCTL_BDEVICE) {                          
    musb->xceiv->state = OTG_STATE_B_IDLE;  
			MUSB_DEV_MODE(musb);                                         
		} else {                                                  
			musb->xceiv->state = OTG_STATE_A_HOST;          
			MUSB_HST_MODE(musb);                                    
		}                                                                                                                                                      
		
    if (!musb->is_active)  {                                          
    mod_timer(&musb_idle_timer, jiffies + msecs_to_jiffies(300));           
		}         
    break;
	}
    spin_unlock_irqrestore(&musb->lock, flags);
}

void musb_platform_try_idle(struct musb *musb, unsigned long timeout)
{
    unsigned long		default_timeout = jiffies + msecs_to_jiffies(300);
    static unsigned long	last_timer;

    if (timeout == 0)
    timeout = default_timeout;

	/* Never idle if active, or when VBUS timeout is not set as host */
    if (musb->is_active || ((musb->a_wait_bcon == 0)
			&& (musb->xceiv->state == OTG_STATE_A_WAIT_BCON))) {
    DBG(4, "%s active, deleting timer\n", otg_state_string(musb));
    del_timer(&musb_idle_timer);
    last_timer = jiffies;
    return;
	}

    if (time_after(last_timer, timeout)) {
		   if (!timer_pending(&musb_idle_timer))
			 last_timer = timeout;
		   else {
			 DBG(4, "Longer idle timer already pending, ignoring\n");
			 return;
		}
	}
    last_timer = timeout;

    DBG(4, "%s inactive, for idle timer for %lu ms\n",
    otg_state_string(musb),
    (unsigned long)jiffies_to_msecs(timeout - jiffies));
    mod_timer(&musb_idle_timer, timeout);
}

/*init USB phy ,enable irq */    
void musb_platform_enable(struct musb *musb)
{
#ifdef SEP0611_PHY_RESET	
    sep0611_gpio_cfgpin(SEP0611_PHY_RESET, SEP0611_GPIO_IO);   /*AO_PORT general purpose  */
    sep0611_gpio_dirpin(SEP0611_PHY_RESET, SEP0611_GPIO_OUT);   /* AO_PORT output    */
    sep0611_gpio_setpin(SEP0611_PHY_RESET, 1);                 /* AO_PORT output data */
#endif   
 
#ifdef SEP0611_PHY_RST

    sep0611_gpio_cfgpin(SEP0611_USB_EN, SEP0611_GPIO_IO);          
    sep0611_gpio_dirpin(SEP0611_USB_EN, SEP0611_GPIO_OUT);
    sep0611_gpio_setpin(SEP0611_USB_EN, 1);
    
    sep0611_gpio_cfgpin(SEP0611_PHY_RST, SEP0611_GPIO_IO);        
    sep0611_gpio_dirpin(SEP0611_PHY_RST, SEP0611_GPIO_OUT);
    sep0611_gpio_setpin(SEP0611_PHY_RST, 1);  
    
    sep0611_gpio_cfgpin(SEP0611_USB5V_EN, SEP0611_GPIO_IO);       
    sep0611_gpio_dirpin(SEP0611_USB5V_EN, SEP0611_GPIO_OUT);
    sep0611_gpio_setpin(SEP0611_USB5V_EN, 1); 

    sep0611_gpio_cfgpin(SEP0611_USB_DE_SEL, SEP0611_GPIO_IO);    
    sep0611_gpio_dirpin(SEP0611_USB_DE_SEL, SEP0611_GPIO_OUT);
    sep0611_gpio_setpin(SEP0611_USB_DE_SEL, 0);

    sep0611_gpio_cfgpin(SEP0611_VBUS_IN_SEL, SEP0611_GPIO_IO);   
    sep0611_gpio_dirpin(SEP0611_VBUS_IN_SEL, SEP0611_GPIO_OUT);
    sep0611_gpio_setpin(SEP0611_VBUS_IN_SEL, 1);
	
    sep0611_gpio_cfgpin(SEP0611_HVBUS_ON, SEP0611_GPIO_IO);        
    sep0611_gpio_dirpin(SEP0611_HVBUS_ON, SEP0611_GPIO_OUT);
    sep0611_gpio_setpin(SEP0611_HVBUS_ON, 0);

    sep0611_gpio_cfgpin(SEP0611_HID_VAL, SEP0611_GPIO_IO);       
    sep0611_gpio_dirpin(SEP0611_HID_VAL, SEP0611_GPIO_OUT);
    sep0611_gpio_setpin(SEP0611_HID_VAL, 0);
    
    sep0611_gpio_cfgpin(SEP0611_OTG_VBUS_ON, SEP0611_GPIO_IO);       
    sep0611_gpio_dirpin(SEP0611_OTG_VBUS_ON, SEP0611_GPIO_IN);
    sep0611_gpio_setpin(SEP0611_OTG_VBUS_ON, 0); 
   
#endif
    
#ifdef SEP0611_ULPI_RST
    sep0611_gpio_cfgpin(SEP0611_USB_EN, SEP0611_GPIO_IO);          
    sep0611_gpio_dirpin(SEP0611_USB_EN, SEP0611_GPIO_OUT);
    sep0611_gpio_setpin(SEP0611_USB_EN, 1);

    sep0611_gpio_cfgpin(SEP0611_ULPI_RST, SEP0611_GPIO_IO);          
    sep0611_gpio_dirpin(SEP0611_ULPI_RST, SEP0611_GPIO_OUT);
    sep0611_gpio_setpin(SEP0611_ULPI_RST, 1);

    sep0611_gpio_cfgpin(SEP0611_HID_VAL, SEP0611_GPIO_IO);          
    sep0611_gpio_dirpin(SEP0611_HID_VAL, SEP0611_GPIO_OUT);
    sep0611_gpio_setpin(SEP0611_HID_VAL, 0);

    sep0611_gpio_cfgpin(SEP0611_USB_DE_SEL, SEP0611_GPIO_IO);          
    sep0611_gpio_dirpin(SEP0611_USB_DE_SEL, SEP0611_GPIO_OUT);
    sep0611_gpio_setpin(SEP0611_USB_DE_SEL, 0);

#endif

    SEP0611_INT_ENABLE(INTSRC_USB);
    SEP0611_INT_ENABLE(INTSRC_USBDMA);
}

void musb_platform_disable(struct musb *musb)
{
    SEP0611_INT_DISABLE(INTSRC_USB); 
    SEP0611_INT_DISABLE(INTSRC_USBDMA);
}

static void sep0611_set_vbus(struct musb *musb, int is_on)
{
    u8		devctl;
	/* HDRC controls CPEN, but beware current surges during device
	 * connect.  They can trigger transient overcurrent conditions
	 * that must be ignored.
	 */

    devctl = musb_readb(musb->mregs, MUSB_DEVCTL);

	if (is_on) {
    musb->is_active = 1;
    musb->xceiv->default_a = 1;
    musb->xceiv->state = OTG_STATE_A_WAIT_VRISE;
    devctl |= MUSB_DEVCTL_SESSION;
    MUSB_HST_MODE(musb);
	} else {
    musb->is_active = 0;

		/* NOTE:  we're skipping A_WAIT_VFALL -> A_IDLE and
		 * jumping right to B_IDLE...
		 */

    musb->xceiv->default_a = 0;
    musb->xceiv->state = OTG_STATE_B_IDLE;
    devctl &= ~MUSB_DEVCTL_SESSION;
    MUSB_DEV_MODE(musb);
	}
    musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);

    DBG(1, "VBUS %s, devctl %02x "
		/* otg %3x conf %08x prcm %08x */ "\n",
    otg_state_string(musb),
    musb_readb(musb->mregs, MUSB_DEVCTL));
}

static int sep0611_set_power(struct otg_transceiver *x, unsigned mA)
{
    return 0;
}

static int musb_platform_resume(struct musb *musb);

int musb_platform_set_mode(struct musb *musb, u8 musb_mode)
{
    u8	devctl = musb_readb(musb->mregs, MUSB_DEVCTL);

    devctl |= MUSB_DEVCTL_SESSION;
    musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);

    switch (musb_mode) {
#ifdef CONFIG_USB_MUSB_HDRC_HCD
    case MUSB_HOST:
    otg_set_host(musb->xceiv, musb->xceiv->host);
    break;
#endif
#ifdef CONFIG_USB_GADGET_MUSB_HDRC
    case MUSB_PERIPHERAL:
    otg_set_peripheral(musb->xceiv, musb->xceiv->gadget);
    break;
#endif
#ifdef CONFIG_USB_MUSB_OTG
    case MUSB_OTG:
    break;
#endif
    default:
    return -EINVAL;
	}
    return 0;
}

static int sep0611_set_host(struct otg_transceiver *otg, struct usb_bus *host)
{
    otg->host = host;
    return 0;
}

static int sep0611_set_peripheral(struct otg_transceiver *otg,struct usb_gadget *gadget)
{
    otg->gadget = gadget;
    return 0;
}

int __init musb_platform_init(struct musb *musb)
{
   struct otg_transceiver *xceiv;
	
    xceiv = kzalloc(sizeof(struct otg_transceiver), GFP_KERNEL);
    if (!xceiv)
    return -ENOMEM;
	
    xceiv->set_peripheral = sep0611_set_peripheral;
	
    otg_set_transceiver(xceiv);
    musb->xceiv = otg_get_transceiver();
    if (!musb->xceiv) {

    return -ENODEV;
	}
        
    musb_platform_resume(musb);

    if (is_host_enabled(musb))
    musb->xceiv->set_host = sep0611_set_host;
    musb->board_set_vbus = sep0611_set_vbus;
    if (is_peripheral_enabled(musb))
    musb->xceiv->set_power = sep0611_set_power;
    musb->a_wait_bcon = MUSB_TIMEOUT_A_WAIT_BCON;

    setup_timer(&musb_idle_timer, musb_do_idle, (unsigned long) musb);

    musb_writew(musb->mregs, RX_DPKTBUFDIS, 0xffff); /* disable RX double packet buffer */
    musb_writew(musb->mregs, TX_DPKTBUFDIS, 0xffff); /* disable TX double packet buffer */

#ifdef SEP0611_ULPI_RST 
/*spdw_board has two usb ports,one is otg(port_index=0), the other is host(port_index=1),default is otg port*/
    musb->port_index = 0;               
#endif

#ifdef  SEP0611_PHY_RST
/*demo_board has two usb ports*/
    musb->port_index = 0;               
#endif
    return 0;
}

int musb_platform_suspend(struct musb *musb)
{
    if (!musb->clock)
    return 0;

    if (musb->xceiv->set_suspend)
    musb->xceiv->set_suspend(musb->xceiv, 1);

    if (musb->set_clock)
    musb->set_clock(musb->clock, 0);
    else
    clk_disable(musb->clock);

    return 0;
}

static int musb_platform_resume(struct musb *musb)
{
    if (!musb->clock)
    return 0;

    if (musb->xceiv->set_suspend)
    musb->xceiv->set_suspend(musb->xceiv, 0);

    if (musb->set_clock)
    musb->set_clock(musb->clock, 1);
    else
    clk_enable(musb->clock);

    return 0;
}


int musb_platform_exit(struct musb *musb)
{

    musb_platform_suspend(musb);

    kfree(musb->xceiv);

    clk_put(musb->clock);
    musb->clock = 0;
    return 0;
}
