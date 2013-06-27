/****************************************************************************
 * drivers/video/sep0611/sep0611_fb.c
 *
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 *
 *	SEP0611 Frame Buffer Driver
 * change log:
 * 2010-04-27 inital version by ZLM
 * 2010-11-19 second version by ZM
 * 2011-1-18 baseline version by ZM
 * 2011-5-11 tigertest version by GGJ
 ****************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/tty.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/console.h>
/**************comment by zyt**************
*	#include <linux/earlysuspend.h>
**************comment by zyt**************/
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/div64.h>

#include <mach/hardware.h>
#include <mach/regs-lcd.h>

#include "sep0611_fb.h"
#include "sep0611_fb_hdmi.h"

#ifdef CONFIG_ARCH_SEP0611
#include <linux/clk.h>
#endif

#ifdef CONFIG_CPU_FREQ
#include <linux/cpufreq.h>
#endif
#ifdef CONFIG_PM
#include <linux/pm.h>
#endif

enum layers_select_t layers_select;

struct layer1_yuv_t layer1_yuv;		//yuv struct including yuv configure

static void resume_work_func(struct work_struct *work);
static DECLARE_DELAYED_WORK(work, resume_work_func);
static struct workqueue_struct *resume_wq;


#define SEP_FB_MIN(a,b) ((a)<(b)?(a):(b))
#define SEP0611_MAX_NUM 4
#define SEP0611FB_NUM SEP_FB_MIN(SEP0611_MAX_NUM, CONFIG_FB_SEP0611_NUM)

#ifdef	CONFIG_FB_SEP0611_LCD_AT070TN93
	#define SEP0611_XRES_MAX 800
	#define SEP0611_YRES_MAX 480
#endif

#ifdef	CONFIG_FB_SEP0611_LCD_AT070TN90
	#define SEP0611_XRES_MAX 800
	#define SEP0611_YRES_MAX 480
#endif

#ifdef	CONFIG_FB_SEP0611_LCD_HSD050
	#define SEP0611_XRES_MAX 800
	#define SEP0611_YRES_MAX 480
#endif


const static sep0611fb_rgb_t sep0611fb_rgb_12 = {
	.red    = {.offset = 8,  .length = 4,},
	.green  = {.offset = 4,  .length = 4,},
	.blue   = {.offset = 0,  .length = 4,},
	.transp = {.offset = 0,  .length = 0,},
};

const static sep0611fb_rgb_t sep0611fb_rgb_16 = {
	.red    = {.offset = 11, .length = 5,},
	.green  = {.offset = 5,  .length = 6,},
	.blue   = {.offset = 0,  .length = 5,},
	.transp = {.offset = 0,  .length = 0,},
};

const static sep0611fb_rgb_t sep0611fb_rgb_18 = {
	.red    = {.offset = 12, .length = 6,},
	.green  = {.offset = 6,  .length = 6,},
	.blue   = {.offset = 0,  .length = 6,},
	.transp = {.offset = 0,  .length = 0,},
};

const static sep0611fb_rgb_t sep0611fb_rgb_24 = {
	.red    = {.offset = 16, .length = 8,},
	.green  = {.offset = 8,  .length = 8,},
	.blue   = {.offset = 0,  .length = 8,},
	.transp = {.offset = 24, .length = 8,},
};

const static sep0611fb_rgb_t sep0611fb_rgb_32 = {
	.red    = {.offset = 16, .length = 8,},
	.green  = {.offset = 8,  .length = 8,},
	.blue   = {.offset = 0,  .length = 8,},
	.transp = {.offset = 24, .length = 8,},
};

static inline int sep0611fb_layer_bpp_validate(unsigned int layer_num, unsigned int bpp)
{
	if ((bpp == 16 || bpp == 32) && (layer_num != layer3))
		return 0;
	if (( bpp == 24 || bpp == 12) && (layer_num == layer1))
		return 0;
	if ((bpp == 1 || bpp == 2) && layer_num == layer3)
		return 0;
	return -1;
}


/*********************************************
Function:    sep0611fb_set_alpha_mode
Description: set  alpha mode only in overlay1
Parameter:   info:
		struct fb_info
	     alpha_mode:
		only in 0 or 1
		0 user mode
		1 argb mode
**********************************************/
static int sep0611fb_set_alpha_mode( struct fb_info *info, unsigned int alpha_mode )
{
	struct sep0611fb_layer *layers = info->par;
	struct sep0611fb_info *fbi     = layers->parent;
	unsigned int layer_num         = layers->layer_id;
	unsigned int tmp;

	if ( (layer_num != layer1) && (layer_num != layer2) ) {
		printk("ERROR: only layer1 and layer2 have alpha blending\n");
		return -EINVAL;
	}

	layers->alpha_mode = alpha_mode;
	tmp = readl( fbi->io + SEP0611_W1BVR + (layer_num - 1) * 4 );
	if( alpha_mode == ALPHA_USR )
		writel(tmp & 0xffffff, fbi->io + SEP0611_W1BVR + (layer_num - 1) * 4); 		/*set bit24 0*/
		else if (alpha_mode == ALPHA_ARGB)
			writel(tmp | 0x1000000, fbi->io + SEP0611_W1BVR + (layer_num - 1) * 4); /*set bit24 1*/
			else return -EINVAL;

	return 0;
}

/*********************************************
Function:    sep0611fb_set_alpha_usr_value
Description: set  alpha usr value only in overlay1
Parameter:   info:
		struct fb_info
	     alpha_level:
		should be in range 0~15
**********************************************/
static int sep0611fb_set_alpha_usr_value(struct fb_info *info, unsigned int alpha_level)
{
	struct sep0611fb_layer *layers 	= info->par;
	struct sep0611fb_info *fbi 	= layers->parent;
	unsigned int layer_num 		= layers->layer_id;
	unsigned int tmp;

	if ((layer_num != layer1) && (layer_num != layer2)) {
		printk("ERROR: only layer1 and layer2 have alpha blending\n");
		return -EINVAL;
	}

	if ((readl( fbi->io + SEP0611_W1BVR + (layer_num - 1) * 4) & 0x1000000) != 0) {
		printk("ERROR: choose a wrong alpha mode\n");
		return -EINVAL;
	}

	if(alpha_level > SEP0611_MAX_ALPHA_LEVEL)
		alpha_level = SEP0611_MAX_ALPHA_LEVEL;

	layers->alpha_level = alpha_level;
	
	tmp = readl(fbi->io + SEP0611_W1BVR+(layer_num - 1) * 4) & (~0x0f);
	writel(alpha_level | tmp, fbi->io + SEP0611_W1BVR+(layer_num - 1) * 4);

	return 0;
}


/*********************************************
Function:    sep0611fb_set_alpha_argb_value
Description: set  alpha argb value only in overlay1
Parameter:   info:
		struct fb_info
	     argb_value:
		should be less than 8
		bit16:ARGB_SAMPLE bit9-8:ARGB_FORMAT
		argb_value:ARGB_SAMPLE | ARGB_FORMAT
**********************************************/
static int sep0611fb_set_alpha_argb_value(struct fb_info *info, unsigned int argb_value)
{
	struct sep0611fb_layer *layers 	= info->par;
	struct sep0611fb_info *fbi 	= layers->parent;
	unsigned int layer_num 		= layers->layer_id;
	unsigned int tmp;

	if ((layer_num != layer1) && (layer_num != layer2)) {
		printk("ERROR: only layer1 and layer2 have alpha blending\n");
		return -EINVAL;
	}

	if ((readl( fbi->io + SEP0611_W1BVR + (layer_num - 1) * 4) & 0x1000000) == 0) {
		printk("ERROR: choose a wrong alpha mode\n");
		return -EINVAL;
	}

	if (argb_value > 7) {
		printk("ERROR: choose a wrong argb value");
		return -EINVAL;
	}

	layers->argb_value = argb_value;

	tmp = readl(fbi->io + SEP0611_W1BVR+(layer_num - 1) * 4) & (~0x10300);
	writel(((argb_value & 3) << 8) | ((argb_value & 4) << 16) | tmp, fbi->io + SEP0611_W1BVR+(layer_num - 1) * 4);		//2011-5-11

	return 0;
}
/*********************************************
Function:    sep0611fb_colorkey_on_off
Description: contol colorkey on/off
Parameter:   info:
		struct fb_info
	     on_off:
		only 0 or 1
**********************************************/
static int sep0611fb_colorkey_on_off(struct fb_info *info, unsigned int on_off)
{
	struct sep0611fb_layer *layers 	= info->par;
	struct sep0611fb_info *fbi 	= layers->parent;
	unsigned int layer_num 		= layers->layer_id;
	unsigned int tmp;

	if ((layer_num != layer1) && (layer_num != layer2)) {
		printk("ERROR: only layer1 and layer2 have color key\n");
		return -EINVAL;
	}

	layers->colorkey_enable = on_off;

	tmp = readl(fbi->io + SEP0611_W1CKR + (layer_num - 1) * 4) & (~(1 << 24));
	writel(tmp | (on_off << 24), fbi->io + SEP0611_W1CKR+(layer_num - 1) * 4);

	return 0;
}


