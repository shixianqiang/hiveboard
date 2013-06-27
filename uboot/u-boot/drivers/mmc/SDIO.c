#include <common.h>
#include <mmc/sdio_hardware.h>
#include <mmc/sdio_typedef.h>
#include <mmc/SDIO1.h>
#include <mmc/SDIO1_protocol.h>
#include <mmc/sdio_DMAC.h>

#define SECTOR_SIZE 0x200
static int sdhc_flag = 0;
static int ready_flag = 0;

static void mydelay(U32 j)
{ 
	U16 i;
	for(i=0; i<j; i++);
}

static void sdInitial()
{
	int sdio_rca;
	printf("Initialize MMC controller!\n");
	sdio_rca = one_SDIO1_initial();
	update_clock_region();
	SDIO1_toggle_standby_transfer(sdio_rca);
	SDIO1_width_switch(sdio_rca, 0x2); //0x2 for 4 bit mode; 0x1 for 1 bit mode
	ready_flag = 1;
}

U16 one_SDIO1_initial(void)
{
    U32 resp;
    U32 temp;
    U16 rca_return;
	U32 css;
    //reset SD controller,FIFO,DMA
	
    resp = *(RP)SDIO1_RESP0; 
    *(RP)SDIO1_CTRL    =  SDIO1_ABT_RD_DAT(0)| SDIO1_SET_IRQ_RESP(0) | SDIO1_READ_WAIT(0) | SDIO1_DMA_EN  (0) \
                        | SDIO1_INT_EN    (0)| SDIO1_DMA_RST     (1) | SDIO1_FIFO_RST (1) | SDIO1_CTRL_RST(1);  
    //enable power to all cards, we have only one card
    *(RP)SDIO1_PWREN   = SDIO1_PWR_ON;
	*(RP)SDIO1_AHBCLKDIV = 0x8;

    // set the interrupt,unmask all the interrupt we have only one card
    *(RP)SDIO1_INTMASK =  SDIO1_INTMASK_EN  (1) | SDIO1_INTMASK_EBE (1) | SDIO1_INTMASK_ACD (0) | SDIO1_INTMASK_SBE (1) \
                        | SDIO1_INTMASK_HLE (1) | SDIO1_INTMASK_FRUN(1) | SDIO1_INTMASK_HTO (1) | SDIO1_INTMASK_DTO (1) \
                        | SDIO1_INTMASK_RTO (1) | SDIO1_INTMASK_DCRC(1) | SDIO1_INTMASK_RCRC(1) | SDIO1_INTMASK_RXDR(0) \
                        | SDIO1_INTMASK_TXDR(0) | SDIO1_INTMASK_DTP (1) | SDIO1_INTMASK_CP  (1) | SDIO1_INTMASK_RE  (1) \
                        | SDIO1_INTMASK_CD  (1);

    //clear int_status bits
    *(RP)SDIO1_RINTSTS = 0x1ffff; 
    //enable global interrput
	*(RP)SDIO1_CTRL    |=  SDIO1_INT_EN  (1) ;       
    // end set the interrput

    //set the card width as a single bit
    *(RP)SDIO1_CTYPE   =  SDIO1_CTYPE_WIDTH1;         
    
    //clock programming    
    *(RP)SDIO1_CLKENA  = SDIO1_CCLK_LW_PWR(0) | SDIO1_CCLK_EN(0);      //disable all clk and non_low_power mode
    
    //update_clock_register_only
    *(RP)SDIO1_CMD     =  SDIO1_CMD_START           | SDIO1_CMD_UPDATE_CLK_REG_ONLY(1) | SDIO1_CMD_CARD_NUM (0) \
                        | SDIO1_CMD_SEND_INIT    (0)| SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (1) \
                        | SDIO1_CMD_SET_STOP     (0)| SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
                        | SDIO1_CMD_HAVE_DAT_TRAN(0)| SDIO1_CMD_CHK_RESP_CRC       (0) | SDIO1_CMD_LONG_RESP(0) \
                        | SDIO1_CMD_RESP_EXPE    (0)| CMD0;  //update_clock_register_only  0x80202000
    
    mydelay(10);
    do
    {
        temp = *(RP)SDIO1_CMD & 0x80000000;
    } while(temp == 0x80000000);      				//wait the command taken by CIU 
    
    *(RP)SDIO1_CLKDIV  = SDIO1_CLK_DIV0(0x1f);      //180MHz
    *(RP)SDIO1_CLKSRC  = CLK_SRC(0);              	//set the clock source choose divider0
    
    *(RP)SDIO1_CMD     =  SDIO1_CMD_START            | SDIO1_CMD_UPDATE_CLK_REG_ONLY(1) | SDIO1_CMD_CARD_NUM (0) \
                        | SDIO1_CMD_SEND_INIT    (0) | SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (1) \
                        | SDIO1_CMD_SET_STOP     (0) | SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
                        | SDIO1_CMD_HAVE_DAT_TRAN(0) | SDIO1_CMD_CHK_RESP_CRC       (0) | SDIO1_CMD_LONG_RESP(0) \
                        | SDIO1_CMD_RESP_EXPE    (0) | CMD0;  //update_clock_register_only  0x80202000
                        
    mydelay(10);
    do
    {
        temp = *(RP)SDIO1_CMD & 0x80000000;
    } while(temp == 0x80000000);          	//wait the command taken by CIU 

    *(RP)SDIO1_CLKENA  = SDIO1_CCLK_LW_PWR(0) | SDIO1_CCLK_EN(1);         //enable non_low_power mode
    
    
    *(RP)SDIO1_CMD     =  SDIO1_CMD_START             | SDIO1_CMD_UPDATE_CLK_REG_ONLY(1) | SDIO1_CMD_CARD_NUM (0) \
                        | SDIO1_CMD_SEND_INIT     (0) | SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (1) \
                        | SDIO1_CMD_SET_STOP      (0) | SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
                        | SDIO1_CMD_HAVE_DAT_TRAN (0) | SDIO1_CMD_CHK_RESP_CRC       (0) | SDIO1_CMD_LONG_RESP(0) \
                        | SDIO1_CMD_RESP_EXPE     (0) | CMD0;  //update_clock_register_only  0x80202000
    
    mydelay(10);
    do
    {
        temp = *(RP)SDIO1_CMD & 0x80000000;
    } while(temp == 0x80000000);            //wait the command taken by CIU  
    
    
    // CMD0 below ,card in idle state after cmd0
    *(RP)SDIO1_CMDARG  = 0x0;             	//command does not require an argument,set to zero
    *(RP)SDIO1_CMD     =  SDIO1_CMD_START             | SDIO1_CMD_UPDATE_CLK_REG_ONLY(0) | SDIO1_CMD_CARD_NUM (0) \
                        | SDIO1_CMD_SEND_INIT     (1) | SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (0) \
                        | SDIO1_CMD_SET_STOP      (0) | SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
                        | SDIO1_CMD_HAVE_DAT_TRAN (0) | SDIO1_CMD_CHK_RESP_CRC       (0) | SDIO1_CMD_LONG_RESP(0) \
                        | SDIO1_CMD_RESP_EXPE     (0) | CMD0; //set CMD0 with send_initialization 0x80008000
   	mydelay(10);       
    temp = *(RP)SDIO1_RINTSTS & 0x4;      	// command done
    while(temp != 0x4)
    {
        temp = *(RP)SDIO1_RINTSTS & 0x4;
    }
   	temp = *(RP)SDIO1_RINTSTS;
    *(RP)SDIO1_RINTSTS = temp;            	//write to bits clear status bit
    mydelay(5);                             	//need to wait Ncc time at least 8 clock
    
    // CMD8 below	
    *(RP)SDIO1_CMDARG  = 0x1AA;          	//command does not require an argument,set to zero
    *(RP)SDIO1_CMD     =  SDIO1_CMD_START             | SDIO1_CMD_UPDATE_CLK_REG_ONLY(0) | SDIO1_CMD_CARD_NUM (0) \
                        | SDIO1_CMD_SEND_INIT     (0) | SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (0) \
                        | SDIO1_CMD_SET_STOP      (0) | SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
                        | SDIO1_CMD_HAVE_DAT_TRAN (0) | SDIO1_CMD_CHK_RESP_CRC       (1) | SDIO1_CMD_LONG_RESP(0) \
                        | SDIO1_CMD_RESP_EXPE     (1) | CMD8; 
    mydelay(10);       
    temp = *(RP)SDIO1_RINTSTS & 0x4;     	// command done
    while(temp != 0x4)
    {
        temp = *(RP)SDIO1_RINTSTS & 0x4;
    }
    temp = *(RP)SDIO1_RINTSTS;
    *(RP)SDIO1_RINTSTS = temp;            	//write to bits clear status bit
    mydelay(5);                            	//need to wait Ncc time at least 8 clock
    
    //CMD55 below,card in idle state after cmd0
    do
    {
        *(RP)SDIO1_CMDARG  = 0x0;        
        *(RP)SDIO1_CMD     =  SDIO1_CMD_START              | SDIO1_CMD_UPDATE_CLK_REG_ONLY(0) | SDIO1_CMD_CARD_NUM (0) \
                            | SDIO1_CMD_SEND_INIT     (0) | SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (0) \
                            | SDIO1_CMD_SET_STOP      (0) | SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
                            | SDIO1_CMD_HAVE_DAT_TRAN (0) | SDIO1_CMD_CHK_RESP_CRC       (1) | SDIO1_CMD_LONG_RESP(0) \
                            | SDIO1_CMD_RESP_EXPE     (1) | CMD55;
                            
        mydelay(1000);                        //need to wait Nrc time at least 8 clock 
        temp = *(RP)SDIO1_RINTSTS & 0x4; 	//command done
        while(temp != 0x4)
        {
            temp = *(RP)SDIO1_RINTSTS & 0x4;
        }
        temp = *(RP)SDIO1_RINTSTS;
       	*(RP)SDIO1_RINTSTS = temp;             //write to bits clear status bit

       	resp = *(RP)SDIO1_RESP0; 
       	//ACMD 41 below
       	*(RP)SDIO1_CMDARG  = 0xC1ff8000; 
       
		*(RP)SDIO1_CMD     = SDIO1_CMD_START              | SDIO1_CMD_UPDATE_CLK_REG_ONLY(0) | SDIO1_CMD_CARD_NUM (0) \
                          | SDIO1_CMD_SEND_INIT     (0) | SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (0) \
                          | SDIO1_CMD_SET_STOP      (0) | SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
                          | SDIO1_CMD_HAVE_DAT_TRAN (0) | SDIO1_CMD_CHK_RESP_CRC       (0) | SDIO1_CMD_LONG_RESP(0) \
                          | SDIO1_CMD_RESP_EXPE     (1) | ACMD41;
	  	mydelay(1000);
	   	temp = *(RP)SDIO1_RINTSTS & 0x4;       //command done
	   	while(temp != 0x4)
	 	{
	     	temp = *(RP)SDIO1_RINTSTS & 0x4;
	  	}
		temp = *(RP)SDIO1_RINTSTS;
       	*(RP)SDIO1_RINTSTS = temp;             //write to bits clear status bit
       	resp = (*(RP)SDIO1_RESP0 & 0x80000000);//OCR with busy
    } while(resp != 0x80000000);
    
    mydelay(10);                            //need to wait Nrc time at least 8 clock  
	css = (*(RP)SDIO1_RESP0 >> 30) & 0X1; 
	if (css == 0)
		sdhc_flag = 0;
	else 
		sdhc_flag = 1;
		
    // CMD2 below
    *(RP)SDIO1_CMDARG  = 0x0;           	//stuff bits
    *(RP)SDIO1_CMD     = SDIO1_CMD_START              | SDIO1_CMD_UPDATE_CLK_REG_ONLY(0) | SDIO1_CMD_CARD_NUM (0) \
                       | SDIO1_CMD_SEND_INIT     (0) | SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (0) \
                       | SDIO1_CMD_SET_STOP      (0) | SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
                       | SDIO1_CMD_HAVE_DAT_TRAN (0) | SDIO1_CMD_CHK_RESP_CRC       (1) | SDIO1_CMD_LONG_RESP(1) \
                       | SDIO1_CMD_RESP_EXPE     (1) | CMD2;  //long response except from card  0x800001c2
    temp = *(RP)SDIO1_RINTSTS & 0x4;    	//command done
    while(temp != 0x4)
    {
        temp = *(RP)SDIO1_RINTSTS & 0x4;
    }
    temp = *(RP)SDIO1_RINTSTS;
    *(RP)SDIO1_RINTSTS = temp;          	//write to bits clear status bit
    resp = *(RP)SDIO1_RESP3; 
    resp = *(RP)SDIO1_RESP2;
    resp = *(RP)SDIO1_RESP1; 
    resp = *(RP)SDIO1_RESP0;            
    mydelay(5);                        		//need to wait Nrc time at least 8 clock
    
    // CMD3 below, card is in ident state
    *(RP)SDIO1_CMDARG = 0x0;            	//set RCA=0x10,default RCA register is 0x0001
    *(RP)SDIO1_CMD    = SDIO1_CMD_START              | SDIO1_CMD_UPDATE_CLK_REG_ONLY(0) | SDIO1_CMD_CARD_NUM (0) \
                       | SDIO1_CMD_SEND_INIT     (0) | SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (0) \
                       | SDIO1_CMD_SET_STOP      (0) | SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
                       | SDIO1_CMD_HAVE_DAT_TRAN (0) | SDIO1_CMD_CHK_RESP_CRC       (1) | SDIO1_CMD_LONG_RESP(0) \
                       | SDIO1_CMD_RESP_EXPE     (1) | CMD3;  ////check respone from card  0x80000143
    temp = *(RP)SDIO1_RINTSTS & 0x4;    	//command done
    while(temp != 0x4)
    {
        temp = *(RP)SDIO1_RINTSTS & 0x4;
    }
    temp = *(RP)SDIO1_RINTSTS;
    *(RP)SDIO1_RINTSTS = temp;           	//write to bits clear status bit
    resp = *(RP)SDIO1_RESP0;
    rca_return=resp >>16;
    mydelay(5);                          		//need to wait Nrc time at least 8 clock
    return rca_return ;
}

