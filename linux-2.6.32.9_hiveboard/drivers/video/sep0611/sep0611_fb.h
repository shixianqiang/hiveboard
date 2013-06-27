#ifndef _DRIVERS_VIDEO_SEP0611_SEP0611FB_H
#define _DRIVERS_VIDEO_SEP0611_SEP0611FB_H

#include <linux/fb.h>
/**************comment by zyt**************
*	#include <linux/earlysuspend.h>
**************comment by zyt**************/
/*********************
   sep0611 lcd register
   more details see sep0611 
   datasheet
**********************/

#define SEP0611_LCDCECR		0X00
#define SEP0611_BECR		0X04
#define SEP0611_W1ECR		0X08
#define SEP0611_W2ECR		0X0C
#define SEP0611_CECR		0X10
#define SEP0611_LCDCBCR		0X14
#define SEP0611_PLCR		0X18
#define SEP0611_PFCR		0X1C
#define SEP0611_PCCR		0X20
#define SEP0611_IOCR		0X24
#define SEP0611_IECR		0X28
#define SEP0611_W1CKR		0X2C
#define SEP0611_W2CKR		0X30	
#define SEP0611_W1BVR		0X34
#define SEP0611_W2BVR		0X38
#define SEP0611_LCDCSCR		0X3C
#define SEP0611_W1TPCR		0X40
#define SEP0611_W1BPCR		0X44
#define SEP0611_W2TPCR		0X48
#define SEP0611_W2BPCR		0X4C
#define SEP0611_CTPCR		0X50
#define SEP0611_CBPCR		0X54
#define SEP0611_ACSR		0X58
#define SEP0611_BBSAR		0X5C
#define SEP0611_W1YBSAR		0X60
#define SEP0611_W1UBSAR		0X64
#define SEP0611_W1VBSAR		0X68
#define SEP0611_W2BSAR		0X6C
#define SEP0611_CBSAR		0X70

//#define SEP0611_BBEAR		0X74
//#define SEP0611_W1YBEAR	0X78
//#define SEP0611_W1UBEAR	0X7C
//#define SEP0611_W1VBEAR	0X80
//#define SEP0611_W2BEAR	0X84
//#define SEP0611_CBEAR		0X88

#define SEP0611_CC0R		0X8C
#define SEP0611_CC1R		0X90
#define SEP0611_CC2R		0X94
#define SEP0611_CC3R		0X98
#define SEP0611_BASE_RAW_IMAGE_WIDTH		0x09c
#define SEP0611_OVER1_RAW_IMAGE_WIDTH		0x0A0
#define SEP0611_OVER2_RAW_IMAGE_WIDTH		0x0A4
//#define SEP0611_DIS_IMAGE_WIDTH_0		0x0A8
//#define SEP0611_DIS_IMAGE_WIDTH_1		0x0AC
//#define SEP0611_DIS_IMAGE_WIDTH_2		0x0B0
#define SEP0611_BTPCR				0x0B4
#define SEP0611_BBPCR				0x0B8
#define SEP0611_BAKGRND_COLOR			0XBC
#define SEP0611_R_ORDER				0XC0
#define SEP0611_G_ORDER				0XC4
#define SEP0611_B_ORDER				0XC8
#define SEP0611_RGB_ORDER			0XCC

/* macro for LCDCECR,BECR,W1ECR,W2ECR,CECR */
#define	WIN_ENABLE	0x1
#define	WIN_DISABLE	0x0

/* macro for LCDC basic control register(LCDCBCR) */
#define		RGB_or_YUV(x)			(x << 31)
#define		YUV_format_OVERLAY1(x)		(x << 29)
#define		BPIX_BASE(x)			(x << 27)
#define		BPIX_OVLAY1(x)			(x << 25)
#define		BPIX_OVLAY2(x)			(x << 23)
#define		BPIX_CURSOR(x)			(x << 21)
#define		RGB_expansion_BASE(x)		(x << 19)
#define		RGB_expansion_OVLAY1(x)		(x << 17)
#define		RGB_expansion_OVLAY2(x)		(x << 15)
#define		UNDER_ERROR_MODE(x)		(x << 13)
#define		PIXPOL(x)			(x << 12)
#define		FLMPOL(x)			(x << 11)
#define		LPPOL(x)			(x << 10)
#define		CLKPOL(x)			(x << 9)
#define		OEPOL(x)			(x << 8)
#define		PCD(x)				(x << 0)

/* macro for panel line control register(PLCR)*/
#define		H_WIDTH(x)			(x << 26)
#define		H_WAIT1(x)			(x << 8)
#define		H_WAIT2(x)			(x << 0)
/* macro for panel frame control register(PFCR)*/
#define		V_WIDTH(x)			(x << 26)
#define		V_WAIT1(x)			(x << 8)
#define		V_WAIT2(x)			(x << 0)
/*macro for panel contrast control register(PCCR)*/
#define		SCR(x)				(x << 9)
#define		CC_EN(x)			(x << 8)
#define		PW(x)				(x << 0)
/*macro for interrupt original state register(IOSR)*/
#define		UDR_ERR				(1 << 3)
#define		ERR_RES				(1 << 2)
#define		EOF				(1 << 1)
#define		BOF				(1 << 0)
/* macro for interrupt enable control register(IECR)*/
#define		UDR_ERR_EN			(1 << 3)
#define		ERR_RES_EN			(1 << 2)
#define		EOF_EN				(1 << 1)
#define		BOF_EN				(1 << 0)


