/*
 * ETHER1008 ethernet driver (MAC + PHY)
 *
 * Copyright (C) 2011 Bryan Wu <cooloney@kernel.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/tcp.h>
#include <linux/skbuff.h>
#include <linux/delay.h>
#include <linux/spi/spi.h>
//--------------add by zyt--------------
#include <linux/platform_device.h>
//--------------add by zyt--------------

#include "ether1008.h"

#ifdef CONFIG_MT6573_SPI
#include <mach/mt6573_gpio.h>
#include <cust_eint.h>
#include <mach/mt6573_eint.h>

#define CUST_EINT_ETHER1008_NUM			CUST_EINT_ETHERNET_NUM
#define CUST_EINT_ETHER1008_DEBOUNCE_CN		0
#define CUST_EINT_ETHER1008_POLARITY		CUST_EINT_POLARITY_HIGH
#define CUST_EINT_ETHER1008_SENSITIVE		CUST_EINT_LEVEL_SENSITIVE
#define CUST_EINT_ETHER1008_DEBOUNCE_EN		CUST_EINT_DEBOUNCE_DISABLE

#define GPIO_ETHER1008_POWER_CONTROL		GPIO102
#endif

//--------------add by zyt--------------
#ifdef CONFIG_ARCH_SEP0611
#include <mach/regs-spi.h>
#include <board/board.h>
#endif
//--------------add by zyt--------------
#define DRV_NAME	"ether1008"
#define DRV_VERSION	"1.0"

#define ETHER1008_MSG_DEFAULT	(NETIF_MSG_PROBE | NETIF_MSG_IFUP | NETIF_MSG_IFDOWN | NETIF_MSG_LINK)

/*
 * Buffer size required for the largest SPI transfer
 * (i.e., reading a frame).
 */
#define SPI_TRANSFER_BUF_LEN	(4 + MAX_FRAMELEN)

#define TX_TIMEOUT	(4 * HZ)

enum {
	RXFILTER_NORMAL,
	RXFILTER_MULTI,
	RXFILTER_PROMISC
};

/* Driver local data */
struct ether1008_net {
	struct net_device *netdev;
//--------------changed by zyt--------------
// -	struct spi_device *spi;
	struct platform_device *pdev;
//--------------changed by zyt--------------
	struct mutex lock;
	struct sk_buff *tx_skb;
	struct work_struct tx_work;
	struct work_struct irq_work;
	struct work_struct setrx_work;
	struct work_struct restart_work;
	bool hw_enable;
	bool full_duplex;
	int rxfilter;
	u32 msg_enable;
	u8 spi_transfer_buf[SPI_TRANSFER_BUF_LEN];
//--------------add by zyt--------------
	int irq;
//--------------add by zyt--------------
};

/* use ethtool to change the level for any given device */
static struct {
	u32 msg_enable;
} debug = { -1 };

//--------------add by zyt--------------
static int netdev_mc_empty(struct net_device *dev )
{
	return 1;
}
static void __iomem *ssi2_regs;
static void ether1008_spi_setup(void)
{
	ssi2_regs = ioremap(SSI2_BASE, 0x1000);
	sep0611_gpio_cfgpin(SEP0611_GPE8, SSI2_RXD);
	sep0611_gpio_cfgpin(SEP0611_GPE9, SSI2_TXD);
	sep0611_gpio_cfgpin(SEP0611_GPE10, SSI2_CS);
	sep0611_gpio_cfgpin(SEP0611_GPE11, SSI2_CLK);

	writel(0x0, ssi2_regs + SSI_ENR);
	writel(0x07,ssi2_regs + SSI_CONTROL0);
	writel(0x01,ssi2_regs + SSI_CONTROL1);
	writel(0xa, ssi2_regs + SSI_BAUDR);
	writel(0x1, ssi2_regs + SSI_TXFTLR);
	writel(0x0, ssi2_regs + SSI_RXFTLR);
	writel(0x0,	ssi2_regs + SSI_DMACR);
	writel(0x0, ssi2_regs + SSI_IMR);
	writel(0x1, ssi2_regs + SSI_SER);
	writel(0x0, ssi2_regs + SSI_ENR);

	udelay(200);
}
static void ether1008_spi_opt_tx(u8 tx_buf)
{
//	printk("spi_tx: 0x%x\n", tx_buf);
	writeb(tx_buf, ssi2_regs + SSI_DR);

	while (!(readl(ssi2_regs + SSI_SR) & 0x4));
	while (readl(ssi2_regs + SSI_SR) & 0x1);
	
	readb(ssi2_regs + SSI_DR);
}
static u8 ether1008_spi_opt_rx(void)
{
	u8 rx_buf= 0 ;
	writeb(0, ssi2_regs + SSI_DR);
	while (!(readl(ssi2_regs + SSI_SR) & 0x4));
	while (readl(ssi2_regs + SSI_SR) & 0x1);

	rx_buf = readb(ssi2_regs + SSI_DR);

//	printk("\tspi_rx: 0x%x\n", rx_buf);
	return rx_buf;

}
static int ether1008_spi_write_then_read(struct ether1008_net *priv, const u8 *txbuf, unsigned tx_len, u8 *rxbuf, unsigned rx_len )
{
	int i;
	writel(0x1, ssi2_regs + SSI_ENR);
	for(i = 0; i < tx_len; i++)
	{
		ether1008_spi_opt_tx(txbuf[i]);
	}
	for(i = 0; i < rx_len; i++)
	{
		rxbuf[i] = ether1008_spi_opt_rx();
	}
	writel(0x0, ssi2_regs + SSI_ENR);
	return 0;
}
static int ether1008_spi_write(struct ether1008_net *priv, const u8 *buf, size_t len)
{
	int i;
	writel(0x1, ssi2_regs + SSI_ENR);
	for(i = 0; i < len; i++)
	{	
		ether1008_spi_opt_tx(buf[i]);
	}
	writel(0x0, ssi2_regs + SSI_ENR);
	return 0;
}

//--------------add by zyt--------------
	
static void dump_packet(const char *msg, int len, const char *data)
{
	printk(KERN_ERR DRV_NAME ": %s - packet len:%d\n", msg, len);
	print_hex_dump(KERN_ERR, "pk data: ", DUMP_PREFIX_OFFSET, 16, 1,\
			data, len, true);
	return;
}

/*
 * Basic SPI read/write operation
 */
