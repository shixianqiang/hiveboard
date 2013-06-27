/*****************************************************************************************
* Filename    :  SEP0611_REG.h
* Description :  This file is used to define the register address of 
*                the registers of the SEP0611 processor.
* Date        :  2010-02-25
* Author      :  Phoenix Liu
* Current Ver :  v1.10
*
* !!! Please Read the File Log Message Before Use This File  
******************************************************************************************/
//
// ---------------------------------------------------------------------------------------
//                           version log                                
//   version      date         Author              modify infomation
//    v1.00    2010-02-25   Phoenix Liu   1) create this header file
//                                        2) define some module's register to this text
//                                           include component address list, vic, timer,
//                                           rtc, uart0-2, ssi1-3, i2c1-3, sdio1-3  
//    v1.01    2010-03-08   Phoenix Liu   1) add DDR2 MCTL register file to this text
//                                        2) add the naming convention list
//    v1.02    2010-03-30   Phoenix Liu   1) add DMAC register file to this text
//                                        2) add the version log
//    v1.03    2010-03-31   Phoenix Liu   1) add I2S register file to this text
//                                        2) add new item to the naming convention list
//                                        3) refine the version log
//    v1.04    2010-04-23   Phoenix Liu   1) add LCDC register file to this text
//                                        2) fix the naming convention list
//    v1.05    2010-05-12   Phoenix Liu   1) add GPIO module's register to this text
//    v1.06    2010-05-13   Phoenix Liu   1) add NAND controller register to this text
//                                        2) add UART3 register define to this text 
//                                        3) modify the bus2 bus3 module base address
//                                           accroding with Tiger_Spec_v1.19.doc
//                                        4) delet unused module register list from this 
//                                           text, include sdio3, i2c2-3, tsi
//                                        5) add the header usage of the different fpga
//                                           image version
//    v1.07    2010-06-29   Phoenix Liu   1) add I2S_SCR register to the list
//                                        2) remove the EMI register list which is not exist
//                                           in SEP0611 anymore, and add nor controller 
//                                           register list
//                                        3) modify the register list define for DMAC, add a
//                                           set of register define that contain the index 
//                                           number of module and channel of DMAC 
//    v1.08    2010-07-29   Phoenix Liu   1) add PMU registers list to the file
//                                        2) modify the RTC register list definition accroding 
//                                           with the change of the RTC RTL(r71)
//                                        3) fixed some errors in the previous version
//                                        4) modify the version reflect table
//    v1.09    2010-09-06   Phoenix Liu   1) modify the register of PMU
//                                        2) modify the module base address according with the 
//                                           change of the RTL r78, add I2C1-3 address delete
//                                           HDMI and ADUIO address
//                                        3) modify the naming convention table 
//    v1.10    2010-09-12   Phoenix Liu   1) modify the SDIO register list according with the 
//                                           change of the rtl 
// -----------------------------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------------------------
//           the usage of the header file for fpga version (version reflect table) 
//   headerfile version             corresponding fpga version       corresponding rtl revision
//          v1.00                               n/a                                n/a
//          v1.01                               n/a                                n/a
//          v1.02                fpga_top_0329   ~ fpga_top_0330                   r27
//          v1.03                fpga_top_ddr2v1 ~ fpga_top_ddr2v7                 r31
//          v1.04                fpga_top_v2_r6  ~ fpga_top_v2_r11                 r31
//          v1.05                fpga_top_v2_r11 ~ fgpa_top_v2_r12                 r36
//          v1.06                fpga_top_v3_r1  ~ fpga_top_v2_r12                 r39
//          		             0718_b1_r1      ~ 0718_b1_r5                      
//          		             0718_b2_r1      ~ 0718_b2_r6                      
//          		             0718_b3_r1      ~ 
//          		             0611_b2_r1      ~
//          v1.07                0718_b1_r6      ~ 0718_b1_r21                   r46/r52
//                               0718_b2_r7      ~ 0718_b2_r10
//                               0611_b1_r1      ~ 0611_b1_r2
//                               0611_b2_r2      ~ 0611_b1_r3
//                               0611_b3_r1      ~ 0611_b1_r5
//          v1.08                0718_b1_r22     ~ 0718_b1_r25                     r71
//                               0718_b2_r11     ~ 0718_b2_r15
//                               0718_b3_r2      ~ 0718_b3_r7
//          v1.09                0718_b1_r26     ~                                 r78
//
// note : from v1.06, this file is used for 3 branches fpga image version
//
// the information of upper block is used for fpga verification and simulation
//
// the list followed is the naming convention
// |--------|-----------|--------|-----------|--------|-----------|
// |  name  | meaning   |  name  | meaning   |  name  | meaning   |
// |--------|-----------|--------|-----------|--------|-----------|
// |  ADDR  | address   |  VECT  | vector    |  PWR   | power     |
// |  CTL   | controller|  COMP  | component |  PIX   | pixel     |
// |  INT   | interrupt |  RX    | receive   |  POL   | polarity  |
// |  MEM   | memory    |  TX    | transmit  |  DAT   | data      |
// |  EN    | enable    |  STAT  | status    |  TAR   | target    |
// |  MSK   | mask      |  CTRL  | controller|  CMD   | command   |
// |  SEL   | select    |  SAMP  | sample    |  MSG   | mssage    |
// |  SFT   | soft      |  CH    | channel   |  CFG   | config    |
// |  STS   | status    |  CNT   | counter   |  DIV   | divider   |
// |--------|-----------|--------|-----------|--------|-----------|

#ifndef __SDIO_SEP0611_REG_H_
#define __SDIO_SEP0611_REG_H_

// component base address define
// the base address of the memory
#define    NOR_MEM_BASE             0x20000000
#define    NOR_MEM_REMAP_ADDR       0x00000000
#define    ESRAM_BASE               0xB2000000
#define    ESRAM_REMAP_ADDR         0x00000000              
#define    DRAM_BASE                0x40000000
#define    DRAM_REMAP_ADDR          0x00000000


// the module base address of bus1-ahb64 and bus4-apb32
#define    VIC_BASE                 0xB0000000
#define    PMU_BASE                 0xB0001000
#define    RTC_BASE                 0xB0002000
#define    TIMER_BASE               0xB0003000
#define    PWM_BASE                 0xB0004000
#define    GPS_BASE                 0xB0005000
#define    GPIO_BASE                0xB0006000
#define    ADC_BASE                 0xB0007000
#define    SYS_CTRL                 0xB0008000

// the module base address of bus3-ahb32 and bus5-apb32
#define    UART_BASE                0xB1000000
#define    UART0_BASE               0xB1000000
#define    UART1_BASE               0xB1001000
#define    UART2_BASE               0xB1002000
#define    UART3_BASE               0xB1003000
#define    SSI_BASE                 0xB1004000
#define    SSI1_BASE                0xB1004000
#define    SSI2_BASE                0xB1005000
#define    SSI3_BASE                0xB1006000
#define    I2C1_BASE                0xB1007000
#define    I2C2_BASE                0xB1008000
#define    I2C3_BASE                0xB1009000
#define    I2S_BASE                 0xB100A000
#define    USB_BASE                 0xB1010000
#define    DMAC2_BASE               0xB1011000
#define    GPU_BASE                 0xB1013000
// note : the HDMI, AUDIO module will not be available in FPGA platform 

// the module base address of bus2-ahb32
#define    NORCTL_BASE              0xB2020000
#define    NAND_BASE                0xB2021000
#define    SDIO_BASE                0xB2022000
#define    SDIO1_BASE               0xB2022000
#define    SDIO2_BASE               0xB2023000
#define    LCDC_BASE                0xB2025000
#define    VPU_BASE                 0xB2026000
#define    DMAC1_BASE               0xB2027000
#define    DDR2CTL_BASE             0xB3000000

// the module base address of GPS sub-system, it
// will not be used in FPGA verification
#define    DMAC3_BASE               0xB4000000

#define    DMAC_BASE

// VIC register list
// VIC_BASE 0xB0000000
#ifdef VIC_BASE

#define    VIC_INT_SEL_L            (VIC_BASE + 0x000)
#define    VIC_INT_SEL_H            (VIC_BASE + 0x004)
#define    VIC_INT_EN_L             (VIC_BASE + 0x008)
#define    VIC_INT_EN_H             (VIC_BASE + 0x00C)
#define    VIC_INT_EN_CLR_L         (VIC_BASE + 0x010)
#define    VIC_INT_EN_CLR_H         (VIC_BASE + 0x014)
#define    VIC_SFT_INT_L            (VIC_BASE + 0x018)
#define    VIC_SFT_INT_H            (VIC_BASE + 0x01C)
#define    VIC_SFT_INT_CLR_L        (VIC_BASE + 0x020)
#define    VIC_SFT_INT_CLR_H        (VIC_BASE + 0x024)
#define    VIC_INT_MSK_ALL          (VIC_BASE + 0x028)
#define    VIC_RAW_INT_SRC_L        (VIC_BASE + 0x030)
#define    VIC_RAW_INT_SRC_H        (VIC_BASE + 0x034)
#define    VIC_RAW_IRQ_STS_L        (VIC_BASE + 0x038)
#define    VIC_RAW_IRQ_STS_H        (VIC_BASE + 0x03C)
#define    VIC_RAW_FIQ_STS_L        (VIC_BASE + 0x040)
#define    VIC_RAW_FIQ_STS_H        (VIC_BASE + 0x044)
#define    VIC_MSK_IRQ_STS_L        (VIC_BASE + 0x048)
#define    VIC_MSK_IRQ_STS_H        (VIC_BASE + 0x04C)
#define    VIC_MSK_FIQ_STS_L        (VIC_BASE + 0x050)
#define    VIC_MSK_FIQ_STS_H        (VIC_BASE + 0x054)
#define    VIC_IRQ_PENDING_L        (VIC_BASE + 0x058)
#define    VIC_IRQ_PENDING_H        (VIC_BASE + 0x05C)
#define    VIC_FIQ_PENDING_L        (VIC_BASE + 0x060)
#define    VIC_FIQ_PENDING_H        (VIC_BASE + 0x064)
#define    VIC_IRQ_VECTOR_BASE      (VIC_BASE + 0x070)
#define    VIC_FIQ_VECTOR_BASE      (VIC_BASE + 0x074)
#define    VIC_IRQ_VECTOR_NUM       (VIC_BASE + 0x078)
#define    VIC_FIQ_VECTOR_NUM       (VIC_BASE + 0x07C)
#define    VIC_IRQ_VECTOR_ADDR      (VIC_BASE + 0x080)
#define    VIC_FIQ_VECTOR_ADDR      (VIC_BASE + 0x084)
#define    VIC_PROIRTY_MASK         (VIC_BASE + 0x090)
#define    VIC_VECTOR_PROIRTY00     (VIC_BASE + 0x100)
#define    VIC_VECTOR_PROIRTY01     (VIC_BASE + 0x104)
#define    VIC_VECTOR_PROIRTY02     (VIC_BASE + 0x108)
#define    VIC_VECTOR_PROIRTY03     (VIC_BASE + 0x10C)
#define    VIC_VECTOR_PROIRTY04     (VIC_BASE + 0x110)
#define    VIC_VECTOR_PROIRTY05     (VIC_BASE + 0x114)
#define    VIC_VECTOR_PROIRTY06     (VIC_BASE + 0x118)
#define    VIC_VECTOR_PROIRTY07     (VIC_BASE + 0x11C)
#define    VIC_VECTOR_PROIRTY08     (VIC_BASE + 0x120)
#define    VIC_VECTOR_PROIRTY09     (VIC_BASE + 0x124)
#define    VIC_VECTOR_PROIRTY10     (VIC_BASE + 0x128)
#define    VIC_VECTOR_PROIRTY11     (VIC_BASE + 0x12C)
#define    VIC_VECTOR_PROIRTY12     (VIC_BASE + 0x130)
#define    VIC_VECTOR_PROIRTY13     (VIC_BASE + 0x134)
#define    VIC_VECTOR_PROIRTY14     (VIC_BASE + 0x138)
#define    VIC_VECTOR_PROIRTY15     (VIC_BASE + 0x13C)
#define    VIC_VECTOR_PROIRTY16     (VIC_BASE + 0x140)
#define    VIC_VECTOR_PROIRTY17     (VIC_BASE + 0x144)
#define    VIC_VECTOR_PROIRTY18     (VIC_BASE + 0x148)
#define    VIC_VECTOR_PROIRTY19     (VIC_BASE + 0x14C)
#define    VIC_VECTOR_PROIRTY20     (VIC_BASE + 0x150)
#define    VIC_VECTOR_PROIRTY21     (VIC_BASE + 0x154)
#define    VIC_VECTOR_PROIRTY22     (VIC_BASE + 0x158)
#define    VIC_VECTOR_PROIRTY23     (VIC_BASE + 0x15C)
#define    VIC_VECTOR_PROIRTY24     (VIC_BASE + 0x160)
#define    VIC_VECTOR_PROIRTY25     (VIC_BASE + 0x164)
#define    VIC_VECTOR_PROIRTY26     (VIC_BASE + 0x168)
#define    VIC_VECTOR_PROIRTY27     (VIC_BASE + 0x16C)
#define    VIC_VECTOR_PROIRTY28     (VIC_BASE + 0x170)
#define    VIC_VECTOR_PROIRTY29     (VIC_BASE + 0x174)
#define    VIC_VECTOR_PROIRTY30     (VIC_BASE + 0x178)
#define    VIC_VECTOR_PROIRTY31     (VIC_BASE + 0x17C)
#define    VIC_VECTOR_PROIRTY32     (VIC_BASE + 0x180)
#define    VIC_VECTOR_PROIRTY33     (VIC_BASE + 0x184)
#define    VIC_VECTOR_PROIRTY34     (VIC_BASE + 0x188)
#define    VIC_VECTOR_PROIRTY35     (VIC_BASE + 0x18C)
#define    VIC_VECTOR_PROIRTY36     (VIC_BASE + 0x190)
#define    VIC_VECTOR_PROIRTY37     (VIC_BASE + 0x194)
#define    VIC_VECTOR_PROIRTY38     (VIC_BASE + 0x198)
#define    VIC_VECTOR_PROIRTY39     (VIC_BASE + 0x19C)
#define    VIC_VECTOR_PROIRTY40     (VIC_BASE + 0x1A0)
#define    VIC_VECTOR_PROIRTY41     (VIC_BASE + 0x1A4)
#define    VIC_VECTOR_PROIRTY42     (VIC_BASE + 0x1A8)
#define    VIC_VECTOR_PROIRTY43     (VIC_BASE + 0x1AC)
#define    VIC_VECTOR_PROIRTY44     (VIC_BASE + 0x1B0)
#define    VIC_VECTOR_PROIRTY45     (VIC_BASE + 0x1B4)
#define    VIC_VECTOR_PROIRTY46     (VIC_BASE + 0x1B8)
#define    VIC_VECTOR_PROIRTY47     (VIC_BASE + 0x1BC)
#define    VIC_VECTOR_PROIRTY48     (VIC_BASE + 0x1C0)
#define    VIC_VECTOR_PROIRTY49     (VIC_BASE + 0x1C4)
#define    VIC_VECTOR_PROIRTY50     (VIC_BASE + 0x1C8)
#define    VIC_VECTOR_PROIRTY51     (VIC_BASE + 0x1CC)
#define    VIC_VECTOR_PROIRTY52     (VIC_BASE + 0x1D0)
#define    VIC_VECTOR_PROIRTY53     (VIC_BASE + 0x1D4)
#define    VIC_VECTOR_PROIRTY54     (VIC_BASE + 0x1D8)
#define    VIC_VECTOR_PROIRTY55     (VIC_BASE + 0x1DC)
#define    VIC_VECTOR_PROIRTY56     (VIC_BASE + 0x1E0)
#define    VIC_VECTOR_PROIRTY57     (VIC_BASE + 0x1E4)
#define    VIC_VECTOR_PROIRTY58     (VIC_BASE + 0x1E8)
#define    VIC_VECTOR_PROIRTY59     (VIC_BASE + 0x1EC)
#define    VIC_VECTOR_PROIRTY60     (VIC_BASE + 0x1F0)
#define    VIC_VECTOR_PROIRTY61     (VIC_BASE + 0x1F4)
#define    VIC_VECTOR_PROIRTY62     (VIC_BASE + 0x1F8)
#define    VIC_VECTOR_PROIRTY63     (VIC_BASE + 0x1FC)

#endif // VIC_BASE

#ifdef PMU_BASE

