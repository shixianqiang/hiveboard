/******************************************************************
File name   :  CODEC.h
Description :  The file is wrote for defining the wm8976' registers,functions and so on.
Date        :  2010-5-19
Author      :  Yanghq
*******************************************************************/
#ifndef CODEC_H_
#define CODEC_H_

#include <asm/arch/vic.h>

/*CODEC registers lists*/

#define CODEC_SRST        0x00  /*software reset*/
#define CODEC_POW1        0x01  /*power management 1,2,3*/
#define CODEC_POW2        0x02
#define CODEC_POW3        0x03
#define CODEC_AUDIO_SET   0x04  /*set the audio interface characteristic*/
#define CODEC_AUDIO_CTL   0x05  /*control the companding of audio date*/
#define CODEC_CGC         0X06  /*clock generation control*/
#define CODEC_ADD_CTL     0x07  /*additional control*/
#define CODEC_GPIO_SET    0x08  /*set the GPIO of the PLL output*/
#define CODEC_DECT_SET    0x09  /*set the jack insert detection */
#define CODEC_DAC_CTL     0x0a  /*set the DAC characteristic*/
#define CODEC_DACL_SET    0x0b  /*left  DAC digital volume control*/
#define CODEC_DACR_SET    0x0c  /*right DAC digital volume control*/
#define CODEC_ADCL_SET    0x0f  /*left ADC volume control*/
#define CODEC_EQ3D_CTL    0x12
#define CODEC_MIC_CTL     0x2c  /*control the mic input*/
#define CODEC_PGA_SET     0x2d  /*set the PGA gain*/
#define CODEC_BOOST_CTL   0x2f  /*control boost characteristic*/
#define CODEC_OUT_CTL     0x31  /*control the outrut of CODEC*/
#define CODEC_MIXL_CTL    0x32  /*left  mixer control*/
#define CODEC_MIXR_CTL    0x33  /*right mixer control*/
#define CODEC_LOUT1VOL    0x34  /*control the LOUT1 volume*/
#define CODEC_ROUT1VOL    0x35  /*control the ROUT1 volume*/

#define setlength(x)   (x==16?0x010:(x==24?0x050:0x070))

/*functions list*/
extern void I2S_Wm8976Playback(U32 pClockconfig);
extern void I2S_Wm8976record(U32 pRecordlength , U32 pClockconfig);
extern void I2S_Wm8976RecordPlaybak(U32 pRecordlength , U32 pClockconfig);
#endif /*CODEC_H_*/
