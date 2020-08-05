################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Test/Main.cpp \
../Test/Mocks.cpp 

OBJS += \
./Test/Main.o \
./Test/Mocks.o 

CPP_DEPS += \
./Test/Main.d \
./Test/Mocks.d 


# Each subdirectory must supply rules for building sources it contributes
Test/%.o: ../Test/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/messam/repos/AR/Can_Test/Can" -I"/home/messam/repos/AR/Can_Test/Stubs" -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


