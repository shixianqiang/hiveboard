/*
 * PKUnity Ultra Media Access Layer (UMAL) Ethernet MAC Registers
 */

/* MAC module of UMAL */
/* UMAL's MAC module includes G/MII interface, several additional PHY
 * interfaces, and MAC control sub-layer, which provides support for control
 * frames (e.g. PAUSE frames).
 */
#define UMAL_CFG1 		__REG(PKUNITY_UMAL_BASE + 0x0000)	/* TX/RX reset and control    */
#define UMAL_CFG2		__REG(PKUNITY_UMAL_BASE + 0x0004)	/* MAC interface mode control */ 
#define UMAL_IPGIFG 		__REG(PKUNITY_UMAL_BASE + 0x0008)	/* Inter Packet/Frame Gap     */ 
#define UMAL_HALFDUPLEX		__REG(PKUNITY_UMAL_BASE + 0x000c)	/* Collision retry or backoff */
#define UMAL_MAXFRAME		__REG(PKUNITY_UMAL_BASE + 0x0010)	/* Maximum Frame Length       */		
#define UMAL_TESTREG		__REG(PKUNITY_UMAL_BASE + 0x001c)	/* Test Regsiter              */
#define UMAL_MIICFG		__REG(PKUNITY_UMAL_BASE + 0x0020)	/* MII Management Configure   */
#define UMAL_MIICMD		__REG(PKUNITY_UMAL_BASE + 0x0024)	/* MII Management Command     */
#define UMAL_MIIADDR 		__REG(PKUNITY_UMAL_BASE + 0x0028)	/* MII Management Address     */
#define UMAL_MIICTRL		__REG(PKUNITY_UMAL_BASE + 0x002c)	/* MII Management Control     */
#define UMAL_MIISTATUS		__REG(PKUNITY_UMAL_BASE + 0x0030)	/* MII Management Status      */
#define UMAL_MIIIDCT		__REG(PKUNITY_UMAL_BASE + 0x0034)	/* MII Managment Indicator    */
#define UMAL_IFCTRL		__REG(PKUNITY_UMAL_BASE + 0x0038)	/* Interface Control 	      */
#define UMAL_IFSTATUS		__REG(PKUNITY_UMAL_BASE + 0x003c)	/* Interface Status           */
#define UMAL_STADDR1		__REG(PKUNITY_UMAL_BASE + 0x0040)	/* MAC address (high 4 bytes) */
#define UMAL_STADDR2		__REG(PKUNITY_UMAL_BASE + 0x0044)	/* MAC address (low 2 bytes)  */

/* FIFO MODULE OF UMAL */
/* UMAL's FIFO module provides data queuing for increased system level
 * throughput
 */
#define UMAL_FIFOCFG0 		__REG(PKUNITY_UMAL_BASE + 0x0048)
#define UMAL_FIFOCFG1 		__REG(PKUNITY_UMAL_BASE + 0x004c)
#define UMAL_FIFOCFG2 		__REG(PKUNITY_UMAL_BASE + 0x0050)
#define UMAL_FIFOCFG3 		__REG(PKUNITY_UMAL_BASE + 0x0054)
#define UMAL_FIFOCFG4 		__REG(PKUNITY_UMAL_BASE + 0x0058)
#define UMAL_FIFOCFG5 		__REG(PKUNITY_UMAL_BASE + 0x005c)
#define UMAL_FIFORAM0 		__REG(PKUNITY_UMAL_BASE + 0x0060)
#define UMAL_FIFORAM1 		__REG(PKUNITY_UMAL_BASE + 0x0064)
#define UMAL_FIFORAM2 		__REG(PKUNITY_UMAL_BASE + 0x0068)
#define UMAL_FIFORAM3 		__REG(PKUNITY_UMAL_BASE + 0x006c)
#define UMAL_FIFORAM4 		__REG(PKUNITY_UMAL_BASE + 0x0070)
#define UMAL_FIFORAM5 		__REG(PKUNITY_UMAL_BASE + 0x0074)
#define UMAL_FIFORAM6 		__REG(PKUNITY_UMAL_BASE + 0x0078)
#define UMAL_FIFORAM7 		__REG(PKUNITY_UMAL_BASE + 0x007c)

/* MAHBE MODUEL OF UMAL */
/* UMAL's MAHBE module interfaces to the host system through 32-bit AHB Master
 * and Slave ports.Registers within the M-AHBE provide Control and Status
 * information concerning these transfers.
 */
#define UMAL_DMATxCtrl		__REG(PKUNITY_UMAL_BASE + 0x0180)	/* Transmit Control              */	
#define UMAL_DMATxDescriptor	__REG(PKUNITY_UMAL_BASE + 0x0184)	/* Pointer to TX Descripter      */
#define UMAL_DMATxStatus	__REG(PKUNITY_UMAL_BASE + 0x0188)	/* Status of Tx Packet Transfers */
#define UMAL_DMARxCtrl		__REG(PKUNITY_UMAL_BASE + 0x018c)	/* Receive Control               */ 
#define UMAL_DMARxDescriptor	__REG(PKUNITY_UMAL_BASE + 0x0190)	/* Pointer to Rx Descriptor      */
#define UMAL_DMARxStatus	__REG(PKUNITY_UMAL_BASE + 0x0194)	/* Status of Rx Packet Transfers */
#define UMAL_DMAIntrMask	__REG(PKUNITY_UMAL_BASE + 0x0198)	/* Interrupt Mask                */
#define UMAL_DMAInterrupt	__REG(PKUNITY_UMAL_BASE + 0x019c)	/* Interrupts, read only         */