static int sep0611fb_set_colorkey_vaule(struct fb_info *info, unsigned int colorkey_value)
{
	struct sep0611fb_layer *layers 	= info->par;
	struct sep0611fb_info *fbi 	= layers->parent;
	unsigned int layer_num 		= layers->layer_id;
	unsigned int tmp;

	if ((layer_num != layer1) && (layer_num != layer2)) {
		printk("ERROR: only layer1 and layer2 have color key\n");
		return -EINVAL;
	}

	tmp = readl(fbi->io + SEP0611_W1CKR + (layer_num - 1) * 4) & (~0xffffff);
	writel(tmp | colorkey_value, fbi->io + SEP0611_W1CKR+(layer_num - 1) * 4);

	return 0;
}

static int sep0611fb_set_cursor_mode(struct fb_info *info, unsigned int mode)
{
	struct sep0611fb_layer *layers = info->par;
	struct sep0611fb_info *fbi = layers->parent;
	unsigned int layer_num = layers->layer_id;
	unsigned int tmp;

	if(layer_num != layer3) {
		printk("ERROR: only layer3 has cursor mode\n");
		return -EINVAL;
	}

	layers->cursor_mode = mode;

	tmp = readl(fbi->io + SEP0611_LCDCBCR) && ~(3 << 21);
	tmp |= mode << 21;
	writel(tmp, fbi->io + SEP0611_LCDCBCR);

	return 0;
}

static int sep0611fb_set_cursor_pal(struct fb_info *info, unsigned int * pal)
{
	struct sep0611fb_layer *layers = info->par;
	struct sep0611fb_info *fbi = layers->parent;
	unsigned int layer_num = layers->layer_id;

	if (layer_num != layer3) {
		printk("ERROR: only layer3 has cursor pal\n");
		return -EINVAL;
	}
	
	if (layers->cursor_mode < 2 ) {
		layers->palette_buffer[0] = *pal;
		layers->palette_buffer[1] = *(pal + 1);
		writel(layers->palette_buffer[0], fbi->io + SEP0611_CC0R);
		writel(layers->palette_buffer[1], fbi->io + SEP0611_CC1R);
	} else {
		layers->palette_buffer[0] = *pal;
		layers->palette_buffer[1] = *(pal + 1);
		layers->palette_buffer[2] = *(pal + 2);
		layers->palette_buffer[3] = *(pal + 3);
		writel(layers->palette_buffer[0], fbi->io + SEP0611_CC0R);
		writel(layers->palette_buffer[1], fbi->io + SEP0611_CC1R);
		writel(layers->palette_buffer[2], fbi->io + SEP0611_CC2R);
		writel(layers->palette_buffer[3], fbi->io + SEP0611_CC3R);
		}

	return 0;
}

/*********************************************
Function:    sep0611fb_yuv_address_configure
Description: cal yuv address
Parameter:   layer:
		struct sep0611fb_layer
**********************************************/

static void sep0611fb_yuv_address_configure(struct sep0611fb_layer *layer)
{
	struct sep0611fb_info *fbi 	= layer->parent;
	struct fb_var_screeninfo *var 	= &layer->fb->var;
	unsigned int layer_num 		= layer->layer_id;
	unsigned int lcdcbcr;
	unsigned int yuv_enable, yuv_mode;
	unsigned int total_size,y_size,u_size,v_size;

	lcdcbcr = readl(fbi->io + SEP0611_LCDCBCR);
	yuv_enable = (lcdcbcr >> 31) & 0x1;
	yuv_mode = (lcdcbcr >> 29) & 0x3;

	if ((layer_num == layer1) && yuv_enable) {

		y_size = var->xres * var->yres;		// CARE!!!  2011-1-18

		switch (yuv_mode) {
		case YUV444:
			u_size = v_size = y_size;
			break;
		case YUV422:
			u_size = v_size = (y_size >> 1);
			break;
		case YUV420:
		case YUV420_semi:
			u_size = v_size = (y_size >> 2);
			break;
		}
		total_size = y_size + u_size + v_size;

		layer1_yuv.yuv_format 	= yuv_mode;
		layer1_yuv.y_size 	= y_size;
		layer1_yuv.u_size 	= u_size;
		layer1_yuv.v_size 	= v_size;
	
	}
	
}

/*********************************************
Function:    sep0611fb_activate_var
Description: set var_screeninfo
Parameter:   layer:
		struct sep0611fb_layer
	     var:
		struct fb_var_screeninfo
**********************************************/
static void sep0611fb_activate_var(struct sep0611fb_layer *layer, struct fb_var_screeninfo *var)
{
	struct sep0611fb_info *fbi = layer->parent;
	unsigned int layer_num = layer->layer_id; 
	unsigned int lcdcbcr_tmp = readl(fbi->io+SEP0611_LCDCBCR);
	unsigned int start_addr;
	unsigned int yuv_enable = var->nonstd & 0x1;
	unsigned int yuv_mode = (var->nonstd >> 1) & 0x3;

	//printk("sep0611fb_activate_var lcdcbcr_tmp = 0x%08x***\n",lcdcbcr_tmp); //debug
	switch (layer_num) {
	case 0:
		lcdcbcr_tmp &= ~BPIX_BASE(3);
		switch (var->bits_per_pixel) {
		case 16:
			lcdcbcr_tmp |= BPIX_BASE(bpp16);
			break;
		case 32:
			lcdcbcr_tmp |= BPIX_BASE(bpp32);
			break;					
		}
		break; 
	case 1:
		lcdcbcr_tmp &= ~(BPIX_OVLAY1(3) | YUV_format_OVERLAY1(3));
		switch (var->bits_per_pixel) {
		case 12:
			if (!yuv_enable)
				lcdcbcr_tmp |= BPIX_OVLAY1(bpp16);
			else if (yuv_mode == 0) /*YUV420*/
				lcdcbcr_tmp |= YUV_format_OVERLAY1(YUV420);
			else if (yuv_mode == 3) /*YUV420-semi*/
				lcdcbcr_tmp |= YUV_format_OVERLAY1(YUV420_semi);
			break;
		case 16:
			lcdcbcr_tmp |= (yuv_enable ? YUV_format_OVERLAY1(YUV422) : BPIX_OVLAY1(bpp16));	
			break;
		case 24:
			lcdcbcr_tmp |= (yuv_enable ? YUV_format_OVERLAY1(YUV444) : BPIX_OVLAY1(bpp32));
			break;					
		case 32:
			lcdcbcr_tmp |= BPIX_OVLAY1(bpp32);
			break; 									
		}
		break;
	case 2:
		lcdcbcr_tmp &= ~BPIX_OVLAY2(3);	
		switch (var->bits_per_pixel) {
		case 16:
			lcdcbcr_tmp |= BPIX_OVLAY2(bpp16);
			break;
		case 32:
			lcdcbcr_tmp |= BPIX_OVLAY2(bpp32);
			break;					
		}
		break; 
	case 3:
		lcdcbcr_tmp &= ~BPIX_CURSOR(3);
		switch (var->bits_per_pixel) {
		case 1:
			lcdcbcr_tmp |= BPIX_CURSOR(bpp1_b);
			break;
		case 2:
			lcdcbcr_tmp |= BPIX_CURSOR(bpp2_b);
			break;	
		}
		break;
	default:
		break;	
	}

	writel(lcdcbcr_tmp, fbi->io+SEP0611_LCDCBCR);

	/**********************************************************
	**YUV configuration**
	*set framebuffer start address(use DMA's physical address),
	*since U and V buffer start address varies with xres width.
	* NOTE:only for YUV mode(layer 1)
	* 2011-1-17
	***********************************************************/
	if ((layer_num == layer1) && yuv_enable) {
		sep0611fb_yuv_address_configure(layer);
		start_addr = readl(fbi->io + SEP0611_W1YBSAR);
		writel(0, fbi->io + SEP0611_ACSR);
		writel(start_addr + layer1_yuv.y_size, fbi->io + SEP0611_W1UBSAR);
		writel(start_addr + layer1_yuv.y_size + layer1_yuv.u_size, fbi->io + SEP0611_W1VBSAR);
		writel(1, fbi->io + SEP0611_ACSR);
	}

	/*set the raw image width, except cursor layer */
	if (layer_num < 3)
		writel(var->xres, fbi->io + SEP0611_BASE_RAW_IMAGE_WIDTH + layer_num * 4);

	/*set alpha blending for layer1 and layer2*/
	if (layer_num == layer1 || layer_num == layer2) {
		sep0611fb_set_alpha_mode(layer->fb, layer->alpha_mode);
		if (layer->alpha_mode == ALPHA_USR)
			sep0611fb_set_alpha_usr_value(layer->fb, layer->alpha_level);
		if (layer->alpha_mode == ALPHA_ARGB)
			sep0611fb_set_alpha_argb_value(layer->fb, layer->argb_value);
	}

	/*set colorkey for layer1 and layer2*/
	if (layer_num == layer1 || layer_num == layer2) {
		sep0611fb_colorkey_on_off(layer->fb, layer->colorkey_enable);
		sep0611fb_set_colorkey_vaule(layer->fb, layer->colorkey_value);
	}

}