#define    PMU_PLL_SET              (PMU_BASE + 0x000)
#define    PMU_APLL_CFG             (PMU_BASE + 0x004)
#define    PMU_MPLL_GFG             (PMU_BASE + 0x008)
#define    PMU_DPLL_CFG             (PMU_BASE + 0x00C)
#define    PMU_PMDR                 (PMU_BASE + 0x010)
#define    PMU_CLK_GT_CFG1          (PMU_BASE + 0x014)
#define    PMU_CLK_GT_CFG2          (PMU_BASE + 0x018)
#define    PMU_PWR_GT_CFG           (PMU_BASE + 0x01C)
#define    PMU_AHB_CLK_CFG          (PMU_BASE + 0x020)
#define    PMU_ARM_CLK_CFG          (PMU_BASE + 0x024)
#define    PMU_DDR_CLK_CFG          (PMU_BASE + 0x028)
#define    PMU_PIX_CLK_CFG          (PMU_BASE + 0x02C)
#define    PMU_GPU_CLK2X_CFG        (PMU_BASE + 0x030)
#define    PMU_DIV_SET              (PMU_BASE + 0x034)
#define    PMU_CRYSTAL_CFG          (PMU_BASE + 0x038)
#define    PMU_MSK_WAKEUP           (PMU_BASE + 0x03C)
#define    PMU_RTCR                 (PMU_BASE + 0x040)
#define    PMU_CLR_WAKEUP           (PMU_BASE + 0x044)
#define    PMU_WAKEUP_TIME          (PMU_BASE + 0x048)
#define    PMU_SLEEP_FLAG           (PMU_BASE + 0x04C)
#define    PMU_WAIT_PWR_SWITCH      (PMU_BASE + 0x050)
#define    PMU_PWR_STATE            (PMU_BASE + 0x054)
#define    PMU_IO_POL_SEL           (PMU_BASE + 0x058)
#define    PMU_PLLLD                (PMU_BASE + 0x05C)
#define    PMU_IC_ENABLE            (PMU_BASE + 0x060)
#define    PMU_IC_TAR               (PMU_BASE + 0x064)
#define    PMU_IC_SCL_LCNT          (PMU_BASE + 0x068)
#define    PMU_IC_SCL_HCNT          (PMU_BASE + 0x06C)
#define    PMU_IC_DATA_CMD          (PMU_BASE + 0x070)
#define    PMU_IC_STATE             (PMU_BASE + 0x074)
#define    PMU_IC_SET               (PMU_BASE + 0x078)
#define    PMU_HA_PWR_OFF_DAT       (PMU_BASE + 0x07C)
#define    PMU_HA_PWR_ON_DAT        (PMU_BASE + 0x080)
#define    PMU_HA_PWR_OFF_DAT_CNT   (PMU_BASE + 0x084)
#define    PMU_HA_PWR_ON_DAT_CNT    (PMU_BASE + 0x088)
#define    PMU_PWR_OFF_TIME         (PMU_BASE + 0x08C)
#define    PMU_PWR_ON_TIME          (PMU_BASE + 0x090)
#define    PMU_POWERON_POL_SEL      (PMU_BASE + 0x094)
#define    PMU_RETURN_ADDR          (PMU_BASE + 0x098)
#define    PMU_INT                  (PMU_BASE + 0x09C)

#endif


// RTC register list
// RTC_BASE 0xB0002000
#ifdef RTC_BASE

#define    RTC_STA_YMD              (RTC_BASE + 0x000)
#define    RTC_STA_HMS              (RTC_BASE + 0x004)
#define    RTC_ALARM                (RTC_BASE + 0x008)
#define    RTC_CTR                  (RTC_BASE + 0x00C)
#define    RTC_INT_EN               (RTC_BASE + 0x010)
#define    RTC_INT_STS              (RTC_BASE + 0x014)
#define    RTC_SAMP                 (RTC_BASE + 0x018)
#define    RTC_WD_CNT               (RTC_BASE + 0x01C)
#define    RTC_CFG_CHK              (RTC_BASE + 0x024)
#define    RTC_TIC_COUNT            (RTC_BASE + 0x040)
#define    RTC_TIC_YMD              (RTC_BASE + 0x044)
#define    RTC_TIC_HMS              (RTC_BASE + 0x048)

#endif // RTC_BASE


// Timer register list
// TIMER_BASE 0xB0003000
// note : timer channel 3~6 do not exist currently
#ifdef TIMER_BASE

#define    TIMERx_BASE(Channel) \
                  (TIMER_BASE+((Channel - 1) * 0x20))
#define    TxLCR(ch)                (TIMERx_BASE(ch)+0x00)
#define    TxCCR(ch)                (TIMERx_BASE(ch)+0x04)
#define    TxCR(ch)                 (TIMERx_BASE(ch)+0x08)
#define    TxISCR(ch)               (TIMERx_BASE(ch)+0x0c)
#define    TxIMSR(ch)               (TIMERx_BASE(ch)+0x10)

#define    TIMER_T1LCR              (TIMER_BASE + 0x000)      
#define    TIMER_T1CCR              (TIMER_BASE + 0x004)      
#define    TIMER_T1CR               (TIMER_BASE + 0x008)      
#define    TIMER_T1ISCR             (TIMER_BASE + 0x00C)      
#define    TIMER_T1IMSR             (TIMER_BASE + 0x010)      
#define    TIMER_T2LCR              (TIMER_BASE + 0x020)      
#define    TIMER_T2CCR              (TIMER_BASE + 0x024)      
#define    TIMER_T2CR               (TIMER_BASE + 0x028)      
#define    TIMER_T2ISCR             (TIMER_BASE + 0x02C)      
#define    TIMER_T2IMSR             (TIMER_BASE + 0x030)      
#define    TIMER_T3LCR              (TIMER_BASE + 0x040)      
#define    TIMER_T3CCR              (TIMER_BASE + 0x044)      
#define    TIMER_T3CR               (TIMER_BASE + 0x048)      
#define    TIMER_T3ISCR             (TIMER_BASE + 0x04C)      
#define    TIMER_T3IMSR             (TIMER_BASE + 0x050)      
#define    TIMER_T3CAPR             (TIMER_BASE + 0x054)      
#define    TIMER_T4LCR              (TIMER_BASE + 0x060)      
#define    TIMER_T4CCR              (TIMER_BASE + 0x064)      
#define    TIMER_T4CR               (TIMER_BASE + 0x068)      
#define    TIMER_T4ISCR             (TIMER_BASE + 0x06C)      
#define    TIMER_T4IMSR             (TIMER_BASE + 0x070)      
#define    TIMER_T4CAPR             (TIMER_BASE + 0x074)      
#define    TIMER_T5LCR              (TIMER_BASE + 0x080)      
#define    TIMER_T5CCR              (TIMER_BASE + 0x084)      
#define    TIMER_T5CR               (TIMER_BASE + 0x088)      
#define    TIMER_T5ISCR             (TIMER_BASE + 0x08C)      
#define    TIMER_T5IMSR             (TIMER_BASE + 0x090)      
#define    TIMER_T5CAPR             (TIMER_BASE + 0x094)      
#define    TIMER_T6LCR              (TIMER_BASE + 0x0A0)      
#define    TIMER_T6CCR              (TIMER_BASE + 0x0A4)      
#define    TIMER_T6CR               (TIMER_BASE + 0x0A8)      
#define    TIMER_T6ISCR             (TIMER_BASE + 0x0AC)      
#define    TIMER_T6IMSR             (TIMER_BASE + 0x0B0)      
#define    TIMER_T6CAPR             (TIMER_BASE + 0x0B4)      
#define    TIMER_T7LCR              (TIMER_BASE + 0x0C0)      
#define    TIMER_T7CCR              (TIMER_BASE + 0x0C4)      
#define    TIMER_T7CR               (TIMER_BASE + 0x0C8)      
#define    TIMER_T7ISCR             (TIMER_BASE + 0x0CC)      
#define    TIMER_T7IMSR             (TIMER_BASE + 0x0D0)      
#define    TIMER_T8LCR              (TIMER_BASE + 0x0E0)      
#define    TIMER_T8CCR              (TIMER_BASE + 0x0E4)      
#define    TIMER_T8CR               (TIMER_BASE + 0x0E8)      
#define    TIMER_T8ISCR             (TIMER_BASE + 0x0EC)      
#define    TIMER_T8IMSR             (TIMER_BASE + 0x0F0)      
#define    TIMER_T9LCR              (TIMER_BASE + 0x100)      
#define    TIMER_T9CCR              (TIMER_BASE + 0x104)      
#define    TIMER_T9CR               (TIMER_BASE + 0x108)      
#define    TIMER_T9ISCR             (TIMER_BASE + 0x10C)      
#define    TIMER_T9IMSR             (TIMER_BASE + 0x110)      
#define    TIMER_T10LCR             (TIMER_BASE + 0x120)      
#define    TIMER_T10CCR             (TIMER_BASE + 0x124)      
#define    TIMER_T10CR              (TIMER_BASE + 0x128)      
#define    TIMER_T10ISCR            (TIMER_BASE + 0x12C)      
#define    TIMER_T10IMSR            (TIMER_BASE + 0x130)      
#define    TIMER_TIMSR              (TIMER_BASE + 0x140)      
#define    TIMER_TISCR              (TIMER_BASE + 0x144)      
#define    TIMER_TISR               (TIMER_BASE + 0x148)      

#endif // TIMER_BASE

// PWM register list
// PWM_BASE 0xB0004000 
// note : do not use the PWM register list currently
#ifdef PWM_BASE

#define    PWM_BASE_CH(x)           (PWM_BASE + (x - 1) * 0x20)
#define    PWM_CTRL_CH(x)           (PWM_BASE_CH(x) + 0x000)        
#define    PWM_DIV_CH(x)            (PWM_BASE_CH(x) + 0x004)        
#define    PWM_PERIOD_CH(x)         (PWM_BASE_CH(x) + 0x008)        
#define    PWM_DATA_CH(x)           (PWM_BASE_CH(x) + 0x00C)        
#define    PWM_CNT_CH(x)            (PWM_BASE_CH(x) + 0x010)        
#define    PWM_STATUS_CH(x)         (PWM_BASE_CH(x) + 0x014)     

#define    PWM1_CTRL                (PWM_BASE + 0x000)        
#define    PWM1_DIV                 (PWM_BASE + 0x004)        
#define    PWM1_PERIOD              (PWM_BASE + 0x008)        
#define    PWM1_DATA                (PWM_BASE + 0x00C)        
#define    PWM1_CNT                 (PWM_BASE + 0x010)        
#define    PWM1_STATUS              (PWM_BASE + 0x014)       
#define    PWM2_CTRL                (PWM_BASE + 0x020)        
#define    PWM2_DIV                 (PWM_BASE + 0x024)        
#define    PWM2_PERIOD              (PWM_BASE + 0x028)        
#define    PWM2_DATA                (PWM_BASE + 0x02C)        
#define    PWM2_CNT                 (PWM_BASE + 0x030)        
#define    PWM2_STATUS              (PWM_BASE + 0x034)        
#define    PWM3_CTRL                (PWM_BASE + 0x040)        
#define    PWM3_DIV                 (PWM_BASE + 0x044)        
#define    PWM3_PERIOD              (PWM_BASE + 0x048)        
#define    PWM3_DATA                (PWM_BASE + 0x04C)        
#define    PWM3_CNT                 (PWM_BASE + 0x050)        
#define    PWM3_STATUS              (PWM_BASE + 0x054)        
#define    PWM4_CTRL                (PWM_BASE + 0x060)        
#define    PWM4_DIV                 (PWM_BASE + 0x064)        
#define    PWM4_PERIOD              (PWM_BASE + 0x068)        
#define    PWM4_DATA                (PWM_BASE + 0x06C)        
#define    PWM4_CNT                 (PWM_BASE + 0x070)        
#define    PWM4_STATUS              (PWM_BASE + 0x074)        
#define    PWM_INTMASK              (PWM_BASE + 0x080)        
#define    PWM_INT                  (PWM_BASE + 0x084)        
#define    PWM_ENABLE               (PWM_BASE + 0x088)

#endif // PWM_BASE


// GPIO register list
// GPIO_BASE 0xB0006000
#ifdef GPIO_BASE_OLD

#define    GPIO_DBCLK_DIV           (GPIO_BASE + 0x000)       
#define    GPIO_PORTA_DIR           (GPIO_BASE + 0x004)       
#define    GPIO_PORTA_SEL           (GPIO_BASE + 0x008)       
#define    GPIO_PORTA_INCTL         (GPIO_BASE + 0x00C)       
#define    GPIO_PORTA_INTRCTL       (GPIO_BASE + 0x010)       
#define    GPIO_PORTA_INTRCLR       (GPIO_BASE + 0x014)       
#define    GPIO_PORTA_DATA          (GPIO_BASE + 0x018)       
#define    GPIO_PORTB_DIR           (GPIO_BASE + 0x01C)       
#define    GPIO_PORTB_SEL           (GPIO_BASE + 0x020)       
#define    GPIO_PORTB_DATA          (GPIO_BASE + 0x024)       
#define    GPIO_PORTC_DIR           (GPIO_BASE + 0x028)       
#define    GPIO_PORTC_SEL           (GPIO_BASE + 0x02C)       
#define    GPIO_PORTC_DATA          (GPIO_BASE + 0x030)       
#define    GPIO_PORTD_DIR           (GPIO_BASE + 0x034)       
#define    GPIO_PORTD_SEL           (GPIO_BASE + 0x038)       
#define    GPIO_PORTD_SPECII        (GPIO_BASE + 0x03C)       
#define    GPIO_PORTD_DATA          (GPIO_BASE + 0x040)       
#define    GPIO_PORTE_DIR           (GPIO_BASE + 0x044)       
#define    GPIO_PORTE_SEL           (GPIO_BASE + 0x048)       
#define    GPIO_PORTE_DATA          (GPIO_BASE + 0x04C)       
#define    GPIO_PORTF_DIR           (GPIO_BASE + 0x050)       
#define    GPIO_PORTF_SEL           (GPIO_BASE + 0x054)       
#define    GPIO_PORTF_INCTL         (GPIO_BASE + 0x058)       
#define    GPIO_PORTF_INTRCTL       (GPIO_BASE + 0x05C)       
#define    GPIO_PORTF_INTRCLR       (GPIO_BASE + 0x060)       
#define    GPIO_PORTF_DATA          (GPIO_BASE + 0x064)       
#define    GPIO_PORTG_DIR           (GPIO_BASE + 0x068)       
#define    GPIO_PORTG_SEL           (GPIO_BASE + 0x06C)       
#define    GPIO_PORTG_DATA          (GPIO_BASE + 0x070)       
#define    GPIO_PORTH_DIR           (GPIO_BASE + 0x07C)       
#define    GPIO_PORTH_SEL           (GPIO_BASE + 0x078)       
#define    GPIO_PORTH_DATA          (GPIO_BASE + 0x07C)       
#define    GPIO_PORTI_DIR           (GPIO_BASE + 0x080)       
#define    GPIO_PORTI_SEL           (GPIO_BASE + 0x084)       
#define    GPIO_PORTI_DATA          (GPIO_BASE + 0x088)

#endif // GPIO_BASE_OLD will not be used

// GPIO register list
// GPIO_BASE 0xB0006000
#ifdef GPIO_BASE

#define    GPIO_DBCLK               (GPIO_BASE + 0x000)

#define    GPIO_PORTA_DIR           (GPIO_BASE + 0x010)
#define    GPIO_PORTA_SEL           (GPIO_BASE + 0x014)
#define    GPIO_PORTA_DATA          (GPIO_BASE + 0x018)
#define    GPIO_PORTA_INTSEL        (GPIO_BASE + 0x01C)
#define    GPIO_PORTA_INTLEL        (GPIO_BASE + 0x020)
#define    GPIO_PORTA_INTPOL        (GPIO_BASE + 0x024)
#define    GPIO_PORTA_INTCLR        (GPIO_BASE + 0x028)
#define    GPIO_PORTA_SECFUN        (GPIO_BASE + 0x02C)

#define    GPIO_PORTB_DIR           (GPIO_BASE + 0x030)
#define    GPIO_PORTB_SEL           (GPIO_BASE + 0x034)
#define    GPIO_PORTB_DATA          (GPIO_BASE + 0x038)
#define    GPIO_PORTB_INTSEL        (GPIO_BASE + 0x03C)
#define    GPIO_PORTB_INTLEL        (GPIO_BASE + 0x040)
#define    GPIO_PORTB_INTPOL        (GPIO_BASE + 0x044)
#define    GPIO_PORTB_INTCLR        (GPIO_BASE + 0x048)
#define    GPIO_PORTB_SECFUN        (GPIO_BASE + 0x04C)

#define    GPIO_PORTC_DIR           (GPIO_BASE + 0x050)
#define    GPIO_PORTC_SEL           (GPIO_BASE + 0x054)
#define    GPIO_PORTC_DATA          (GPIO_BASE + 0x058)
#define    GPIO_PORTC_INTSEL        (GPIO_BASE + 0x05C)
#define    GPIO_PORTC_INTLEL        (GPIO_BASE + 0x060)
#define    GPIO_PORTC_INTPOL        (GPIO_BASE + 0x064)
#define    GPIO_PORTC_INTCLR        (GPIO_BASE + 0x068)
#define    GPIO_PORTC_SECFUN        (GPIO_BASE + 0x06C)

#define    GPIO_PORTD_DIR           (GPIO_BASE + 0x070)
#define    GPIO_PORTD_SEL           (GPIO_BASE + 0x074)
#define    GPIO_PORTD_DATA          (GPIO_BASE + 0x078)
#define    GPIO_PORTD_INTSEL        (GPIO_BASE + 0x07C)
#define    GPIO_PORTD_INTLEL        (GPIO_BASE + 0x080)
#define    GPIO_PORTD_INTPOL        (GPIO_BASE + 0x084)
#define    GPIO_PORTD_INTCLR        (GPIO_BASE + 0x088)
#define    GPIO_PORTD_SECFUN        (GPIO_BASE + 0x08C)

