/*********************************************************************
* Filename    :  SEP0718_reg.h
**********************************************************************/
#ifndef __SEP0718_REG_H_
#define __SEP0718_REG_H_

//*****************************************************************************
// 中断号(wrong)
//*****************************************************************************
#define    INTSRC_LCDC          61
#define    INTSRC_GPU           60

#define    INTSRC_VPU           59
#define    INTSRC_TIMER3        58
#define    INTSRC_TIMER2        57
#define    INTSRC_TIMER1        56

#define    INTSRC_NAND          55
#define    INTSRC_I2S           54
#define    INTSRC_I2C3          53
#define    INTSRC_I2C2          52
#define    INTSRC_I2C1          51
#define    INTSRC_I2C           51
#define    INTSRC_SSI3          50
#define    INTSRC_SSI2          49
#define    INTSRC_SSI1          48

#define    INTSRC_SDIO2         47
#define    INTSRC_SDIO1         46
#define    INTSRC_UART3         45
#define    INTSRC_UART2         44
#define    INTSRC_UART1         43
#define    INTSRC_UART0         42
#define    INTSRC_PWM           41
#define    INTSRC_USB           40
#define    INTSRC_USBDMA        39
#define    INTSRC_DMAC2         38
#define    INTSRC_DMAC1         37
#define    INTSRC_PMUIRQ_A11    36
#define    INTSRC_DMAIRQ_A11    35
#define    INTSRC_PMU           34
#define    INTSRC_RTC           33
#define    INTSRC_WATCHDOG      32

#define    INTSRC_PORTE12       31
#define    INTSRC_PORTE11       30
#define    INTSRC_PORTE10       29
#define    INTSRC_PORTE9        28
#define    INTSRC_PORTE5        27
#define    INTSRC_PORTE4        26
#define    INTSRC_PORTD9        25
#define    INTSRC_PORTD8        24
#define    INTSRC_PORTD3        23
#define    INTSRC_PORTD2        22
#define    INTSRC_PORTD1        21
#define    INTSRC_PORTD0        20
#define    INTSRC_PORTC3        19
#define    INTSRC_PORTC2        18
#define    INTSRC_PORTC1        17
#define    INTSRC_PORTC0        16
#define    INTSRC_EXT15         15
#define    INTSRC_EXT14         14
#define    INTSRC_EXT13         13
#define    INTSRC_EXT12         12
#define    INTSRC_EXT11         11
#define    INTSRC_EXT10         10
#define    INTSRC_EXT9          9
#define    INTSRC_EXT8          8
#define    INTSRC_EXT7          7
#define    INTSRC_EXT6          6
#define    INTSRC_EXT5          5
#define    INTSRC_EXT4          4
#define    INTSRC_AO_EXT3       3
#define    INTSRC_AO_EXT2       2
#define    INTSRC_AO_EXT1       1
#define    INTSRC_AO_EXT0       0
     
/*********************** UART ********************************/
#define    UART_BASE        0xb1000000
#define    UART0_BASE       0xb1000000
#define    UART1_BASE       0xb1001000
#define    UART2_BASE       0xb1002000

//UART0	
#define    UART0_DLBL       (*(volatile unsigned *)0xb1000000)
#define    UART0_RXFIFO     (*(volatile unsigned *)0xb1000000)
#define    UART0_TXFIFO     (*(volatile unsigned *)0xb1000000)
#define    UART0_DLBH       (*(volatile unsigned *)0xb1000004)
#define    UART0_IER        (*(volatile unsigned *)0xb1000004)
#define    UART0_IIR        (*(volatile unsigned *)0xb1000008)
#define    UART0_FCR        (*(volatile unsigned *)0xb1000008)
#define    UART0_LCR        (*(volatile unsigned *)0xb100000C)
#define    UART0_MCR        (*(volatile unsigned *)0xb1000010)
#define    UART0_LSR        (*(volatile unsigned *)0xb1000014)
#define    UART0_MSR        (*(volatile unsigned *)0xb1000018)

#define	RXFIFO0         	 (0xb1000000)
#define	TXFIFO0            (0xb1000000)

#define	RdRXFIFO0()        (*(volatile unsigned char *)0xb1000000)
#define	WrTXFIFO0(ch)      (*(volatile unsigned char *)0xb1000000)=(unsigned char)(ch)

