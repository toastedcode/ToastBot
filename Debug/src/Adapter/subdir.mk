################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Adapter/IpClientAdapter.cpp \
../src/Adapter/IpServerAdapter.cpp \
../src/Adapter/MqttClientAdapter.cpp \
../src/Adapter/WebSocketAdapter.cpp 

OBJS += \
./src/Adapter/IpClientAdapter.o \
./src/Adapter/IpServerAdapter.o \
./src/Adapter/MqttClientAdapter.o \
./src/Adapter/WebSocketAdapter.o 

CPP_DEPS += \
./src/Adapter/IpClientAdapter.d \
./src/Adapter/IpServerAdapter.d \
./src/Adapter/MqttClientAdapter.d \
./src/Adapter/WebSocketAdapter.d 


# Each subdirectory must supply rules for building sources it contributes
src/Adapter/%.o: ../src/Adapter/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


