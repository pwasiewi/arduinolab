################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/usr/share/arduino/libraries/SD/src/File.cpp \
/usr/share/arduino/libraries/SD/src/SD.cpp 

LINK_OBJ += \
./Libraries/SD/src/File.cpp.o \
./Libraries/SD/src/SD.cpp.o 

CPP_DEPS += \
./Libraries/SD/src/File.cpp.d \
./Libraries/SD/src/SD.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/SD/src/File.cpp.o: /usr/share/arduino/libraries/SD/src/File.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/share/arduino/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10605 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"/usr/share/arduino/hardware/arduino/avr/cores/arduino" -I"/usr/share/arduino/hardware/arduino/avr/variants/standard" -I"/usr/share/arduino/libraries/SD" -I"/usr/share/arduino/libraries/SD/src" -I"/usr/share/arduino/hardware/arduino/avr/libraries/SPI" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

Libraries/SD/src/SD.cpp.o: /usr/share/arduino/libraries/SD/src/SD.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/share/arduino/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10605 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"/usr/share/arduino/hardware/arduino/avr/cores/arduino" -I"/usr/share/arduino/hardware/arduino/avr/variants/standard" -I"/usr/share/arduino/libraries/SD" -I"/usr/share/arduino/libraries/SD/src" -I"/usr/share/arduino/hardware/arduino/avr/libraries/SPI" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '


