################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ByteVetor.cpp \
../src/Cabecalho.cpp \
../src/Cliente.cpp \
../src/Mensagem.cpp \
../src/ModuloAtraso.cpp \
../src/Simulador-Cliente.cpp \
../src/Timer.cpp 

OBJS += \
./src/ByteVetor.o \
./src/Cabecalho.o \
./src/Cliente.o \
./src/Mensagem.o \
./src/ModuloAtraso.o \
./src/Simulador-Cliente.o \
./src/Timer.o 

CPP_DEPS += \
./src/ByteVetor.d \
./src/Cabecalho.d \
./src/Cliente.d \
./src/Mensagem.d \
./src/ModuloAtraso.d \
./src/Simulador-Cliente.d \
./src/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


