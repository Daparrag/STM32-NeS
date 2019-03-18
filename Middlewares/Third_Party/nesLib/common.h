#ifndef __Nes_common_
#define __Nes_common_

#include <stdio.h>
#include <stdlib.h>



#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif 

#define NTH_BIT(x,n) ( ((x) >> n)  & 1 )
//#define NES_RGB_PC10
#define NES_RGB_CLASIC

#define  MAXIMUM_ROM_SIZE 0x14000 // 81920 bytes

//#pragma location=0x800C000
//__attribute__((section ("FLASH"), shared))static unsigned char rom[1024] ={0x00};


#endif /*__Nes_common_*/



