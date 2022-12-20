################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/LPUART.c \
../src/Setting.c \
../src/clocks_and_modes.c \
../src/main.c 

OBJS += \
./src/LPUART.o \
./src/Setting.o \
./src/clocks_and_modes.o \
./src/main.o 

C_DEPS += \
./src/LPUART.d \
./src/Setting.d \
./src/clocks_and_modes.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/LPUART.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


