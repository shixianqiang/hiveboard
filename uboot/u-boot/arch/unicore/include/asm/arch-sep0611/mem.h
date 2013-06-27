/*********************************************************************
* Filename    :  mem.h
* Author      :  SixRoom
* Date        :  2011-01-12
* Description :  
* Version  	  :  v1.00
**********************************************************************/
#ifndef MEM_H_
#define MEM_H_
#include 	<asm/arch/vic.h>

void RegRWVerify(U32 pRegAddr, REG_VLD_WIDTH pRegVldWidth);
void RegRstValueVerify(U32 pRegAddr, U32 pRegInitValue, REG_VLD_WIDTH pRegVldWidth);
U64 ReadMemValue(U32 pAddr, U32 pBitWidth, U32 pIncrDir);
void WriteMemValue(U32 pAddr, U32 pBitWidth, U32 pIncrDir, U64 pWrData);
void MemInitData(U32 pSrcAddr, U32 pDstAddr, U32 pBlkSz, U32 pSrcWidth, U32 pDstWidth, U32 pSrcIncr, U32 pDstIncr);
void MemInitSrcData(U32 pSrcAddr, U32 pBlkSz, U32 pSrcWidth, U32 pSrcIncr);
void MemCheck(U32 pSrcAddr, U32 pDstAddr, U32 pBlkSz, U32 pSrcWidth, U32 pDstWidth, U32 pSrcIncr, U32 pDstIncr) ;

#endif /*MEM_H_*/
