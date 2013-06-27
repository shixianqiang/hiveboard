/* kernel\drivers\video\sep0611\sep_fb_ioctrl.h
 *   Head file for framebuffer driver ioctrl
 * Author : lixiang
 * Version: 1.0.1 2011-7-6
*/

#ifndef _SEP0611_FB_IOCTL_H
#define _SEP0611_FB_IOCTL_H

#include <linux/types.h>

#define SEPFB_IOCTL_MAGIC	'f'

#define SEP_LCD_FB_IOCTL		        0x46FF

#define SEP0611FB_IOCTL_SET_HDMI_TIMING					0x26
//#define SEP_LCD_BL_SET								0x0100

typedef struct 
{
	int width;
	int height;
}sep_hdmisize;

struct lcdc_timing_params_t {

	unsigned int pcd;

	unsigned int lpw;
	unsigned int lpc;
	unsigned int lswc;
	unsigned int lewc;
	
	unsigned int fpw;
	unsigned int flc;
	unsigned int fswc;
	unsigned int fewc;
};

#endif
