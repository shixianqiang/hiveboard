/*
 * kernel/sound/soc/sep/cs3700.c
 *        2011-11-14
 * Based on wolfson's cs3700.c code
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/initval.h>
#include <sound/tlv.h>

#include <board/board.h>
#include "sep0611-i2s.h"
#include "alsa_debug.h"
#include "cs3700.h"

#define AUDIO_NAME "CS3700"
#define CS3700_VERSION "1.0"

#define CS3700_NUM_REG 116

#define cs3700_write_reg(a, b) cs3700_write(codec, a, b)

/* codec private data */
struct cs3700_priv {
	unsigned int sysclk;
	struct snd_soc_codec codec;
	u16 reg_cache[CS3700_NUM_REG];
};

/* If the i2c layer weren't so broken, we could pass this kind of data
   around */
struct snd_soc_device *cs3700_socdev;	

static int cs3700_i2c_write(struct i2c_client *client, const char *buf, int count);

/*
 * cs3700 register cache
 * We can't read the CS3700 register space when we
 * are using 2 wire for device control, so we cache them instead.
 */
static const u16 cs3700_reg[] = {
	0x0000, 0x0000, 0x0000, 0x0000,  /* 0  */
	0x0000, 0x0000, 0x0000, 0x0000,  /* 4  */
	0x0000, 0x0000, 0x0000, 0x0000,  /* 8  */
	0x0000, 0x0000, 0x0000, 0x0000,  /* 18 */
	0x0000, 0x0000, 0x0000, 0x0000,  /* 20 */
	0x0000, 0x0000, 0x0000, 0x0000,  /* 28 */
	0x0000, 0x0000, 0x0000, 0x0000,  /* 30 */
	0x0000, 0x0000, 0x0000, 0x0000,  /* 38 */
	0x0000, 0x0000, 0x0000, 0x0000,  /* 40 */
	0x0000, 0x0000, 0x0000, 0x0000,  /* 48 */
	0x0000, 0x0000, 0x0000, 0x0000,	 /* 50 */
	0x0000, 0x0000, 0x0000, 0x0000,	 /* 58 */
	0x0000, 0x0000, 0x0000, 0x0000,	 /* 60 */
	0x0000, 0x0000, 0x0000, 0x0000,	 /* 68 */
};

/*
 * read cs3700 register cache
 */
static inline unsigned int cs3700_read_reg_cache(struct snd_soc_codec *codec,
		unsigned int reg)
{
	u16 *cache = codec->reg_cache;

	if (reg > CS3700_NUM_REG)
		return -1;
	return cache[reg];
}

/*
 * write cs3700 register cache
 */
static inline void cs3700_write_reg_cache(struct snd_soc_codec *codec,
		unsigned int reg, unsigned int value)
{
	u16 *cache = codec->reg_cache;

	if (reg > CS3700_NUM_REG)
		return -1;
	cache[reg] = value;
}

static int cs3700_write(struct snd_soc_codec *codec, unsigned int reg,
		unsigned int value)
{
	u8 data[3];

	/* data is
	 *	 D23..D16 CS3700register offset
	 *   D15..D8  register high data
	 *   D8...D0  register low  data
	 */
	data[0] = reg & 0x00ff;
	data[1] = (value >> 8) & 0x00ff;
	data[2] = value & 0x00ff;

	cs3700_write_reg_cache (codec, reg, value);
	if (codec->hw_write(codec->control_data, data, 3) == 3)
		return 0;
	else
		return -EIO;
}

static unsigned int cs3700_read_reg(struct snd_soc_codec *codec, unsigned char reg)
{
	u16 value = -1;	
	unsigned char buf[2];
	struct i2c_client *i2c = codec->control_data;

	if (codec == NULL || codec->control_data == NULL) {
		printk("codec or control data is NULL");
		return -EIO;
	}

	buf[0] = reg;
	i2c_master_send(i2c, buf, 1); 
	i2c_master_recv(i2c, buf, 2);
	
	value = (buf[0] << 8) | buf[1];

	return value; 
}

