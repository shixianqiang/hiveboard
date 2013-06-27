/* linux/sound/soc/sep/sep0611-pcm.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *  cgm <chenguangming@wxseuic.com>
 *
 * Southeast University ASIC SoC support
 *
 * SEP0611 ASoC PCM interface
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  05-31-2011	cgm initial version
 *
 */

#ifndef _SEP0611_PCM_H
#define _SEP0611_PCM_H

typedef struct{
    struct snd_pcm_substream *play_ptr;
    struct snd_pcm_substream *capt_ptr;
}sep0611_pcms_t;

extern sep0611_pcms_t sep0611_pcms;

struct sep0611_runtime_data {
	spinlock_t lock;
	dma_addr_t dma_buffer;     	/* physical address of dma buffer */
	size_t buffer_len;
	size_t period_len;
	unsigned long dma_ch_mask;	/* dma channel mask */
	struct sep0611_i2s_dma_params *dma_params;
	struct dma_info *dma_info;
};

void sep0611_pcm_period_done(void *arg);

#endif /* _SEP0611_PCM_H */
