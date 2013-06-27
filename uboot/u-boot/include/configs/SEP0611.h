/*
* SEP0611
*
*2009-11-2 rockie cheng
*/

#ifndef __CONFIG_H
#define __CONFIG_H

#include <asm/arch/hardware.h>
#include <../board/seuic/SEP0611/include/board.h>
#include <config_cmd_default.h>
/* High Level Configuration Options(easy to change) */
//#define DEBUG
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_USE_IRQ
#define CONFIG_ZIMAGE_BOOT
#define CONFIG_SKIP_RELOCATE_UBOOT			
#define CONFIG_SYS_TEXT_BASE	0x40400000
#define TEXT_BASE CONFIG_SYS_TEXT_BASE	

/* Size of malloc() pool */
#define CONFIG_MONITOR_LEN			(256 * 1024)		/* Reserve 256 kB for Monitor	*/
#define CONFIG_ENV_SIZE				0x4000				/* 1024 bytes may be used for env vars*/
#define CONFIG_SYS_MALLOC_LEN			(CONFIG_ENV_SIZE + 128*1024 )
#define CONFIG_SYS_GBL_DATA_SIZE		128					/* size in bytes reserved for initial data */
#define CFG_MALLOC_LEN	CONFIG_SYS_MALLOC_LEN
#define CFG_GBL_DATA_SIZE CONFIG_SYS_GBL_DATA_SIZE		
//#define CONFIG_STACKSIZE_IRQ
//#define CONFIG_STACKSIZE_FIQ

#undef  CONFIG_SHOW_ACTIVITY 

/* select serial console configuration */
#define CONFIG_SERIAL1			1	/* we use Serial line 1 */

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_BAUDRATE			115200
#define CONFIG_BOOTP_MASK    	(CONFIG_BOOTP_DEFAULT|CONFIG_BOOTP_BOOTFILESIZE)

#define CONFIG_NET_MULTI	1
#define BOARD_LATE_INIT		1
#define CONFIG_DRIVER_DM9000	1
#define CONFIG_DM9000_USE_16BIT
#define CONFIG_DM9000_NO_SROM
#define CONFIG_DM9000_BASE	0xa1000000
#define DM9000_IO	0xa1000000
#define DM9000_DATA	0xa1000800
//#define CONFIG_DM9000_DEBUG


/* Command line configuration.*/
#define CONFIG_CMD_MKGENFS
#define CONFIG_CMD_SDFATREAD
#define CONFIG_CMD_AUTOSCRIPT	/* Autoscript Support		*/
#define CONFIG_CMD_BDI	       	/* bdinfo			*/
#define CONFIG_CMD_BOOTD	/* bootd			*/
#define CONFIG_CMD_CONSOLE	/* coninfo			*/
#define CONFIG_CMD_ECHO		/* echo arguments		*/
#define CONFIG_CMD_ENV		/* saveenv			*/
#define CONFIG_CMD_FLASH	/* flinfo, erase, protect	*/
#define CONFIG_CMD_IMI		/* iminfo			*/
#define CONFIG_CMD_ITEST	/* Integer (and string) test	*/
#define CONFIG_CMD_LOADB	/* loadb			*/
#define CONFIG_CMD_LOADS	/* loads			*/
#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop mtest */
#define CONFIG_CMD_MISC		/* Misc functions like sleep etc*/
#define CONFIG_CMD_RUN		/* run command in env variable	*/


#define CONFIG_BOOTDELAY	3
#define CONFIG_ETHADDR		00:50:c2:1e:af:fb
//#define CONFIG_BOOTARGS  	"root=/dev/nfs rw nfsroot=192.168.0.3:/nfs,v3 ip=192.168.0.2:192.168.0.3:192.168.0.1:255.255.255.0:sep0611:eth0:off console=ttyS0,115200 mem=256mb" 
//#define CONFIG_BOOTARGS  	"root=/dev/mtdblock4 rootfstype=yaffs console=ttyS0,115200 mem=208mb"
#define CONFIG_BOOTARGS "root=/dev/mmcblk0p2 rw console=ttyS0,115200 mem=80mb rootdelay=2 init=/init"
//#define CONFIG_BOOTARGS  	"set bootargs console=ttyS0,115200 ubi.mtd=2 root=ubi0_0 rootfstype=ubifs mem=216mb"
//#define CONFIG_BOOTARGS "root=/dev/ram0 rw console=ttyS0,115200 initrd=0x41000000,0x800000 mem=216mb init=/init"

