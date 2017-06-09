################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Component/DistanceSensor.cpp \
../src/Component/FollowAI.cpp \
../src/Component/FollowAI_2.cpp \
../src/Component/Motor.cpp \
../src/Component/MotorPair.cpp \
../src/Component/Scanner.cpp \
../src/Component/Sensor.cpp \
../src/Component/ServoComponent.cpp 

OBJS += \
./src/Component/DistanceSensor.o \
./src/Component/FollowAI.o \
./src/Component/FollowAI_2.o \
./src/Component/Motor.o \
./src/Component/MotorPair.o \
./src/Component/Scanner.o \
./src/Component/Sensor.o \
./src/Component/ServoComponent.o 

CPP_DEPS += \
./src/Component/DistanceSensor.d \
./src/Component/FollowAI.d \
./src/Component/FollowAI_2.d \
./src/Component/Motor.d \
./src/Component/MotorPair.d \
./src/Component/Scanner.d \
./src/Component/Sensor.d \
./src/Component/ServoComponent.d 


# Each subdirectory must supply rules for building sources it contributes
src/Component/%.o: ../src/Component/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


