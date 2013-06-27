/************************************************
 * sep0611_overlay drivers
 * Author:	fuzhuo <fuzhuo233@gmail.com>
 * Data:	2011/6/23
 * Path:	drivers/video/sep0611_overlay.c
 ************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
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
#include <linux/videodev.h>            
#include <linux/miscdevice.h>
#include <linux/poll.h>
#include <linux/timer.h>

#include <mach/hardware.h>
#include <mach/sep0611overlay_ioctrl.h>

#include "sep0611_overlay.h"

#define debug 0
#define dprintk(msg...) if (debug) { printk( "sep0611_overlay: " msg); }
#define DEVICE_NAME "overlay"
#define DEV_MINOR	202
#define MINOR_ID	1

#define TEST 0
#define USE_PICTURE 1
typedef struct 
{
	unsigned int width;
	unsigned int height;
	unsigned int pcd;
	unsigned int h_width;
	unsigned int h_wait1;
	unsigned int h_wait2;
	unsigned int v_width;
	unsigned int v_wait1;
	unsigned int v_wait2;
} lcdc_timming;

static overlay_config_t overlay_cfg;
static struct v4l2_format overlay_fmt;
//static int overlay_forbid;
static int overlay_use = 0;// recorde times of overlay be used
static int overlay_en_count = 0;
void __iomem *io;
void __iomem *io_pmu;//io base for pmu

/*******  just for test *********/
dma_addr_t over1_addr_phy;
unsigned int * over1_addr_vir;

dma_addr_t cursor_addr_phy;
unsigned int * cursor_addr_vir;
struct timer_list cursor_timer;
/********************************/
static int sep0611_overlay_drawcursor(void);

static char sep0611_overlay_use(void)
{
	return overlay_use;
}
EXPORT_SYMBOL(sep0611_overlay_use);

static void sep0611_common_enable(void)
{
	unsigned int data;
	// enable alpha blending
	data = readl(io + SEP0611_W1BVR);
	data &= ~(1<<24);//alpha enable
	data &=~(0xf<<0);
	data |=12;//set alpha value(0~15)
	writel(data, io + SEP0611_W1BVR);
	// enable color key
	data = readl(io + SEP0611_W1CKR);
	data &= ~(1<<24);
	data |= (1<<24);//enable c-k
	data &=~(0xffffff);// use black as color key
	writel(data, io + SEP0611_W1CKR);
	data = readl(io + SEP0611_W1CKR);
	dprintk("after set colorkey data=0x%x\n",data);

}
EXPORT_SYMBOL(sep0611_common_enable);

static void sep0611_common_disable(void)
{
	unsigned int data;
	dprintk("common disable\n");
	// disable alpha blending
	data = readl(io + SEP0611_W1BVR);
	//data = 0;
	data &=~(1<<24);
	data |= (0xf);// set no alpha
	writel(data, io + SEP0611_W1BVR);
	// disable alpha blending
	data = readl(io + SEP0611_W1CKR);
	data &= ~(1<<24);//disable color key
	writel(data, io + SEP0611_W1CKR);
}
EXPORT_SYMBOL(sep0611_common_disable);

static int sep0611_overlay_q_buffer(unsigned int arg)
{
	unsigned int curY_phyAddr, curU_phyAddr=0, curV_phyAddr=0;
	curY_phyAddr = arg;
	dprintk("go to %s\n", __func__);
	// fuzhuo add to disable alpha and C-K
	if(overlay_en_count!=0) {sep0611_common_disable();overlay_en_count=0;}
	//cal the YUV addr
	if(!(overlay_fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_RGB565 || overlay_fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_RGB24))
	{
		switch(overlay_fmt.fmt.pix.pixelformat)
		{
			case V4L2_PIX_FMT_RGB565:
				dprintk("q_buffer:RGB565 addr=0x%x\n",curY_phyAddr);
				break;
			case V4L2_PIX_FMT_RGB24:
				dprintk("q_buffer:RGB8888 addr=0x%x\n",curY_phyAddr);
				break;
			case V4L2_PIX_FMT_YUV444:
			   	curU_phyAddr = curY_phyAddr + overlay_cfg.width*overlay_cfg.height;
		   		curV_phyAddr = curU_phyAddr + overlay_cfg.width*overlay_cfg.height;
				dprintk("q_buffer: YUV444, addr=0x%x,0x%x,0x%x\n",curY_phyAddr,curU_phyAddr,curV_phyAddr);
		 		break;
			case V4L2_PIX_FMT_YUV422P:
				curU_phyAddr = curY_phyAddr + overlay_cfg.width*overlay_cfg.height;
				curV_phyAddr = curU_phyAddr + overlay_cfg.width*overlay_cfg.height/2;	
				dprintk("q_buffer: YUV422p, addr=0x%x,0x%x,0x%x\n",curY_phyAddr,curU_phyAddr,curV_phyAddr);
				break;
			case V4L2_PIX_FMT_YUV420:
			   	curU_phyAddr = curY_phyAddr + overlay_cfg.width*overlay_cfg.height;
				curV_phyAddr = curU_phyAddr + overlay_cfg.width*overlay_cfg.height/4;
				dprintk("q_buffer: YUV420, addr=0x%x,0x%x,0x%x\n",curY_phyAddr,curU_phyAddr,curV_phyAddr);
				break;
			case V4L2_PIX_FMT_NV12:
			   	curU_phyAddr = curY_phyAddr + overlay_cfg.width*overlay_cfg.height;
				//curV_phyAddr = curU_phyAddr + overlay_cfg.width*overlay_cfg.height/4;
				curV_phyAddr = curU_phyAddr;
				dprintk("q_buffer: YUV420-semi, addr=0x%x,0x%x,0x%x\n",curY_phyAddr,curU_phyAddr,curV_phyAddr);
				break;
			default:
				dprintk("unsupport YUV format\n");
				break;
		}
	}
	// write regs with RGB or YUV framebuffer start address
	if(overlay_fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_RGB565 || overlay_fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_RGB24)
	{
		dprintk("q_buffer: set RGB addr\n");
		writel(0, io + SEP0611_ACSR);
		writel(curY_phyAddr, io + SEP0611_W1YBSAR);
		writel(1, io + SEP0611_ACSR);
	}
	else // format is YUV not RGB
	{
		writel(0, io + SEP0611_ACSR);
		writel(curY_phyAddr, io + SEP0611_W1YBSAR);
		writel(curU_phyAddr, io + SEP0611_W1UBSAR);
		writel(curV_phyAddr, io + SEP0611_W1VBSAR);
		writel(1, io + SEP0611_ACSR);
	}
	return 0;
} 

