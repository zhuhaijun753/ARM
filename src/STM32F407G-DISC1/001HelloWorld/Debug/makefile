################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
helloworld.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
helloworld.list \


# All Target
all: helloworld.elf secondary-outputs

# Tool invocations
helloworld.elf: $(OBJS) $(USER_OBJS) E:\git\ST-Firmware\src\STM32F407G-DISC1\001HelloWorld\STM32F407VGTX_FLASH.ld
	arm-none-eabi-gcc -o "helloworld.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"E:\git\ST-Firmware\src\STM32F407G-DISC1\001HelloWorld\STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="helloworld.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

helloworld.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "helloworld.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
