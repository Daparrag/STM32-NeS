################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/misc.c \
C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_dma.c \
C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_exti.c \
C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_fsmc.c \
C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_gpio.c \
C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_i2c.c \
C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_rcc.c \
C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_sdio.c \
C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_syscfg.c \
C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_usart.c 

OBJS += \
./utilities/stm32f4-lib/misc.o \
./utilities/stm32f4-lib/stm32f4xx_dma.o \
./utilities/stm32f4-lib/stm32f4xx_exti.o \
./utilities/stm32f4-lib/stm32f4xx_fsmc.o \
./utilities/stm32f4-lib/stm32f4xx_gpio.o \
./utilities/stm32f4-lib/stm32f4xx_i2c.o \
./utilities/stm32f4-lib/stm32f4xx_rcc.o \
./utilities/stm32f4-lib/stm32f4xx_sdio.o \
./utilities/stm32f4-lib/stm32f4xx_syscfg.o \
./utilities/stm32f4-lib/stm32f4xx_usart.o 

C_DEPS += \
./utilities/stm32f4-lib/misc.d \
./utilities/stm32f4-lib/stm32f4xx_dma.d \
./utilities/stm32f4-lib/stm32f4xx_exti.d \
./utilities/stm32f4-lib/stm32f4xx_fsmc.d \
./utilities/stm32f4-lib/stm32f4xx_gpio.d \
./utilities/stm32f4-lib/stm32f4xx_i2c.d \
./utilities/stm32f4-lib/stm32f4xx_rcc.d \
./utilities/stm32f4-lib/stm32f4xx_sdio.d \
./utilities/stm32f4-lib/stm32f4xx_syscfg.d \
./utilities/stm32f4-lib/stm32f4xx_usart.d 


# Each subdirectory must supply rules for building sources it contributes
utilities/stm32f4-lib/misc.o: C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/misc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -D__USE_STM32F4_DISCOVERY_SD__ -DCHIPS_IMPL '-DHSE_VALUE=8000000U' -DUSE_STDPERIPH_DRIVER -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Projects/STM32F4-Discovery/Templates_LL/Inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/STM32F4-Discovery" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Device/St/STM32f4xx/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Middlewares/Third_Party/nesLib"  -Os -g3 -Wall -fmessage-length=0 -Wno-strict-aliasing -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

utilities/stm32f4-lib/stm32f4xx_dma.o: C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_dma.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -D__USE_STM32F4_DISCOVERY_SD__ -DCHIPS_IMPL '-DHSE_VALUE=8000000U' -DUSE_STDPERIPH_DRIVER -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Projects/STM32F4-Discovery/Templates_LL/Inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/STM32F4-Discovery" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Device/St/STM32f4xx/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Middlewares/Third_Party/nesLib"  -Os -g3 -Wall -fmessage-length=0 -Wno-strict-aliasing -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

utilities/stm32f4-lib/stm32f4xx_exti.o: C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_exti.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -D__USE_STM32F4_DISCOVERY_SD__ -DCHIPS_IMPL '-DHSE_VALUE=8000000U' -DUSE_STDPERIPH_DRIVER -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Projects/STM32F4-Discovery/Templates_LL/Inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/STM32F4-Discovery" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Device/St/STM32f4xx/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Middlewares/Third_Party/nesLib"  -Os -g3 -Wall -fmessage-length=0 -Wno-strict-aliasing -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

utilities/stm32f4-lib/stm32f4xx_fsmc.o: C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_fsmc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -D__USE_STM32F4_DISCOVERY_SD__ -DCHIPS_IMPL '-DHSE_VALUE=8000000U' -DUSE_STDPERIPH_DRIVER -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Projects/STM32F4-Discovery/Templates_LL/Inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/STM32F4-Discovery" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Device/St/STM32f4xx/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Middlewares/Third_Party/nesLib"  -Os -g3 -Wall -fmessage-length=0 -Wno-strict-aliasing -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

utilities/stm32f4-lib/stm32f4xx_gpio.o: C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -D__USE_STM32F4_DISCOVERY_SD__ -DCHIPS_IMPL '-DHSE_VALUE=8000000U' -DUSE_STDPERIPH_DRIVER -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Projects/STM32F4-Discovery/Templates_LL/Inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/STM32F4-Discovery" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Device/St/STM32f4xx/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Middlewares/Third_Party/nesLib"  -Os -g3 -Wall -fmessage-length=0 -Wno-strict-aliasing -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

utilities/stm32f4-lib/stm32f4xx_i2c.o: C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_i2c.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -D__USE_STM32F4_DISCOVERY_SD__ -DCHIPS_IMPL '-DHSE_VALUE=8000000U' -DUSE_STDPERIPH_DRIVER -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Projects/STM32F4-Discovery/Templates_LL/Inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/STM32F4-Discovery" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Device/St/STM32f4xx/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Middlewares/Third_Party/nesLib"  -Os -g3 -Wall -fmessage-length=0 -Wno-strict-aliasing -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

utilities/stm32f4-lib/stm32f4xx_rcc.o: C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_rcc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -D__USE_STM32F4_DISCOVERY_SD__ -DCHIPS_IMPL '-DHSE_VALUE=8000000U' -DUSE_STDPERIPH_DRIVER -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Projects/STM32F4-Discovery/Templates_LL/Inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/STM32F4-Discovery" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Device/St/STM32f4xx/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Middlewares/Third_Party/nesLib"  -Os -g3 -Wall -fmessage-length=0 -Wno-strict-aliasing -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

utilities/stm32f4-lib/stm32f4xx_sdio.o: C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_sdio.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -D__USE_STM32F4_DISCOVERY_SD__ -DCHIPS_IMPL '-DHSE_VALUE=8000000U' -DUSE_STDPERIPH_DRIVER -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Projects/STM32F4-Discovery/Templates_LL/Inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/STM32F4-Discovery" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Device/St/STM32f4xx/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Middlewares/Third_Party/nesLib"  -Os -g3 -Wall -fmessage-length=0 -Wno-strict-aliasing -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

utilities/stm32f4-lib/stm32f4xx_syscfg.o: C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_syscfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -D__USE_STM32F4_DISCOVERY_SD__ -DCHIPS_IMPL '-DHSE_VALUE=8000000U' -DUSE_STDPERIPH_DRIVER -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Projects/STM32F4-Discovery/Templates_LL/Inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/STM32F4-Discovery" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Device/St/STM32f4xx/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Middlewares/Third_Party/nesLib"  -Os -g3 -Wall -fmessage-length=0 -Wno-strict-aliasing -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

utilities/stm32f4-lib/stm32f4xx_usart.o: C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/src/stm32f4xx_usart.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -D__USE_STM32F4_DISCOVERY_SD__ -DCHIPS_IMPL '-DHSE_VALUE=8000000U' -DUSE_STDPERIPH_DRIVER -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Projects/STM32F4-Discovery/Templates_LL/Inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/STM32F4-Discovery" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Device/St/STM32f4xx/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Middlewares/Third_Party/nesLib"  -Os -g3 -Wall -fmessage-length=0 -Wno-strict-aliasing -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


