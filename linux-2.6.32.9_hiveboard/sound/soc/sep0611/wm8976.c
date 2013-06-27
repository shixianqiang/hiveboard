/*
 * wm8976.c  --  WM8976 ALSA Soc Audio driver
 *
 * modified by cgm at 2011.5.1 to surpport sep0611 board
 *
 * Copyright 2007-9 Wolfson Microelectronics PLC.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/i2c.h>
#include <linux/spi/spi.h>
#include <linux/platform_device.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/initval.h>

#include "sep0611-i2s.h"

#include "wm8976.h"
#include "alsa_debug.h"

struct snd_soc_codec_device soc_codec_dev_wm8976;

/*
 * wm8976 register cache
 * We can't read the WM8976 register space when we are
 * using 2 wire for device control, so we cache them instead.
 */
static const u16 wm8976_reg[WM8976_CACHEREGNUM] = {
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0050, 0x0000, 0x0140, 0x0000,
	0x0000, 0x0000, 0x0000, 0x00ff,
	0x00ff, 0x0000, 0x0100, 0x00ff,
	0x00ff, 0x0000, 0x012c, 0x002c,
	0x002c, 0x002c, 0x002c, 0x0000,
	0x0032, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0038, 0x000b, 0x0032, 0x0000,
	0x0008, 0x000c, 0x0093, 0x00e9,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0033, 0x0010, 0x0010, 0x0100,
	0x0100, 0x0002, 0x0001, 0x0001,
	0x0039, 0x0039, 0x0039, 0x0039,
	0x0001, 0x0001,
};

/* Clock divider Id's */
enum wm8976_clk_id{
	WM8976_OPCLK_RATE,
	WM8976_BCLK_DIV,
};

enum wm8976_sysclk_src {
	WM8976_PLL,
	WM8976_MCLK,
};

/* codec private data */
struct wm8976_priv {
	enum snd_soc_control_type control_type;
	unsigned int f_pllout;
	unsigned int f_mclk;
	unsigned int f_256fs;
	unsigned int f_opclk;
	int mclk_idx;
	enum wm8976_sysclk_src sysclk;
	u16 reg_cache[WM8976_CACHEREGNUM];
};

static struct snd_soc_device *wm8976_socdev;

static int wm8976_i2c_write(struct i2c_client *client, const char *buf, int count);

/*
 * read wm8976 register cache
 */
static inline unsigned int wm8976_read_reg_cache(struct snd_soc_codec  *codec,
	unsigned int reg)
{
	u16 *cache = codec->reg_cache;
	if (reg == WM8976_RESET)
		return 0;
	if (reg >= WM8976_CACHEREGNUM)
		return -1;
	return cache[reg];
}

/*
 * write wm8976 register cache
 */
static inline void wm8976_write_reg_cache(struct snd_soc_codec  *codec,
	u16 reg, unsigned int value)
{
	u16 *cache = codec->reg_cache;
	if (reg >= WM8976_CACHEREGNUM)
		return;
	cache[reg] = value;
}

/*
 * write to the WM8976 register space
 */
static int wm8976_write(struct snd_soc_codec *codec, unsigned int reg,
	unsigned int value)
{
	u8 data[2];

	/* data is
	 *   D15..D9 WM8976 register offset
	 *   D8...D0 register data
	 */
	data[0] = (reg << 1) | ((value >> 8) & 0x0001);
	data[1] = value & 0x00ff;

	wm8976_write_reg_cache(codec, reg, value);
	if (codec->hw_write(codec->control_data, data, 2) == 2)
		return 0;
	else
		return -EIO;
}

#define wm8976_reset(c)	wm8976_write(c, WM8976_RESET, 0)

static const char *wm8976_companding[] = {"Off", "NC", "u-law", "A-law" };
static const char *wm8976_eqmode[] = {"Capture", "Playback" };
static const char *wm8976_bw[] = {"Narrow", "Wide" };
static const char *wm8976_eq1[] = {"80Hz", "105Hz", "135Hz", "175Hz" };
static const char *wm8976_eq2[] = {"230Hz", "300Hz", "385Hz", "500Hz" };
static const char *wm8976_eq3[] = {"650Hz", "850Hz", "1.1kHz", "1.4kHz" };
static const char *wm8976_eq4[] = {"1.8kHz", "2.4kHz", "3.2kHz", "4.1kHz" };
static const char *wm8976_eq5[] = {"5.3kHz", "6.9kHz", "9kHz", "11.7kHz" };
static const char *wm8976_alc[] = {"ALC both on", "ALC left only", "ALC right only", "Limiter" };

static const struct soc_enum wm8976_enum[] = {
	SOC_ENUM_SINGLE(WM8976_COMP, 1, 4, wm8976_companding), /* adc */
	SOC_ENUM_SINGLE(WM8976_COMP, 3, 4, wm8976_companding), /* dac */
	
	SOC_ENUM_SINGLE(WM8976_EQ1,  8, 2, wm8976_eqmode),
	SOC_ENUM_SINGLE(WM8976_EQ1,  5, 4, wm8976_eq1),
	
	SOC_ENUM_SINGLE(WM8976_EQ2,  8, 2, wm8976_bw),
	SOC_ENUM_SINGLE(WM8976_EQ2,  5, 4, wm8976_eq2),
	
	SOC_ENUM_SINGLE(WM8976_EQ3,  8, 2, wm8976_bw),
	SOC_ENUM_SINGLE(WM8976_EQ3,  5, 4, wm8976_eq3),
	
	SOC_ENUM_SINGLE(WM8976_EQ4,  8, 2, wm8976_bw),
	SOC_ENUM_SINGLE(WM8976_EQ4,  5, 4, wm8976_eq4),
	
	SOC_ENUM_SINGLE(WM8976_EQ5,  8, 2, wm8976_bw),
	SOC_ENUM_SINGLE(WM8976_EQ5,  5, 4, wm8976_eq5),
	
	SOC_ENUM_SINGLE(WM8976_ALC3, 8, 2, wm8976_alc),
};

