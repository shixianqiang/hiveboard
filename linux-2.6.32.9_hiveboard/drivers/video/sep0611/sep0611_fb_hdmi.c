/*kernel\drivers\video\sep0611\sep_fb_hdmi.c
 *   This file will operate the lcdc registers
 *   when hdmi starts to change the resolution
 * Anthor : lixiang  
 * Version: 1.0.1 2011-7-6
 */
#include <linux/delay.h>
#include <mach/hardware.h>
#include <asm/io.h>
#include "sep0611_fb_hdmi.h"
#include "sep0611_fb.h"
///*****************************************************************************
//*
//* Defines
//*
//******************************************************************************/
///* macro for panel line control register(PLCR)*/
//#define		H_WIDTH(x)			(x << 26)
//#define		H_WAIT1(x)			(x << 8)
//#define		H_WAIT2(x)			(x << 0)
///* macro for panel frame control register(PFCR)*/
//#define		V_WIDTH(x)			(x << 26)
//#define		V_WAIT1(x)			(x << 8)
//#define		V_WAIT2(x)			(x << 0)
///*macro for panel contrast control register(PCCR)*/
//#define		SCR(x)				(x << 9)
//#define		CC_EN(x)			(x << 8)
//#define		PW(x)				(x << 0)
///* macro for	LCDC size control register(LCDCSCR) */
//#define		XMAX(x)				(x << 16)
//#define		YMAX(x)				(x << 0)
//#define		PCD(x)				(x << 0)

/*  Config lcdc timimg when hdmi chooses a resolution
 *  params: mode[lcdc_timimg_parms_t] lcdc timimg to 
 *          set
 *  return: null
 */
void sep0611_hdmi_lcdc_timing(struct lcdc_timing_params_t *mode)
{
    unsigned int plcr = 0;
	unsigned int pfcr = 0;
	unsigned int lcdcscr = 0;
	unsigned int lcdcbcr = 0;
	
	plcr = H_WIDTH(mode->lpw) | H_WAIT1((mode->lswc + 1)) | H_WAIT2((mode->lewc + 1));
	pfcr = V_WIDTH(mode->fpw) | V_WAIT1((mode->fswc + 1)) | V_WAIT2((mode->fewc + 1));
	
	lcdcscr = XMAX((mode->lpc + 1))|YMAX((mode->flc + 1));
	lcdcbcr = readl(LCDC_BCR_V);
	lcdcbcr &= ~0x3f;
	lcdcbcr |= PCD(mode->pcd);
	
	writel(0x0, LCDC_ECR_V);
	
	msleep(20);
	
	writel(lcdcbcr, LCDC_BCR_V);
	writel(plcr, LCDC_PLCR_V);
	writel(pfcr, LCDC_PFCR_V);
	writel(lcdcscr, LCDC_SCR_V);
	
	msleep(20);
	
	writel(0x1, LCDC_ECR_V);
	
	msleep(10);
}
