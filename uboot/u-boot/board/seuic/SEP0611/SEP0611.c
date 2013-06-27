/*
 * (C) Copyright 2004
 * DAVE Srl
 * http://www.dave-tech.it
 * http://www.wawnet.biz
 * mailto:info@wawnet.biz
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

#include <common.h>
#include <asm/arch/hardware.h>
#include <net.h>
#include <asm/mach-types.h>


extern int dm9000_initialize(bd_t *);
unsigned int * NF_Buf;

/*
 * Miscelaneous platform dependent initialization
 */
int board_init (void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gd->bd->bi_arch_number = 2556;
	gd->bd->bi_boot_params = 0x40000100;

	return 0;
}

int board_late_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;
#if defined(CONFIG_CMD_NET)
	eth_init(gd->bd);
#endif
	return 0;
}

int dram_init (void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

	return (0);
}
int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_DRIVER_DM9000
	/*First, do power resest for DM9000*/
#ifdef SEP0611_ETH_EN
	GPIO_OUT_HIGH(SEP0611_ETH_EN);
	udelay(1000);
#endif

#ifdef SEP0611_ETH_RST_
	GPIO_OUT_HIGH(SEP0611_ETH_RST_);
	udelay(10000);
	GPIO_OUT_LOW(SEP0611_ETH_RST_);
	udelay(10000);
	GPIO_OUT_HIGH(SEP0611_ETH_RST_);
#endif
	rc = dm9000_initialize(bis);
	return rc;
}
#endif