static const struct snd_kcontrol_new wm8976_snd_controls[] = {
	SOC_SINGLE("Digital Loopback Switch", WM8976_COMP, 0, 1, 0),	/*R5[0]*/

	SOC_ENUM("ADC Companding", wm8976_enum[0]),						/*R5[2:1]*/
	SOC_ENUM("DAC Companding", wm8976_enum[1]),						/*R5[4:3]*/

	SOC_SINGLE("Jack Detection Enable", WM8976_JACK1, 6, 1, 0),		/*R9[6]*/

	SOC_DOUBLE("DAC Inversion Switch", WM8976_DAC, 0, 1, 1, 0),		/*R10[1:0]*/

	SOC_DOUBLE_R("Headphone Playback Volume", WM8976_DACVOLL, WM8976_DACVOLR, 0, 127, 0), /*R11,R12*/

	SOC_SINGLE("High Pass Filter Switch", WM8976_ADC, 8, 1, 0),		/*R14[8]*/
	SOC_SINGLE("High Pass Cut Off", WM8976_ADC, 4, 7, 0),			/*R14[6:4]*/
	SOC_SINGLE("ADC Inversion Switch", WM8976_ADC, 0, 1, 0),		/*R14[0]*/

	SOC_SINGLE("Capture Volume", WM8976_ADCVOL,  0, 127, 0),		/*R15*/

	SOC_ENUM("Equaliser Function", wm8976_enum[2]),					/*R18*/
	SOC_ENUM("EQ1 Cut Off", wm8976_enum[3]),
	SOC_SINGLE("EQ1 Volume", WM8976_EQ1,  0, 31, 1),

	SOC_ENUM("Equaliser EQ2 Bandwith", wm8976_enum[4]),				/*R19*/
	SOC_ENUM("EQ2 Cut Off", wm8976_enum[5]),
	SOC_SINGLE("EQ2 Volume", WM8976_EQ2,  0, 31, 1),

	SOC_ENUM("Equaliser EQ3 Bandwith", wm8976_enum[6]),				/*R20*/
	SOC_ENUM("EQ3 Cut Off", wm8976_enum[7]),
	SOC_SINGLE("EQ3 Volume", WM8976_EQ3,  0, 31, 1),

	SOC_ENUM("Equaliser EQ4 Bandwith", wm8976_enum[8]),				/*R21*/
	SOC_ENUM("EQ4 Cut Off", wm8976_enum[9]),
	SOC_SINGLE("EQ4 Volume", WM8976_EQ4,  0, 31, 1),

	SOC_ENUM("Equaliser EQ5 Bandwith", wm8976_enum[10]),			/*R22*/
	SOC_ENUM("EQ5 Cut Off", wm8976_enum[11]),
	SOC_SINGLE("EQ5 Volume", WM8976_EQ5,  0, 31, 1),

	SOC_SINGLE("DAC Playback Limiter Switch", WM8976_DACLIM1,  8, 1, 0),	/*R22[8]*/
	SOC_SINGLE("DAC Playback Limiter Decay", WM8976_DACLIM1,  4, 15, 0),	/*R22[7:4]*/
	SOC_SINGLE("DAC Playback Limiter Attack", WM8976_DACLIM1,  0, 15, 0),	/*R22[3:0]*/

	SOC_SINGLE("DAC Playback Limiter Threshold", WM8976_DACLIM2,  4, 7, 0),	/*R23[6:4]*/
	SOC_SINGLE("DAC Playback Limiter Boost", WM8976_DACLIM2,  0, 15, 0),	/*R23[3:0]*/

	SOC_SINGLE("ALC Enable Switch", WM8976_ALC1,  8, 1, 0),					/*R32*/
	SOC_SINGLE("ALC Capture Max Gain", WM8976_ALC1,  3, 7, 0),
	SOC_SINGLE("ALC Capture Min Gain", WM8976_ALC1,  0, 7, 0),

	SOC_SINGLE("ALC Capture ZC Switch", WM8976_ALC2,  8, 1, 0),				/*R33*/
	SOC_SINGLE("ALC Capture Hold", WM8976_ALC2,  4, 7, 0),
	SOC_SINGLE("ALC Capture Target", WM8976_ALC2,  0, 15, 0),

	SOC_ENUM("ALC Capture Mode", wm8976_enum[12]),							/*R34*/
	SOC_SINGLE("ALC Capture Decay", WM8976_ALC3,  4, 15, 0),
	SOC_SINGLE("ALC Capture Attack", WM8976_ALC3,  0, 15, 0),

	SOC_SINGLE("ALC Capture Noise Gate Switch", WM8976_NGATE,  3, 1, 0),	/*R35*/
	SOC_SINGLE("ALC Capture Noise Gate Threshold", WM8976_NGATE,  0, 7, 0),

	/* Input PGA*/
	SOC_SINGLE("Capture PGA ZC Switch", WM8976_INPPGA,  7, 1, 0),			/*R45*/
	SOC_SINGLE("Capture PGA Volume", WM8976_INPPGA,  0, 63, 0),
	
	/* Mixer #3: Boost (Input) mixer */
	SOC_SINGLE("PGA Boost(+20dB)", WM8976_ADCBOOST, 8, 1, 0),				/*R47*/
	SOC_SINGLE("L2 Boost Volume", WM8976_ADCBOOST, 4, 3, 0),
	SOC_SINGLE("Aux L2 Volume", WM8976_ADCBOOST, 0, 3, 0),

	/* OUT1 - Headphones */
	SOC_DOUBLE_R("Headphone Playback ZC Switch", WM8976_HPVOLL,  WM8976_HPVOLR, 7, 1, 0),	/*R52, R53*/
	SOC_DOUBLE_R("Headphone Playback Switch", WM8976_HPVOLL,  WM8976_HPVOLR, 6, 1, 1),
	SOC_DOUBLE_R("Headphone Playback Volume", WM8976_HPVOLL,  WM8976_HPVOLR, 0, 63, 0),
	
	/* OUT2 - Speakers */
	SOC_DOUBLE_R("Speaker Playback ZC Switch", WM8976_SPKVOLL,  WM8976_SPKVOLR, 7, 1, 0),	/*R54, R55*/
	SOC_DOUBLE_R("Speaker Playback Switch", WM8976_SPKVOLL,  WM8976_SPKVOLR, 6, 1, 1),
	SOC_DOUBLE_R("Speaker Playback Volume", WM8976_SPKVOLL,  WM8976_SPKVOLR, 0, 63, 0),
	
	/* OUT3 - Line Output */
	SOC_SINGLE("Line Playback Switch", WM8976_OUT3MIX, 6, 1, 1),
};

