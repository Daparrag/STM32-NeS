#ifndef _CARTRIDGE_H_
#define _CARTRIDGE_H_

#include <stdio.h>
#include "common.h"




uint8_t Cartridge_access(uint8_t modewr, uint16_t addr, uint8_t v);
uint8_t Cartridge_chr_access(uint8_t modewr, uint16_t addr, uint8_t v);

void Cartridge_signal_scanline();
void load (const char * fileName);
uint8_t Cartriedge_loaded();
void cartridge_err();



#endif

//if(!lcd_init){ STM32f4_Discovery_LCD_Init(); lcd_init=TRUE; }
//	if(!SDcard_mount()){ SDcard_init();}