#define    GPIO_PORTE_DIR           (GPIO_BASE + 0x090)
#define    GPIO_PORTE_SEL           (GPIO_BASE + 0x094)
#define    GPIO_PORTE_DATA          (GPIO_BASE + 0x098)
#define    GPIO_PORTE_INTSEL        (GPIO_BASE + 0x09C)
#define    GPIO_PORTE_INTLEL        (GPIO_BASE + 0x0A0)
#define    GPIO_PORTE_INTPOL        (GPIO_BASE + 0x0A4)
#define    GPIO_PORTE_INTCLR        (GPIO_BASE + 0x0A8)
#define    GPIO_PORTE_SECFUN        (GPIO_BASE + 0x0AC)

#define    GPIO_PORTF_DIR           (GPIO_BASE + 0x0B0)
#define    GPIO_PORTF_SEL           (GPIO_BASE + 0x0B4)
#define    GPIO_PORTF_DATA          (GPIO_BASE + 0x0B8)
#define    GPIO_PORTF_INTSEL        (GPIO_BASE + 0x0BC)
#define    GPIO_PORTF_INTLEL        (GPIO_BASE + 0x0C0)
#define    GPIO_PORTF_INTPOL        (GPIO_BASE + 0x0C4)
#define    GPIO_PORTF_INTCLR        (GPIO_BASE + 0x0C8)
#define    GPIO_PORTF_SECFUN        (GPIO_BASE + 0x0CC)

#define    GPIO_PORTG_DIR           (GPIO_BASE + 0x0D0)
#define    GPIO_PORTG_SEL           (GPIO_BASE + 0x0D4)
#define    GPIO_PORTG_DATA          (GPIO_BASE + 0x0D8)
#define    GPIO_PORTG_INTSEL        (GPIO_BASE + 0x0DC)
#define    GPIO_PORTG_INTLEL        (GPIO_BASE + 0x0E0)
#define    GPIO_PORTG_INTPOL        (GPIO_BASE + 0x0E4)
#define    GPIO_PORTG_INTCLR        (GPIO_BASE + 0x0E8)
#define    GPIO_PORTG_SECFUN        (GPIO_BASE + 0x0EC)

#define    GPIO_PORTH_DIR           (GPIO_BASE + 0x0F0)
#define    GPIO_PORTH_SEL           (GPIO_BASE + 0x0F4)
#define    GPIO_PORTH_DATA          (GPIO_BASE + 0x0F8)
#define    GPIO_PORTH_INTSEL        (GPIO_BASE + 0x0FC)
#define    GPIO_PORTH_INTLEL        (GPIO_BASE + 0x100)
#define    GPIO_PORTH_INTPOL        (GPIO_BASE + 0x104)
#define    GPIO_PORTH_INTCLR        (GPIO_BASE + 0x108)
#define    GPIO_PORTH_SECFUN        (GPIO_BASE + 0x10C)

#define    GPIO_PORTI_DIR           (GPIO_BASE + 0x110)
#define    GPIO_PORTI_SEL           (GPIO_BASE + 0x114)
#define    GPIO_PORTI_DATA          (GPIO_BASE + 0x118)
#define    GPIO_PORTI_INTSEL        (GPIO_BASE + 0x11C)
#define    GPIO_PORTI_INTLEL        (GPIO_BASE + 0x120)
#define    GPIO_PORTI_INTPOL        (GPIO_BASE + 0x124)
#define    GPIO_PORTI_INTCLR        (GPIO_BASE + 0x128)
#define    GPIO_PORTI_SECFUN        (GPIO_BASE + 0x12C) 

#endif // GPIO_BASE

// universal UART register list
#ifdef UART_BASE

#define    UART_BASE_CH(x)          (UART_BASE + (x * 0x1000))
#define    UART_DLBL_CH(x)          (UART_BASE_CH(x) + 0x000)
#define    UART_RXFIFO_CH(x)        (UART_BASE_CH(x) + 0x000)
#define    UART_TXFIFO_CH(x)        (UART_BASE_CH(x) + 0x000)
#define    UART_DLBH_CH(x)          (UART_BASE_CH(x) + 0x004)
#define    UART_IER_CH(x)           (UART_BASE_CH(x) + 0x004)
#define    UART_IIR_CH(x)           (UART_BASE_CH(x) + 0x008)
#define    UART_FCR_CH(x)           (UART_BASE_CH(x) + 0x008)
#define    UART_LCR_CH(x)           (UART_BASE_CH(x) + 0x00C)
#define    UART_MCR_CH(x)           (UART_BASE_CH(x) + 0x010)
#define    UART_LSR_CH(x)           (UART_BASE_CH(x) + 0x014)
#define    UART_MSR_CH(x)           (UART_BASE_CH(x) + 0x018)

#endif // UART_BASE

// UART0 register list
// UART0_BASE 0xB1000000
/*********************** UART ********************************/
#define    UART_BASE                0xB1000000
#define    UART0_BASE               0xB1000000
#define    UART1_BASE               0xB1001000
#define    UART2_BASE               0xB1002000

//UART0	
#define    UART0_DLBL       (*(volatile unsigned *)0xB1000000)
#define    UART0_RXFIFO     (*(volatile unsigned *)0xB1000000)
#define    UART0_TXFIFO     (*(volatile unsigned *)0xB1000000)
#define    UART0_DLBH       (*(volatile unsigned *)0xB1000004)
#define    UART0_IER        (*(volatile unsigned *)0xB1000004)
#define    UART0_IIR        (*(volatile unsigned *)0xB1000008)
#define    UART0_FCR        (*(volatile unsigned *)0xB1000008)
#define    UART0_LCR        (*(volatile unsigned *)0xB100000C)
#define    UART0_MCR        (*(volatile unsigned *)0xB1000010)
#define    UART0_LSR        (*(volatile unsigned *)0xB1000014)
#define    UART0_MSR        (*(volatile unsigned *)0xB1000018)
	
#define RXFIFO0         (0xB1000000)
#define TXFIFO0         (0xB1000000)
	
#define RdRXFIFO0()     (*(volatile unsigned char *)0xB1000000)
#define WrTXFIFO0(ch)   (*(volatile unsigned char *)0xB1000000) = (unsigned char)(ch)

// UART1 register list
// UART1_BASE 0xB1001000
#ifdef UART1_BASE

#define    UART1_DLBL               (UART1_BASE + 0x000)
#define    UART1_RXFIFO             (UART1_BASE + 0x000)
#define    UART1_TXFIFO             (UART1_BASE + 0x000)
#define    UART1_DLBH               (UART1_BASE + 0x004)
#define    UART1_IER                (UART1_BASE + 0x004)
#define    UART1_IIR                (UART1_BASE + 0x008)
#define    UART1_FCR                (UART1_BASE + 0x008)
#define    UART1_LCR                (UART1_BASE + 0x00C)
#define    UART1_MCR                (UART1_BASE + 0x010)
#define    UART1_LSR                (UART1_BASE + 0x014)
#define    UART1_MSR                (UART1_BASE + 0x018)

#endif // UART1_BASE

// UART2 register list
// UART2_BASE 0xB1002000
#ifdef UART2_BASE

#define    UART2_DLBL               (UART2_BASE + 0x000)
#define    UART2_RXFIFO             (UART2_BASE + 0x000)
#define    UART2_TXFIFO             (UART2_BASE + 0x000)
#define    UART2_DLBH               (UART2_BASE + 0x004)
#define    UART2_IER                (UART2_BASE + 0x004)
#define    UART2_IIR                (UART2_BASE + 0x008)
#define    UART2_FCR                (UART2_BASE + 0x008)
#define    UART2_LCR                (UART2_BASE + 0x00C)
#define    UART2_MCR                (UART2_BASE + 0x010)
#define    UART2_LSR                (UART2_BASE + 0x014)
#define    UART2_MSR                (UART2_BASE + 0x018)

#endif // UART2_BASE


// UART3 register list
// UART3_BASE 0xB1003000
#ifdef UART3_BASE

#define    UART3_DLBL               (UART3_BASE + 0x000)
#define    UART3_RXFIFO             (UART3_BASE + 0x000)
#define    UART3_TXFIFO             (UART3_BASE + 0x000)
#define    UART3_DLBH               (UART3_BASE + 0x004)
#define    UART3_IER                (UART3_BASE + 0x004)
#define    UART3_IIR                (UART3_BASE + 0x008)
#define    UART3_FCR                (UART3_BASE + 0x008)
#define    UART3_LCR                (UART3_BASE + 0x00C)
#define    UART3_MCR                (UART3_BASE + 0x010)
#define    UART3_LSR                (UART3_BASE + 0x014)
#define    UART3_MSR                (UART3_BASE + 0x018)

#endif // UART3_BASE

// universal SSI register list
// SSI_BASE 0xB1004000
#ifdef SSI_BASE

#define    SSI_BASE_CH(x)           (SSI_BASE + (x - 1) * 0x1000)
#define    SSI_CONTROL0_CH(x)       (SSI_BASE_CH(x) + 0x000)
#define    SSI_CONTROL1_CH(x)       (SSI_BASE_CH(x) + 0x004)
#define    SSI_SSIENR_CH(x)         (SSI_BASE_CH(x) + 0x008)
#define    SSI_MWCR_CH(x)           (SSI_BASE_CH(x) + 0x00C)
#define    SSI_SER_CH(x)            (SSI_BASE_CH(x) + 0x010)
#define    SSI_BAUDR_CH(x)          (SSI_BASE_CH(x) + 0x014)
#define    SSI_TXFTLR_CH(x)         (SSI_BASE_CH(x) + 0x018)
#define    SSI_RXFTLR_CH(x)         (SSI_BASE_CH(x) + 0x01C)
#define    SSI_TXFLR_CH(x)          (SSI_BASE_CH(x) + 0x020)
#define    SSI_RXFLR_CH(x)          (SSI_BASE_CH(x) + 0x024)
#define    SSI_SR_CH(x)             (SSI_BASE_CH(x) + 0x028)
#define    SSI_IMR_CH(x)            (SSI_BASE_CH(x) + 0x02C)
#define    SSI_ISR_CH(x)            (SSI_BASE_CH(x) + 0x030)
#define    SSI_RISR_CH(x)           (SSI_BASE_CH(x) + 0x034)
#define    SSI_TXOICR_CH(x)         (SSI_BASE_CH(x) + 0x038)
#define    SSI_RXOICR_CH(x)         (SSI_BASE_CH(x) + 0x03C)
#define    SSI_RXUICR_CH(x)         (SSI_BASE_CH(x) + 0x040)
#define    SSI_ICR_CH(x)            (SSI_BASE_CH(x) + 0x048)
#define    SSI_DMACR_CH(x)          (SSI_BASE_CH(x) + 0x04C)
#define    SSI_DMATDLR_CH(x)        (SSI_BASE_CH(x) + 0x050)
#define    SSI_DMARDLR_CH(x)        (SSI_BASE_CH(x) + 0x054)
#define    SSI_DR_CH(x)             (SSI_BASE_CH(x) + 0x060)

#endif // SSI_BASE

// SSI1 register list
// SSI1_BASE 0xB1004000
#ifdef SSI1_BASE

#define    SSI1_CONTROL0            (SSI1_BASE + 0x000)
#define    SSI1_CONTROL1            (SSI1_BASE + 0x004)
#define    SSI1_SSI1ENR             (SSI1_BASE + 0x008)
#define    SSI1_MWCR                (SSI1_BASE + 0x00C)
#define    SSI1_SER                 (SSI1_BASE + 0x010)
#define    SSI1_BAUDR               (SSI1_BASE + 0x014)
#define    SSI1_TXFTLR              (SSI1_BASE + 0x018)
#define    SSI1_RXFTLR              (SSI1_BASE + 0x01C)
#define    SSI1_TXFLR               (SSI1_BASE + 0x020)
#define    SSI1_RXFLR               (SSI1_BASE + 0x024)
#define    SSI1_SR                  (SSI1_BASE + 0x028)
#define    SSI1_IMR                 (SSI1_BASE + 0x02C)
#define    SSI1_ISR                 (SSI1_BASE + 0x030)
#define    SSI1_RISR                (SSI1_BASE + 0x034)
#define    SSI1_TXOICR              (SSI1_BASE + 0x038)
#define    SSI1_RXOICR              (SSI1_BASE + 0x03C)
#define    SSI1_RXUICR              (SSI1_BASE + 0x040)
#define    SSI1_ICR                 (SSI1_BASE + 0x048)
#define    SSI1_DMACR               (SSI1_BASE + 0x04C)
#define    SSI1_DMATDLR             (SSI1_BASE + 0x050)
#define    SSI1_DMARDLR             (SSI1_BASE + 0x054)
#define    SSI1_DR                  (SSI1_BASE + 0x060)

#endif // SSI1_BASE

// SSI2 register list
// SSI2_BASE 0xB1005000
#ifdef SSI2_BASE

#define    SSI2_CONTROL0            (SSI2_BASE + 0x000)
#define    SSI2_CONTROL1            (SSI2_BASE + 0x004)
#define    SSI2_SSI2ENR             (SSI2_BASE + 0x008)
#define    SSI2_MWCR                (SSI2_BASE + 0x00C)
#define    SSI2_SER                 (SSI2_BASE + 0x010)
#define    SSI2_BAUDR               (SSI2_BASE + 0x014)
#define    SSI2_TXFTLR              (SSI2_BASE + 0x018)
#define    SSI2_RXFTLR              (SSI2_BASE + 0x01C)
#define    SSI2_TXFLR               (SSI2_BASE + 0x020)
#define    SSI2_RXFLR               (SSI2_BASE + 0x024)
#define    SSI2_SR                  (SSI2_BASE + 0x028)
#define    SSI2_IMR                 (SSI2_BASE + 0x02C)
#define    SSI2_ISR                 (SSI2_BASE + 0x030)
#define    SSI2_RISR                (SSI2_BASE + 0x034)
#define    SSI2_TXOICR              (SSI2_BASE + 0x038)
#define    SSI2_RXOICR              (SSI2_BASE + 0x03C)
#define    SSI2_RXUICR              (SSI2_BASE + 0x040)
#define    SSI2_ICR                 (SSI2_BASE + 0x048)
#define    SSI2_DMACR               (SSI2_BASE + 0x04C)
#define    SSI2_DMATDLR             (SSI2_BASE + 0x050)
#define    SSI2_DMARDLR             (SSI2_BASE + 0x054)
#define    SSI2_DR                  (SSI2_BASE + 0x060)

#endif // SSI2_BASE

// SSI3 register list
// SSI3_BASE 0xB1006000
#ifdef SSI3_BASE

#define    SSI3_CONTROL0            (SSI3_BASE + 0x000)
#define    SSI3_CONTROL1            (SSI3_BASE + 0x004)
#define    SSI3_SSI3ENR             (SSI3_BASE + 0x008)
#define    SSI3_MWCR                (SSI3_BASE + 0x00C)
#define    SSI3_SER                 (SSI3_BASE + 0x010)
#define    SSI3_BAUDR               (SSI3_BASE + 0x014)
#define    SSI3_TXFTLR              (SSI3_BASE + 0x018)
#define    SSI3_RXFTLR              (SSI3_BASE + 0x01C)
#define    SSI3_TXFLR               (SSI3_BASE + 0x020)
#define    SSI3_RXFLR               (SSI3_BASE + 0x024)
#define    SSI3_SR                  (SSI3_BASE + 0x028)
#define    SSI3_IMR                 (SSI3_BASE + 0x02C)
#define    SSI3_ISR                 (SSI3_BASE + 0x030)
#define    SSI3_RISR                (SSI3_BASE + 0x034)
#define    SSI3_TXOICR              (SSI3_BASE + 0x038)
#define    SSI3_RXOICR              (SSI3_BASE + 0x03C)
#define    SSI3_RXUICR              (SSI3_BASE + 0x040)
#define    SSI3_ICR                 (SSI3_BASE + 0x048)
#define    SSI3_DMACR               (SSI3_BASE + 0x04C)
#define    SSI3_DMATDLR             (SSI3_BASE + 0x050)
#define    SSI3_DMARDLR             (SSI3_BASE + 0x054)
#define    SSI3_DR                  (SSI3_BASE + 0x060)

#endif // SSI3_BASE

// universal I2C register list
// I2C_BASE 0xB1007000
#ifdef I2C_BASE

