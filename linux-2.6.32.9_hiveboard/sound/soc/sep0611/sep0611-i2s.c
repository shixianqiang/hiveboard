/* linux/sound/soc/sep/sep0611-i2s.c
 *
 * Copyright (c) 2009-2011 SEUIC
 *  cgm <chenguangming@wxseuic.com>
 *
 * Southeast University ASIC SoC support
 *
 * SEP0611 ASoC I2S interface
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  05-31-2011	cgm initial version
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/initval.h>
#include <sound/soc.h>

#include <mach/hardware.h>
#include <mach/gpio.h>
#include <mach/regs-i2s.h>
#include <mach/regs-gpio.h>

#include <asm/io.h>

#include "sep0611-pcm.h"
#include "sep0611-i2s.h"
#include "i2s_dma.h"

#include "alsa_debug.h"

#ifdef CONFIG_CPU_FREQ
#include <linux/cpufreq.h>
#endif

#ifdef CONFIG_PM
#include <linux/pm.h>
#endif
/*
*sep0611 i2s information
*@base:		the base virtual address of ioremap()
*@fast_mode:fast mode use to 24bits
*@wss:		see the i2s' registers note
*@sclk_gate:the width of sclk gate
*@dev:		pointer to platfrom_device->dev
*/
struct sep0611_i2s_info {
	void __iomem *base;
	unsigned int sample_bits;
	unsigned int sample_rate;
	unsigned int fast_mode;
	unsigned int wss;
	unsigned int sclk_gate;
	unsigned int clk_rate;
	struct device *dev;
	struct clk *clk;
};

static struct sep0611_i2s_info i2s_info;

static struct sep0611_i2s_dma_params sep0611_i2s_stereo_out = {
	.name		= "I2S PCM Stereo out",
   	.dma_addr	= I2S_BASE + I2S_TXDMA,
	.channel	= PLAY_CH,
};

static struct sep0611_i2s_dma_params sep0611_i2s_stereo_in = {
	.name		= "I2S PCM Stereo in",
 	.dma_addr	= I2S_BASE + I2S_RXDMA,
	.channel	= CAPT_CH,
};

/*
 * init i2s control registers and GPIO function
 */
void init_i2s_gpio(void)
{
   	alsa_dbg("%s\n", __func__);  
    
   	/* select the special funtion */
	sep0611_gpio_cfgpin(SEP0611_GPC0, I2S_SDI0);
	sep0611_gpio_cfgpin(SEP0611_GPC1, I2S_SDO0);
	sep0611_gpio_cfgpin(SEP0611_GPC2, I2S_WS);
	sep0611_gpio_cfgpin(SEP0611_GPC3, I2S_CLK);
}


/*
 * set up i2s
 */
static int sep0611_i2s_startup(struct snd_pcm_substream *substream, struct snd_soc_dai *dai)
{
	alsa_dbg("%s\n", __func__);
	writel(0x01, i2s_info.base + I2S_IER); 		/*enable i2s*/

	writel(0x33, i2s_info.base + I2S_IMR0); 	/*mask int of channel 0(both tx and rt)*/
	writel(0x30, i2s_info.base + I2S_IMR1); 	/*mask int of channel 1(only have tx)*/
	writel(0x30, i2s_info.base + I2S_IMR2); 	/*mask int of channel 2(only have tx)*/
	writel(0x30, i2s_info.base + I2S_IMR3); 	/*mask int of channel 3(only have tx)*/
			
	if(substream->stream == SNDRV_PCM_STREAM_PLAYBACK){
		writel(0x00, i2s_info.base + I2S_TER0);	/*disable all tx channel first*/
		writel(0x00, i2s_info.base + I2S_TER1);
		writel(0x00, i2s_info.base + I2S_TER2);
		writel(0x00, i2s_info.base + I2S_TER3);
		
		writel(0x01, i2s_info.base + I2S_TXFFR);/*tx fifo flush*/
		writel(0x01, i2s_info.base + I2S_RTXDMA);/*reset dma pointer*/
		writel(0x07, i2s_info.base + I2S_TFCR0);/*tx fifo threshold*/
		writel(0x01, i2s_info.base + I2S_ITER);	/*tx enable*/
	}
	else{
		writel(0x00, i2s_info.base + I2S_RER0);	/*disable all rx channel(there is only one rx channel)*/
		
   		writel(0x01, i2s_info.base + I2S_RXFFR);/*recieve fifo flush*/
		writel(0x01, i2s_info.base + I2S_RRXDMA);/*reset dma pointer*/
		writel(0x09, i2s_info.base + I2S_RFCR0);/*rx fifo threshold*/
		writel(0x01, i2s_info.base + I2S_IRER);	/*recieve enable*/
	}
	
	writel(0x00, i2s_info.base + I2S_CER);		/*disable clock first, it may be enabled in sep0611_i2s_set_dai_sysclk()*/
	
  	return 0;
}

