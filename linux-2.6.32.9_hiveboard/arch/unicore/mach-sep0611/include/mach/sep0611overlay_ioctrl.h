#ifndef _SEP0611OVERLAY_IOCTRL_H_
#define _SEP0611OVERLAY_IOCTRL_H_

/* macro for ioctls */
#define OVERLAY_GET_POSITION        10
#define OVERLAY_GET_SCREEN_INFO     20
#define OVERLAY_SET_POSITION        30
#define OVERLAY_QUEUE_BUFFER        40
#define OVERLAY_SET_CONFIGURE       50
#define OVERLAY_SET_DISABLE         60
#define OVERLAY_SET_CROMA           70
#define OVERLAY_COMMON_ENABLE		67
#define OVERLAY_COMMON_DISABLE		68
#define OVERLAY_GET_STARTBUF		105
#define OVERLAY_SET_ENABLE			65
#define OVERLAY_SWITCHTO_HDMI		75
#define OVERLAY_SWITCHTO_VIDEO		76
#define OVERLAY_SWITCHTO_LCD		77

#define OVERLAY_FORBID              100
// fuzhuo add for test use
#define OVERLAY_LAYERS_CTL			104
#define OVERLAY_AUTORP_CTL			103

#define OVERLAY_GET_IMG				111

#define OVERLAY_SET_LCDCTIMING			114
#define OVERLAY_SET_LCDCBCR			115

#define OVERLAY_PRINT_REGS			133
#define OVERLAY_SHOW_CURSOR			135
#define OVERLAY_SETPOS_CURSOR			137

typedef struct
{
	unsigned int sx;
	unsigned int sy;
	unsigned int width;
	unsigned int height;
	unsigned int format;
	unsigned int transform;
} overlay_config_t;

typedef struct
{
	unsigned int enable;
	unsigned int chromaR; // 0~8 key color , 16~ 24 mask 
	unsigned int chromaG; // 0~8 key color , 16~ 24 mask 
	unsigned int chromaB; // 0~8 key color , 16~ 24 mask 
} overlay_chroma_t;


#endif /* SEP0611_OVERLAY_H_ */

