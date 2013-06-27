/* linux/arch/unicore/mach-sep0611/include/board/board.h
 *
 * Copyright (c) 2009-2011 SEUIC
 *  cgm <chenguangming@wxseuic.com>
 *
 * Southeast University ASIC SoC support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  05-31-2011	cgm initial version
 *
 */

#ifndef __ASM_BOARD_H
#define __ASM_BOARD_H

#ifdef CONFIG_TIGER_DEMO_BOARD
	#include "board-demo.h"
#endif

#ifdef CONFIG_TIGER_TEST_BOARD
	#include "board-test.h"
#endif

#ifdef CONFIG_TIGER_SPDW_BOARD
	#include "board-spdw.h"
#endif

#ifdef CONFIG_HIVEBOARD
	#include "hiveboard.h"
#endif


#endif