/* macro for	LCDC size control register(LCDCSCR) */
#define		XMAX(x)				(x << 16)
#define		YMAX(x)				(x << 0)

/* macro for lcdc position register(four layers), left top and size*/
#define		LEFT_TOP(x,y)			((x & 0x7ff) << 16 | (y & 0x7ff) << 0)
#define		RIGHT_BOTTOM(x,y)		((x & 0x7ff) << 16 | (y & 0x7ff) << 0)

/* macro for address configuration status register(ACSR)*/
#define 	ADDRESS_NOT_STABLE	0X0
#define		ADDRESS_STABLE		0X1

/* macro for .... and so on, fixed later*/


#define		Full_Screen		0x0
#define		Part_Screen		0x1

#define		RGB			0x0
#define		YUV			0x1

#define		bpp32		0x02
#define		bpp16			0x00

#define		YUV420			0x00
#define		YUV422			0x01
#define		YUV444			0x02
#define		YUV420_semi		0x03

#define		bpp1_a			0x00
#define		bpp1_b			0x01
#define		bpp2_a			0x10
#define		bpp2_b			0x11

#define		UNDER_NONE		0X00
#define		UNDER_FRAME_RESET	0X01   
#define		UNDER_WAIT		0X02

#define		ALPHA_USR		0x00
#define		ALPHA_ARGB		0x01

	
#define	SEP0611_MAX_ALPHA_LEVEL		0x0f

//ioctl selection
#define	SEP0611FB_IOCTL_LAYERS_SELECT				0X10
#define	SEP0611FB_IOCTL_BACKLIGHT_ON_OFF			0X11
#define	SEP0611FB_IOCTL_BRIGHTNESS_ON_OFF			0X12
#define	SEP0611FB_IOCTL_SET_BRIGHTNESS_MODE			0X13
#define	SEP0611FB_IOCTL_SET_BRIGHTNESS_VALUE			0X14
#define	SEP0611FB_IOCTL_LAYERS_ON_OFF				0X15
#define	SEP0611FB_IOCTL_SET_FRAMEBUFFER_OFFSET			0X16
#define	SEP0611FB_IOCTL_SET_ALPHA_MODE				0x17
#define	SEP0611FB_IOCTL_SET_ALPHA_USR_VALUE			0X18
#define	SEP0611FB_IOCTL_SET_ALPHA_ARGB_VALUE			0X19
#define	SEP0611FB_IOCTL_SET_ALPHA_USR_VALUE_UP_DOWN		0X1A
#define	SEP0611FB_IOCTL_COLORKEY_ON_OFF				0X1B
#define	SEP0611FB_IOCTL_SET_COLORKEY_VALUE			0X1C
#define	SEP0611FB_IOCTL_YUV_ON_OFF				0X1D
#define	SEP0611FB_IOCTL_SET_YUV_MODE				0X1E
#define	SEP0611FB_IOCTL_SET_YUV_OFFSET_ADDRESS			0X1F
#define	SEP0611FB_IOCTL_SET_SCREEN_COORDINATE			0X20
#define	SEP0611FB_IOCTL_GET_SCREEN_COORDINATE			0X21
#define	SEP0611FB_IOCTL_SET_CURSOR_MODE				0X22
#define	SEP0611FB_IOCTL_SET_CURSOR_PAL				0X23
#define	SEP0611FB_IOCTL_SET_RAW_IMAGE_ATTRIBUTES		0X24
#define	SEP0611FB_IOCTL_GET_RAW_IMAGE_ATTRIBUTES		0X25

enum layers_select_t {
	layer0 = 0,
	layer1 = 1,
	layer2 = 2,
	layer3 = 3,
} ; 

struct layer1_yuv_t {
	unsigned int	yuv_format;
	unsigned int	y_size;
	unsigned int	u_size;
	unsigned int	v_size;
	unsigned int	y_start;
	unsigned int	u_start;
	unsigned int	v_start;
};

typedef struct {
	struct fb_bitfield red;
	struct fb_bitfield green;
	struct fb_bitfield blue;
	struct fb_bitfield transp;
} sep0611fb_rgb_t;