static void sep0611_overlay_fmt_set(unsigned int fmt)
{
	//TODO set fmt and write regs
	unsigned int data;
	overlay_fmt.fmt.pix.pixelformat = fmt;/* be careful this format is int */
	overlay_cfg.format = fmt;
	if(fmt == V4L2_PIX_FMT_RGB565)
	{
		dprintk("color mode = rgb565\n");
		//set use rgb
		data = readl(io + SEP0611_LCDCBCR);
		data &= ~(RGB_or_YUV(1));
	   	data |= RGB_or_YUV(0);	
		writel(data, io + SEP0611_LCDCBCR);
		//set rgb565_16bpp
		data = readl(io + SEP0611_LCDCBCR);
		data &= ~(BPIX_OVLAY1(0x3));
		data |= BPIX_OVLAY1(0x0);//rgb 16bpp
		writel(data, io + SEP0611_LCDCBCR);
	}
	else if(fmt == V4L2_PIX_FMT_RGB24)
	{
		dprintk("color mode = RGB888\n");
		//set use rgb
		data = readl(io + SEP0611_LCDCBCR);
		data &= ~(RGB_or_YUV(1));
		data |= RGB_or_YUV(0);
		writel(data, io + SEP0611_LCDCBCR);
		//set rgb888	
		data = readl(io + SEP0611_LCDCBCR);
		data &= ~(BPIX_OVLAY1(0x3));
		data |= BPIX_OVLAY1(0x2);//rgb888 24bpp
		writel(data, io + SEP0611_LCDCBCR);
	}
	else if(fmt == V4L2_PIX_FMT_YUV420)
	{
		dprintk("color mode = YUV420\n");
		//set use yuv 
		data = readl(io + SEP0611_LCDCBCR);
		data &= ~(RGB_or_YUV(1));
		data |= RGB_or_YUV(1);
		writel(data, io + SEP0611_LCDCBCR);
		//set yuv420	
		data = readl(io + SEP0611_LCDCBCR);
		data &= ~(YUV_format_OVERLAY1(0x3));
		data |= YUV_format_OVERLAY1(0x0);//yuv420
		writel(data, io + SEP0611_LCDCBCR);
	}
	else if(fmt == V4L2_PIX_FMT_YUV422P)
	{
		dprintk("color mode = YUV422p\n");
		//set use yuv 
		data = readl(io + SEP0611_LCDCBCR);
		data &= ~(RGB_or_YUV(1));
		data |= RGB_or_YUV(1);
		writel(data, io + SEP0611_LCDCBCR);
		//set yuv422p
		data = readl(io + SEP0611_LCDCBCR);
		data &= ~(YUV_format_OVERLAY1(0x3));
		data |= YUV_format_OVERLAY1(0x1);//yuv422p
		writel(data, io + SEP0611_LCDCBCR);
	}
	else if(fmt == V4L2_PIX_FMT_YUV444)
	{
		dprintk("color mode = YUV444\n");
		//set use yuv 
		data = readl(io + SEP0611_LCDCBCR);
		data &= ~(RGB_or_YUV(1));
		data |= RGB_or_YUV(1);
		writel(data, io + SEP0611_LCDCBCR);
		//set yuv444	
		data = readl(io + SEP0611_LCDCBCR);
		data &= ~(YUV_format_OVERLAY1(0x3));
		data |= YUV_format_OVERLAY1(0x2);//yuv444
		writel(data, io + SEP0611_LCDCBCR);
	}
	else if(fmt == V4L2_PIX_FMT_NV12)
	{
		dprintk("color mode = YUV420semi\n")
		//set use yuv 
		data = readl(io + SEP0611_LCDCBCR);
		data &= ~(RGB_or_YUV(1));
		data |= RGB_or_YUV(1);
		writel(data, io + SEP0611_LCDCBCR);
		//set yuv420-semi
		data = readl(io + SEP0611_LCDCBCR);
		data &= ~(YUV_format_OVERLAY1(0x3));
		data |= YUV_format_OVERLAY1(0x3);//yuv420-semi
		writel(data, io + SEP0611_LCDCBCR);
	}
	sep0611_overlay_q_buffer(over1_addr_phy);
}