#define    I2C_BASE_CH(x)           (I2C_BASE + (x - 1) * 0x1000) 
#define    I2C_TAR_CH(x)            (I2C_BASE_CH(x) + 0x004)
#define    I2C_SAR_CH(x)            (I2C_BASE_CH(x) + 0x008)
#define    I2C_HS_MADDR_CH(x)       (I2C_BASE_CH(x) + 0x00C)
#define    I2C_DATA_CMD_CH(x)       (I2C_BASE_CH(x) + 0x010)
#define    I2C_SS_SCL_HCNT_CH(x)    (I2C_BASE_CH(x) + 0x014)
#define    I2C_SS_SCL_LCNT_CH(x)    (I2C_BASE_CH(x) + 0x018)
#define    I2C_FS_SCL_HCNT_CH(x)    (I2C_BASE_CH(x) + 0x01C)
#define    I2C_FS_SCL_LCNT_CH(x)    (I2C_BASE_CH(x) + 0x020)
#define    I2C_HS_SCL_HCNT_CH(x)    (I2C_BASE_CH(x) + 0x024)
#define    I2C_HS_SCL_LCNT_CH(x)    (I2C_BASE_CH(x) + 0x028)
#define    I2C_INTR_STAR_CH(x)      (I2C_BASE_CH(x) + 0x02C)
#define    I2C_INTR_MASK_CH(x)      (I2C_BASE_CH(x) + 0x030)
#define    I2C_RAW_INTR_STAR_CH(x)  (I2C_BASE_CH(x) + 0x034)
#define    I2C_RX_TL_CH(x)          (I2C_BASE_CH(x) + 0x038)
#define    I2C_TX_TL_CH(x)          (I2C_BASE_CH(x) + 0x03C)
#define    I2C_CLR_INTR_CH(x)       (I2C_BASE_CH(x) + 0x040)
#define    I2C_CLR_RX_UNDER_CH(x)   (I2C_BASE_CH(x) + 0x044)
#define    I2C_CLR_RX_OVER_CH(x)    (I2C_BASE_CH(x) + 0x048)
#define    I2C_CLR_TX_OVER_CH(x)    (I2C_BASE_CH(x) + 0x04C)
#define    I2C_CLR_RD_REQ_CH(x)     (I2C_BASE_CH(x) + 0x050)
#define    I2C_CLR_TX_ABRT_CH(x)    (I2C_BASE_CH(x) + 0x054)
#define    I2C_CLR_RX_DONE_CH(x)    (I2C_BASE_CH(x) + 0x058)
#define    I2C_CLR_ACTIVITY_CH(x)   (I2C_BASE_CH(x) + 0x05C)
#define    I2C_CLR_STOP_DET_CH(x)   (I2C_BASE_CH(x) + 0x060)
#define    I2C_CLR_START_DET_CH(x)  (I2C_BASE_CH(x) + 0x064)
#define    I2C_CLR_GEN_CALL_CH(x)   (I2C_BASE_CH(x) + 0x068)
#define    I2C_ENABLE_CH(x)         (I2C_BASE_CH(x) + 0x06C)
#define    I2C_STATUS_CH(x)         (I2C_BASE_CH(x) + 0x070)
#define    I2C_TXFLR_CH(x)          (I2C_BASE_CH(x) + 0x074)
#define    I2C_RXFLR_CH(x)          (I2C_BASE_CH(x) + 0x078)
#define    I2C_TX_ABRT_SRC_CH(x)    (I2C_BASE_CH(x) + 0x080)
#define    I2C_DMA_CR_CH(x)         (I2C_BASE_CH(x) + 0x088)
#define    I2C_DMA_TDLR_CH(x)       (I2C_BASE_CH(x) + 0x08C)
#define    I2C_DMA_RDLR_CH(x)       (I2C_BASE_CH(x) + 0x090)
#define    I2C_COMP_PARAM1_CH(x)    (I2C_BASE_CH(x) + 0x0F4)
#define    I2C_COMP_VISION_CH(x)    (I2C_BASE_CH(x) + 0x0F8)
#define    I2C_COMP_TYPE_CH(x)      (I2C_BASE_CH(x) + 0x0FC)

#endif // I2C_BASE

// I2C1 register list
// I2C1_BASE 0xB1006000
#ifdef I2C1_BASE

#define    I2C1_CON                 (I2C1_BASE + 0x000)
#define    I2C1_TAR                 (I2C1_BASE + 0x004)
#define    I2C1_SAR                 (I2C1_BASE + 0x008)
#define    I2C1_HS_MADDR            (I2C1_BASE + 0x00C)
#define    I2C1_DATA_CMD            (I2C1_BASE + 0x010)
#define    I2C1_SS_SCL_HCNT         (I2C1_BASE + 0x014)
#define    I2C1_SS_SCL_LCNT         (I2C1_BASE + 0x018)
#define    I2C1_FS_SCL_HCNT         (I2C1_BASE + 0x01C)
#define    I2C1_FS_SCL_LCNT         (I2C1_BASE + 0x020)
#define    I2C1_HS_SCL_HCNT         (I2C1_BASE + 0x024)
#define    I2C1_HS_SCL_LCNT         (I2C1_BASE + 0x028)
#define    I2C1_INTR_STAR           (I2C1_BASE + 0x02C)
#define    I2C1_INTR_MASK           (I2C1_BASE + 0x030)
#define    I2C1_RAW_INTR_STAR       (I2C1_BASE + 0x034)
#define    I2C1_RX_TL               (I2C1_BASE + 0x038)
#define    I2C1_TX_TL               (I2C1_BASE + 0x03C)
#define    I2C1_CLR_INTR            (I2C1_BASE + 0x040)
#define    I2C1_CLR_RX_UNDER        (I2C1_BASE + 0x044)
#define    I2C1_CLR_RX_OVER         (I2C1_BASE + 0x048)
#define    I2C1_CLR_TX_OVER         (I2C1_BASE + 0x04C)
#define    I2C1_CLR_RD_REQ          (I2C1_BASE + 0x050)
#define    I2C1_CLR_TX_ABRT         (I2C1_BASE + 0x054)
#define    I2C1_CLR_RX_DONE         (I2C1_BASE + 0x058)
#define    I2C1_CLR_ACTIVITY        (I2C1_BASE + 0x05C)
#define    I2C1_CLR_STOP_DET        (I2C1_BASE + 0x060)
#define    I2C1_CLR_START_DET       (I2C1_BASE + 0x064)
#define    I2C1_CLR_GEN_CALL        (I2C1_BASE + 0x068)
#define    I2C1_ENABLE              (I2C1_BASE + 0x06C)
#define    I2C1_STATUS              (I2C1_BASE + 0x070)
#define    I2C1_TXFLR               (I2C1_BASE + 0x074)
#define    I2C1_RXFLR               (I2C1_BASE + 0x078)
#define    I2C1_TX_ABRT_SRC         (I2C1_BASE + 0x080)
#define    I2C1_DMA_CR              (I2C1_BASE + 0x088)
#define    I2C1_DMA_TDLR            (I2C1_BASE + 0x08C)
#define    I2C1_DMA_RDLR            (I2C1_BASE + 0x090)
#define    I2C1_COMP_PARAM1         (I2C1_BASE + 0x0F4)
#define    I2C1_COMP_VISION         (I2C1_BASE + 0x0F8)
#define    I2C1_COMP_TYPE           (I2C1_BASE + 0x0FC)

#endif // I2C1_BASE

// I2C2 register list
// I2C2_BASE 0xB1006000
#ifdef I2C2_BASE

#define    I2C2_CON                 (I2C2_BASE + 0x000)
#define    I2C2_TAR                 (I2C2_BASE + 0x004)
#define    I2C2_SAR                 (I2C2_BASE + 0x008)
#define    I2C2_HS_MADDR            (I2C2_BASE + 0x00C)
#define    I2C2_DATA_CMD            (I2C2_BASE + 0x010)
#define    I2C2_SS_SCL_HCNT         (I2C2_BASE + 0x014)
#define    I2C2_SS_SCL_LCNT         (I2C2_BASE + 0x018)
#define    I2C2_FS_SCL_HCNT         (I2C2_BASE + 0x01C)
#define    I2C2_FS_SCL_LCNT         (I2C2_BASE + 0x020)
#define    I2C2_HS_SCL_HCNT         (I2C2_BASE + 0x024)
#define    I2C2_HS_SCL_LCNT         (I2C2_BASE + 0x028)
#define    I2C2_INTR_STAR           (I2C2_BASE + 0x02C)
#define    I2C2_INTR_MASK           (I2C2_BASE + 0x030)
#define    I2C2_RAW_INTR_STAR       (I2C2_BASE + 0x034)
#define    I2C2_RX_TL               (I2C2_BASE + 0x038)
#define    I2C2_TX_TL               (I2C2_BASE + 0x03C)
#define    I2C2_CLR_INTR            (I2C2_BASE + 0x040)
#define    I2C2_CLR_RX_UNDER        (I2C2_BASE + 0x044)
#define    I2C2_CLR_RX_OVER         (I2C2_BASE + 0x048)
#define    I2C2_CLR_TX_OVER         (I2C2_BASE + 0x04C)
#define    I2C2_CLR_RD_REQ          (I2C2_BASE + 0x050)
#define    I2C2_CLR_TX_ABRT         (I2C2_BASE + 0x054)
#define    I2C2_CLR_RX_DONE         (I2C2_BASE + 0x058)
#define    I2C2_CLR_ACTIVITY        (I2C2_BASE + 0x05C)
#define    I2C2_CLR_STOP_DET        (I2C2_BASE + 0x060)
#define    I2C2_CLR_START_DET       (I2C2_BASE + 0x064)
#define    I2C2_CLR_GEN_CALL        (I2C2_BASE + 0x068)
#define    I2C2_ENABLE              (I2C2_BASE + 0x06C)
#define    I2C2_STATUS              (I2C2_BASE + 0x070)
#define    I2C2_TXFLR               (I2C2_BASE + 0x074)
#define    I2C2_RXFLR               (I2C2_BASE + 0x078)
#define    I2C2_TX_ABRT_SRC         (I2C2_BASE + 0x080)
#define    I2C2_DMA_CR              (I2C2_BASE + 0x088)
#define    I2C2_DMA_TDLR            (I2C2_BASE + 0x08C)
#define    I2C2_DMA_RDLR            (I2C2_BASE + 0x090)
#define    I2C2_COMP_PARAM1         (I2C2_BASE + 0x0F4)
#define    I2C2_COMP_VISION         (I2C2_BASE + 0x0F8)
#define    I2C2_COMP_TYPE           (I2C2_BASE + 0x0FC)

#endif // I2C2_BASE

// I2C3 register list
// I2C3_BASE 0xB1006000
#ifdef I2C3_BASE

#define    I2C3_CON                 (I2C3_BASE + 0x000)
#define    I2C3_TAR                 (I2C3_BASE + 0x004)
#define    I2C3_SAR                 (I2C3_BASE + 0x008)
#define    I2C3_HS_MADDR            (I2C3_BASE + 0x00C)
#define    I2C3_DATA_CMD            (I2C3_BASE + 0x010)
#define    I2C3_SS_SCL_HCNT         (I2C3_BASE + 0x014)
#define    I2C3_SS_SCL_LCNT         (I2C3_BASE + 0x018)
#define    I2C3_FS_SCL_HCNT         (I2C3_BASE + 0x01C)
#define    I2C3_FS_SCL_LCNT         (I2C3_BASE + 0x020)
#define    I2C3_HS_SCL_HCNT         (I2C3_BASE + 0x024)
#define    I2C3_HS_SCL_LCNT         (I2C3_BASE + 0x028)
#define    I2C3_INTR_STAR           (I2C3_BASE + 0x02C)
#define    I2C3_INTR_MASK           (I2C3_BASE + 0x030)
#define    I2C3_RAW_INTR_STAR       (I2C3_BASE + 0x034)
#define    I2C3_RX_TL               (I2C3_BASE + 0x038)
#define    I2C3_TX_TL               (I2C3_BASE + 0x03C)
#define    I2C3_CLR_INTR            (I2C3_BASE + 0x040)
#define    I2C3_CLR_RX_UNDER        (I2C3_BASE + 0x044)
#define    I2C3_CLR_RX_OVER         (I2C3_BASE + 0x048)
#define    I2C3_CLR_TX_OVER         (I2C3_BASE + 0x04C)
#define    I2C3_CLR_RD_REQ          (I2C3_BASE + 0x050)
#define    I2C3_CLR_TX_ABRT         (I2C3_BASE + 0x054)
#define    I2C3_CLR_RX_DONE         (I2C3_BASE + 0x058)
#define    I2C3_CLR_ACTIVITY        (I2C3_BASE + 0x05C)
#define    I2C3_CLR_STOP_DET        (I2C3_BASE + 0x060)
#define    I2C3_CLR_START_DET       (I2C3_BASE + 0x064)
#define    I2C3_CLR_GEN_CALL        (I2C3_BASE + 0x068)
#define    I2C3_ENABLE              (I2C3_BASE + 0x06C)
#define    I2C3_STATUS              (I2C3_BASE + 0x070)
#define    I2C3_TXFLR               (I2C3_BASE + 0x074)
#define    I2C3_RXFLR               (I2C3_BASE + 0x078)
#define    I2C3_TX_ABRT_SRC         (I2C3_BASE + 0x080)
#define    I2C3_DMA_CR              (I2C3_BASE + 0x088)
#define    I2C3_DMA_TDLR            (I2C3_BASE + 0x08C)
#define    I2C3_DMA_RDLR            (I2C3_BASE + 0x090)
#define    I2C3_COMP_PARAM1         (I2C3_BASE + 0x0F4)
#define    I2C3_COMP_VISION         (I2C3_BASE + 0x0F8)
#define    I2C3_COMP_TYPE           (I2C3_BASE + 0x0FC)

#endif //I2C3_BASE


// I2S register list
// I2S_BASE 0xB1008000
#ifdef I2S_BASE

#define    I2S_IER                  (I2S_BASE + 0x000)
#define    I2S_IRER                 (I2S_BASE + 0x004)
#define    I2S_ITER                 (I2S_BASE + 0x008)
#define    I2S_CER                  (I2S_BASE + 0x00C)
#define    I2S_CCR                  (I2S_BASE + 0x010)
#define    I2S_RXFFR                (I2S_BASE + 0x014)
#define    I2S_TXFFR                (I2S_BASE + 0x018)
// channel 0
#define    I2S_LRBR0                (I2S_BASE + 0x020)
#define    I2S_LTHR0                (I2S_BASE + 0x020)
#define    I2S_RRBR0                (I2S_BASE + 0x024)
#define    I2S_RTHR0                (I2S_BASE + 0x024)
#define    I2S_RER0                 (I2S_BASE + 0x028)
#define    I2S_TER0                 (I2S_BASE + 0x02C)
#define    I2S_RCR0                 (I2S_BASE + 0x030)
#define    I2S_TCR0                 (I2S_BASE + 0x034)
#define    I2S_ISR0                 (I2S_BASE + 0x038)
#define    I2S_IMR0                 (I2S_BASE + 0x03C)
#define    I2S_ROR0                 (I2S_BASE + 0x040)
#define    I2S_TOR0                 (I2S_BASE + 0x044)
#define    I2S_RFCR0                (I2S_BASE + 0x048)
#define    I2S_TFCR0                (I2S_BASE + 0x04C)
#define    I2S_RFF0                 (I2S_BASE + 0x050)
#define    I2S_TFF0                 (I2S_BASE + 0x054)
// channel 1
#define    I2S_LRBR1                (I2S_BASE + 0x060)
#define    I2S_LTHR1                (I2S_BASE + 0x060)
#define    I2S_RRBR1                (I2S_BASE + 0x064)
#define    I2S_RTHR1                (I2S_BASE + 0x064)
#define    I2S_RER1                 (I2S_BASE + 0x068)
#define    I2S_TER1                 (I2S_BASE + 0x06C)
#define    I2S_RCR1                 (I2S_BASE + 0x070)
#define    I2S_TCR1                 (I2S_BASE + 0x074)
#define    I2S_ISR1                 (I2S_BASE + 0x078)
#define    I2S_IMR1                 (I2S_BASE + 0x07C)
#define    I2S_ROR1                 (I2S_BASE + 0x080)
#define    I2S_TOR1                 (I2S_BASE + 0x084)
#define    I2S_RFCR1                (I2S_BASE + 0x088)
#define    I2S_TFCR1                (I2S_BASE + 0x08C)
#define    I2S_RFF1                 (I2S_BASE + 0x090)
#define    I2S_TFF1                 (I2S_BASE + 0x094)
// channel 2
#define    I2S_LRBR2                (I2S_BASE + 0x0A0)
#define    I2S_LTHR2                (I2S_BASE + 0x0A0)
#define    I2S_RRBR2                (I2S_BASE + 0x0A4)
#define    I2S_RTHR2                (I2S_BASE + 0x0A4)
#define    I2S_RER2                 (I2S_BASE + 0x0A8)
#define    I2S_TER2                 (I2S_BASE + 0x0AC)
#define    I2S_RCR2                 (I2S_BASE + 0x0B0)
#define    I2S_TCR2                 (I2S_BASE + 0x0B4)
#define    I2S_ISR2                 (I2S_BASE + 0x0B8)
#define    I2S_IMR2                 (I2S_BASE + 0x0BC)
#define    I2S_ROR2                 (I2S_BASE + 0x0C0)
#define    I2S_TOR2                 (I2S_BASE + 0x0C4)
#define    I2S_RFCR2                (I2S_BASE + 0x0C8)
#define    I2S_TFCR2                (I2S_BASE + 0x0CC)
#define    I2S_RFF2                 (I2S_BASE + 0x0D0)
#define    I2S_TFF2                 (I2S_BASE + 0x0D4)
// channel 3
#define    I2S_LRBR3                (I2S_BASE + 0x0E0)
#define    I2S_LTHR3                (I2S_BASE + 0x0E0)
#define    I2S_RRBR3                (I2S_BASE + 0x0E4)
#define    I2S_RTHR3                (I2S_BASE + 0x0E4)
#define    I2S_RER3                 (I2S_BASE + 0x0E8)
#define    I2S_TER3                 (I2S_BASE + 0x0EC)
#define    I2S_RCR3                 (I2S_BASE + 0x0F0)
#define    I2S_TCR3                 (I2S_BASE + 0x0F4)
#define    I2S_ISR3                 (I2S_BASE + 0x0F8)
#define    I2S_IMR3                 (I2S_BASE + 0x0FC)
#define    I2S_ROR3                 (I2S_BASE + 0x100)
#define    I2S_TOR3                 (I2S_BASE + 0x104)
#define    I2S_RFCR3                (I2S_BASE + 0x108)
#define    I2S_TFCR3                (I2S_BASE + 0x10C)
#define    I2S_RFF3                 (I2S_BASE + 0x110)
#define    I2S_TFF3                 (I2S_BASE + 0x114)
#define    I2S_SCR                  (I2S_BASE + 0x118)
// I2S DMA CTL REG
#define    I2S_RXDMA                (I2S_BASE + 0x1C0)
#define    I2S_RRXDMA               (I2S_BASE + 0x1C4)
#define    I2S_TXDMA                (I2S_BASE + 0x1C8)
#define    I2S_RTXDMA               (I2S_BASE + 0x1CC)
// CONSTANT REG
#define    I2S_COMP_PARAM2          (I2S_BASE + 0x1F0)
#define    I2S_COMP_PARAM1          (I2S_BASE + 0x1F4)
#define    I2S_COMP_VERSION         (I2S_BASE + 0x1F8)
#define    I2S_COMP_TYPE            (I2S_BASE + 0x1FC) 

