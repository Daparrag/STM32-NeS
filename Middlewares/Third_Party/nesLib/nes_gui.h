#ifndef __NES_GUI_H
#define __NES_GUI_H

#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_lcd.h"
#include "SDcard.h"
#include "nes_gui.h"
#include "background.h"
#include "common.h"



void nes_background();
void nes_render();
void new_frame(uint16_t * pixels);


#endif