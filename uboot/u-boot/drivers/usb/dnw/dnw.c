/*
 * drivers/usb/dnw/dnw.c
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
#include <config.h>
#include <circbuf.h>
#include <stdio_dev.h>
#include "usb/musb_udc.h"
#include "usbdescriptors.h"
#include "usbdevice.h"
#include "../musb/musb_core.h"
#include "dnw.h"
#include "../musb/sep0611_musb.h"
#include <asm/io.h>
#include <asm/arch/hardware.h>

static void str2wide (char *str, u16 * wide);
static void dnw_init_endpoints (void);
int dnw_cdc_setup(struct usb_device_request *request, struct urb *urb);
static void dnw_event_handler (struct usb_device_instance *device,
				  usb_device_event_t event, int data);
static void dnw_init_instances (void);
static void dnw_init_strings (void);
static void dnw_init_type(void);

/*
 * Global flag
 */
int dnw_configured_flag = 0;

int download_run=0;
volatile u32 dwUSBBufBase;
volatile u32 dwUSBBufSize;
volatile u32 dwUSBBufReadPtr;
volatile u32 dwUSBBufWritePtr;
volatile u32 dwWillDMACnt;
volatile u32 bDMAPending;

volatile u32 tempDownloadAddress;
volatile u32 downloadAddress;
volatile unsigned char *downPt;
volatile u32 downloadFileSize;
volatile u16 checkSum;
volatile int isUsbdSetConfiguration;
volatile u32 totalDmaCount;
volatile u32 fifo_full_ok = 0;//0-empty, 1-full
volatile u32 ep2_first = 0;
/*
 * Serial number
 */
static char serial_number[16];

/* 
 * defined and used by gadget/ep0.c
 */
extern struct usb_string_descriptor **usb_strings;

/*
 * Indicies, References
 */
static unsigned short interface_count = 0;
static struct usb_string_descriptor *dnw_string_table[STR_COUNT];

/*
 * USB Descriptor Strings
 */
static u8 wstrLang[4] = {4,USB_DT_STRING,0x9,0x8};
static u8 wstrManufacturer[2 + 2*(sizeof(CONFIG_USBD_MANUFACTURER)-1)];
static u8 wstrProduct[2 + 2*(sizeof(CONFIG_USBD_PRODUCT_NAME)-1)];
static u8 wstrSerial[2 + 2*(sizeof(serial_number) - 1)];

/*
 * Instance variables
 */
static struct usb_device_instance device_instance[1];
static struct usb_bus_instance bus_instance[1];
static struct usb_configuration_instance config_instance[NUM_CONFIGS];
static struct usb_interface_instance interface_instance[MAX_INTERFACES];
static struct usb_alternate_instance alternate_instance[MAX_INTERFACES];

/* 
 * one extra for control endpoint
 */
static struct usb_endpoint_instance endpoint_instance[NUM_ENDPOINTS+1];

/* 
 * Standard USB Data Structures
 */
static struct usb_interface_descriptor interface_descriptors[MAX_INTERFACES];
static struct usb_endpoint_descriptor *ep_descriptor_ptrs[NUM_ENDPOINTS];
static struct usb_configuration_descriptor	*configuration_descriptor = 0;

static struct usb_device_descriptor device_descriptor = {
	.bLength         = 	sizeof(struct usb_device_descriptor),
	.bDescriptorType =	USB_DT_DEVICE,
	.bcdUSB          =	cpu_to_le16(0x0200),//USB_BCD_VERSION, changed for 2.0
	.bDeviceClass    = 	0xFF,//self defined device class
	.bDeviceSubClass =	0x00,
	.bDeviceProtocol =	0x00,
	.bMaxPacketSize0 =	64,
	.idVendor 		  =	cpu_to_le16(0x04e8),//0x5345, changed for 2.0
	.idProduct       = 	cpu_to_le16(0x1234),
	.bcdDevice       =	cpu_to_le16(0x0100),
	.iManufacturer   =	0x01,
	.iProduct        =	0x02,
	.iSerialNumber   =	0x0,
	.bNumConfigurations = 0x01
};


struct dnw_config_desc {
	struct usb_configuration_descriptor configuration_desc;
	struct usb_interface_descriptor interface_desc;
	struct usb_endpoint_descriptor data_endpoints[NUM_ENDPOINTS];
} __attribute__((packed));