/* Left Output Mixer */
static const struct snd_kcontrol_new wm8976_left_mixer_controls[] = {
	SOC_DAPM_SINGLE("PCM Playback Switch", WM8976_MIXL, 0, 1, 0),
	SOC_DAPM_SINGLE("Line Bypass Switch", WM8976_MIXL, 1, 1, 0),
	SOC_DAPM_SINGLE("Aux Playback Switch", WM8976_MIXL, 5, 1, 0),
};

/* Right Output Mixer */
static const struct snd_kcontrol_new wm8976_right_mixer_controls[] = {
	SOC_DAPM_SINGLE("PCM Playback Switch", WM8976_MIXR, 0, 1, 0),
	SOC_DAPM_SINGLE("Line Bypass Switch", WM8976_MIXR, 1, 1, 0),
	SOC_DAPM_SINGLE("Aux Playback Switch", WM8976_MIXR, 5, 1, 0),
};

/* Mixer #2: Input PGA Mute */
static const struct snd_kcontrol_new wm8976_left_input_mixer[] = {
	SOC_DAPM_SINGLE("L2 Switch", WM8976_INPUT, 2, 1, 0),
	SOC_DAPM_SINGLE("MicN Switch", WM8976_INPUT, 1, 1, 0),
	SOC_DAPM_SINGLE("MicP Switch", WM8976_INPUT, 0, 1, 0),
};

/* TODO Widgets */
static const struct snd_soc_dapm_widget wm8976_dapm_widgets[] = {
	SND_SOC_DAPM_DAC("Left DAC", "Left HiFi Playback",
			 WM8976_POWER3, 0, 0),
	SND_SOC_DAPM_DAC("Right DAC", "Right HiFi Playback",
			 WM8976_POWER3, 1, 0),
	SND_SOC_DAPM_ADC("Left ADC", "Left HiFi Capture",
			 WM8976_POWER2, 0, 0),
			 
	SND_SOC_DAPM_MIXER("Left Output Mixer", WM8976_POWER3, 2, 0,
			wm8976_left_mixer_controls,
			ARRAY_SIZE(wm8976_left_mixer_controls)),	
	SND_SOC_DAPM_MIXER("Right Output Mixer", WM8976_POWER3, 3, 0,
			wm8976_right_mixer_controls,
			ARRAY_SIZE(wm8976_right_mixer_controls)),
	SND_SOC_DAPM_MIXER("Left Input Mixer", WM8976_POWER2, 2, 0,
			wm8976_left_input_mixer,
			ARRAY_SIZE(wm8976_left_input_mixer)),

	SND_SOC_DAPM_PGA("Left Boost Mixer", WM8976_POWER2,
			 4, 0, NULL, 0),
	SND_SOC_DAPM_PGA("Right Boost Mixer", WM8976_POWER2,
			 5, 0, NULL, 0),

	SND_SOC_DAPM_PGA("Input PGA mute", WM8976_INPPGA,
			 6, 1, NULL, 0),

	SND_SOC_DAPM_PGA("Left Headphone Out", WM8976_POWER2,
			 7, 0, NULL, 0),
	SND_SOC_DAPM_PGA("Right Headphone Out", WM8976_POWER2,
			 8, 0, NULL, 0),

	SND_SOC_DAPM_PGA("Left Speaker Out", WM8976_POWER3,
			 6, 0, NULL, 0),
	SND_SOC_DAPM_PGA("Right Speaker Out", WM8976_POWER3,
			 5, 0, NULL, 0),

	SND_SOC_DAPM_MIXER("OUT4 VMID", WM8976_POWER3,
			   8, 0, NULL, 0),

	SND_SOC_DAPM_MICBIAS("Mic Bias", WM8976_POWER1, 4, 0),

	SND_SOC_DAPM_INPUT("LMICN"),
	SND_SOC_DAPM_INPUT("LMICP"),
	SND_SOC_DAPM_INPUT("LAUX"),
	SND_SOC_DAPM_INPUT("RAUX"),
	SND_SOC_DAPM_INPUT("L2"),
	SND_SOC_DAPM_OUTPUT("LHP"),
	SND_SOC_DAPM_OUTPUT("RHP"),
	SND_SOC_DAPM_OUTPUT("LSPK"),
	SND_SOC_DAPM_OUTPUT("RSPK"),
};

static const struct snd_soc_dapm_route audio_map[] = {
	/* Output mixer */
	{"Right Output Mixer", "PCM Playback Switch", "Right DAC"},
	{"Right Output Mixer", "Aux Playback Switch", "RAUX"},
	{"Right Output Mixer", "Line Bypass Switch", "Right Boost Mixer"},

	{"Left Output Mixer", "PCM Playback Switch", "Left DAC"},
	{"Left Output Mixer", "Aux Playback Switch", "LAUX"},
	{"Left Output Mixer", "Line Bypass Switch", "Left Boost Mixer"},

	/* Outputs */
	{"Right Headphone Out", NULL, "Right Output Mixer"},
	{"RHP", NULL, "Right Headphone Out"},

	{"Left Headphone Out", NULL, "Left Output Mixer"},
	{"LHP", NULL, "Left Headphone Out"},

	{"Right Speaker Out", NULL, "Right Output Mixer"},
	{"RSPK", NULL, "Right Speaker Out"},

	{"Left Speaker Out", NULL, "Left Output Mixer"},
	{"LSPK", NULL, "Left Speaker Out"},

	/* Boost Mixer */

	{"Left ADC", NULL, "Left Boost Mixer"},

	{"Left Boost Mixer", NULL, "LAUX"},
	{"Left Boost Mixer", NULL, "Input PGA mute"},
	{"Left Boost Mixer", NULL, "L2"},

	/* Input PGA */
	{"Input PGA mute", NULL, "Left Input Mixer"},

	{"Left Input Mixer", "L2 Switch", "L2"},
	{"Left Input Mixer", "MicN Switch", "LMICN"},
	{"Left Input Mixer", "MicP Switch", "LMICP"},
};