static int sep0611_overlay_get_screenInfo(overlay_config_t * arg)
{
	unsigned int data,width_t,height_t;
	overlay_config_t tmp;
	data = readl(io + SEP0611_LCDCSCR);// read size from regs
	width_t = data>>16;
	height_t = data & 0xFFFF;
	tmp.sx = 0; tmp.sy = 0;
	tmp.width = width_t; tmp.height = height_t;
	if(copy_to_user((overlay_config_t *)arg, &tmp, sizeof(overlay_config_t)))
		return -EFAULT;
	return 0;
}

static int sep0611_overlay_set_size(overlay_config_t arg)
{
	overlay_config_t tmp=arg;
	// set size
	overlay_cfg.width = arg.width;	overlay_cfg.height = tmp.height;

	overlay_fmt.fmt.pix.width = tmp.width;
	overlay_fmt.fmt.pix.height = tmp.height;
	// raw img width
	writel(overlay_cfg.width, io + SEP0611_OVER1_RAW_IMAGE_WIDTH);
	// size
	writel(RIGHT_BOTTOM(overlay_cfg.width,overlay_cfg.height), io + SEP0611_W1BPCR);
	return 0;
}

static int sep0611_overlay_set_pos(overlay_config_t arg)
{
	overlay_config_t tmp=arg;
	unsigned int data,width_t,height_t;
	data = readl(io + SEP0611_LCDCSCR);// read size from regs
	width_t = data>>16;
	height_t = data & 0xFFFF;

	if(tmp.sx + tmp.width > width_t)
	{
		if(tmp.width > width_t){
			tmp.sx = 0;
			//tmp.width = width_t;
		}else{
			tmp.sx = (width_t - tmp.width)/2;
		}
	}
	if(tmp.sy + tmp.height > height_t)
	{
		if(tmp.height > height_t){
			tmp.sy = 0;
			//tmp.height = height_t;
		}else{
			tmp.sy = (height_t - tmp.height)/2;
		}
	}
	overlay_cfg.sx = tmp.sx;	overlay_cfg.sy = tmp.sy;
	overlay_cfg.width = tmp.width; overlay_cfg.height = tmp.height;

	dprintk("set post(%d,%d,%d,%d, format=%d)\n",overlay_cfg.sx,overlay_cfg.sy,overlay_cfg.width,overlay_cfg.height,overlay_cfg.format);
	/**** write regs ****/
	// top left
	writel(LEFT_TOP(overlay_cfg.sx,overlay_cfg.sy), io + SEP0611_W1TPCR);
	/****** set size and color fmt *******/
	sep0611_overlay_set_size(tmp);
	sep0611_overlay_fmt_set(overlay_fmt.fmt.pix.pixelformat);
	return 0;
}

static int sep0611_overlay_get_pos(overlay_config_t * arg)
{
	overlay_config_t tmp;
	dprintk("go to %s\n", __func__);
	tmp.sx = overlay_cfg.sx; tmp.sy = overlay_cfg.sy;
	tmp.width = overlay_cfg.width; tmp.height = overlay_cfg.height;
	if(copy_to_user((overlay_config_t *)arg, &tmp, sizeof(overlay_config_t)))
		return -EFAULT;
	return 0;
}

static int sep0611_overlay_set_configure(overlay_config_t arg)
{
	dprintk("go to %s\n", __func__);
	dprintk("args:(%d,%d,%d,%d,%d)\n",arg.sx, arg.sy, arg.width, arg.height, arg.format);

	//TODO set the fmt
	sep0611_overlay_fmt_set(arg.format);	
	// set position
	sep0611_overlay_set_pos(arg);
	// set size and raw img width
	//sep0611_overlay_set_size(arg); /* this already do ing sep0611_overlay_set_pos() */
	return 0;
}

static int sep0611_overlay_disable(void)
{
	dprintk("go to %s\n", __func__);
	writel(WIN_DISABLE, io + SEP0611_W1ECR); 
	return 0;
}

static int sep0611_overlay_enable(void)
{
	dprintk("go to %s\n", __func__);
	writel(WIN_ENABLE, io + SEP0611_W1ECR);
	return 0;
}