/*
 * set i2s data format
 */
static int sep0611_i2s_set_dai_fmt(struct snd_soc_dai *cpu_dai, unsigned int format)
{	
	unsigned int rtcr;	/* RCR/TCR value */
	
	alsa_dbg("%s\n", __func__);	
	
	switch(format)
	{
	case SNDRV_PCM_FORMAT_S16_LE:
		i2s_info.sample_bits = 16;
		i2s_info.wss = 0;
		i2s_info.sclk_gate = 0;
		i2s_info.fast_mode = 0;
		rtcr = 0x02;	/*16bits*/
		break;
	
	case SNDRV_PCM_FORMAT_S20_3LE:
		i2s_info.sample_bits = 20;
		i2s_info.wss = 1;
		i2s_info.sclk_gate = 3;
		i2s_info.fast_mode = 1;
		rtcr = 0x03; 	/*20bits*/
		break;
	
	case SNDRV_PCM_FORMAT_S24_3LE:
		i2s_info.sample_bits = 24;
		i2s_info.wss = 1;
		i2s_info.sclk_gate = 0;
		i2s_info.fast_mode = 1;
		rtcr = 0x04; 	/*24bits*/
		break;
	
	case SNDRV_PCM_FORMAT_S32_LE:
		i2s_info.sample_bits = 32;
		i2s_info.wss = 2;
		i2s_info.sclk_gate = 0;
		i2s_info.fast_mode = 0;
		rtcr = 0x05; 	/*32bits*/
		break;
	
	default:
		i2s_info.sample_bits = 16;
		i2s_info.wss = 0;
		i2s_info.sclk_gate = 0;
		i2s_info.fast_mode = 0;
		rtcr = 0;
	}
	
	/* tx and rx must be same */
	writel(rtcr, i2s_info.base + I2S_TCR0);
	writel(rtcr, i2s_info.base + I2S_RCR0);

	return 0;
}

/*
 * calculate i2s clock in master mode
 */
static unsigned int sep0611_i2s_calc_clk(unsigned int rate, unsigned int bits)
{
    unsigned int n1, n2;
    unsigned int scr;
	unsigned int mclk = i2s_info.clk_rate;

	if(mclk == 0){
		printk("ERR:not find i2s clock.\n");
		return 0;
	}

    n1 = mclk/rate/bits/2;
    
    /* n1 must to be even */
    if(n1%2 == 1){
        n1 -= 1;
    }

    n2 = mclk/rate/2 - ((bits-1) * n1);

    scr = n2<<12 | n1<<4 | 1;
    
    return scr;    
}

/*
 * set i2s clock in master mode
 */
static int sep0611_i2s_set_dai_sysclk(struct snd_soc_dai *cpu_dai,
	int clk_id, unsigned int freq, int dir)
{
	unsigned temp;
	
	alsa_dbg("%s\n", __func__);

	i2s_info.sample_rate = freq;

	if(clk_id == SLAVE_MODE){
		return 0;
	}
	else{/*in master mode. use hardware or software to configure clock*/
		if(i2s_info.sample_bits == 0){
			printk("BUG:i2s_info.sample_bits == 0???\n");
			goto out;
		}
		
		if(clk_id == MASTER_MODE_HARDWARE){
			writel(0x00, i2s_info.base + I2S_SCR);
		}
		else{
			temp = sep0611_i2s_calc_clk(freq, i2s_info.sample_bits);
			writel(temp, i2s_info.base + I2S_SCR);
		}
		
		temp = (freq<<8) | (i2s_info.wss<<3) | (i2s_info.sclk_gate);
		writel(temp, i2s_info.base + I2S_CCR);
		
		/*enable clock generator*/
		writel(0x01, i2s_info.base + I2S_CER);
	}

out:	
   	return 0;
}