#define CONFIG_NETMASK  	255.255.255.0
#define CONFIG_IPADDR   	192.168.0.2
#define CONFIG_SERVERIP		192.168.0.1

//#define CONFIG_BOOTCOMMAND  "tftp 0x40008000 uImage; bootm 0x40008000"
#define CONFIG_BOOTCOMMAND  "sdfatread uImage 0x40c00000 ;bootm 0x40c00000"
//#define CONFIG_BOOTCOMMAND  "mtdparts default; nand read 0x41000000 0x1000000 0x800000; nand read 0x40008000 0x400000 0x300000; bootm 0x40008000;" 
 
#define	CONFIG_CMD_NET 
#define	CONFIG_CMD_PING 
#define CONFIG_CMD_DHCP 
#define	CONFIG_CMD_DATE 	
#define	CONFIG_CMD_ELF		
#define	CONFIG_CMD_ENV		
#define	CONFIG_CMD_FLASH 
		
/* Miscellaneous configurable options */
#define	CONFIG_SYS_LONGHELP									/* undef to save memory		*/
#define	CONFIG_SYS_PROMPT			"SEP0611=>"				/* Monitor Command Prompt	*/
#define	CONFIG_SYS_CBSIZE			256						/* Console I/O Buffer Size	*/
#define	CONFIG_SYS_PBSIZE 			(CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16) 	/* Print Buffer Size */
#define	CONFIG_SYS_MAXARGS			16								/* max number of command args	*/
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE					/* Boot Argument Buffer Size	*/

#define CONFIG_SYS_MEMTEST_START	0x40740000				/* memtest works on	*/
#define CONFIG_SYS_MEMTEST_END	0x40780000					/* 4 ... 8 MB in DRAM	*/

#undef  CONFIG_SYS_CLKS_IN_HZ										/* everything, incl board info, in Hz */
#define	CONFIG_SYS_LOAD_ADDR		0x40008000						/* default load address	*/
#define	CONFIG_SYS_HZ				50000000UL						/* 50MHz */
						
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }	/* valid baudrates */

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	(128*1024)	/* regular stack */

#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	(4*1024)	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	(4*1024)	/* FIQ stack */
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	1	   		/* we have 1 banks of DRAM */
#define PHYS_SDRAM_1				0x40000000 	/* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE		0x02000000 	/* 32 MB */

#define CONFIG_SYS_MAX_FLASH_SECT	1
#define CONFIG_ENV_IS_NOWHERE		1
#define CONFIG_SYS_MAX_FLASH_BANKS	1

/*-----------------------------------------------------------------------
 * nand config
 */
#define NAND_MAX_CHIPS          1
#define CONFIG_BOOT_NAND
#define CONFIG_NAND
#define NAND_ENABLE_CE()	(NFCONF_REG &= ~(1 << 16))
#define NAND_DISABLE_CE()  (NFCONF_REG |=(1 << 16))
#define NF_TRANSRnB()		do { while(!(NFSTAT_REG & (1 << 0))); } while(0)

//nand
#define CONFIG_CMD_NAND
#define CONFIG_SYS_NAND_BASE_LIST	{ 0x02000000, }
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_MAX_CHIPS	1
#define CONFIG_NAND_SEP0611

#define CONFIG_CMD_NAND_YAFFS
#define CONFIG_YAFFS2
#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_DEVICE

#define MTDIDS_DEFAULT			"nand0=nand0"

#define MTDPARTS_DEFAULT		"mtdparts=nand0:"	\
									"2M(uboot)ro,"	\
									"14M(kernel),"	\
									"16M(ramdisk),"	\
									"992M(system),"	\
									"1G(data),"	 

/*-----------------------------------------------------------------------
 * usb device config. fgs
 */
#define CONFIG_USB_SEP0611      1
#define CONFIG_MUSB_UDC			1
#define CONFIG_USB_DEVICE 

#define CONFIG_UBOOT_BASE		0x40400000

/************************************************************************
*Add support for read data from sd card
************************************************************************/
//#define CONFIG_CMD_MMCREAD
//#define CONFIG_SEP_MMC
/*
 * Linux TAGs (see lib_arm/armlinux.c)
*/
#define CONFIG_CMDLINE_TAG
#undef CONFIG_SETUP_MEMORY_TAGS
//#define CONFIG_INITRD_TAG
#define CONFIG_CMDLINE_EDITING
#define CONFIG_AUTO_COMPLETE

#endif	/* __CONFIG_H */

