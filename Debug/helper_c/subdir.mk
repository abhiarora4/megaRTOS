################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../helper_c/array.c \
../helper_c/bitArray.c \
../helper_c/circularBuffer.c \
../helper_c/json.c \
../helper_c/json_former.c \
../helper_c/priorityQueue.c \
../helper_c/queue.c \
../helper_c/stack.c \
../helper_c/strings.c 

OBJS += \
./helper_c/array.o \
./helper_c/bitArray.o \
./helper_c/circularBuffer.o \
./helper_c/json.o \
./helper_c/json_former.o \
./helper_c/priorityQueue.o \
./helper_c/queue.o \
./helper_c/stack.o \
./helper_c/strings.o 

C_DEPS += \
./helper_c/array.d \
./helper_c/bitArray.d \
./helper_c/circularBuffer.d \
./helper_c/json.d \
./helper_c/json_former.d \
./helper_c/priorityQueue.d \
./helper_c/queue.d \
./helper_c/stack.d \
./helper_c/strings.d 


# Each subdirectory must supply rules for building sources it contributes
helper_c/%.o: ../helper_c/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