int cs3700_update_bits(struct snd_soc_codec *codec, unsigned int reg,
		unsigned int mask, unsigned int value)
{
	int change;
	unsigned int old, new;

	old = cs3700_read_reg_cache(codec, reg);
	new = (old & ~mask) | value;
	change = old != new;
	if (change)
		cs3700_write(codec, reg, new);

	return change;
}

/*
 * CS3700 Controls
 */

static const char *cs3700_spkn_source_sel[] = {"RN", "RP", "LN"};
static const char *cs3700_spk_pga[] = {"Vmid","HP mixer","SPK mixer","Mono Mixer"};
static const char *cs3700_hpl_pga[]  = {"Vmid","HPL mixer"};
static const char *cs3700_hpr_pga[]  = {"Vmid","HPR mixer"};
static const char *cs3700_mono_pga[] = {"Vmid","HP mixer","SPK mixer","Mono Mixer"};
static const char *cs3700_amp_type_sel[] = {"Class AB","Class D"};
static const char *cs3700_mic_boost_sel[] = {"Bypass","20db","30db","40db"};
static const char *cs3700_eq_sel[] = {"NORMAL", "CLUB","DANCE", "LIVE","POP",
					"ROCK", "OPPO", "TREBLE", "BASS"};    

static const struct soc_enum cs3700_enum[] = {

};

static const struct snd_kcontrol_new cs3700_snd_controls[] = {

};

/* add non dapm controls */
static int cs3700_add_controls(struct snd_soc_codec *codec)
{
	int err, i;

	for (i = 0; i < ARRAY_SIZE(cs3700_snd_controls); i++) {
		err = snd_ctl_add(codec->card,
				snd_soc_cnew(&cs3700_snd_controls[i],codec, NULL));
		if (err < 0)
			return err;
	}
	return 0;
}

/* Left Headphone Mixers */
static const struct snd_kcontrol_new cs3700_hpl_mixer_controls[] = {

};

/* Right Headphone Mixers */
static const struct snd_kcontrol_new cs3700_hpr_mixer_controls[] = {

};

//Left Record Mixer
static const struct snd_kcontrol_new cs3700_captureL_mixer_controls[] = {

};


//Right Record Mixer
static const struct snd_kcontrol_new cs3700_captureR_mixer_controls[] = {

};

/* Speaker Mixer */
static const struct snd_kcontrol_new cs3700_speaker_mixer_controls[] = {

};


/* Mono Mixer */
static const struct snd_kcontrol_new cs3700_mono_mixer_controls[] = {

};

/* mono output mux */
static const struct snd_kcontrol_new cs3700_mono_mux_controls =
SOC_DAPM_ENUM("Route", cs3700_enum[4]);

/* speaker left output mux */
static const struct snd_kcontrol_new cs3700_hp_spk_mux_controls =
SOC_DAPM_ENUM("Route", cs3700_enum[1]);


/* headphone left output mux */
static const struct snd_kcontrol_new cs3700_hpl_out_mux_controls =
SOC_DAPM_ENUM("Route", cs3700_enum[2]);

/* headphone right output mux */
static const struct snd_kcontrol_new cs3700_hpr_out_mux_controls =
SOC_DAPM_ENUM("Route", cs3700_enum[3]);

static const struct snd_soc_dapm_widget cs3700_dapm_widgets[] = {

};

static const struct snd_soc_dapm_route audio_map[] = {
	
};

static int cs3700_add_widgets(struct snd_soc_codec *codec)
{
	snd_soc_dapm_new_controls(codec, cs3700_dapm_widgets,
				ARRAY_SIZE(cs3700_dapm_widgets));
	snd_soc_dapm_add_routes(codec, audio_map, ARRAY_SIZE(audio_map));
	snd_soc_dapm_new_widgets(codec);

	return 0;
}

/*************************** end dapm *******************************/

/* PLL divisors */
struct _pll_div {
	u32 pll_in;
	u32 pll_out;
	u16 regvalue;
};

static const struct _pll_div codec_pll_div[] = {
	
};

/**
 * Our sep0611 must use CPU's BCLK as the source clock of cs3700's internal
 * PLL to match the synchronism of CPU and CODEC when CODEC works in slave 
 * mode, or it will couse noise!!!
 */
