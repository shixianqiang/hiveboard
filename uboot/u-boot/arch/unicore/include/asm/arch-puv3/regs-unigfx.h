/*
 * PKUnity UNIGFX Registers
 */


#define UDE_BASE      (PKUNITY_UNIGFX_BASE + 0x1400)
#define UGE_BASE      (PKUNITY_UNIGFX_BASE + 0x0000)

/* command reg for UNIGFX DE */
#define UDE_CFG       __REG(UDE_BASE + 0x0000) /* control reg */
#define UDE_FSA       __REG(UDE_BASE + 0x0004) /* framebuffer start address reg */
#define UDE_LS        __REG(UDE_BASE + 0x0008) /* line size reg */
#define UDE_PS        __REG(UDE_BASE + 0x000C) /* pitch size reg */
#define UDE_HAT       __REG(UDE_BASE + 0x0010) /* horizontal active time reg */  
#define UDE_HBT       __REG(UDE_BASE + 0x0014) /* horizontal blank time reg */
#define UDE_HST       __REG(UDE_BASE + 0x0018) /* horizontal sync time reg */ 
#define UDE_VAT       __REG(UDE_BASE + 0x001C) /* vertival active time reg */
#define UDE_VBT       __REG(UDE_BASE + 0x0020) /* vertival blank time reg */
#define UDE_VST       __REG(UDE_BASE + 0x0024) /* vertival sync time reg */
#define UDE_CXY       __REG(UDE_BASE + 0x0028) /* cursor position */
#define UDE_CC0       __REG(UDE_BASE + 0x002C) /* cursor front color */
#define UDE_CC1       __REG(UDE_BASE + 0x0030) /* cursor background color */
#define UDE_VXY       __REG(UDE_BASE + 0x0034) /* video position */
#define UDE_VSA       __REG(UDE_BASE + 0x0040) /* video start address reg */
#define UDE_VS        __REG(UDE_BASE + 0x004C) /* video size reg */

/* command reg for UNIGFX GE */
#define UGE_SRCXY     __REG(UGE_BASE + 0x0000) /* src xy reg */	
#define UGE_DSTXY     __REG(UGE_BASE + 0x0004) /* dst xy reg */
#define UGE_PITCH     __REG(UGE_BASE + 0x0008) /* pitch reg */
#define UGE_SRCSTART  __REG(UGE_BASE + 0x000C) /* src start reg */	
#define UGE_DSTSTART  __REG(UGE_BASE + 0x0010) /* dst start reg */	
#define UGE_WIDHEIGHT __REG(UGE_BASE + 0x0014) /* width height reg */
#define UGE_ROPALPHA  __REG(UGE_BASE + 0x0018) /* rop alpah reg */
#define UGE_FCOLOR    __REG(UGE_BASE + 0x001C) /* front color */
#define UGE_BCOLOR    __REG(UGE_BASE + 0x0020) /* background color */
#define UGE_SCH       __REG(UGE_BASE + 0x0024) /* src color key for high value */
#define UGE_DCH       __REG(UGE_BASE + 0x0028) /* dst color key for high value */
#define UGE_SCL       __REG(UGE_BASE + 0x002C) /* src color key for low value */
#define UGE_DCL       __REG(UGE_BASE + 0x0030) /* dst color key for low value */
#define UGE_CLIP0     __REG(UGE_BASE + 0x0034) /* clip 0 reg */
#define UGE_CLIP1     __REG(UGE_BASE + 0x0038) /* clip 1 reg */
#define UGE_COMMAND   __REG(UGE_BASE + 0x003C) /* command reg */
#define UGE_P0        __REG(UGE_BASE + 0x0040) /* pattern 0 */
#define UGE_P1        __REG(UGE_BASE + 0x0044)
#define UGE_P2        __REG(UGE_BASE + 0x0048)
#define UGE_P3        __REG(UGE_BASE + 0x004C)
#define UGE_P4        __REG(UGE_BASE + 0x0050)
#define UGE_P5        __REG(UGE_BASE + 0x0054)
#define UGE_P6        __REG(UGE_BASE + 0x0058)
#define UGE_P7        __REG(UGE_BASE + 0x005C)
#define UGE_P8        __REG(UGE_BASE + 0x0060)
#define UGE_P9        __REG(UGE_BASE + 0x0064)
#define UGE_P10       __REG(UGE_BASE + 0x0068)
#define UGE_P11       __REG(UGE_BASE + 0x006C)
#define UGE_P12       __REG(UGE_BASE + 0x0070)
#define UGE_P13       __REG(UGE_BASE + 0x0074)
#define UGE_P14       __REG(UGE_BASE + 0x0078)
#define UGE_P15       __REG(UGE_BASE + 0x007C)
#define UGE_P16       __REG(UGE_BASE + 0x0080)
#define UGE_P17       __REG(UGE_BASE + 0x0084)
#define UGE_P18       __REG(UGE_BASE + 0x0088)
#define UGE_P19       __REG(UGE_BASE + 0x008C)
#define UGE_P20       __REG(UGE_BASE + 0x0090)
#define UGE_P21       __REG(UGE_BASE + 0x0094)
#define UGE_P22       __REG(UGE_BASE + 0x0098)
#define UGE_P23       __REG(UGE_BASE + 0x009C)
#define UGE_P24       __REG(UGE_BASE + 0x00A0)
#define UGE_P25       __REG(UGE_BASE + 0x00A4)
#define UGE_P26       __REG(UGE_BASE + 0x00A8)
#define UGE_P27       __REG(UGE_BASE + 0x00AC)
#define UGE_P28       __REG(UGE_BASE + 0x00B0)
#define UGE_P29       __REG(UGE_BASE + 0x00B4)
#define UGE_P30       __REG(UGE_BASE + 0x00B8)
#define UGE_P31       __REG(UGE_BASE + 0x00BC)

#define UDE_CFG_DST_MASK	FMASK(2, 8)
#define UDE_CFG_DST8            FIELD(0x0, 2, 8)
#define UDE_CFG_DST16           FIELD(0x1, 2, 8)
#define UDE_CFG_DST24           FIELD(0x2, 2, 8)
#define UDE_CFG_DST32           FIELD(0x3, 2, 8)

#define UDE_CFG_GDEN_ENABLE     FIELD(1, 1, 3) /* GDEN enable */
#define UDE_CFG_VDEN_ENABLE     FIELD(1, 1, 4) /* VDEN enable */
#define UDE_CFG_CDEN_ENABLE     FIELD(1, 1, 5) /* CDEN enable */
#define UDE_CFG_TIMEUP_ENABLE   FIELD(1, 1, 6) /* TIMEUP enable */
