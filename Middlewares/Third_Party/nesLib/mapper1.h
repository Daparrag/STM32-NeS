#ifndef _MAPPER1_H_
#define _MAPPER1_H_

#include "mapper.h"
#include "common.h"

typedef struct 
{
	int writeN;
	uint8_t tmpReg;
    uint8_t regs[4];
}Mapper1;

void MP1_apply();
void mapper1(uint8_t * rom);
uint8_t MP1_write (uint16_t addr, uint8_t v);
uint8_t MP1_chr_write (uint16_t addr, uint8_t v);

void mapper1_init();


#endif