static void sep0611fb_set_fb_addr(struct sep0611fb_info *fbi, int layer_no, unsigned int offset)
{
	struct sep0611fb_layer *layers = fbi->sep_layers[layer_no];
	unsigned int video_phy_temp = layers->fb->fix.smem_start;
	unsigned int start_addr;
	unsigned int yuv_enable;

	start_addr = video_phy_temp + offset;
	yuv_enable = readl(fbi->io + SEP0611_LCDCBCR) & (1 << 31);

	writel(0, fbi->io + SEP0611_ACSR);

	if (layer_no < 2)
		writel(start_addr, fbi->io + SEP0611_BBSAR + layer_no * 4);
	else if (layer_no < 4)
			writel(start_addr, fbi->io + SEP0611_W2BSAR + (layer_no - 2) * 4);

	// YUV mode double buffer
	if ((layer_no == layer1) && yuv_enable) {
		writel(start_addr + layer1_yuv.y_size, fbi->io + SEP0611_W1UBSAR);
		writel(start_addr + layer1_yuv.y_size + layer1_yuv.u_size, fbi->io + SEP0611_W1VBSAR);
	}
		
	writel(1, fbi->io + SEP0611_ACSR);

}


static int sep0611fb_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
	struct sep0611fb_layer *layers 	= info->par;
	struct sep0611fb_info *fbi 	= layers->parent;

	if (sep0611fb_layer_bpp_validate(layers->layer_id, var->bits_per_pixel)) {
		dev_err(fbi->dev, "layer[%d]: unsupported bpp %d\n", layers->layer_id,
			var->bits_per_pixel);
		return -EINVAL;
	}

	if(var->xres > SEP0611_XRES_MAX)
		var->xres = SEP0611_XRES_MAX;
	if(var->yres > SEP0611_YRES_MAX)
		var->yres = SEP0611_YRES_MAX;

	if(var->xres > var->xres_virtual)
		var->xres = var->xres_virtual;
	if(var->yres > var->yres_virtual)
		var->yres = var->yres_virtual;

	switch (var->bits_per_pixel) {
		case 16:
			if (!var->nonstd) {
			// RGB565 packed:possess 2 bytes
			var->red 	= sep0611fb_rgb_16.red;
			var->green 	= sep0611fb_rgb_16.green;
			var->blue 	= sep0611fb_rgb_16.blue;
			var->transp 	= sep0611fb_rgb_16.transp;
			}
			break;
		case 32:
			// ARGB8888:possess 4 bytes 
			var->red = sep0611fb_rgb_32.red;
			var->green = sep0611fb_rgb_32.green;
			var->blue = sep0611fb_rgb_32.blue;
			var->transp = sep0611fb_rgb_32.transp;
			break;
		case 1:
			// check the 1bpp mode
			var->red.offset   = 0;
			var->red.length   = 1;
			var->green.offset = 0;
			var->green.length = 1;
			var->blue.offset  = 0;
			var->blue.length  = 1;
			break;
		case 2:
			// check the 2bpp mode 
			var->red.offset   = 0;
			var->red.length   = 2;
			var->green.offset = 0;
			var->green.length = 2;
			var->blue.offset  = 0;
			var->blue.length  = 2;
			break;
		case 24:
		case 12:
			/*YUV444 which equals RGB888, and YUV420(or YUV420-semi) equals RGB444*/
			if ((layers->layer_id == layer1) && var->nonstd) {
				var->red = var->bits_per_pixel == 24 ? sep0611fb_rgb_24.red : sep0611fb_rgb_12.red;
				var->green = var->bits_per_pixel == 24 ? sep0611fb_rgb_24.green : sep0611fb_rgb_12.green;
				var->blue = var->bits_per_pixel == 24 ? sep0611fb_rgb_24.blue : sep0611fb_rgb_12.blue;
				var->transp = var->bits_per_pixel == 24 ? sep0611fb_rgb_24.transp : sep0611fb_rgb_12.transp;
				break;/*else goto default rgb*/
			}
		default:
			printk("unsupport bits_per_pixel by sep0611 lcdc.Now round up to 16bpp!\n");
			var->red 	= sep0611fb_rgb_16.red;
			var->green 	= sep0611fb_rgb_16.green;
			var->blue 	= sep0611fb_rgb_16.blue;
			var->transp 	= sep0611fb_rgb_16.transp;
			break;

	}

	// horizontal timing and vertical timing
	var->pixclock 		= sep0611_lcdc_info.pixclock;
	var->left_margin 	= sep0611_lcdc_info.left_margin;
	var->right_margin 	= sep0611_lcdc_info.right_margin;
	var->upper_margin 	= sep0611_lcdc_info.upper_margin;
	var->lower_margin 	= sep0611_lcdc_info.lower_margin;
	var->hsync_len 		= sep0611_lcdc_info.hsync_len;
	var->vsync_len 		= sep0611_lcdc_info.vsync_len;

	return 0;
}

static void sep0611fb_yuv_calculate_line_length(struct fb_var_screeninfo *var, struct sep0611fb_layer *layer)
{
	struct sep0611fb_info *fbi = layer->parent;
	unsigned int yuv_mode;
	unsigned int yuv_bytes;

	yuv_mode = (readl(fbi->io + SEP0611_LCDCBCR) >> 29) & 0x3;
	switch(yuv_mode) {
	case YUV444:
		yuv_bytes = var->xres_virtual * 3;
		break;
	case YUV422:
		yuv_bytes = var->xres_virtual * 2;
		break;
	case YUV420:
	case YUV420_semi:
		yuv_bytes = var->xres_virtual * 3 / 2;
		break;
	}

	layer->fb->fix.line_length = yuv_bytes;

}

static int sep0611fb_set_par(struct fb_info *info)
{
	struct fb_var_screeninfo *var = &info->var;
	struct sep0611fb_layer *layers = info->par;
	unsigned int layer_num = layers->layer_id;
	struct sep0611fb_info *fbi = layers->parent;
	unsigned int yuv_enable = readl(fbi->io + SEP0611_LCDCBCR) & (1 << 31);


	if (var->bits_per_pixel >= 12)
		info->fix.visual = FB_VISUAL_TRUECOLOR;
	else
		info->fix.visual = FB_VISUAL_PSEUDOCOLOR;

	// YUV frame only supports frame span, so line_length has nonsense for xpan and ypan
	// With both xpan and yan functionalities, we use xres intead of xres_virtual that is used in ypan only

	if ((layer_num == layer1) && yuv_enable)
		sep0611fb_yuv_calculate_line_length(var, layers); 
	else
		info->fix.line_length = var->xres * var->bits_per_pixel >> 3;
	// activate this new configuration 
	sep0611fb_activate_var(layers, var);

	return 0;
}