static int wm8976_add_widgets(struct snd_soc_codec *codec)
{
	snd_soc_dapm_new_controls(codec, wm8976_dapm_widgets,
				  ARRAY_SIZE(wm8976_dapm_widgets));

	snd_soc_dapm_add_routes(codec, audio_map, ARRAY_SIZE(audio_map));

	snd_soc_dapm_new_widgets(codec);
	return 0;
}

/* PLL divisors */
struct wm8976_pll_div {
	u32 k;
	u8 n;
	u8 div2;
};


static void pll_factors(struct snd_soc_codec *codec,
	struct wm8976_pll_div *pll_div, unsigned int target, unsigned int source)
{
	u64 k_part;
	unsigned int k, n_div, n_mod;

	n_div = target / source;
	if (n_div < 6) {
		source >>= 1;
		pll_div->div2 = 1;
		n_div = target / source;
	}
	else {
		pll_div->div2 = 0;
	}

	if (n_div < 6 || n_div > 12)
		printk("WM8976 N value exceeds recommended range! N = %u\n", n_div);

	pll_div->n = n_div;
	n_mod = target - source * n_div;
	k_part = FIXED_PLL_SIZE * (long long)n_mod + source / 2;

	do_div(k_part, source);

	k = k_part & 0xFFFFFFFF;

	pll_div->k = k;
}

/* MCLK dividers */
static const int mclk_numerator[]	= {1, 3, 2, 3, 4, 6, 8, 12};
static const int mclk_denominator[]	= {1, 2, 1, 1, 1, 1, 1, 1};

/*
 * find index >= idx, such that, for a given f_out,
 * 3 * f_mclk / 4 <= f_PLLOUT < 13 * f_mclk / 4
 * f_out can be f_256fs or f_opclk, currently only used for f_256fs. Can be
 * generalised for f_opclk with suitable coefficient arrays, but currently
 * the OPCLK divisor is calculated directly, not iteratively.
 */
static int wm8976_enum_mclk(unsigned int f_out, unsigned int f_mclk,
	unsigned int *f_pllout)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(mclk_numerator); i++) {
		unsigned int f_pllout_x4 = 4 * f_out * mclk_numerator[i] / mclk_denominator[i];
		if (3 * f_mclk <= f_pllout_x4 && f_pllout_x4 < 13 * f_mclk) {
			*f_pllout = f_pllout_x4 / 4;
			return i;
		}
	}

	return -EINVAL;
}

/*
 * Calculate internal frequencies and dividers, according to Figure 40
 * "PLL and Clock Select Circuit" in WM8976 datasheet Rev. 2.6
 */
static int wm8976_configure_pll(struct snd_soc_codec *codec)
{
	struct wm8976_priv *wm8976 = codec->private_data;
	struct wm8976_pll_div pll_div;
	unsigned int f_opclk = wm8976->f_opclk, f_mclk = wm8976->f_mclk,
		f_256fs = wm8976->f_256fs;
	unsigned int f2;

	if (!f_mclk)
		return -EINVAL;

	if (f_opclk) {
		unsigned int opclk_div;
		/* Cannot set up MCLK divider now, do later */
		wm8976->mclk_idx = -1;

		/*
		 * The user needs OPCLK. Choose OPCLKDIV to put
		 * 6 <= R = f2 / f1 < 13, 1 <= OPCLKDIV <= 4.
		 * f_opclk = f_mclk * prescale * R / 4 / OPCLKDIV, where
		 * prescale = 1, or prescale = 2. Prescale is calculated inside
		 * pll_factors(). We have to select f_PLLOUT, such that
		 * f_mclk * 3 / 4 <= f_PLLOUT < f_mclk * 13 / 4. Must be
		 * f_mclk * 3 / 16 <= f_opclk < f_mclk * 13 / 4.
		 */
		if (16 * f_opclk < 3 * f_mclk || 4 * f_opclk >= 13 * f_mclk)
			return -EINVAL;

		if (4 * f_opclk < 3 * f_mclk)
			/* Have to use OPCLKDIV */
			opclk_div = (3 * f_mclk / 4 + f_opclk - 1) / f_opclk;
		else
			opclk_div = 1;

		dev_dbg(codec->dev, "%s: OPCLKDIV=%d\n", __func__, opclk_div);

		snd_soc_update_bits(codec, WM8976_GPIO, 0x30, (opclk_div - 1) << 4);

		wm8976->f_pllout = f_opclk * opclk_div;
	}
	else if (f_256fs) {
		/*
		 * Not using OPCLK, but PLL is used for the codec, choose R:
		 * 6 <= R = f2 / f1 < 13, to put 1 <= MCLKDIV <= 12.
		 * f_256fs = f_mclk * prescale * R / 4 / MCLKDIV, where
		 * prescale = 1, or prescale = 2. Prescale is calculated inside
		 * pll_factors(). We have to select f_PLLOUT, such that
		 * f_mclk * 3 / 4 <= f_PLLOUT < f_mclk * 13 / 4. Must be
		 * f_mclk * 3 / 48 <= f_256fs < f_mclk * 13 / 4. This means MCLK
		 * must be 3.781MHz <= f_MCLK <= 32.768MHz
		 */
		int idx = wm8976_enum_mclk(f_256fs, f_mclk, &wm8976->f_pllout);
		if (idx < 0)
			return idx;

		wm8976->mclk_idx = idx;

		/* GPIO1 into default mode as input - before configuring PLL */
		snd_soc_update_bits(codec, WM8976_GPIO, 7, 0);
	}
	else {
		return -EINVAL;
	}

	f2 = wm8976->f_pllout * 4;

	dev_dbg(codec->dev, "%s: f_MCLK=%uHz, f_PLLOUT=%uHz\n", __func__,
		wm8976->f_mclk, wm8976->f_pllout);

	pll_factors(codec, &pll_div, f2, wm8976->f_mclk);

	dev_dbg(codec->dev, "%s: calculated PLL N=0x%x, K=0x%x, div2=%d\n",
		__func__, pll_div.n, pll_div.k, pll_div.div2);

	/* Turn PLL off for configuration... */
	snd_soc_update_bits(codec, WM8976_POWER1, 0x20, 0);

	snd_soc_write(codec, WM8976_PLLN, (pll_div.div2 << 4) | pll_div.n);
	snd_soc_write(codec, WM8976_PLLK1, (pll_div.k >> 18));
	snd_soc_write(codec, WM8976_PLLK2, (pll_div.k >> 9) & 0x1ff);
	snd_soc_write(codec, WM8976_PLLK3, (pll_div.k & 0x1ff));

	/* ...and on again */
	snd_soc_update_bits(codec, WM8976_POWER1, 0x20, 0x20);

	if (f_opclk)
		/* Output PLL (OPCLK) to GPIO1 */
		snd_soc_update_bits(codec, WM8976_GPIO, 7, 4);

	return 0;
}