void update_clock_region(void)
{
    U32 temp;
    *(RP)SDIO1_CLKENA = SDIO1_CCLK_LW_PWR(0) | SDIO1_CCLK_EN(0);
    *(RP)SDIO1_CMD    = SDIO1_CMD_START           | SDIO1_CMD_UPDATE_CLK_REG_ONLY(1) | SDIO1_CMD_CARD_NUM (0) \
                      | SDIO1_CMD_SEND_INIT    (0)| SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (1) \
                      | SDIO1_CMD_SET_STOP     (0)| SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
                      | SDIO1_CMD_HAVE_DAT_TRAN(0)| SDIO1_CMD_CHK_RESP_CRC       (0) | SDIO1_CMD_LONG_RESP(0) \
                      | SDIO1_CMD_RESP_EXPE    (0)| CMD0; 
    
    do
    {
        temp = *(RP)SDIO1_CMD & 0x80000000;
    }while(temp == 0x80000000);              //wait the command taken by CIU
    
    *(RP)SDIO1_CLKDIV = SDIO1_CLK_DIV0(0x0); 
    *(RP)SDIO1_CMD    = SDIO1_CMD_START           | SDIO1_CMD_UPDATE_CLK_REG_ONLY(1) | SDIO1_CMD_CARD_NUM (0) \
                      | SDIO1_CMD_SEND_INIT    (0)| SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (1) \
                      | SDIO1_CMD_SET_STOP     (0)| SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
                      | SDIO1_CMD_HAVE_DAT_TRAN(0)| SDIO1_CMD_CHK_RESP_CRC       (0) | SDIO1_CMD_LONG_RESP(0) \
                      | SDIO1_CMD_RESP_EXPE    (0)| CMD0; 
    do
    {
        temp = *(RP)SDIO1_CMD & 0x80000000;
    }while(temp == 0x80000000);              //wait the command taken by CIU
    *(RP)SDIO1_CLKENA = SDIO1_CCLK_LW_PWR(0) | SDIO1_CCLK_EN(1);
    
    *(RP)SDIO1_CMD    = SDIO1_CMD_START           | SDIO1_CMD_UPDATE_CLK_REG_ONLY(1) | SDIO1_CMD_CARD_NUM (0) \
                      | SDIO1_CMD_SEND_INIT    (0)| SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (1) \
                      | SDIO1_CMD_SET_STOP     (0)| SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
                      | SDIO1_CMD_HAVE_DAT_TRAN(0)| SDIO1_CMD_CHK_RESP_CRC       (0) | SDIO1_CMD_LONG_RESP(0) \
                      | SDIO1_CMD_RESP_EXPE    (0)| CMD0; 
    do
    {
        temp = *(RP)SDIO1_CMD & 0x80000000;
    }while(temp == 0x80000000);              //wait the command taken by CIU
}