/*********************** TIMER ********************************/
#define    TIMER_BASE       0xb0003000
/*
#define    TIMER_T1LCR      (*(volatile unsigned *)(TIMER_BASE + 0x000))      
#define    TIMER_T1CCR      (*(volatile unsigned *)(TIMER_BASE + 0x004))      
#define    TIMER_T1CR       (*(volatile unsigned *)(TIMER_BASE + 0x008))
#define    TIMER_T1ISCR     (*(volatile unsigned *)(TIMER_BASE + 0x00C))      
#define    TIMER_T1IMSR     (*(volatile unsigned *)(TIMER_BASE + 0x010))

#define    TIMER_TIMSR      (*(volatile unsigned *)(TIMER_BASE + 0x140))      
#define    TIMER_TISCR      (*(volatile unsigned *)(TIMER_BASE + 0x144))      
#define    TIMER_TISR       (*(volatile unsigned *)(TIMER_BASE + 0x148))

#define    TIMER_T2LCR      (*(volatile unsigned *)(TIMER_BASE + 0x020))      
#define    TIMER_T2CCR      (*(volatile unsigned *)(TIMER_BASE + 0x024))      
#define    TIMER_T2CR       (*(volatile unsigned *)(TIMER_BASE + 0x028))
#define    TIMER_T2ISCR     (*(volatile unsigned *)(TIMER_BASE + 0x02C))      
#define    TIMER_T2IMSR     (*(volatile unsigned *)(TIMER_BASE + 0x030))
*/
#define    TIMERx_BASE(Channel) \
                  (TIMER_BASE+((Channel - 1) * 0x20))
#define    TxLCR(ch)                (TIMERx_BASE(ch)+0x00)
#define    TxCCR(ch)                (TIMERx_BASE(ch)+0x04)
#define    TxCR(ch)                 (TIMERx_BASE(ch)+0x08)
#define    TxISCR(ch)               (TIMERx_BASE(ch)+0x0c)
#define    TxIMSR(ch)               (TIMERx_BASE(ch)+0x10)
#define    TxCAPR(ch)               (TIMERx_BASE(ch)+0x14)


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
/*********************** gpio ********************************/
#define    GPIO_BASE       0xb0006000
#define    GPIO_AOPORT_BASE 0xb0005000
#define    GPIO_AO_BASE GPIO_AOPORT_BASE

#define    GPIO_AOPORT_DIR           (GPIO_AOPORT_BASE + 0x110)
#define    GPIO_AOPORT_SEL           (GPIO_AOPORT_BASE + 0x114)
#define    GPIO_AOPORT_DATA         (GPIO_AOPORT_BASE + 0x118)

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
#define    GPIO_PORTI_INTRSEL        (GPIO_BASE + 0x11C)
#define    GPIO_PORTI_INTRLEL        (GPIO_BASE + 0x120)
#define    GPIO_PORTI_INTRPOL        (GPIO_BASE + 0x124)
#define    GPIO_PORTI_INTRCLR        (GPIO_BASE + 0x128)
#define    GPIO_PORTI_SECFUN        (GPIO_BASE + 0x12C) 

/*****************************************emi****************************/
//#define    EMI_BASE       0x32010000
#define    EMI_BASE       0x32010000
#define    SDRAM_CONFIG_REG         (*(volatile unsigned *)(EMI_BASE + 0x000))
#define    SDRAM_TIMING_REG1        (*(volatile unsigned *)(EMI_BASE + 0x004))
#define    SDRAM_TIMING_REG2        (*(volatile unsigned *)(EMI_BASE + 0x008))
#define    SDRAM_CONTROL_REG        (*(volatile unsigned *)(EMI_BASE + 0x00c))
#define    SDRAM_REFRESH_REG        (*(volatile unsigned *)(EMI_BASE + 0x010))
#define    CSA_BASE_ADDR            (*(volatile unsigned *)(EMI_BASE + 0x014))
#define    CSE_BASE_ADDR            (*(volatile unsigned *)(EMI_BASE + 0x018))
#define    CSB_BASE_ADDR            (*(volatile unsigned *)(EMI_BASE + 0x01c))
#define    CSC_BASE_ADDR            (*(volatile unsigned *)(EMI_BASE + 0x020))
#define    CSA_MASK_REG             (*(volatile unsigned *)(EMI_BASE + 0x054))
#define    CSE_MASK_REG             (*(volatile unsigned *)(EMI_BASE + 0x058))
#define    CSB_MASK_REG             (*(volatile unsigned *)(EMI_BASE + 0x05C))
#define    CSC_MASK_REG             (*(volatile unsigned *)(EMI_BASE + 0x060))
#define    CSA_ALIAS_ADDR           (*(volatile unsigned *)(EMI_BASE + 0x074))
#define    CSE_ALIAS_ADDR           (*(volatile unsigned *)(EMI_BASE + 0x078))
#define    CSA_REMAP_ADDR           (*(volatile unsigned *)(EMI_BASE + 0x084))
#define    CSE_REMAP_ADDR           (*(volatile unsigned *)(EMI_BASE + 0x088))
#define    SM_TIMING_SET1           (*(volatile unsigned *)(EMI_BASE + 0x094))
#define    SM_TIMING_SET2           (*(volatile unsigned *)(EMI_BASE + 0x098))
#define    SM_TIMING_SET3           (*(volatile unsigned *)(EMI_BASE + 0x09C))
#define    SM_WIDTH_REG             (*(volatile unsigned *)(EMI_BASE + 0x0a4))
#define    DDR_EXT_MODE_REG         (*(volatile unsigned *)(EMI_BASE + 0x0aC))
#define    SDRAM_TYPE_REG           (*(volatile unsigned *)(EMI_BASE + 0x0B0))
#define    REMAP_REG_ADDR           (*(volatile unsigned *)(EMI_BASE + 0x0B4))

