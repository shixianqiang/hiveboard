// the list followed is the naming convention
// |--------|-----------|--------|-----------|
// |  name  | meaning   |  name  | meaning   |
// |--------|-----------|--------|-----------|
// |  ADDR  | address   |  VECT  | vector    |
// |  CTL   | controller|  COMP  | component |
// |  INT   | interrupt |  RX    | receive   |
// |  MEM   | memory    |  TX    | transmit  |
// |  EN    | enable    |  STAT  | status    |
// |  MSK   | mask      |  CTRL  | controller|
// |  SLT   | select    |  SAMP  | sample    |
// |  SFT   | soft      |  CH    | channel   |
// |  STS   | status    |  CNT   | count(er) |
// |  PRIOR | priority  |  SUSP  | suspend   |
// |  HW    | hardware  |  SW    | software  | 
// |  DST   | dstination|  SRC   | source    |
// |  SEL   | select    |  POL   | polarity  |
// |  MAX   | maximum   |  BST   | burst     |
// |  ATUO  | automatic |  PROT  | protection|
// |  UPD   | update    |  PER   | peripheral|
// |  TRAN  | transfer  |  INCR  | increment |
// |  BLK   | block     |  SCAT  | scatter   |
// |  GATH  | gather    |  SZ    | size      |

// |  LLP   | linked list pointer            |
// |--------|-----------|--------|-----------|
#ifndef __SDIO_DMAC_H_
#define __SDIO_DMAC_H_
// DMAC Configuration Register
#define    DMAC_CFG_REG_EN(x)                  (x)                 //Enable DMAC. x=0,disable DMAC; x=1, enable DMAC

// DMAC Channel Enable Register
#define    DMAC_CH_EN(x)                       (x)                  //Enable the Channel.x=0,enable channel; x=1,diable channel
#define    DMAC_CH_WRITE_EN(x)                 (x << 8)                  //Channel Enable Write Enable.x=0,disable; x=1,enable


// DMAC Configuration Register for Channel x, x=0~7
#define    DMAC_CH_PRIOR(x)                    (x << 5)                  //Channel Priority. x:0~7. 7 is the highest priority, and 0 is the lowest   
#define    DMAC_CH_SUSP(x)                     (x << 8)                  //Channel Suspend. x=0,not suspend;x=1,suspend DMA transfer from the source 
#define    DMAC_FIFO_EMPTY(x)                  (x << 9)                  //read only. Indicates if there is data left in the channel FIFO. 1:channel fifo empty; 0: channel fifo not empty;
#define    DMAC_HW_SW_SEL_DST(x)               (x << 10)                 //Destination Software or Hardware Handshaking Select. x=0, hardware handshaking; x=1, software handshaking. if the dest peripheral is memory, then this bit is ignored.

#define    DMAC_HW_SW_SEL_SRC(x)               (x << 11)                 //Source Software or Hardware Handshaking Select. x=0, hardware handshaking; x=1, software handshaking. if the SRC peripheral is memory, then this bit is ignored.
#define    DMAC_CH_LOCK_LEVEL(x)               (x << 12)                 //Channel Lock Level.x=00,over complete DMA transfer; x=01, over complete DMA block tranfer; x=1x, over complete DMA transaction.
#define    DMAC_BUS_LOCK_LEVEL(x)              (x << 14)                 //Bus Lock Level.x=00,over complete DMA transfer; x=01, over complete DMA block tranfer; x=1x, over complete DMA transaction.
#define    DMAC_CH_LOCK(x)                     (x << 16)                 //Channel Lock Bit.x=1,exclusive access to the master bus interface  
#define    DMAC_BUS_LOCK(x)                    (x << 17)                 //Bus Lock Bit. when active, the AHB bus master signal is aserted  
#define    DMAC_DST_HS_POL(x)                  (x << 18)                 //DST Handshaking Interface Polarity.x=0,Active high; x=1, Active low
#define    DMAC_SRC_HS_POL(x)                  (x << 19)                 //SRC Handshaking Interface Polarity.x=0,Active high; x=1, Active low
#define    DMAC_MAX_AMBA_BST_LEN(x)            (x << 20)                 //Maximum AMBA Burst Length.A value of 0 indicates that software is not limiting the maximum AMBA burst length for DMA tranfer on this channel
#define    DMAC_AUTO_RELOAD_SRC(x)             (x << 30)                 //Automatic Source Reload. The SARx register can be automatically reloaded from its initial value at the end of block tranfer for multi-block transfers.
#define    DMAC_AUTO_RELOAD_DST(x)             (x << 31)                 //Automatic Destination Reload. The DSTx register can be automatically reloaded from its initial value at the end of block tranfer for multi-block transfers.
#define    DMAC_FLOW_CTL_MODE(x)               (x)                       //Flow Control Mode.x=0,source transaction requests are serviced; x=1, source transaction requests are not serviced
#define    DMAC_FIFO_MODE(x)                   (x << 1)                  //FIFO Mode Select. x=0,space/data available for single AHB tranfer of the specified transfer width. 
#define    DMAC_PROT_CTL(x)                    (x << 2)                  //Protection Control bits used to drive the AHB HPROT[3:1] bus.
#define    DMAC_DST_STAT_UPD_EN(x)             (x << 5)                  //Destination Status Update Enable.
#define    DMAC_SRC_STAT_UPD_EN(x)             (x << 6)                  //Source Status Update Enable.  
#define    DMAC_SRC_PER(x)                     (x << 7)                  //x=0~15,Assigns a hardware handshaking interface to the source of channel of x.  
#define    DMAC_DST_PER(x)                     (x << 11)                 //x=0~15,Assigns a hardware handshaking interface to the destination of channel of x. 

