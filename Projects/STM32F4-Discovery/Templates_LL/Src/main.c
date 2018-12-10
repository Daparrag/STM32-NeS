/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    main.c
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   LCD Diaplay DEMO      
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Embest SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
  * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
  * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_lcd.h"
#include "stm32f4_discovery_sdio_sd.h"
#include "ff.h"
#include "ppu.h"
#include "palette.h"


#ifndef TRUE
#define TRUE 1
#endif
/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1   "     STM32F4xx      " 
#define MESSAGE2   " Device running on  " 
#define MESSAGE3   " stm32f4_discovery  " 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
  
/* Private functions ---------------------------------------------------------*/
static void NVIC_Configuration(void);

static void test_palette()
{
//	uint8_t index_x = 0;
//	uint8_t index_y = 0;
//
	uint8_t idx_x = 0;
	uint8_t idx_y = 0;


	uint8_t i = 0;
	for(i=0; i < 92; i++){
		LCD_DrawFilledRect(idx_x,idx_y,idx_x+16,idx_y+16,*(nesRgb+i),*(nesRgb+i));
		idx_x+=16;
		if(i % 32 == 0 )
			idx_y+=16;
	}




//	for(idx=0; idx < 96; idx++ )
//	{
//		LCD_Clear(*(nesRgb + idx));
//		for (dlycnt = 0; dlycnt < 1000000; dlycnt++);
//	}
			//LCD_RGB_draw_pixel(index_x,index_y,*nesRgb);

//	while(1){
//		LCD_Clear(LCD_COLOR_BLACK);
//		LCD_DrawPicture(index_x,index_y,24,30,&Mario1);
//		for (dlycnt = 0; dlycnt < 500000; dlycnt++);
//		LCD_Clear(LCD_COLOR_BLACK);
//		LCD_DrawPicture(index_x,index_y,24,30,&Mario2);
//		for (dlycnt = 0; dlycnt < 500000; dlycnt++);
//		LCD_Clear(LCD_COLOR_BLACK);
//		LCD_DrawPicture(index_x,index_y,24,30,&Mario3);
//		for (dlycnt = 0; dlycnt < 500000; dlycnt++);
//		index_x=(index_x+10)%240;
//		if(index_x==0)index_y=(index_y+30)%256;
//
//	}
}






/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  volatile uint32_t dlycnt;

  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
  */

  /* wait the power stable */
  for (dlycnt = 0; dlycnt < 10000000; dlycnt++);

  STM32f4_Discovery_LCD_Init();

  /* Display message on stm32f4_discovery LCD **********************************/
  /* Clear the LCD */ 
  //LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Back Color */
  //LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the LCD Text Color */
  //LCD_SetTextColor(LCD_COLOR_WHITE);
  //LCD_DisplayStringLine(LINE(3), (uint8_t *)MESSAGE1);
  //LCD_DisplayStringLine(LINE(4), (uint8_t *)MESSAGE2);
  //LCD_DisplayStringLine(LINE(5), (uint8_t *)MESSAGE3);
  /* wait for a moment */
  //for (dlycnt = 0; dlycnt < 50000000; dlycnt++);

  /* LCD RGB Test */
  test_palette();
 // test_cpu();
  while (1);
}

/**
  * @brief  Configures SDIO IRQ channel.
  * @param  None
  * @retval None
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = SD_SDIO_DMA_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_Init(&NVIC_InitStructure);
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
