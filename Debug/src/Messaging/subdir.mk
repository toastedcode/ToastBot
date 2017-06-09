################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Messaging/JsonProtocol.cpp 

OBJS += \
./src/Messaging/JsonProtocol.o 

CPP_DEPS += \
./src/Messaging/JsonProtocol.d 


# Each subdirectory must supply rules for building sources it contributes
src/Messaging/%.o: ../src/Messaging/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