static int spi_rw_op(struct ether1008_net *priv, u8 cmd, u16 addr,
		u8 *data, int len, int is_read)
{
	int ret;

	/* 2 bytes SPI command codes */
	priv->spi_transfer_buf[0] =
		 ((addr & 0xF00) >> 8) | (cmd << 4);
	priv->spi_transfer_buf[1] = addr & 0xFF;

	/* Read operation */
	if (is_read) {
		priv->spi_transfer_buf[2] = SPI_CMD_DUMMY_WAIT;
		priv->spi_transfer_buf[3] = SPI_CMD_DUMMY_WAIT;
		ret = ether1008_spi_write_then_read(priv, priv->spi_transfer_buf, 4,
						data, len);
	/* Write operation */
	} else {
		if (len > 0)
			memcpy(priv->spi_transfer_buf + 2, data, len);
		ret = ether1008_spi_write(priv, priv->spi_transfer_buf, len + 2);
	}

	if (ret && netif_msg_drv(priv))
		printk(KERN_DEBUG DRV_NAME ": %s() failed: ret = %d\n",
			__func__, ret);

	return ret;
}

/* Internal Register byte read */
static u8 spi_read_int_regb(struct ether1008_net *priv, u16 addr)
{
	u8 val = 0;
	int ret;

	ret = spi_rw_op(priv, SPI_CMD_READ_INT_REG, addr, (u8 *)&val, 1, 1);
	if (!ret)
		return val;
	else
		return 0;
}

/* Internal Register 2 bytes read */
static u16 spi_read_int_regw(struct ether1008_net *priv, u16 addr)
{
	u16 val = 0;
	int ret;

	ret = spi_rw_op(priv, SPI_CMD_READ_INT_REG, addr, (u8 *)&val, 2, 1);
	if (!ret)
		return val;
	else
		return 0;
}

/* Internal Register byte write */
static void spi_write_int_regb(struct ether1008_net *priv, u16 addr, u8 val)
{
	spi_rw_op(priv, SPI_CMD_WRITE_INT_REG, addr, (u8 *)&val, 1, 0);
}

/* Internal Register 2 bytes write */
static void spi_write_int_regw(struct ether1008_net *priv, u16 addr, u16 val)
{
	spi_rw_op(priv, SPI_CMD_WRITE_INT_REG, addr, (u8 *)&val, 2, 0);
}

/* MAC Register 4 bytes read */
static u32 spi_read_mac_reg(struct ether1008_net *priv, u16 addr)
{
	u32 val;

	spi_rw_op(priv, SPI_CMD_READ_MAC_REG, addr, (u8 *)&val, 4, 1);
	return val;
}

/* MAC Register 4 bytes write */
static void spi_write_mac_reg(struct ether1008_net *priv, u16 addr, u32 val)
{
	spi_rw_op(priv, SPI_CMD_WRITE_MAC_REG, addr, (u8 *)&val, 4, 0);
}

static void ether1008_reset_all(struct ether1008_net *priv)
{
	spi_rw_op(priv, SPI_CMD_RESET_ALL, 0xF00, NULL, 0, 0);

	udelay(100);
}

/*
 * FIFO read operation
 * @len: byte number of data, excluding the CRC16 and length in bytes
 */
static int
spi_read_fifo(struct ether1008_net *priv, u8 cmd, int len, u8 *data)
{
	u8 *tx_buf = priv->spi_transfer_buf;
	u8 *rx_buf = priv->spi_transfer_buf + 4;
	struct spi_transfer h_t, d_t;
	struct spi_message msg;
//-	int ret, len_in_word;
	int ret = 0, len_in_word;

	/* FIFO reading should be 4 bytes aligned */
	if (len % 4)
		len_in_word = len / 4 + 1;
	else
		len_in_word = len / 4;

	/* 2 bytes SPI command codes */
	tx_buf[0] = ((len_in_word & 0xF00) >> 8) | (cmd << 4);
	tx_buf[1] = len_in_word & 0xFF;
	/* 2 bytes dummy bytes */
	tx_buf[2] = tx_buf[3] = SPI_CMD_DUMMY_WAIT;

	h_t.tx_buf = tx_buf;
	h_t.rx_buf = NULL;
	h_t.len = 4;

	d_t.tx_buf = NULL;
	d_t.rx_buf = rx_buf;
	d_t.len = len_in_word * 4;

/**************comment by zyt**************
	spi_message_init(&msg);
	spi_message_add_tail(&h_t, &msg);
	spi_message_add_tail(&d_t, &msg);

	ret = spi_sync(priv->spi, &msg);
	if (ret == 0) {
		memcpy(data, rx_buf, len);
		ret = msg.status;
	}
**************comment by zyt**************/
	ether1008_spi_write_then_read(priv, tx_buf, 4, rx_buf, len_in_word * 4);
	memcpy(data, rx_buf, len);

	if (ret && netif_msg_drv(priv))
		printk(KERN_DEBUG DRV_NAME ": %s() failed: ret = %d\n",
			__func__, ret);

	return ret;
}

#define spi_read_data_rxfifo(priv, len, data) \
	spi_read_fifo(priv, SPI_CMD_READ_RX_FIFO, len, data)
#define spi_read_state_txfifo(priv, len, data) \
	spi_read_fifo(priv, SPI_CMD_READ_TX_STAT, len, data)

static u32 ether1008_packet_crc_header(int len)
{
	u32 crc = len & 0xFFFF;
	u32 shift = len & 0xFFFF;
	int i;

	for (i = 0; i < 16; i++) {
		shift = shift << 1;
		if (shift & 0x10000)
			shift ^= 0x8005;
	}

	crc |= shift << 16;
	return crc;
}

/*
 * FIFO write operation
 * @len: byte number of data, excluding the CRC16 and length in bytes
 */
static int spi_write_txfifo(struct ether1008_net *priv, int len,
			 const u8 *data)
{
	u8 *tx_buf = priv->spi_transfer_buf;
	u32 crc;
	int tx_len, len_in_word, ret;

	/* Total length in byte for SPI transfer */
	tx_len = 6 + ((len % 4) > 0 ? (len + 4 - len % 4) : len);

	/* 1 word for CRC/Length header, len / 4 word and dummy padding */
	len_in_word = 1 + len / 4 + ((len % 4) > 0 ? 1 : 0);
	/* Fill 6 bytes data of SPI header transfer */
	tx_buf[0] = ((len_in_word & 0xF00) >> 8) |
			(SPI_CMD_WRITE_TX_FIFO << 4);
	tx_buf[1] = len_in_word & 0xFF;
	crc = ether1008_packet_crc_header(len);
	memcpy(tx_buf + 2, &crc, 4);

	/* Copy data to tx_buf */
	memcpy(tx_buf + 6, data, len);

	ret = ether1008_spi_write(priv, tx_buf, tx_len);
	if (ret && netif_msg_drv(priv))
		printk(KERN_DEBUG DRV_NAME ": %s() failed: ret = %d\n",
			__func__, ret);

	return ret;
}

