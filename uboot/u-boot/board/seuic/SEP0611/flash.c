/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Gary Jennejohn, DENX Software Engineering, <garyj@denx.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/* #define DEBUG */

#include <common.h>
#include <environment.h>

#define FLASH_BANK_SIZE 0x1000000	/* 2 x   8 MB */
#define MAIN_SECT_SIZE  0x40000		/* 2 x 128 kB */

flash_info_t flash_info[CONFIG_SYS_MAX_FLASH_BANKS];


#define CMD_READ_ARRAY		0x00FF00FF
#define CMD_IDENTIFY		0x00900090
#define CMD_ERASE_SETUP		0x00200020
#define CMD_ERASE_CONFIRM	0x00D000D0
#define CMD_PROGRAM		0x00400040
#define CMD_RESUME		0x00D000D0
#define CMD_SUSPEND		0x00B000B0
#define CMD_STATUS_READ		0x00700070
#define CMD_STATUS_RESET	0x00500050

#define BIT_BUSY		0x00800080
#define BIT_ERASE_SUSPEND	0x00400040
#define BIT_ERASE_ERROR		0x00200020
#define BIT_PROGRAM_ERROR	0x00100010
#define BIT_VPP_RANGE_ERROR	0x00080008
#define BIT_PROGRAM_SUSPEND	0x00040004
#define BIT_PROTECT_ERROR	0x00020002
#define BIT_UNDEFINED		0x00010001

#define BIT_SEQUENCE_ERROR	0x00300030
#define BIT_TIMEOUT		0x80000000

/*-----------------------------------------------------------------------
 */

ulong flash_init (void)
{
	return 12;
}
/*-----------------------------------------------------------------------
 */
void flash_print_info (flash_info_t * info)
{
	
}
/*-----------------------------------------------------------------------
 */

int flash_error (ulong code)
{
	/* Check bit patterns */
	/* SR.7=0 is busy, SR.7=1 is ready */
	/* all other flags indicate error on 1 */
	/* SR.0 is undefined */
	/* Timeout is our faked flag */

	/* sequence is described in Intel 290644-005 document */

	/* check Timeout */
	if (code & BIT_TIMEOUT) {
		puts ("Timeout\n");
		return ERR_TIMOUT;
	}

	/* check Busy, SR.7 */
	if (~code & BIT_BUSY) {
		puts ("Busy\n");
		return ERR_PROG_ERROR;
	}

	/* check Vpp low, SR.3 */
	if (code & BIT_VPP_RANGE_ERROR) {
		puts ("Vpp range error\n");
		return ERR_PROG_ERROR;
	}

	/* check Device Protect Error, SR.1 */
	if (code & BIT_PROTECT_ERROR) {
		puts ("Device protect error\n");
		return ERR_PROG_ERROR;
	}

	/* check Command Seq Error, SR.4 & SR.5 */
	if (code & BIT_SEQUENCE_ERROR) {
		puts ("Command seqence error\n");
		return ERR_PROG_ERROR;
	}

	/* check Block Erase Error, SR.5 */
	if (code & BIT_ERASE_ERROR) {
		puts ("Block erase error\n");
		return ERR_PROG_ERROR;
	}

	/* check Program Error, SR.4 */
	if (code & BIT_PROGRAM_ERROR) {
		puts ("Program error\n");
		return ERR_PROG_ERROR;
	}

	/* check Block Erase Suspended, SR.6 */
	if (code & BIT_ERASE_SUSPEND) {
		puts ("Block erase suspended\n");
		return ERR_PROG_ERROR;
	}

	/* check Program Suspended, SR.2 */
	if (code & BIT_PROGRAM_SUSPEND) {
		puts ("Program suspended\n");
		return ERR_PROG_ERROR;
	}

	/* OK, no error */
	return ERR_OK;
}

/*-----------------------------------------------------------------------
 */

int flash_erase (flash_info_t * info, int s_first, int s_last)
{
	return 12;
}
/*-----------------------------------------------------------------------
 * Copy memory to flash
 */

static int write_word (flash_info_t * info, ulong dest, ulong data)
{
	return 12;
}
/*-----------------------------------------------------------------------
 * Copy memory to flash.
 */

int write_buff (flash_info_t * info, uchar * src, ulong addr, ulong cnt)
{
	return 12;
}
