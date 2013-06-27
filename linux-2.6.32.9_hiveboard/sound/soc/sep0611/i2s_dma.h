/* linux/sound/soc/sep/i2s_dma.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *  cgm <chenguangming@wxseuic.com>
 *
 * Southeast University ASIC SoC support
 *
 * DMAC configuration for SEP0611 ASoC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  05-31-2011	cgm initial version
 *
 */
 
#ifndef _I2S_DMA_H
#define _I2S_DMA_H

#include <mach/dma.h>

#define PLAYBACK	0
#define CAPTURE		1

#define RUNNING 	0
#define STOPED		1

#define PLAY_CH		DMA_I2S_TX_CH
#define CAPT_CH		DMA_I2S_RX_CH

void i2s_dma_init(void);
int  i2s_dma_prepare(struct snd_pcm_substream *substream);
void i2s_dma_free(struct snd_pcm_substream *substream);
void i2s_dma_start(struct snd_pcm_substream *substream);
void i2s_dma_stop(struct snd_pcm_substream *substream);
unsigned long i2s_dma_pointer(struct snd_pcm_substream *substream);

enum dma_trans_type{
	MEM_TO_MEM = 0,
	MEM_TO_DEV,
	DEV_TO_MEM,
	DEV_TO_DEV,
};

struct dma_info{
	struct list_head dma_list_head;
	unsigned long cfg_lo;
	unsigned long cfg_hi;
	unsigned long ctrl_lo;
	unsigned long ctrl_hi;
	unsigned long src_addr;
	unsigned long dst_addr;
	unsigned long first_llp;
	enum dma_trans_type trans_type;
};

#endif /* _I2S_DMA_H */
