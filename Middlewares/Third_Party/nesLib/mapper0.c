#include "mapper.h"
#include "mapper0.h"
#include "common.h"



void mapper0(uint8_t * rom){
	
	mapper(rom);
	map_prg(32, 0, 0);
	map_chr(8, 0, 0);
}


void mapper0_init(){
	mapperStr.map_fun.mapper = mapper0;
}
