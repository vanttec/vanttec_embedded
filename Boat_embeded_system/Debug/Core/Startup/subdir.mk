################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f401vctx.s 

S_DEPS += \
./Core/Startup/startup_stm32f401vctx.d 

OBJS += \
./Core/Startup/startup_stm32f401vctx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/startup_stm32f401vctx.o: ../Core/Startup/startup_stm32f401vctx.s
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -I"C:/BOAT_BETA/Boat_embeded_system/ros_lib/ros" -x assembler-with-cpp -MMD -MP -MF"Core/Startup/startup_stm32f401vctx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

