/*
 * (C) Copyright 2002-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
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

/*
 * To match the U-Boot user interface on unicore platforms to the U-Boot
 * standard (as on PPC platforms), some messages with debug character
 * are removed from the default U-Boot build.
 *
 * Define DEBUG here if you want additional info as shown below
 * printed upon startup:
 *
 * U-Boot code: 00F00000 -> 00F3C774  BSS: -> 00FC3274
 * IRQ Stack: 00ebff7c
 * FIQ Stack: 00ebef7c
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <stdio_dev.h>
#include <timestamp.h>
#include <version.h>
#include <net.h>
#include <serial.h>
#include <nand.h>
#include <onenand_uboot.h>
#include <mmc.h>
#include <asm/u-boot-unicore.h>
#include <asm/arch/hardware.h>
#include <asm/types.h>

extern int dnw_init(void);

#ifdef CONFIG_BITBANGMII
#include <miiphy.h>
#endif

#ifdef CONFIG_DRIVER_SMC91111
#include "../drivers/net/smc91111.h"
#endif
#ifdef CONFIG_DRIVER_LAN91C96
#include "../drivers/net/lan91c96.h"
#endif

u32 i , j , k; 
u32 random1 , random2 , random[65]={0};

//DECLARE_GLOBAL_DATA_PTR;
volatile gd_t *gd;

ulong monitor_flash_len;

#ifdef CONFIG_HAS_DATAFLASH
extern int  AT91F_DataflashInit(void);
extern void dataflash_print_info(void);
#endif

#ifndef CONFIG_IDENT_STRING
#define CONFIG_IDENT_STRING ""
#endif

const char version_string[] =
	"U-Boot 2011.03 (" U_BOOT_DATE " - " U_BOOT_TIME ")"CONFIG_IDENT_STRING;

#ifdef CONFIG_DRIVER_RTL8019
extern void rtl8019_get_enetaddr (uchar * addr);
#endif

#if defined(CONFIG_HARD_I2C) || \
    defined(CONFIG_SOFT_I2C)
#include <i2c.h>
#endif


/************************************************************************
 * Coloured LED functionality
 ************************************************************************
 * May be supplied by boards if desired
 */
void inline __coloured_LED_init (void) {}
void coloured_LED_init (void) __attribute__((alias("__coloured_LED_init")));
void inline __red_LED_on (void) {}
void red_LED_on (void) __attribute__((alias("__red_LED_on")));
void inline __red_LED_off(void) {}
void red_LED_off(void) __attribute__((alias("__red_LED_off")));
void inline __green_LED_on(void) {}
void green_LED_on(void) __attribute__((alias("__green_LED_on")));
void inline __green_LED_off(void) {}
void green_LED_off(void) __attribute__((alias("__green_LED_off")));
void inline __yellow_LED_on(void) {}
void yellow_LED_on(void) __attribute__((alias("__yellow_LED_on")));
void inline __yellow_LED_off(void) {}
void yellow_LED_off(void) __attribute__((alias("__yellow_LED_off")));
void inline __blue_LED_on(void) {}
void blue_LED_on(void) __attribute__((alias("__blue_LED_on")));
void inline __blue_LED_off(void) {}
void blue_LED_off(void) __attribute__((alias("__blue_LED_off")));

/************************************************************************
 * Init Utilities							*
 ************************************************************************
 * Some of this code should be moved into the core functions,
 * or dropped completely,
 * but let's get it working (again) first...
 */

#if defined(CONFIG_unicore_DCC) && !defined(CONFIG_BAUDRATE)
#define CONFIG_BAUDRATE 115200
#endif
static int init_baudrate (void)
{
	char tmp[64];	/* long enough for environment variables */
	int i = getenv_r ("baudrate", tmp, sizeof (tmp));
	gd->bd->bi_baudrate = gd->baudrate = (i > 0)
			? (int) simple_strtoul (tmp, NULL, 10)
			: CONFIG_BAUDRATE;

	return (0);
}

