/*
 * (C) Copyright 2011
 */
#include <common.h>
#include <command.h>
#include <mmc/SDIO1.h>
#include <asm/byteorder.h>

#define SECTOR_SIZE 0x200

static int sdhc_flag = 0;
extern void ReadMMC(U32, S32, U32);

int do_mmcread (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char buf[32];
	int loadAddr = 0x40500000;
	int startBlock = 8;
	int count = 500;

    switch (argc) {
        case 1:
        {
            printf ("Usage:\n%s\n", cmdtp->usage);
            return 1;
        }
        case 2:
        {
            loadAddr = (int)simple_strtoul(argv[1], NULL, 16);
			ReadMMC(startBlock, count, loadAddr);
            break;
        }
        case 3:
        {
            loadAddr = (int)simple_strtoul(argv[1], NULL, 16);
            startBlock = (int)simple_strtoul(argv[2], NULL, 16);
			ReadMMC(startBlock, count, loadAddr);
            break;
        }
        case 4:
        {
            loadAddr = (int)simple_strtoul(argv[1], NULL, 16);
            startBlock = (int)simple_strtoul(argv[2], NULL, 16);
            count = (int)simple_strtoul(argv[3], NULL, 16);
			ReadMMC(startBlock, count, loadAddr);
            break;
        }
        default:
        {
            printf ("Usage:\n%s\n", cmdtp->usage);
            return 1;
        }
    }

    sprintf(buf, "%X", loadAddr);
    setenv("mmcLoadAddr", buf);

    sprintf(buf, "%X", startBlock);
    setenv("mmcStartBlock", buf);

    sprintf(buf, "%X", count);
    setenv("mmcCount", buf);
    return 0;
}

U_BOOT_CMD(
    mmcread,    4,    0,    do_mmcread,
    "mmcread - Read Data from sd card to memory",
    "[ReadAddress] [LoadAddress]"
);