/*****************************************VIC****************************/
#define    VIC_BASE    0xB0000000

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

/****************************nfc**********************************/
#define    NAND_BASE                0xB2021000

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

/*****************************dmac1**************************************/
#define    DMAC1_BASE               0xB2027000
#define    DMAC2_BASE               0xB1011000


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


////////////////////////////////////////////////////

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
#define    DMAC1_CHEN               (DMAC1_BASE + 0x3A0)
#define    DMAC1_ID                 (DMAC1_BASE + 0x3A8)
#define    DMAC1_TEST               (DMAC1_BASE + 0x3B0)
#define    DMAC1_COMP_PARAM6        (DMAC1_BASE + 0x3C8)
#define    DMAC1_COMP_PARAM5        (DMAC1_BASE + 0x3D0)
#define    DMAC1_COMP_PARAM4        (DMAC1_BASE + 0x3D8)
#define    DMAC1_COMP_PARAM3        (DMAC1_BASE + 0x3E0)
#define    DMAC1_COMP_PARAM2        (DMAC1_BASE + 0x3E8)
#define    DMAC1_COMP_PARAM1        (DMAC1_BASE + 0x3F0)
#define    DMAC1_COMP_ID            (DMAC1_BASE + 0x3F8)
/*************************dma2*********************************/
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
#define    DMAC2_CHEN               (DMAC2_BASE + 0x3A0)
#define    DMAC2_ID                 (DMAC2_BASE + 0x3A8)
#define    DMAC2_TEST               (DMAC2_BASE + 0x3B0)
#define    DMAC2_COMP_PARAM6        (DMAC2_BASE + 0x3C8)
#define    DMAC2_COMP_PARAM5        (DMAC2_BASE + 0x3D0)
#define    DMAC2_COMP_PARAM4        (DMAC2_BASE + 0x3D8)
#define    DMAC2_COMP_PARAM3        (DMAC2_BASE + 0x3E0)
#define    DMAC2_COMP_PARAM2        (DMAC2_BASE + 0x3E8)
#define    DMAC2_COMP_PARAM1        (DMAC2_BASE + 0x3F0)
#define    DMAC2_COMP_ID            (DMAC2_BASE + 0x3F8

/******************rtc***********************/
#define    RTC_BASE                 0xB0002000

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

/******************usb***********************/
#define    USB_BASE                 0xB1010000
#define    USB_DMA_BASE   			(USB_BASE + 0x200)
/******************vpu***********************/

#define    VPU_BASE                 0xB2026000
#define    VPU_ID                   0xB2026004
/******************gpu***********************/
#define    GPU_BASE                 0xB1013000

/******************pmu***********************/
#define    PMU_BASE                 0xB0001000
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
#define    PMU_INT_POL_SEL          (PMU_BASE + 0x058)
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
#define    PMU_PWR_ON_POL_SEL       (PMU_BASE + 0x094)
#define    PMU_RETURN_ADDR          (PMU_BASE + 0x098)
#define    PMU_INT                  (PMU_BASE + 0x09C)

#define    SSI1_BASE                0xB1004000
/*********************************lcdc***************************/

#define    LCDC_BASE                0xB2025000

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
#define    BAKGRND_COLOR    	    (LCDC_BASE + 0x0BC)

/*****************************************i2s*************************************************************/
#define    SYS_CTRL                 0xB0008000
#define    I2S_BASE                 0xB100A000

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

/**************************************i2c*********************************************/
#define    I2C_BASE                0xB1007000

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

#define int_enable(intnum)                            \
do{                                                   \
      if(intnum < 32)                                 \
          *(volatile unsigned int *)VIC_INT_EN_L |= (1 << intnum);          \
      else                                            \
          *(volatile unsigned int *)VIC_INT_EN_H |= (1 << (intnum - 32));   \
  }while(0)

#define int_disable(intnum)                           \
do{                                                   \
      if(intnum < 32)                                 \
          *(volatile unsigned int *)VIC_INT_EN_L &= ~(1 << intnum);        \
      else                                            \
          *(volatile unsigned int *)VIC_INT_EN_H &= ~(1 << (intnum - 32)); \
  }while(0)

#define int_clr(intnum)                           \
do{                                                   \
      if(intnum < 32)                                 \
          *(volatile unsigned int *)VIC_INT_EN_CLR_L &= ~(1 << intnum);        \
      else                                            \
          *(volatile unsigned int *)VIC_INT_EN_CLR_H &= ~(1 << (intnum - 32)); \
  }while(0)   

#endif
