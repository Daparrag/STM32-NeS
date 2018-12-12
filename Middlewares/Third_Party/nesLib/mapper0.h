#ifndef _MAPPER0_H_
#define _MAPPER0_H_
#include "mapper.h"
#include "common.h"


void Mapper0_init(){

}


void mapper0(uint8_t * rom){

	MAPPER_init(rom);

	map_prg(32, 0, 0);
	map_chr(8, 0, 0);

}


#endif
