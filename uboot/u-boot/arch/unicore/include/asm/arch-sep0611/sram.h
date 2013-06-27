#ifndef SRAM_H_
#define SRAM_H_

#include <asm/arch/vic.h>

#define Bank1_NOR_ADDR  (0x20000000)
#define Bank2_NOR_ADDR  (0x21000000)
#define ADDR16_SHIFT(x) (Bank1_NOR_ADDR + (x<<1))
#define NOR16_WRITE(Address, Data)   (*(U16*)(Address) = (Data))

#endif /*SRAM_H_*/
