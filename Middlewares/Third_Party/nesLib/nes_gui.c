#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_lcd.h"
#include "system_stm32f4xx.h"
#include "SDcard.h"
#include "nes_gui.h"
#include "background.h"
#include "common.h"
#include "ff.h"
#include "cartridge.h"



static uint8_t lcd_init = FALSE;
static uint8_t pause = FALSE;
static uint16_t * frame = NULL; 


static void nes_game();

void nes_background(){


	LCD_DrawPicture(0,0,320,240,(uint8_t *)&background);
	

	
}


void nes_menu(){


FRESULT fr;     /* Return value */
DIR dj;         /* Directory search object */
FILINFO fno;    /* File information */
uint8_t line=0;

	LCD_SetTextColor(Black);
	fr = f_findfirst(&dj, &fno, "", "*.nes");  /* Start to search for photo files */
	while (fr == FR_OK && fno.fname[0] && line!=29) {         /* Repeat while an item is found */
        LCD_DisplayStringLine(LINE(line), (uint8_t *)fno.fname);
        line++;
        fr = f_findnext(&dj, &fno);               /* Search for next item */
    }

}



void nes_render(){

	LCD_Clear(LCD_COLOR_BLACK);
	


	if(Cartriedge_loaded())
		nes_game();
	else
		nes_background();

}


void nes_gui_init(){

	if(!lcd_init){ STM32f4_Discovery_LCD_Init(); lcd_init=TRUE; } 
	if(!SDcard_mount()){ SDcard_init();}
}


void nes_game(){
	int i = 0;
	for(i = 0; i < LCD_PIXEL_HEIGHT; i++ )
	{
		LCD_nes_rowplot(frame,i,256,32);
		frame+=256;
	}
}



void new_frame(uint16_t* pixels){
	frame = pixels; 
}

void nes_run()
{
	
    // Framerate control:
	 uint32_t frameStart, frameTime;
	const int FPS   = 60;
    const int DELAY = 1000.0f / FPS;


    while(1){
    	frameStart = GetSysClock();

    }

     if (!pause) CPU_run_frame();
     nes_render();

      // Wait to mantain framerate:
     	frameTime = GetSysClock() - frameStart;
     	if (frameTime < DELAY){}





}





