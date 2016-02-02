################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../helper/bitArray.c \
../helper/circularBuffer.c \
../helper/jason.c \
../helper/queue.c \
../helper/stack.c 

OBJS += \
./helper/bitArray.o \
./helper/circularBuffer.o \
./helper/jason.o \
./helper/queue.o \
./helper/stack.o 

C_DEPS += \
./helper/bitArray.d \
./helper/circularBuffer.d \
./helper/jason.d \
./helper/queue.d \
./helper/stack.d 


# Each subdirectory must supply rules for building sources it contributes
helper/%.o: ../helper/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


