/* linux/sound/soc/sep/alsa_debug.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *  cgm <chenguangming@wxseuic.com>
 *
 * Southeast University ASIC SoC support
 *
 * ALSA debug switch
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  05-31-2011	cgm initial version
 *
 */
 
#ifndef _ALSA_DEBUG_H
#define _ALSA_DEBUG_H

//#define _ALSA_DEBUG_ON
//#define _DEBUG_I2S_REGS_VIEW
//#define _DEBUG_DMAC_REGS_VIEW
//#define _DEBUG_CODEC_REGS_VIEW

#ifdef _ALSA_DEBUG_ON
	#define alsa_dbg(fmt, args...) printk(fmt, ##args)
#else
	#define alsa_dbg(fmt, args...)
#endif

#endif /* _ALSA_DEBUG_H */
