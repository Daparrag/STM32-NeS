#ifndef __SDClass_
#define __SDClass_

#include <stdio.h>
#include "ff.h"
#include "stm32f4_discovery_sdio_sd.h"



//#define TEST_MARIO
extern FATFS filesystem;		/* volume lable */
extern FRESULT ret;			  /* Result code */
extern FIL file;				    /* File object */
extern DIR dir;				    /* Directory object */
extern FILINFO fno;			  /* File information object */
extern UINT bw, br;

uint8_t SDcard_init();
uint8_t SDcard_mount();






#endif /*__SDClass_*/

