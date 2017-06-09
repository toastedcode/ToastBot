################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BlynkBot/ConfigServer.cpp \
../BlynkBot/EepromUtils.cpp 

OBJS += \
./BlynkBot/ConfigServer.o \
./BlynkBot/EepromUtils.o 

CPP_DEPS += \
./BlynkBot/ConfigServer.d \
./BlynkBot/EepromUtils.d 


# Each subdirectory must supply rules for building sources it contributes
BlynkBot/%.o: ../BlynkBot/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


