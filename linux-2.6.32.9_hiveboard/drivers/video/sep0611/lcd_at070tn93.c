#include <linux/delay.h>
#include <mach/hardware.h>
#include <board/board.h>

#include "sep0611_fb.h"

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
#define		_800_480_H_WIDTH	1
#define		_800_480_H_WAIT1	45
#define		_800_480_H_WAIT2	45

/* macro for Panel frame control register(LCDC_PFCR) */
#define		_800_480_V_WIDTH	1
#define		_800_480_V_WAIT1	22
#define		_800_480_V_WAIT2	22

/* macro for Panel contrast control register(LCDC_PCCR) */
#define		_800_480_SCR		1
#define		_800_480_CC_EN		1
#define		_800_480_PW			0

struct sep0611_lcdc_info_t sep0611_lcdc_info;	// sep0611_lcdc configure information


/*********************************************
Function:    sep0611fb_set_lcdc_info
Description: set sep0611_lcd infromation including bpp value in
	     each overlay
Parameter:   void
**********************************************/

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
#elif defined(CONFIG_FB_SEP0611_LAYER1_BPP18)
	lcdcbcr |= BPIX_OVLAY1(bpp18_unpacked);
	sep0611_lcdc_info.layer_bpp[1] = 18;
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

	reg_pcd = pcd & 0x1 ? pcd - 1 : pcd - 2;
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

//	sep0611_lcdc_info.pixclock     = KHZ2PICOS(240 * 1000 / (_800_480_PCD + 2));
	sep0611_lcdc_info.pixclock     = KHZ2PICOS(clkrate / 1000 / pcd);
	sep0611_lcdc_info.left_margin  = _800_480_H_WAIT1;
	sep0611_lcdc_info.right_margin = _800_480_H_WAIT2;
	sep0611_lcdc_info.hsync_len    = _800_480_H_WIDTH;
	sep0611_lcdc_info.upper_margin = _800_480_V_WAIT1;
	sep0611_lcdc_info.lower_margin = _800_480_V_WAIT2;
	sep0611_lcdc_info.vsync_len    = _800_480_V_WIDTH;
}

/*******************************
Function:    sep0611_enable_backlight
Description: control backlight on/off
Parameter:   enable:
	     	1 open backlight
	     	0 close backlight
********************************/
void sep0611_enable_backlight( int enable )
{
	if ( enable == 1 ) {
#ifdef SEP0611_LCD_EN
    sep0611_gpio_cfgpin(SEP0611_LCD_EN, SEP0611_GPIO_IO);   /* GPIO */
    sep0611_gpio_dirpin(SEP0611_LCD_EN, SEP0611_GPIO_OUT);  /* output */
    sep0611_gpio_setpin(SEP0611_LCD_EN, GPIO_HIGH);
#endif
#ifdef SEP0611_LCD_MODE
    sep0611_gpio_cfgpin(SEP0611_LCD_MODE, SEP0611_GPIO_IO);   /* GPIO */
    sep0611_gpio_dirpin(SEP0611_LCD_MODE, SEP0611_GPIO_OUT);  /* output */
    sep0611_gpio_setpin(SEP0611_LCD_MODE, GPIO_LOW);
#endif
#ifdef SEP0611_LCD_RST
    sep0611_gpio_cfgpin(SEP0611_LCD_RST, SEP0611_GPIO_IO);   /* GPIO */
    sep0611_gpio_dirpin(SEP0611_LCD_RST, SEP0611_GPIO_OUT);  /* output */
    sep0611_gpio_setpin(SEP0611_LCD_RST, GPIO_LOW);
    msleep(10);
    sep0611_gpio_setpin(SEP0611_LCD_RST, GPIO_HIGH);
#endif
	msleep(50);
#ifdef SEP0611_LCD_BL_EN
    sep0611_gpio_cfgpin(SEP0611_LCD_BL_EN, SEP0611_GPIO_IO);   /* GPIO */
    sep0611_gpio_dirpin(SEP0611_LCD_BL_EN, SEP0611_GPIO_OUT);  /* output */
    sep0611_gpio_setpin(SEP0611_LCD_BL_EN, GPIO_HIGH);
#endif
	}

 	if ( enable == 0 ) {
#ifdef SEP0611_LCD_BL_EN
    sep0611_gpio_cfgpin(SEP0611_LCD_BL_EN, SEP0611_GPIO_IO);   /* GPIO */
    sep0611_gpio_dirpin(SEP0611_LCD_BL_EN, SEP0611_GPIO_OUT);  /* output */
    sep0611_gpio_setpin(SEP0611_LCD_BL_EN, GPIO_LOW);
#endif
#ifdef SEP0611_LCD_EN
    sep0611_gpio_setpin(SEP0611_LCD_EN, GPIO_LOW);
#endif
	}
}


/*******************************
Function:    sep0611fb_init_hw
Description: init sep0611-lcd hardware
Parameter:   fbi:
	     struct sep0611fb_info 
********************************/
void sep0611fb_init_hw( struct sep0611fb_info *fbi )
{
	printk(KERN_INFO "LCD AT070TN93  will be initialized\n");
	
	sep0611fb_set_lcdc_info(fbi->clkrate);
//	sep0611_enable_backlight(1);

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

}

//ajust pwm
int sep0611fb_set_backlight_level(int value)
{
	int pccr=0;	
	
	value = 180 - value*2/3;
	
	pccr = SCR(_800_480_SCR) | CC_EN(_800_480_CC_EN) | PW(value);
	writel(pccr, LCDC_PCCR_V);	

	return 0;
}