static int display_banner (void)
{
	printf ("\n\n%s\n\n", version_string);
	printf (" ************************************\n");
	printf (" **       SEP6200 BOOTLOADER       **\n");
	printf (" **             V1.0               **\n");
	printf (" **     (c)Copyright Prochip Inc.  **\n");
	printf (" ************************************\n\n");
	debug ("U-Boot code: %08lX -> %08lX  BSS: -> %08lX\n",
	       _unicoreboot_start, _bss_start, _bss_end);
#ifdef CONFIG_MODEM_SUPPORT
	debug ("Modem Support enabled\n");
#endif
#ifdef CONFIG_USE_IRQ
	debug ("IRQ Stack: %08lx\n", IRQ_STACK_START);
	debug ("FIQ Stack: %08lx\n", FIQ_STACK_START);
#endif

	return (0);
}

/*
 * WARNING: this code looks "cleaner" than the PowerPC version, but
 * has the disadvantage that you either get nothing, or everything.
 * On PowerPC, you might see "DRAM: " before the system hangs - which
 * gives a simple yet clear indication which part of the
 * initialization if failing.
 */
static int display_dram_config (void)
{
	int i;

#ifdef DEBUG
	puts ("RAM Configuration:\n");

	for(i=0; i<CONFIG_NR_DRAM_BANKS; i++) {
		printf ("Bank #%d: %08lx ", i, gd->bd->bi_dram[i].start);
		print_size (gd->bd->bi_dram[i].size, "\n");
	}
#else
	ulong size = 0;
#if 0
	for (i=0; i<CONFIG_NR_DRAM_BANKS; i++) {
		size += gd->bd->bi_dram[i].size;
	}
	puts("DRAM:  ");
	print_size(size, "\n");
#endif
#endif

	return (0);
}

#ifndef CONFIG_SYS_NO_FLASH
static void display_flash_config (ulong size)
{
	puts ("Flash: ");
	print_size (size, "\n");
}
#endif /* CONFIG_SYS_NO_FLASH */