void SDIO1_toggle_standby_transfer(U16 rca)
{
    U32 rca_temp = rca <<16;
    U32 resp;
    // --send CMD13 first(Address card sends tis status register)
    resp = cmd_response_resp(rca_temp,CMD13); 	// card is in standby state
    // --send CMD7 (toggle the card between the stand_by and transfer states)
    resp = cmd_response_resp(rca_temp,CMD7);	// card is in standby state
    // --send CMD13 (Address card sends tis status register)
    resp = cmd_response_resp(rca_temp,CMD13);	// card is in tran state
}

void SDIO1_width_switch(U16 rca, U16 width)
{
    U32 temp;
	*(RP)SDIO1_CMDARG  = rca << 16;        
    *(RP)SDIO1_CMD     =  SDIO1_CMD_START              | SDIO1_CMD_UPDATE_CLK_REG_ONLY(0) | SDIO1_CMD_CARD_NUM (0) \
                         | SDIO1_CMD_SEND_INIT     (0) | SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (0) \
                         | SDIO1_CMD_SET_STOP      (0) | SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
                         | SDIO1_CMD_HAVE_DAT_TRAN (0) | SDIO1_CMD_CHK_RESP_CRC       (1) | SDIO1_CMD_LONG_RESP(0) \
                         | SDIO1_CMD_RESP_EXPE     (1) | CMD55;
                         
    mydelay(10);                        	//need to wait Nrc time at least 8 clock 
    temp = *(RP)SDIO1_RINTSTS & 0x4; 	//command done
    while(temp != 0x4)
   	{
     	temp = *(RP)SDIO1_RINTSTS & 0x4;
   	}
    temp = *(RP)SDIO1_RINTSTS;
    *(RP)SDIO1_RINTSTS = temp;       	//write to bits clear status bit
           
    //ACMD6 below
    *(RP)SDIO1_CMDARG  = width; 		// 0x0:1 bit mode; 0x2: 4 bit mode
    *(RP)SDIO1_CMD     = SDIO1_CMD_START              | SDIO1_CMD_UPDATE_CLK_REG_ONLY(0) | SDIO1_CMD_CARD_NUM (0) \
                        | SDIO1_CMD_SEND_INIT     (0) | SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (0) \
                        | SDIO1_CMD_SET_STOP      (0) | SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
                        | SDIO1_CMD_HAVE_DAT_TRAN (0) | SDIO1_CMD_CHK_RESP_CRC       (0) | SDIO1_CMD_LONG_RESP(0) \
                        | SDIO1_CMD_RESP_EXPE     (1) | ACMD6; 
                         	   
    mydelay(1000);
    temp = *(RP)SDIO1_RINTSTS & 0x4; 	//command done
    while(temp != 0x4)
    {
  		temp = *(RP)SDIO1_RINTSTS & 0x4;
    }
    temp = *(RP)SDIO1_RINTSTS;
    *(RP)SDIO1_RINTSTS = temp;    
}