static const struct _pll_div codec_bclk_pll_div[] = {

};

static int cs3700_set_dai_pll(struct snd_soc_dai *codec_dai,
		int pll_id, unsigned int freq_in, unsigned int freq_out)
{
	int i;
	int ret = -EINVAL;
	struct snd_soc_codec *codec = codec_dai->codec;

	return 0;
}

struct _coeff_div {
	u32 mclk;
	u32 rate;
	u16 fs;
	u16 regvalue;
};

/* codec hifi mclk (after PLL) clock divider coefficients */
static const struct _coeff_div coeff_div[] = {

};

static int get_coeff(int mclk, int rate)
{
	int i;
	
	//printk("get_coeff mclk=%d,rate=%d\n",mclk,rate);

	for (i = 0; i < ARRAY_SIZE(coeff_div); i++) {
		if (coeff_div[i].rate == rate && coeff_div[i].mclk == mclk)
			return i;
	}
	return -EINVAL;
}

/*
 * Clock after PLL and dividers
 */
 /*in this driver, you have to set sysclk to be 24576000,
 * but you don't need to give a clk to be 24576000, our 
 * internal pll will generate this clock! so it won't make
 * you any difficult.
 */
static int cs3700_set_dai_sysclk(struct snd_soc_dai *codec_dai,
		int clk_id, unsigned int freq, int dir)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	struct cs3700_priv *cs3700 = codec->private_data;

	if ((freq >= (256 * 8000)) && (freq <= (512 * 48000))) {
		cs3700->sysclk = freq;
		return 0;	
	}
	
	printk("unsupported sysclk freq %u for audio i2s\n", freq);
	printk("Set sysclk to 24.576Mhz by default\n");	

	cs3700->sysclk = 24576000;
	return 0;
}

static int cs3700_set_dai_fmt(struct snd_soc_dai *codec_dai,
		unsigned int fmt)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	u16 iface = 0;

	/* set master/slave audio interface */
	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM:
		iface = 0x0000;
		break;
	case SND_SOC_DAIFMT_CBS_CFS:
		iface = 0x8000;
		break;
	default:
		return -EINVAL;
	}

	/* interface format */
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
		iface |= 0x0000;
		break;
	case SND_SOC_DAIFMT_RIGHT_J:
		iface |= 0x0001;
		break;
	case SND_SOC_DAIFMT_LEFT_J:
		iface |= 0x0002;
		break;
	case SND_SOC_DAIFMT_DSP_A:
		iface |= 0x0003;
		break;
	case SND_SOC_DAIFMT_DSP_B:
		iface |= 0x4003;
		break;
	default:
		return -EINVAL;
	}

	/* clock inversion */
	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF:
		iface |= 0x0000;
		break;
	case SND_SOC_DAIFMT_IB_NF:
		iface |= 0x0100;
		break;
	default:
		return -EINVAL;
	}

	//cs3700_write(codec,RT5621_AUDIO_INTERFACE,iface);
	return 0;
}

static int cs3700_pcm_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params,
	struct snd_soc_dai *codec_dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_device *socdev = rtd->socdev;
	struct snd_soc_codec *codec = socdev ->card->codec;
	struct cs3700_priv *cs3700 = codec->private_data;
	u16 iface ;
	int coeff = get_coeff(cs3700->sysclk, params_rate(params));			//jgf

	//printk("cs3700_pcm_hw_params\n");
	if (coeff < 0)
		coeff = get_coeff(24576000, params_rate(params));	  /*if not set sysclk, default to be 24.576MHz*/

	/* bit size */
	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S16_LE:
		iface |= 0x0000;
		break;
	case SNDRV_PCM_FORMAT_S20_3LE:
		iface |= 0x0004;
		break;
	case SNDRV_PCM_FORMAT_S24_LE:
		iface |= 0x0008;
		break;
	case SNDRV_PCM_FORMAT_S32_LE:
		iface |= 0x000c;
		break;
	}

	/* set iface & srate */
	//cs3700_write(codec, RT5621_AUDIO_INTERFACE, iface);

	//if (coeff >= 0)
	//	cs3700_write(codec, RT5621_STEREO_AD_DA_CLK_CTRL, coeff_div[coeff].regvalue);

