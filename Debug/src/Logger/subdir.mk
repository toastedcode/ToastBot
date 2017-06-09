################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Logger/RemoteLogger.cpp 

OBJS += \
./src/Logger/RemoteLogger.o 

CPP_DEPS += \
./src/Logger/RemoteLogger.d 


# Each subdirectory must supply rules for building sources it contributes
src/Logger/%.o: ../src/Logger/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


