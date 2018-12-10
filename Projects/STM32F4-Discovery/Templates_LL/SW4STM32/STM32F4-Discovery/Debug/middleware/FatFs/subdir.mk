################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a/diskio.c \
C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a/ff.c 

OBJS += \
./middleware/FatFs/diskio.o \
./middleware/FatFs/ff.o 

C_DEPS += \
./middleware/FatFs/diskio.d \
./middleware/FatFs/ff.d 


# Each subdirectory must supply rules for building sources it contributes
middleware/FatFs/diskio.o: C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a/diskio.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -D__USE_STM32F4_DISCOVERY_SD__ -DCHIPS_IMPL '-DHSE_VALUE=8000000U' -DUSE_STDPERIPH_DRIVER -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Projects/STM32F4-Discovery/Templates_LL/Inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/STM32F4-Discovery" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Device/St/STM32f4xx/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Middlewares/Third_Party/nesLib"  -Os -g3 -Wall -fmessage-length=0 -Wno-strict-aliasing -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

middleware/FatFs/ff.o: C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a/ff.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407xx -D__USE_STM32F4_DISCOVERY_SD__ -DCHIPS_IMPL '-DHSE_VALUE=8000000U' -DUSE_STDPERIPH_DRIVER -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Projects/STM32F4-Discovery/Templates_LL/Inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/FatFs_vR0.08a" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/STM32F4-Discovery" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Utilities/stm32f4-stdlib/inc" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Drivers/CMSIS/Device/St/STM32f4xx/Include" -I"C:/Users/guzman/Documents/myrepositories/stm32snes/Middlewares/Third_Party/nesLib"  -Os -g3 -Wall -fmessage-length=0 -Wno-strict-aliasing -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