#if (CTRL_MODE != SLAVE_MODE)
	//cs3700_update_bits(codec, CS3700_IFACE, 0x040, 0x000);				/* Now cs3700 works in slaver */
	alsa_dbg("cs3700 works in slaver\n");
	return 0;
#else
	//cs3700_update_bits(codec, CS3700_IFACE, 0x040, 0x040);				/* Now cs3700 works in master */
	alsa_dbg("cs3700 works in master\n");
#endif

	return 0;
}

static int cs3700_mute(struct snd_soc_dai *dai, int mute)
{
	return 0;
}

static int cs3700_set_audio_output(struct snd_soc_codec *codec)
{
	alsa_dbg("%s\n", __func__);

#if 1
	cs3700_write_reg(0x3A, 0x0030);				//cs3700 works in slave;16bit;44.1;SYSCLK=FLLout=11.2896M;FLLin=DACLK=44.1; 
	cs3700_write_reg(0x1E, 0x0082);				 
	cs3700_write_reg(0x01, 0x0100);				 
	cs3700_write_reg(0x1E, 0x009A);				 
	cs3700_write_reg(0x03, 0x0060);				//
	cs3700_write_reg(0x1E, 0x001A);				

	cs3700_write_reg(0x24, 0x0108);				//FLL config
	cs3700_write_reg(0x25, 0x0008);				//FLL config
	cs3700_write_reg(0x27, 0x0010);				//FLL ref=44.1K; N = 256; not use K;
	cs3700_write_reg(0x28, 0x0080);				//Fvco/8
	cs3700_write_reg(0x29, 0x0180);				//FLL ref < 48K; FLL ref = DACLRC;

	cs3700_write_reg(0x01, 0x0141);				//FLL enable 
	//mdelay(400);
	cs3700_write_reg(0x01, 0x0149);				//FLL enable 

	cs3700_write_reg(0x1E, 0x0002);				//------JGF change bit[8] and bit[7]!!! must be 0!!!
	cs3700_write_reg(0x01, 0x0049 | 0x5d);				 
	//cs3700_write_reg(0x01, 0x0049);				 

	//cs3700_write_reg(0x02, 0xC180 | cs3700_reg[0x02]);	//??????????????is a bug! bit[8] and bit[7] must enable!!!
	cs3700_write_reg(0x02, 0x8000);	//??????????????is a bug! bit[8] and bit[7] must enable!!!
	//cs3700_write_reg(0x02, 0xC180);			//??????????????is a bug! bit[8] and bit[7] must enable!!!
	cs3700_write_reg(0x03, 0x00EF);				//

	cs3700_write_reg(0x04, 0x4011);				//
	cs3700_write_reg(0x05, 0x4000);				//

	cs3700_write_reg(0x06, 0x0100);				//slave mode;BCLK in; SYSCLK=FLLout;
	cs3700_write_reg(0x07, 0x0000);				//
	cs3700_write_reg(0x09, 0x0020);				//DAC input

	cs3700_write_reg(0x0A, 0x0020);				//256*Fs;
	cs3700_write_reg(0x0B, 0x01C0);				//JGF2012-05-16 0x01C0		//0db
	cs3700_write_reg(0x0C, 0x01C0);				//

	cs3700_write_reg(0x2C, 0x0150);				//
	cs3700_write_reg(0x2D, 0x0150);				//

	cs3700_write_reg(0x33, 0x0079);				//enable (old mute LOUT1)
	cs3700_write_reg(0x34, 0x0079);				//3db

	cs3700_write_reg(0x35, 0x0139);				//
	cs3700_write_reg(0x36, 0x0139);				//

	cs3700_write_reg(0x3A, 0x009C);				//
	//mdelay(400);
	cs3700_write_reg(0x3A, 0x00CC);				//
	cs3700_write_reg(0x3A, 0x00C8);				//
	cs3700_write_reg(0x3A, 0x00C0);				//
#endif

	return 0;
}

