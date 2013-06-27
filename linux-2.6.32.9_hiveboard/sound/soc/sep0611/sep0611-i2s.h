/* linux/sound/soc/sep/sep0611-i2s.h
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
 
#ifndef _SEP0611_I2S_H
#define _SEP0611_I2S_H

#define	SLAVE_MODE				0
#define	MASTER_MODE_HARDWARE	1
#define	MASTER_MODE_SOFTWARE	2

//#define CTRL_MODE SLAVE_MODE
#define CTRL_MODE MASTER_MODE_SOFTWARE
//#define CTRL_MODE MASTER_MODE_HARDWARE

struct sep0611_i2s_dma_params {
    char *name; /* stream identifier */
    int channel;
    dma_addr_t dma_addr;
};

extern struct snd_soc_dai sep0611_i2s_dai[];

#endif