/*
 * set hardware parameters of i2s, to set format and clock
 */
static int sep0611_i2s_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params, struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->dai->cpu_dai;
	
	alsa_dbg("%s\n", __func__);
	
	if(substream->stream == SNDRV_PCM_STREAM_PLAYBACK){
		cpu_dai->dma_data = &sep0611_i2s_stereo_out;
	}
	else{
		cpu_dai->dma_data = &sep0611_i2s_stereo_in;
	}
	
	/*set format and clock*/
	sep0611_i2s_set_dai_fmt(cpu_dai, params_format(params));
	
#if CTRL_MODE == SLAVE_MODE
	sep0611_i2s_set_dai_sysclk(cpu_dai, SLAVE_MODE, params_rate(params), 0);
#else
	#if CTRL_MODE == MASTER_MODE_SOFTWARE
	sep0611_i2s_set_dai_sysclk(cpu_dai, MASTER_MODE_SOFTWARE, params_rate(params), 0);
	#else
	sep0611_i2s_set_dai_sysclk(cpu_dai, MASTER_MODE_HARDWARE, params_rate(params), 0);
	#endif
#endif

	if((i2s_info.fast_mode) && (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)){
		writel(0x03, i2s_info.base + I2S_ITER);	/*fast mode enable*/		
	}
  
   	return 0;
}

#ifdef SEP0611_SPK_CTL
	extern void sep0611_spk_out(bool enable);
#else
	#define sep0611_spk_out(x)
#endif

static int sep0611_i2s_trigger(struct snd_pcm_substream *substream, 
	int cmd, struct snd_soc_dai *dai)
{
	int ret = 0;
	int stream_id = substream->stream;
	
	alsa_dbg("in %s\n", __func__);

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		
		if(stream_id == SNDRV_PCM_STREAM_PLAYBACK){
			writel(0x01, i2s_info.base + I2S_TER0);  /*enable tx*/
			sep0611_spk_out(true);
		}
		else{
			writel(0x01, i2s_info.base + I2S_RER0);  /*enable rx*/
		}
		
		break;

	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
	  	if(stream_id == SNDRV_PCM_STREAM_PLAYBACK){
			sep0611_spk_out(false);
			writel(0x00, i2s_info.base + I2S_TER0);  /*disable tx*/
		}
		else{
			writel(0x00, i2s_info.base + I2S_RER0);  /*disable rx*/
		}
		break;

 	default:
   		ret = -EINVAL;
    }

#ifdef _DEBUG_I2S_REGS_VIEW
    alsa_dbg("\ni2s regs_list:\n");
    alsa_dbg("I2S_IER:0x%lx\n", readl(i2s_info.base + I2S_IER));
    alsa_dbg("I2S_IRER:0x%lx\n", readl(i2s_info.base + I2S_IRER));
    alsa_dbg("I2S_ITER:0x%lx\n", readl(i2s_info.base + I2S_ITER));
    alsa_dbg("I2S_CER:0x%lx\n", readl(i2s_info.base + I2S_CER));
    alsa_dbg("I2S_CCR:0x%lx\n", readl(i2s_info.base + I2S_CCR));
    
    alsa_dbg("I2S_ISR0:0x%lx\n", readl(i2s_info.base + I2S_ISR0));
    alsa_dbg("I2S_ISR1:0x%lx\n", readl(i2s_info.base + I2S_ISR1));
    alsa_dbg("I2S_ISR2:0x%lx\n", readl(i2s_info.base + I2S_ISR2));
    alsa_dbg("I2S_ISR3:0x%lx\n", readl(i2s_info.base + I2S_ISR3));
    
    alsa_dbg("I2S_RER0:0x%lx\n", readl(i2s_info.base + I2S_RER0));
    
    alsa_dbg("I2S_TER0:0x%lx\n", readl(i2s_info.base + I2S_TER0));
    alsa_dbg("I2S_TER1:0x%lx\n", readl(i2s_info.base + I2S_TER1));
    alsa_dbg("I2S_TER2:0x%lx\n", readl(i2s_info.base + I2S_TER2));
    alsa_dbg("I2S_TER3:0x%lx\n", readl(i2s_info.base + I2S_TER3));
    
    alsa_dbg("I2S_RCR0:0x%lx\n", readl(i2s_info.base + I2S_RCR0));
    
    alsa_dbg("I2S_TCR0:0x%lx\n", readl(i2s_info.base + I2S_TCR0));
    alsa_dbg("I2S_TCR1:0x%lx\n", readl(i2s_info.base + I2S_TCR1));
    alsa_dbg("I2S_TCR2:0x%lx\n", readl(i2s_info.base + I2S_TCR2));
    alsa_dbg("I2S_TCR3:0x%lx\n", readl(i2s_info.base + I2S_TCR3));
    
    alsa_dbg("I2S_IMR0:0x%lx\n", readl(i2s_info.base + I2S_IMR0));                             
    alsa_dbg("I2S_IMR1:0x%lx\n", readl(i2s_info.base + I2S_IMR1));
    alsa_dbg("I2S_IMR2:0x%lx\n", readl(i2s_info.base + I2S_IMR2));
    alsa_dbg("I2S_IMR3:0x%lx\n", readl(i2s_info.base + I2S_IMR3));
    
    alsa_dbg("I2S_RFCR0:0x%lx\n", readl(i2s_info.base + I2S_RFCR0));
    alsa_dbg("I2S_TFCR0:0x%lx\n", readl(i2s_info.base + I2S_TFCR0));