/* Commands for UMAL_CFG1 register */
#define UMAL_CFG1_TXENABLE 	FIELD(1, 1, 0)
#define UMAL_CFG1_RXENABLE 	FIELD(1, 1, 2)
#define UMAL_CFG1_TXFLOWCTL	FIELD(1, 1, 4)
#define UMAL_CFG1_RXFLOWCTL	FIELD(1, 1, 5)
#define UMAL_CFG1_CONFLPBK 	FIELD(1, 1, 8)
#define UMAL_CFG1_RESET 	FIELD(1, 1, 31)	
#define UMAL_CFG1_CONFFLCTL 	(MAC_TX_FLOW_CTL | MAC_RX_FLOW_CTL)	

/* Commands for UMAL_CFG2 register */
#define UMAL_CFG2_FULLDUPLEX	FIELD(1, 1, 0)
#define UMAL_CFG2_CRCENABLE	FIELD(1, 1, 1)
#define UMAL_CFG2_PADCRC	FIELD(1, 1, 2)
#define UMAL_CFG2_LENGTHCHECK	FIELD(1, 1, 4)
#define UMAL_CFG2_MODEMASK	FMASK(2, 8)
#define UMAL_CFG2_NIBBLEMODE	FIELD(1, 2, 8)
#define UMAL_CFG2_BYTEMODE	FIELD(2, 2, 8)
#define UMAL_CFG2_PREAMBLENMASK	FMASK(4, 12)
#define UMAL_CFG2_DEFPREAMBLEN	FIELD(7, 4, 12)
#define UMAL_CFG2_FD100		(UMAL_CFG2_DEFPREAMBLEN | UMAL_CFG2_NIBBLEMODE | \
	       			UMAL_CFG2_LENGTHCHECK | UMAL_CFG2_PADCRC | \
				UMAL_CFG2_CRCENABLE | UMAL_CFG2_FULLDUPLEX)
#define UMAL_CFG2_FD1000	(UMAL_CFG2_DEFPREAMBLEN | UMAL_CFG2_BYTEMODE | \
				UMAL_CFG2_LENGTHCHECK | UMAL_CFG2_PADCRC | \
				UMAL_CFG2_CRCENABLE | UMAL_CFG2_FULLDUPLEX)
#define UMAL_CFG2_HD100		(UMAL_CFG2_DEFPREAMBLEN | UMAL_CFG2_NIBBLEMODE | \
				UMAL_CFG2_LENGTHCHECK | UMAL_CFG2_PADCRC | \
				UMAL_CFG2_CRCENABLE) 

/*Command for UMAL_IFCTRL register*/
#define UMAL_IFCTRL_RESET	FIELD(1, 1, 31)

/*Command for UMAL_MIICFG register */
#define UMAL_MIICFG_RESET	FIELD(1, 1, 31)

/*Command for UMAL_MIICMD register */
#define UMAL_MIICMD_READ	FIELD(1, 1, 0)

/*Command for UMAL_MIIIDCT register */
#define UMAL_MIIIDCT_BUSY	FIELD(1, 1, 0)
#define UMAL_MIIIDCT_NOTVALID 	FIELD(1, 1, 2)

/* Commands for DMATxCtrl regesters */
#define UMAL_DMA_Enable		FIELD(1, 1, 0)

/* Commands for DMARxCtrl regesters */
#define UMAL_DMAIntrMask_ENABLEHALFWORD	FIELD(1, 1, 16)

/* Command for DMARxStatus */
#define CLR_RX_BUS_ERR		FIELD(1, 1, 3)
#define CLR_RX_OVERFLOW 	FIELD(1, 1, 2)
#define CLR_RX_PKT		FIELD(1, 1, 0)

/* Command for DMATxStatus */
#define CLR_TX_BUS_ERR		FIELD(1, 1, 3)
#define CLR_TX_UNDERRUN		FIELD(1, 1, 1)
#define CLR_TX_PKT		FIELD(1, 1, 0)

/* Commands for DMAIntrMask and DMAInterrupt register */
#define INT_RX_MASK		FIELD(0xd, 4, 4)
#define INT_TX_MASK		FIELD(0xb, 4, 0)

#define INT_RX_BUS_ERR		FIELD(1, 1, 7)
#define INT_RX_OVERFLOW		FIELD(1, 1, 6)
#define INT_RX_PKT		FIELD(1, 1, 4)
#define INT_TX_BUS_ERR		FIELD(1, 1, 3)
#define INT_TX_UNDERRUN		FIELD(1, 1, 1)
#define INT_TX_PKT		FIELD(1, 1, 0)

/*MARCOS of UMAL's descriptors */
#define UMAL_DESC_PACKETSIZE_EMPTY	FIELD(1, 1, 31)
#define UMAL_DESC_PACKETSIZE_NONEMPTY	FIELD(0, 1, 31)
#define UMAL_DESC_PACKETSIZE_SIZEMASK	FMASK(12, 0)