U32 cmd_response_resp(U32 cmdarg, U32 index)
{
    U32 temp;
    U32 resp0;
    *(RP)SDIO1_CMDARG = cmdarg;
    *(RP)SDIO1_CMD    =  SDIO1_CMD_START           | SDIO1_CMD_UPDATE_CLK_REG_ONLY(0) | SDIO1_CMD_CARD_NUM (0) \
                       | SDIO1_CMD_SEND_INIT    (0)| SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (1) \
                       | SDIO1_CMD_SET_STOP     (0)| SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
                       | SDIO1_CMD_HAVE_DAT_TRAN(0)| SDIO1_CMD_CHK_RESP_CRC       (1) | SDIO1_CMD_LONG_RESP(0) \
                       | SDIO1_CMD_RESP_EXPE    (1)| index;  //response except,check_response_crc,wait_prvdata_complete, cardnum <<16 | index | 0x80002140
    	
    temp = *(RP)SDIO1_RINTSTS & 0x4;          //command done
    while(temp != 0x4)
    {
        temp = *(RP)SDIO1_RINTSTS & 0x4;
    }
    temp = *(RP)SDIO1_RINTSTS;
    *(RP)SDIO1_RINTSTS = temp;                //write to bits clear status bit
    resp0 = *(RP)SDIO1_RESP0;
    return *(RP)SDIO1_RESP0;      
}

