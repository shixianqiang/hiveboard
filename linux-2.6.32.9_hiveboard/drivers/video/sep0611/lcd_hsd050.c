/*
 * hsd050_lcd.c -- support for HSD050 LCD
 *
 * Copyright (C) 2009, 2010 Telechips, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/mutex.h>
#include <linux/delay.h>

#include <mach/hardware.h>
#include <asm/io.h>
#include <mach/gpio.h>

<<<<<<< HEAD
/********* macros for the specific TFT screen***********/
/* macro for LCDC size control register(LCDC_SCR) */
#define		SEP0611_LCD_WIDTH	800
#define		SEP0611_LCD_HEIGHT	480

#define		_800_480_PIXPOL		0
#define		_800_480_FLMPOL		1
#define		_800_480_LPPOL		1
#define		_800_480_CLKPOL		0 
#define		_800_480_OEPOL		0
//#define		_800_480_PCD		4
#define		FRESH_RATE			80

/* Macro for Panel line control register(LCDC_PLCR) */
#define		_800_480_H_WIDTH	30
#define		_800_480_H_WAIT1	19
#define		_800_480_H_WAIT2	20

/* macro for Panel frame control register(LCDC_PFCR) */
#define		_800_480_V_WIDTH	1
#define		_800_480_V_WAIT1	6
#define		_800_480_V_WAIT2	12

/* macro for Panel contrast control register(LCDC_PCCR) */
#define		_800_480_SCR		1
#define		_800_480_CC_EN		1
#define		_800_480_PW			0
=======
#include "sep0611_fb.h"
>>>>>>> master

/*****************************************************************************
*
* Defines
*
******************************************************************************/
/*GPIO bit for the back light and lcdc_enable*/
#define		GPIO_LCDENABLE_BIT	(1 << 8) //PI8
#define		GPIO_BACKLIGHT_BIT	(1 << 6) //PI6

static struct mutex panel_lock;
static char lcd_pwr_state;
//static unsigned int lcd_bl_level;


/*****************************************************************************
*
* External Functions
*
******************************************************************************/
extern void lcdc_initialize(struct lcd_panel *lcd_spec);

