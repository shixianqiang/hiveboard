/* linux/sound/soc/sep/sep0611-pcm.c
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

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>

#include <linux/delay.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>

#include <asm/dma.h>
#include <mach/hardware.h>
#include <mach/irqs.h>

#include "i2s_dma.h"
#include "sep0611-pcm.h"
#include "sep0611-i2s.h"

#include "alsa_debug.h"

sep0611_pcms_t sep0611_pcms;	/* record the opened pcm's pointer */

#define SEP0611_PCM_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 |\
                           SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_22050 |\
                           SNDRV_PCM_RATE_32000 | SNDRV_PCM_RATE_44100 |\
                           SNDRV_PCM_RATE_48000 )

#define SEP0611_PCM_FMTS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE |\
						  SNDRV_PCM_FMTBIT_S24_3LE | SNDRV_PCM_FMTBIT_S32_LE)

static struct snd_pcm_hardware snd_pcm_playback =
{
	.info				= (SNDRV_PCM_INFO_INTERLEAVED |
				 	 	   SNDRV_PCM_INFO_BLOCK_TRANSFER |
				  		   SNDRV_PCM_INFO_MMAP | SNDRV_PCM_INFO_MMAP_VALID),
	.formats			= SEP0611_PCM_FMTS,
	.rates				= SEP0611_PCM_RATES,
	.rate_min			= 8000,
	.rate_max			= 48000,
	.channels_min		= 1,
	.channels_max		= 2,
	.buffer_bytes_max	= 16*4096,
	.period_bytes_min	= 4096,
	.period_bytes_max	= 4096,
	.periods_min		= 8,
	.periods_max		= 64,
};

static struct snd_pcm_hardware snd_pcm_capture =
{
	.info				= (SNDRV_PCM_INFO_INTERLEAVED |
				 		  SNDRV_PCM_INFO_BLOCK_TRANSFER |
				 		  SNDRV_PCM_INFO_MMAP | SNDRV_PCM_INFO_MMAP_VALID),
	.formats			= SEP0611_PCM_FMTS,
	.rates				= SEP0611_PCM_RATES,
	.rate_min			= 8000,
	.rate_max			= 48000,
	.channels_min		= 1,
	.channels_max		= 2,
	.buffer_bytes_max	= 16*4096,
	.period_bytes_min	= 4096,
	.period_bytes_max	= 4096,
	.periods_min		= 6,
	.periods_max		= 64,
};

/*
 * called by the DMA driver when complete a period transmit.
 */
void sep0611_pcm_period_done(void *arg)
{
	snd_pcm_period_elapsed((struct snd_pcm_substream *)arg);
}

static int sep0611_pcm_hw_params(struct snd_pcm_substream *substream, struct snd_pcm_hw_params *params)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct sep0611_runtime_data *prtd = runtime->private_data;
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct sep0611_i2s_dma_params *dma = rtd->dai->cpu_dai->dma_data;
	unsigned long totsize = params_buffer_bytes(params);	/*get the buffer size from params(maybe not equal to allocated in xxx_pcm_open)*/
   	unsigned long period_bytes = params_period_bytes(params);
  
  	if(!dma)
  		return 0;

	snd_pcm_set_runtime_buffer(substream, &substream->dma_buffer);     
	runtime->dma_bytes = totsize; /* runtime dma_bytes should be the real value in hw_params */

	spin_lock_irq(&prtd->lock);

	/* fill the private runtime data struct */
	prtd->dma_params = dma;	
	prtd->dma_buffer = runtime->dma_addr;
	prtd->period_len = period_bytes;
	prtd->buffer_len = totsize;
   	prtd->dma_ch_mask = 1<<(dma->channel);
   
	spin_unlock_irq(&prtd->lock);
	
	/* store the substream in pcms */
	if(substream->stream == SNDRV_PCM_STREAM_PLAYBACK){		
		sep0611_pcms.play_ptr = substream;
	}
	else{
		sep0611_pcms.capt_ptr = substream;
	}

   	alsa_dbg("period_len=0x%08lX, buffer_len=0x%08lX, buffer_addr=0x%08lX, rate=%d\n",
          	prtd->period_len, prtd->buffer_len, prtd->dma_buffer, params_rate(params));

   	return 0;  
}

