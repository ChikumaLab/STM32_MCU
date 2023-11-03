################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../UserApp/Src/app_lte_cmd.c \
../UserApp/Src/app_sys_cmd.c \
../UserApp/Src/bme280.c \
../UserApp/Src/drv_ae_aqm1602a.c \
../UserApp/Src/drv_bme280.c \
../UserApp/Src/drv_cmd_If.c \
../UserApp/Src/drv_gps.c \
../UserApp/Src/drv_int_cntroller.c \
../UserApp/Src/drv_lteModule.c \
../UserApp/Src/drv_oled_ssd1306.c \
../UserApp/Src/drv_xbee_wifi.c \
../UserApp/Src/file_controller.c \
../UserApp/Src/main_app.c 

OBJS += \
./UserApp/Src/app_lte_cmd.o \
./UserApp/Src/app_sys_cmd.o \
./UserApp/Src/bme280.o \
./UserApp/Src/drv_ae_aqm1602a.o \
./UserApp/Src/drv_bme280.o \
./UserApp/Src/drv_cmd_If.o \
./UserApp/Src/drv_gps.o \
./UserApp/Src/drv_int_cntroller.o \
./UserApp/Src/drv_lteModule.o \
./UserApp/Src/drv_oled_ssd1306.o \
./UserApp/Src/drv_xbee_wifi.o \
./UserApp/Src/file_controller.o \
./UserApp/Src/main_app.o 

C_DEPS += \
./UserApp/Src/app_lte_cmd.d \
./UserApp/Src/app_sys_cmd.d \
./UserApp/Src/bme280.d \
./UserApp/Src/drv_ae_aqm1602a.d \
./UserApp/Src/drv_bme280.d \
./UserApp/Src/drv_cmd_If.d \
./UserApp/Src/drv_gps.d \
./UserApp/Src/drv_int_cntroller.d \
./UserApp/Src/drv_lteModule.d \
./UserApp/Src/drv_oled_ssd1306.d \
./UserApp/Src/drv_xbee_wifi.d \
./UserApp/Src/file_controller.d \
./UserApp/Src/main_app.d 


# Each subdirectory must supply rules for building sources it contributes
UserApp/Src/%.o UserApp/Src/%.su: ../UserApp/Src/%.c UserApp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -DARM_MATH_CM4 -D__FPU_PRESENT -c -I../FATFS/Target -I"C:/Users/User/Desktop/LTE/UserApp/Inc" -I../FATFS/App -I../USB_HOST/App -I../USB_HOST/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/User/Desktop/LTE/Drivers/CMSIS/DSP/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-UserApp-2f-Src

clean-UserApp-2f-Src:
	-$(RM) ./UserApp/Src/app_lte_cmd.d ./UserApp/Src/app_lte_cmd.o ./UserApp/Src/app_lte_cmd.su ./UserApp/Src/app_sys_cmd.d ./UserApp/Src/app_sys_cmd.o ./UserApp/Src/app_sys_cmd.su ./UserApp/Src/bme280.d ./UserApp/Src/bme280.o ./UserApp/Src/bme280.su ./UserApp/Src/drv_ae_aqm1602a.d ./UserApp/Src/drv_ae_aqm1602a.o ./UserApp/Src/drv_ae_aqm1602a.su ./UserApp/Src/drv_bme280.d ./UserApp/Src/drv_bme280.o ./UserApp/Src/drv_bme280.su ./UserApp/Src/drv_cmd_If.d ./UserApp/Src/drv_cmd_If.o ./UserApp/Src/drv_cmd_If.su ./UserApp/Src/drv_gps.d ./UserApp/Src/drv_gps.o ./UserApp/Src/drv_gps.su ./UserApp/Src/drv_int_cntroller.d ./UserApp/Src/drv_int_cntroller.o ./UserApp/Src/drv_int_cntroller.su ./UserApp/Src/drv_lteModule.d ./UserApp/Src/drv_lteModule.o ./UserApp/Src/drv_lteModule.su ./UserApp/Src/drv_oled_ssd1306.d ./UserApp/Src/drv_oled_ssd1306.o ./UserApp/Src/drv_oled_ssd1306.su ./UserApp/Src/drv_xbee_wifi.d ./UserApp/Src/drv_xbee_wifi.o ./UserApp/Src/drv_xbee_wifi.su ./UserApp/Src/file_controller.d ./UserApp/Src/file_controller.o ./UserApp/Src/file_controller.su ./UserApp/Src/main_app.d ./UserApp/Src/main_app.o ./UserApp/Src/main_app.su

.PHONY: clean-UserApp-2f-Src

