#ifndef __SDIO_SDIO1_PROTOCOL_H_
#define __SDIO_SDIO1_PROTOCOL_H_
/*------commands                                            type    argument                response */
/*Basic Commands, Class 0 */
#define    CMD0         0      /* SD_GO_IDLE_STATE           bc     [31:0] stuff bits            -    */
#define    CMD1         1      /* MMC_SEND_OP_COND               */
#define    CMD2         2      /* SD_ALL_SEND_CID            bcr    [31:0] stuff bits            R2   */
#define    CMD3         3      /* SD_SET_RELATIVE_ADDR       bcr    [31:0] stuff bits            R1   */
#define    CMD4         4      /* SD_SET_DSR                 bc     [31:16] DSR [15:0]stuff bits R1b  */
#define    CMD5         5      /* SDIO_SEND_OP_COND             */
#define    CMD6         6      /* SDIO_SWITCH_FUNC              */
#define    CMD7         7      /* SD_SELECT/DESELECTED_CARD  ac     [31:16] DSR [15:0]stuff bits R1b             R1b  */
#define    CMD8         8 
#define    CMD9         9      /* SD_SEND_CSD                ac     [31:16] RCA [15:0]stuff bits R2  */
#define    CMD10        10     /* SD_SEND_CID                ac     [31:16] RCA [15:0]stuff bits R2  */

#define    CMD12        12     /* SD_STOP_TRANSMISSION       ac     [31:0]stuff bits             R1b */
#define    CMD13        13     /* SD_SEND_STATUS             ac     [31:16] RCA [15:0]stuff bits R1  */

#define    CMD15        15     /* SD_GO_INACTIVE_STATE       ac     [31:16] RCA [15:0]stuff bits -   */
// CMD6, CMD8,CMD11, CMD14 are reserved commands
 
/* Block Oriented Read Commands, Class 2 */
#define    CMD16        16     /* SD_SET_BLOCKLEN            ac     [31:0] block length           R1 */
#define    CMD17        17     /* SD_READ_SINGLE_BLOCK adtc  adtc   [31:0] data address           R1 */
#define    CMD18        18     /* SD_READ_MULTIPLE_BLOCK     adtc   [31:0] data address           R1 */
//CMD19~CMD23 are reserved commands

/* Block Oriented Write Commands, Class 4 */
#define    CMD24        24     /* SD_WRITE_BLOCK            adtc    [31:0] data address           R1 */
#define    CMD25        25     /* SD_WRITE_MULTIPLE_BLOCK   adtc    [31:0] data address           R1 */
#define    CMD27        27     /* SD_PROGRAM_CSD            adtc    [31:0] stuff bits             R1 */               R1  */
//CMD26 is reserved for manufacture


/* Block Orineted Write Protection Commands, Class 6 */
#define    CMD28        28    /* SD_SET_WRITE_PROT          ac      [31:0] data addr             R1b */
#define    CMD29        29    /* SD_CLR_WRITE_PROT          ac      [31:0] data addr             R1b */
#define    CMD30        30    /* SD_SEND_WRITE_PROT         adtc    [31:0] write protect data address R1 */
// CMD31 is reserved command


/* Erase Commands, Class 5 */
#define    CMD32        32     /* SD_ERASE_WR_BLK_START   ac       [31:0] data address           R1  */
#define    CMD33        33     /* SD_ERASE_WR_BLK_END     ac       [31:0] data addressss         R1  */
#define    CMD38        38     /* ERASE                   ac       [31:0] stuff bits             R1b */  

// CMD34~CMD37,CMD39,CMD40~CMD41 are reserved commands

/* Lock Card Commands,Class 7*/
#define    CMD42        42     /* SD_LOCK_UNLOCK          adtc     [31:0] stuff bits             R1 */            
// CMD43~CMD54 are reserved commands

/* Application Specific Commands,Class 8 */
#define    CMD55        55     /* SD_APP_CMD              ac       [31:16] RCA [15:0]stuff bits  R1  */
#define    CMD56        56     /* SD_GEN_CMD              adtc     [31:1] stuff bits [0] RD/WR   R1  */
//CMD57~CMD63 are reserved commands

/* Application Specific Commands used/reserved by SD Memory Card*/
#define    ACMD6        6      /* SD_SET_BUS_WIDTH        ac       [31:2] stuff bits [1:0] bus width R1*/
#define    ACMD13       13     /* SD_STATUS               adtc     [31:0] stuff bits              R1  */
#define    ACMD22       22     /* SD_SEND_NUM_WR_BLOCKS   adtc     [31:0] stuff bits              R1  */
#define    ACMD23       23     /* SD_SET_WR_BLK_ERASE_COUNT ac     [31:23]stuff bits [22:0] number of blocks R1 */
#define    ACMD41       41     /* SD_APP_OP_COND          bcr      [31:0] OCR without busy        R3  */
#define    ACMD42       42     /* SD_SET_CLR_CARD_DETECT  ac       [31:1] stuff bits [0]set_sd    R1  */
#define    ACMD51       51     /* SD_SEND_SCR             adtc     [31:0] staff bits              R1  */
// ACMD17~ACMD21, ACMD24~ACMD40,ACMD43~ACMD49 are reserved commands


#endif

