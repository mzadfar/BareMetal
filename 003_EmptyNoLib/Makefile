#Path to stlink folder for uploading code to board                         
#It does not needed if st-flash --version is recognized in terminal
#which means st-flash command can be used globally
STLINK = ~/git_workspace/STM32/stlink-testing/bin

#Put your source files here (*.c)                                         
SRCS = main.c stm32_startup.c

#Libraries source files, add ones that you intent to use                  
#SRCS += stm32f4xx_rcc.c                                                   
#SRCS += stm32f4xx_gpio.c                                                  
#SRCS += stm32f4xx_usart.c

#Binaries will be generated with this name (.elf, .bin, .hex)             
PROJ_NAME = final

#Put your STM32F4 library code directory here, change YOURUSERNAME to yours
STM_COMMON = /home/ah/git_workspace/STM32/en.stm32f4_fpu_julia/STM32F4xx_AN4044_FW_V1.0.1/

# Compiler settings. Only edit CFLAGS to include other header files.
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

#Compiler flags
MACH	= cortex-m4
CFLAGS  = -mcpu=$(MACH) -mthumb -mthumb-interwork -mlittle-endian -std=gnu11
CFLAGS += -g -O0 -Wall
CFLAGS += -nostdlib -T stm32_ls.ld -Wl,-Map=final.map
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -I. 

# Include files from STM libraries                                         
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/Include
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/Device/ST/STM32F4xx/Include
CFLAGS += -I$(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/inc
CFLAGS += -I$(STM_COMMON)/Utilities/STM32_EVAL
#
#add startup file to build                                                
#SRCS += $(STM_COMMON)/Libraries/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc_ride7/startup_stm32f4xx.s

# Commands
all: proj

proj: $(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)                                                  
	$(CC) $(CFLAGS) $^ -o $@
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

clean:
	rm -rf *.o *.elf *.bin *.hex *.map $(PROJ_NAME).elf $(PROJ_NAME).hex $(PROJ_NAME).bin

load:
	openocd -f board/st_nucleo_f4.cfg

# Flash the STM32F4
burn: proj
	st-flash write $(PROJ_NAME).bin 0x8000000
	#$(STLINK)/st-flash write $(PROJ_NAME).bin 0x8000000 
