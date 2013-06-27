/* linux/sound/soc/sep/sep0611_board.c
 *
 * Copyright (c) 2009-2011 SEUIC
 *  cgm <chenguangming@wxseuic.com>
 *
 * Southeast University ASIC SoC support
 *
 * SoC audio for SEP0611 Board
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  05-31-2011	cgm initial version
 *  04-13-2012	jgf new for cs3700
 *
 */
 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/platform_device.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>

#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/irqs.h>

#include <board/board.h>

#include "sep0611-pcm.h"
#include "sep0611-i2s.h"
#include "i2s_dma.h"
#include "cs3700.h"

#include "alsa_debug.h"

#ifdef SEP0611_AUDIO_EN
#define SEP0611_SPK_CTL 0

static __inline void sep0611_codec_gpio_init(void)
{
	alsa_dbg("%s\n", __func__);
	
    sep0611_gpio_cfgpin(SEP0611_AUDIO_EN, SEP0611_GPIO_IO);	/* GPIO */
    sep0611_gpio_dirpin(SEP0611_AUDIO_EN, SEP0611_GPIO_OUT);/* output */
}

static __inline void sep0611_codec_enable(bool enable)
{
	alsa_dbg("%s\n", __func__);
    
	if(enable)
    	sep0611_gpio_setpin(SEP0611_AUDIO_EN, GPIO_HIGH);	/* codec power on */
	else
    	sep0611_gpio_setpin(SEP0611_AUDIO_EN, GPIO_LOW);	/* codec power off */
}

#endif

#ifdef SEP0611_SPK_CTL
static __inline void sep0611_spk_gpio_init(void)
{
	alsa_dbg("%s\n", __func__);

    sep0611_gpio_cfgpin(SEP0611_SPK_CTL, SEP0611_GPIO_IO);	/* GPIO */
    sep0611_gpio_dirpin(SEP0611_SPK_CTL, SEP0611_GPIO_OUT);	/* output */		
}

void sep0611_spk_out(bool enable)
{
	alsa_dbg("%s\n", __func__);
    
	if(enable)	
		sep0611_gpio_setpin(SEP0611_SPK_CTL, GPIO_HIGH);	/* speaker power on */
	else
    	sep0611_gpio_setpin(SEP0611_SPK_CTL, GPIO_LOW);		/* speaker power off */
}

#endif

#ifdef SEP0611_SPK_CTL
static int sep0611_board_startup(struct snd_pcm_substream *substream)
{
	alsa_dbg("%s\n", __func__);
	sep0611_gpio_setpin(SEP0611_SPK_CTL, GPIO_HIGH);	/* speaker power on */

	return 0;
}

static void sep0611_board_shutdown(struct snd_pcm_substream *substream)
{
	alsa_dbg("%s\n", __func__);
    sep0611_gpio_setpin(SEP0611_SPK_CTL, GPIO_LOW);		/* speaker power off */
}

#endif
static int sep0611_board_hw_params(struct snd_pcm_substream *substream, struct snd_pcm_hw_params *params)
{
	alsa_dbg("%s\n", __func__);
	
	return 0;
}

static struct snd_soc_ops sep0611_ops = {
//	.startup 	= sep0611_board_startup,
	.hw_params 	= sep0611_board_hw_params,
//	.shutdown 	= sep0611_board_shutdown,
};

static int sep0611_cs3700_init(struct snd_soc_codec *codec)
{
	alsa_dbg("%s\n", __func__);
	
	return 0;
}

extern struct snd_soc_platform sep0611_soc_platform;

/*
 * sep0611 digital audio interface glue - connects codec <--> CPU
 */
static struct snd_soc_dai_link sep0611_dai[] = {
	[0]={
		.name		= "sep0611-dai",
		.stream_name= "sep0611-audio",
		.cpu_dai 	= &sep0611_i2s_dai[0],
		.codec_dai 	= &cs3700_dai,
		.init 		= sep0611_cs3700_init,
		.ops 		= &sep0611_ops,
	},
};

static int sep0611_board_probe(struct platform_device *pdev)
{
	alsa_dbg("%s\n", __func__);

#ifdef SEP0611_AUDIO_EN
    sep0611_codec_gpio_init();
    sep0611_codec_enable(true);
#endif

#ifdef SEP0611_SPK_CTL
    sep0611_spk_gpio_init();
    sep0611_spk_out(false);
#endif

   	return 0;
}

static int sep0611_board_remove(struct platform_device *pdev)
{
#ifdef SEP0611_AUDIO_EN
	sep0611_codec_enable(false);
#endif

#ifdef SEP0611_SPK_CTL
	sep0611_spk_out(false);
#endif

	return 0;
}

#ifdef CONFIG_PM
static int sep0611_board_suspend_post(struct platform_device *pdev, pm_message_t state)
{
	alsa_dbg("%s\n", __func__);

#ifdef SEP0611_AUDIO_EN
	sep0611_codec_enable(false);
#endif

#ifdef SEP0611_SPK_CTL
	sep0611_spk_out(false);
#endif

	return 0;
}

static int sep0611_board_resume_pre(struct platform_device *pdev)
{
	alsa_dbg("%s\n", __func__);

#ifdef SEP0611_AUDIO_EN
	sep0611_codec_gpio_init();
	sep0611_codec_enable(true);
#endif

#ifdef SEP0611_SPK_CTL
	sep0611_spk_gpio_init();
	sep0611_spk_out(true);
#endif

	return 0;
}
#else

#define sep0611_board_suspend_post	NULL
#define sep0611_board_resume_pre	NULL

#endif


/*
 * sep0611 audio machine driver
 */
static struct snd_soc_card snd_soc_machine_sep0611 = {
	.platform 	= &sep0611_soc_platform,
	.name		= "sep0611_board",
   	.probe 		= sep0611_board_probe,
	.remove		= sep0611_board_remove,
	.suspend_post = sep0611_board_suspend_post,
	.resume_pre	= sep0611_board_resume_pre,
	.dai_link 	= sep0611_dai,
	.num_links 	= ARRAY_SIZE(sep0611_dai),
};

/*
 * sep0611 audio subsystem 
 */
static struct snd_soc_device sep0611_snd_devdata = {
	.card 		= &snd_soc_machine_sep0611,
	.codec_dev	= &soc_codec_dev_cs3700,
};

static struct platform_device *sep0611_snd_device;

static int __init sep0611_snd_init(void)
{
	int ret;
	struct snd_soc_dai_link *link = &sep0611_snd_devdata.card->dai_link[0];
	
	sep0611_snd_device = platform_device_alloc("soc-audio", -1);
	if (!sep0611_snd_device)
		return -ENOMEM;

	platform_set_drvdata(sep0611_snd_device, &sep0611_snd_devdata);
	sep0611_snd_devdata.dev = &sep0611_snd_device->dev;
	
	link->cpu_dai->dev = &sep0611_snd_device->dev;
	link->codec_dai->dev = &sep0611_snd_device->dev;
	
	ret = platform_device_add(sep0611_snd_device);

	if (ret)
		platform_device_put(sep0611_snd_device);

	return ret;
}

static void __exit sep0611_snd_exit(void)
{
	platform_device_unregister(sep0611_snd_device);
}

module_init(sep0611_snd_init);
module_exit(sep0611_snd_exit);

/* Module information */
MODULE_AUTHOR("juguofeng <jgfntu@163.com>");
MODULE_DESCRIPTION("ALSA SoC for SEP0611-CS3700 Board");
MODULE_LICENSE("GPL");
