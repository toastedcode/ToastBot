################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Behavior/Behavior.cpp \
../src/Behavior/ScoutBehavior.cpp \
../src/Behavior/ServoPanBehavior.cpp 

OBJS += \
./src/Behavior/Behavior.o \
./src/Behavior/ScoutBehavior.o \
./src/Behavior/ServoPanBehavior.o 

CPP_DEPS += \
./src/Behavior/Behavior.d \
./src/Behavior/ScoutBehavior.d \
./src/Behavior/ServoPanBehavior.d 


# Each subdirectory must supply rules for building sources it contributes
src/Behavior/%.o: ../src/Behavior/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


