################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include arduino/core/subdir.mk
-include Libraries/SoftwareSerial/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
arduino.ar \

ELF += \
basic_serial01.elf \

EEP += \
basic_serial01.eep \

FLASH_IMAGE += \
basic_serial01.hex \


# All Target
all: basic_serial01

# Tool invocations
basic_serial01: $(FLASH_IMAGE)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"/usr/share/arduino/hardware/tools/avr/bin/avr-size" -A "/home/pwas/workspacearduino/basic_serial01/Release/basic_serial01.elf"
	@echo 'Finished building target: $@'
	@echo ' '

arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"/usr/share/arduino/hardware/tools/avr/bin/avr-ar" rcs  "/home/pwas/workspacearduino/basic_serial01/Release/arduino.ar"   $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

basic_serial01.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"/usr/share/arduino/hardware/tools/avr/bin/avr-gcc" -Os -Wl,--gc-sections -mmcu=atmega168  -o "/home/pwas/workspacearduino/basic_serial01/Release/basic_serial01.elf"   $(LINK_OBJ) $(AR)  "/home/pwas/workspacearduino/basic_serial01/Release/arduino.ar" "-L/home/pwas/workspacearduino/basic_serial01/Release" -lm
	@echo 'Finished building: $@'
	@echo ' '

basic_serial01.eep: $(ELF)
	@echo 'Create eeprom image'
	"/usr/share/arduino/hardware/tools/avr/bin/avr-objcopy" -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  "/home/pwas/workspacearduino/basic_serial01/Release/basic_serial01.elf" "/home/pwas/workspacearduino/basic_serial01/Release/basic_serial01.eep"
	@echo 'Finished building: $@'
	@echo ' '

basic_serial01.hex: $(EEP)
	@echo 'Create Flash image (ihex format)'
	"/usr/share/arduino/hardware/tools/avr/bin/avr-objcopy" -O ihex -R .eeprom  "/home/pwas/workspacearduino/basic_serial01/Release/basic_serial01.elf" "/home/pwas/workspacearduino/basic_serial01/Release/basic_serial01.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(PDE_DEPS)$(C_DEPS)$(AR)$(EEP)$(FLASH_IMAGE)$(CC_DEPS)$(AR_OBJ)$(C++_DEPS)$(LINK_OBJ)$(CXX_DEPS)$(ASM_DEPS)$(INO_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(ELF)$(CPP_DEPS) basic_serial01
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