static struct dnw_config_desc dnw_configuration_descriptors[NUM_CONFIGS] = {
	{
		.configuration_desc ={
			.bLength = 0x9,//sizeof(struct usb_configuration_descriptor),
			.bDescriptorType = USB_DT_CONFIG,
 			.wTotalLength = 0x20,//<cfg desc>+<if desc>+<endp0 desc>+<endp1 desc>
			.bNumInterfaces = 0x1,
			.bConfigurationValue = 1,
			.iConfiguration = 0,
			.bmAttributes = BMATTRIBUTE_SELF_POWERED|BMATTRIBUTE_RESERVED,
			.bMaxPower = 25
		},
		.interface_desc = {
            //Interface 1 
			.bLength  = 0x9,//sizeof(struct usb_interface_descriptor),
			.bDescriptorType = USB_DT_INTERFACE,
			.bInterfaceNumber = 0,
			.bAlternateSetting = 0,
			.bNumEndpoints = 0x02,//endpoints except EP0
			.bInterfaceClass = 0xff,
			.bInterfaceSubClass = 0x0,
			.bInterfaceProtocol = 0x0,
			.iInterface = 0x0,
		},
		.data_endpoints = {
			{//ep1 bulk in
				.bLength		    = 0x7,//sizeof(struct usb_endpoint_descriptor),
				.bDescriptorType	= USB_DT_ENDPOINT,
				.bEndpointAddress	= 1 | USB_DIR_IN,
				.bmAttributes		= USB_ENDPOINT_XFER_BULK,
				.wMaxPacketSize		= cpu_to_le16(EP1_PKT_SIZE),
				.bInterval		    = 0x0,//not used 
			},
			{//ep2 bulk out
				.bLength		    = 0x7,//sizeof(struct usb_endpoint_descriptor),
				.bDescriptorType	= USB_DT_ENDPOINT,
				.bEndpointAddress	= 2 | USB_DIR_OUT,
				.bmAttributes		= USB_ENDPOINT_XFER_BULK,
				.wMaxPacketSize		= cpu_to_le16(EP2_PKT_SIZE),
				.bInterval		    = 0x0,//not used 
			},
		},
	},
};

/* setting the device qualifier descriptor and a string descriptor */
const u8 qualifier_desc[] =
{
	0x0a,	/*  0 desc size */
	0x06,	/*  1 desc type (DEVICE_QUALIFIER)*/
	0x00,	/*  2 USB release */
	0x02,	/*  3 => 2.00*/
	0xFF,	/*  4 class */
	0x00,	/*  5 subclass */
	0x00,	/*  6 protocol */
	64,	/*  7 max pack size */
	0x01,	/*  8 number of other-speed configuration */
	0x00,	/*  9 reserved */
};
const u8 config_full[] =
{
	0x09,	/*  0 desc size */
	0x07,	/*  1 desc type (other speed)*/
	0x20,	/*  2 Total length of data returned */
	0x00,	/*  3 */
	0x01,	/*  4 Number of interfaces supported by this speed configuration */
	0x01,	/*  5 value to use to select configuration */
	0x00,	/*  6 index of string desc */
			/*  7 same as configuration desc */
	0x80|0x40,//	CONF_ATTR_DEFAULT|CONF_ATTR_SELFPOWERED,
	0x19,	/*  8 same as configuration desc */

};

const u8 config_full_total[] =
{
  0x09, 0x07 ,0x20 ,0x00 ,0x01 ,0x01 ,0x00 ,0xC0 ,0x19,
  0x09 ,0x04 ,0x00 ,0x00 ,0x02 ,0xff ,0x00 ,0x00 ,0x00,
  0x07 ,0x05 ,0x83 ,0x02 ,0x40 ,0x00 ,0x00,
  0x07 ,0x05 ,0x04 ,0x02 ,0x40 ,0x00 ,0x00
};
/* 
 * utility function for converting char* to wide string used by USB
 */
static void str2wide (char *str, u16 * wide)
{
	int i;
	for (i = 0; i < strlen (str) && str[i]; i++){
		#if defined(__LITTLE_ENDIAN)
			wide[i] = (u16) str[i];
		#elif defined(__BIG_ENDIAN)
			wide[i] = ((u16)(str[i])<<8);
		#else
			#error "__LITTLE_ENDIAN or __BIG_ENDIAN undefined"
		#endif
	}
}

/*
 * Reads and returns a character from the serial port
 *   - Times out after delay iterations checking for presence of character
 *   - Sets *error_p to UART error bits or - on timeout
 *   - On timeout, sets *error_p to -1 and returns 0
 */
