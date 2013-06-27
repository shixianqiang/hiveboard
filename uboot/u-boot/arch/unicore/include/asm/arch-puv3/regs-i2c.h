/*
 * PKUnity Inter-integrated Circuit (I2C) Registers
 */
#define I2C_CON		__REG(PKUNITY_I2C_BASE + 0x0000) /* Control Reg. */
#define I2C_TAR		__REG(PKUNITY_I2C_BASE + 0x0004) /* Target Address Reg. */
#define I2C_DATACMD	__REG(PKUNITY_I2C_BASE + 0x0010) /* Data buffer and command Reg. */
#define I2C_ENABLE	__REG(PKUNITY_I2C_BASE + 0x006C) /* Enable Reg. */
#define I2C_STATUS	__REG(PKUNITY_I2C_BASE + 0x0070) /* Status Reg. */
#define I2C_TXFLR	__REG(PKUNITY_I2C_BASE + 0x0074) /* Tx FIFO Length Reg. */
#define I2C_RXFLR	__REG(PKUNITY_I2C_BASE + 0x0078) /* Rx FIFO Length Reg. */

#define I2C_CON_MASTER          FIELD(1, 1, 0)
#define I2C_CON_SPEED_STD       FIELD(1, 2, 1)
#define I2C_CON_SPEED_FAST      FIELD(2, 2, 1)
#define I2C_CON_RESTART         FIELD(1, 1, 5)
#define I2C_CON_SLAVEDISABLE    FIELD(1, 1, 6)

#define I2C_DATACMD_READ        FIELD(1, 1, 8)
#define I2C_DATACMD_WRITE       FIELD(0, 1, 8)
#define I2C_DATACMD_DAT_MASK    FMASK(8, 0)
#define I2C_DATACMD_DAT(v)      FIELD((v), 8, 0)

#define I2C_ENABLE_ENABLE       FIELD(1, 1, 0)
#define I2C_ENABLE_DISABLE      FIELD(0, 1, 0)

#define I2C_STATUS_RFF          FIELD(1, 1, 4)
#define I2C_STATUS_RFNE         FIELD(1, 1, 3)
#define I2C_STATUS_TFE          FIELD(1, 1, 2)
#define I2C_STATUS_TFNF         FIELD(1, 1, 1)
#define I2C_STATUS_ACTIVITY     FIELD(1, 1, 0)

#ifdef CONFIG_PUV3_NB0916
#define I2C_TAR_SPD	0x50
#define I2C_TAR_PWIC    0x55
#define I2C_TAR_EEPROM	0x57
#endif
#ifdef CONFIG_PUV3_SMW0919
#define I2C_TAR_SPD	0x50
#define I2C_TAR_EEPROM	0x57
#endif
#ifdef CONFIG_PUV3_DB0923
#define I2C_TAR_SPD	0x50
#define I2C_TAR_EEPROM	0x57
#endif
#ifdef CONFIG_PUV3_HNC
#define I2C_TAR_SPD	0x50
#define I2C_TAR_EEPROM	0x57
#endif
