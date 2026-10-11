################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/HSV_to_RGB.c \
../Core/Src/adcMesure.c \
../Core/Src/bq24070.c \
../Core/Src/eeprom.c \
../Core/Src/engine.c \
../Core/Src/main.c \
../Core/Src/myPixel.c \
../Core/Src/my_button.c \
../Core/Src/pixelEffects.c \
../Core/Src/powerLogic.c \
../Core/Src/stm32g0xx_hal_msp.c \
../Core/Src/stm32g0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g0xx.c 

OBJS += \
./Core/Src/HSV_to_RGB.o \
./Core/Src/adcMesure.o \
./Core/Src/bq24070.o \
./Core/Src/eeprom.o \
./Core/Src/engine.o \
./Core/Src/main.o \
./Core/Src/myPixel.o \
./Core/Src/my_button.o \
./Core/Src/pixelEffects.o \
./Core/Src/powerLogic.o \
./Core/Src/stm32g0xx_hal_msp.o \
./Core/Src/stm32g0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g0xx.o 

C_DEPS += \
./Core/Src/HSV_to_RGB.d \
./Core/Src/adcMesure.d \
./Core/Src/bq24070.d \
./Core/Src/eeprom.d \
./Core/Src/engine.d \
./Core/Src/main.d \
./Core/Src/myPixel.d \
./Core/Src/my_button.d \
./Core/Src/pixelEffects.d \
./Core/Src/powerLogic.d \
./Core/Src/stm32g0xx_hal_msp.d \
./Core/Src/stm32g0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g0xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DUSE_HAL_DRIVER -DSTM32G030xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/HSV_to_RGB.cyclo ./Core/Src/HSV_to_RGB.d ./Core/Src/HSV_to_RGB.o ./Core/Src/HSV_to_RGB.su ./Core/Src/adcMesure.cyclo ./Core/Src/adcMesure.d ./Core/Src/adcMesure.o ./Core/Src/adcMesure.su ./Core/Src/bq24070.cyclo ./Core/Src/bq24070.d ./Core/Src/bq24070.o ./Core/Src/bq24070.su ./Core/Src/eeprom.cyclo ./Core/Src/eeprom.d ./Core/Src/eeprom.o ./Core/Src/eeprom.su ./Core/Src/engine.cyclo ./Core/Src/engine.d ./Core/Src/engine.o ./Core/Src/engine.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/myPixel.cyclo ./Core/Src/myPixel.d ./Core/Src/myPixel.o ./Core/Src/myPixel.su ./Core/Src/my_button.cyclo ./Core/Src/my_button.d ./Core/Src/my_button.o ./Core/Src/my_button.su ./Core/Src/pixelEffects.cyclo ./Core/Src/pixelEffects.d ./Core/Src/pixelEffects.o ./Core/Src/pixelEffects.su ./Core/Src/powerLogic.cyclo ./Core/Src/powerLogic.d ./Core/Src/powerLogic.o ./Core/Src/powerLogic.su ./Core/Src/stm32g0xx_hal_msp.cyclo ./Core/Src/stm32g0xx_hal_msp.d ./Core/Src/stm32g0xx_hal_msp.o ./Core/Src/stm32g0xx_hal_msp.su ./Core/Src/stm32g0xx_it.cyclo ./Core/Src/stm32g0xx_it.d ./Core/Src/stm32g0xx_it.o ./Core/Src/stm32g0xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g0xx.cyclo ./Core/Src/system_stm32g0xx.d ./Core/Src/system_stm32g0xx.o ./Core/Src/system_stm32g0xx.su

.PHONY: clean-Core-2f-Src

