/*********************************************************************
* Filename    :  LCD.h
* Description :  
* Date        :  2010-03-22
* Author      :  SixRoom
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//			RGB图片存放说明
//					0x40100000 : 800*480 24bpp
//					0x40300000 : 800*480 18bpp
//					0x40500000 : 800*480 16bpp
//
//					0x41100000 : 640*480 24bpp
//					0x41200000 : 640*480 18bpp
//					0x41300000 : 640*480 16bpp
//
//					0x42100000 : 480*320 24bpp
//					0x42200000 : 480*320 18bpp
//					0x42300000 : 480*320 16bpp
//
//					0x43100000 : 320*240 24bpp
//					0x43200000 : 320*240 18bpp
//					0x43300000 : 320*240 16bpp
//
//					0x48000000 : 字体
//			YUV视频存放说明
//					0x40700000	608*320*2 YUV420
//					0x40900000	608*320*2 YUV422
//					0x40B00000	608*320*2 YUV444
//					0x40D00000	608*320*2 SemiYUV420
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
*********************************************************************/
#ifndef	__LCDC_H_
#define __LCDC_H_

#include <asm/arch/vic.h>

#define    BAKGRND_COLOR    (LCDC_BASE + 0x0BC)
#define    R_ORDER			(LCDC_BASE + 0x0C0)
#define    G_ORDER			(LCDC_BASE + 0x0C4)
#define    B_ORDER			(LCDC_BASE + 0x0C8)
#define    RGB_ORDER		(LCDC_BASE + 0x0CC)

#define		BPP_16				(0)	
#define		BPP_18				(1)
#define		BPP_24  			(2)

#define		BPP_1A          	(0)
#define		BPP_1B				(1)
#define		BPP_2A				(2)
#define		BPP_2B				(3)

#define		YUV_420				(0)
#define		YUV_422				(1)
#define		YUV_444				(2)
#define		SEMI_YUV			(3)

#define 	BUSY				(1)
#define 	IDLE				(0)

#define		OVERLAY_RGB			(0 << 31)
#define		OVERLAY_YUV			(1 << 31)

//自动修复功能配置参数
#define 	AUTO_REPAIR_BIT(x)	(x)

//Macro for Panel contrast control register(LCDC_PCCR)
#define		SCR				(1 << 9)
#define		CC_EN			(1 << 8)

//Color_Key 配置参数
#define 	ColorKey_EN		(1<<24)
#define 	ColorKey_DIS	(0<<24)

//alphableeding 配置参数
#define 	Alpha_EN		(0<<24)
#define 	Alpha(x)		(x)

#define 	ARGB_EN			(1<<24)
#define 	ARGB_Vld_H		(0<<16)	//ARGB高四位数据有效
#define 	ARGB_Vld_L		(1<<16) //ARGB低四位数据有效
#define 	ARGB_FORMAT(x)	((x)<<8)

//------------------------------------------------------------
//base
	#define BASE_IMAGE_PART_1			1
	#define BASE_IMAGE_PART_2			1
	#define BASE_IMAGE_PART_3			1
	#define BASE_IMAGE_PART_4			1
	#define BASE_IMAGE_PART_5			1
//------------------------------------------------------------
//overlay1
	#define OVERLAY1_IMAGE_PART_1		1
	#define OVERLAY1_IMAGE_PART_2		1
	#define OVERLAY1_IMAGE_PART_3		1
	#define OVERLAY1_IMAGE_PART_4		1
	#define OVERLAY1_IMAGE_PART_5		1
//------------------------------------------------------------
//overlay2
	#define OVERLAY2_IMAGE_PART_1		1
	#define OVERLAY2_IMAGE_PART_2		1
	#define OVERLAY2_IMAGE_PART_3		1
	#define OVERLAY2_IMAGE_PART_4		1
	#define OVERLAY2_IMAGE_PART_5		1
////------------------------------------------------------------
//cursor
//------------------------------------------------------------

#define LCDC_OT_LAYER		LAYER0
#define LCDC_OT_HSIZE		800
#define LCDC_OT_VSIZE		480
#define LCDC_OT_ReFreshRate	40
#define LCDC_OT_POORLIGHT	100
#define LCDC_OT_PIXPOL		0
#define LCDC_OT_FLMPOL		1
#define LCDC_OT_LPPOL		1
#define LCDC_OT_CLKPOL		0
#define LCDC_OT_OEPOL		0
#define LCDC_OT_HWIDTH		48
#define LCDC_OT_HWAIT1		40
#define LCDC_OT_HWAIT2		40
#define LCDC_OT_VWIDTH		3
#define LCDC_OT_VWAIT1		29
#define LCDC_OT_VWAIT2		13
#define LCDC_OT_PCD			4

extern void LCDC_Test(void);


#endif