static unsigned long msec20_to_jiffies;

/*
 * Wait until the PHY operation is complete.
 */
static int wait_phy_ready(struct ether1008_net *priv)
{
	unsigned long timeout = jiffies + msec20_to_jiffies;

	while (spi_read_mac_reg(priv, EMAC_GMII_ADDR) & EMAC_GMII_ADDR_GB) {
	/* 20 msec timeout read */
		if (time_after(jiffies, timeout)) {
			if (netif_msg_drv(priv))
				dev_dbg(&priv->pdev->dev,
					"PHY ready timeout!\n");
			return -ETIMEDOUT;
		}
		cpu_relax();
	}
	return 0;
}

//#define PHY_PA			0x7
//#define PHY_CR			0x0

#define PHY_PA			0x10
#define PHY_CR			0x2
/*
 * PHY register read
 */
static u16 ether1008_phy_read(struct ether1008_net *priv, u8 address)
{
	u32 command;
	u32 ret;

	mutex_lock(&priv->lock);

	command = EMAC_GMII_ADDR_GB | (PHY_CR << 2) |
		(address << 6) | (PHY_PA << 11);

	spi_write_mac_reg(priv, EMAC_GMII_ADDR, command);
	wait_phy_ready(priv);
	ret = spi_read_mac_reg(priv, EMAC_GMII_DATA);

	mutex_unlock(&priv->lock);

	return (ret & EMAC_GMII_DATA_GD);
}

static int ether1008_phy_write(struct ether1008_net *priv, u8 address, u16 data)
{
	int ret;
	u32 command;

	mutex_lock(&priv->lock);

	command = EMAC_GMII_ADDR_GB | EMAC_GMII_ADDR_GW |
		(PHY_CR << 2) | (address << 6) | (PHY_PA << 11);

	spi_write_mac_reg(priv, EMAC_GMII_ADDR, command);
	wait_phy_ready(priv);
	spi_write_mac_reg(priv, EMAC_GMII_DATA, data);

	mutex_unlock(&priv->lock);

	return ret;
}

/*
 * Program the hardware MAC address from dev->dev_addr.
 */
static int ether1008_set_hw_macaddr(struct net_device *ndev)
{
	int ret;
	u32 data;
	struct ether1008_net *priv = netdev_priv(ndev);

	mutex_lock(&priv->lock);
	if (!priv->hw_enable) {
		if (netif_msg_drv(priv))
			printk(KERN_INFO DRV_NAME
				": %s: Setting MAC address to %pM\n",
				ndev->name, ndev->dev_addr);
		data = ndev->dev_addr[5] << 8 | ndev->dev_addr[4];
		spi_write_mac_reg(priv, EMAC_MAC_ADDR0_HIGH, data);
		data = *(u32 *)ndev->dev_addr;
		spi_write_mac_reg(priv, EMAC_MAC_ADDR0_LOW, data);
		ret = 0;
	} else {
		if (netif_msg_drv(priv))
			printk(KERN_DEBUG DRV_NAME
				": %s() Hardware must be disabled to set "
				"Mac address\n", __func__);
		ret = -EBUSY;
	}
	mutex_unlock(&priv->lock);
	return ret;
}

/*
 * Store the new hardware address in dev->dev_addr, and update the MAC.
 */
static int ether1008_set_mac_address(struct net_device *dev, void *addr)
{
	struct sockaddr *address = addr;

	if (netif_running(dev))
		return -EBUSY;
	if (!is_valid_ether_addr(address->sa_data))
		return -EADDRNOTAVAIL;

	memcpy(dev->dev_addr, address->sa_data, dev->addr_len);
	return ether1008_set_hw_macaddr(dev);
}

/*
 * Check chip ID
 */
static int ether1008_check_chip_id(struct ether1008_net *priv)
{
	u8 id[7];
	u16 id16;
	u32 id32;

	/* Check major chip ID  (== "NL1110") */
	spi_rw_op(priv, SPI_CMD_READ_INT_REG, EI_CHIP_ID, id, 6, 1);
	id[6] = '\0';
	if (!strcmp(id, "NL1110")) {
		if (netif_msg_drv(priv))
			printk(KERN_INFO DRV_NAME ": CHIP major ID: %s\n", id);
	} else {
		if (netif_msg_drv(priv))
			printk(KERN_ERR DRV_NAME ": %s() Invalid CHIP major ID %s\n",
				__func__, id);
		return 0;
	}

	/* Check minor chip ID (== 0x0100) */
	spi_rw_op(priv, SPI_CMD_READ_INT_REG, EI_CHIP_ID + 6, (u8 *)&id16, 2, 1);
	if (netif_msg_drv(priv))
		printk(KERN_INFO DRV_NAME ": CHIP minor ID: 0x%04x\n", id16);

	/* Check MAC ID (== 0x1037) */
	id32 = spi_read_mac_reg(priv, EMAC_VERSION);
	if (netif_msg_drv(priv))
		printk(KERN_INFO DRV_NAME ": MAC ID: 0x%08x\n", id32);

	/* Check PHY ID */
	id16 = ether1008_phy_read(priv, EPHY_ID1);
	if (netif_msg_drv(priv))
		printk(KERN_INFO DRV_NAME ": PHY ID1: 0x%08x\n", id16);

	id16 = ether1008_phy_read(priv, EPHY_ID2);
	if (netif_msg_drv(priv))
		printk(KERN_INFO DRV_NAME ": PHY ID2: 0x%08x\n", id16);

	return 1;
}