static int sep0611_switchto_mode(unsigned int arg)
{
	unsigned int data,width,height,sx,sy;
	switch(arg)
	{
		case 0://to video mode
			dprintk("switch to video mode\n");
			//1.enable overlay2
			writel(WIN_ENABLE, io + SEP0611_W2ECR);
			//set overlay2's pos to zero
			writel(LEFT_TOP(0,0), io + SEP0611_W2TPCR);	
			//2.disable base
			writel(WIN_DISABLE, io + SEP0611_BECR);
			//3.disable cursor
			writel(WIN_DISABLE, io + SEP0611_CECR);
			break;
		case 1://to HDMI mode
			dprintk("switch to HDMI mode\n");
			//1.open cursor
			sep0611_overlay_drawcursor();
			writel(WIN_ENABLE, io + SEP0611_CECR);
			//2.set the pose of overlay2
			data = readl(io + SEP0611_LCDCSCR);
			width = data>>16;
			height = data & 0xFFFF;
			sx = (width-800)/2;
			sy = (height-480);
			if(sx<0 || sy<0)
			{
				// is no need to draw the cursor and overlay2
				// so disable overlay2 and cursor
				writel(WIN_DISABLE, io + SEP0611_W2ECR);
				writel(WIN_DISABLE, io + SEP0611_CECR);
				sx=0;sy=0;
			}else{
				//centre the overlay2 to the bottom
				writel(LEFT_TOP(sx,sy), io + SEP0611_W2TPCR);	
			}
			del_timer(&cursor_timer);
			cursor_timer.expires = jiffies + 100*3;
			add_timer(&cursor_timer);
			break;
		case 2://to LCD mode
			dprintk("swidth to LCD mode\n");
			//1.disable overlay2
			writel(WIN_DISABLE, io + SEP0611_W2ECR);
			//2.enable base
			writel(WIN_ENABLE, io + SEP0611_BECR);
			break;
		default:
			break;
	}
	return 0;
}

static int sep0611_overlay_layers_ctl(unsigned int intArg)
{
	unsigned int data;
	if(intArg & 0x1<<4){//enable
		dprintk("enable layer ");
		if((intArg & 0x7) == 0x0){
			dprintk("base\n");
			writel(1, io + SEP0611_BECR);
		}
		else if((intArg & 0x7) == 0x1){
			dprintk("overlay1\n");
			writel(1, io + SEP0611_W1ECR);
		}
		else if((intArg & 0x7) == 0x2){
			dprintk("overlay2\n");
			writel(1, io + SEP0611_W2ECR);
		}
		else if((intArg & 0x7) == 0x3){
			dprintk("cursor\n");
			writel(1, io + SEP0611_CECR);
		}
		else if((intArg & 0x7) == 0x4){
			dprintk("overlay2's color-key\n");
			data = readl(io + SEP0611_W2CKR);
			data &= ~(1<<24);
			data |= (1<<24);//enable c-k
			data &=~(0xffffff);// use black as color key
			writel(data, io + SEP0611_W2CKR);
		}
		else if((intArg & 0x7) == 0x5){
			dprintk("overlay2's alpha\n");
			data = readl(io + SEP0611_W2BVR);
			data &= ~(1<<24);//alpha enable
			data &=~(0xf<<0);
			data |=12;//set alpha value(0~15)
			writel(data, io + SEP0611_W2BVR);
		}
	}else{//disable
		dprintk("disable layer ");
		if((intArg & 0x7) == 0x0){
			dprintk("base\n");
			writel(0, io + SEP0611_BECR);
		}
		else if((intArg & 0x7) == 0x1){
			dprintk("overlay1\n");
			writel(0, io + SEP0611_W1ECR);
		}
		else if((intArg & 0x7) == 0x2){
			dprintk("overlay2\n");
			writel(0, io + SEP0611_W2ECR);
		}
		else if((intArg & 0x7) == 0x3){
			dprintk("cursor\n");
			writel(0, io + SEP0611_CECR);
		}
		else if((intArg & 0x7) == 0x4){
			dprintk("overlay2's color-key\n");
			data = readl(io + SEP0611_W2CKR);
			data &= ~(1<<24);
			writel(data, io + SEP0611_W2CKR);
		}
		else if((intArg & 0x7) == 0x5){
			dprintk("overlay2's alpha\n");
			data = readl(io + SEP0611_W2BVR);
			data &= ~(1<<24);
			//data |= (1<<24);//alpha disable
			data |=0xf;
			writel(data, io + SEP0611_W2BVR);
		}
	}
	return 0;
}