char awaitkey(unsigned long delay, int* error_p)
{
    int i;

    if (delay == -1) {
        while (1) {
            if (tstc()) /* we got a key press	*/
                return getc();
        }
    }
    else {        
        for (i = 0; i < delay; i++) {
    		if (tstc()) /* we got a key press	*/
    			return getc();
            udelay (10*1000);
        }
    }
    
    if (error_p)
        *error_p = -1;
    return 0;
}

#define CTRL(x)   (x & 0x1f)
#define INTR      CTRL('C')

/*
 * Initialize the usb client port.
 *
 */
void dnw_init (void)
{
	
	serial_printf("dnw_init...");
	dnw_init_type();
	
	/* Now, set up USB controller and infrastructure */
	udc_init ();		/* Basic USB initialization */

	dnw_init_strings ();
	dnw_init_instances ();

	udc_startup_events (device_instance);/* Enable dev, init udc pointers */
	udc_connect ();		/* Enable pullup for host detection */

	dnw_init_endpoints ();

	serial_printf("done\n");

	return;
}

static void dnw_init_type(void)
{
	/* Assign endpoint descriptors */
	ep_descriptor_ptrs[0] =&dnw_configuration_descriptors[0].data_endpoints[0];
	ep_descriptor_ptrs[1] =&dnw_configuration_descriptors[0].data_endpoints[1];

	/* Configuration Descriptor */
	configuration_descriptor = (struct usb_configuration_descriptor*)&dnw_configuration_descriptors;

	/* Interface count */
	interface_count = NUM_DNW_INTERFACES;
}
static void dnw_init_strings (void)
{
	struct usb_string_descriptor *string;

	dnw_string_table[STR_LANG] = (struct usb_string_descriptor*)wstrLang;

	string = (struct usb_string_descriptor *) wstrManufacturer;
	string->bLength = sizeof(wstrManufacturer);
	string->bDescriptorType = USB_DT_STRING;
	str2wide (CONFIG_USBD_MANUFACTURER, string->wData);
	dnw_string_table[STR_MANUFACTURER]=string;


	string = (struct usb_string_descriptor *) wstrProduct;
	string->bLength = sizeof(wstrProduct);
	string->bDescriptorType = USB_DT_STRING;
	str2wide (CONFIG_USBD_PRODUCT_NAME, string->wData);
	dnw_string_table[STR_PRODUCT]=string;


	string = (struct usb_string_descriptor *) wstrSerial;
	string->bLength = sizeof(serial_number);
	string->bDescriptorType = USB_DT_STRING;
	str2wide (serial_number, string->wData);
	dnw_string_table[STR_SERIAL]=string;

	/* Now, initialize the string table for ep0 handling */
	usb_strings = dnw_string_table;
}