<<<<<<< HEAD
static void sep0611fb_set_lcdc_info(int clkrate)
{
	unsigned int lcdcbcr = 0;
	unsigned int pcd = clkrate / FRESH_RATE / SEP0611_LCD_WIDTH / SEP0611_LCD_HEIGHT;
	unsigned int reg_pcd;

#ifdef CONFIG_FB_SEP0611_LAYER0_BPP16
	lcdcbcr |= BPIX_BASE(bpp16);
	sep0611_lcdc_info.layer_bpp[0] = 16;
#elif defined(CONFIG_FB_SEP0611_LAYER0_BPP32)
	lcdcbcr |= BPIX_BASE(bpp32);
	sep0611_lcdc_info.layer_bpp[0] = 32;
#endif

#ifdef CONFIG_FB_SEP0611_LAYER1_BPP16
	lcdcbcr |= BPIX_OVLAY1(bpp16);
	sep0611_lcdc_info.layer_bpp[1] = 16;
#elif defined(CONFIG_FB_SEP0611_LAYER1_BPP32)
	lcdcbcr |= BPIX_OVLAY1(bpp32);
	sep0611_lcdc_info.layer_bpp[1] = 32;
#elif defined(CONFIG_FB_SEP0611_LAYER1_YUV444)
	lcdcbcr = lcdcbcr | YUV_format_OVERLAY1(YUV444) | RGB_or_YUV(YUV);
	sep0611_lcdc_info.layer_bpp[1] = 24;
#elif defined(CONFIG_FB_SEP0611_LAYER1_YUV422)
	lcdcbcr = lcdcbcr | YUV_format_OVERLAY1(YUV422) | RGB_or_YUV(YUV);
	sep0611_lcdc_info.layer_bpp[1] = 16;
#elif defined(CONFIG_FB_SEP0611_LAYER1_YUV420)
	lcdcbcr = lcdcbcr | YUV_format_OVERLAY1(YUV420) | RGB_or_YUV(YUV);
	sep0611_lcdc_info.layer_bpp[1] = 12;
#elif defined(CONFIG_FB_SEP0611_LAYER1_YUV420_SEMI)
	lcdcbcr = lcdcbcr | YUV_format_OVERLAY1(YUV420_semi) | RGB_or_YUV(YUV);
	sep0611_lcdc_info.layer_bpp[1] = 12;
#endif

#ifdef CONFIG_FB_SEP0611_LAYER2_BPP16
	lcdcbcr = lcdcbcr | BPIX_OVLAY2(bpp16);
	sep0611_lcdc_info.layer_bpp[2] = 16;
#elif defined(CONFIG_FB_SEP0611_LAYER2_BPP32)
	lcdcbcr = lcdcbcr | BPIX_OVLAY2(bpp32);
	sep0611_lcdc_info.layer_bpp[2] = 32;
#endif

#ifdef CONFIG_FB_SEP0611_LAYER3_BPP1_A
	lcdcbcr |= BPIX_CURSOR(bpp1_a);
	sep0611_lcdc_info.layer_bpp[3] = 1;
#elif defined(CONFIG_FB_SEP0611_LAYER3_BPP1_B)
	lcdcbcr |= BPIX_CURSOR(bpp1_b);
	sep0611_lcdc_info.layer_bpp[3] = 1;
#elif defined(CONFIG_FB_SEP0611_LAYER3_BPP2_A)
	lcdcbcr |= BPIX_CURSOR(bpp2_a);
	sep0611_lcdc_info.layer_bpp[3] = 2;
#elif defined(CONFIG_FB_SEP0611_LAYER3_BPP2_B)
	lcdcbcr |= BPIX_CURSOR(bpp2_b);
	sep0611_lcdc_info.layer_bpp[3] = 2;
#endif
	
	sep0611_lcdc_info.xoffset = 0;
	sep0611_lcdc_info.yoffset = 0;

	reg_pcd = pcd & 0x1 ? pcd - 1: pcd -2;
	lcdcbcr |= PIXPOL(_800_480_PIXPOL) | FLMPOL(_800_480_FLMPOL) | LPPOL(_800_480_LPPOL) | \
			CLKPOL(_800_480_CLKPOL) | OEPOL(_800_480_OEPOL) | PCD(reg_pcd) | UNDER_ERROR_MODE(UNDER_WAIT);

	sep0611_lcdc_info.regs.lcdcbcr 	= lcdcbcr;
	sep0611_lcdc_info.regs.plcr    	= H_WIDTH(_800_480_H_WIDTH) | H_WAIT1(_800_480_H_WAIT1) | H_WAIT2(_800_480_H_WAIT2);
	sep0611_lcdc_info.regs.pfcr    	= V_WIDTH(_800_480_V_WIDTH) | V_WAIT1(_800_480_V_WAIT1) | V_WAIT2(_800_480_V_WAIT2);
	sep0611_lcdc_info.regs.pccr    	= SCR(_800_480_SCR) | CC_EN(_800_480_CC_EN) | PW(_800_480_PW);

	sep0611_lcdc_info.width  	= SEP0611_LCD_WIDTH;
	sep0611_lcdc_info.height 	= SEP0611_LCD_HEIGHT;
	sep0611_lcdc_info.xres   	= SEP0611_LCD_WIDTH;
	sep0611_lcdc_info.yres   	= SEP0611_LCD_HEIGHT;

#if defined(CONFIG_FB_SEP0611_DOUBLE_BUFFER)
	sep0611_lcdc_info.xres_virtual 	= SEP0611_LCD_WIDTH;
	sep0611_lcdc_info.yres_virtual 	= SEP0611_LCD_HEIGHT*2;
#else
	sep0611_lcdc_info.xres_virtual 	 = SEP0611_LCD_WIDTH;
	sep0611_lcdc_info.yres_virtual	 = SEP0611_LCD_HEIGHT;
#endif 
	
	sep0611_lcdc_info.osd_width  	 = SEP0611_LCD_WIDTH;
	sep0611_lcdc_info.osd_height 	 = SEP0611_LCD_HEIGHT;
	sep0611_lcdc_info.osd_xres   	 = SEP0611_LCD_WIDTH;
	sep0611_lcdc_info.osd_yres   	 = SEP0611_LCD_HEIGHT;

	// overlay needn't addition virtual in common, so, here it is the same of xres/yres.
	sep0611_lcdc_info.osd_xres_virtual = SEP0611_LCD_WIDTH;
	sep0611_lcdc_info.osd_yres_virtual = SEP0611_LCD_HEIGHT;

	// 2010-11-30
	sep0611_lcdc_info.regs.cc0r = 0x000000;	// black
	sep0611_lcdc_info.regs.cc1r = 0xffffff;	// white
	sep0611_lcdc_info.regs.cc2r = 0xff0000;	// red
	sep0611_lcdc_info.regs.cc3r = 0x00ff00;	// green or transparent

	// base and overlay positon
	sep0611_lcdc_info.regs.lcdcscr = XMAX(SEP0611_LCD_WIDTH)|YMAX(SEP0611_LCD_HEIGHT);
	sep0611_lcdc_info.regs.btpcr   = LEFT_TOP(0,0);
	sep0611_lcdc_info.regs.bbpcr   = RIGHT_BOTTOM(SEP0611_LCD_WIDTH,SEP0611_LCD_HEIGHT);
	sep0611_lcdc_info.regs.w1tpcr  = LEFT_TOP(0,0);
	sep0611_lcdc_info.regs.w1bpcr  = RIGHT_BOTTOM(SEP0611_LCD_WIDTH,SEP0611_LCD_HEIGHT);
	sep0611_lcdc_info.regs.w2tpcr  = LEFT_TOP(0,0);
	sep0611_lcdc_info.regs.w2bpcr  = RIGHT_BOTTOM(SEP0611_LCD_WIDTH,SEP0611_LCD_HEIGHT);
	sep0611_lcdc_info.regs.ctpcr   = LEFT_TOP(0,0);
	sep0611_lcdc_info.regs.cbpcr   = RIGHT_BOTTOM(SEP0611_LCD_WIDTH,SEP0611_LCD_HEIGHT);

	sep0611_lcdc_info.pixclock     = KHZ2PICOS(clkrate / 1000 / pcd);
	sep0611_lcdc_info.left_margin  = _800_480_H_WAIT1;
	sep0611_lcdc_info.right_margin = _800_480_H_WAIT2;
	sep0611_lcdc_info.hsync_len    = _800_480_H_WIDTH;
	sep0611_lcdc_info.upper_margin = _800_480_V_WAIT1;
	sep0611_lcdc_info.lower_margin = _800_480_V_WAIT2;
	sep0611_lcdc_info.vsync_len    = _800_480_V_WIDTH;
=======
static int hsd050_panel_init(struct lcd_panel *panel)
{
	printk("%s : %d\n", __func__, 0);
	return 0;
>>>>>>> master
}

static int hsd050_set_power(struct lcd_panel *panel, int on)
{
	if(1 == on)
	{
		
		writel(readl(GPIO_PORTI_SEL_V) | GPIO_LCDENABLE_BIT, GPIO_PORTI_SEL_V ); 		/*sel*/
		writel(readl(GPIO_PORTI_DIR_V) & (~GPIO_LCDENABLE_BIT), GPIO_PORTI_DIR_V); 	/*dir*/
		writel(readl(GPIO_PORTI_DATA_V) &(~ GPIO_LCDENABLE_BIT), GPIO_PORTI_DATA_V); 	/*data*/
//lcd PF special function
		writel(0 , GPIO_PORTF_SEL_V); 		/*sel*/
		
		lcdc_initialize(panel);
	}
	else
	{
		writel(readl(GPIO_PORTI_DATA_V ) | GPIO_LCDENABLE_BIT, GPIO_PORTI_DATA_V ); 		/*data*/
		writel(readl(GPIO_PORTI_DATA_V ) & (~GPIO_BACKLIGHT_BIT), GPIO_PORTI_DATA_V); 	/*data*/
	}
	return 0;
}

//ajust pwm
static int hsd050_set_backlight_level(struct lcd_panel *panel,int value)
{
	int pccr=0;	
	
	value = 180 - value*2/3;
	pccr = SCR(_800_480_SCR) | CC_EN(_800_480_CC_EN) | PW(value);
	writel(pccr, LCDC_PCCR_V);	

	return 0;
}

static int hsd050_set_backlight_power(struct lcd_panel *panel, int on)
{
<<<<<<< HEAD
	printk(KERN_INFO "LCD HSD050GT09(800*480)  will be initialized\n");
	sep0611fb_set_lcdc_info(fbi->clkrate);
	sep0611_enable_backlight(1);

	writel(WIN_DISABLE, fbi->io + SEP0611_LCDCECR);
	writel(WIN_DISABLE, fbi->io + SEP0611_BECR);
	writel(WIN_DISABLE, fbi->io + SEP0611_W1ECR);
	writel(WIN_DISABLE, fbi->io + SEP0611_W2ECR);
	writel(WIN_DISABLE, fbi->io + SEP0611_CECR);

	writel(sep0611_lcdc_info.regs.lcdcbcr, fbi->io + SEP0611_LCDCBCR);
	writel(sep0611_lcdc_info.regs.plcr, fbi->io + SEP0611_PLCR);
	writel(sep0611_lcdc_info.regs.pfcr, fbi->io + SEP0611_PFCR);
	writel(sep0611_lcdc_info.regs.pccr, fbi->io + SEP0611_PCCR);
	writel(sep0611_lcdc_info.regs.lcdcscr, fbi->io + SEP0611_LCDCSCR);

	writel(sep0611_lcdc_info.regs.cc0r, fbi->io + SEP0611_CC0R);
	writel(sep0611_lcdc_info.regs.cc1r, fbi->io + SEP0611_CC1R);
	writel(sep0611_lcdc_info.regs.cc2r, fbi->io + SEP0611_CC2R);
	writel(sep0611_lcdc_info.regs.cc3r, fbi->io + SEP0611_CC3R);

	//printk("sep0611fb_init_hw sep0611_lcdc_info.regs.lcdcbcr=0x%08x***********\n",sep0611_lcdc_info.regs.lcdcbcr); //debug
	//printk("sep0611fb_init_hw SEP0611_LCDCBCR=0x%08x***********\n",readl(fbi->io + SEP0611_LCDCBCR)); //debug
=======
	if(1 == on)
	{
		
		writel(readl(GPIO_PORTI_SEL_V ) | GPIO_BACKLIGHT_BIT, GPIO_PORTI_SEL_V); 		/*sel*/
		writel(readl(GPIO_PORTI_DIR_V ) & (~GPIO_BACKLIGHT_BIT), GPIO_PORTI_DIR_V); 	/*dir*/
		writel(readl(GPIO_PORTI_DATA_V ) | GPIO_BACKLIGHT_BIT, GPIO_PORTI_DATA_V); 		/*data*/
	}
	else
	{
		writel(readl(GPIO_PORTI_DATA_V) & (~GPIO_BACKLIGHT_BIT), GPIO_PORTI_DATA_V); 	/*data*/
	}


	return 0;
>>>>>>> master
}

static struct lcd_panel hsd050_panel = {

	.name		= "HSD050",
	.manufacturer	= "NONE",
	.id		= PANEL_ID_HSD050,
	.xres		= 800,
	.yres		= 480,
	.width		= 154,
	.height		= 85,
	.bpp		= 24,
	.pixpol     = 0,
	.flmpol     = 1,
	.lppol      = 1,
	.clkpol     = 0,
	.oepol      = 0,
	.scr        = 1,
	.ccen       = 1,
	.pw         = 30,
	.clk_freq	= 333000,
	.clk_div	= 4,
	.bus_width	= 24,
	.lpw		= 30,
	.lpc		= 799,
	.lswc		= 18,
	.lewc		= 19,
	.fpw1		= 1,
	.flc1		= 479,
	.fswc1		= 5,
	.fewc1		= 11,
	.init		= hsd050_panel_init,
	.set_power	= hsd050_set_power,
	.set_backlight_level = hsd050_set_backlight_level,
	.set_backlight_power = hsd050_set_backlight_power,
};

static int hsd050_probe(struct platform_device *pdev)
{
	printk("%s : %d\n", __func__, 0);

	mutex_init(&panel_lock);
	lcd_pwr_state = 1;

	hsd050_panel.dev = &pdev->dev;
	
	sepfb_register_panel(&hsd050_panel);
	
	//enable lcdc
	hsd050_set_power(&hsd050_panel, 1);
	
	//enable backlight
	hsd050_set_backlight_power(&hsd050_panel, 1);
	
	return 0;
}

static int hsd050_remove(struct platform_device *pdev)
{
	return 0;
}

static struct platform_driver hsd050_lcd = {
	.probe	= hsd050_probe,
	.remove	= hsd050_remove,
	.driver	= {
		.name	= "hsd050_lcd",
		.owner	= THIS_MODULE,
	},
};

static __init int hsd050_init(void)
{
	printk("~ %s ~ \n", __func__);
	return platform_driver_register(&hsd050_lcd);
}

static __exit void hsd050_exit(void)
{
	platform_driver_unregister(&hsd050_lcd);
}

subsys_initcall(hsd050_init);
module_exit(hsd050_exit);

MODULE_DESCRIPTION("HSD050 LCD driver");
MODULE_LICENSE("GPL");