static int sep0611_overlay_settimming(lcdc_timming timming)
{
	unsigned int data;
	dprintk("go to %s\n", __func__);

	//disable LCDC
	writel(WIN_DISABLE, io + SEP0611_LCDCECR);

#if 1
	if(timming.width == 9999)//switch PLL source
	{	
		data = readl(io_pmu + 0x2c);
		dprintk("pmu_0x2c = 0x%x\n", data);
		data = readl(io_pmu + 0x34);
		dprintk("pmu_0x34 = 0x%x\n", data);

		data = readl(io_pmu + 0x2c);
		data &= ~(0x3<<4);
		if(timming.height == 9999) ;//use MPLL
		else {data |= 0x1<<4; /*data|=0x1;*/}		//use APLL
		writel(data, io_pmu + 0x2c);//to use APLL 800MHz

		data = readl(io_pmu + 0x34);
		data &= ~(0x1<<3);
		data |= 0x1<<3;
		writel(data , io_pmu + 0x34);

		for(;;)
		{
			data = readl(io_pmu + 0x34);
			data &= 0x1<<3;
			dprintk("data = %d\n",data);
			if(data ==0) break;
			msleep(100);
		}
		dprintk("after write the regs\n");
		data = readl(io_pmu + 0x2c);
		dprintk("pmu_0x2c = 0x%x\n", data);
		data = readl(io_pmu + 0x34);
		dprintk("pmu_0x34 = 0x%x\n", data);
		dprintk("pmu config OK\n");
		writel(WIN_ENABLE, io + SEP0611_LCDCECR);
		return 0;
	}

#endif
	dprintk("lcd.t=%d, %d, %d, %d, %d, %d, %d, %d, %d\n",timming.width,timming.height,timming.pcd,timming.h_width,timming.h_wait1,timming.h_wait2,timming.v_width,timming.v_wait1,timming.v_wait2);

	msleep(10);
	//size
	data = 0;
	data |= XMAX(timming.width)|YMAX(timming.height);
	writel(data, io + SEP0611_LCDCSCR);

	// PCD
	data = readl(io + SEP0611_LCDCBCR);
	data &=~(0x1F);
	data |=timming.pcd;
	writel(data, io + SEP0611_LCDCBCR);

	// H timming
	data = 0;
	data = H_WIDTH(timming.h_width) | H_WAIT1(timming.h_wait1) | H_WAIT2(timming.h_wait2);
	writel(data, io + SEP0611_PLCR);

	// V timming
	data = 0;
	data = V_WIDTH(timming.v_width) | V_WAIT1(timming.v_wait1) | V_WAIT2(timming.v_wait2);
	writel(data, io + SEP0611_PFCR);

	msleep(10);
	//enable LCDC
	writel(WIN_ENABLE, io + SEP0611_LCDCECR);
	return 0;
}

static int sep0611_overlay_drawcursor(void)
{
	unsigned int data;
	unsigned int i;
	const unsigned int gImage[20]={
		0b11111111111111111111111111111001,
		0b11111111111111111111111111100101,
		0b11111111111111111111111111011101,
		0b11111111111111111111111100111101,
		0b11111111111111111111110011111101,
		0b11111111111111111111101111111101,
		0b11111111111111111111000111111101,
		0b11111111111111111111111001111101,
		0b11111111111111111111111110111101,
		0b11111111111111111111111110111101,
		0b11111111111111111111111101111101,
		0b11111111111111111111111101101101,
		0b11111111111111111111111011001101,
		0b11111111111111111111111010110101,
		0b11111111111111111111110110110101,
		0b11111111111111111111110101111001,
		0b11111111111111111111101101111101,
		0b11111111111111111111011011111111,
		0b11111111111111111111010111111111,
		0b11111111111111111111000111111111,
	};


	dprintk("goto %s", __func__);
	memset(cursor_addr_vir,0x0,32*20/4);//clear the buffer date
	

	data=LEFT_TOP(400,240);
	writel(data, io + SEP0611_CTPCR);//left top
	data=RIGHT_BOTTOM(32,20);
	writel(data, io + SEP0611_CBPCR);//set cursor size

	data=readl(io + SEP0611_LCDCBCR);
	data &= ~(0x3<<21);
	data |= (0x0<<21);//set cursor layer as 1bpp 1-color and transparent
	writel(data, io + SEP0611_LCDCBCR);

	writel(0xFFFFFF, io + SEP0611_CC0R);//color 0 black
	writel(0x00FF00, io + SEP0611_CC1R);//color 1 blue
	writel(0x0000FF, io + SEP0611_CC2R);//color 2 green
	writel(0xFFFFFF, io + SEP0611_CC3R);//color 3 it is none use(transparent)

	writel(0, io + SEP0611_ACSR);
	writel(cursor_addr_phy, io + SEP0611_CBSAR);
	writel(1, io + SEP0611_ACSR);

	for(i=0;i<20;i++)
		*(unsigned int *)(cursor_addr_vir+i)=gImage[i];
	//***************************//end
	writel(1, io + SEP0611_CECR);//enable the cursor layer
	return 0;
}

static int sep0611_overlay_setpos_cursor(unsigned int data)
{
	unsigned int x,y,tmpdata;
	tmpdata = readl(io + SEP0611_LCDCSCR);// read size from regs
	x = data>>16 & ~(0x1);
	if(x > 768) x=768;
	if((tmpdata>>16) > 800) x += ((tmpdata>>16)-800)/2;
	y = data & 0xFFFF;
	if((tmpdata & 0xFFFF) > 480) y += (tmpdata & 0xFFFF) - 480;
	writel(RIGHT_BOTTOM(x, y), io + SEP0611_CTPCR);
	writel(WIN_ENABLE, io + SEP0611_CECR);
	del_timer(&cursor_timer);
	cursor_timer.expires = jiffies + 100*3;
	add_timer(&cursor_timer);
	return 0;
}

