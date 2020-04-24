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
OpenOCD_Semihosting.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
OpenOCD_Semihosting.list \

OBJCOPY_BIN += \
OpenOCD_Semihosting.bin \


# All Target
all: OpenOCD_Semihosting.elf secondary-outputs

# Tool invocations
OpenOCD_Semihosting.elf: $(OBJS) $(USER_OBJS) C:\Users\yeong\Desktop\git_doc\ARM\src\stDriverDevelopment\429ZI_DISC1\OpenOCD_Semihosting\STM32F429ZITX_FLASH.ld
	arm-none-eabi-gcc -o "OpenOCD_Semihosting.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\yeong\Desktop\git_doc\ARM\src\stDriverDevelopment\429ZI_DISC1\OpenOCD_Semihosting\STM32F429ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="OpenOCD_Semihosting.map" -Wl,--gc-sections -static -specs=rdimon.specs -lc -lrdimon --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

OpenOCD_Semihosting.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "OpenOCD_Semihosting.list"
	@echo 'Finished building: $@'
	@echo ' '

OpenOCD_Semihosting.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "OpenOCD_Semihosting.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
