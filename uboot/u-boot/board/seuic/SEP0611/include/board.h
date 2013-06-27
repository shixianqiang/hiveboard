 /* board/seuic/sep0611/include/board.h
 *
 * Copyright (c) 2009-2012  Prochip
 *  woody.wu
 *
 * Southeast University ASIC SoC support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  07-23-2012	woody initial version
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

#ifdef CONFIG_TIGER_TERMINAL_BOARD
#include "board-terminal.h"
#endif

#endif

