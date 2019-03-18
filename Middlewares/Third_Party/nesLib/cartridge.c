#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
#include "CPU_6502.h"
#include "ppu.h"
#include "cartridge.h"
#include "common.h"
#include "mapper.h"
#include "mapper0.h"
#include "mapper1.h"
#include "SDcard.h"


static unsigned char rom[MAXIMUM_ROM_SIZE] = {0xff}; //maximum rom size
/*accessing to PRG memory*/
uint8_t Cartridge_access(uint8_t modewr, uint16_t addr, uint8_t v)
{
	if(!modewr) return mapperStr.map_fun.read(addr);
	else  	return mapperStr.map_fun.write(addr,v);
}

/*accessing to CHR memory*/
uint8_t Cartridge_chr_access(uint8_t modewr, uint16_t addr, uint8_t v)
{
	if (!modewr) return mapperStr.map_fun.chr_read(addr);
	else 		 return mapperStr.map_fun.chr_write(addr,v);
}

/*cartriedge signal scanline*/
void Cartridge_signal_scanline(){
	mapperStr.map_fun.signal_scanline();
}


/* Load the ROM from a file. */
void load (const char * fileName)
{
	if(!SDcard_mount()) SDcard_init();

	ret = f_open (&file,fileName,FA_READ); if(ret) cartridge_err();
	ret = f_lseek(&file,f_size(&file));    if(ret) cartridge_err();
	int size = f_size(&file);
	f_lseek(&file,0);					   if(ret) cartridge_err();
	//uint8_t * rom  = (uint8_t *) malloc (size); if(!rom)cartridge_err();
	ret = f_read(&file,rom,size,&br); 		if (ret || !br) cartridge_err();
	ret = f_close(&file); 					if (ret) cartridge_err();

	int mapperNum = (rom[7] & 0xF0) | (rom[6] >> 4);
	if(Cartriedge_loaded()) mapper_delete();
	
	switch (mapperNum)
	{
		case 0: mapper0_init();break;
		case 1: mapper1_init();break;
	}
	mapperStr.map_fun.mapper(rom);

/*Fixme Start CPU, RESET PPU ...*/
	CPU_power();
	PPU_reset();

}


uint8_t Cartriedge_loaded()
{


	 return mapper_loaded();
}


void cartridge_err(){
	/*
	and error occour 

	*/
	while(1);

}