#endif // I2S_BASE


// NAND Controller register list
// NAND_BASE 0x32021000
#ifdef NAND_BASE

#define    NAND_CFG                 (NAND_BASE + 0x000)
#define    NAND_CMD                 (NAND_BASE + 0x004)
#define    NAND_ADDR                (NAND_BASE + 0x008)
#define    NAND_SDATA               (NAND_BASE + 0x00C)
#define    NAND_CTRL                (NAND_BASE + 0x010)
#define    NAND_INIT                (NAND_BASE + 0x014)
#define    NAND_DMA                 (NAND_BASE + 0x018)
#define    NAND_STATUS              (NAND_BASE + 0x01C)
#define    NAND_ECC_PARITY0         (NAND_BASE + 0x020)
#define    NAND_ECC_PARITY1         (NAND_BASE + 0x024)
#define    NAND_ECC_PARITY2         (NAND_BASE + 0x028)
#define    NAND_ECC_PARITY3         (NAND_BASE + 0x02C)
#define    NAND_ECC_PARITY4         (NAND_BASE + 0x030)
#define    NAND_ECC_PARITY5         (NAND_BASE + 0x034)
#define    NAND_ECC_PARITY6         (NAND_BASE + 0x038)
#define    NAND_ECC_PARITY7         (NAND_BASE + 0x03C)
#define    NAND_ECC_PARITY8         (NAND_BASE + 0x040)
#define    NAND_ECC_PARITY9         (NAND_BASE + 0x044)
#define    NAND_ECC_PARITY10        (NAND_BASE + 0x048)
#define    NAND_ECC_PARITY11        (NAND_BASE + 0x04C)
#define    NAND_ECC_PARITY12        (NAND_BASE + 0x050)
#define    NAND_ECC_PARITY13        (NAND_BASE + 0x054)
#define    NAND_ERR_ADDR0           (NAND_BASE + 0x058)
#define    NAND_ERR_ADDR1           (NAND_BASE + 0x05C)
#define    NAND_ERR_ADDR2           (NAND_BASE + 0x060)
#define    NAND_ERR_ADDR3           (NAND_BASE + 0x064)
#define    NAND_ERR_ADDR4           (NAND_BASE + 0x068)
#define    NAND_ERR_ADDR5           (NAND_BASE + 0x06C)
#define    NAND_ERR_ADDR6           (NAND_BASE + 0x070)
#define    NAND_ERR_ADDR7           (NAND_BASE + 0x074)
#define    NAND_ERR_ADDR8           (NAND_BASE + 0x078)
#define    NAND_ERR_ADDR9           (NAND_BASE + 0x07C)
#define    NAND_ERR_ADDR10          (NAND_BASE + 0x080)
#define    NAND_ERR_ADDR11          (NAND_BASE + 0x084)
#define    NAND_ERR_ADDR12          (NAND_BASE + 0x088)
#define    NAND_ERR_ADDR13          (NAND_BASE + 0x08C)
#define    NAND_ERR_ADDR14          (NAND_BASE + 0x090)
#define    NAND_ERR_ADDR15          (NAND_BASE + 0x094)
#define    NAND_ERR_ADDR16          (NAND_BASE + 0x098)
#define    NAND_ERR_ADDR17          (NAND_BASE + 0x09C)
#define    NAND_ERR_ADDR18          (NAND_BASE + 0x0A0)
#define    NAND_ERR_ADDR19          (NAND_BASE + 0x0A4)
#define    NAND_ERR_ADDR20          (NAND_BASE + 0x0A8)
#define    NAND_ERR_ADDR21          (NAND_BASE + 0x0AC)
#define    NAND_ERR_ADDR22          (NAND_BASE + 0x0B0)
#define    NAND_ERR_ADDR23          (NAND_BASE + 0x0B4)
#define    NAND_ERR_ADDR24          (NAND_BASE + 0x0B8)
#define    NAND_ERR_ADDR25          (NAND_BASE + 0x0BC)
#define    NAND_ERR_ADDR26          (NAND_BASE + 0x0C0)
#define    NAND_ERR_ADDR27          (NAND_BASE + 0x0C4)
#define    NAND_ERR_ADDR28          (NAND_BASE + 0x0C8)
#define    NAND_ERR_ADDR29          (NAND_BASE + 0x0CC)
#define    NAND_FDATA               (NAND_BASE + 0x100)

#endif

// SDIO register list
// SDIO_BASE 0x32022000
#ifdef SDIO_BASE

#define    SDIO_BASE_CH(x)          (SDIO_BASE + (x - 1) * 0x1000)
#define    SDIO_CTRL_CH(x)          (SDIO_BASE_CH(x) + 0x000)
#define    SDIO_PWREN_CH(x)         (SDIO_BASE_CH(x) + 0x004)
#define    SDIO_CLKDIV_CH(x)        (SDIO_BASE_CH(x) + 0x008)
#define    SDIO_CLKSRC_CH(x)        (SDIO_BASE_CH(x) + 0x00C)
#define    SDIO_CLKENA_CH(x)        (SDIO_BASE_CH(x) + 0x010)
#define    SDIO_TMOUT_CH(x)         (SDIO_BASE_CH(x) + 0x014)
#define    SDIO_CTYPE_CH(x)         (SDIO_BASE_CH(x) + 0x018)
#define    SDIO_BLKSIZ_CH(x)        (SDIO_BASE_CH(x) + 0x01C)
#define    SDIO_BYTCNT_CH(x)        (SDIO_BASE_CH(x) + 0x020)
#define    SDIO_INTMASK_CH(x)       (SDIO_BASE_CH(x) + 0x024)
#define    SDIO_CMDARG_CH(x)        (SDIO_BASE_CH(x) + 0x028)
#define    SDIO_CMD_CH(x)           (SDIO_BASE_CH(x) + 0x02C)
#define    SDIO_RESP0_CH(x)         (SDIO_BASE_CH(x) + 0x030)
#define    SDIO_RESP1_CH(x)         (SDIO_BASE_CH(x) + 0x034)
#define    SDIO_RESP2_CH(x)         (SDIO_BASE_CH(x) + 0x038)
#define    SDIO_RESP3_CH(x)         (SDIO_BASE_CH(x) + 0x03C)
#define    SDIO_MINTSTS_CH(x)       (SDIO_BASE_CH(x) + 0x040)
#define    SDIO_RINTSTS_CH(x)       (SDIO_BASE_CH(x) + 0x044)
#define    SDIO_STATUS_CH(x)        (SDIO_BASE_CH(x) + 0x048)
#define    SDIO_FIFOTH_CH(x)        (SDIO_BASE_CH(x) + 0x04C)
#define    SDIO_CDETECT_CH(x)       (SDIO_BASE_CH(x) + 0x050)
#define    SDIO_WRTPRT_CH(x)        (SDIO_BASE_CH(x) + 0x054)
#define    SDIO_GPIO_CH(x)          (SDIO_BASE_CH(x) + 0x058)
#define    SDIO_TCBCNT_CH(x)        (SDIO_BASE_CH(x) + 0x05C)
#define    SDIO_TBBCNT_CH(x)        (SDIO_BASE_CH(x) + 0x060)
#define    SDIO_DEBNCE_CH(x)        (SDIO_BASE_CH(x) + 0x064)
#define    SDIO_USRID_CH(x)         (SDIO_BASE_CH(x) + 0x068)
#define    SDIO_VERID_CH(x)         (SDIO_BASE_CH(x) + 0x06C)
#define    SDIO_HCON_CH(x)          (SDIO_BASE_CH(x) + 0x070)
#define    SDIO_APBCLKDIV_CH(x)     (SDIO_BASE_CH(x) + 0x074)
#define    SDIO_BMOD_CH(x)          (SDIO_BASE_CH(x) + 0x080)
#define    SDIO_PLDMND_CH(x)        (SDIO_BASE_CH(x) + 0x084)
#define    SDIO_DBADDR_CH(x)        (SDIO_BASE_CH(x) + 0x088)
#define    SDIO_IDSTS_CH(x)         (SDIO_BASE_CH(x) + 0x08C)
#define    SDIO_IDINTEN_CH(x)       (SDIO_BASE_CH(x) + 0x090)
#define    SDIO_DSCADDR_CH(x)       (SDIO_BASE_CH(x) + 0x094)
#define    SDIO_BUFADDR_CH(x)       (SDIO_BASE_CH(x) + 0x098)
#define    SDIO_AHBCLKDIV_CH(x)     (SDIO_BASE_CH(x) +0x0A0)
#define    SDIO_DATA_CH(x)          (SDIO_BASE_CH(x) + 0x100)

#endif // SDIO_BASE

// SDIO1 register list
// SDIO1_BASE 0x32022000
#ifdef SDIO1_BASE

#define    SDIO1_CTRL               (SDIO1_BASE + 0x000)
#define    SDIO1_PWREN              (SDIO1_BASE + 0x004)
#define    SDIO1_CLKDIV             (SDIO1_BASE + 0x008)
#define    SDIO1_CLKSRC             (SDIO1_BASE + 0x00C)
#define    SDIO1_CLKENA             (SDIO1_BASE + 0x010)
#define    SDIO1_TMOUT              (SDIO1_BASE + 0x014)
#define    SDIO1_CTYPE              (SDIO1_BASE + 0x018)
#define    SDIO1_BLKSIZ             (SDIO1_BASE + 0x01C)
#define    SDIO1_BYTCNT             (SDIO1_BASE + 0x020)
#define    SDIO1_INTMASK            (SDIO1_BASE + 0x024)
#define    SDIO1_CMDARG             (SDIO1_BASE + 0x028)
#define    SDIO1_CMD                (SDIO1_BASE + 0x02C)
#define    SDIO1_RESP0              (SDIO1_BASE + 0x030)
#define    SDIO1_RESP1              (SDIO1_BASE + 0x034)
#define    SDIO1_RESP2              (SDIO1_BASE + 0x038)
#define    SDIO1_RESP3              (SDIO1_BASE + 0x03C)
#define    SDIO1_MINTSTS            (SDIO1_BASE + 0x040)
#define    SDIO1_RINTSTS            (SDIO1_BASE + 0x044)
#define    SDIO1_STATUS             (SDIO1_BASE + 0x048)
#define    SDIO1_FIFOTH             (SDIO1_BASE + 0x04C)
#define    SDIO1_CDETECT            (SDIO1_BASE + 0x050)
#define    SDIO1_WRTPRT             (SDIO1_BASE + 0x054)
#define    SDIO1_GPIO               (SDIO1_BASE + 0x058)
#define    SDIO1_TCBCNT             (SDIO1_BASE + 0x05C)
#define    SDIO1_TBBCNT             (SDIO1_BASE + 0x060)
#define    SDIO1_DEBNCE             (SDIO1_BASE + 0x064)
#define    SDIO1_USRID              (SDIO1_BASE + 0x068)
#define    SDIO1_VERID              (SDIO1_BASE + 0x06C)
#define    SDIO1_HCON               (SDIO1_BASE + 0x070)
#define    SDIO1_APBCLKDIV          (SDIO1_BASE + 0x074)
#define    SDIO1_BMOD               (SDIO1_BASE + 0x080)
#define    SDIO1_PLDMND             (SDIO1_BASE + 0x084)
#define    SDIO1_DBADDR             (SDIO1_BASE + 0x088)
#define    SDIO1_IDSTS              (SDIO1_BASE + 0x08C)
#define    SDIO1_IDINTEN            (SDIO1_BASE + 0x090)
#define    SDIO1_DSCADDR            (SDIO1_BASE + 0x094)
#define    SDIO1_BUFADDR            (SDIO1_BASE + 0x098)
#define    SDIO1_AHBCLKDIV          (SDIO1_BASE + 0x0A0)
#define    SDIO1_DATA               (SDIO1_BASE + 0x100)
#define    SDIO1_DATA_BE            (SDIO1_BASE + 0x800)

#endif // SDIO1_BASE

// SDIO2 register list
// SDIO2_BASE 0x32023000
#ifdef SDIO2_BASE

#define    SDIO2_CTRL               (SDIO2_BASE + 0x000)
#define    SDIO2_PWREN              (SDIO2_BASE + 0x004)
#define    SDIO2_CLKDIV             (SDIO2_BASE + 0x008)
#define    SDIO2_CLKSRC             (SDIO2_BASE + 0x00C)
#define    SDIO2_CLKENA             (SDIO2_BASE + 0x010)
#define    SDIO2_TMOUT              (SDIO2_BASE + 0x014)
#define    SDIO2_CTYPE              (SDIO2_BASE + 0x018)
#define    SDIO2_BLKSIZ             (SDIO2_BASE + 0x01C)
#define    SDIO2_BYTCNT             (SDIO2_BASE + 0x020)
#define    SDIO2_INTMASK            (SDIO2_BASE + 0x024)
#define    SDIO2_CMDARG             (SDIO2_BASE + 0x028)
#define    SDIO2_CMD                (SDIO2_BASE + 0x02C)
#define    SDIO2_RESP0              (SDIO2_BASE + 0x030)
#define    SDIO2_RESP1              (SDIO2_BASE + 0x034)
#define    SDIO2_RESP2              (SDIO2_BASE + 0x038)
#define    SDIO2_RESP3              (SDIO2_BASE + 0x03C)
#define    SDIO2_MINTSTS            (SDIO2_BASE + 0x040)
#define    SDIO2_RINTSTS            (SDIO2_BASE + 0x044)
#define    SDIO2_STATUS             (SDIO2_BASE + 0x048)
#define    SDIO2_FIFOTH             (SDIO2_BASE + 0x04C)
#define    SDIO2_CDETECT            (SDIO2_BASE + 0x050)
#define    SDIO2_WRTPRT             (SDIO2_BASE + 0x054)
#define    SDIO2_GPIO               (SDIO2_BASE + 0x058)
#define    SDIO2_TCBCNT             (SDIO2_BASE + 0x05C)
#define    SDIO2_TBBCNT             (SDIO2_BASE + 0x060)
#define    SDIO2_DEBNCE             (SDIO2_BASE + 0x064)
#define    SDIO2_USRID              (SDIO2_BASE + 0x068)
#define    SDIO2_VERID              (SDIO2_BASE + 0x06C)
#define    SDIO2_HCON               (SDIO2_BASE + 0x070)
#define    SDIO2_APBCLKDIV          (SDIO2_BASE + 0x074)
#define    SDIO2_BMOD               (SDIO2_BASE + 0x080)
#define    SDIO2_PLDMND             (SDIO2_BASE + 0x084)
#define    SDIO2_DBADDR             (SDIO2_BASE + 0x088)
#define    SDIO2_IDSTS              (SDIO2_BASE + 0x08C)
#define    SDIO2_IDINTEN            (SDIO2_BASE + 0x090)
#define    SDIO2_DSCADDR            (SDIO2_BASE + 0x094)
#define    SDIO2_BUFADDR            (SDIO2_BASE + 0x098)
#define    SDIO2_AHBCLKDIV          (SDIO2_BASE + 0x0A0)
#define    SDIO2_DATA               (SDIO2_BASE + 0x100) // 0x100 ~ 0x7ff
#define    SDIO1_DATA_BE            (SDIO1_BASE + 0x800) // 0x800 ~ 0xfff

#endif // SDIO2_BASE

// SDIO3 register list
// SDIO3_BASE 0x32024000
#ifdef SDIO3_BASE