static void dnw_init_instances (void)
{
	int i;

	/* initialize device instance */
	memset (device_instance, 0, sizeof (struct usb_device_instance));
	device_instance->device_state = STATE_INIT;
	device_instance->device_descriptor = &device_descriptor;
	device_instance->event = dnw_event_handler;
	device_instance->cdc_recv_setup = dnw_cdc_setup;
	device_instance->bus = bus_instance;
	device_instance->configurations = NUM_CONFIGS;
	device_instance->configuration_instance_array = config_instance;

	/* initialize bus instance */
	memset (bus_instance, 0, sizeof (struct usb_bus_instance));
	bus_instance->device = device_instance;
	bus_instance->endpoint_array = endpoint_instance;
	bus_instance->max_endpoints = 1;
	bus_instance->maxpacketsize = 64;
	bus_instance->serial_number_str = serial_number;

	/* configuration instance */
	memset (config_instance, 0, sizeof (struct usb_configuration_instance));
	config_instance->interfaces = interface_count;
	config_instance->configuration_descriptor = configuration_descriptor;
	config_instance->interface_instance_array = interface_instance;

	/* interface instance */
	memset (interface_instance, 0, sizeof (struct usb_interface_instance));
	interface_instance->alternates = 1;
	interface_instance->alternates_instance_array = alternate_instance;

	/* alternates instance */
	memset (alternate_instance, 0, sizeof (struct usb_alternate_instance));
	alternate_instance->interface_descriptor = interface_descriptors;
	alternate_instance->endpoints = NUM_ENDPOINTS;
	alternate_instance->endpoints_descriptor_array = ep_descriptor_ptrs;

	/* endpoint instances */
	memset (&endpoint_instance[0], 0, sizeof (struct usb_endpoint_instance));
	endpoint_instance[0].endpoint_address = 0;
	endpoint_instance[0].rcv_packetSize = 64;//EP0_MAX_PACKET_SIZE;
	endpoint_instance[0].rcv_attributes = USB_ENDPOINT_XFER_CONTROL;
	endpoint_instance[0].tx_packetSize = 64;//EP0_MAX_PACKET_SIZE;
	endpoint_instance[0].tx_attributes = USB_ENDPOINT_XFER_CONTROL;
	udc_setup_ep (device_instance, 0, &endpoint_instance[0]);

	for (i = 1; i <= NUM_ENDPOINTS; i++) 
	{
		memset (&endpoint_instance[i], 0, sizeof (struct usb_endpoint_instance));

		endpoint_instance[i].endpoint_address =
			ep_descriptor_ptrs[i - 1]->bEndpointAddress;

		endpoint_instance[i].rcv_attributes =
			ep_descriptor_ptrs[i - 1]->bmAttributes;

		endpoint_instance[i].rcv_packetSize =
			le16_to_cpu(ep_descriptor_ptrs[i - 1]->wMaxPacketSize);

		endpoint_instance[i].tx_attributes =
			ep_descriptor_ptrs[i - 1]->bmAttributes;

		endpoint_instance[i].tx_packetSize =
			le16_to_cpu(ep_descriptor_ptrs[i - 1]->wMaxPacketSize);

		endpoint_instance[i].tx_attributes =
			ep_descriptor_ptrs[i - 1]->bmAttributes;

		urb_link_init (&endpoint_instance[i].rcv);
		urb_link_init (&endpoint_instance[i].rdy);
		urb_link_init (&endpoint_instance[i].tx);
		urb_link_init (&endpoint_instance[i].done);

		if (endpoint_instance[i].endpoint_address & USB_DIR_IN)
		{
			endpoint_instance[i].tx_urb = usbd_alloc_urb(device_instance, &endpoint_instance[i]);
		}	
		else
		{	
			endpoint_instance[i].rcv_urb = usbd_alloc_urb (device_instance, &endpoint_instance[i]);
		}
	}
}

static void dnw_event_handler (struct usb_device_instance *device,
				  usb_device_event_t event, int data)
{
	switch (event) {
	case DEVICE_RESET:
	case DEVICE_BUS_INACTIVE:
		dnw_configured_flag = 0;
		break;
	case DEVICE_CONFIGURED:
		dnw_configured_flag = 1;
		break;

	case DEVICE_ADDRESS_ASSIGNED:
		dnw_init_endpoints ();

	default:
		break;
	}
}

int dnw_cdc_setup(struct usb_device_request *request, struct urb *urb)
{
	printf("@dnw_cdc_setup(), we do nothing!\n");
	return 0;
}

static void dnw_init_endpoints (void)
{
	int i;

	bus_instance->max_endpoints = NUM_ENDPOINTS + 1;
	for (i = 1; i <= NUM_ENDPOINTS; i++) {
		udc_setup_ep (device_instance, i, &endpoint_instance[i]);
	}
}

u32 usb_receive(char *buf, size_t len, u32 wait)
{//we didn't use the 3 params transformed into here by caller
 //it will be used in the furture version.
	int first=1;
	u8 tempMem[16];
	char c;

	downloadAddress=(u32)tempMem;
	downPt=(unsigned char *)downloadAddress;/* This address is used for receiving first 8 byte. */
	
    first = 1;	
	ep2_first = 0;
	fifo_full_ok = 0;
    downloadFileSize=0;

   	if(isUsbdSetConfiguration==0)
    {
	    serial_printf("USB host is not connected yet.\n");
	}

	while(downloadFileSize==0) /* wait until send a file */
	{
		if(first==1 && isUsbdSetConfiguration!=0)
		{
	       serial_printf("USB host is connected. Waiting a download.\n");
	       first=0;	
		}

        if(fifo_full_ok == 1)
            break;

		c = awaitkey(1, 0);
		if ((c & 0x7f) == INTR)
		{
			serial_printf("Cancelled by user\n");
			return 0;
		}
    }

	while(fifo_full_ok == 1)
	{

		if((totalDmaCount+8) == downloadFileSize)
        {

			fifo_full_ok = 0;
            break;
        }

		c = awaitkey(1, 0);
		if ((c & 0x7f) == INTR)
		{
			serial_printf("Cancelled by user\n");
			return 0;
		}
	}

	serial_printf("Transfor completed.\n");
	serial_printf("  image at [%08x:%08x]\n", downloadAddress, downloadAddress + totalDmaCount - 2);
    //totalDmaCount is (n+2), include last 2 bytes of CheckSum

	return downloadFileSize - 10;
}