static int cs3700_set_audio_input(struct snd_soc_codec *codec)
{
	alsa_dbg("%s\n", __func__);

#if 1
	//test capture
	cs3700_write_reg(0x01, 0x001D);				//cs3700 works in slave;16bit;44.1;SYSCLK=FLLout=11.2896M;FLLin=DACLK=44.1; 
	//cs3700_write_reg(0x02, 0xC1AA | cs3700_reg[0x02]);	//enable LOUT1; mute LOUT1 => disable LOUT1 
	cs3700_write_reg(0x02, 0xC1AA);				//enable LOUT1; mute LOUT1 => disable LOUT1 

	cs3700_write_reg(0x15, 0x0060);				 
	cs3700_write_reg(0x16, 0x0118);				//IN1_L 6db -> 12db	###########
	//cs3700_write_reg(0x0F, 0x01DB);			//10db 
	cs3700_write_reg(0x0F, 0x01F0);				//17.625db 
	cs3700_write_reg(0x1A, 0x0040);				 
	cs3700_write_reg(0x18, 0x0043);				//############ IN2_L boost 6db  ; importmant

	cs3700_write_reg(0x04, 0x0010);				//in -> ADCL/ADLR 

	cs3700_write_reg(0x24, 0x0108);				//FLL config
	cs3700_write_reg(0x25, 0x0008);				//FLL config
	cs3700_write_reg(0x27, 0x0010);				//FLL ref=44.1K; N = 256; not use K;
	cs3700_write_reg(0x28, 0x0080);				//Fvco/8
	cs3700_write_reg(0x29, 0x0180);				//FLL ref < 48K; FLL ref = DACLRC;

	mdelay(10);

	cs3700_write_reg(0x01, 0x005D);				//enable FLL 

	cs3700_write_reg(0x05, 0x4000);				//

	cs3700_write_reg(0x06, 0x0100);				//slave mode;BCLK in; SYSCLK=FLLout;
	cs3700_write_reg(0x07, 0x0000);				//
	cs3700_write_reg(0x08, 0x0000);				//ADC input

	cs3700_write_reg(0x0A, 0x0020);				//256*Fs;

#endif

	return 0;
}

//static int cs3700_change_vol(int vol)
static int cs3700_change_vol(char vol)
{
	//int volume = 0;
	char volume = vol;
	struct snd_soc_codec *codec = cs3700_socdev->card->codec;
	
	/* TODO (FIX ME) */
	switch (volume) {
		case '7':
			cs3700_write_reg(0x0B, 0x01C0);
			cs3700_write_reg(0x0C, 0x01C0);
			break;
		case '6':
			cs3700_write_reg(0x0B, 0x01B8);
			cs3700_write_reg(0x0C, 0x01B8);
			break;
		case '5':
			cs3700_write_reg(0x0B, 0x01B0);
			cs3700_write_reg(0x0C, 0x01B0);
			break;
		case '4':
			cs3700_write_reg(0x0B, 0x01A8);
			cs3700_write_reg(0x0C, 0x01A8);
			break;
		case '3':
			cs3700_write_reg(0x0B, 0x01A0);
			cs3700_write_reg(0x0C, 0x01A0);
			break;
		case '2':
			cs3700_write_reg(0x0B, 0x0190);
			cs3700_write_reg(0x0C, 0x0190);
			break;
		case '1':
			cs3700_write_reg(0x0B, 0x0170);
			cs3700_write_reg(0x0C, 0x0170);
			break;
		case '0':
			cs3700_write_reg(0x0B, 0x0100);
			cs3700_write_reg(0x0C, 0x0100);
			break;
		default:
			break;
	}
}

static int cs3700_pcm_prepare(struct snd_pcm_substream *substream,
		struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_device *socdev = rtd->socdev;
	struct snd_soc_codec *codec = socdev->card->codec;

	alsa_dbg("%s\n", __func__);

	if(substream->stream == SNDRV_PCM_STREAM_PLAYBACK){
		cs3700_set_audio_output(codec);			//Playback
	}
	else{
		cs3700_set_audio_input(codec);			//Capture
	}

	return 0;
}