static void timer_hide_cursor(unsigned long arg)
{
	writel(WIN_DISABLE, io + SEP0611_CECR);
}

#if USE_PICTURE 
static int sep0611_overlay_get_img(void);
#endif
/******************************** the module driver ************************************************************/
static int sep0611_overlay_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	unsigned int intArg,data;
	overlay_config_t config;
	lcdc_timming timming_tmp;
	switch(cmd)
	{
#if 0
		case OVERLAY_FORBID:
			if(copy_from_user(&intArg, (int *)arg, sizeof(int)))
				return -EFAULT;
			overlay_forbid = intArg;
			return 0;
#endif
#if USE_PICTURE
		case 111://GET_IMG
			//if(copy_from_user(&config, (overlay_config_t *)arg, sizeof(overlay_config_t)))
			//	return -EFAULT;
			return sep0611_overlay_get_img();
#endif
		case OVERLAY_PRINT_REGS:
			dprintk("print regs:\n");
			for(intArg=0; intArg<=0xCC;intArg+=4)
			{
				printk("reg[0x%x] = (0x%x)\n", (unsigned int)io + intArg, readl(io + intArg));
			}
			break;
		case OVERLAY_GET_STARTBUF:
			dprintk("overlay_kernel ioctl get startbuffer!\n");
			if(copy_to_user((unsigned int *)arg, &over1_addr_phy, sizeof(unsigned int)))
			{
				dprintk("error while copy!\n");
				return -EFAULT;
			}else{
			//	dprintk("success get startbuf: 0x%x",over1_addr_phy);
			}
			break;
		case OVERLAY_AUTORP_CTL:
			if(copy_from_user(&intArg, (int *)arg, sizeof(int)))
				return -EFAULT;
			if(intArg == 1){
				writel(0, io + SEP0611_ACSR);
				data = readl(io + SEP0611_LCDCBCR);
				data &=~(0x3<<13);
				data |=(0x2<<13);
				writel(data, io + SEP0611_LCDCBCR);
				mdelay(10);
				writel(1, io + SEP0611_ACSR);
				dprintk("ctl enable auto repair\n");
			}else{
				data = readl(io + SEP0611_LCDCBCR);
				data &=~(0x3<<13);
				writel(data, io + SEP0611_LCDCBCR);
				dprintk("ctl disable auto repair\n");
			}
			break;
		case OVERLAY_SHOW_CURSOR:
			return sep0611_overlay_drawcursor();
		case OVERLAY_SETPOS_CURSOR:
			if(copy_from_user(&intArg, (int *)arg, sizeof(int)))
				return -EFAULT;
			return sep0611_overlay_setpos_cursor(intArg);

		case OVERLAY_LAYERS_CTL://enable or disable layers
			if(copy_from_user(&intArg, (int *)arg, sizeof(int)))
				return -EFAULT;
			sep0611_overlay_layers_ctl(intArg);
			break;
		case 114://set lcdc timming
			if(copy_from_user(&timming_tmp, (lcdc_timming *)arg, sizeof(lcdc_timming)))
				return -EFAULT;
			sep0611_overlay_settimming(timming_tmp);
			break;
		case 115://set lcdc base layer size and format
			if(copy_from_user(&config, (overlay_config_t *)arg, sizeof(overlay_config_t)))
				return -EFAULT;
			intArg = 0;
			dprintk("set base size:(%d,%d)\n", config.width, config.height);
			intArg = XMAX(config.width)|YMAX(config.height);
			writel(intArg, io + SEP0611_BBPCR);
			writel(config.width, io + SEP0611_BASE_RAW_IMAGE_WIDTH); 
			switch(config.format)
			{
				case V4L2_PIX_FMT_RGB565:
					dprintk("use rgb565\n");
					data = readl(io + SEP0611_LCDCBCR);
					data &=~(0x3<<27);
					writel(data, io + SEP0611_LCDCBCR);
					break;
				case V4L2_PIX_FMT_RGB24:
					dprintk("use rgb888\n");
					data = readl(io + SEP0611_LCDCBCR);
					data &= ~(0x3<<27);
					data |= (0x2<<27);
					writel(data, io + SEP0611_LCDCBCR);
					break;
				default:
					dprintk("not support!\n");
					break;
			}
			break;
		case OVERLAY_GET_POSITION:
			return sep0611_overlay_get_pos((overlay_config_t*)arg);

		case OVERLAY_GET_SCREEN_INFO:
			return sep0611_overlay_get_screenInfo((overlay_config_t*)arg);

		case OVERLAY_SET_POSITION:
			if(copy_from_user(&config, (overlay_config_t *)arg, sizeof(overlay_config_t)))
				return -EFAULT;
			return sep0611_overlay_set_pos(config);

		case OVERLAY_QUEUE_BUFFER:
			if(copy_from_user(&intArg, (overlay_config_t *)arg, sizeof(unsigned int)))
				return -EFAULT;
			return sep0611_overlay_q_buffer((unsigned int)intArg);

		case OVERLAY_SET_CONFIGURE:
			if(copy_from_user(&config, (overlay_config_t *)arg, sizeof(overlay_config_t)))
				return -EFAULT;
			sep0611_overlay_set_configure(config);
			//sep0611_overlay_q_buffer(over1_addr_phy);
			break;

		case OVERLAY_SET_DISABLE:
			return sep0611_overlay_disable();

		case OVERLAY_SET_ENABLE:
			return sep0611_overlay_enable();

		case OVERLAY_COMMON_ENABLE:
			sep0611_common_enable();
			return 0;
		case OVERLAY_COMMON_DISABLE:
			sep0611_common_disable();
			return 0;
		case OVERLAY_SWITCHTO_VIDEO:
			sep0611_switchto_mode(0);
			break;
		case OVERLAY_SWITCHTO_HDMI:
			sep0611_switchto_mode(1);
			break;
		case OVERLAY_SWITCHTO_LCD:
			sep0611_switchto_mode(2);
			break;
		default:
			dprintk(" Unsupported IOCTL(%d)!!!\n", cmd);      
			break;			
	}
	return 0;
}