//read mulitple block test
void ReadMMC(U32 sector_start, S32 nob, U32 ram_start)
{   
	U32 temp;
    U32 nob_temp;
    U32 word_len;
	if(!ready_flag)
		 sdInitial();
	printf("Read From MMC!\n");
    // --set the translate parameter
	*(RP)SDIO1_CTYPE   =  SDIO1_CTYPE_WIDTH4;                 
    *(RP)SDIO1_BLKSIZ  =  SECTOR_SIZE;                //block size 512 bytes then we have 16 blocks to write
    *(RP)SDIO1_FIFOTH  =  MSIZE8 | RX_WMARK(7) | TX_WMARK(8);           //MSize = 8,TX_Wmark = 8,RX_WMark = 7
    *(RP)SDIO1_CTRL   |= SDIO1_FIFO_RST (1); 
    *(RP)SDIO1_CTRL   |= SDIO1_DMA_EN(1);
    for (; ;)
    {
    	if ((SECTOR_SIZE * nob) < 0x2000)
    	{
    		nob_temp = nob;
    		word_len = (nob_temp * SECTOR_SIZE)/4 ;
    	}
    	else 
    	{
    		nob_temp = 0x2000/SECTOR_SIZE;
    		word_len = 0x2000 >> 2;
    	}
    	*(RP)SDIO1_BYTCNT  =  nob_temp * SECTOR_SIZE; 

		*((RP)(DMAC1_CLRTFR+4)  ) |=  0 ;
	    *((RP)(DMAC1_CLRTFR)    ) |=  0x1 ; 
		*((RP)(DMAC1_CLRBLK+4)  ) |=  0 ;
	    *((RP)(DMAC1_CLRBLK)    ) |=  0x1 ; 
		*((RP)(DMAC1_CLRSRCTR+4)) |=  0 ;
	    *((RP)(DMAC1_CLRSRCTR)  ) |=  0x1 ; 
		*((RP)(DMAC1_CLRDSTTR+4)) |=  0 ;
	    *((RP)(DMAC1_CLRDSTTR)  ) |=  0x1 ; 
		*((RP)(DMAC1_CLRERR+4)  ) |=  0 ;
	    *((RP)(DMAC1_CLRERR)    ) |=  0x1 ; 
	    
	    // Initial ESRAM for verification
	    
	    // Write the starting source address in the SARx register for channel 0
	    *((RP)(DMAC1_SAR0+4))   =   0 ; 
	    *((RP)(DMAC1_SAR0)  )   =   SDIO1_DATA;

	    // Write the starting destination address in the SARx register for channel 0
	    *((RP)(DMAC1_DAR0+4))   =   0 ; 
	    *((RP)(DMAC1_DAR0)  )   =   ram_start;    
	    
	    // Programe CTLx and CFGx, for SD to ddr2, DMAC_DST_MASTER_SEL=0
		*((RP)(DMAC1_CTL0+4))   =   DMAC_DONE          (0) | DMAC_BLK_TRAN_SZ       (word_len);
		*((RP)(DMAC1_CTL0)  )   =   DMAC_SRC_LLP_EN    (0) | DMAC_DST_LLP_EN        (0) | DMAC_SRC_MASTER_SEL(1) \
		                          | DMAC_DST_MASTER_SEL(0) | DMAC_TRAN_TYPE_FLOW_CTL(2) | DMAC_DST_SCAT_EN   (0) \
		                          | DMAC_SRC_GATH_EN   (0) | DMAC_SRC_BST_SZ        (2) | DMAC_DST_BST_SZ    (2) \
		                          | DMAC_SRC_INCR      (3) | DMAC_DST_INCR          (0) | DMAC_SRC_TRAN_WIDTH(2) \
	                              | DMAC_DST_TRAN_WIDTH(2) | DMAC_INT_EN(1);
	                              				
	   	// Channel 0 Configuration Register
		*((RP)(DMAC1_CFG0+4))  =   DMAC_DST_PER        (0) | DMAC_SRC_PER  (0) | DMAC_SRC_STAT_UPD_EN(0) \
		                         | DMAC_DST_STAT_UPD_EN(0) | DMAC_PROT_CTL (1) | DMAC_FIFO_MODE      (1) \
		                         | DMAC_FLOW_CTL_MODE  (0); 
		*((RP)(DMAC1_CFG0)  )  =   DMAC_AUTO_RELOAD_DST(0) | DMAC_AUTO_RELOAD_SRC(0) | DMAC_MAX_AMBA_BST_LEN(0) \
		                         | DMAC_SRC_HS_POL     (0) | DMAC_DST_HS_POL     (0) | DMAC_BUS_LOCK        (0) \
		                         | DMAC_CH_LOCK        (0) | DMAC_BUS_LOCK_LEVEL (0) | DMAC_HW_SW_SEL_SRC   (0) \
		                         | DMAC_HW_SW_SEL_DST  (1) | DMAC_CH_SUSP        (0) | DMAC_CH_PRIOR        (0);
		                         
		// Programe the LLPx register with 0
	    *((RP)(DMAC1_LLP0+4))  =   0 ;
	    *((RP)(DMAC1_LLP0  ))  =   DMAC_LOC(0) | DMAC_LIST_MASTER_SEL(0);  
	    
	    // If Scatter is enabled, programe the DSRx register for channel x
	    *((RP)(DMAC1_DSR0+4))  =   0 ;
	    *((RP)(DMAC1_DSR0  ))  =   DMAC_DST_SCAT_INTERVAL(0x4) | DMAC_DST_SCAT_CNT(4);
	    
	     //If Gather is enabled, programe the SGRx register for channel x
	    *((RP)(DMAC1_SGR0+4))  =   0 ;
	    *((RP)(DMAC1_SGR0  ))  =   DMAC_SRC_GATH_INTERVAL(0x4) | DMAC_SRC_GATH_CNT(4);
	    // Enable channel
		*((RP)(DMAC1_CHENA+4)) =   0 ;
	    *((RP)(DMAC1_CHENA  )) =   DMAC_CH_WRITE_EN(1) | DMAC_CH_EN(1);
	    // --send CMD18 (read mulitple block)
		if(sdhc_flag == 0x1)//SDHC
	    	*(RP)SDIO1_CMDARG = sector_start;
	    else //SDCard
	    	*(RP)SDIO1_CMDARG = sector_start * SECTOR_SIZE;
	    *(RP)SDIO1_CMD    = SDIO1_CMD_START            | SDIO1_CMD_UPDATE_CLK_REG_ONLY(0) | SDIO1_CMD_CARD_NUM (0) \
	                      | SDIO1_CMD_SEND_INIT    (0)| SDIO1_CMD_STP_ABT            (0) | SDIO1_CMD_WAIT_DAT (1) \
	                      | SDIO1_CMD_SET_STOP     (1)| SDIO1_CMD_BLK_TRAN           (0) | SDIO1_CMD_WRITE    (0) \
	                      | SDIO1_CMD_HAVE_DAT_TRAN(1)| SDIO1_CMD_CHK_RESP_CRC       (1) | SDIO1_CMD_LONG_RESP(0) \
	                      | SDIO1_CMD_RESP_EXPE    (1)| CMD18;  //read mulitple_block and no_set_auto_stop cardnum <<16 | 0x80003352
	    temp = *(RP)SDIO1_RINTSTS & 0x8;          //transmit FIFO data request
	    while(temp != 0x8)
	    {
	        temp = *(RP)SDIO1_RINTSTS & 0x8;
	    }
	    temp = *(RP)SDIO1_RINTSTS;
	    *(RP)SDIO1_RINTSTS = temp;                //write to bits clear status bit   
	    nob -= (0x2000 / SECTOR_SIZE);
	    sector_start = sector_start + 0x10;
	    ram_start = ram_start + 0x2000;
	    
	    if (ram_start > 0x48000000)
	    ram_start = 0x41000000; 
	    
	    if (nob <=0)
	    {
	    	break;
	    }
  	}                                                                                                       
}