#define    SDIO3_CTRL               (SDIO3_BASE + 0x000)
#define    SDIO3_PWREN              (SDIO3_BASE + 0x004)
#define    SDIO3_CLKDIV             (SDIO3_BASE + 0x008)
#define    SDIO3_CLKSRC             (SDIO3_BASE + 0x00C)
#define    SDIO3_CLKENA             (SDIO3_BASE + 0x010)
#define    SDIO3_TMOUT              (SDIO3_BASE + 0x014)
#define    SDIO3_CTYPE              (SDIO3_BASE + 0x018)
#define    SDIO3_BLKSIZ             (SDIO3_BASE + 0x01C)
#define    SDIO3_BYTCNT             (SDIO3_BASE + 0x020)
#define    SDIO3_INTMASK            (SDIO3_BASE + 0x024)
#define    SDIO3_CMDARG             (SDIO3_BASE + 0x028)
#define    SDIO3_CMD                (SDIO3_BASE + 0x02C)
#define    SDIO3_RESP0              (SDIO3_BASE + 0x030)
#define    SDIO3_RESP1              (SDIO3_BASE + 0x034)
#define    SDIO3_RESP2              (SDIO3_BASE + 0x038)
#define    SDIO3_RESP3              (SDIO3_BASE + 0x03C)
#define    SDIO3_MINTSTS            (SDIO3_BASE + 0x040)
#define    SDIO3_RINTSTS            (SDIO3_BASE + 0x044)
#define    SDIO3_STATUS             (SDIO3_BASE + 0x048)
#define    SDIO3_FIFOTH             (SDIO3_BASE + 0x04C)
#define    SDIO3_CDETECT            (SDIO3_BASE + 0x050)
#define    SDIO3_WRTPRT             (SDIO3_BASE + 0x054)
#define    SDIO3_GPIO               (SDIO3_BASE + 0x058)
#define    SDIO3_TCBCNT             (SDIO3_BASE + 0x05C)
#define    SDIO3_TBBCNT             (SDIO3_BASE + 0x060)
#define    SDIO3_DEBNCE             (SDIO3_BASE + 0x064)
#define    SDIO3_USRID              (SDIO3_BASE + 0x068)
#define    SDIO3_VERID              (SDIO3_BASE + 0x06C)
#define    SDIO3_HCON               (SDIO3_BASE + 0x070)
#define    SDIO3_APBCLKDIV          (SDIO3_BASE + 0x074)
#define    SDIO3_BMOD               (SDIO3_BASE + 0x080)
#define    SDIO3_PLDMND             (SDIO3_BASE + 0x084)
#define    SDIO3_DBADDR             (SDIO3_BASE + 0x088)
#define    SDIO3_IDSTS              (SDIO3_BASE + 0x08C)
#define    SDIO3_IDINTEN            (SDIO3_BASE + 0x090)
#define    SDIO3_DSCADDR            (SDIO3_BASE + 0x094)
#define    SDIO3_BUFADDR            (SDIO3_BASE + 0x098)
#define    SDIO3_DATA               (SDIO3_BASE + 0x100)

#endif // SDIO3_BASE


// NOR controller register list
// NORCTL_BASE 0x32020000
#define    SDRAM_CONFIG_REG         (NORCTL_BASE + 0x000)
#define    SDRAM_TIMING_REG1        (NORCTL_BASE + 0x004)
#define    SDRAM_TIMING_REG2        (NORCTL_BASE + 0x008)
#define    SDRAM_CONTROL_REG        (NORCTL_BASE + 0x00c)
#define    SDRAM_REFRESH_REG        (NORCTL_BASE + 0x010)
#define    CSA_BASE_ADDR            (NORCTL_BASE + 0x014)
#define    CSE_BASE_ADDR            (NORCTL_BASE + 0x018)
#define    CSB_BASE_ADDR            (NORCTL_BASE + 0x01c)
#define    CSC_BASE_ADDR            (NORCTL_BASE + 0x020)
#define    CSA_MASK_REG             (NORCTL_BASE + 0x054)
#define    CSE_MASK_REG             (NORCTL_BASE + 0x058)
#define    CSB_MASK_REG             (NORCTL_BASE + 0x05C)
#define    CSC_MASK_REG             (NORCTL_BASE + 0x060)
#define    CSA_ALIAS_ADDR           (NORCTL_BASE + 0x074)
#define    CSE_ALIAS_ADDR           (NORCTL_BASE + 0x078)
#define    CSA_REMAP_ADDR           (NORCTL_BASE + 0x084)
#define    CSE_REMAP_ADDR           (NORCTL_BASE + 0x088)
#define    SM_TIMING_SET1           (NORCTL_BASE + 0x094)
#define    SM_TIMING_SET2           (NORCTL_BASE + 0x098)
#define    SM_TIMING_SET3           (NORCTL_BASE + 0x09C)
#define    SM_WIDTH_REG             (NORCTL_BASE + 0x0a4)
#define    DDR_EXT_MODE_REG         (NORCTL_BASE + 0x0aC)
#define    SDRAM_TYPE_REG           (NORCTL_BASE + 0x0B0)
#define    REMAP_REG_ADDR           (NORCTL_BASE + 0x0B4)

// DMAC register define
#ifdef DMAC_BASE

#define    DMAC_BASE_NUM(x)         ((x == 1)? DMAC1_BASE : DMAC2_BASE)

// DMAC(x) channel(n)
#define    DMAC_SAR(x, n)           (DMAC_BASE_NUM(x) + (0x58 * n) + 0x000)
#define    DMAC_DAR(x, n)           (DMAC_BASE_NUM(x) + (0x58 * n) + 0x008)
#define    DMAC_LLP(x, n)           (DMAC_BASE_NUM(x) + (0x58 * n) + 0x010)
#define    DMAC_CTL(x, n)           (DMAC_BASE_NUM(x) + (0x58 * n) + 0x018)
#define    DMAC_SSTAT(x, n)         (DMAC_BASE_NUM(x) + (0x58 * n) + 0x020)
#define    DMAC_DSTAT(x, n)         (DMAC_BASE_NUM(x) + (0x58 * n) + 0x028)
#define    DMAC_SSTATR(x, n)        (DMAC_BASE_NUM(x) + (0x58 * n) + 0x030)
#define    DMAC_DSTATR(x, n)        (DMAC_BASE_NUM(x) + (0x58 * n) + 0x038)
#define    DMAC_CH_CFG(x, n)        (DMAC_BASE_NUM(x) + (0x58 * n) + 0x040)
#define    DMAC_SGR(x, n)           (DMAC_BASE_NUM(x) + (0x58 * n) + 0x048)
#define    DMAC_DSR(x, n)           (DMAC_BASE_NUM(x) + (0x58 * n) + 0x050)

// DMAC channel 0
#define    DMAC_SAR0_NUM(x)         (DMAC_BASE_NUM(x) + 0x000)
#define    DMAC_DAR0_NUM(x)         (DMAC_BASE_NUM(x) + 0x008)
#define    DMAC_LLP0_NUM(x)         (DMAC_BASE_NUM(x) + 0x010)
#define    DMAC_CTL0_NUM(x)         (DMAC_BASE_NUM(x) + 0x018)
#define    DMAC_SSTAT0_NUM(x)       (DMAC_BASE_NUM(x) + 0x020)
#define    DMAC_DSTAT0_NUM(x)       (DMAC_BASE_NUM(x) + 0x028)
#define    DMAC_SSTATR0_NUM(x)      (DMAC_BASE_NUM(x) + 0x030)
#define    DMAC_DSTATR0_NUM(x)      (DMAC_BASE_NUM(x) + 0x038)
#define    DMAC_CFG0_NUM(x)         (DMAC_BASE_NUM(x) + 0x040)
#define    DMAC_SGR0_NUM(x)         (DMAC_BASE_NUM(x) + 0x048)
#define    DMAC_DSR0_NUM(x)         (DMAC_BASE_NUM(x) + 0x050)

// DMAC channel 1
#define    DMAC_SAR1_NUM(x)         (DMAC_BASE_NUM(x) + 0x058)
#define    DMAC_DAR1_NUM(x)         (DMAC_BASE_NUM(x) + 0x060)
#define    DMAC_LLP1_NUM(x)         (DMAC_BASE_NUM(x) + 0x068)
#define    DMAC_CTL1_NUM(x)         (DMAC_BASE_NUM(x) + 0x070)
#define    DMAC_SSTAT1_NUM(x)       (DMAC_BASE_NUM(x) + 0x078)
#define    DMAC_DSTAT1_NUM(x)       (DMAC_BASE_NUM(x) + 0x080)
#define    DMAC_SSTATR1_NUM(x)      (DMAC_BASE_NUM(x) + 0x088)
#define    DMAC_DSTATR1_NUM(x)      (DMAC_BASE_NUM(x) + 0x090)
#define    DMAC_CFG1_NUM(x)         (DMAC_BASE_NUM(x) + 0x098)
#define    DMAC_SGR1_NUM(x)         (DMAC_BASE_NUM(x) + 0x0A0)
#define    DMAC_DSR1_NUM(x)         (DMAC_BASE_NUM(x) + 0x0A8)

// DMAC channel 2
#define    DMAC_SAR2_NUM(x)         (DMAC_BASE_NUM(x) + 0x0B0)
#define    DMAC_DAR2_NUM(x)         (DMAC_BASE_NUM(x) + 0x0B8)
#define    DMAC_LLP2_NUM(x)         (DMAC_BASE_NUM(x) + 0x0C0)
#define    DMAC_CTL2_NUM(x)         (DMAC_BASE_NUM(x) + 0x0C8)
#define    DMAC_SSTAT2_NUM(x)       (DMAC_BASE_NUM(x) + 0x0D0)
#define    DMAC_DSTAT2_NUM(x)       (DMAC_BASE_NUM(x) + 0x0D8)
#define    DMAC_SSTATR2_NUM(x)      (DMAC_BASE_NUM(x) + 0x0E0)
#define    DMAC_DSTATR2_NUM(x)      (DMAC_BASE_NUM(x) + 0x0E8)
#define    DMAC_CFG2_NUM(x)         (DMAC_BASE_NUM(x) + 0x0F0)
#define    DMAC_SGR2_NUM(x)         (DMAC_BASE_NUM(x) + 0x0F8)
#define    DMAC_DSR2_NUM(x)         (DMAC_BASE_NUM(x) + 0x100)

// DMAC channel 3
#define    DMAC_SAR3_NUM(x)         (DMAC_BASE_NUM(x) + 0x108)
#define    DMAC_DAR3_NUM(x)         (DMAC_BASE_NUM(x) + 0x110)
#define    DMAC_LLP3_NUM(x)         (DMAC_BASE_NUM(x) + 0x118)
#define    DMAC_CTL3_NUM(x)         (DMAC_BASE_NUM(x) + 0x120)
#define    DMAC_SSTAT3_NUM(x)       (DMAC_BASE_NUM(x) + 0x128)
#define    DMAC_DSTAT3_NUM(x)       (DMAC_BASE_NUM(x) + 0x130)
#define    DMAC_SSTATR3_NUM(x)      (DMAC_BASE_NUM(x) + 0x138)
#define    DMAC_DSTATR3_NUM(x)      (DMAC_BASE_NUM(x) + 0x140)
#define    DMAC_CFG3_NUM(x)         (DMAC_BASE_NUM(x) + 0x148)
#define    DMAC_SGR3_NUM(x)         (DMAC_BASE_NUM(x) + 0x150)
#define    DMAC_DSR3_NUM(x)         (DMAC_BASE_NUM(x) + 0x158)

// DMAC channel 4
#define    DMAC_SAR4_NUM(x)         (DMAC_BASE_NUM(x) + 0x160)
#define    DMAC_DAR4_NUM(x)         (DMAC_BASE_NUM(x) + 0x168)
#define    DMAC_LLP4_NUM(x)         (DMAC_BASE_NUM(x) + 0x170)
#define    DMAC_CTL4_NUM(x)         (DMAC_BASE_NUM(x) + 0x178)
#define    DMAC_SSTAT4_NUM(x)       (DMAC_BASE_NUM(x) + 0x180)
#define    DMAC_DSTAT4_NUM(x)       (DMAC_BASE_NUM(x) + 0x188)
#define    DMAC_SSTATR4_NUM(x)      (DMAC_BASE_NUM(x) + 0x190)
#define    DMAC_DSTATR4_NUM(x)      (DMAC_BASE_NUM(x) + 0x198)
#define    DMAC_CFG4_NUM(x)         (DMAC_BASE_NUM(x) + 0x1A0)
#define    DMAC_SGR4_NUM(x)         (DMAC_BASE_NUM(x) + 0x1A8)
#define    DMAC_DSR4_NUM(x)         (DMAC_BASE_NUM(x) + 0x1B0)

// DMAC channel 5
#define    DMAC_SAR5_NUM(x)         (DMAC_BASE_NUM(x) + 0x1B8)
#define    DMAC_DAR5_NUM(x)         (DMAC_BASE_NUM(x) + 0x1C0)
#define    DMAC_LLP5_NUM(x)         (DMAC_BASE_NUM(x) + 0x1C8)
#define    DMAC_CTL5_NUM(x)         (DMAC_BASE_NUM(x) + 0x1D0)
#define    DMAC_SSTAT5_NUM(x)       (DMAC_BASE_NUM(x) + 0x1D8)
#define    DMAC_DSTAT5_NUM(x)       (DMAC_BASE_NUM(x) + 0x1E0)
#define    DMAC_SSTATR5_NUM(x)      (DMAC_BASE_NUM(x) + 0x1E8)
#define    DMAC_DSTATR5_NUM(x)      (DMAC_BASE_NUM(x) + 0x1F0)
#define    DMAC_CFG5_NUM(x)         (DMAC_BASE_NUM(x) + 0x1F8)
#define    DMAC_SGR5_NUM(x)         (DMAC_BASE_NUM(x) + 0x200)
#define    DMAC_DSR5_NUM(x)         (DMAC_BASE_NUM(x) + 0x208)

// DMAC channel 6
#define    DMAC_SAR6_NUM(x)         (DMAC_BASE_NUM(x) + 0x210)
#define    DMAC_DAR6_NUM(x)         (DMAC_BASE_NUM(x) + 0x218)
#define    DMAC_LLP6_NUM(x)         (DMAC_BASE_NUM(x) + 0x220)
#define    DMAC_CTL6_NUM(x)         (DMAC_BASE_NUM(x) + 0x228)
#define    DMAC_SSTAT6_NUM(x)       (DMAC_BASE_NUM(x) + 0x230)
#define    DMAC_DSTAT6_NUM(x)       (DMAC_BASE_NUM(x) + 0x238)
#define    DMAC_SSTATR6_NUM(x)      (DMAC_BASE_NUM(x) + 0x240)
#define    DMAC_DSTATR6_NUM(x)      (DMAC_BASE_NUM(x) + 0x248)
#define    DMAC_CFG6_NUM(x)         (DMAC_BASE_NUM(x) + 0x250)
#define    DMAC_SGR6_NUM(x)         (DMAC_BASE_NUM(x) + 0x258)
#define    DMAC_DSR6_NUM(x)         (DMAC_BASE_NUM(x) + 0x260)

// DMAC channel 7
#define    DMAC_SAR7_NUM(x)         (DMAC_BASE_NUM(x) + 0x268)
#define    DMAC_DAR7_NUM(x)         (DMAC_BASE_NUM(x) + 0x270)
#define    DMAC_LLP7_NUM(x)         (DMAC_BASE_NUM(x) + 0x278)
#define    DMAC_CTL7_NUM(x)         (DMAC_BASE_NUM(x) + 0x280)
#define    DMAC_SSTAT7_NUM(x)       (DMAC_BASE_NUM(x) + 0x288)
#define    DMAC_DSTAT7_NUM(x)       (DMAC_BASE_NUM(x) + 0x290)
#define    DMAC_SSTATR7_NUM(x)      (DMAC_BASE_NUM(x) + 0x298)
#define    DMAC_DSTATR7_NUM(x)      (DMAC_BASE_NUM(x) + 0x2A0)
#define    DMAC_CFG7_NUM(x)         (DMAC_BASE_NUM(x) + 0x2A8)
#define    DMAC_SGR7_NUM(x)         (DMAC_BASE_NUM(x) + 0x2B0)
#define    DMAC_DSR7_NUM(x)         (DMAC_BASE_NUM(x) + 0x2B8)

#define    DMAC_RAWTFR_NUM(x)       (DMAC_BASE_NUM(x) + 0x2C0)
#define    DMAC_RAWBLK_NUM(x)       (DMAC_BASE_NUM(x) + 0x2C8)
#define    DMAC_RAWSRCTR_NUM(x)     (DMAC_BASE_NUM(x) + 0x2D0)
#define    DMAC_RAWDSTTR_NUM(x)     (DMAC_BASE_NUM(x) + 0x2D8)
#define    DMAC_RAWERR_NUM(x)       (DMAC_BASE_NUM(x) + 0x2E0)

#define    DMAC_STATRF_NUM(x)       (DMAC_BASE_NUM(x) + 0x2E8)
#define    DMAC_STATBLK_NUM(x)      (DMAC_BASE_NUM(x) + 0x2F0)
#define    DMAC_STATSRCTR_NUM(x)    (DMAC_BASE_NUM(x) + 0x2F8)
#define    DMAC_STATDSTTR_NUM(x)    (DMAC_BASE_NUM(x) + 0x300)
#define    DMAC_STATERR_NUM(x)      (DMAC_BASE_NUM(x) + 0x308)

#define    DMAC_MASKTRF_NUM(x)      (DMAC_BASE_NUM(x) + 0x310)
#define    DMAC_MASKBLK_NUM(x)      (DMAC_BASE_NUM(x) + 0x318)
#define    DMAC_MASKSRCTR_NUM(x)    (DMAC_BASE_NUM(x) + 0x320)
#define    DMAC_MASKDSTTR_NUM(x)    (DMAC_BASE_NUM(x) + 0x328)
#define    DMAC_MASKERR_NUM(x)      (DMAC_BASE_NUM(x) + 0x330)

