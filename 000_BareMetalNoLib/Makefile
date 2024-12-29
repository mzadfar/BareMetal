PROJ_NAME = final
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
MACH=cortex-m4
# -c means that compile or assemble the source file, but do not link (section 3.2 options controlling the kind of output)
# but with out machine architect -mcpu (section 3.19 machine dependent options), -c cannot finish assembling to generate .o file
# however, if we use -S instead of -c, it stop after the stage of compilation proper, and do not assemble. The output is in the form of an assembler .s file (note that instead of .o file, .s file should be specified in the command).
# -std comes from section 3.4 options controlling C dialect (gnu11 is ISO C11 standard)
CFLAGS= -c -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -O0
LDFLAGS= -nostdlib -T stm32_ls.ld -Wl,-Map=final.map 

# Commands
all: main.o led.o stm32_startup.o proj

proj: $(PROJ_NAME).elf
# $^ denotes dependancy like manin.c, and $@ denotes target like main.o
main.o:main.c
	$(CC) $(CFLAGS) -o $@ $^

led.o:led.c
	$(CC) $(CFLAGS) -o $@ $^

stm32_startup.o:stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^

$(PROJ_NAME).elf: main.o led.o stm32_startup.o
	$(CC) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	
clean:
	rm -rf *.o *.elf *.bin *.hex *.map

load:
	openocd -f board/st_nucleo_f4.cfg

# Flash the STM32F4
burn: proj
	st-flash write $(PROJ_NAME).bin 0x8000000
#$(STLINK)/st-flash write $(PROJ_NAME).bin 0x8000000 