static int sep0611_pcm_hw_free(struct snd_pcm_substream *substream)
{
	struct sep0611_runtime_data *prtd = substream->runtime->private_data;

	alsa_dbg("%s\n", __func__);
	if (prtd->dma_params) {
 		snd_pcm_set_runtime_buffer(substream, NULL);
    }
    
    if(substream->stream == SNDRV_PCM_STREAM_PLAYBACK){
		sep0611_pcms.play_ptr = NULL;
	}
	else{
		sep0611_pcms.capt_ptr = NULL;
	}
	
	if(prtd->dma_info)
		i2s_dma_free(substream);
		
	return 0;
}

static int sep0611_pcm_prepare(struct snd_pcm_substream *substream)
{
	struct sep0611_runtime_data *prtd = substream->runtime->private_data;
	
	alsa_dbg("%s\n", __func__);
	
	/* make sure the last dma structs be free */
	if(prtd->dma_info)
    	i2s_dma_free(substream);
    	
	if(i2s_dma_prepare(substream) < 0){
		return -ENOMEM;
	}
	
	return 0;
}

static int sep0611_pcm_trigger(struct snd_pcm_substream *substream, int cmd)
{
	int ret = 0;
	alsa_dbg("%s\n", __func__);
	
	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:        
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		i2s_dma_start(substream);		
		break;

	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		i2s_dma_stop(substream);
 		break;

	default:
		ret = -EINVAL;
	}

	return ret;
}

/*
 * buffer point update
 * current position range 0-(buffer_size-1)
 */
static snd_pcm_uframes_t sep0611_pcm_pointer(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct sep0611_runtime_data *prtd = runtime->private_data;	
	snd_pcm_uframes_t frames;
	unsigned long pos;
	
	pos = i2s_dma_pointer(substream) - prtd->dma_buffer;
	
	frames = bytes_to_frames(runtime, pos);
	
	if(frames >= runtime->buffer_size)
		frames -= runtime->buffer_size;
	
   	return frames;
}

static int sep0611_pcm_open(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct sep0611_runtime_data *prtd; 
	int res = 0;

	alsa_dbg("%s\n", __func__);

	if(substream->stream == SNDRV_PCM_STREAM_PLAYBACK){
		snd_soc_set_runtime_hwparams(substream, &snd_pcm_playback);
	}
	else{
		snd_soc_set_runtime_hwparams(substream, &snd_pcm_capture);
	}

	prtd = kmalloc(sizeof(struct sep0611_runtime_data), GFP_KERNEL);
	if(prtd == NULL){
		res = -ENOMEM;
	}
	prtd->dma_info = NULL;
	runtime->private_data = prtd;
	spin_lock_init(&prtd->lock);  
	
	/*ensure that buffer size is a multiple of period size*/
	if((res = snd_pcm_hw_constraint_integer(runtime, SNDRV_PCM_HW_PARAM_PERIODS)) < 0)
	{
		printk("failed to set hw constraint integer\n");
		res = -EINVAL;
	}
	      
	return res;
}

static int sep0611_pcm_close(struct snd_pcm_substream *substream)
{
	struct sep0611_runtime_data *prtd = substream->runtime->private_data;
	
	if(prtd)	
		kfree(prtd);
	   
	return 0;
}

static int sep0611_pcm_mmap(struct snd_pcm_substream *substream, struct vm_area_struct *vma)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

	alsa_dbg("%s\n", __func__);

	return dma_mmap_writecombine(substream->pcm->card->dev, vma,
		runtime->dma_area,
		runtime->dma_addr,
		runtime->dma_bytes);
}

struct snd_pcm_ops sep0611_pcm_ops = {
    .open   	= sep0611_pcm_open,
    .close  	= sep0611_pcm_close,
    .hw_params 	= sep0611_pcm_hw_params,
    .hw_free 	= sep0611_pcm_hw_free,
    .prepare 	= sep0611_pcm_prepare,
    .trigger 	= sep0611_pcm_trigger,
    .pointer 	= sep0611_pcm_pointer,
    .mmap 		= sep0611_pcm_mmap,
    .ioctl  	= snd_pcm_lib_ioctl,
};