// DMAC Control Register for Channel x
#define    DMAC_INT_EN(x)                      (x)                       //Interrupt Enable Bit.if set, then all interrupt-generating sources are enabled
#define    DMAC_DST_TRAN_WIDTH(x)              (x << 1)                  //Destination Transfer Width.0:8; 1:16; 2:32; 3:64; 4:128; 5:256; 11x:256
#define    DMAC_SRC_TRAN_WIDTH(x)              (x << 4)                  //Source Transfer Width. 0:8; 1:16; 2:32; 3:64; 4:128; 5:256; 11x:256
#define    DMAC_DST_INCR(x)                    (x << 7)                  //Destination Address Increment.0:Increment; 1:Decrement; 1x:No change  
#define    DMAC_SRC_INCR(x)                    (x << 9)                  //Source Address Increment.0:Increment; 1:Decrement; 1x:No change  
#define    DMAC_DST_BST_SZ(x)                  (x << 11)                 //Destination Burst Transaction Length. 0:1; 1:4; 2:8; 3:16; 4:32; 5:64; 6:128; 7:256
#define    DMAC_SRC_BST_SZ(x)                  (x << 14)                 //Source Burst Transaction Length. 0:1; 1:4; 2:8; 3:16; 4:32; 5:64; 6:128; 7:256
#define    DMAC_SRC_GATH_EN(x)                 (x << 17)                 //Source gather enable bit.0:gather enabled; 1: gather disabled
#define    DMAC_DST_SCAT_EN(x)                 (x << 18)                 //Destination Scatter enable bit. 0:scatter enabled; 1:scatter disabled
#define    DMAC_TRAN_TYPE_FLOW_CTL(x)          (x << 20)                 //Transfer Type and Flow Control. 0: MEM to MEM, DMAC; 1: MEM to PER, DMAC; 2: PER to MEM, DMAC; 3:PER to PER, DMAC; 4: PER to MEM, PER; 5: PER to PER, SRC PER; 6: MEM to PER,PER; 7:PER to PER, DST PER;
#define    DMAC_DST_MASTER_SEL(x)              (x << 23)                 //Destination Master Select. 0:AHB master1;1:AHB master2; 2:AHB master3; 3: AHB master4
#define    DMAC_SRC_MASTER_SEL(x)              (x << 25)                 //Source Master Select. 0:AHB master1;1:AHB master2; 2:AHB master3; 3: AHB master4     
#define    DMAC_DST_LLP_EN(x)                  (x << 27)                 //Block chaining enable on the destination side 
#define    DMAC_SRC_LLP_EN(x)                  (x << 28)                 //Block chaining enable on the source side
#define    DMAC_BLK_TRAN_SZ(x)                 (x)                       //Block Tranfer Size.
#define    DMAC_DONE(x)                        (x << 12 )                //Done bit

// DMAC Linked List Pointer Register for Channel x 
#define    DMAC_LIST_MASTER_SEL(x)             (x)                       //List Master Select. 00:AHB master 1; 01:AHB master 2; 10:AHB master 3; 11:AHB master 4;
#define    DMAC_LOC(x)                         (x << 2)                  //Start Address in Memory of next LLI 

// DMAC Destination Scatter Register for Channel x
#define    DMAC_DST_SCAT_INTERVAL(x)           (x)                        //Destination Scatter Interval
#define    DMAC_DST_SCAT_CNT(x)                (x << 20)                  //Destination Scatter Count

// DMAC Source Gather Register for Channel x
#define    DMAC_SRC_GATH_INTERVAL(x)           (x)                        //Source Gather Interval
#define    DMAC_SRC_GATH_CNT(x)                (x << 20)                  //Source Gather Count


#endif