static int sep0611_overlay_release(struct inode *inode, struct file *file)
{
	overlay_use--;
	//if(overlay_use == 1) sep0611_overlay_disable();//TODO overlay_use=1 mean the cursor layer was onpen
	dprintk(" ===========> sep0611_overlay_release num:%d \n", overlay_use);
	return 0;
}

static int sep0611_overlay_open(struct inode *inode, struct file *file)
{
	dprintk("open overlay and (%d,%d,%d,%d,%d)\n", overlay_cfg.sx, overlay_cfg.sy, overlay_cfg.width, overlay_cfg.height, overlay_cfg.format);
	//overlay_cfg.sx = 0;overlay_cfg.sy=0;overlay_cfg.width=800;overlay_cfg.height=480;
	// fuzhuo add to enable alpha and C-K
	//if(overlay_en_count==0 && overlay_use==0) {sep0611_common_enable();overlay_en_count++;}
	//sep0611_common_disable();
	overlay_use ++;
	//if(overlay_use>1) sep0611_overlay_enable();//TODO overlay_use>1 means not only the cursor use it
	dprintk(" ====> sep0611_overlay_open num:%d \n", overlay_use);
	return 0;	
}

static int sep0611_overlay_mmap(struct file *file, struct vm_area_struct *vma)
{
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	if(remap_pfn_range(vma,vma->vm_start, vma->vm_pgoff , vma->vm_end - vma->vm_start, vma->vm_page_prot))
	{
		return -EAGAIN;
	}

	vma->vm_ops	= NULL;
	vma->vm_flags 	|= VM_IO;
	vma->vm_flags 	|= VM_RESERVED;
	
	return 0;
}

DECLARE_WAIT_QUEUE_HEAD(overlay_wait);
static unsigned int sep0611_overlay_poll(struct file *file, struct poll_table_struct *wait)
{
	dprintk(" sep0611_overlay_poll wait[%d][%d]!!!\n", (unsigned)wait, (unsigned)&overlay_wait);
	poll_wait(file, &(overlay_wait), wait);	
	dprintk(" sep0611_overlay_poll finish[%d][%d]!!!\n", (unsigned)wait, (unsigned)&overlay_wait);	
	return POLLIN;
}

#if USE_PICTURE
/************************open pic file***********************************/
static int picture_file_deal(const void * file_path, u32 * picture_addr)
{
	char *path = (char *) file_path;
	char *pdata, *end;
	int count;
	struct file *fd;
	mm_segment_t fs;
	loff_t pos;
	//int i;
	if(path == NULL)
	{
		printk("File name is NULL\n");
		return -1;
	}
	
	pdata = (char *)picture_addr;
	end = pdata + 0x200000;
	
	fd = filp_open(path, O_RDONLY, 0644);
	if(IS_ERR(fd))
	{
		printk("Can't open picture file %s\n", path);
		return -1;
	}
	
	fs = get_fs();
	set_fs(KERNEL_DS);
	pos = 0;
	
	printk("\nWait for reading file... \n");
	do
	{
		count = vfs_read(fd, pdata, 0x1000, &pos);
		if(count < 0x1000)
		{
			pdata += count;
			break;
		}
		pdata += 0x1000;
	}while(pdata < end);
	
	filp_close(fd, NULL);
	set_fs(fs);
	return 0;
}

