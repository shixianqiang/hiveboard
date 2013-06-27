/*
 * common/cmd_usbslave.c
 * Code specific to ASIC center of SouthEast University
 * 
 * Maintained by Fan Guisheng (Neo)<fgsink@gmail.com>
 * 
 * Copyright (C) 2001-2011 ASIC & SouthEast University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Contributors & Additions/Fixes:
 *  First version by fgs(Neo)
 * 
 * TODO:
 */

#include <common.h>
#include <command.h>
#include <asm/byteorder.h>

#ifdef CONFIG_USB_DEVICE

#ifdef CONFIG_USE_IRQ
    #define IRQ_STACK_START    (_unicoreboot_start - CONFIG_SYS_MALLOC_LEN - CONFIG_SYS_GBL_DATA_SIZE - 4)
    #define FIQ_STACK_START    (IRQ_STACK_START - CONFIG_STACKSIZE_IRQ)
    #define FREE_RAM_END        (FIQ_STACK_START - CONFIG_STACKSIZE_FIQ - CONFIG_STACKSIZE)
    #define FREE_RAM_SIZE        (FREE_RAM_END - PHYS_SDRAM_1)
#else
    #define    FREE_RAM_END    (_unicoreboot_start - CONFIG_SYS_MALLOC_LEN - CONFIG_SYS_GBL_DATA_SIZE - 4 - CONFIG_STACKSIZE)
    #define    FREE_RAM_SIZE    (FREE_RAM_END - PHYS_SDRAM_1)
#endif

int g_bUSBWait = 1;
unsigned int g_dwDownloadLen = 0;

extern int dnw_configured_flag;

extern int download_run;
extern volatile u32 dwUSBBufBase;
extern volatile u32 dwUSBBufSize;
extern volatile u32 dwUSBBufReadPtr;
extern volatile u32 dwUSBBufWritePtr;
extern volatile u32 dwWillDMACnt;
extern volatile u32 bDMAPending;

extern volatile u32 tempDownloadAddress;
extern volatile u32 downloadAddress;
extern volatile unsigned char *downPt;
extern volatile u32 downloadFileSize;
extern volatile u16 checkSum;
extern volatile int isUsbdSetConfiguration;
extern volatile u32 totalDmaCount;
extern volatile u32 fifo_full_ok;//0-empty, 1-full

extern u32 usb_receive(char *buf, size_t len, unsigned int wait);

int do_usbload (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    size_t len = ~0UL;
    char buf[32];

    download_run = 1;
    switch (argc) {
        case 1:
        {
            break;
        }
        case 2:
        {
            g_bUSBWait = (int)simple_strtoul(argv[1], NULL, 16);
            break;
        }

        case 3:
        {
            g_bUSBWait = (int)simple_strtoul(argv[1], NULL, 16);
            load_addr = simple_strtoul(argv[2], NULL, 16);
            download_run = 0;
            break;
        }
        
        default:
        {
            printf ("Usage:\n%s\n", cmdtp->usage);
            return 1;
        }
    }

    dwUSBBufBase = load_addr;
    dwUSBBufSize = (FREE_RAM_SIZE&(~(0x80000-1)));
    if (g_bUSBWait)
        len = FREE_RAM_SIZE;

    g_dwDownloadLen = usb_receive((char *)dwUSBBufBase, len, g_bUSBWait);

    sprintf(buf, "%X", g_dwDownloadLen);
    setenv("filesize", buf);
    
    downloadFileSize = 0;    
    
    return 0;
}

U_BOOT_CMD(
    usbload,    3,    0,    do_usbload,
    "usbload - Get file from host(PC), use DNW tool.",
    "[loadAddress]\n"
);

#endif
