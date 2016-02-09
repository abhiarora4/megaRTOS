################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../helper/array.c \
../helper/bitArray.c \
../helper/circularBuffer.c \
../helper/json.c \
../helper/json_former.c \
../helper/priorityQueue.c \
../helper/queue.c \
../helper/stack.c \
../helper/strings.c 

OBJS += \
./helper/array.o \
./helper/bitArray.o \
./helper/circularBuffer.o \
./helper/json.o \
./helper/json_former.o \
./helper/priorityQueue.o \
./helper/queue.o \
./helper/stack.o \
./helper/strings.o 

C_DEPS += \
./helper/array.d \
./helper/bitArray.d \
./helper/circularBuffer.d \
./helper/json.d \
./helper/json_former.d \
./helper/priorityQueue.d \
./helper/queue.d \
./helper/stack.d \
./helper/strings.d 


# Each subdirectory must supply rules for building sources it contributes
helper/%.o: ../helper/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


