#ifndef __SDIO_SDIO1_H_
#define __SDIO_SDIO1_H_

//Macro define for SDIO1_CTRL
#define    SDIO1_CTRL_RST(x)             (x << 0)
#define    SDIO1_FIFO_RST(x)             (x << 1)
#define    SDIO1_DMA_RST(x)              (x << 2)
#define    SDIO1_INT_EN(x)               (x << 4)
#define    SDIO1_DMA_EN(x)               (x << 5)
#define    SDIO1_READ_WAIT(x)            (x << 6)
#define    SDIO1_SET_IRQ_RESP(x)         (x << 7)
#define    SDIO1_ABT_RD_DAT(x)           (x << 8)

//Macro define for SDIO1_PWREN
#define    SDIO1_PWR_OFF                 (0)
#define    SDIO1_PWR_ON                  (1)

// Timeout Register
#define    resp_tmout(x)                 (x)
#define    data_tout(x)                  (x << 8)

//Macro define for SDIO1_CLKDIV
#define    SDIO1_CLK_DIV0(x)             (x << 0 )
#define    SDIO1_CLK_DIV1(x)             (x << 8 )
#define    SDIO1_CLK_DIV2(x)             (x << 16)
#define    SDIO1_CLK_DIV3(x)             (x << 24)

// SD Clock Source Register
#define    CLK_SRC(x)                    (x)//x can be 0,1,2,3
 
// FIFO Threshold Watermark Register
#define   MSIZE1                         (0 << 28) 
#define   MSIZE4                         (1 << 28) 
#define   MSIZE8                         (2 << 28)  
#define   MSIZE16                        (3 << 28)  
#define   MSIZE32                        (4 << 28)  
#define   MSIZE64                        (5 << 28)  
#define   MSIZE128                       (6 << 28)  
#define   MSIZE256                       (7 << 28)  
#define   RX_WMARK(X)                    (X << 16)
#define   TX_WMARK(X)                    (X)

//Macro define for SDIO1_CLKSRC
#define    SDIO1_CLK_SOURCE(x)           (x << 0 )

//Macro define for SDIO1_CLKENA
#define    SDIO1_CCLK_EN(x)              (x << 0 )
#define    SDIO1_CCLK_LW_PWR(x)          (x << 16)

//Macro define for SDIO1_CTYPE
#define    SDIO1_CTYPE_WIDTH1             (0)
#define    SDIO1_CTYPE_WIDTH4             (1)

//Macro define for SDIO1_INTMASK
#define    SDIO1_INTMASK_CD(x)           (x)
#define    SDIO1_INTMASK_RE(x)           (x << 1 )
#define    SDIO1_INTMASK_CP(x)           (x << 2 )
#define    SDIO1_INTMASK_DTP(x)          (x << 3 )
#define    SDIO1_INTMASK_TXDR(x)         (x << 4 )
#define    SDIO1_INTMASK_RXDR(x)         (x << 5 )
#define    SDIO1_INTMASK_RCRC(x)         (x << 6 )
#define    SDIO1_INTMASK_DCRC(x)         (x << 7 )
#define    SDIO1_INTMASK_RTO(x)          (x << 8 )
#define    SDIO1_INTMASK_DTO(x)          (x << 9 )
#define    SDIO1_INTMASK_HTO(x)          (x << 10)
#define    SDIO1_INTMASK_FRUN(x)         (x << 11)
#define    SDIO1_INTMASK_HLE(x)          (x << 12)
#define    SDIO1_INTMASK_SBE(x)          (x << 13)
#define    SDIO1_INTMASK_ACD(x)          (x << 14)
#define    SDIO1_INTMASK_EBE(x)          (x << 15)
#define    SDIO1_INTMASK_EN(x)           (x << 16)

//Macro define for SDIO1_CMD
#define    SDIO1_CMD_RESP_EXPE(x)        (x << 6 )
#define    SDIO1_CMD_LONG_RESP(x)        (x << 7 )
#define    SDIO1_CMD_CHK_RESP_CRC(x)     (x << 8 )
#define    SDIO1_CMD_HAVE_DAT_TRAN(x)    (x << 9 )
#define    SDIO1_CMD_WRITE(x)            (x << 10)
#define    SDIO1_CMD_BLK_TRAN(x)         (x << 11)  //x=0,block transfer,x=1,stream transfer
#define    SDIO1_CMD_SET_STOP(x)         (x << 12)
#define    SDIO1_CMD_WAIT_DAT(x)         (x << 13)
#define    SDIO1_CMD_STP_ABT(x)          (x << 14)
#define    SDIO1_CMD_SEND_INIT(x)        (x << 15)
#define    SDIO1_CMD_CARD_NUM(x)         (x << 16)
#define    SDIO1_CMD_UPDATE_CLK_REG_ONLY(x) (x << 21)   //x can be 1 or 0
#define    SDIO1_CMD_START               (1 << 31)


#endif
 