//including sep0611 lcd register 
struct sep0611fb_hw
{
	/*LCDC and four layers enable register*/
	unsigned long lcdcecr;
	unsigned long becr;
	unsigned long w1ecr;
	unsigned long w2ecr;
	unsigned long cecr;
	/*configuration register*/
	unsigned long lcdcbcr;
	unsigned long plcr;
	unsigned long pfcr;
	unsigned long pccr;
	unsigned long iocr;
	unsigned long iecr;
	/*color-key,alpha-blending*/	
	unsigned long w1ckr;
	unsigned long w2ckr;
	unsigned long w1bvr;
	unsigned long w2bvr;
	unsigned long bakgrnd_color;
	unsigned long r_order;
	unsigned long g_order;
	unsigned long b_order;
	unsigned long rgb_order;
	/*size*/
	unsigned long acsr;
	unsigned long lcdcscr;
 	unsigned long btpcr;
	unsigned long bbpcr;
	unsigned long w1tpcr;
	unsigned long w1bpcr;
	unsigned long w2tpcr;
	unsigned long w2bpcr;
	unsigned long ctpcr;
	unsigned long cbpcr;
	unsigned long raw_image_width_0;
	unsigned long raw_image_width_1;
	unsigned long raw_image_width_2;
	/*feach layer's buffer start address*/
	unsigned long bbsar;
	unsigned long w1ybsar;
	unsigned long w1ubsar;
	unsigned long w1vbsar;
	unsigned long w2bsar;
	unsigned long cbsar;

	/*cusor palette*/
	unsigned long cc0r;
	unsigned long cc1r;
	unsigned long cc2r;
	unsigned long cc3r;
	
};

struct layer_coordinate_t {
	unsigned int	coordinate_x;
	unsigned int	coordinate_y;
	unsigned int	size_x;
	unsigned int	size_y;
};

//including sep0611 each overlays'configure information
struct sep0611fb_layer {
	struct fb_info	*fb;
	unsigned int	layer_id;

	struct sep0611fb_info	*parent;

	unsigned int	colorkey_enable;
	unsigned int	colorkey_value;

	unsigned int	alpha_mode;
	unsigned int	alpha_level;
	unsigned int	argb_value;

	unsigned int	yuv_enable;
	unsigned int	yuv_mode;
	struct layer1_yuv_t	layer1_yuv;
	unsigned int	cursor_mode;

	struct layer_coordinate_t	layer_coordinate[CONFIG_FB_SEP0611_NUM];
	//struct layer_coordinate_t layer_coordinate[4];

	unsigned int	max_bpp;
	unsigned int	max_xres;
	unsigned int	max_yres;

	/* raw memory addresses */
	dma_addr_t	map_dma;	/* physical */
	u_char *	map_cpu;	/* virtual */
	unsigned int	map_size;
	/* addresses of pieces placed in raw buffer */
	dma_addr_t	screen_dma;	/* physical address of frame buffer */
	u_char *	screen_cpu;	/* virtual address of frame buffer */

	unsigned int	lcd_offset_x;
	unsigned int	lcd_offset_y;

	unsigned int	pseudo_pal[16];
	unsigned int	palette_buffer[256];
};

struct sep0611fb_info {
	struct device	*dev;
	struct clk	*clk;
	unsigned int clkrate;
	struct resource	*mem;
	void __iomem	*io;

	struct sep0611fb_layer	*sep_layers[CONFIG_FB_SEP0611_NUM];

#ifdef CONFIG_HAS_EARLYSUSPEND
	struct early_suspend fb_early_suspend_info;
#endif
};

#define PALETTE_BUFF_CLEAR	(0x80000000) /*entry is clear/invalid*/

struct sep0611_lcdc_info_t {
	/* Screen size */
	int width;
	int height;

	/* Screen info */
	int xres;
	int yres;

	/* Virtual Screen info */
	int xres_virtual;
	int yres_virtual;
	int xoffset;
	int yoffset;

	/* OSD Screen size (overlay 1)*/
	int osd_width;
	int osd_height;

	/* OSD Screen info */
	int osd_xres;
	int osd_yres;

	/* OSD Screen info */
	int osd_xres_virtual;
	int osd_yres_virtual;

	int layer_bpp[4];
	int yuv_mode;
	//int layer_bytes_per_pixel[4];
	unsigned long pixclock;

	
	/* lcd configuration registers */
	struct sep0611fb_hw regs;
	
	int hsync_len;
	int left_margin;
	int right_margin;
	int vsync_len;
	int upper_margin;
	int lower_margin;
	int sync;

	int cmap_grayscale:1;
	int cmap_inverse:1;
	int cmap_static:1;
	int unused:29;

	/* backlight info */
	int backlight_min;
	int backlight_max;
	int backlight_default;

	int vs_offset;
	int brightness;
	int palette_win;
	int backlight_level;
	int backlight_power;
	int lcd_power;
};


/********************************
*  Extern structure and Functions
*********************************/

extern struct sep0611_lcdc_info_t sep0611_lcdc_info;
void sep0611fb_init_hw(struct sep0611fb_info *fbi);
void sep0611_enable_backlight(int);
int sep0611fb_set_backlight_level(int value);

#endif 
/*_DRIVERS_VIDEO_SEP0611_SEP0611FB_H*/