/* Debug routine to dump useful register contents */
static void ether1008_dump_regs(struct ether1008_net *priv, const char *msg)
{
        mutex_lock(&priv->lock);
        printk(KERN_INFO DRV_NAME " %s\n"
                "Int:    Control0       Control1        SPI_Ctrl\n"
                "        0x%02x         0x%02x          0x%02x\n"
		"        RX_Flen_TH     RX_FIFO_TH      TX_FIFO_TH      TX_STAT_TH\n"
		"        0x%04x         0x%04x          0x%04x          0x%4x\n"
		"        Intr_mask      Intr_state\n"
		"        0x%04x         0x%04x\n"
		"        RX_FLEN_CNT    RX_FIFO_FLVL    TX_FIFO_FLVL    TX_STAT_FLVL\n"
                "        0x%02x         0x%04x          0x%04x          0x%04x\n"
		"MAC:    MAC_Config     Frame_filter    Flow_Control    Op_Mode\n"
		"        0x%08x         0x%08x          0x%08x          0x%8x\n"
		"        MAC_Addr0_High MAC_Addr0_Low\n"
		"        0x%08x         0x%08x\n",
		msg,
		spi_read_int_regb(priv, EI_CTRL0), spi_read_int_regb(priv, EI_CTRL1), spi_read_int_regb(priv, EI_SPI_CTRL),

		spi_read_int_regw(priv, EI_RX_FLEN_FIFO_TH), spi_read_int_regw(priv, EI_RX_DATA_FIFO_TH),
		spi_read_int_regw(priv, EI_TX_DATA_FIFO_TH), spi_read_int_regw(priv, EI_TX_STAT_FIFO_TH),

		spi_read_int_regw(priv, EI_INTR_MASK), spi_read_int_regw(priv, EI_INTR_STAT),

        spi_read_int_regb(priv, EI_RX_FLEN_FIFO_CNT), spi_read_int_regw(priv, EI_RX_DATA_FIFO_FLVL),
        spi_read_int_regw(priv, EI_TX_DATA_FIFO_FLVL), spi_read_int_regw(priv, EI_TX_STAT_FIFO_FLVL),

		spi_read_mac_reg(priv, EMAC_CONFIG), spi_read_mac_reg(priv, EMAC_FRAME_FILTER),
		spi_read_mac_reg(priv, EMAC_FLOW_CONTROL), spi_read_mac_reg(priv, EMAC_OP_MODE),
		spi_read_mac_reg(priv, EMAC_MAC_ADDR0_HIGH), spi_read_mac_reg(priv, EMAC_MAC_ADDR0_LOW)
	);
	mutex_unlock(&priv->lock);
}

static void ether1008_power_on(struct ether1008_net *priv)
{
	u8 phy_reg;

	/* Turn on the LDO power supply */
#ifdef GPIO_ETHER1008_POWER_CONTROL
	mt_set_gpio_mode(GPIO_ETHER1008_POWER_CONTROL, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_ETHER1008_POWER_CONTROL, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_ETHER1008_POWER_CONTROL, GPIO_OUT_ONE);
#endif

	/* Turn on PHY */
	phy_reg = spi_read_int_regb(priv, EI_PHY_CTRL);
	phy_reg |= EI_PHY_CTRL_PWRDN;
	spi_write_int_regb(priv, EI_PHY_CTRL, phy_reg);
}

static void ether1008_power_off(struct ether1008_net *priv)
{
	u8 phy_reg;

	/* Turn off PHY */
	phy_reg = spi_read_int_regb(priv, EI_PHY_CTRL);
	phy_reg &= ~EI_PHY_CTRL_PWRDN;
	spi_write_int_regb(priv, EI_PHY_CTRL, phy_reg);

	/* Turn off the LDO power supply */
#ifdef GPIO_ETHER1008_POWER_CONTROL
	mt_set_gpio_mode(GPIO_ETHER1008_POWER_CONTROL, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_ETHER1008_POWER_CONTROL, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_ETHER1008_POWER_CONTROL, GPIO_OUT_ZERO);
#endif
}

//--------------add by zyt--------------
static void ether1008_led_init(struct ether1008_net *priv)
{
	spi_write_int_regw(priv,EI_PHY_LED, 0x8F);
}
//--------------add by zyt--------------
static int ether1008_hw_init(struct ether1008_net *priv)
{
	int ret;
	u32 mac_reg;

	if (netif_msg_drv(priv))
		printk(KERN_DEBUG DRV_NAME ": %s() - %s\n", __func__,
			priv->full_duplex ? "FullDuplex" : "HalfDuplex");

	mutex_lock(&priv->lock);
	/* first reset the chip */
	ether1008_reset_all(priv);
	priv->hw_enable = false;
	priv->rxfilter = RXFILTER_NORMAL;
	mutex_unlock(&priv->lock);
	ether1008_led_init(priv);
	/*
	 * Check the RevID.
	 */
	ret = ether1008_check_chip_id(priv);
	if (!ret)
		return 0;

	/* Frame filter MAC setting = 0x450 */
	mac_reg = EMAC_FF_PM | 0x1 << EMAC_FF_PCF_SHIFT | EMAC_FF_HPF;
	spi_write_mac_reg(priv, EMAC_FRAME_FILTER, mac_reg);

	/* Operation mode (= 0x6201D40)*/
	mac_reg = EMAC_OP_FUF | EMAC_OP_EFC | 0x2 << EMAC_OP_RFA_SHIFT |
			0x3 << EMAC_OP_RFD_SHIFT | EMAC_OP_TSF | EMAC_OP_RSF |
			EMAC_OP_DT;
	spi_write_mac_reg(priv, EMAC_OP_MODE, mac_reg); 

	/* Flow control setting (= 0xFFFF000E) */
	mac_reg = EMAC_FC_TFE | EMAC_FC_RFE | EMAC_FC_UP | EMAC_FC_PT;
	spi_write_mac_reg(priv, EMAC_FLOW_CONTROL, mac_reg);

	/* FIFO Threshold */
	spi_write_int_regw(priv, EI_RX_FLEN_FIFO_TH, 1);
	spi_write_int_regw(priv, EI_RX_DATA_FIFO_TH, 512);
	spi_write_int_regw(priv, EI_TX_DATA_FIFO_TH, 256);
	spi_write_int_regw(priv, EI_TX_STAT_FIFO_TH, 1);

	/* RX FIFO timeout value (= 1ms) */
	spi_write_int_regw(priv, EI_RX_DATA_FIFO_TO, 100);

	/* MAC config (= 0x810e880) */
	mac_reg = EMAC_CONFIG_ACS | EMAC_CONFIG_DM | EMAC_CONFIG_DO |
		EMAC_CONFIG_JE | EMAC_CONFIG_2KPE;
	spi_write_mac_reg(priv, EMAC_CONFIG, mac_reg);

	return 1;
}

