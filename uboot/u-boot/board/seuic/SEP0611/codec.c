/******************************************************************
File name   :  "CODEC.c"
Description :  The code is wrote to configure the WM8976.
Date        :  2010-5-19
Author      :  Yanghq
*******************************************************************/

#include"../include/codec.h"
#include"../include/iic.h"

extern AUDIO gWaveAudio;

void I2S_Wm8976Playback(U32 pClockconfig)
{	
	 write_reg(GPIO_PORTA_SEL ,0x01);	 
	 write_reg(GPIO_PORTA_DATA,0x01);	 
	 write_reg(GPIO_PORTA_DIR ,0x01);	
	 I2C_InitCodec();	 
	 I2C_WriteCodecReg(CODEC_SRST,0x000);	 
	 I2C_WriteCodecReg(CODEC_POW1,0x00D);  
	 I2C_WriteCodecReg(CODEC_POW2,0x180);	 
	 I2C_WriteCodecReg(CODEC_POW3,0x00F);	 
     I2C_WriteCodecReg(CODEC_AUDIO_SET, setlength(gWaveAudio.datalength>>16)); 	 
	 I2C_WriteCodecReg(CODEC_AUDIO_CTL,0x000);	 
	 I2C_WriteCodecReg(CODEC_CGC,pClockconfig);	 			 
	 I2C_WriteCodecReg(CODEC_ADD_CTL,0x000);	 	 
     I2C_WriteCodecReg(CODEC_DACL_SET,0x1ff);	
	 I2C_WriteCodecReg(CODEC_DACR_SET,0x1ff);	 	 
	 I2C_WriteCodecReg(CODEC_OUT_CTL, 0x002);	 
	 I2C_WriteCodecReg(CODEC_MIXL_CTL,0x001);	 
	 I2C_WriteCodecReg(CODEC_MIXR_CTL,0x001);	 
	 I2C_WriteCodecReg(CODEC_LOUT1VOL,0x13f);	 
	 I2C_WriteCodecReg(CODEC_ROUT1VOL,0x13f);	 
}

void I2S_Wm8976record(U32 pRecordlength , U32 pClockconfig)
{
	 write_reg(GPIO_PORTA_SEL ,0x01);	 
	 write_reg(GPIO_PORTA_DATA,0x01);	 
	 write_reg(GPIO_PORTA_DIR ,0x01);	 	 
	 I2C_InitCodec();	
	 I2C_WriteCodecReg(CODEC_SRST,0x000);	
	 I2C_WriteCodecReg(CODEC_POW1,0x01D);  	 
	 I2C_WriteCodecReg(CODEC_POW2,0x015);	 
	 I2C_WriteCodecReg(CODEC_POW3,0x000);	
	 I2C_WriteCodecReg(CODEC_AUDIO_SET , pRecordlength);	 
	 I2C_WriteCodecReg(CODEC_AUDIO_CTL,0x001);	 
	 I2C_WriteCodecReg(CODEC_CGC , pClockconfig);	
	 I2C_WriteCodecReg(CODEC_ADD_CTL ,0x000);	
	 I2C_WriteCodecReg(CODEC_ADCL_SET,0x1ff);	
	 I2C_WriteCodecReg(CODEC_MIC_CTL ,0x003);			
	 I2C_WriteCodecReg(CODEC_PGA_SET, 0x13f);	
	 I2C_WriteCodecReg(CODEC_BOOST_CTL,0x000);    
}

void I2S_Wm8976RecordPlaybak(U32 pRecordlength , U32 pClockconfig)
{
	 write_reg(GPIO_PORTA_SEL ,0x01);	 
	 write_reg(GPIO_PORTA_DATA,0x01);	 
	 write_reg(GPIO_PORTA_DIR ,0x01);	 	 
	 I2C_InitCodec();    
	 I2C_WriteCodecReg(CODEC_SRST,0x000);	 
	 I2C_WriteCodecReg(CODEC_POW1,0x01D);  	 
	 I2C_WriteCodecReg(CODEC_POW2,0x195);	 
	 I2C_WriteCodecReg(CODEC_POW3,0x00f);
	 I2C_WriteCodecReg(CODEC_AUDIO_SET , pRecordlength);	 
	 I2C_WriteCodecReg(CODEC_AUDIO_CTL,0x000);	 
	 I2C_WriteCodecReg(CODEC_CGC , pClockconfig);	 
	 I2C_WriteCodecReg(CODEC_ADD_CTL ,0x001);	 
	 I2C_WriteCodecReg(CODEC_MIXL_CTL,0x001);	 
	 I2C_WriteCodecReg(CODEC_MIXR_CTL,0x001); 
	 I2C_WriteCodecReg(CODEC_LOUT1VOL,0x13f);	 
	 I2C_WriteCodecReg(CODEC_ROUT1VOL,0x13f);	 
	 I2C_WriteCodecReg(CODEC_ADCL_SET,0x1ff);	
	 I2C_WriteCodecReg(CODEC_MIC_CTL ,0x003);			
	 I2C_WriteCodecReg(CODEC_PGA_SET, 0x13f);	
	 I2C_WriteCodecReg(CODEC_BOOST_CTL,0x000);
}

