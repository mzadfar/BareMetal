PROJ_NAME = final
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
MACH=cortex-m4
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wall -O0
LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=rdimon.specs -T stm32_ls.ld -Wl,-Map=final.map 

all: main.o led.o stm32_startup.o syscalls.o proj

proj: $(PROJ_NAME).elf

main.o:main.c
	$(CC) $(CFLAGS) -o $@ $^

led.o:led.c
	$(CC) $(CFLAGS) -o $@ $^

stm32_startup.o:stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^

syscalls.o:syscalls.c
	$(CC) $(CFLAGS) -o $@ $^

$(PROJ_NAME).elf: main.o led.o stm32_startup.o
	$(CC) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	
clean:
	rm -rf *.o *.elf *.bin *.hex *.map

load:
	openocd -f board/st_nucleo_f4.cfg

