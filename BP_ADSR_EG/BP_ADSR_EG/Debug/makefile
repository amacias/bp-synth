################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Src/subdir.mk
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
BP_ADSR_EG.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
BP_ADSR_EG.list \

OBJCOPY_HEX += \
BP_ADSR_EG.hex \


# All Target
all: BP_ADSR_EG.elf secondary-outputs

# Tool invocations
BP_ADSR_EG.elf: $(OBJS) $(USER_OBJS) ../STM32F411CEUx_FLASH.ld
	arm-none-eabi-gcc -o "BP_ADSR_EG.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"../STM32F411CEUx_FLASH.ld" --specs=nosys.specs -Wl,-Map="BP_ADSR_EG.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

BP_ADSR_EG.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "BP_ADSR_EG.list"
	@echo 'Finished building: $@'
	@echo ' '

BP_ADSR_EG.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "BP_ADSR_EG.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