/*
 * Configure WM8976 clock dividers.
 */
static int wm8976_set_dai_clkdiv(struct snd_soc_dai *codec_dai,
				 int div_id, int div)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	struct wm8976_priv *wm8976 = codec->private_data;
	int ret = 0;

	switch (div_id) {
	case WM8976_OPCLK_RATE:
		wm8976->f_opclk = div;

		if (wm8976->f_mclk)
			/*
			 * We know the MCLK frequency, the user has requested
			 * OPCLK, configure the PLL based on that and start it
			 * and OPCLK immediately. We will configure PLL to match
			 * user-requested OPCLK frquency as good as possible.
			 * In fact, it is likely, that matching the sampling
			 * rate, when it becomes known, is more important, and
			 * we will not be reconfiguring PLL then, because we
			 * must not interrupt OPCLK. But it should be fine,
			 * because typically the user will request OPCLK to run
			 * at 256fs or 512fs, and for these cases we will also
			 * find an exact MCLK divider configuration - it will
			 * be equal to or double the OPCLK divisor.
			 */
			ret = wm8976_configure_pll(codec);
		break;
	case WM8976_BCLKDIV:
		if (div & ~0x1c)
			return -EINVAL;
		snd_soc_update_bits(codec, WM8976_CLOCK, 0x1c, div);
		break;
	default:
		return -EINVAL;
	}

	dev_dbg(codec->dev, "%s: ID %d, value %u\n", __func__, div_id, div);

	return ret;
}

/*
 * @freq:	when .set_pll() us not used, freq is codec MCLK input frequency
 */
static int wm8976_set_dai_sysclk(struct snd_soc_dai *codec_dai, int clk_id,
	unsigned int freq, int dir)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	struct wm8976_priv *wm8976 = codec->private_data;
	int ret = 0;

	dev_dbg(codec->dev, "%s: ID %d, freq %u\n", __func__, clk_id, freq);

	if (freq) {
		wm8976->f_mclk = freq;

		/* Even if MCLK is used for system clock, might have to drive OPCLK */
		if (wm8976->f_opclk)
			ret = wm8976_configure_pll(codec);

		/* Our sysclk is fixed to 256 * fs, will configure in .hw_params()  */

		if (!ret)
			wm8976->sysclk = clk_id;
	}

	if (wm8976->sysclk == WM8976_PLL && (!freq || clk_id == WM8976_MCLK)) {
		/* Clock CODEC directly from MCLK */
		snd_soc_update_bits(codec, WM8976_CLOCK, 0x100, 0);

		/* GPIO1 into default mode as input - before configuring PLL */
		snd_soc_update_bits(codec, WM8976_GPIO, 7, 0);

		/* Turn off PLL */
		snd_soc_update_bits(codec, WM8976_POWER1, 0x20, 0);
		wm8976->sysclk = WM8976_MCLK;
		wm8976->f_pllout = 0;
		wm8976->f_opclk = 0;
	}

	return ret;
}

/*
 * Set ADC and Voice DAC format.
 */
static int wm8976_set_dai_fmt(struct snd_soc_dai *codec_dai, unsigned int fmt)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	/*
	 * BCLK polarity mask = 0x100, LRC clock polarity mask = 0x80,
	 * Data Format mask = 0x18: all will be calculated anew
	 */
	u16 iface = snd_soc_read(codec, WM8976_IFACE) & ~0x198;
	u16 clk = snd_soc_read(codec, WM8976_CLOCK);

	dev_dbg(codec->dev, "%s\n", __func__);

	/* set master/slave audio interface */
	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM:
		clk |= 1;
		break;
	case SND_SOC_DAIFMT_CBS_CFS:
		clk &= ~1;
		break;
	default:
		return -EINVAL;
	}

	/* interface format */
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
		iface |= 0x10;
		break;
	case SND_SOC_DAIFMT_RIGHT_J:
		break;
	case SND_SOC_DAIFMT_LEFT_J:
		iface |= 0x8;
		break;
	case SND_SOC_DAIFMT_DSP_A:
		iface |= 0x18;
		break;
	default:
		return -EINVAL;
	}

	/* clock inversion */
	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF:
		break;
	case SND_SOC_DAIFMT_IB_IF:
		iface |= 0x180;
		break;
	case SND_SOC_DAIFMT_IB_NF:
		iface |= 0x100;
		break;
	case SND_SOC_DAIFMT_NB_IF:
		iface |= 0x80;
		break;
	default:
		return -EINVAL;
	}
	
	snd_soc_write(codec, WM8976_IFACE, iface);
	snd_soc_write(codec, WM8976_CLOCK, clk);

	return 0;
}