static void ether1008_hw_enable(struct ether1008_net *priv)
{
	u8 regb;
	u32 mac_reg;
	u16 intr;

	mutex_lock(&priv->lock);

	/* Enable interrupt mask */
	spi_write_int_regw(priv, EI_INTR_STAT, 0xFFFF);
	intr = ~INTR_MASK;
	spi_write_int_regw(priv, EI_INTR_MASK, intr);

	/* Enable interrupt output */
	regb = spi_read_int_regb(priv, EI_SPI_CTRL);
	regb |= EI_SPI_CTRL_INTR_OE;
	spi_write_int_regb(priv, EI_SPI_CTRL, regb);

	/* Enable RX/TX logic */
	mac_reg = spi_read_mac_reg(priv, EMAC_CONFIG);
	mac_reg |= EMAC_CONFIG_RE | EMAC_CONFIG_TE;
	spi_write_mac_reg(priv, EMAC_CONFIG, mac_reg);

	priv->hw_enable = true;

	mutex_unlock(&priv->lock);
	
	if (netif_msg_hw(priv))
		ether1008_dump_regs(priv, "Hw Enable: ");
}

static void ether1008_hw_disable(struct ether1008_net *priv)
{
	u8 regb;
	u32 mac_reg;

	mutex_lock(&priv->lock);

	/* Disable RX/TX logic */
	mac_reg = spi_read_mac_reg(priv, EMAC_CONFIG);
	mac_reg &= ~(EMAC_CONFIG_RE | EMAC_CONFIG_TE);
	spi_write_mac_reg(priv, EMAC_CONFIG, mac_reg);

	/* Disable interrupt mask */
	spi_write_int_regw(priv, EI_INTR_STAT, 0xFFFF);
	spi_write_int_regw(priv, EI_INTR_MASK, 0x0);

	/* Disable interrupt output */
	regb = spi_read_int_regb(priv, EI_SPI_CTRL);
	regb &= ~EI_SPI_CTRL_INTR_OE;
	spi_write_int_regb(priv, EI_SPI_CTRL, regb);

	priv->hw_enable = false;

	mutex_unlock(&priv->lock);
}

/* TX status */
static void ether1008_tx_status(struct net_device *ndev)
{
    struct ether1008_net *priv = netdev_priv(ndev);
	u32 status;

	/* Read 4 bytes status out from TX status FIFO */
	spi_read_state_txfifo(priv, 4, (u8 *)&status);
	dev_dbg(&ndev->dev, "TX status is 0x%08x\n", status);
	//printk("TX status is 0x%08x\n", status);
	if (status)
			ndev->stats.tx_errors++;
	else
			ndev->stats.tx_packets++;

	if (priv->tx_skb) {
			if (!status)
					ndev->stats.tx_bytes += priv->tx_skb->len;
			dev_kfree_skb(priv->tx_skb);
			priv->tx_skb = NULL;
	}

	netif_wake_queue(ndev);
}

/*
 * Access the PHY to determine link status
 */
static void ether1008_check_link_status(struct net_device *ndev)
{
	struct ether1008_net *priv = netdev_priv(ndev);
	u16 reg;

	reg = ether1008_phy_read(priv, EPHY_BMSR);
	if (reg & EPHY_BMSR_LINK_STATUS) {
		netif_carrier_on(ndev);
		if (netif_msg_ifup(priv))
			dev_info(&ndev->dev, "link up\n");
	} else {
		if (netif_msg_ifdown(priv))
			dev_info(&ndev->dev, "link down\n");
		netif_carrier_off(ndev);
	}
}

/* RX handler */
static int ether1008_rx_interrupt(struct net_device *ndev)
{
	struct ether1008_net *priv = netdev_priv(ndev);
	struct sk_buff *skb = NULL;
	int ret, len;
	u32 head, status;
	//u32 length1, length2, length3;	
	//length1 = spi_read_int_regb(priv, 0x52);
	//length2 = spi_read_int_regb(priv, 0x53);
	//length3 = spi_read_int_regb(priv, 0x54);
	//printk("length1 is 0x%x\n", length1);
	//printk("length2 is 0x%x\n", length2);
	//printk("length3 is 0x%x\n", length3);
	/* Read out first 4 bytes to get length of the packet */
	spi_read_data_rxfifo(priv, 4, (u8 *)&head);

	//printk("head is 0x%x\n", head);
	/* [11:0] of head is the real packet length */
	len = head & 0xFFF;
	if (head != ether1008_packet_crc_header(head)) {
		dev_err(&ndev->dev, "CRC Error: head 0x%08x, crc 0x%08x!\n", head, ether1008_packet_crc_header(len));
		return -EIO;
	}

	skb = dev_alloc_skb(len + NET_IP_ALIGN);
	if (!skb) {
		if (netif_msg_rx_err(priv))
			dev_err(&ndev->dev,
				"out of memory for Rx'd frame\n");
		ndev->stats.rx_dropped++;
	} else {
		skb->dev = ndev;
		skb_reserve(skb, NET_IP_ALIGN);
		
		/* copy the packet and status from the receive buffer */
		spi_read_data_rxfifo(priv, len, skb_put(skb, len));

		/* Read out RX packet status */
		spi_read_data_rxfifo(priv, 4, (u8 *)&status);
		if (status & 0x80) {
			dev_err(&ndev->dev, "status 0x%08x\n", status);
			ndev->stats.rx_dropped++;
			return -EIO;
		}

		if (netif_msg_pktdata(priv))
			dump_packet(__func__, skb->len, skb->data);
		skb->protocol = eth_type_trans(skb, ndev);
		/* update statistics */
		ndev->stats.rx_packets++;
		ndev->stats.rx_bytes += len;
		//printk("Got packet done\n");
		//dump_packet(__func__, skb->len, skb->data);
		netif_rx_ni(skb);
	}

	return ret;
}