static int sep0611fb_blank(int blank_mode, struct fb_info *info)
{
	//printk("sep0611fb_blank\n");
	struct sep0611fb_layer *layers = info->par;
	struct sep0611fb_info *fbi = layers->parent;
	unsigned int layer_num = layers->layer_id;
	//printk("ZM in sep0611fb_blank**************\n");
	switch (blank_mode) {

	// set lcd on, turn backlight on 
	case FB_BLANK_UNBLANK:
		sep0611_enable_backlight(1);
		writel(1, fbi->io + SEP0611_LCDCECR);									// 2011-1-17
		writel(1, fbi->io + SEP0611_BECR + 4 * layer_num);
		break;

	/****************************************************** 
	 *NOTE: hardware does NOT support VSYNC or HSYNC suspend,
	 *so set lcd on and turn backlight off
	 *******************************************************/

	case FB_BLANK_VSYNC_SUSPEND:
	case FB_BLANK_HSYNC_SUSPEND:

	case FB_BLANK_NORMAL:
		// sep0611_enable_backlight(0);										// in test we do not turn off
		writel(1, fbi->io + SEP0611_LCDCECR);									// 2011-1-17
		writel(1, fbi->io + SEP0611_BECR + 4 * layer_num);
		break;
		// set lcd off and turn backlight off 
	case FB_BLANK_POWERDOWN:
		sep0611_enable_backlight(0);
		writel(1, fbi->io + SEP0611_LCDCECR);									
		writel(0, fbi->io + SEP0611_BECR + 4 * layer_num);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}


/*********************************************
Function:    sep0611fb_pan_display
Description: pan dispaly only in ypan,this will
		be used in double framebuffer
Parameter:   var:
		struct fb_var_screeninfo,you
		should update this struct
	     info:
		struct fb_info
**********************************************/
static int sep0611fb_pan_display(struct fb_var_screeninfo *var, struct fb_info *info)
{
	//printk("GGJ sep0611fb_pan_display\n");
	struct sep0611fb_layer *layers = info->par;
	struct sep0611fb_info *fbi = layers->parent;
	unsigned int offset;
//--------------add by zyt--------------
	unsigned int data;
//--------------add by zyt--------------
	offset   = (var->yoffset * info->fix.line_length);

	sep0611fb_set_fb_addr(fbi, layers->layer_id, offset);

	/********************** fuzhuo add to FORCE pan_display overlay2 *****/
/**************comment by zyt**************
	*	unsigned int data;
**************comment by zyt**************/
	data=readl(fbi->io + SEP0611_BBSAR);
	//printk(" id=%d, bbstart addr=0x%x\n", layers->layer_id, data);
	writel(0, fbi->io + SEP0611_ACSR);
//	writel(data + 800*392*2, fbi->io + SEP0611_W2BSAR);
	writel(data , fbi->io + SEP0611_W2BSAR);
	writel(1, fbi->io + SEP0611_ACSR);
	/*********************************************************************/
	return 0;
}

static inline unsigned int sep0611fb_chan_to_field(unsigned int chan, struct fb_bitfield *bf)
{
	chan &= 0xffff;
	chan >>= 16 - bf->length;

	return chan << bf->offset;
}

static int sep0611fb_setcolreg(unsigned int regno, unsigned int red, unsigned int green, unsigned int blue, unsigned int transp, struct fb_info *info)
{
	//printk("sep0611fb_setcolreg\n");
	struct sep0611fb_layer *layers 	= info->par;
	unsigned int val;
	int ret = 1;
	/*************************************************
	*if grayscale is true, we convert RGB to grayscale
	*r = g = b = 0.3r + 0.59g + 0.11b
	**************************************************/
	if (info->var.grayscale)
		red = green = blue = (77 * red + 151 * green + 28 * blue) >> 8;

	switch (info->fix.visual) {
	case FB_VISUAL_TRUECOLOR:
		if (regno < 16) {
			unsigned int *pal = info->pseudo_palette;

			val  = sep0611fb_chan_to_field(red, &info->var.red);
			val |= sep0611fb_chan_to_field(green, &info->var.green);
			val |= sep0611fb_chan_to_field(blue, &info->var.blue);

			pal[regno] = val;
		}
		break;

	case FB_VISUAL_PSEUDOCOLOR:
		if (regno < 256) {
			if (layers->fb->var.grayscale) {
				layers->palette_buffer[regno] = ((blue >> 8) & 0x00ff);
				return 0;
			}

			val = ((red >> 0) & 0xf800);
			val |= ((green >> 5) & 0x07e0);
			val |= ((blue >> 11) & 0x001f);

			layers->palette_buffer[regno] = val;
			ret = 0;
		}
		break;
	}

	return ret;
}


/**********************************************sep0611fb_ioctl functions************************************************/

static int sep0611fb_yuv_on_off(struct fb_info *info, unsigned int on_off)
{
	struct sep0611fb_layer *layers 	= info->par;
	struct sep0611fb_info *fbi 	= layers->parent;
	unsigned int layer_num		= layers->layer_id;
	unsigned int lcdcbcr;

	if (layer_num != layer1) {
		printk("ERROR: only layer 1 has YUV display\n");
		return -EINVAL;
	}
	
	layers->yuv_enable 	= on_off;
	layers->fb->var.nonstd 	= on_off;		// 201-1-17

	lcdcbcr 		= readl(fbi->io + SEP0611_LCDCBCR) & ~(1 << 31);
	writel(lcdcbcr | (on_off << 31), fbi->io + SEP0611_LCDCBCR);

	return 0;
}

static int sep0611fb_set_yuv_mode(struct fb_info *info, unsigned int mode)
{
	struct sep0611fb_layer *layers 	= info->par;
	struct sep0611fb_info *fbi 	= layers->parent;
	unsigned int layer_num		= layers->layer_id;
	unsigned int lcdcbcr;

	if (layer_num != layer1) {
		printk("ERROR: only layer 1 has YUV display\n");
		return -EINVAL;
	}

	layers->yuv_mode = mode;
	lcdcbcr = readl(fbi->io + SEP0611_LCDCBCR) & ~(3 << 29);
	writel(lcdcbcr | (mode << 29), fbi->io + SEP0611_LCDCBCR);	

	return 0;
}

static int sep0611fb_get_screen_coordinate(struct fb_info *info, struct layer_coordinate_t *coordinate)
{
	//struct fb_var_screeninfo *var = &info->var;
	struct sep0611fb_layer *layers = info->par;
	struct sep0611fb_info *fbi = layers->parent;
	int layer_num =  layers->layer_id;
	//struct layer_coordinate_t pos;
	unsigned int left_top, right_bottom;

	/*********************************
	todo:fix constraint on resolution
	**********************************/
	//pos.size_x = var->width;
	//pos.size_y = var->height;

	if (layer_num == layer0) {
		left_top = readl(fbi->io + SEP0611_BTPCR);
		right_bottom = readl(fbi->io + SEP0611_BBPCR);
	} else {
			left_top = readl(fbi->io + SEP0611_W1TPCR + (layer_num - 1) * 8);
			right_bottom = readl(fbi->io + SEP0611_W1BPCR + (layer_num - 1) * 8);		
		}

	coordinate->coordinate_x = (left_top >> 16) & 0x7ff;
	coordinate->coordinate_y = left_top & 0x7ff;
	coordinate->size_x = (right_bottom >> 16) & 0x7ff;
	coordinate->size_y = right_bottom & 0x7ff;

	// copy_to_user(coordinate, &pos, sizeof(pos));
	return 0;	
}

/*
static int sep0611fb_get_screen_info(struct fb_info *info, overlay_config_t * arg )
{
	struct fb_var_screeninfo *var = &info->var;
	overlay_config_t pos;
	
	pos.w = var->width;
	pos.h = var->height;
	pos.s_w = 0;
	pos.s_h = 0;
	
	copy_to_user((overlay_config_t*)arg, &pos, sizeof(pos));		

	return 0;	
}
*/

static int sep0611fb_set_screen_coordinate(struct fb_info *info, struct layer_coordinate_t *coordinate)
{
	struct sep0611fb_layer *layers = info->par;
	struct sep0611fb_info *fbi = layers->parent;
	unsigned int layer_num =  layers->layer_id;
	unsigned int left_top, right_bottom;

	/**************************************
	*TODO:fix the constraint on resolution
	***************************************/
	//var->width = pos.size_x;
	//var->height = pos.size_y;

	layers->layer_coordinate[layer_num].coordinate_x = coordinate->coordinate_x;
	layers->layer_coordinate[layer_num].coordinate_y = coordinate->coordinate_y;
	layers->layer_coordinate[layer_num].size_x = coordinate->size_x;
	layers->layer_coordinate[layer_num].size_y = coordinate->size_y;

	left_top = LEFT_TOP(coordinate->coordinate_x, coordinate->coordinate_y);
	right_bottom = RIGHT_BOTTOM(coordinate->size_x, coordinate->size_y);

	//writel(WIN_DISABLE, fbi->io + SEP0611_LCDCECR);

	if(layer_num == layer0) {
		writel(left_top, fbi->io + SEP0611_BTPCR);
		writel(right_bottom, fbi->io + SEP0611_BBPCR);
	} else {
		writel(left_top, fbi->io + SEP0611_W1TPCR + (layer_num - 1) * 8);
		writel(right_bottom, fbi->io + SEP0611_W1BPCR + (layer_num - 1) * 8);
	}
		
	//writel(WIN_ENABLE, fbi->io+SEP0611_LCDCECR);

	return 0;	
}

/*
static int sep0611fb_queue_buffer(struct fb_info *info, unsigned long* arg )
{
	return 0;
}
*/

static int sep0611fb_layers_on_off(struct fb_info *info, unsigned long arg )
{
	struct sep0611fb_layer *layers = info->par;
	struct sep0611fb_info *fbi = layers->parent;
	int layer_num = layers->layer_id;

	if (arg == WIN_ENABLE || arg == WIN_DISABLE) {
		writel(arg, fbi->io + SEP0611_BECR + layer_num * 4);
		return 0;
	}	

	return -EINVAL;
}

/**********************************************end*************************************************************/


/*********************************************
Function:    sep0611fb_ioctl
Description: ioctl cmd seletion
Parameter:   info:
		struct fb_info
	     cmd:
		which commamd you want to operation
		these are defined in sep0611_fb.h
	     arg:
		user's parameter
**********************************************/
static int sep0611fb_ioctl(struct fb_info *info, unsigned int cmd, unsigned long arg)
{
	struct sep0611fb_layer *layers = info->par;
	struct sep0611fb_info *fbi = layers->parent;
	unsigned int alpha_level, brightness;
	struct layer_coordinate_t pos;
	struct lcdc_timing_params_t lcdc_timing;

	switch(cmd) 
	{
		/******************************************************
		*when it comes to that we do not have struct fb_info and
		*cannot deal with the exact layer, this may help you 
		*******************************************************/
		case SEP0611FB_IOCTL_LAYERS_SELECT:
			layers_select = (enum layers_select_t) arg;
			break;

		case SEP0611FB_IOCTL_BACKLIGHT_ON_OFF:
			if (arg == 1)
				sep0611_enable_backlight(1);
			else
				sep0611_enable_backlight(0);
			break;

		case SEP0611FB_IOCTL_BRIGHTNESS_ON_OFF:
			brightness = readl(fbi->io + SEP0611_PCCR) & ~(1 << 8);
			writel(brightness | (arg << 8), fbi->io + SEP0611_PCCR);
			break;

		case SEP0611FB_IOCTL_SET_BRIGHTNESS_MODE:
			brightness = readl(fbi->io + SEP0611_PCCR) & ~(3 << 9);
			writel(brightness | (arg << 9), fbi->io + SEP0611_PCCR);
			break;

		case SEP0611FB_IOCTL_SET_BRIGHTNESS_VALUE:
			brightness = readl(fbi->io + SEP0611_PCCR) & ~0xff;
			writel(brightness | arg, fbi->io +SEP0611_PCCR);
			break;

		case SEP0611FB_IOCTL_LAYERS_ON_OFF:
			if (sep0611fb_layers_on_off(info, arg))
				return -EFAULT;
			break;

		// case SEP0611FB_IOCTL_SET_FRAMEBUFFER_OFFSET:
			// sep0611fb_layer_set_framebuffer_offset(info, arg);	

		case SEP0611FB_IOCTL_SET_ALPHA_MODE:
			if (sep0611fb_set_alpha_mode(info, arg))
				return -EFAULT;
			break;

		case SEP0611FB_IOCTL_SET_ALPHA_USR_VALUE:
			if (sep0611fb_set_alpha_usr_value(info, arg))
				return -EFAULT;
			break;

		case SEP0611FB_IOCTL_SET_ALPHA_ARGB_VALUE:
			if (sep0611fb_set_alpha_argb_value(info, arg))
				return -EFAULT;
			break;

		case SEP0611FB_IOCTL_SET_ALPHA_USR_VALUE_UP_DOWN:
			alpha_level = layers->alpha_level;
			if (arg == 1) {
				if (alpha_level < SEP0611_MAX_ALPHA_LEVEL)
					alpha_level++;
			} else if (arg == 0) {
				if (alpha_level > 0)
					alpha_level--;
				}
			if (sep0611fb_set_alpha_usr_value(info, alpha_level))
				return -EFAULT;
			break;

		case SEP0611FB_IOCTL_COLORKEY_ON_OFF:
			sep0611fb_colorkey_on_off(info, arg);
			break;

		case SEP0611FB_IOCTL_SET_COLORKEY_VALUE:
			if (sep0611fb_set_colorkey_vaule(info, arg))
				return -EFAULT;
			break;
		/*****************************************************************
		*NOTE: strongly do NOT recommend these two options below, since we
 		*set that during the time fb driver loading
		******************************************************************/
		case SEP0611FB_IOCTL_YUV_ON_OFF:
			sep0611fb_yuv_on_off(info, arg);
			break;

		case SEP0611FB_IOCTL_SET_YUV_MODE:
			if (sep0611fb_set_yuv_mode(info, arg))
				return -EFAULT;
			break;

		case SEP0611FB_IOCTL_SET_SCREEN_COORDINATE:
			if (copy_from_user(&pos, (struct layer_coordinate *) arg, sizeof(pos)))
				return -EFAULT;
			if (sep0611fb_set_screen_coordinate(info, &pos))
				return -EFAULT;
			break;

		case SEP0611FB_IOCTL_GET_SCREEN_COORDINATE:
			if (sep0611fb_get_screen_coordinate(info, &pos))
				return -EFAULT;
			if (copy_to_user((struct layer_coordinate *) arg, &pos, sizeof(pos)))
				return -EFAULT;
			break;

		case SEP0611FB_IOCTL_SET_CURSOR_MODE:
			if (sep0611fb_set_cursor_mode(info, arg))
				return -EFAULT;
			break;

		case SEP0611FB_IOCTL_SET_CURSOR_PAL:
			if (sep0611fb_set_cursor_pal(info, (unsigned int *) arg))
				return -EFAULT;
			break;

		case SEP0611FB_IOCTL_SET_HDMI_TIMING:
			if (copy_from_user(&lcdc_timing, (struct lcdc_timing_params_t *) arg, sizeof(lcdc_timing)))
				return -EFAULT;
			sep0611_hdmi_lcdc_timing(&lcdc_timing);
			break;
#if 0

		case OVERLAY_GET_SCREEN_INFO:
			sep0611fb_get_screen_info(info, (overlay_config_t*) arg );
			break;

		case OVERLAY_QUEUE_BUFFER:
			sep0611fb_queue_buffer(info, (unsigned long*) arg);
			break;

		case OVERLAY_SET_CONFIGURE:
			//sep0611fb_set_configure((overlay_config_t*)arg, layer_num);
			break;		
#endif		
		
		default:
			printk("sep0611fb unsupport ioctl cmd\n");
			break;
	}

	return 0;
}

struct fb_ops sep0611fb_ops = {
	.owner		= THIS_MODULE,
	.fb_check_var	= sep0611fb_check_var,
	.fb_set_par	= sep0611fb_set_par,
	.fb_blank	= sep0611fb_blank,
	.fb_pan_display	= sep0611fb_pan_display,
	.fb_setcolreg	= sep0611fb_setcolreg,
	.fb_fillrect	= cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
	.fb_ioctl	= sep0611fb_ioctl,
};

/*********************************************
Function:    sep0611fb_calculate_yuv_smem_len
Description: calculate yuv mem length
Parameter:   layer:
		struct sep0611fb_layer
**********************************************/
static void sep0611fb_calculate_yuv_smem_len(struct sep0611fb_layer *layer)
{
	struct sep0611fb_info *fbi 	= layer->parent;
	struct fb_var_screeninfo *var 	= &layer->fb->var;
	unsigned int layer_num 		= layer->layer_id;
	unsigned int lcdcbcr;
	unsigned int yuv_enable, yuv_mode;
	unsigned int total_size,y_size,u_size,v_size;

	lcdcbcr 	= readl(fbi->io + SEP0611_LCDCBCR);
	yuv_enable 	= (lcdcbcr >> 31) & 0x1;
	yuv_mode 	= (lcdcbcr >> 29) & 0x3;

	y_size = var->xres_virtual * var->yres_virtual;

	/* layer1 chooses yuv mode */
	if((layer_num == layer1) && yuv_enable) {
		switch(yuv_mode) {
		case YUV444:
			u_size = v_size = y_size;
			break;
		case YUV422:
			u_size = v_size = (y_size >> 1);
			break;
		case YUV420:
		case YUV420_semi:
			u_size = v_size = (y_size >> 2);
			break;
		}

		total_size = y_size + u_size + v_size;

		layer->fb->fix.smem_len = total_size;

	}

}
 


static void sep0611fb_init_fbinfo(struct sep0611fb_info *fbi, char *drv_name, int layer_no)
{
	struct sep0611fb_layer *layers = fbi->sep_layers[layer_no];
	struct fb_var_screeninfo *var  = &layers->fb->var;
	unsigned int lcdcbcr 	       = readl(fbi->io + SEP0611_LCDCBCR);
	unsigned int yuv_enable		   = (lcdcbcr >> 31) & 0x1;
	unsigned int yuv_mode		   = (lcdcbcr >> 29) & 0x3;
	int i;

	strcpy(layers->fb->fix.id, drv_name);

	layers->fb->fix.type 	       = FB_TYPE_PACKED_PIXELS;
	layers->fb->fix.type_aux       = 0;
	layers->fb->fix.xpanstep       = 0;
	layers->fb->fix.ypanstep       = 1;
	layers->fb->fix.ywrapstep      = 0;
	layers->fb->fix.accel          = FB_ACCEL_NONE;


	layers->fb->pseudo_palette     = &layers->pseudo_pal;

	for (i = 0; i < 256; i++)
		layers->palette_buffer[i] = PALETTE_BUFF_CLEAR;

	layers->fb->fbops 	       = &sep0611fb_ops;
	layers->fb->flags	       = FBINFO_FLAG_DEFAULT;

	/*
	 *  31-----------3  2--------1    0
	 *  |   unused   |  |yuv_mode|  |std|
	 */
	if ((layer_no == layer1) && yuv_enable)
		layers->fb->var.nonstd = (yuv_mode << 1) | yuv_enable;
	else	layers->fb->var.nonstd = 0;

	layers->fb->var.activate       = FB_ACTIVATE_NOW;
	layers->fb->var.accel_flags    = 0;
	layers->fb->var.vmode 	       = FB_VMODE_NONINTERLACED;

	layers->fb->var.xoffset        = sep0611_lcdc_info.xoffset;
	layers->fb->var.yoffset        = sep0611_lcdc_info.yoffset;

	if (layer_no < 3) {
		layers->fb->var.height = sep0611_lcdc_info.height;
		layers->fb->var.width  = sep0611_lcdc_info.width;

		layers->fb->var.xres   = sep0611_lcdc_info.xres;
		layers->fb->var.yres   = sep0611_lcdc_info.yres;

		layers->fb->var.xres_virtual = sep0611_lcdc_info.xres_virtual;
		layers->fb->var.yres_virtual = sep0611_lcdc_info.yres_virtual;
	} else {
		layers->fb->var.height  = sep0611_lcdc_info.osd_height;
		layers->fb->var.width   = sep0611_lcdc_info.osd_width;

		layers->fb->var.xres 	= sep0611_lcdc_info.osd_xres;
		layers->fb->var.yres 	= sep0611_lcdc_info.osd_yres;


		layers->fb->var.xres_virtual = sep0611_lcdc_info.osd_xres_virtual;
		layers->fb->var.yres_virtual = sep0611_lcdc_info.osd_yres_virtual;
	}

	layers->fb->var.bits_per_pixel  = sep0611_lcdc_info.layer_bpp[layer_no];		// care here
	layers->fb->var.pixclock 	= sep0611_lcdc_info.pixclock;
	layers->fb->var.hsync_len 	= sep0611_lcdc_info.hsync_len;
	layers->fb->var.left_margin 	= sep0611_lcdc_info.left_margin;
	layers->fb->var.right_margin 	= sep0611_lcdc_info.right_margin;
	layers->fb->var.vsync_len 	= sep0611_lcdc_info.vsync_len;
	layers->fb->var.upper_margin 	= sep0611_lcdc_info.upper_margin;
	layers->fb->var.lower_margin 	= sep0611_lcdc_info.lower_margin;
	
	//finfo->fb.var.sync 		= sep0611_lcdc_info.sync;
	//finfo->fb.var.grayscale 	= sep0611_lcdc_info.cmap_grayscale;//this will be fixed up

	layers->fb->fix.smem_len = var->xres_virtual * var->yres_virtual * sep0611_lcdc_info.layer_bpp[layer_no] >> 3;

	/* calculate yuv framebuffer length of layer1 if it is yuv mode */
	if ((layer_no == 1) && (yuv_enable == 1))
		sep0611fb_calculate_yuv_smem_len(layers);
	else
		layers->fb->fix.line_length = var->xres * var->bits_per_pixel >> 3;

}

/*********************************************
Function:    sep0611fb_map_video_memory
Description: map video memory
Parameter:   fbi:
		struct sep0611fb_info
	     layer_no:
		int the layer which you want to
		build video memory
**********************************************/
static int sep0611fb_map_video_memory(struct sep0611fb_info *fbi, int layer_no)
{
	struct sep0611fb_layer *layers = fbi->sep_layers[layer_no];

	layers->map_size = PAGE_ALIGN(layers->fb->fix.smem_len);


	//Care:map_cpu is virtual address of framebuffer, while map_dma physical address

	layers->map_cpu = dma_alloc_writecombine(fbi->dev, layers->map_size, &layers->map_dma, GFP_KERNEL);
	if (layers->map_cpu) {
		printk("Layer[%d]: map video memory  virtual addr[0x%p] physical addr[0x%08x] size[0x%08x]\n",
			layers->layer_id, layers->map_cpu, layers->map_dma, layers->map_size);
		// prevent initial garbage on screen 
		memset(layers->map_cpu, 0x0, layers->map_size);

		layers->screen_dma = layers->map_dma;
		layers->fb->screen_base = layers->map_cpu;
		layers->fb->fix.smem_start = layers->map_dma;
	}

	if (!layers->map_cpu)
		return -ENOMEM;

	return 0;
}

static void sep0611fb_unmap_video_memory(struct sep0611fb_info *fbi, int layer_no)
{
	struct sep0611fb_layer *layers = fbi->sep_layers[layer_no];

	dma_free_writecombine(fbi->dev, layers->map_size, layers->map_cpu,  layers->map_dma);

}



/*********************************************
Function:    sep0611fb_init_registers
Description: init sep0611 lcd register,including
		size,position,buffer address configuration
Parameter:   fbi:
		struct sep0611fb_info
	     layer_no:
		layer num
**********************************************/
static void sep0611fb_init_registers(struct sep0611fb_info *fbi, int layer_no)
{
	struct sep0611fb_layer *layers = fbi->sep_layers[layer_no];
	struct fb_var_screeninfo *var = &layers->fb->var;
	unsigned int start_address;
	unsigned int yuv_enable;

//--------------add by zyt--------------
	unsigned int data;
//--------------add by zyt--------------
	start_address = layers->screen_dma;
	yuv_enable = readl(fbi->io + SEP0611_LCDCBCR) & (1 << 31);

	writel(0, fbi->io + SEP0611_ACSR);	

	switch(layer_no)
	{
		case 0:
			sep0611_lcdc_info.regs.bbsar = start_address;
			writel(sep0611_lcdc_info.regs.bbsar, fbi->io + SEP0611_BBSAR);
			
			writel(var->xres, fbi->io + SEP0611_BASE_RAW_IMAGE_WIDTH);

			writel(sep0611_lcdc_info.regs.btpcr, fbi->io + SEP0611_BTPCR);
			writel(sep0611_lcdc_info.regs.bbpcr, fbi->io + SEP0611_BBPCR);
			writel(1, fbi->io+SEP0611_BECR);
			break;
		case 1:
			sep0611_lcdc_info.regs.w1ybsar = start_address;
			writel(sep0611_lcdc_info.regs.w1ybsar, fbi->io+SEP0611_W1YBSAR);
			if (yuv_enable) {
				sep0611fb_yuv_address_configure(layers);
				writel(start_address + layer1_yuv.y_size, fbi->io + SEP0611_W1UBSAR);
				writel(start_address + layer1_yuv.y_size + layer1_yuv.u_size, fbi->io + SEP0611_W1VBSAR);
			}

			writel(var->xres, fbi->io + SEP0611_OVER1_RAW_IMAGE_WIDTH);
			writel(sep0611_lcdc_info.regs.w1tpcr, fbi->io+SEP0611_W1TPCR);
			writel(sep0611_lcdc_info.regs.w1bpcr, fbi->io+SEP0611_W1BPCR);
			writel(0, fbi->io+SEP0611_W1ECR);
			//writel(1, fbi->io+SEP0611_W1ECR);/*2010-12-13*/
			break;
		case 2:
		//	sep0611_lcdc_info.regs.w2bsar = start_address;
			sep0611_lcdc_info.regs.w2bsar = sep0611_lcdc_info.regs.bbsar;
			writel(sep0611_lcdc_info.regs.w2bsar, fbi->io+SEP0611_W2BSAR);

			writel(var->xres, fbi->io + SEP0611_OVER2_RAW_IMAGE_WIDTH);

			writel(sep0611_lcdc_info.regs.w2tpcr, fbi->io+SEP0611_W2TPCR);
			writel(sep0611_lcdc_info.regs.w2bpcr, fbi->io+SEP0611_W2BPCR);
			writel(0, fbi->io+SEP0611_W2ECR);
			//writel(1, fbi->io+SEP0611_W2ECR);

			//set overlay2 rgb expansion
/**************comment by zyt**************
	*		unsigned int data;
**************comment by zyt**************/
			data = readl(fbi->io + SEP0611_LCDCBCR);
			data &= ~(0x3<<15);
			data |= (0x2<<15);
			writel(data, fbi->io + SEP0611_LCDCBCR);

			//open color key
			data = readl(fbi->io + SEP0611_W2CKR);
			data &= ~(1<<24);
			data |= (1<<24);//enable c-k
			data &=~(0xffffff);// use black as color key
			data |=0x070307;
			writel(data, fbi->io + SEP0611_W2CKR);
			data = readl(fbi->io + SEP0611_W2CKR); 
			printk("after set colorkey data=0x%x\n",data);	
			break;
		case 3:
			sep0611_lcdc_info.regs.cbsar = start_address;
			writel(sep0611_lcdc_info.regs.cbsar, fbi->io+SEP0611_CBSAR);

			writel(sep0611_lcdc_info.regs.ctpcr, fbi->io+SEP0611_CTPCR);
			writel(sep0611_lcdc_info.regs.cbpcr, fbi->io+SEP0611_CBPCR);
			writel(0, fbi->io+SEP0611_CECR);
			break;
		default:
			break;
	}

	writel(1, fbi->io + SEP0611_ACSR);
}

/*********************************************
Function:    sep0611fb_probe_layers
Description: sep0611 lcd each layer probe should 
		be cared
Parameter:   info:
		struct sep0611fb_info
	     layer_no:
		layer num
**********************************************/
static int __devinit sep0611fb_probe_layers(struct sep0611fb_info *info, unsigned int layer_no)
{
	struct fb_info *fbinfo;
	char driver_name[] = "sep0611fb";
	int ret = 0;

	fbinfo = framebuffer_alloc(sizeof(struct sep0611fb_layer), info->dev);
	if (!fbinfo)
		return -ENOMEM;

	info->sep_layers[layer_no] = fbinfo->par;
	info->sep_layers[layer_no]->fb = fbinfo;
	info->sep_layers[layer_no]->layer_id = layer_no;
	info->sep_layers[layer_no]->parent = info;	

	if (layer_no == layer1 || layer_no == layer2)
		info->sep_layers[layer_no]->alpha_level = 15;

	// basic hardware initialization 
	if (layer_no == layer0)
		sep0611fb_init_hw(info);

	// initialize the var and fix parameters 
	sep0611fb_init_fbinfo(info, driver_name, layer_no);

	// Initialize video memory 
	ret = sep0611fb_map_video_memory(info, layer_no);
	if (ret) {
		printk("Failed to allocate video RAM: %d\n", ret);
		ret = -ENOMEM;
		goto free_fbinfo;
	}

	sep0611fb_init_registers(info, layer_no);

	sep0611fb_check_var(&info->sep_layers[layer_no]->fb->var, info->sep_layers[layer_no]->fb);

	ret = register_framebuffer(info->sep_layers[layer_no]->fb);
	if (ret < 0) {
		printk(KERN_ERR "Failed to register framebuffer device: %d\n", ret);
		goto unmap_video_memory;	
	}
	return 0;

unmap_video_memory:
	sep0611fb_unmap_video_memory(info, layer_no);
free_fbinfo:
	framebuffer_release(fbinfo);
	fbinfo = NULL;
	return ret;
}

static void sep0611fb_release_layers(struct sep0611fb_info *info, unsigned int layer_no)
{
	struct fb_info *fbinfo = info->sep_layers[layer_no]->fb;
	if(fbinfo) {
		unregister_framebuffer(fbinfo);
		sep0611fb_unmap_video_memory(info, layer_no);
		framebuffer_release(fbinfo);
	} 
}


void reserve_lcd_info(struct sep0611fb_info *fbi)
{
	struct sep0611fb_hw *regs = &sep0611_lcdc_info.regs;

	//regs->lcdcecr			= readl(fbi->io + SEP0611_LCDCECR);
	regs->becr				= readl(fbi->io + SEP0611_BECR);
	regs->w1ecr				= readl(fbi->io + SEP0611_W1ECR);
	regs->w2ecr				= readl(fbi->io + SEP0611_W2ECR);
	regs->cecr				= readl(fbi->io + SEP0611_CECR);

	regs->lcdcbcr			= readl(fbi->io + SEP0611_LCDCBCR);
	regs->plcr				= readl(fbi->io + SEP0611_PLCR);
	regs->pfcr				= readl(fbi->io + SEP0611_PFCR);
	regs->pccr				= readl(fbi->io + SEP0611_PCCR);
//	regs->iocr				= readl(fbi->io + SEP0611_IOCR);
//	regs->iecr				= readl(fbi->io + SEP0611_IECR);

	regs->w1ckr				= readl(fbi->io + SEP0611_W1CKR);
	regs->w2ckr				= readl(fbi->io + SEP0611_W2CKR);
	regs->w1bvr				= readl(fbi->io + SEP0611_W1BVR);
	regs->w2bvr				= readl(fbi->io + SEP0611_W2BVR);
	regs->cc0r				= readl(fbi->io + SEP0611_CC0R);
	regs->cc1r				= readl(fbi->io + SEP0611_CC1R);
	regs->cc2r				= readl(fbi->io + SEP0611_CC2R);
	regs->cc3r				= readl(fbi->io + SEP0611_CC3R);
	regs->bakgrnd_color		= readl(fbi->io + SEP0611_BAKGRND_COLOR);
	regs->r_order			= readl(fbi->io + SEP0611_R_ORDER);
	regs->g_order			= readl(fbi->io + SEP0611_G_ORDER);
	regs->b_order			= readl(fbi->io + SEP0611_B_ORDER);
	regs->rgb_order			= readl(fbi->io + SEP0611_RGB_ORDER);

//	regs->acsr				= readl(fbi->io + SEP0611_ACSR);
	regs->lcdcscr			= readl(fbi->io + SEP0611_LCDCSCR);
	regs->btpcr				= readl(fbi->io + SEP0611_BTPCR);
	regs->bbpcr				= readl(fbi->io + SEP0611_BBPCR);
	regs->w1tpcr			= readl(fbi->io + SEP0611_W1TPCR);
	regs->w1bpcr			= readl(fbi->io + SEP0611_W1BPCR);
	regs->w2tpcr			= readl(fbi->io + SEP0611_W2TPCR);
	regs->w2bpcr			= readl(fbi->io + SEP0611_W2BPCR);
	regs->ctpcr				= readl(fbi->io + SEP0611_CTPCR);
	regs->cbpcr				= readl(fbi->io + SEP0611_CBPCR);
	regs->raw_image_width_0	= readl(fbi->io + SEP0611_BASE_RAW_IMAGE_WIDTH);
	regs->raw_image_width_1	= readl(fbi->io + SEP0611_OVER1_RAW_IMAGE_WIDTH);
	regs->raw_image_width_2	= readl(fbi->io + SEP0611_OVER2_RAW_IMAGE_WIDTH);

	regs->bbsar				= readl(fbi->io + SEP0611_BBSAR);
	regs->w1ybsar			= readl(fbi->io + SEP0611_W1YBSAR);
	regs->w1ubsar			= readl(fbi->io + SEP0611_W1UBSAR);
	regs->w1vbsar			= readl(fbi->io + SEP0611_W1VBSAR);
	regs->w2bsar			= readl(fbi->io + SEP0611_W2BSAR);
	regs->cbsar				= readl(fbi->io + SEP0611_CBSAR);
	
}

void resume_lcd_info(struct sep0611fb_info *fbi)
{
	struct sep0611fb_hw *regs = &sep0611_lcdc_info.regs;
	writel(0x0, LCDC_ACSR_V);
	writel(regs->lcdcbcr,			fbi->io + SEP0611_LCDCBCR);
	writel(regs->plcr,				fbi->io + SEP0611_PLCR);
	writel(regs->pfcr,				fbi->io + SEP0611_PFCR);
//	writel(regs->pccr,				fbi->io + SEP0611_PCCR);
	writel(regs->plcr,				fbi->io + SEP0611_PLCR);

	writel(regs->lcdcscr,			fbi->io + SEP0611_LCDCSCR);
	writel(regs->btpcr,				fbi->io + SEP0611_BTPCR);
	writel(regs->bbpcr,				fbi->io + SEP0611_BBPCR);
	writel(regs->w1tpcr,			fbi->io + SEP0611_W1TPCR);
	writel(regs->w1bpcr,			fbi->io + SEP0611_W1BPCR);
	writel(regs->w2tpcr,			fbi->io + SEP0611_W2TPCR);
	writel(regs->w2bpcr,			fbi->io + SEP0611_W2BPCR);
	writel(regs->ctpcr,				fbi->io + SEP0611_CTPCR);
	writel(regs->cbpcr,				fbi->io + SEP0611_CBPCR);
	writel(regs->raw_image_width_0,	fbi->io + SEP0611_BASE_RAW_IMAGE_WIDTH);
	writel(regs->raw_image_width_1,	fbi->io + SEP0611_OVER1_RAW_IMAGE_WIDTH);
	writel(regs->raw_image_width_2,	fbi->io + SEP0611_OVER2_RAW_IMAGE_WIDTH);

	writel(regs->bbsar,				fbi->io + SEP0611_BBSAR);
	writel(regs->w1ybsar,			fbi->io + SEP0611_W1YBSAR);
	writel(regs->w1ubsar,			fbi->io + SEP0611_W1UBSAR);
	writel(regs->w1vbsar,			fbi->io + SEP0611_W1VBSAR);
	writel(regs->w2bsar,			fbi->io + SEP0611_W2BSAR);
	writel(regs->cbsar,				fbi->io + SEP0611_CBSAR);

	writel(regs->w1ckr,				fbi->io + SEP0611_W1CKR);
	writel(regs->w2ckr,				fbi->io + SEP0611_W2CKR);
	writel(regs->w1bvr,				fbi->io + SEP0611_W1BVR);
	writel(regs->w2bvr,				fbi->io + SEP0611_W2BVR);
	writel(regs->cc0r,				fbi->io + SEP0611_CC0R);
	writel(regs->cc1r,				fbi->io + SEP0611_CC1R);
	writel(regs->cc2r,				fbi->io + SEP0611_CC2R);
	writel(regs->cc3r,				fbi->io + SEP0611_CC3R);
	writel(regs->bakgrnd_color,		fbi->io + SEP0611_BAKGRND_COLOR);
	writel(regs->r_order,			fbi->io + SEP0611_R_ORDER);
	writel(regs->g_order,			fbi->io + SEP0611_G_ORDER);
	writel(regs->b_order,			fbi->io + SEP0611_B_ORDER);
	writel(regs->rgb_order,			fbi->io + SEP0611_RGB_ORDER);

//	writel(regs->lcdcecr,			fbi->io + SEP0611_LCDCECR);
	writel(regs->becr,				fbi->io + SEP0611_BECR);
	writel(regs->w1ecr,				fbi->io + SEP0611_W1ECR);
	writel(regs->w2ecr,				fbi->io + SEP0611_W2ECR);
	writel(regs->cecr,				fbi->io + SEP0611_CECR);
	writel(0x1, LCDC_ACSR_V);
	writel(0x1, LCDC_ECR_V);
}

#ifdef CONFIG_HAS_EARLYSUSPEND
static void sep0611fb_early_suspend(struct early_suspend *h)
{
	struct sep0611fb_info *fbi;
	fbi = container_of(h, struct sep0611fb_info, fb_early_suspend_info);

	sep0611_enable_backlight(0);

	reserve_lcd_info(fbi);
	printk("%s\n", __func__);

	clk_disable(fbi->clk);

	writel(0, fbi->io + SEP0611_LCDCECR);
	writel(0, fbi->io + SEP0611_BECR);
	writel(0, fbi->io + SEP0611_W1ECR);
	writel(0, fbi->io + SEP0611_W2ECR);
	writel(0, fbi->io + SEP0611_CECR);
}

static void resume_work_func(struct work_struct *work)
{
	sep0611_enable_backlight(1);
}

static void sep0611fb_late_resume(struct early_suspend *h)
{
	struct sep0611fb_info *fbi;
	fbi = container_of(h, struct sep0611fb_info, fb_early_suspend_info);

	printk("%s\n", __func__);
	
	clk_enable(fbi->clk);

	resume_lcd_info(fbi);

	cancel_delayed_work(&work);
	queue_delayed_work(resume_wq, &work, msecs_to_jiffies(500));
}
#endif

/*************add by zy**************/
//#ifdef CONFIG_CPU_FREQ
#if	0
struct notifier_block   sep_fb_freq_transition;
static int sep_fb_cpufreq_transition(struct notifier_block *nb,
                      unsigned long val, void *data)
{
//	printk("%s\n",__func__);
	struct	sep0611fb_layer *layer;
	struct	sep0611fb_info *fbi	= layer->parent;
	int tmp,ret;
	unsigned int pcd = 0;
	unsigned long lcdc_rate;

	lcdc_rate = clk_get_rate(fbi->clk);
//	printk("lcdc_rate is %ld\n",lcdc_rate);
   	mdelay(1);   	
//	writel(WIN_DISABLE, info->io+SEP0611_LCDCECR);	//禁止LCDC  
	tmp = readl(fbi->io + SEP0611_LCDCBCR);	
//	printk("tmp is 0x%x\n",tmp);
	pcd = lcdc_rate/800/480/22+1;//22 should be 21.7,but it matters little
	pcd&= 0x3e;
	pcd-= 2;
	tmp = tmp | pcd;
	writel(tmp, fbi->io + SEP0611_LCDCBCR);
//	writel(WIN_ENABLE, info->io+SEP0611_LCDCECR);
//	mdelay(1); 
//	printk("out: %s\n",__func__);
    return 0;
}

static inline int sep_fb_cpufreq_register(void)
{
    sep_fb_freq_transition.notifier_call = sep_fb_cpufreq_transition;

    return cpufreq_register_notifier(&sep_fb_freq_transition,
                     CPUFREQ_TRANSITION_NOTIFIER);
}

static inline void sep_fb_cpufreq_deregister(void)
{
    cpufreq_unregister_notifier(&sep_fb_freq_transition,
                    CPUFREQ_TRANSITION_NOTIFIER);
}

#else
static inline int sep_fb_cpufreq_register(void)
{
    return 0;
}

static inline void sep_fb_cpufreq_deregister(void)
{
}
#endif
/*************add by zy**************/

static int __init sep0611fb_probe(struct platform_device *pdev)
{
	struct resource *res;
	struct sep0611fb_info *info;
	int index = 0, ret;
	//printk("ZM(5) begin probe: enter function sep0611fb_probe $$\n");//debug	
	info = kzalloc(sizeof(struct sep0611fb_info), GFP_KERNEL);
	if(info == NULL) {
		dev_err(&pdev->dev, "Failed to allocate memory for framebuffer\n");
		return -EINVAL;
	}

	info->dev = &pdev->dev;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	if (res == NULL) {
		dev_err(&pdev->dev, "Failed to get memory registers\n");
		ret = -ENXIO;
		goto dealloc_fb;
	}

	info->mem = request_mem_region(res->start, resource_size(res), pdev->name);

	if (info->mem == NULL) {
		dev_err(&pdev->dev, "Failed to get memory region\n");
		ret = -ENOENT;
		goto dealloc_fb;
	}

	info->io = ioremap(res->start, resource_size(res));

	if (info->io == NULL) {
		dev_err(&pdev->dev, "Failed to IO remap\n");
		ret = -ENXIO;
		goto release_mem;
	}

#ifdef CONFIG_ARCH_SEP0611	
	info->clk = clk_get(NULL, "lcdc");
	if (!info->clk || IS_ERR(info->clk)) {
		printk(KERN_INFO "failed to get lcd clock source\n");
		ret =  -ENOENT;
		goto release_io;
	}
	clk_enable(info->clk);
	info->clkrate = clk_get_rate(info->clk);
#endif

	/*************add by zy**************/
	sep_fb_cpufreq_register();
	/*************add by zy**************/

	for (index = 0; index < SEP0611FB_NUM; index++) 
	{ 
		ret = sep0611fb_probe_layers(info, index);
		if(ret < 0) {
			dev_err(&pdev->dev,"Failed to create layer[%d]\n",index);
			for(;index >= 0;index--)
				sep0611fb_release_layers(info, index);
			goto release_clock;
		}

	}

	platform_set_drvdata(pdev,info);

#ifdef CONFIG_HAS_EARLYSUSPEND
	info->fb_early_suspend_info.suspend = sep0611fb_early_suspend;
	info->fb_early_suspend_info.resume = sep0611fb_late_resume;
	info->fb_early_suspend_info.level = EARLY_SUSPEND_LEVEL_DISABLE_FB;
	register_early_suspend(&info->fb_early_suspend_info);
#endif
	// the last step: open the lcdc and enable the backlight 
	writel(1, info->io+SEP0611_LCDCECR);
	sep0611_enable_backlight(1);
	//printk("sep0611fb_probe sep0611_lcdc_info.regs.lcdcbcr=0x%08x***********\n",sep0611_lcdc_info.regs.lcdcbcr); //debug
	//printk("SEP0611_LCDCBCR = 0x%08x***********************************\n", readl(info->io + SEP0611_LCDCBCR));  //debug
	return 0;

release_clock:
#ifdef CONFIG_ARCH_SEP0611 	
	clk_disable(info->clk);
//	clk_put(info->clk);
#endif
 release_io:
	iounmap(info->io);

release_mem:
	release_resource(info->mem);
	kfree(info->mem);

dealloc_fb:
	kfree(info);
	return ret;
}


static int sep0611fb_remove(struct platform_device *pdev)
{
	struct sep0611fb_info *fbi = platform_get_drvdata(pdev);
	int index;

	platform_set_drvdata(pdev, 0);

#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&fbi->fb_early_suspend_info);
#endif

	for (index = 0; index < SEP0611FB_NUM; index++)
		if (fbi->sep_layers[index])
			sep0611fb_release_layers(fbi, index);

	/*************add by zhangyue**************/
	sep_fb_cpufreq_deregister();
	/*************add by zhangyue**************/

#ifdef CONFIG_ARCH_SEP0611
	if (fbi->clk) {
		clk_disable(fbi->clk);
//		clk_put(fbi->clk);
	 	fbi->clk = NULL;
	}
#endif

	iounmap(fbi->io);
	release_resource(fbi->mem);
	kfree(fbi->mem);
	kfree(fbi);

	return 0;
}


/********************************
 this region will be fixed
 care:early suspend
      later resume
*********************************/
#ifdef CONFIG_PM

int sep0611fb_suspend(struct platform_device *dev, pm_message_t state)
{
	printk("%s\n", __func__);
	//sep0611_enable_backlight(0);
	//  other operations will be fixed later
	return 0;
}

int sep0611fb_resume(struct platform_device *dev)
{
	printk("%s\n", __func__);
	//sep0611_enable_backlight(1);
	//  other operations will be fixed later
	return 0;
}

#else

#define sep0611fb_suspend 	NULL
#define sep0611fb_resume 	NULL

#endif 


static struct platform_driver sep0611fb_driver = {
	.probe		= sep0611fb_probe,
	.remove		= sep0611fb_remove,
	.suspend	= sep0611fb_suspend,
	.resume		= sep0611fb_resume,
	.driver		= {
		.name	= "sep0611-fb",
		.owner	= THIS_MODULE,
	},
};

int __devinit sep0611fb_init(void)
{
	printk("SEP0611 Framebuffer driver,(C) SOUTHEAST UNIVERSITY\n");
	resume_wq = create_singlethread_workqueue("resume_wq");
	return platform_driver_register(&sep0611fb_driver);
}

static void __exit sep0611fb_cleanup(void)
{
	if (resume_wq)
		destroy_workqueue(resume_wq);
	platform_driver_unregister(&sep0611fb_driver);
}

module_init(sep0611fb_init);
module_exit(sep0611fb_cleanup);

MODULE_AUTHOR("leeming ,leeming1203@gmail.com");
MODULE_DESCRIPTION("SEP0611 Framebuffer Driver");
MODULE_LICENSE("GPL");

