#ifndef _SEP0611_OVERLAY_H_
#define _SEP0611_OVERLAY_H_

/****************** regs for sep0611 overlay ***********************/
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

#define		bpp24_unpacked		0x02
#define		bpp18_unpacked		0x01
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

#endif /* SEP0611_OVERLAY_H_ */