static void ether1008_irq_work_handler(struct work_struct *work)
{
	struct ether1008_net *priv =
		container_of(work, struct ether1008_net, irq_work);
	struct net_device *ndev = priv->netdev;
	int intr_stat, mac_intr;
	//printk("%s, %d\n", __func__, __LINE__);

	intr_stat = spi_read_int_regw(priv, EI_INTR_STAT);
	mac_intr = spi_read_mac_reg(priv, EMAC_INTR_STAT);
	while(intr_stat)
	{
			//printk("intr status 0x%x\n",intr_stat);
			//printk("mac  status 0x%x\n",mac_intr);
			/* PHY link status change */
			if (intr_stat & EI_INTR_PHY_LINK) {
				dev_dbg(&ndev->dev, "PHY link status change\n");
				//printk("PHY link status change\n");
				ether1008_check_link_status(ndev);
			}

			if (intr_stat & EI_INTR_RX_FLEN_FIFO_TH) {
				dev_dbg(&ndev->dev, "Got a packet!\n");
				//printk("Got a packet!\n");
				ether1008_rx_interrupt(ndev);		
			}

			if (intr_stat & EI_INTR_TX_STAT_FIFO_TH) {
				dev_dbg(&ndev->dev, "Got a TX status!\n");
				//printk("Got a TX status!\n");
				ether1008_tx_status(ndev);
			}
			/* Clear Interrupt State */
			spi_write_int_regw(priv, EI_INTR_STAT, intr_stat);
			spi_write_mac_reg(priv, EMAC_INTR_STAT, mac_intr);
			udelay(10);

			intr_stat = spi_read_int_regw(priv, EI_INTR_STAT);
			mac_intr = spi_read_mac_reg(priv, EMAC_INTR_STAT);
	}

#ifdef CONFIG_MT6573_SPI
	mt65xx_eint_unmask(CUST_EINT_ETHER1008_NUM);
#endif
#ifdef CONFIG_ARCH_SEP0611
	sep0611_gpio_clrirq(SEP0611_ETHER1008_INT);
	//sep0611_gpio_cfgpin(SEP0611_ETHER1008_INT, SEP0611_GPIO_IO);
	*(volatile unsigned long*)0xf400611c |= 0x1<<5;
#endif 
}

/*
 * Hardware transmit function.
 * Fill the buffer memory and send the contents of the transmit buffer
 * onto the network
 */
static void ether1008_hw_tx(struct ether1008_net *priv)
{
	int len = priv->tx_skb->len;

	if (len > SPI_TRANSFER_BUF_LEN - 1 || len <= 0)
		return;
	
	if (netif_msg_pktdata(priv))
		dump_packet(__func__,	\
			    priv->tx_skb->len, priv->tx_skb->data);

	//dump_packet(__func__,	\
			priv->tx_skb->len, priv->tx_skb->data);
	mutex_lock(&priv->lock);
	spi_write_txfifo(priv, len, priv->tx_skb->data);
	mutex_unlock(&priv->lock);
}

static netdev_tx_t ether1008_send_packet(struct sk_buff *skb,
					struct net_device *dev)
{
	struct ether1008_net *priv = netdev_priv(dev);

	if (netif_msg_tx_queued(priv))
		printk(KERN_DEBUG DRV_NAME ": %s() enter\n", __func__);

	/* If some error occurs while trying to transmit this
	 * packet, you should return '1' from this function.
	 * In such a case you _may not_ do anything to the
	 * SKB, it is still owned by the network queueing
	 * layer when an error is returned.  This means you
	 * may not modify any SKB fields, you may not free
	 * the SKB, etc.
	 */
	netif_stop_queue(dev);

	/* Remember the skb for deferred processing */
	priv->tx_skb = skb;
	schedule_work(&priv->tx_work);

	return NETDEV_TX_OK;
}

static void ether1008_tx_work_handler(struct work_struct *work)
{
	struct ether1008_net *priv =
		container_of(work, struct ether1008_net, tx_work);

	/* actual delivery of data */
	ether1008_hw_tx(priv);
}

#ifdef CONFIG_MT6573_SPI
static struct ether1008_net *irq_priv;