#define    DMAC_CLRTFR_NUM(x)       (DMAC_BASE_NUM(x) + 0x338)
#define    DMAC_CLRBLK_NUM(x)       (DMAC_BASE_NUM(x) + 0x340)
#define    DMAC_CLRSRCTR_NUM(x)     (DMAC_BASE_NUM(x) + 0x348)
#define    DMAC_CLRDSTTR_NUM(x)     (DMAC_BASE_NUM(x) + 0x350)
#define    DMAC_CLRERR_NUM(x)       (DMAC_BASE_NUM(x) + 0x358)
#define    DMAC_STATINT_NUM(x)      (DMAC_BASE_NUM(x) + 0x360)

#define    DMAC_SRCREQ_NUM(x)       (DMAC_BASE_NUM(x) + 0x368)
#define    DMAC_DSTREQ_NUM(x)       (DMAC_BASE_NUM(x) + 0x370)
#define    DMAC_SGSRCREQ_NUM(x)     (DMAC_BASE_NUM(x) + 0x378)
#define    DMAC_SGDSTREQ_NUM(x)     (DMAC_BASE_NUM(x) + 0x380)
#define    DMAC_LSTSRC_NUM(x)       (DMAC_BASE_NUM(x) + 0x388)
#define    DMAC_LSTDST_NUM(x)       (DMAC_BASE_NUM(x) + 0x390)

#define    DMAC_CFG_NUM(x)          (DMAC_BASE_NUM(x) + 0x398)
#define    DMAC_CHENA_NUM(x)        (DMAC_BASE_NUM(x) + 0x3A0)
#define    DMAC_ID_NUM(x)           (DMAC_BASE_NUM(x) + 0x3A8)
#define    DMAC_TEST_NUM(x)         (DMAC_BASE_NUM(x) + 0x3B0)
#define    DMAC_COMP_PARAM6_NUM(x)  (DMAC_BASE_NUM(x) + 0x3C8)
#define    DMAC_COMP_PARAM5_NUM(x)  (DMAC_BASE_NUM(x) + 0x3D0)
#define    DMAC_COMP_PARAM4_NUM(x)  (DMAC_BASE_NUM(x) + 0x3D8)
#define    DMAC_COMP_PARAM3_NUM(x)  (DMAC_BASE_NUM(x) + 0x3E0)
#define    DMAC_COMP_PARAM2_NUM(x)  (DMAC_BASE_NUM(x) + 0x3E8)
#define    DMAC_COMP_PARAM1_NUM(x)  (DMAC_BASE_NUM(x) + 0x3F0)
#define    DMAC_COMP_ID_NUM(x)      (DMAC_BASE_NUM(x) + 0x3F8)

#endif // DMAC_BASE_NUM

#ifdef DMAC1_BASE
// DMAC1_BASE 32027000
// DMAC1 channel 0
#define    DMAC1_SAR0               (DMAC1_BASE + 0x000)
#define    DMAC1_DAR0               (DMAC1_BASE + 0x008)
#define    DMAC1_LLP0               (DMAC1_BASE + 0x010)
#define    DMAC1_CTL0               (DMAC1_BASE + 0x018)
#define    DMAC1_SSTAT0             (DMAC1_BASE + 0x020)
#define    DMAC1_DSTAT0             (DMAC1_BASE + 0x028)
#define    DMAC1_SSTATR0            (DMAC1_BASE + 0x030)
#define    DMAC1_DSTATR0            (DMAC1_BASE + 0x038)
#define    DMAC1_CFG0               (DMAC1_BASE + 0x040)
#define    DMAC1_SGR0               (DMAC1_BASE + 0x048)
#define    DMAC1_DSR0               (DMAC1_BASE + 0x050)

// DMAC1 channel 1
#define    DMAC1_SAR1               (DMAC1_BASE + 0x058)
#define    DMAC1_DAR1               (DMAC1_BASE + 0x060)
#define    DMAC1_LLP1               (DMAC1_BASE + 0x068)
#define    DMAC1_CTL1               (DMAC1_BASE + 0x070)
#define    DMAC1_SSTAT1             (DMAC1_BASE + 0x078)
#define    DMAC1_DSTAT1             (DMAC1_BASE + 0x080)
#define    DMAC1_SSTATR1            (DMAC1_BASE + 0x088)
#define    DMAC1_DSTATR1            (DMAC1_BASE + 0x090)
#define    DMAC1_CFG1               (DMAC1_BASE + 0x098)
#define    DMAC1_SGR1               (DMAC1_BASE + 0x0A0)
#define    DMAC1_DSR1               (DMAC1_BASE + 0x0A8)

// DMAC1 channel 2
#define    DMAC1_SAR2               (DMAC1_BASE + 0x0B0)
#define    DMAC1_DAR2               (DMAC1_BASE + 0x0B8)
#define    DMAC1_LLP2               (DMAC1_BASE + 0x0C0)
#define    DMAC1_CTL2               (DMAC1_BASE + 0x0C8)
#define    DMAC1_SSTAT2             (DMAC1_BASE + 0x0D0)
#define    DMAC1_DSTAT2             (DMAC1_BASE + 0x0D8)
#define    DMAC1_SSTATR2            (DMAC1_BASE + 0x0E0)
#define    DMAC1_DSTATR2            (DMAC1_BASE + 0x0E8)
#define    DMAC1_CFG2               (DMAC1_BASE + 0x0F0)
#define    DMAC1_SGR2               (DMAC1_BASE + 0x0F8)
#define    DMAC1_DSR2               (DMAC1_BASE + 0x100)

// DMAC1 channel 3
#define    DMAC1_SAR3               (DMAC1_BASE + 0x108)
#define    DMAC1_DAR3               (DMAC1_BASE + 0x110)
#define    DMAC1_LLP3               (DMAC1_BASE + 0x118)
#define    DMAC1_CTL3               (DMAC1_BASE + 0x120)
#define    DMAC1_SSTAT3             (DMAC1_BASE + 0x128)
#define    DMAC1_DSTAT3             (DMAC1_BASE + 0x130)
#define    DMAC1_SSTATR3            (DMAC1_BASE + 0x138)
#define    DMAC1_DSTATR3            (DMAC1_BASE + 0x140)
#define    DMAC1_CFG3               (DMAC1_BASE + 0x148)
#define    DMAC1_SGR3               (DMAC1_BASE + 0x150)
#define    DMAC1_DSR3               (DMAC1_BASE + 0x158)

// DMAC1 channel 4
#define    DMAC1_SAR4               (DMAC1_BASE + 0x160)
#define    DMAC1_DAR4               (DMAC1_BASE + 0x168)
#define    DMAC1_LLP4               (DMAC1_BASE + 0x170)
#define    DMAC1_CTL4               (DMAC1_BASE + 0x178)
#define    DMAC1_SSTAT4             (DMAC1_BASE + 0x180)
#define    DMAC1_DSTAT4             (DMAC1_BASE + 0x188)
#define    DMAC1_SSTATR4            (DMAC1_BASE + 0x190)
#define    DMAC1_DSTATR4            (DMAC1_BASE + 0x198)
#define    DMAC1_CFG4               (DMAC1_BASE + 0x1A0)
#define    DMAC1_SGR4               (DMAC1_BASE + 0x1A8)
#define    DMAC1_DSR4               (DMAC1_BASE + 0x1B0)

// DMAC1 channel 5
#define    DMAC1_SAR5               (DMAC1_BASE + 0x1B8)
#define    DMAC1_DAR5               (DMAC1_BASE + 0x1C0)
#define    DMAC1_LLP5               (DMAC1_BASE + 0x1C8)
#define    DMAC1_CTL5               (DMAC1_BASE + 0x1D0)
#define    DMAC1_SSTAT5             (DMAC1_BASE + 0x1D8)
#define    DMAC1_DSTAT5             (DMAC1_BASE + 0x1E0)
#define    DMAC1_SSTATR5            (DMAC1_BASE + 0x1E8)
#define    DMAC1_DSTATR5            (DMAC1_BASE + 0x1F0)
#define    DMAC1_CFG5               (DMAC1_BASE + 0x1F8)
#define    DMAC1_SGR5               (DMAC1_BASE + 0x200)
#define    DMAC1_DSR5               (DMAC1_BASE + 0x208)

// DMAC1 channel 6
#define    DMAC1_SAR6               (DMAC1_BASE + 0x210)
#define    DMAC1_DAR6               (DMAC1_BASE + 0x218)
#define    DMAC1_LLP6               (DMAC1_BASE + 0x220)
#define    DMAC1_CTL6               (DMAC1_BASE + 0x228)
#define    DMAC1_SSTAT6             (DMAC1_BASE + 0x230)
#define    DMAC1_DSTAT6             (DMAC1_BASE + 0x238)
#define    DMAC1_SSTATR6            (DMAC1_BASE + 0x240)
#define    DMAC1_DSTATR6            (DMAC1_BASE + 0x248)
#define    DMAC1_CFG6               (DMAC1_BASE + 0x250)
#define    DMAC1_SGR6               (DMAC1_BASE + 0x258)
#define    DMAC1_DSR6               (DMAC1_BASE + 0x260)

// DMAC1 channel 7
#define    DMAC1_SAR7               (DMAC1_BASE + 0x268)
#define    DMAC1_DAR7               (DMAC1_BASE + 0x270)
#define    DMAC1_LLP7               (DMAC1_BASE + 0x278)
#define    DMAC1_CTL7               (DMAC1_BASE + 0x280)
#define    DMAC1_SSTAT7             (DMAC1_BASE + 0x288)
#define    DMAC1_DSTAT7             (DMAC1_BASE + 0x290)
#define    DMAC1_SSTATR7            (DMAC1_BASE + 0x298)
#define    DMAC1_DSTATR7            (DMAC1_BASE + 0x2A0)
#define    DMAC1_CFG7               (DMAC1_BASE + 0x2A8)
#define    DMAC1_SGR7               (DMAC1_BASE + 0x2B0)
#define    DMAC1_DSR7               (DMAC1_BASE + 0x2B8)

#define    DMAC1_RAWTFR             (DMAC1_BASE + 0x2C0)
#define    DMAC1_RAWBLK             (DMAC1_BASE + 0x2C8)
#define    DMAC1_RAWSRCTR           (DMAC1_BASE + 0x2D0)
#define    DMAC1_RAWDSTTR           (DMAC1_BASE + 0x2D8)
#define    DMAC1_RAWERR             (DMAC1_BASE + 0x2E0)

#define    DMAC1_STATRF             (DMAC1_BASE + 0x2E8)
#define    DMAC1_STATBLK            (DMAC1_BASE + 0x2F0)
#define    DMAC1_STATSRCTR          (DMAC1_BASE + 0x2F8)
#define    DMAC1_STATDSTTR          (DMAC1_BASE + 0x300)
#define    DMAC1_STATERR            (DMAC1_BASE + 0x308)

#define    DMAC1_MASKTRF            (DMAC1_BASE + 0x310)
#define    DMAC1_MASKBLK            (DMAC1_BASE + 0x318)
#define    DMAC1_MASKSRCTR          (DMAC1_BASE + 0x320)
#define    DMAC1_MASKDSTTR          (DMAC1_BASE + 0x328)
#define    DMAC1_MASKERR            (DMAC1_BASE + 0x330)

#define    DMAC1_CLRTFR             (DMAC1_BASE + 0x338)
#define    DMAC1_CLRBLK             (DMAC1_BASE + 0x340)
#define    DMAC1_CLRSRCTR           (DMAC1_BASE + 0x348)
#define    DMAC1_CLRDSTTR           (DMAC1_BASE + 0x350)
#define    DMAC1_CLRERR             (DMAC1_BASE + 0x358)
#define    DMAC1_STATINT            (DMAC1_BASE + 0x360)

#define    DMAC1_SRCREQ             (DMAC1_BASE + 0x368)
#define    DMAC1_DSTREQ             (DMAC1_BASE + 0x370)
#define    DMAC1_SGSRCREQ           (DMAC1_BASE + 0x378)
#define    DMAC1_SGDSTREQ           (DMAC1_BASE + 0x380)
#define    DMAC1_LSTSRC             (DMAC1_BASE + 0x388)
#define    DMAC1_LSTDST             (DMAC1_BASE + 0x390)

#define    DMAC1_CFG                (DMAC1_BASE + 0x398)
#define    DMAC1_CHENA              (DMAC1_BASE + 0x3A0)
#define    DMAC1_ID                 (DMAC1_BASE + 0x3A8)
#define    DMAC1_TEST               (DMAC1_BASE + 0x3B0)
#define    DMAC1_COMP_PARAM6        (DMAC1_BASE + 0x3C8)
#define    DMAC1_COMP_PARAM5        (DMAC1_BASE + 0x3D0)
#define    DMAC1_COMP_PARAM4        (DMAC1_BASE + 0x3D8)
#define    DMAC1_COMP_PARAM3        (DMAC1_BASE + 0x3E0)
#define    DMAC1_COMP_PARAM2        (DMAC1_BASE + 0x3E8)
#define    DMAC1_COMP_PARAM1        (DMAC1_BASE + 0x3F0)
#define    DMAC1_COMP_ID            (DMAC1_BASE + 0x3F8)

#endif // DMAC1_BASE

#ifdef DMAC2_BASE

// DMAC2_BASE 31011000
// DMAC2 channel 0
#define    DMAC2_SAR0               (DMAC2_BASE + 0x000)
#define    DMAC2_DAR0               (DMAC2_BASE + 0x008)
#define    DMAC2_LLP0               (DMAC2_BASE + 0x010)
#define    DMAC2_CTL0               (DMAC2_BASE + 0x018)
#define    DMAC2_SSTAT0             (DMAC2_BASE + 0x020)
#define    DMAC2_DSTAT0             (DMAC2_BASE + 0x028)
#define    DMAC2_SSTATR0            (DMAC2_BASE + 0x030)
#define    DMAC2_DSTATR0            (DMAC2_BASE + 0x038)
#define    DMAC2_CFG0               (DMAC2_BASE + 0x040)
#define    DMAC2_SGR0               (DMAC2_BASE + 0x048)
#define    DMAC2_DSR0               (DMAC2_BASE + 0x050)

// DMAC2 channel 1
#define    DMAC2_SAR1               (DMAC2_BASE + 0x058)
#define    DMAC2_DAR1               (DMAC2_BASE + 0x060)
#define    DMAC2_LLP1               (DMAC2_BASE + 0x068)
#define    DMAC2_CTL1               (DMAC2_BASE + 0x070)
#define    DMAC2_SSTAT1             (DMAC2_BASE + 0x078)
#define    DMAC2_DSTAT1             (DMAC2_BASE + 0x080)
#define    DMAC2_SSTATR1            (DMAC2_BASE + 0x088)
#define    DMAC2_DSTATR1            (DMAC2_BASE + 0x090)
#define    DMAC2_CFG1               (DMAC2_BASE + 0x098)
#define    DMAC2_SGR1               (DMAC2_BASE + 0x0A0)
#define    DMAC2_DSR1               (DMAC2_BASE + 0x0A8)

// DMAC2 channel 2
#define    DMAC2_SAR2               (DMAC2_BASE + 0x0B0)
#define    DMAC2_DAR2               (DMAC2_BASE + 0x0B8)
#define    DMAC2_LLP2               (DMAC2_BASE + 0x0C0)
#define    DMAC2_CTL2               (DMAC2_BASE + 0x0C8)
#define    DMAC2_SSTAT2             (DMAC2_BASE + 0x0D0)
#define    DMAC2_DSTAT2             (DMAC2_BASE + 0x0D8)
#define    DMAC2_SSTATR2            (DMAC2_BASE + 0x0E0)
#define    DMAC2_DSTATR2            (DMAC2_BASE + 0x0E8)
#define    DMAC2_CFG2               (DMAC2_BASE + 0x0F0)
#define    DMAC2_SGR2               (DMAC2_BASE + 0x0F8)
#define    DMAC2_DSR2               (DMAC2_BASE + 0x100)

// DMAC2 channel 3
#define    DMAC2_SAR3               (DMAC2_BASE + 0x108)
#define    DMAC2_DAR3               (DMAC2_BASE + 0x110)
#define    DMAC2_LLP3               (DMAC2_BASE + 0x118)
#define    DMAC2_CTL3               (DMAC2_BASE + 0x120)
#define    DMAC2_SSTAT3             (DMAC2_BASE + 0x128)
#define    DMAC2_DSTAT3             (DMAC2_BASE + 0x130)
#define    DMAC2_SSTATR3            (DMAC2_BASE + 0x138)
#define    DMAC2_DSTATR3            (DMAC2_BASE + 0x140)
#define    DMAC2_CFG3               (DMAC2_BASE + 0x148)
#define    DMAC2_SGR3               (DMAC2_BASE + 0x150)
#define    DMAC2_DSR3               (DMAC2_BASE + 0x158)

// DMAC2 channel 4
#define    DMAC2_SAR4               (DMAC2_BASE + 0x160)
#define    DMAC2_DAR4               (DMAC2_BASE + 0x168)
#define    DMAC2_LLP4               (DMAC2_BASE + 0x170)
#define    DMAC2_CTL4               (DMAC2_BASE + 0x178)
#define    DMAC2_SSTAT4             (DMAC2_BASE + 0x180)
#define    DMAC2_DSTAT4             (DMAC2_BASE + 0x188)
#define    DMAC2_SSTATR4            (DMAC2_BASE + 0x190)
#define    DMAC2_DSTATR4            (DMAC2_BASE + 0x198)
#define    DMAC2_CFG4               (DMAC2_BASE + 0x1A0)
#define    DMAC2_SGR4               (DMAC2_BASE + 0x1A8)
#define    DMAC2_DSR4               (DMAC2_BASE + 0x1B0)