/* TODO: liam need to make this lower power with dapm */
static int cs3700_set_bias_level(struct snd_soc_codec *codec, 
		enum snd_soc_bias_level level)
{
	switch (level) {
		case SND_SOC_BIAS_ON:
			break;
		case SND_SOC_BIAS_PREPARE:
			break;
		case SND_SOC_BIAS_STANDBY:
			break;
		case SND_SOC_BIAS_OFF:

			//cs3700_update_bits(codec, 0x02, 0x8080, 0x8080);
			//cs3700_update_bits(codec, 0x04, 0x8080, 0x8080);
			//cs3700_write(codec, 0x3e, 0x0000);
			//cs3700_write(codec, 0x3c, 0x0000);
			//cs3700_write(codec, 0x3a, 0x0000);
			break;		
	}
	codec->bias_level = level;

	alsa_dbg("%s\n", __func__);

	return 0;
}

static void cs3700_shutdown(struct snd_pcm_substream *substream, struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_device *socdev = rtd->socdev;
	struct snd_soc_codec *codec = socdev->card->codec;

	alsa_dbg("%s\n", __func__);

	/*
	 * You can do something here to shutdown the codec!
	 * but now, we already have the cs3700_set_bias_level function!
	 * Intend to privent the pop!
	 */
	//cs3700_set_bias_level(codec, SND_SOC_BIAS_OFF);
}

#define CS3700_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 |\
		SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_22050 |SNDRV_PCM_RATE_32000| SNDRV_PCM_RATE_44100 | \
		SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_88200 | SNDRV_PCM_RATE_96000)

#define CS3700_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE |\
		SNDRV_PCM_FMTBIT_S24_LE)

struct snd_soc_dai_ops cs3700_dai_ops = {
	.prepare		= cs3700_pcm_prepare,	
	.hw_params		= cs3700_pcm_hw_params,
	.shutdown 		= cs3700_shutdown,
	.digital_mute 	= cs3700_mute,
	.set_fmt 		= cs3700_set_dai_fmt,
	.set_sysclk		= cs3700_set_dai_sysclk,
};

struct snd_soc_dai cs3700_dai = {
	.name = "CS3700",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 1,
		.channels_max = 2,
		.rates = CS3700_RATES,
		.formats = CS3700_FORMATS,
	},
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 2,
		.rates = CS3700_RATES,
		.formats = CS3700_FORMATS,
	},
	.ops = &cs3700_dai_ops,
};

struct snd_soc_dai *snd_soc_dai = &cs3700_dai;
EXPORT_SYMBOL_GPL(cs3700_dai);

#ifdef CONFIG_PM
static int cs3700_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct snd_soc_device *socdev = platform_get_drvdata(pdev);
	struct snd_soc_codec *codec = socdev->card->codec;

	cs3700_set_bias_level(codec, SND_SOC_BIAS_OFF);
	return 0;
}

static int cs3700_resume(struct platform_device *pdev)
{
	struct snd_soc_device *socdev = platform_get_drvdata(pdev);
	struct snd_soc_codec *codec = socdev->card->codec;
	int i;
	u8 data[3];
	u16 *cache = codec->reg_cache;

	/* Sync reg_cache with the hardware */
	for (i = 0; i < CS3700_NUM_REG; i++) {
		data[0] = i << 1;
		data[1] = ((cache[i] & 0xFF00) >> 8);
		data[2] = cache[i] & 0x00ff;
		codec->hw_write(codec->control_data, data, 3);
	}

	cs3700_set_bias_level(codec, SND_SOC_BIAS_STANDBY);

	return 0;
}
#else
#define cs3700_suspend NULL
#define cs3700_resume  NULL
#endif

static void sep0611_cs3700_init(struct snd_soc_codec *codec)
{
	/*You can do something here to init the codec!*/
	cs3700_write_reg(0x00, 0xFFFF);				//reset
}

