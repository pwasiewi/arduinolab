################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
INO_SRCS += \
../lcd2_11.ino 

CPP_SRCS += \
../.ino.cpp \
../MenuEntry.cpp \
../MenuIntHelper.cpp \
../MenuLCD.cpp \
../MenuManager.cpp 

LINK_OBJ += \
./.ino.cpp.o \
./MenuEntry.cpp.o \
./MenuIntHelper.cpp.o \
./MenuLCD.cpp.o \
./MenuManager.cpp.o 

INO_DEPS += \
./lcd2_11.ino.d 

CPP_DEPS += \
./.ino.cpp.d \
./MenuEntry.cpp.d \
./MenuIntHelper.cpp.d \
./MenuLCD.cpp.d \
./MenuManager.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
.ino.cpp.o: ../.ino.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/share/arduino/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10605 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"/usr/share/arduino/hardware/arduino/avr/cores/arduino" -I"/usr/share/arduino/hardware/arduino/avr/variants/standard" -I"/usr/share/arduino/libraries/LiquidCrystal" -I"/usr/share/arduino/libraries/LiquidCrystal/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

MenuEntry.cpp.o: ../MenuEntry.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/share/arduino/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10605 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"/usr/share/arduino/hardware/arduino/avr/cores/arduino" -I"/usr/share/arduino/hardware/arduino/avr/variants/standard" -I"/usr/share/arduino/libraries/LiquidCrystal" -I"/usr/share/arduino/libraries/LiquidCrystal/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

MenuIntHelper.cpp.o: ../MenuIntHelper.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/share/arduino/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10605 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"/usr/share/arduino/hardware/arduino/avr/cores/arduino" -I"/usr/share/arduino/hardware/arduino/avr/variants/standard" -I"/usr/share/arduino/libraries/LiquidCrystal" -I"/usr/share/arduino/libraries/LiquidCrystal/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

MenuLCD.cpp.o: ../MenuLCD.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/share/arduino/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10605 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"/usr/share/arduino/hardware/arduino/avr/cores/arduino" -I"/usr/share/arduino/hardware/arduino/avr/variants/standard" -I"/usr/share/arduino/libraries/LiquidCrystal" -I"/usr/share/arduino/libraries/LiquidCrystal/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

MenuManager.cpp.o: ../MenuManager.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/share/arduino/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10605 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"/usr/share/arduino/hardware/arduino/avr/cores/arduino" -I"/usr/share/arduino/hardware/arduino/avr/variants/standard" -I"/usr/share/arduino/libraries/LiquidCrystal" -I"/usr/share/arduino/libraries/LiquidCrystal/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

lcd2_11.o: ../lcd2_11.ino
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/share/arduino/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10605 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"/usr/share/arduino/hardware/arduino/avr/cores/arduino" -I"/usr/share/arduino/hardware/arduino/avr/variants/standard" -I"/usr/share/arduino/libraries/LiquidCrystal" -I"/usr/share/arduino/libraries/LiquidCrystal/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '


