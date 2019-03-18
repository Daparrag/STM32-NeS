#include <stdlib.h>
#include "mapper.h"
#include "ppu.h"
#include "common.h"


MAPPER mapperStr;
static uint8_t mapped_ld =FALSE;

uint8_t write( uint16_t addr, uint8_t data ){ return data;}
uint8_t chr_write( uint16_t addr, uint8_t data ){ return data;}
void signal_scanline() {}


uint8_t read(uint16_t addr)
{
	if(addr >= 0x8000)
		return mapperStr.prg[mapperStr.prgMap[(addr-0x8000) /0x2000] + ((addr - 0x8000) % 0x2000)];
	else
		return mapperStr.prgRam[addr - 0x6000];
}


uint8_t chr_read(uint16_t addr)
{
	return mapperStr.chr[mapperStr.chrMap[addr / 0x400] + (addr % 0x400)];
}


void map_prg(int pageKBs, int slot, int bank) 
{
	int i;

	if(bank < 0 )
		bank = (mapperStr.prgSize / (0x400*pageKBs)) + bank;

	for(i = 0; i < (pageKBs/8); i++)
		mapperStr.prgMap[(pageKBs/8) * slot + i] = (pageKBs*0x400*bank + 0x2000*i) % mapperStr.prgSize;
}


void map_chr(int pageKBs, int slot, int bank)
{
	int i;
	for (i = 0; i < pageKBs; i++)
		mapperStr.chrMap[pageKBs*slot + i] = (pageKBs*0x400*bank + 0x400*i) % mapperStr.chrSize;

}

void mapper(uint8_t * rom)
{
	mapperStr.rom = rom;
	//Reading from header
	mapperStr.prgSize = rom[4] * 0x4000;
	mapperStr.chrSize = rom[5] * 0x2000;
	mapperStr.prgRamSize   = rom[8] ? rom[8] * 0x2000 : 0x2000;
	set_mirroring((rom[6] & 1) ? VERTICAL : HORIZONTAL);
	mapperStr.prg = rom + 16;
	mapperStr.prgRam = (uint8_t *) malloc(mapperStr.prgRamSize * sizeof(uint8_t));
	if(!mapperStr.prgRam)while(1);

	//CHR ROM:
	if(mapperStr.chrSize)
		mapperStr.chr = rom +16 +mapperStr.prgSize;
	//CHR RAM:
	else
	{
		mapperStr.chrRam  = TRUE;
		mapperStr.chrSize = 0x2000;
		mapperStr.chr = (uint8_t *) malloc(mapperStr.chrSize * sizeof(uint8_t));
		if(!mapperStr.chr)while(1);
	}
	mapperStr.map_fun.chr_write = chr_write;
	mapperStr.map_fun.chr_read = chr_read;
	mapperStr.map_fun.read = read;
	mapperStr.map_fun.write = write;
	mapperStr.map_fun.signal_scanline = signal_scanline;
	mapped_ld=TRUE;
}


uint8_t mapper_loaded(){ return mapped_ld;}


void mapper_delete()
{

	free(mapperStr.rom);
	free(mapperStr.prgRam);
	if(mapperStr.chrRam)
		free(mapperStr.chr);
	mapped_ld =FALSE;

}