static irqreturn_t ether1008_irq(void)
{
	struct ether1008_net *priv = irq_priv;
#else
#ifdef CONFIG_ARCH_SEP0611
static struct ether1008_net *irq_priv;

static irqreturn_t ether1008_irq(void)
{
	struct ether1008_net *priv = irq_priv;
#else
static irqreturn_t ether1008_irq(int irq, void *dev_id)
{
	struct ether1008_net *priv = dev_id;
#endif
#endif

	/*
	 * Can't do anything in interrupt context because we need to
	 * block (spi_sync() is blocking) so fire of the interrupt
	 * handling workqueue.
	 * Remember that we access ether1008 registers through SPI bus
	 * via spi_sync() call.
	 */
	*(unsigned volatile long *)0xf400611c &= ~(0x1<<5);
	schedule_work(&priv->irq_work);

	return IRQ_HANDLED;
}

#ifdef CONFIG_MT6573_SPI
static void __init init_ether1008_irq(struct ether1008_net *priv)
{
	/* Store priv struct locally */
	irq_priv = priv;

	mt_set_gpio_mode(GPIO_ETHERNET_EINT_PIN, GPIO_MODE_02);
	mt_set_gpio_dir(GPIO_ETHERNET_EINT_PIN, GPIO_DIR_IN);
	mt_set_gpio_pull_enable(GPIO_ETHERNET_EINT_PIN, GPIO_PULL_ENABLE);
	mt_set_gpio_pull_select(GPIO_ETHERNET_EINT_PIN, GPIO_PULL_DOWN);

	mt65xx_eint_set_sens(CUST_EINT_ETHER1008_NUM, CUST_EINT_ETHER1008_SENSITIVE);
	mt65xx_eint_set_hw_debounce(CUST_EINT_ETHER1008_NUM, CUST_EINT_ETHER1008_DEBOUNCE_CN);
	mt65xx_eint_registration(CUST_EINT_ETHER1008_NUM, CUST_EINT_ETHER1008_DEBOUNCE_EN, CUST_EINT_ETHER1008_POLARITY, ether1008_irq, 0); 
	mt65xx_eint_unmask(CUST_EINT_ETHER1008_NUM);
}
#endif
#ifdef CONFIG_ARCH_SEP0611
static void __init init_ether1008_irq(struct ether1008_net *priv)
{
	irq_priv = priv;
	sep0611_gpio_cfgpin(SEP0611_ETHER1008_INT, SEP0611_GPIO_IO);
	sep0611_gpio_dirpin(SEP0611_ETHER1008_INT, SEP0611_GPIO_IN);
	sep0611_gpio_setirq(SEP0611_ETHER1008_INT, HIGH_TRIG);
	enable_irq(SEP0611_ETHER1008_INTSRC);
}
#endif

static void ether1008_tx_timeout(struct net_device *ndev)
{
	struct ether1008_net *priv = netdev_priv(ndev);

	if (netif_msg_timer(priv))
		dev_err(&ndev->dev, DRV_NAME " tx timeout\n");

	//printk("%s %d\n", __func__, __LINE__);
	ndev->stats.tx_errors++;
	/* can't restart safely under softirq */
	schedule_work(&priv->restart_work);
}

/*
 * Open/initialize the board. This is called (in the current kernel)
 * sometime after booting when the 'ifconfig' program is run.
 *
 * This routine should set everything up anew at each open, even
 * registers that "should" only need to be set once at boot, so that
 * there is non-reboot way to recover if something goes wrong.
 */
static int ether1008_net_open(struct net_device *dev)
{
	struct ether1008_net *priv = netdev_priv(dev);

	if (netif_msg_drv(priv))
		printk(KERN_DEBUG DRV_NAME ": %s() enter\n", __func__);

	if (!is_valid_ether_addr(dev->dev_addr)) {
		if (netif_msg_ifup(priv))
			dev_err(&dev->dev, "invalid MAC address %pM\n",
				dev->dev_addr);
		return -EADDRNOTAVAIL;
	}

	/* Reset the hardware here */
	ether1008_hw_disable(priv);

	ether1008_power_on(priv);

	if (!ether1008_hw_init(priv)) {
		if (netif_msg_ifup(priv))
			dev_err(&dev->dev, "hw_reset() failed\n");
		return -EINVAL;
	}

	/* Update the MAC address (in case user has changed it) */
	ether1008_set_hw_macaddr(dev);
	/* Enable interrupts */
	ether1008_hw_enable(priv);
	/* check link status */
	ether1008_check_link_status(dev);
	/* We are now ready to accept transmit requests from
	 * the queueing layer of the networking.
	 */
	netif_start_queue(dev);

	return 0;
}

/* The inverse routine to net_open(). */
static int ether1008_net_close(struct net_device *dev)
{
	struct ether1008_net *priv = netdev_priv(dev);

	if (netif_msg_drv(priv))
		printk(KERN_DEBUG DRV_NAME ": %s() enter\n", __func__);

	//printk(KERN_ERR DRV_NAME ": %s() enter\n", __func__);
	ether1008_hw_disable(priv);
	ether1008_power_off(priv);
	netif_stop_queue(dev);

	return 0;
}

/*
 * Set or clear the multicast filter for this adapter
 * num_addrs == -1	Promiscuous mode, receive all packets
 * num_addrs == 0	Normal mode, filter out multicast packets
 * num_addrs > 0	Multicast mode, receive normal and MC packets
 */
static void ether1008_set_multicast_list(struct net_device *dev)
{
	struct ether1008_net *priv = netdev_priv(dev);
	int oldfilter = priv->rxfilter;

	if (dev->flags & IFF_PROMISC) {
		if (netif_msg_link(priv))
			dev_info(&dev->dev, "promiscuous mode\n");
		priv->rxfilter = RXFILTER_PROMISC;
	} else if ((dev->flags & IFF_ALLMULTI) || !netdev_mc_empty(dev)) {
		if (netif_msg_link(priv))
			dev_info(&dev->dev, "%smulticast mode\n",
				(dev->flags & IFF_ALLMULTI) ? "all-" : "");
		priv->rxfilter = RXFILTER_MULTI;
	} else {
		if (netif_msg_link(priv))
			dev_info(&dev->dev, "normal mode\n");
		priv->rxfilter = RXFILTER_NORMAL;
	}

	if (oldfilter != priv->rxfilter)
		schedule_work(&priv->setrx_work);
}

static void ether1008_setrx_work_handler(struct work_struct *work)
{
	struct ether1008_net *priv =
		container_of(work, struct ether1008_net, setrx_work);

	if (priv->rxfilter == RXFILTER_PROMISC) {
		if (netif_msg_drv(priv))
			printk(KERN_DEBUG DRV_NAME ": promiscuous mode\n");
	} else if (priv->rxfilter == RXFILTER_MULTI) {
		if (netif_msg_drv(priv))
			printk(KERN_DEBUG DRV_NAME ": multicast mode\n");
	} else {
		if (netif_msg_drv(priv))
			printk(KERN_DEBUG DRV_NAME ": normal mode\n");
	}
}

static void ether1008_restart_work_handler(struct work_struct *work)
{
	struct ether1008_net *priv =
			container_of(work, struct ether1008_net, restart_work);
	struct net_device *ndev = priv->netdev;
	int ret;

	rtnl_lock();
	if (netif_running(ndev)) {
		ether1008_net_close(ndev);
		ret = ether1008_net_open(ndev);
		if (unlikely(ret)) {
			dev_info(&ndev->dev, " could not restart %d\n", ret);
			dev_close(ndev);
		}
	}
	rtnl_unlock();
}

/* ......................... ETHTOOL SUPPORT ........................... */

static void
ether1008_get_drvinfo(struct net_device *dev, struct ethtool_drvinfo *info)
{
	strlcpy(info->driver, DRV_NAME, sizeof(info->driver));
	strlcpy(info->version, DRV_VERSION, sizeof(info->version));
	strlcpy(info->bus_info,
		dev_name(dev->dev.parent), sizeof(info->bus_info));
}

static int
ether1008_get_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	struct ether1008_net *priv = netdev_priv(dev);

	cmd->transceiver = XCVR_INTERNAL;
	cmd->supported	= SUPPORTED_10baseT_Half | SUPPORTED_10baseT_Full
			| SUPPORTED_100baseT_Half | SUPPORTED_100baseT_Full
			| SUPPORTED_TP;
	cmd->speed	= SPEED_100;
	cmd->duplex	= priv->full_duplex ? DUPLEX_FULL : DUPLEX_HALF;
	cmd->port	= PORT_TP;
	cmd->autoneg	= AUTONEG_ENABLE;

	return 0;
}

static int
ether1008_set_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	return -EINVAL;
}

static u32 ether1008_get_msglevel(struct net_device *dev)
{
	struct ether1008_net *priv = netdev_priv(dev);
	return priv->msg_enable;
}

static void ether1008_set_msglevel(struct net_device *dev, u32 val)
{
	struct ether1008_net *priv = netdev_priv(dev);
	priv->msg_enable = val;
}

static const struct ethtool_ops ether1008_ethtool_ops = {
	.get_settings	= ether1008_get_settings,
	.set_settings	= ether1008_set_settings,
	.get_drvinfo	= ether1008_get_drvinfo,
	.get_msglevel	= ether1008_get_msglevel,
	.set_msglevel	= ether1008_set_msglevel,
};

