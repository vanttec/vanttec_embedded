################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ros_lib/duration.cpp \
../ros_lib/time.cpp 

OBJS += \
./ros_lib/duration.o \
./ros_lib/time.o 

CPP_DEPS += \
./ros_lib/duration.d \
./ros_lib/time.d 


# Each subdirectory must supply rules for building sources it contributes
ros_lib/duration.o: ../ros_lib/duration.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DSTM32G431xx -DUSE_HAL_DRIVER -DDEBUG -c -I"/home/abiel/Documents/VantTec/vanttec_embedded/Boat_Nucleo/ros_lib" -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"ros_lib/duration.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
ros_lib/time.o: ../ros_lib/time.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DSTM32G431xx -DUSE_HAL_DRIVER -DDEBUG -c -I"/home/abiel/Documents/VantTec/vanttec_embedded/Boat_Nucleo/ros_lib" -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"ros_lib/time.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