#endif
    
	alsa_dbg("out %s\n", __func__);
 	return ret;
}

static void sep0611_i2s_shutdown(struct snd_pcm_substream *substream, struct snd_soc_dai *dai)
{
	int stream_id = substream->stream;
	
	if(stream_id == SNDRV_PCM_STREAM_PLAYBACK){
		writel(0x00, i2s_info.base + I2S_ITER);	/*tx disable*/	
	}
	else{
		writel(0x00, i2s_info.base + I2S_IRER);	/*rx disable*/
	}
	i2s_info.sample_rate = 0;

	alsa_dbg("%s\n", __func__);
}

#ifdef CONFIG_PM
#define I2S_PM_REGS_NUM (0x1C0>>2)
volatile unsigned long *i2s_regs;

static int sep0611_i2s_suspend(struct snd_soc_dai *dai)
{
	int i;
	volatile unsigned long *p_regs = i2s_info.base;

	alsa_dbg("%s\n", __func__);
    
	i2s_regs = kmalloc((I2S_PM_REGS_NUM << 2), GFP_KERNEL);
	if(i2s_regs == NULL)
		return -ENOMEM;

	for(i=0; i<I2S_PM_REGS_NUM; i++)
		i2s_regs[i] = *p_regs++;

	clk_disable(i2s_info.clk);

	return 0;

}

static int sep0611_i2s_resume(struct snd_soc_dai *dai)
{
	int i;
	volatile unsigned long *p_regs = i2s_info.base;

	clk_enable(i2s_info.clk);
	
	init_i2s_gpio();

	for(i=0; i<I2S_PM_REGS_NUM; i++)
		*p_regs++ = i2s_regs[i];

	kfree(i2s_regs);
	i2s_regs = NULL;

    return 0;
}

#else

#define sep0611_i2s_suspend	NULL
#define sep0611_i2s_resume	NULL

#endif

#define SEP0611_I2S_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 |\
                           SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_22050 |\
                           SNDRV_PCM_RATE_32000 | SNDRV_PCM_RATE_44100 |\
                           SNDRV_PCM_RATE_48000 )

#define SEP0611_I2S_TX_FMTS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE |\
						  	 SNDRV_PCM_FMTBIT_S24_3LE | SNDRV_PCM_FMTBIT_S32_LE)

#define SEP0611_I2S_RX_FMTS	(SNDRV_PCM_FMTBIT_S16_LE)

/*************add by zy**************/
#ifdef CONFIG_CPU_FREQ_BROKEN_SOUND
struct notifier_block   sep_i2s_freq_transition;
static int sep_i2s_cpufreq_transition(struct notifier_block *nb,
                      unsigned long val, void *data)
{
	unsigned temp;

	i2s_info.clk_rate = clk_get_rate(i2s_info.clk);
	
	alsa_dbg("i2s-rate is :%d\n",is2_info.clk_rate);

	if(i2s_info.sample_rate == 0)
			return;
#if CTRL_MODE == SLAVE_MODE
	return;
#elif CTRL_MODE == MASTER_MODE_HARDWARE
	writel(0x00, i2s_info.base + I2S_SCR);
#else
	temp = sep0611_i2s_calc_clk(i2s_info.sample_rate, i2s_info.sample_bits);
	writel(temp, i2s_info.base + I2S_SCR);
#endif
	temp = (i2s_info.sample_rate<<8) | (i2s_info.wss<<3) | (i2s_info.sclk_gate);
	writel(temp, i2s_info.base + I2S_CCR);
		
	/*enable clock generator*/
	writel(0x01, i2s_info.base + I2S_CER);

    return 0;
}