static int cs3700_init(struct snd_soc_device *socdev)
{
	struct snd_soc_codec *codec = socdev->card->codec;	
	int ret = 0;

	codec->name  	= "CS3700";
	codec->owner 	= THIS_MODULE;
	codec->dai   	= &cs3700_dai;
	codec->num_dai 	= 1;
	codec->read  	= cs3700_read_reg_cache;
	codec->write 	= cs3700_write;
	codec->bias_level	  = SND_SOC_BIAS_OFF;
	codec->set_bias_level = cs3700_set_bias_level;
	codec->reg_cache_size = ARRAY_SIZE(cs3700_reg);
	codec->reg_cache = kmemdup(cs3700_reg, sizeof(cs3700_reg), GFP_KERNEL);

	if (codec->reg_cache == NULL)
		return -ENOMEM;

	/* register pcms */
	ret = snd_soc_new_pcms(socdev, SNDRV_DEFAULT_IDX1, SNDRV_DEFAULT_STR1);
	if (ret < 0) {
		printk("CS3700: failed to create pcms\n");
		goto pcm_err;
	}

	sep0611_cs3700_init(codec);				/* init cs3700 registers */

	/* dapm controls */
	cs3700_add_controls(codec);				/* We not use dapm function */
	cs3700_add_widgets(codec);

	/* init card */
	ret = snd_soc_init_card(socdev);
	if (ret < 0) {
		printk("CS3700: failed to register card\n");
		goto card_err;
	}

	return ret;

card_err:
	snd_soc_dapm_free(socdev);
	snd_soc_free_pcms(socdev);
pcm_err:
	kfree(codec->reg_cache);
	return ret;
}


static int cs3700_i2c_probe(struct i2c_client *i2c,
		const struct i2c_device_id *id)
{
	struct snd_soc_device *socdev = cs3700_socdev;
	struct snd_soc_codec *codec = socdev->card->codec;
	int ret;

	alsa_dbg("%s\n", __func__);

	i2c_set_clientdata(i2c, codec);	
	codec->control_data = i2c;

	ret = cs3700_init(socdev);
	if (ret < 0)
		pr_err("Failed to initialise CS3700\n");

	return ret;
}

static int cs3700_i2c_remove(struct i2c_client *client)
{
	struct snd_soc_codec *codec = i2c_get_clientdata(client);
	kfree(codec->reg_cache);
	return 0;
}

static int cs3700_i2c_write(struct i2c_client *client, const char *buf, int count)
{
	/* You can do something here! */	

	return i2c_master_send(client, buf, count);
}

#ifdef CONFIG_PM
static int cs3700_i2c_suspend(struct i2c_client *client, pm_message_t msg)
{
	return snd_soc_suspend_device(&client->dev);
}

static int cs3700_i2c_resume(struct i2c_client *client)
{
	return snd_soc_resume_device(&client->dev);
}
#else
#define cs3700_i2c_suspend NULL
#define cs3700_i2c_resume NULL
#endif

static const struct i2c_device_id cs3700_i2c_id[] = {
	{"CS3700", 0},
};
MODULE_DEVICE_TABLE(i2c, cs3700_i2c_id);

static struct i2c_driver cs3700_i2c_driver = {
	.driver = {
		.name 	= "CS3700 I2C Codec",
		.owner 	= THIS_MODULE,
	},
	.id_table 	= cs3700_i2c_id,
	.probe 		= cs3700_i2c_probe,
	.remove 	= cs3700_i2c_remove,
};

static int cs3700_show_regs(void)
{
	struct snd_soc_codec *codec = cs3700_socdev->card->codec;
	unsigned char i;

	for (i = 0; i < 116; i++ ) {
		printk("cs3700_reg[%02X] = 0x%04X\n", i, cs3700_read_reg(codec, i));	/* read cs3700 regs */
	}

	return 0;
}

static int cs3700_show_regs_cash(void)
{
	struct snd_soc_codec *codec = cs3700_socdev->card->codec;
	int i;

	for (i = 0; i < 59; i++) {
		printk("cs3700_reg[%02d] = 0x%04x\n", i, cs3700_read_reg_cache(codec, i));
	}

	return 0;
}

