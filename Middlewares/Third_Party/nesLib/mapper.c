#include "Mapper.h"
#include "common.h"

MAPPER mapper;


uint8_t write( uint16_t addr, uint8_t data ){ return data;}
uint8_t chr_write( uint16_t addr, uint8_t data ){ return data;}
void signal_scanline() {}


uint8_t read(uint16_t addr)
{
	if(addr >= 0x8000)
		return mapper.prg[mapper.prgMap[(addr-0x8000) /0x2000] + ((addr - 0x8000) % 0x2000)];
	else
		return mapper.prgRam[addr - 0x6000];
}


uint8_t chr_read(uint16_t addr)
{
	return mapper.chr[mapper.chrMap[addr / 0x400] + (addr % 0x400)];
}


void map_prg(int pageKBs, int slot, int bank) 
{
	int i;

	if(bank < 0 )
		bank = (mapper.prgSize / (0x400*pageKBs)) + bank;

	for(i = 0; i < (pageKBs/8); i++)
		mapper.prgMap[(pageKBs/8) * slot + i] = (pageKBs*0x400*bank + 0x2000*i) % mapper.prgSize;
}


void map_chr(int pageKBs, int slot, int bank)
{
	int i;
	for (int i = 0; i < pageKBs; i++)
		mapper.chrMap[pageKBs*slot + i] = (pageKBs*0x400*bank + 0x400*i) % mapper.chrSize;		

}

void mapper(uint8_t * rom)
{

}