static int sep0611_overlay_get_img(void)
{
	overlay_config_t config = overlay_cfg;
	dprintk("ioctl get img\n");
	memset(over1_addr_vir, 0xFFFFFFFF, 800*480*4*2);
	switch(config.format)
	{
		case V4L2_PIX_FMT_RGB565:
			dprintk("get img rgb565!\n");
			picture_file_deal("/data/test/my_320x240_16bpp.bin", over1_addr_vir);
			break;
		case V4L2_PIX_FMT_RGB24:
			dprintk("get img rgb888!\n");
			picture_file_deal("/data/test/my_320x240_18bpp.bin", over1_addr_vir);
			break;
		case V4L2_PIX_FMT_YUV444:
			dprintk("get img YUV444!\n");
			picture_file_deal("/data/test/yuv444_608_320.yuv", over1_addr_vir);
			break;
		case V4L2_PIX_FMT_YUV420:
			dprintk("get img YUV420!\n");
			picture_file_deal("/data/test/yuv420_608_320.yuv", over1_addr_vir);
			break;
		case V4L2_PIX_FMT_NV12:
			dprintk("get img YUV420semi!\n");
			picture_file_deal("/data/test/yuv420s_480_320.yuv", over1_addr_vir);
			break;
		default:
			dprintk("not support this format!\n");
			break;
	}
	return 0;
}
#endif

static struct file_operations sep0611_overlay_fops = {
	.owner          = THIS_MODULE,
	.poll           = sep0611_overlay_poll,
	.ioctl          = sep0611_overlay_ioctl,
	.mmap           = sep0611_overlay_mmap,
	.open           = sep0611_overlay_open,
	.release        = sep0611_overlay_release, 
};

static struct miscdevice sep0611_overlay_misc_device = {
	DEV_MINOR,
	DEVICE_NAME,
	&sep0611_overlay_fops,
};

static int __init sep0611_overlay_probe(struct platform_device *pdev)
{
	dprintk("go to function overlay probe!\n");
	init_timer(&cursor_timer);
	cursor_timer.function = &timer_hide_cursor;
	cursor_timer.data = 0;
	if (misc_register(&sep0611_overlay_misc_device))
	{
		dprintk("register misc device error!\n");
		return -EBUSY;
	}
	io = ioremap(LCDC_BASE, SZ_4K - 1);
	io_pmu = ioremap(0x30001000, 0xa4);
	/******************* start everything for test ********************************/
	over1_addr_vir = dma_alloc_writecombine(NULL, 1920*1080*2, &over1_addr_phy, GFP_KERNEL);

	cursor_addr_vir = dma_alloc_writecombine(NULL, 32*20/4, &cursor_addr_phy, GFP_KERNEL);//apply a cursor buffer

	memset(over1_addr_vir,0x0,1920*1080*2);//set all pixel to black color and enable_common at open
	overlay_cfg.sx=0; overlay_cfg.sy=0;
	overlay_cfg.width = 800;overlay_cfg.height = 480;
	overlay_cfg.format=V4L2_PIX_FMT_NV12;
	sep0611_overlay_set_configure(overlay_cfg);
	sep0611_overlay_q_buffer(over1_addr_phy);
	//sep0611_common_enable();

	dprintk("over1_addr_phy=0x%x\n",(int)over1_addr_phy);
	dprintk("over1_addr_vir=0x%x\n",(int)over1_addr_vir);
	/**************************** test over ***************************************/
	return 0;
}

static int sep0611_overlay_remove(struct platform_device *pdev)
{
	dprintk("go to %s\n", __func__);
	sep0611_overlay_disable();
	iounmap(io);
	misc_deregister(&sep0611_overlay_misc_device);
	// just for img test
	dma_free_writecombine(NULL, 1920*1080*2, over1_addr_vir, over1_addr_phy);
	dma_free_writecombine(NULL, 32*20/4, cursor_addr_vir, cursor_addr_phy);
	return 0;
}

static int sep0611_overlay_suspend(struct platform_device *pdev, pm_message_t state)
{
	dprintk("go to %s\n", __func__);
	if(overlay_use != 0)
	{
		dprintk("not used \n");
	}
	return 0;
}

static int sep0611_overlay_resume(struct platform_device *pdev)
{
	dprintk("go to %s\n", __func__);
	return 0;
}

/*
static void sep0611_overlay_platform_release(struct device * dev)
{
	dprintk("go to %s\n", __func__);
}

static struct platform_device sep0611_overlay_device = {
	.name   = "sep0611_overlay",
	.dev	= {
		.release = sep0611_overlay_platform_release,
	},
	.id = 0,
};
*/

static struct platform_driver sep0611_overlay_driver = {
	.driver = {
		.name = "sep0611_overlay",
		.owner = THIS_MODULE,
	},
	.probe = sep0611_overlay_probe,
	.remove = sep0611_overlay_remove,
	.suspend = sep0611_overlay_suspend,
	.resume = sep0611_overlay_resume,
};

static int __init sep0611_overlay_init(void)
{
	dprintk("sep0611_overlay driver init\n");

//	platform_device_register(&sep0611_overlay_device);
	platform_driver_register(&sep0611_overlay_driver);

	return 0;
}

static void __exit sep0611_overlay_exit(void)
{
	dprintk("sep0611_overlay driver exit\n");
	platform_driver_unregister(&sep0611_overlay_driver);
	//platform_device_unregister(&sep0611_overlay_device);
}

module_init(sep0611_overlay_init);
module_exit(sep0611_overlay_exit);

MODULE_AUTHOR("fuzhuo ,fuzhuo233@gmail.com");
MODULE_DESCRIPTION("SEP0611 Overlay Driver");
MODULE_LICENSE("GPL");