static int cs3700_probe(struct platform_device *pdev)
{
	struct snd_soc_device *socdev = platform_get_drvdata(pdev);
	struct snd_soc_codec *codec;
	struct cs3700_priv *cs3700;
	int ret = 0;

	alsa_dbg("Enter::%s----%d\n", __func__, __LINE__);
	printk("CS3700 Audio Codec %s\n", CS3700_VERSION);

	codec = kzalloc(sizeof(struct snd_soc_codec), GFP_KERNEL);
	if (codec == NULL)
		return -ENOMEM;

	cs3700 = kzalloc(sizeof(struct cs3700_priv), GFP_KERNEL);
	if (cs3700 == NULL) {
		kfree(codec);
		return -ENOMEM;
	}
	
	printk("-----------------------\n");		//JGF

	codec->private_data = cs3700;
	socdev->card->codec = codec;

	mutex_init(&codec->mutex);
	INIT_LIST_HEAD(&codec->dapm_widgets);		/* must have, or will be err!!! */
	INIT_LIST_HEAD(&codec->dapm_paths);

	cs3700_socdev = socdev;						/* save the snd_soc_device, then we can use it anywhere */


	codec->hw_write = (hw_write_t)cs3700_i2c_write;

	ret = i2c_add_driver(&cs3700_i2c_driver);
	if (ret != 0){
		printk(KERN_ERR "can't add i2c driver");
		kfree(codec->private_data);
		kfree(codec);
	}

	return ret;
}

static int cs3700_remove(struct platform_device *pdev)
{
	struct snd_soc_device *socdev = platform_get_drvdata(pdev);

	snd_soc_free_pcms(socdev);
	snd_soc_dapm_free(socdev);

	return 0;
}

struct snd_soc_codec_device soc_codec_dev_cs3700 = {
	.probe		= cs3700_probe,
	.remove 	= cs3700_remove,
	.suspend 	= cs3700_suspend,
	.resume		= cs3700_resume,
};
EXPORT_SYMBOL_GPL(soc_codec_dev_cs3700);

static int __init cs3700_modinit(void)
{
	return  snd_soc_register_dai(&cs3700_dai);
}
module_init(cs3700_modinit);

static void __exit cs3700_exit(void)
{
	snd_soc_unregister_dai(&cs3700_dai);
}
module_exit(cs3700_exit);

MODULE_DESCRIPTION("ASoC CS3700 driver");
MODULE_AUTHOR("juguofeng <juguofeng@foxmail.com");
MODULE_LICENSE("GPL");

#define _DEBUG_CS3700 1

#if _DEBUG_CS3700
int test_open(struct inode *inode, struct file *filp)
{
	return 0;
}

ssize_t test_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
	//unsigned char c = 0;
	//int vol = 0;
	char vol = 0;
	int ret;

	//ret = copy_from_user(&c, buff, 1);
	//ret = copy_from_user(&vol, buff, sizeof(int));
	ret = copy_from_user(&vol, buff, sizeof(char));
	if(ret < 0){
		printk("copy_from_user err!\n");
		return ret;
	}
	//printk("[kernel] vol frome use is : %d\n", vol);
	printk("[kernel] vol frome use is : %c\n", vol);
#if 0
	if (c == '1'){
	  cs3700_show_regs();
	  printk("==========================\n");
	  cs3700_show_regs_cash();
	}
#endif
	cs3700_change_vol(vol);
	
	return 1;
}

int test_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static struct file_operations test_fops = {
	.owner   = THIS_MODULE,
	.open    = test_open,
	.write   = test_write,
	.release = test_release,
};

struct miscdevice test_misc_dev;
static int __init sep_ts_test_init(void)
{
	int ret;

	test_misc_dev.minor = MISC_DYNAMIC_MINOR;
	//test_misc_dev.name = "cs3700_test";
	test_misc_dev.name = "cs3700_volume";
	test_misc_dev.fops = &test_fops;

	ret = misc_register(&test_misc_dev);
	if (ret < 0){
		printk("misc_register err!\n");
		return ret;
	}

	return 0;
}

static void __exit sep_ts_test_exit(void)
{
}

late_initcall(sep_ts_test_init);
module_exit(sep_ts_test_exit);

#endif

