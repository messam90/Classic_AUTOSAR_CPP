################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Lin/Lin.cpp \
../Lin/socket.cpp 

OBJS += \
./Lin/Lin.o \
./Lin/socket.o 

CPP_DEPS += \
./Lin/Lin.d \
./Lin/socket.d 


# Each subdirectory must supply rules for building sources it contributes
Lin/%.o: ../Lin/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/messam/Downloads/AR/Lin_Test/Lin" -I"/home/messam/Downloads/AR/Lin_Test/Stubs" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