static int wm8976_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params,
	struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_device *socdev = rtd->socdev;
	struct snd_soc_codec *codec = socdev->card->codec;
	struct wm8976_priv *wm8976 = codec->private_data;
	/* Word length mask = 0x60 */
	u16 iface_ctl = snd_soc_read(codec, WM8976_IFACE) & ~0x60;
	/* Sampling rate mask = 0xe (for filters) */
	u16 add_ctl = snd_soc_read(codec, WM8976_ADD) & ~0xe;
	unsigned int f_sel, diff, diff_best = INT_MAX;
	int i, best = 0;
	alsa_dbg("%s\n", __func__);
	
	if (!wm8976->f_mclk)
		return -EINVAL;
		
	/* bit size */
	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S16_LE:
		break;
	case SNDRV_PCM_FORMAT_S20_3LE:
		iface_ctl |= 0x20;
		break;
	case SNDRV_PCM_FORMAT_S24_3LE:
		iface_ctl |= 0x40;
		break;
	case SNDRV_PCM_FORMAT_S32_LE:
		iface_ctl |= 0x60;
		break;
	}

	/* filter coefficient */
	switch (params_rate(params)) {
	case 8000:
		add_ctl |= 0x5 << 1;
		break;
	case 11025:
		add_ctl |= 0x4 << 1;
		break;
	case 16000:
		add_ctl |= 0x3 << 1;
		break;
	case 22050:
		add_ctl |= 0x2 << 1;
		break;
	case 32000:
		add_ctl |= 0x1 << 1;
		break;
	case 44100:
	case 48000:
		break;
	}

	/* Sampling rate is known now, can configure the MCLK divider */
	wm8976->f_256fs = params_rate(params) * 256;

	if (wm8976->sysclk == WM8976_MCLK) {
		wm8976->mclk_idx = -1;
		f_sel = wm8976->f_mclk;
	}
	else {
		int ret = wm8976_configure_pll(codec);
		if (ret < 0)
			return ret;

		f_sel = wm8976->f_pllout;
	}

	if (wm8976->mclk_idx < 0) {
		/* Either MCLK is used directly, or OPCLK is used */
		if (f_sel < wm8976->f_256fs || f_sel > 12 * wm8976->f_256fs)
			return -EINVAL;

		for (i = 0; i < ARRAY_SIZE(mclk_numerator); i++) {
			diff = abs(wm8976->f_256fs * 3 - f_sel * 3 * mclk_denominator[i] / mclk_numerator[i]);

			if (diff < diff_best) {
				diff_best = diff;
				best = i;
			}

			if (!diff)
				break;
		}
	}
	else {
		/* OPCLK not used, codec driven by PLL */
		best = wm8976->mclk_idx;
		diff = 0;
	}

	if (diff){
		dev_warn(codec->dev, "Imprecise sampling rate: %uHz%s\n",
			f_sel * mclk_denominator[best] / mclk_numerator[best] / 256,
			wm8976->sysclk == WM8976_MCLK ?
			", consider using PLL" : "");
	}

	dev_dbg(codec->dev, "%s: fmt %d, rate %u, MCLK divisor #%d\n", __func__,
		params_format(params), params_rate(params), best);

	/* MCLK divisor mask = 0xe0 */
	snd_soc_update_bits(codec, WM8976_CLOCK, 0xe0, best << 5);

	snd_soc_write(codec, WM8976_IFACE, iface_ctl);
	snd_soc_write(codec, WM8976_ADD, add_ctl);

	if (wm8976->sysclk == WM8976_PLL){
		/* Run CODEC from PLL instead of MCLK */
		snd_soc_update_bits(codec, WM8976_CLOCK, 0x100, 0x100);
		snd_soc_update_bits(codec, WM8976_POWER1, 0x020, 0x020);
	}
	else{
		/* Clock CODEC directly from MCLK */
		snd_soc_update_bits(codec, WM8976_CLOCK, 0x100, 0);
		snd_soc_update_bits(codec, WM8976_POWER1, 0x020, 0);
	}
	
#if CTRL_MODE == SLAVE_MODE
	snd_soc_update_bits(codec, WM8976_CLOCK, 0x01, 1);	/* now, wm8976 works in master mode */
#else
	snd_soc_update_bits(codec, WM8976_CLOCK, 0x01, 0);	/* now, wm8976 works in slave mode */
#endif
	return 0;
}

static void wm8976_shutdown(struct snd_pcm_substream *substream,
	struct snd_soc_dai *dai)
{
	alsa_dbg("%s\n", __func__);
}

static int wm8976_mute(struct snd_soc_dai *dai, int mute)
{
	struct snd_soc_codec *codec = dai->codec;
	u16 mute_reg = wm8976_read_reg_cache(codec, WM8976_DAC) & 0xffbf;
	
	alsa_dbg("%s\n", __func__);
	
	if(mute)
		wm8976_write(codec, WM8976_DAC, mute_reg | 0x40);
	else
		wm8976_write(codec, WM8976_DAC, mute_reg);

	return 0;
}


static int wm8976_set_audio_output(struct snd_soc_codec *codec)
{
	alsa_dbg("%s\n", __func__);
	
	snd_soc_update_bits(codec, WM8976_POWER1, 0x00F, 0x00D);/*R1 0X00D*/
    snd_soc_update_bits(codec, WM8976_POWER2, 0x1C0, 0x180);/*R2 0X191*/
	snd_soc_update_bits(codec, WM8976_POWER3, 0x1FF, 0x06F);/*R3 0X000*/

	return 0;
}

static int wm8976_set_audio_input(struct snd_soc_codec *codec)
{
	alsa_dbg("%s\n", __func__);
	
	snd_soc_update_bits(codec, WM8976_POWER1, 0x01F, 0x01D);/*R1 0X01D*/
    snd_soc_update_bits(codec, WM8976_POWER2, 0x01F, 0x015);/*R2 0X191*/

	return 0;
}

