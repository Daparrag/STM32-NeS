#include <stdio.h>
#include "SDcard.h"
#include "common.h" 
#include "ff.h"
#include "stm32f4_discovery_sdio_sd.h"



//#define SDcard_filesystem

FATFS filesystem;		/* volume lable */
FRESULT ret;			  /* Result code */
FIL file;				    /* File object */
DIR dir;				    /* Directory object */
FILINFO fno;			  /* File information object */
UINT bw, br;
static uint8_t sd_mount = FALSE; /*mount flag*/

uint8_t SDcard_init()
{
	/*mount the filesystem*/
		ret = f_mount(0, &filesystem);
		if(!ret)sd_mount = TRUE;

return ret;
}

uint8_t SDcard_mount()
{
	return sd_mount;
}