static const struct net_device_ops ether1008_netdev_ops = {
	.ndo_open		= ether1008_net_open,
	.ndo_stop		= ether1008_net_close,
	.ndo_start_xmit		= ether1008_send_packet,
	.ndo_set_multicast_list = ether1008_set_multicast_list,
	.ndo_set_mac_address	= ether1008_set_mac_address,
	.ndo_tx_timeout		= ether1008_tx_timeout,
	.ndo_change_mtu		= eth_change_mtu,
	.ndo_validate_addr	= eth_validate_addr,
};

static int __devinit ether1008_probe(struct platform_device *pdev)
{
	struct net_device *dev;
	struct ether1008_net *priv;
	int ret = 0;

	if (netif_msg_drv(&debug))
		dev_info(&pdev->dev, DRV_NAME " Ethernet driver %s loaded\n",
			DRV_VERSION);

	dev = alloc_etherdev(sizeof(struct ether1008_net));
	if (!dev) {
		if (netif_msg_drv(&debug))
			dev_err(&pdev->dev, DRV_NAME
				": unable to alloc new ethernet\n");
		ret = -ENOMEM;
		goto error_alloc;
	}

	priv = netdev_priv(dev);

	priv->irq =	platform_get_irq(pdev, 0);
	if(priv->irq < 0){
		dev_err(&pdev->dev, DRV_NAME
				": unable to get irq resources\n");
	}

	priv->netdev = dev;	/* priv to netdev reference */
	priv->pdev = pdev;	/* priv to platform reference */
	priv->msg_enable = netif_msg_init(debug.msg_enable,
						ETHER1008_MSG_DEFAULT);
	priv->full_duplex = 1;

	mutex_init(&priv->lock);
	INIT_WORK(&priv->tx_work, ether1008_tx_work_handler);
	INIT_WORK(&priv->setrx_work, ether1008_setrx_work_handler);
	INIT_WORK(&priv->irq_work, ether1008_irq_work_handler);
	INIT_WORK(&priv->restart_work, ether1008_restart_work_handler);
	dev_set_drvdata(&pdev->dev, priv);	/* platform to priv reference */
	SET_NETDEV_DEV(dev, &pdev->dev);

	ether1008_spi_setup();
	/* Power on chip for hardware init */
	ether1008_power_on(priv);

	ret = ether1008_hw_init(priv);
	if (!ret) {
		if (netif_msg_probe(priv))
			dev_info(&pdev->dev, DRV_NAME " chip not found\n");
		ret = -EIO;
		goto error_irq;
	}
	random_ether_addr(dev->dev_addr);
	ether1008_set_hw_macaddr(dev);

#ifdef CONFIG_MT6573_SPI
	init_ether1008_irq(priv);
#else
#ifdef CONFIG_ARCH_SEP0611
	init_ether1008_irq(priv);
#endif
	/* Board setup must set the relevant edge trigger type;
	 * level triggers won't currently work.
	 */
	ret = request_irq(priv->irq, ether1008_irq, 0, DRV_NAME, priv);
	if (ret < 0) {
		if (netif_msg_probe(priv))
			dev_err(&pdev->dev, DRV_NAME ": request irq %d failed "
				"(ret = %d)\n", priv->irq, ret);
		goto error_irq;
	}
#endif

	dev->if_port = IF_PORT_10BASET;
	dev->irq = priv->irq;
	dev->netdev_ops = &ether1008_netdev_ops;
	dev->watchdog_timeo = TX_TIMEOUT;
	SET_ETHTOOL_OPS(dev, &ether1008_ethtool_ops);

	ret = register_netdev(dev);
	if (ret) {
		if (netif_msg_probe(priv))
			dev_err(&pdev->dev, "register netdev " DRV_NAME
				" failed (ret = %d)\n", ret);
		goto error_register;
	}
	dev_info(&dev->dev, DRV_NAME " driver registered\n");

	/* Power off chip for power saving */
	ether1008_power_off(priv);

	return 0;

error_register:
	free_irq(priv->irq, priv);
error_irq:
	ether1008_power_off(priv);
	free_netdev(dev);
error_alloc:
	return ret;
}

static int __devexit ether1008_remove(struct platform_device *pdev)
{
	struct ether1008_net *priv = dev_get_drvdata(&pdev->dev);

	if (netif_msg_drv(priv))
		printk(KERN_DEBUG DRV_NAME ": remove\n");

	ether1008_power_off(priv);
	unregister_netdev(priv->netdev);
	free_irq(priv->irq, priv);
	free_netdev(priv->netdev);

	return 0;
}

#ifdef CONFIG_PM
static int ether1008_suspend(struct platform_device *pdev, pm_message_t mesg)
{
    struct ether1008_net *priv = dev_get_drvdata(&pdev->dev);
    struct net_device *dev = priv->netdev;

    if (netif_running(dev)) {
        ether1008_hw_disable(priv);
        ether1008_power_off(priv);
        netif_device_detach(dev);
    }

    return 0;   
}

static int ether1008_resume(struct platform_device *pdev)
{
	struct ether1008_net *priv = dev_get_drvdata(&pdev->dev);
    struct net_device *dev = priv->netdev;

    if (netif_running(dev)) {
        ether1008_power_on(priv);
        ether1008_hw_init(priv);
        ether1008_set_hw_macaddr(dev);
        ether1008_hw_enable(priv);
        ether1008_check_link_status(dev);
        netif_device_attach(dev);
    }

    return 0;
}
#else
#define ether1008_suspend NULL
#define ether1008_resume NULL
#endif

static struct platform_driver ether1008_driver = {
	.driver = {
		   .name = "ether1008",
		   .owner = THIS_MODULE,
	 },
	.probe = ether1008_probe,
	.remove = __devexit_p(ether1008_remove),
    .suspend = ether1008_suspend,
    .resume = ether1008_resume,
};

static int __init ether1008_init(void)
{
	msec20_to_jiffies = msecs_to_jiffies(20);

	return platform_driver_register(&ether1008_driver);
}

module_init(ether1008_init);

static void __exit ether1008_exit(void)
{
	platform_driver_unregister(&ether1008_driver);
}

module_exit(ether1008_exit);

MODULE_DESCRIPTION(DRV_NAME "ether1008 ethernet driver");
MODULE_AUTHOR("Bryan Wu <cooloney@kernel.org>; John Zhang <xtayyt@gmail.com>");
MODULE_LICENSE("GPL");
module_param_named(debug, debug.msg_enable, int, 0);
MODULE_PARM_DESC(debug, "Debug verbosity level (0=none, ..., ffff=all)");
MODULE_ALIAS("platform:" DRV_NAME);
