#ifndef _ETHER1008_H
#define _ETHER1008_H

/*
 * ETHER1008 Internal Registers
 * (Use internal register read/write SPI command)
 */
#define	EI_CTRL0			0x00
#define EI_CTRL0_CHKSUM			0x03
#define EI_CTRL0_VLAN			0x0C
#define EI_CTRL0_SA			0x70
#define EI_CTRL0_SA_SHIFT		4
#define EI_CTRL0_CRC_REPL		0x80

#define EI_CTRL1			0x01
#define EI_CTRL1_DISPAD			0x01
#define EI_CTRL1_DISCRC			0x02
#define EI_CTRL1_TX_AUTO_STAT_POP	0x03

#define	EI_TX_PBL			0x02

#define EI_TX_WATERMARK			0x03
#define EI_TX_WATERMARK_BIT		0x80

#define EI_RXFIFO_FRM_CNT		0x08
#define EI_RXFIFO_FRM_CNT_BIT		0x7F

#define EI_FRAMEFLUSH			0x09
#define EI_FRAMEFLUSH_BIT		0x01

#define EI_RX_PBL_L			0x0A

#define EI_RX_WATERMARK			0x0B
#define EI_RX_PBL_H_BIT			0x01
#define EI_RX_WATERMARK_BIT		0x80

#define EI_CLK_OUT_DIV_PARA		0x10

#define	EI_CLK_CTRL			0x11
#define	EI_CLK_CTRL_OUT_SEL		0x03
#define	EI_CLK_CTRL_OUT_EN		0x04
#define	EI_CLK_CTRL_RX_SEL		0x08
#define	EI_CLK_CTRL_TX_SEL		0x10
#define	EI_CLK_CTRL_GATE_MAC_TX		0x20
#define	EI_CLK_CTRL_GATE_MAC_RX		0x40
#define	EI_CLK_CTRL_GATE_MAC_APP	0x80

#define EI_SPI_CTRL			0x12
#define EI_SPI_CTRL_NEDGE_DATA		0x01
#define EI_SPI_CTRL_INTR_OE		0x02

#define EI_RX_FLEN_FIFO_TH		0x20

#define EI_RX_DATA_FIFO_TH		0x21

#define EI_RX_DATA_FIFO_TO		0x23

#define EI_TX_DATA_FIFO_TH		0x25

#define EI_TX_STAT_FIFO_TH		0x27

#define EI_FIFO_CLR			0x28
#define EI_FIFO_CLR_RX_DATA		0x01
#define EI_FIFO_CLR_TX_DATA		0x02
#define EI_FIFO_CLR_TX_STAT		0x04

#define EI_PHY_CTRL			0x30
#define EI_PHY_CTRL_PWRDN		0x01
#define EI_PHY_CTRL_OSCMD		0x02
#define EI_PHY_CTRL_PWRSV		0x04

#define EI_PHY_LED			0x31
#define EI_PHY_LED0_SEL			0x07
#define EI_PHY_LED0_OE			0x08
#define EI_PHY_LED1_SEL			0x70
#define EI_PHY_LED1_OE			0x80

#define EI_PHY_ID			0x32

#define EI_INTR_MASK			0x40
#define EI_INTR_STAT			0x50
#define EI_INTR_RX_FLEN_FIFO_TH		0x0001
#define EI_INTR_RX_DATA_FIFO_TH		0x0002
#define EI_INTR_RX_DATA_FIFO_TO		0x0004
#define EI_INTR_TX_DATA_FIFO_TH		0x0008
#define EI_INTR_TX_STAT_FIFO_TH		0x0010
#define EI_INTR_MCI			0x0020
#define EI_INTR_PMT			0x0040
#define EI_INTR_PHY_LINK		0x0080
#define EI_INTR_SPI_TX_LEN_L		0x0100
#define EI_INTR_SPI_TX_LEN_G		0x0200
#define EI_INTR_SPI_RX_LEN_G		0x0400
#define EI_INTR_SPI_TXF_OVR		0x0800
#define EI_INTR_SPI_RXF_NDR		0x1000
#define EI_INTR_SPI_TX_FLEN_CRC_ERR	0x2000
#define EI_INTR_SPI_RX_FLEN_CRC_ERR	0x4000
#define EI_INTR_SPI_STAT_DEAD		0x8000

#define EI_RX_FLEN_FIFO_CNT		0x52

#define EI_RX_DATA_FIFO_FLVL		0x53

#define EI_TX_DATA_FIFO_FLVL		0x55

#define EI_TX_STAT_FIFO_FLVL		0x57

#define EI_RAW_INTR			0x60

#define EI_CHIP_ID			0x70		

/*
 * ETHER1008 MAC Registers
 * 4 bytes aligned accessing
 * (Use MAC register read/write SPI command)
 */
#define EMAC_CONFIG			0x00
#define EMAC_CONFIG_RE			0x4
#define EMAC_CONFIG_TE			0x8
#define EMAC_CONFIG_ACS			0x80
#define EMAC_CONFIG_DM			0x800
#define EMAC_CONFIG_DO			0x2000
#define EMAC_CONFIG_FES			0x4000
#define EMAC_CONFIG_PS			0x8000
#define EMAC_CONFIG_JE			0x100000
#define EMAC_CONFIG_BE			0x200000
#define EMAC_CONFIG_2KPE		0x8000000

#define EMAC_FRAME_FILTER		0x04
#define EMAC_FF_PM			0x10
#define EMAC_FF_PCF			0xC0
#define EMAC_FF_PCF_SHIFT		6
#define EMAC_FF_HPF			0x400

#define EMAC_HASH_TABLE_HIGH		0x08
#define EMAC_HASH_TABLE_LOW		0x0C

