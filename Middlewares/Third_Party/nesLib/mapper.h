#ifndef _MAPPER_H_
#define _MAPPER_H_
#include <stdio.h>
#include "common.h"




typedef struct
{
	uint8_t	(*read)( uint16_t addr );
	uint8_t	(*write)( uint16_t addr, uint8_t v );
	uint8_t	(*chr_read)( uint16_t addr );
	uint8_t (*chr_write)( uint16_t addr, uint8_t v );
	void 	(*signal_scanline)( void );
	void  (*mapper) (uint8_t * rom);

}fmapper;



typedef struct 
{
	uint8_t * rom;
	uint8_t chrRam;
	uint32_t prgMap[4];
	uint32_t chrMap[8];

	/* mapper functions */
	 fmapper map_fun;
	 uint8_t *prg, *chr, *prgRam;
	 uint32_t prgSize, chrSize, prgRamSize;
}MAPPER;

extern MAPPER mapperStr;

void mapper(uint8_t * rom);
void map_prg(int pageKBs, int slot, int bank);
void map_chr(int pageKBs, int slot, int bank);
uint8_t mapper_loaded();
void mapper_delete();

#endif