#if defined(CONFIG_HARD_I2C) || defined(CONFIG_SOFT_I2C)
static int init_func_i2c (void)
{
	puts ("I2C:   ");
	i2c_init (CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
	puts ("ready\n");
	return (0);
}
#endif

#if defined(CONFIG_CMD_PCI) || defined (CONFIG_PCI)
#include <pci.h>
static int unicore_pci_init(void)
{
	pci_init();
	return 0;
}
#endif /* CONFIG_CMD_PCI || CONFIG_PCI */

/*
 * Breathe some life into the board...
 *
 * Initialize a serial port as console, and carry out some hardware
 * tests.
 *
 * The first part of initialization is running from Flash memory;
 * its main purpose is to initialize the RAM so that we
 * can relocate the monitor code to RAM.
 */

/*
 * All attempts to come up with a "common" initialization sequence
 * that works for all boards and architectures failed: some of the
 * requirements are just _too_ different. To get rid of the resulting
 * mess of board dependent #ifdef'ed code we now make the whole
 * initialization sequence configurable to the user.
 *
 * The requirements for any new initalization function is simple: it
 * receives a pointer to the "global data" structure as it's only
 * argument, and returns an integer return code, where 0 means
 * "continue" and != 0 means "fatal error, hang the system".
 */
typedef int (init_fnc_t) (void);

int print_cpuinfo (void);

init_fnc_t *init_sequence[] = {
#if defined(CONFIG_ARCH_CPU_INIT)
	arch_cpu_init,		/* basic arch cpu dependent setup */
#endif
	board_init,		/* basic board dependent setup */
	//timer_test,
#if defined(CONFIG_USE_IRQ)
	interrupt_init,		/* set up exceptions */
#endif
	//timer_init,		/* initialize timer */
#ifdef CONFIG_FSL_ESDHC
	get_clocks,
#endif
	env_init,		/* initialize environment */
	init_baudrate,		/* initialze baudrate settings */
	serial_init,
		/* serial communications setup */
	console_init_f,		/* stage 1 init of console */
	display_banner,		/* say that we are here */
#if defined(CONFIG_DISPLAY_CPUINFO)
	print_cpuinfo,		/* display cpu info (and speed) */
#endif
#if defined(CONFIG_DISPLAY_BOARDINFO)
	checkboard,		/* display board info */
#endif
#if defined(CONFIG_HARD_I2C) || defined(CONFIG_SOFT_I2C)
	init_func_i2c,
#endif
	dram_init,		/* configure available RAM banks */
#if defined(CONFIG_CMD_PCI) || defined (CONFIG_PCI)
	unicore_pci_init,
#endif
	display_dram_config, /* This line is a bad guy!!!!!*/
	NULL,
};

static int sep0611_gpioconfig(void)
{     //power hold    
     *(volatile unsigned int*)(GPIO_AOPORT_DIR) &= ~(1<<2);
     *(volatile unsigned int*)(GPIO_AOPORT_SEL) |= (1<<2);
     *(volatile unsigned int*)(GPIO_AOPORT_DATA) |= (1<<2);

     *(volatile unsigned int*)GPIO_PORTF_SEL = 0x0;

     /*Configure GPIO for UART0, but not UART1, UART2, UART3 */
     *(volatile unsigned int*)GPIO_PORTD_SEL &= ~(0x3<<10);
     *(volatile unsigned int*)GPIO_PORTD_SEL |= 0x3ff; 

     /*EMI Data bus for Dm9000   */
     *(volatile unsigned int*)GPIO_PORTB_SEL &= ~(0xFFFF<<0);  // for Etherne    t
     /*EMI Address bus for Dm9000, only ADDRESS[10] is needed */
     *(volatile unsigned int*)GPIO_PORTA_SEL &= ~(0x1<<10);   //CMD
     /*EMI for DM9000, OE WE CS are needed.*/
     *(volatile unsigned int*)GPIO_PORTA_SEL &= ~(0x17<<12);  //OE WE CS
     /*GPIO for DM9000, .*/
     *(volatile unsigned int*)GPIO_PORTC_DIR &= ~(0x1<<3); //PC3 output
     *(volatile unsigned int*)GPIO_PORTC_SEL |= (0x1<<3);
     *(volatile unsigned int*)GPIO_PORTC_DATA |= (0x1<<3);     //LAN_RST

     *(volatile unsigned int*)GPIO_PORTE_DIR &= ~(0x1<<1); //PE1 output
     *(volatile unsigned int*)GPIO_PORTE_SEL |= (0x1<<1);
     *(volatile unsigned int*)GPIO_PORTE_DATA &= ~(0x1<<1);     //LAN_EN

     /*FIXME:Attention, wo should not confiure EMI in this way  */
     *(volatile unsigned *)(0xb2020094) = 0x3ffca;
     return 0 ;
}

static int sep0611_gpioconfig_woody(void)
{
#ifdef CONFIG_TIGER_TERMINAL_BOARD
    /*Configure GPIO AO PORT for power hold (AP2, HIGH)*/	
//    *(volatile unsigned int*)(GPIO_AOPORT_SEL) |= (1<<2);
//    *(volatile unsigned int*)(GPIO_AOPORT_DIR) &= ~(1<<2);
//	*(volatile unsigned int*)(GPIO_AOPORT_DATA) |= (1<<2);
    *(volatile unsigned int*)(GPIO_AOPORT_SEL) = 0x7c ;  // gpio and interrupt
    *(volatile unsigned int*)(GPIO_AOPORT_DATA) = 0x74 ; // AP2 HIGH
    *(volatile unsigned int*)(GPIO_AOPORT_DIR) = 0xab ;
    *(volatile unsigned int*)(GPIO_AOPORT_DATA) = 0x74 ; // AP3 input
#endif

    /*Configure GPIO PORT F*/ 
//	*(volatile unsigned int*)GPIO_PORTF_SEL = 0x0;
    *(volatile unsigned int*)GPIO_PORTF_SEL = 0x00000010 ; //0x10
    *(volatile unsigned int*)GPIO_PORTF_DATA = 0x00000000 ; //low for led
    *(volatile unsigned int*)GPIO_PORTF_DIR = 0x1fffffef ; // PF4 output

	/*Configure GPIO PORT D for UART0, UART1, UART2, UART3 */
    *(volatile unsigned int*)GPIO_PORTD_SEL &= (0x3<<4); //0x30
    *(volatile unsigned int*)GPIO_PORTD_SEL |= (0x3<<4); //0x30
    *(volatile unsigned int*)GPIO_PORTD_DATA &= ~(0x3<<4) ; // LOW 
    *(volatile unsigned int*)GPIO_PORTD_DIR = 0x7ffcf ; // PD4/5 output

	/*PORT B , EMI Data bus for Dm9000   */
    *(volatile unsigned int*)GPIO_PORTB_SEL = 0x00 ;  //for SRAM DATA
    *(volatile unsigned int*)GPIO_PORTB_DIR = 0xffff ;             

	/*PORT A , EMI Address bus for Dm9000, only ADDRESS[10] is needed */
    *(volatile unsigned int*)GPIO_PORTA_SEL = 0x08bff ; //0x8bff
    *(volatile unsigned int*)GPIO_PORTA_DATA = 0x7c ;
    *(volatile unsigned int*)GPIO_PORTA_DIR = 0x17400 ; // for output
	
        /*EMI for DM9000, OE WE CS are needed.*/
//	*(volatile unsigned int*)GPIO_PORTA_SEL &= ~(0x17<<12);  //OE WE CS

	/*PORT C , General purpose GPIO and NAND .*/
//    *(volatile unsigned int*)GPIO_PORTC_SEL = 0xc0cf ;
//    *(volatile unsigned int*)GPIO_PORTC_DATA = 0xc0cc ;
//    *(volatile unsigned int*)GPIO_PORTC_DIR = 0x3f30 ; //PC6/7/14/15 output
      *(volatile unsigned int*)GPIO_PORTC_SEL = 0x000f ;
      

    /*Configure GPIO PORT I AND Interrupt*/
    *(volatile unsigned int*)GPIO_PORTI_SEL = 0x129f ;
    *(volatile unsigned int*)GPIO_PORTI_DATA = 0x0010 ; //PI7/12 input
    *(volatile unsigned int*)GPIO_PORTI_DIR = 0xfdef ;
    *(volatile unsigned int*)GPIO_PORTI_DATA = 0x0010 ; //PI9/4 output

    /*Configure GPIO PORT E*/
    *(volatile unsigned int*)GPIO_PORTE_SEL = 0x00000f ;
    *(volatile unsigned int*)GPIO_PORTE_DATA &= ~(0xf<<0) ; //low 
    *(volatile unsigned int*)GPIO_PORTE_DIR &= ~(0xf<<0) ; //PE0/1/2/3 output

	/*FIXME:Attention, do not confiure EMI in this way  */
//	*(volatile unsigned *)(0xb2020094) = 0x3ffca;
	return 0 ;
	
}


void start_unicoreboot(void)
{
	init_fnc_t **init_fnc_ptr;

	sep0611_gpioconfig();
	/* Pointer is writable since we allocated a register for it */
	gd = (gd_t*)(_unicoreboot_start - CONFIG_SYS_MALLOC_LEN - sizeof(gd_t));
	/* compiler optimization barrier needed for GCC >= 3.4 */
	__asm__ __volatile__("": : :"memory");

	memset ((void*)gd, 0, sizeof (gd_t));
	gd->bd = (bd_t*)((char*)gd - sizeof(bd_t));
	memset (gd->bd, 0, sizeof (bd_t));

	gd->flags |= GD_FLG_RELOC;

	monitor_flash_len = _bss_start - _unicoreboot_start;

	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) {
		if ((*init_fnc_ptr)() != 0) {
			hang ();
		}
	}
	/* unicoreboot_start is defined in the board-specific linker script */
	mem_malloc_init (_unicoreboot_start - CONFIG_SYS_MALLOC_LEN, CONFIG_SYS_MALLOC_LEN);


#if defined(CONFIG_CMD_NAND)
	puts ("NAND:  ");
	nand_init();		/* go init the NAND */
#endif


	/* initialize environment */
	env_relocate ();

	/* initialize the eth */
#if defined(CONFIG_CMD_NET)
	eth_initialize(gd->bd);
#endif

	/* IP Address */
	gd->bd->bi_ip_addr = getenv_IPaddr ("ipaddr");
		/* MAC Address */
	{
		int i;
		ulong reg;
		char *s, *e;
		char tmp[64];

		i = getenv_r ("ethaddr", tmp, sizeof (tmp));
		s = (i > 0) ? tmp : NULL;

		for (reg = 0; reg < 6; ++reg) {
			gd->bd->bi_enetaddr[reg] = s ? simple_strtoul (s, &e, 16) : 0;
			if (s)
				s = (*e) ? e + 1 : e;
		}
	}

	stdio_init ();	/* get the devices list going. */

	jumptable_init ();


	console_init_r ();	/* fully init console as a device */

//	dnw_init();
	/* enable exceptions */
	enable_interrupts ();
	for(;;){
		printf("OK!\n");
		main_loop();
	}
}

void hang (void)
{
	puts ("### ERROR ### Please RESET the board ###\n");
	for (;;);
}