static int sep0611_pcm_preallocate_dma_buffer(struct snd_pcm *pcm, int stream)
{
	struct snd_pcm_substream *substream = pcm->streams[stream].substream;
	struct snd_dma_buffer *buf = &substream->dma_buffer;
	size_t size = 0;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		size = snd_pcm_playback.buffer_bytes_max;
	}
	else {
		size = snd_pcm_capture.buffer_bytes_max;
	}

	buf->dev.type = SNDRV_DMA_TYPE_DEV;
	buf->dev.dev = pcm->card->dev;
	buf->private_data = NULL;

	alsa_dbg("sep0611 pcm preallocate dma buffer size [%d]\n", size);
	buf->area = dma_alloc_writecombine(pcm->card->dev, size, &buf->addr, GFP_KERNEL);
	if (!buf->area) {
		return -ENOMEM;
    }
    
	buf->bytes = size;
	return 0;
}

static void sep0611_pcm_free_dma_buffers(struct snd_pcm *pcm)
{
    struct snd_pcm_substream *substream;
    struct snd_dma_buffer *buf;
    int stream;
	alsa_dbg("%s\n", __func__);

    for (stream = 0; stream < 2; stream++) {
        substream = pcm->streams[stream].substream;
        if(!substream){
        	continue;
        }

        buf = &substream->dma_buffer;
        if(!buf->area){
        	continue;
  		}

        dma_free_writecombine(pcm->card->dev, buf->bytes, buf->area, buf->addr);
        buf->area = NULL;
    }
}

static u64 sep0611_pcm_dmamask = DMA_32BIT_MASK;

int sep0611_pcm_new(struct snd_card *card, struct snd_soc_dai *dai, struct snd_pcm *pcm)
{
	int ret = 0;
	alsa_dbg("%s\n", __func__);

	if (!card->dev->dma_mask) {
   		card->dev->dma_mask = &sep0611_pcm_dmamask;
	}
	if (!card->dev->coherent_dma_mask) {
 		card->dev->coherent_dma_mask = DMA_32BIT_MASK;
	}
	
	if (dai->playback.channels_min) {
   		ret = sep0611_pcm_preallocate_dma_buffer(pcm, SNDRV_PCM_STREAM_PLAYBACK);
 		if (ret) {
 			goto out; 
 		}
	}
	if (dai->capture.channels_min) {
  		ret = sep0611_pcm_preallocate_dma_buffer(pcm, SNDRV_PCM_STREAM_CAPTURE);
  		if (ret) { 
  			goto out;
  		}
	} 

out:
	return ret;
}

int sep0611_pcm_suspend(struct snd_soc_dai *cpu_dai)
{
	return 0;
}

int sep0611_pcm_resume(struct snd_soc_dai *cpu_dai)
{
	return 0;
}

struct snd_soc_platform sep0611_soc_platform = {
    .name  		= "SEP0611-ASoC",
    .pcm_ops	= &sep0611_pcm_ops,
    .pcm_new 	= sep0611_pcm_new,
    .pcm_free	= sep0611_pcm_free_dma_buffers,
    .suspend 	= sep0611_pcm_suspend,
    .resume 	= sep0611_pcm_resume,
};

EXPORT_SYMBOL_GPL(sep0611_soc_platform);

static int __init sep0611_soc_platform_init(void)
{
	return snd_soc_register_platform(&sep0611_soc_platform);
}
module_init(sep0611_soc_platform_init);

static void __exit sep0611_soc_platform_exit(void)
{
	snd_soc_unregister_platform(&sep0611_soc_platform);
}
module_exit(sep0611_soc_platform_exit);

MODULE_AUTHOR("Chen Guangming");
MODULE_DESCRIPTION("SEP0611 ASoC PCM interface");
MODULE_LICENSE("GPL");