static inline int sep_i2s_cpufreq_register(void)
{
    sep_i2s_freq_transition.notifier_call = sep_i2s_cpufreq_transition;

    return cpufreq_register_notifier(&sep_i2s_freq_transition,
                     CPUFREQ_TRANSITION_NOTIFIER);
}

static inline void sep_i2s_cpufreq_deregister(void)
{
    cpufreq_unregister_notifier(&sep_i2s_freq_transition,
                    CPUFREQ_TRANSITION_NOTIFIER);
}

#else
static inline int sep_i2s_cpufreq_register(void)
{
    return 0;
}

static inline void sep_i2s_cpufreq_deregister(void)
{
}
#endif
/*************add by zy**************/

static int sep0611_i2s_probe(struct platform_device *pdev, struct snd_soc_dai *dai)
{
	struct clk *clk;

	alsa_dbg("%s\n", __func__);
	
	i2s_info.dev = &pdev->dev;
	i2s_info.base = ioremap(I2S_BASE, 0x200); 	/*get the virtual address*/
	if(i2s_info.base == 0){
		printk("ERR: failed to ioremap i2s' io memory region\n");
		return -ENOENT;
	}

	clk = clk_get(NULL, "i2s");
	if(clk == NULL){
		printk("ERR:failed to get i2s clock!\n");
		return -ENODEV;
	}
	clk_enable(clk);

	i2s_info.clk = clk;
	i2s_info.clk_rate = clk_get_rate(clk);
	/*************add by zy**************/
	sep_i2s_cpufreq_register();
	/*************add by zy**************/
	init_i2s_gpio();
	i2s_dma_init();
	
	writel(0x00, i2s_info.base + I2S_IER); 	/*disable i2s*/ 
	
  	return 0;
}

struct snd_soc_dai_ops sep0611_i2s_dai_ops = {
	.startup 	= sep0611_i2s_startup,
	.shutdown 	= sep0611_i2s_shutdown,
	.trigger 	= sep0611_i2s_trigger,
	.hw_params 	= sep0611_i2s_hw_params,
	.set_fmt 	= sep0611_i2s_set_dai_fmt,
	.set_sysclk = sep0611_i2s_set_dai_sysclk,
};

struct snd_soc_dai sep0611_i2s_dai[] = {
	[0] = {
		.name 		= "SEP0611-I2S",
		.id 		= 0,
	  	.probe 		= sep0611_i2s_probe,
		.suspend	= sep0611_i2s_suspend,
		.resume 	= sep0611_i2s_resume,
		.ops 		= &sep0611_i2s_dai_ops,
		.playback 	= {
			.channels_min 	= 2,
		 	.channels_max 	= 2,
		   	.rates 			= SEP0611_I2S_RATES,
		 	.formats 		= SEP0611_I2S_TX_FMTS,
		},
		.capture 	= {
		   	.channels_min 	= 2,
		  	.channels_max 	= 2,
		  	.rates 			= SEP0611_I2S_RATES,
		 	.formats 		= SEP0611_I2S_RX_FMTS,
		},
	}
};
EXPORT_SYMBOL_GPL(sep0611_i2s_dai);

static int __init sep0611_i2s_init(void)
{
	return snd_soc_register_dais(sep0611_i2s_dai, ARRAY_SIZE(sep0611_i2s_dai));
}
module_init(sep0611_i2s_init);

static void __exit sep0611_i2s_exit(void)
{
	snd_soc_unregister_dais(sep0611_i2s_dai, ARRAY_SIZE(sep0611_i2s_dai));
	/*************add by zy**************/
	sep_i2s_cpufreq_deregister();
	/*************add by zy**************/
}
module_exit(sep0611_i2s_exit);

/* Module information */
MODULE_AUTHOR("chenguangming@wxseuic.com");
MODULE_DESCRIPTION("SEP0611 ASoC I2S Interface");
MODULE_LICENSE("GPL");

