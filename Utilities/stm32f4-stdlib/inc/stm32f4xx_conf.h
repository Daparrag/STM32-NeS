/**
  ******************************************************************************
  * @file    stm32f4xx_conf.h  
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Library configuration file.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Mauro Marinoni: Modified to include Erika Enterprise barriers */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_CONF_H
#define __STM32F4xx_CONF_H

#if !defined  (HSE_VALUE)
/* Define the HSE value equal to 8 MHz on the STM32F4-DISCOVERY Kit */
 #define HSE_VALUE    ((uint32_t)8000000) 
#endif /* HSE_VALUE */

/* Includes ------------------------------------------------------------------*/
// MM: Fic me! Protect with barriers
#if defined (__USE_STM32F4XX_SPD_ADC__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_adc.h"
#endif

#if defined (__USE_STM32F4XX_SPD_CAN__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_can.h"
#endif

#if defined (__USE_STM32F4XX_SPD_CRC__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_crc.h"
#endif

#if defined (__USE_STM32F4XX_SPD_CRYP__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_cryp.h"
#endif

#if defined (__USE_STM32F4XX_SPD_DAC__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_dac.h"
#endif

#if defined (__USE_STM32F4XX_SPD_DBGMCU__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_dbgmcu.h"
#endif

#if defined (__USE_STM32F4XX_SPD_DCMI__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_dcmi.h"
#endif

#if defined (__USE_STM32F4XX_SPD_DMA__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_dma.h"
#endif

#if defined (__USE_STM32F4XX_SPD_EXTI__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_exti.h"
#endif

#if defined (__USE_STM32F4XX_SPD_FLASH__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_flash.h"
#endif

#if defined (__USE_STM32F4XX_SPD_FSMC__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_fsmc.h"
#endif

#if defined (__USE_STM32F4XX_SPD_HASH__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_hash.h"
#endif

#if defined (__USE_STM32F4XX_SPD_GPIO__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_gpio.h"
#endif

#if defined (__USE_STM32F4XX_SPD_I2C__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_i2c.h"
#endif

#if defined (__USE_STM32F4XX_SPD_IWDG__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_iwdg.h"
#endif

#if defined (__USE_STM32F4XX_SPD_PWR__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_pwr.h"
#endif

#if defined (__USE_STM32F4XX_SPD_RCC__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_rcc.h"
#endif

#if defined (__USE_STM32F4XX_SPD_RNG__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_rng.h"
#endif

#if defined (__USE_STM32F4XX_SPD_RTC__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_rtc.h"
#endif

#if defined (__USE_STM32F4XX_SPD_SDIO__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_sdio.h"
#endif

#if defined (__USE_STM32F4XX_SPD_SPI__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_spi.h"
#endif

#if defined (__USE_STM32F4XX_SPD_SYSCFG__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_syscfg.h"
#endif

#if defined (__USE_STM32F4XX_SPD_TIM__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_tim.h"
#endif

#if defined (__USE_STM32F4XX_SPD_USART__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_usart.h"
#endif

#if defined (__USE_STM32F4XX_SPD_WWDG__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "stm32f4xx_wwdg.h"
#endif

#if defined (__USE_STM32F4XX_SPD_MISC__) || defined (__USE_STM32F4XX_SPD_ALL_)
#include "misc.h" /* High level functions for NVIC and SysTick (add-on to CMSIS functions) */
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* If an external clock source is used, then the value of the following define 
   should be set to the value of the external clock source, else, if no external 
   clock is used, keep this define commented */
/*#define I2S_EXTERNAL_CLOCK_VAL   12288000 */ /* Value of the external clock in Hz */


/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed. 
  *   If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __STM32F4xx_CONF_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