static void sep0611_wm8976_init(struct snd_soc_codec *codec)
{
	wm8976_write(codec, WM8976_COMP, 0x000);   	/*R5 0X000*/
	wm8976_write(codec, WM8976_DACVOLL, 0x1FF);	/*R11 0X1FF*/
	wm8976_write(codec, WM8976_DACVOLR, 0x1FF);	/*R12 0X1FF*/
	wm8976_write(codec, WM8976_ADCVOL, 0x1FF);	/*R15 0X1FF*/
	wm8976_write(codec, WM8976_OUTPUT, 0x002); 	/*R49 0X002*/
	wm8976_write(codec, WM8976_MIXL, 0x001); 	/*R50 0X001*/
	wm8976_write(codec, WM8976_MIXR, 0x001); 	/*R51 0X001*/
	wm8976_write(codec, WM8976_HPVOLL, 0x130);	/*R52 0X8F*/
	wm8976_write(codec, WM8976_HPVOLR, 0x130);	/*R53 0X8F*/
	wm8976_write(codec, WM8976_SPKVOLL, 0x130);	/*R54 0X8F*/
	wm8976_write(codec, WM8976_SPKVOLR, 0x130);	/*R55 0X8F*/
	wm8976_write(codec, WM8976_INPUT, 0x003); 	/*R44 0X003*/
	wm8976_write(codec, WM8976_INPPGA, 0x13F);	/*R45 0X13F*/
	wm8976_write(codec, WM8976_BEEP, 0x01F);	/*R43 0X01F, this is necessary for path of speaker */
}

static int wm8976_pcm_prepare(struct snd_pcm_substream *substream,
			      struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_device *socdev = rtd->socdev;
	struct snd_soc_codec *codec = socdev->card->codec;
#ifdef _DEBUG_CODEC_REGS_VIEW
	int i;
	u16 *cache = codec->reg_cache;
#endif
	alsa_dbg("%s\n", __func__);
	
	if(substream->stream == SNDRV_PCM_STREAM_PLAYBACK){
		wm8976_set_audio_output(codec);
	}
	else{
		wm8976_set_audio_input(codec);
	}
	
#ifdef _DEBUG_CODEC_REGS_VIEW
	printk("codec wm8976's all registers:\n");
	for(i=1; i<WM8976_CACHEREGNUM; i++)
	{
		printk("0x%03x, ", 0x1FF & cache[i]);
		if(i%4 == 0)
			printk("\n");
	}
	printk("\n");
#endif

	return 0;
}

/* TODO: liam need to make this lower power with dapm */
static int wm8976_set_bias_level(struct snd_soc_codec *codec,
	enum snd_soc_bias_level level)
{
	u16 power1 = snd_soc_read(codec, WM8976_POWER1) & ~3;
	
	switch (level) {
	case SND_SOC_BIAS_ON:
	case SND_SOC_BIAS_PREPARE:
		power1 |= 1;  /* VMID 75k */
		snd_soc_write(codec, WM8976_POWER1, power1);
		break;
	case SND_SOC_BIAS_STANDBY:
		/* bit 3: enable bias, bit 2: enable I/O tie off buffer */
		power1 |= 0xc;

		if (codec->bias_level == SND_SOC_BIAS_OFF) {
			/* Initial cap charge at VMID 5k */
			snd_soc_write(codec, WM8976_POWER1, power1 | 0x3);
			mdelay(100);
		}

		power1 |= 0x2;  /* VMID 500k */
		snd_soc_write(codec, WM8976_POWER1, power1);
		break;
	case SND_SOC_BIAS_OFF:
		/* Preserve PLL - OPCLK may be used by someone */
		snd_soc_update_bits(codec, WM8976_POWER1, ~0x20, 0);
		snd_soc_write(codec, WM8976_POWER2, 0);
		snd_soc_write(codec, WM8976_POWER3, 0);
		break;
	}
	
	dev_dbg(codec->dev, "%s: %d, %x\n", __func__, level, power1);

	codec->bias_level = level;
	
	alsa_dbg("%s\n", __func__);

	return 0;
}
	
#define WM8976_RATES \
	(SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 | SNDRV_PCM_RATE_16000 | \
	SNDRV_PCM_RATE_22050 | SNDRV_PCM_RATE_32000 | SNDRV_PCM_RATE_44100 | \
	SNDRV_PCM_RATE_48000)

#define WM8976_OUT_FORMATS \
	(SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE | \
	SNDRV_PCM_FMTBIT_S24_3LE | SNDRV_PCM_FMTBIT_S32_LE)

#define WM8976_IN_FORMATS \
	(SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE | \
	SNDRV_PCM_FMTBIT_S24_3LE | SNDRV_PCM_FMTBIT_S32_LE)

struct snd_soc_dai_ops wm8976_dai_ops = {
	.prepare 		= wm8976_pcm_prepare,
	.hw_params 		= wm8976_hw_params,
	.shutdown 		= wm8976_shutdown,
	.digital_mute 	= wm8976_mute,
	.set_fmt 		= wm8976_set_dai_fmt,
	.set_clkdiv 	= wm8976_set_dai_clkdiv,
	.set_sysclk		= wm8976_set_dai_sysclk,
};

struct snd_soc_dai wm8976_dai = {
	.name 		= "WM8976 HiFi",
	.playback 	= {
		.stream_name 	= "Playback",
		.channels_min 	= 1,
		.channels_max 	= 2,
		.rates 			= WM8976_RATES,
		.formats 		= WM8976_OUT_FORMATS,
	},
	.capture 	= {
		.stream_name 	= "Capture",
		.channels_min 	= 1,
		.channels_max 	= 2,
		.rates 			= WM8976_RATES,
		.formats 		= WM8976_IN_FORMATS,
	},
	.ops 		= &wm8976_dai_ops,
};
EXPORT_SYMBOL_GPL(wm8976_dai);

static int wm8976_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct snd_soc_device *socdev = platform_get_drvdata(pdev);
	struct snd_soc_codec *codec = socdev->card->codec;

	wm8976_set_bias_level(codec, SND_SOC_BIAS_OFF);
	
	return 0;
}

static int wm8976_resume(struct platform_device *pdev)
{
	struct snd_soc_device *socdev = platform_get_drvdata(pdev);
	struct snd_soc_codec *codec = socdev->card->codec;
	int i;
	u8 data[2];
	u16 *cache = codec->reg_cache;
	
	/* Sync reg_cache with the hardware */
	for (i = 0; i < ARRAY_SIZE(wm8976_reg); i++) {
		data[0] = (i << 1) | ((cache[i] >> 8) & 0x0001);
		data[1] = cache[i] & 0x00ff;
		codec->hw_write(codec->control_data, data, 2);
	}

	wm8976_set_bias_level(codec, SND_SOC_BIAS_STANDBY);
	wm8976_set_bias_level(codec, codec->suspend_bias_level);

	return 0;
}