#define EMAC_GMII_ADDR			0x10
#define EMAC_GMII_ADDR_PA		0xF800
#define EMAC_GMII_ADDR_GR		0x07C0
#define EMAC_GMII_ADDR_CR		0x003C
#define EMAC_GMII_ADDR_GW		0x0002
#define EMAC_GMII_ADDR_GB		0x0001
 
#define EMAC_GMII_DATA			0x14
#define EMAC_GMII_DATA_GD		0xFFFF

#define EMAC_FLOW_CONTROL		0x18
#define EMAC_FC_TFE			0x2
#define EMAC_FC_RFE			0x4
#define EMAC_FC_UP			0x8
#define EMAC_FC_PT			0xFFFF0000

#define EMAC_VLAN_TAG			0x1C
#define EMAC_VERSION			0x20
#define EMAC_DEBUG			0x24
#define EMAC_REMOTE_WKUP_FRAME_FILTER	0x28
#define EMAC_PMT_CNTR_STAT		0x2C
#define EMAC_INTR_STAT			0x38
#define EMAC_INTR_MASK			0x3C
#define EMAC_MAC_ADDR0_HIGH		0x40
#define EMAC_MAC_ADDR0_LOW		0x44
#define EMAC_MAC_ADDR1_HIGH		0x48
#define EMAC_MAC_ADDR1_LOW		0x4C
#define EMAC_MAC_ADDR2_HIGH		0x50
#define EMAC_MAC_ADDR2_LOW		0x54

#define EMAC_VLAN_TAG_INCL_RPLC		0x584
#define EMAC_VLAN_TAG_HASH_TABLE	0x588

#define EMAC_OP_MODE			0x800
#define EMAC_OP_FUF			0x40
#define EMAC_OP_EFC			0x100
#define EMAC_OP_RFA			0x600
#define EMAC_OP_RFA_SHIFT		9
#define EMAC_OP_RFD			0x1800
#define EMAC_OP_RFD_SHIFT		11
#define EMAC_OP_TSF			0x200000
#define EMAC_OP_RSF			0x2000000
#define EMAC_OP_DT			0x4000000

/*
 * ETHER1008 PHY Registers
 * Use MAC register SPI command and access should through
 * GMII_ADDR and GMII_DATA registers
 */
#define EPHY_BMCR		0x00

#define EPHY_BMSR		0x01
#define EPHY_BMSR_LINK_STATUS	0x04

#define EPHY_ID1		0x02
#define EPHY_ID2		0x03
#define EPHY_ANAR		0x04
#define EPHY_ANLPAR		0x05
#define EPHY_ANER		0x06

/* SPI command codes */
#define SPI_CMD_WRITE_INT_REG	0x0
#define SPI_CMD_READ_INT_REG	0x1
#define SPI_CMD_WRITE_MAC_REG	0x2
#define SPI_CMD_READ_MAC_REG	0x3
#define SPI_CMD_WRITE_TX_FIFO	0x4
#define SPI_CMD_READ_RX_FIFO	0x5
#define SPI_CMD_FIFO_RW		0x6
#define SPI_CMD_READ_TX_STAT	0x7
#define SPI_CMD_RESET_ALL	0xF

#define SPI_CMD_DUMMY_WAIT	0x0

#define spi_cmd_write_int_reg(addr, data0, data1) \
	spi_cmd_rw(SPI_CMD_WRITE_INT_REG, addr, data0, data1)

#define spi_cmd_read_int_reg(addr) \
	spi_cmd_rw(SPI_CMD_READ_INT_REG, addr, \
		SPI_CMD_DUMMY_WAIT, SPI_CMD_DUMMY_WAIT)

#define spi_cmd_write_mac_reg(addr, data0, data1) \
	spi_cmd_rw(SPI_CMD_WRITE_MAC_REG, addr, data0, data1)

#define spi_cmd_read_mac_reg(addr) \
	spi_cmd_rw(SPI_CMD_READ_MAC_REG, addr, \
		SPI_CMD_DUMMY_WAIT, SPI_CMD_DUMMY_WAIT)

#define spi_cmd_write_tx_fifo(len, data0, data1) \
	spi_cmd_rw(SPI_CMD_WRITE_TX_FIFO, len, data0, data1)

#define spi_cmd_read_rx_fifo(len) \
	spi_cmd_rw(SPI_CMD_READ_RX_FIFO, len, \
		SPI_CMD_DUMMY_WAIT, SPI_CMD_DUMMY_WAIT)

#define spi_cmd_fifo_rw(rx_len, tx_len) \
	spi_cmd_rw(SPI_CMD_FIFO_RW, rx_len, \
		(tx_len & SPI_TX_LEN_B2_MASK) >> SPI_TX_LEN_B2_SHIFT, \
		(tx_len & SPI_CMD_BYTE_MASK))

#define spi_cmd_read_tx_stat(len) \
	spi_cmd_rw(SPI_CMD_READ_TX_STAT, (len & SPI_READ_TX_STAT_LEN_MASK) \
		SPI_CMD_DUMMY_WAIT, SPI_CMD_DUMMY_WAIT)

#define spi_cmd_reset_all() \
	spi_cmd_rw(SPI_CMD_RESET_ALL, 0xF | SPI_CMD_DUMMY_WAIT, \
		SPI_CMD_DUMMY_WAIT, SPI_CMD_DUMMY_WAIT)

/* maximum ethernet frame length */
#define MAX_FRAMELEN            1518

#define INTR_MASK	(EI_INTR_RX_DATA_FIFO_TH | EI_INTR_RX_DATA_FIFO_TO | EI_INTR_TX_DATA_FIFO_TH)

#endif