// DMAC2 channel 5
#define    DMAC2_SAR5               (DMAC2_BASE + 0x1B8)
#define    DMAC2_DAR5               (DMAC2_BASE + 0x1C0)
#define    DMAC2_LLP5               (DMAC2_BASE + 0x1C8)
#define    DMAC2_CTL5               (DMAC2_BASE + 0x1D0)
#define    DMAC2_SSTAT5             (DMAC2_BASE + 0x1D8)
#define    DMAC2_DSTAT5             (DMAC2_BASE + 0x1E0)
#define    DMAC2_SSTATR5            (DMAC2_BASE + 0x1E8)
#define    DMAC2_DSTATR5            (DMAC2_BASE + 0x1F0)
#define    DMAC2_CFG5               (DMAC2_BASE + 0x1F8)
#define    DMAC2_SGR5               (DMAC2_BASE + 0x200)
#define    DMAC2_DSR5               (DMAC2_BASE + 0x208)

// DMAC2 channel 6
#define    DMAC2_SAR6               (DMAC2_BASE + 0x210)
#define    DMAC2_DAR6               (DMAC2_BASE + 0x218)
#define    DMAC2_LLP6               (DMAC2_BASE + 0x220)
#define    DMAC2_CTL6               (DMAC2_BASE + 0x228)
#define    DMAC2_SSTAT6             (DMAC2_BASE + 0x230)
#define    DMAC2_DSTAT6             (DMAC2_BASE + 0x238)
#define    DMAC2_SSTATR6            (DMAC2_BASE + 0x240)
#define    DMAC2_DSTATR6            (DMAC2_BASE + 0x248)
#define    DMAC2_CFG6               (DMAC2_BASE + 0x250)
#define    DMAC2_SGR6               (DMAC2_BASE + 0x258)
#define    DMAC2_DSR6               (DMAC2_BASE + 0x260)

// DMAC2 channel 7
#define    DMAC2_SAR7               (DMAC2_BASE + 0x268)
#define    DMAC2_DAR7               (DMAC2_BASE + 0x270)
#define    DMAC2_LLP7               (DMAC2_BASE + 0x278)
#define    DMAC2_CTL7               (DMAC2_BASE + 0x280)
#define    DMAC2_SSTAT7             (DMAC2_BASE + 0x288)
#define    DMAC2_DSTAT7             (DMAC2_BASE + 0x290)
#define    DMAC2_SSTATR7            (DMAC2_BASE + 0x298)
#define    DMAC2_DSTATR7            (DMAC2_BASE + 0x2A0)
#define    DMAC2_CFG7               (DMAC2_BASE + 0x2A8)
#define    DMAC2_SGR7               (DMAC2_BASE + 0x2B0)
#define    DMAC2_DSR7               (DMAC2_BASE + 0x2B8)

#define    DMAC2_RAWTFR             (DMAC2_BASE + 0x2C0)
#define    DMAC2_RAWBLK             (DMAC2_BASE + 0x2C8)
#define    DMAC2_RAWSRCTR           (DMAC2_BASE + 0x2D0)
#define    DMAC2_RAWDSTTR           (DMAC2_BASE + 0x2D8)
#define    DMAC2_RAWERR             (DMAC2_BASE + 0x2E0)

#define    DMAC2_STATRF             (DMAC2_BASE + 0x2E8)
#define    DMAC2_STATBLK            (DMAC2_BASE + 0x2F0)
#define    DMAC2_STATSRCTR          (DMAC2_BASE + 0x2F8)
#define    DMAC2_STATDSTTR          (DMAC2_BASE + 0x300)
#define    DMAC2_STATERR            (DMAC2_BASE + 0x308)

#define    DMAC2_MASKTRF            (DMAC2_BASE + 0x310)
#define    DMAC2_MASKBLK            (DMAC2_BASE + 0x318)
#define    DMAC2_MASKSRCTR          (DMAC2_BASE + 0x320)
#define    DMAC2_MASKDSTTR          (DMAC2_BASE + 0x328)
#define    DMAC2_MASKERR            (DMAC2_BASE + 0x330)

#define    DMAC2_CLRTFR             (DMAC2_BASE + 0x338)
#define    DMAC2_CLRBLK             (DMAC2_BASE + 0x340)
#define    DMAC2_CLRSRCTR           (DMAC2_BASE + 0x348)
#define    DMAC2_CLRDSTTR           (DMAC2_BASE + 0x350)
#define    DMAC2_CLRERR             (DMAC2_BASE + 0x358)
#define    DMAC2_STATINT            (DMAC2_BASE + 0x360)

#define    DMAC2_SRCREQ             (DMAC2_BASE + 0x368)
#define    DMAC2_DSTREQ             (DMAC2_BASE + 0x370)
#define    DMAC2_SGSRCREQ           (DMAC2_BASE + 0x378)
#define    DMAC2_SGDSTREQ           (DMAC2_BASE + 0x380)
#define    DMAC2_LSTSRC             (DMAC2_BASE + 0x388)
#define    DMAC2_LSTDST             (DMAC2_BASE + 0x390)

#define    DMAC2_CFG                (DMAC2_BASE + 0x398)
#define    DMAC2_CHENA              (DMAC2_BASE + 0x3A0)
#define    DMAC2_ID                 (DMAC2_BASE + 0x3A8)
#define    DMAC2_TEST               (DMAC2_BASE + 0x3B0)
#define    DMAC2_COMP_PARAM6        (DMAC2_BASE + 0x3C8)
#define    DMAC2_COMP_PARAM5        (DMAC2_BASE + 0x3D0)
#define    DMAC2_COMP_PARAM4        (DMAC2_BASE + 0x3D8)
#define    DMAC2_COMP_PARAM3        (DMAC2_BASE + 0x3E0)
#define    DMAC2_COMP_PARAM2        (DMAC2_BASE + 0x3E8)
#define    DMAC2_COMP_PARAM1        (DMAC2_BASE + 0x3F0)
#define    DMAC2_COMP_ID            (DMAC2_BASE + 0x3F8)

#endif // DMAC2_BASE

// LCDC register define 
// LCDC_BASE 0x32025000
#ifdef LCDC_BASE

#define    LCDC_ECR                 (LCDC_BASE + 0x000)
#define    LCDC_BECR                (LCDC_BASE + 0x004)
#define    LCDC_W1ECR               (LCDC_BASE + 0x008)
#define    LCDC_W2ECR               (LCDC_BASE + 0x00C)
#define    LCDC_CECR                (LCDC_BASE + 0x010)
#define    LCDC_BCR                 (LCDC_BASE + 0x014)
#define    LCDC_PLCR                (LCDC_BASE + 0x018)
#define    LCDC_PFCR                (LCDC_BASE + 0x01C)
#define    LCDC_PCCR                (LCDC_BASE + 0x020)
#define    LCDC_IOCR                (LCDC_BASE + 0x024)
#define    LCDC_IECR                (LCDC_BASE + 0x028)
#define    LCDC_W1CKR               (LCDC_BASE + 0x02C)
#define    LCDC_W2CKR               (LCDC_BASE + 0x030)
#define    LCDC_W1BVR               (LCDC_BASE + 0x034)
#define    LCDC_W2BVR               (LCDC_BASE + 0x038)
#define    LCDC_SCR                 (LCDC_BASE + 0x03C)
#define    LCDC_W1TPCR              (LCDC_BASE + 0x040)
#define    LCDC_W1BPCR              (LCDC_BASE + 0x044)
#define    LCDC_W2TPCR              (LCDC_BASE + 0x048)
#define    LCDC_W2BPCR              (LCDC_BASE + 0x04C)
#define    LCDC_CTPCR               (LCDC_BASE + 0x050)
#define    LCDC_CBPCR               (LCDC_BASE + 0x054)
#define    LCDC_ACSR                (LCDC_BASE + 0x058)
#define    LCDC_BBSAR               (LCDC_BASE + 0x05C)
#define    LCDC_W1YBSAR             (LCDC_BASE + 0x060)
#define    LCDC_W1UBSAR             (LCDC_BASE + 0x064)
#define    LCDC_W1VBSAR             (LCDC_BASE + 0x068)
#define    LCDC_W2BSAR              (LCDC_BASE + 0x06C)
#define    LCDC_CBSAR               (LCDC_BASE + 0x070)
#define    LCDC_BBEAR               (LCDC_BASE + 0x074)
#define    LCDC_W1YBEAR             (LCDC_BASE + 0x078)
#define    LCDC_W1UBEAR             (LCDC_BASE + 0x07C)
#define    LCDC_W1VBEAR             (LCDC_BASE + 0x080)
#define    LCDC_W2BEAR              (LCDC_BASE + 0x084)
#define    LCDC_CBEAR               (LCDC_BASE + 0x088)
#define    LCDC_CC0R                (LCDC_BASE + 0x08C)
#define    LCDC_CC1R                (LCDC_BASE + 0x090)
#define    LCDC_CC2R                (LCDC_BASE + 0x094)
#define    LCDC_CC3R                (LCDC_BASE + 0x098)
#define    LCDC_BRIWR               (LCDC_BASE + 0x09C)
#define    LCDC_OVL1RIWR            (LCDC_BASE + 0x0A0)
#define    LCDC_OVL2RIWR            (LCDC_BASE + 0x0A4)
#define    LCDC_BDIWR               (LCDC_BASE + 0x0A8)
#define    LCDC_OVL1IWR             (LCDC_BASE + 0x0AC)
#define    LCDC_OVL2IWR             (LCDC_BASE + 0x0B0)
#define    LCDC_BTPCR               (LCDC_BASE + 0x0B4)
#define    LCDC_BBPCR               (LCDC_BASE + 0x0B8)

#endif // LCDC_BASE


// DDR2 Controller register define
// DDR2CTL_BASE 0x33000000
#ifdef DDR2CTL_BASE
 
#define    DDR2_CCR                 (DDR2CTL_BASE + 0x000)
#define    DDR2_DCR                 (DDR2CTL_BASE + 0x004)
#define    DDR2_IOCR                (DDR2CTL_BASE + 0x008)
#define    DDR2_CSR                 (DDR2CTL_BASE + 0x00C)
#define    DDR2_DRR                 (DDR2CTL_BASE + 0x010)
#define    DDR2_TPR0                (DDR2CTL_BASE + 0x014)
#define    DDR2_TPR1                (DDR2CTL_BASE + 0x018)
#define    DDR2_TPR2                (DDR2CTL_BASE + 0x01C)
#define    DDR2_GDLLCR              (DDR2CTL_BASE + 0x020)
#define    DDR2_DLLCR0              (DDR2CTL_BASE + 0x024)
#define    DDR2_DLLCR1              (DDR2CTL_BASE + 0x028)
#define    DDR2_DLLCR2              (DDR2CTL_BASE + 0x02C)
#define    DDR2_DLLCR3              (DDR2CTL_BASE + 0x030)
#define    DDR2_DLLCR4              (DDR2CTL_BASE + 0x034)
#define    DDR2_DLLCR5              (DDR2CTL_BASE + 0x038)
#define    DDR2_DLLCR6              (DDR2CTL_BASE + 0x03C)
#define    DDR2_DLLCR7              (DDR2CTL_BASE + 0x040)
#define    DDR2_DLLCR8              (DDR2CTL_BASE + 0x044)
#define    DDR2_DLLCR9              (DDR2CTL_BASE + 0x048)
#define    DDR2_RSLR0               (DDR2CTL_BASE + 0x04C)
#define    DDR2_RSLR1               (DDR2CTL_BASE + 0x050)
#define    DDR2_RSLR2               (DDR2CTL_BASE + 0x054)
#define    DDR2_RSLR3               (DDR2CTL_BASE + 0x058)
#define    DDR2_RDGR0               (DDR2CTL_BASE + 0x05C)
#define    DDR2_RDGR1               (DDR2CTL_BASE + 0x060)
#define    DDR2_RDGR2               (DDR2CTL_BASE + 0x064)
#define    DDR2_RDGR3               (DDR2CTL_BASE + 0x068)
#define    DDR2_DQTR0               (DDR2CTL_BASE + 0x06C)
#define    DDR2_DQTR1               (DDR2CTL_BASE + 0x070)
#define    DDR2_DQTR2               (DDR2CTL_BASE + 0x074)
#define    DDR2_DQTR3               (DDR2CTL_BASE + 0x078)
#define    DDR2_DQTR4               (DDR2CTL_BASE + 0x07C)
#define    DDR2_DQTR5               (DDR2CTL_BASE + 0x080)
#define    DDR2_DQTR6               (DDR2CTL_BASE + 0x084)
#define    DDR2_DQTR7               (DDR2CTL_BASE + 0x088)
#define    DDR2_DQTR8               (DDR2CTL_BASE + 0x08C)
#define    DDR2_DQSTR               (DDR2CTL_BASE + 0x090)
#define    DDR2_DQSBTR              (DDR2CTL_BASE + 0x094)
#define    DDR2_ODTCR               (DDR2CTL_BASE + 0x098)
#define    DDR2_DTR0                (DDR2CTL_BASE + 0x09C)
#define    DDR2_DTR1                (DDR2CTL_BASE + 0x0A0)
#define    DDR2_DTAR                (DDR2CTL_BASE + 0x0A4)
#define    DDR2_ZQCR0               (DDR2CTL_BASE + 0x0A8)
#define    DDR2_ZQCR1               (DDR2CTL_BASE + 0x0AC)
#define    DDR2_ZQSR                (DDR2CTL_BASE + 0x0B0)

#define    DDR2_MR                  (DDR2CTL_BASE + 0x1F0)
#define    DDR2_EMR                 (DDR2CTL_BASE + 0x1F4)
#define    DDR2_EMR2                (DDR2CTL_BASE + 0x1F8)
#define    DDR2_EMR3                (DDR2CTL_BASE + 0x1FC)

#define    DDR2_DGSTATUS            (DDR2CLT_BASE + 0x3FC)
#define    DDR2_HPCR0               (DDR2CTL_BASE + 0x400)
#define    DDR2_HPCR1               (DDR2CTL_BASE + 0x404)
#define    DDR2_HPCR2               (DDR2CTL_BASE + 0x408)
#define    DDR2_HPCR3               (DDR2CTL_BASE + 0x40C)
#define    DDR2_HPCR4               (DDR2CTL_BASE + 0x410)
#define    DDR2_HPCR5               (DDR2CTL_BASE + 0x414)
#define    DDR2_HPCR6               (DDR2CTL_BASE + 0x418)
#define    DDR2_HPCR7               (DDR2CTL_BASE + 0x41C)
#define    DDR2_HPCR8               (DDR2CTL_BASE + 0x420)
#define    DDR2_HPCR9               (DDR2CTL_BASE + 0x424)
#define    DDR2_HPCR10              (DDR2CTL_BASE + 0x428)
#define    DDR2_HPCR11              (DDR2CTL_BASE + 0x42C)
#define    DDR2_HPCR12              (DDR2CTL_BASE + 0x430)
#define    DDR2_HPCR13              (DDR2CTL_BASE + 0x434)
#define    DDR2_HPCR14              (DDR2CTL_BASE + 0x438)
#define    DDR2_HPCR15              (DDR2CTL_BASE + 0x43C)
#define    DDR2_HPCR16              (DDR2CTL_BASE + 0x440)
#define    DDR2_HPCR17              (DDR2CTL_BASE + 0x444)
#define    DDR2_HPCR18              (DDR2CTL_BASE + 0x448)
#define    DDR2_HPCR19              (DDR2CTL_BASE + 0x44C)
#define    DDR2_HPCR20              (DDR2CTL_BASE + 0x450)
#define    DDR2_HPCR21              (DDR2CTL_BASE + 0x454)
#define    DDR2_HPCR22              (DDR2CTL_BASE + 0x458)
#define    DDR2_HPCR23              (DDR2CTL_BASE + 0x45C)
#define    DDR2_HPCR24              (DDR2CTL_BASE + 0x460)
#define    DDR2_HPCR25              (DDR2CTL_BASE + 0x464)
#define    DDR2_HPCR26              (DDR2CTL_BASE + 0x468)
#define    DDR2_HPCR27              (DDR2CTL_BASE + 0x46C)
#define    DDR2_HPCR28              (DDR2CTL_BASE + 0x470)
#define    DDR2_HPCR29              (DDR2CTL_BASE + 0x474)
#define    DDR2_HPCR30              (DDR2CTL_BASE + 0x478)
#define    DDR2_HPCR31              (DDR2CTL_BASE + 0x47C)

#define    DDR2_PQCR0               (DDR2CTL_BASE + 0x480)
#define    DDR2_PQCR1               (DDR2CTL_BASE + 0x484)
#define    DDR2_PQCR2               (DDR2CTL_BASE + 0x488)
#define    DDR2_PQCR3               (DDR2CTL_BASE + 0x48C)
#define    DDR2_PQCR4               (DDR2CTL_BASE + 0x490)
#define    DDR2_PQCR5               (DDR2CTL_BASE + 0x494)
#define    DDR2_PQCR6               (DDR2CTL_BASE + 0x498)
#define    DDR2_PQCR7               (DDR2CTL_BASE + 0x49C)

#define    DDR2_MMGCR               (DDR2CTL_BASE + 0x4A0)

#endif // DDR2CTL_BASE

#define UART_CLK (45000000UL)

#endif // SEP0611_REG