/* add non dapm controls */
static int wm8976_add_controls(struct snd_soc_codec *codec)
{
	int err, i;

	for (i = 0; i < ARRAY_SIZE(wm8976_snd_controls); i++) {
		err = snd_ctl_add(codec->card,
				snd_soc_cnew(&wm8976_snd_controls[i],
					codec, NULL));
		if (err < 0)
			return err;
	}

	return 0;
}

static struct i2c_driver wm8976_i2c_driver;
static int wm8976_probe(struct platform_device *pdev)
{
	struct snd_soc_device *socdev = platform_get_drvdata(pdev);
	struct snd_soc_codec *codec;
	struct wm8976_priv *wm8976;
	int ret = 0;

	codec = kzalloc(sizeof(struct snd_soc_codec), GFP_KERNEL);
	if (codec == NULL)
		return -ENOMEM;

	wm8976 = kzalloc(sizeof(struct wm8976_priv), GFP_KERNEL);
	if (wm8976 == NULL) {
		kfree(codec);
		return -ENOMEM;
	}

	codec->private_data = wm8976;
	socdev->card->codec = codec;
	mutex_init(&codec->mutex);
	INIT_LIST_HEAD(&codec->dapm_widgets);
	INIT_LIST_HEAD(&codec->dapm_paths);

	wm8976_socdev = socdev;
	ret = -ENODEV;

	codec->hw_write = (hw_write_t)wm8976_i2c_write;
	ret = i2c_add_driver(&wm8976_i2c_driver);
	if (ret != 0) {
		dev_err(&pdev->dev, "can't add i2c driver\n");
		return ret;
	}

	wm8976->f_mclk = WM8976_MCLK_FREQ;
	wm8976->sysclk = WM8976_PLL;

	if (ret != 0) {
		kfree(codec->private_data);
		kfree(codec);
	}
	
	return 0;
}

/* power down chip */
static int wm8976_remove(struct platform_device *pdev)
{
	struct snd_soc_device *socdev = platform_get_drvdata(pdev);

	snd_soc_free_pcms(socdev);
	snd_soc_dapm_free(socdev);

	return 0;
}

struct snd_soc_codec_device soc_codec_dev_wm8976 = {
	.probe 		= wm8976_probe,
	.remove 	= wm8976_remove,
	.suspend	= wm8976_suspend,
	.resume 	= wm8976_resume,
};
EXPORT_SYMBOL_GPL(soc_codec_dev_wm8976);

static int wm8976_init(struct snd_soc_device *socdev)
{
	struct snd_soc_codec *codec = socdev->card->codec;	
	int ret = 0;
	
	codec->name  = "WM8976";
	codec->owner = THIS_MODULE;
	codec->read  = wm8976_read_reg_cache;
	codec->write = wm8976_write;
	codec->set_bias_level = wm8976_set_bias_level;
	codec->dai   = &wm8976_dai;
	codec->num_dai = 1;
	codec->reg_cache_size = ARRAY_SIZE(wm8976_reg);
	codec->reg_cache = kmemdup(wm8976_reg, sizeof(wm8976_reg), GFP_KERNEL);
	if (codec->reg_cache == NULL)
		return -ENOMEM;

	/* register pcms */
	ret = snd_soc_new_pcms(socdev, SNDRV_DEFAULT_IDX1, SNDRV_DEFAULT_STR1);
	if (ret < 0) {
		printk(KERN_ERR "WM8976: failed to create pcms\n");
		goto pcm_err;
	}

	wm8976_reset(codec);

	wm8976_add_controls(codec);
	wm8976_add_widgets(codec);
	ret = snd_soc_init_card(socdev);
	if (ret < 0) {
		printk(KERN_ERR "WM8976: failed to register card\n");
		goto card_err;
	}
	
	sep0611_wm8976_init(codec);

	return ret;

card_err:
	snd_soc_free_pcms(socdev);
	snd_soc_dapm_free(socdev);
pcm_err:
	kfree(codec->reg_cache);
	return ret;
}

static int wm8976_i2c_probe(struct i2c_client *i2c,
			    const struct i2c_device_id *id)
{
	struct snd_soc_device *socdev = wm8976_socdev;
	struct snd_soc_codec *codec = socdev->card->codec;
	int ret;
	
	i2c_set_clientdata(i2c, codec);	
	codec->control_data = i2c;

	ret = wm8976_init(socdev);
	if (ret < 0)
		pr_err("failed to initialise WM8976\n");

	return ret;
}

static int wm8976_i2c_remove(struct i2c_client *client)
{
	struct snd_soc_codec *codec = i2c_get_clientdata(client);
	kfree(codec->reg_cache);
	return 0;
}

static int wm8976_i2c_write(struct i2c_client *client, const char *buf, int count)
{
	struct snd_soc_codec *codec = i2c_get_clientdata(client);
	unsigned char reg;
	unsigned int value;
	
	reg = (buf[0] >> 1) & 0x7F;
	value = ((buf[0] & 0x01)<<8) | buf[1];

	wm8976_write_reg_cache(codec, reg, value);
	
	return i2c_master_send(client, buf, count);
}

static const struct i2c_device_id wm8976_i2c_id[] = {
	{"WM8976", 0},
};
MODULE_DEVICE_TABLE(i2c, wm8976_i2c_id);

static struct i2c_driver wm8976_i2c_driver = {
	.driver = {
		.name 	= "WM8976(I2C)",
		.owner 	= THIS_MODULE,
	},
	.probe 		= wm8976_i2c_probe,
	.remove 	= wm8976_i2c_remove,
	.id_table 	= wm8976_i2c_id,
};

static int __init wm8976_modinit(void)
{
	return  snd_soc_register_dai(&wm8976_dai);
}
module_init(wm8976_modinit);

static void __exit wm8976_exit(void)
{
	snd_soc_unregister_dai(&wm8976_dai);
}
module_exit(wm8976_exit);

MODULE_AUTHOR("chenguangming@wxseuic.com");
MODULE_DESCRIPTION("SEP ASoC WM8976 driver");
MODULE_LICENSE("GPL");
