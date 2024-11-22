# Bare Metal Programming

The purpose of this repository is to start from practicing bare metal programming, and proide a base for embedded code development, then move to using Hardware Abstraction Layer (HAL) of STM32F439ZI developed by ST.

## Description

The following topics will be practised here on a NUCLEO-F439ZI board:
* Bare metal with no standard library
* Bare metal with standard library
* Bare metal SemiHosting
* Bare metal Empty Project with no standard library
* Bare metal Empty Project with standard library
* TBare metal Empty Project with SemiHosting
* LEDs blinking
* LEDs On/Off via Push Button 
* Virtual COM Port Transmitt Character 
* Virtual COM Port Transmitt Text 
* Virtual COM Port Transmitt Number 
* SysTick Initialization 
* Virtual COM Port Transmitt/Receive Character 
* Virtual COM Port Transmitt/Receive Text 
* Virtual COM Port Buffer/Sendback 
* UART with Buffer/Sendback 
* ADC
* ADC READ Float
* ADC using DMA
* ADC using DMA Reads Board Temperature
* SPI
* Timer
* PWM
* PWM Dimming
* DAC

## Getting Started

### Dependencies

* Host can be any OS like Fedora 40 Linux in this case or Ubuntu, Windows etc.  
* arm-none-eabi-gcc should be installed.
* gcc should be installed.
* cmake should be installed.
* Download and install [ST-LINK](https://www.st.com/en/development-tools/stsw-link007.html) latest firmware.
* In this Fedora 40 OS, Download [ST-LINK](https://koji.fedoraproject.org/koji/buildinfo?buildID=2389744) v1/v2 JTAG/SWD debugging/flashing tool for STM32 microcontrollers
* After above installation, Cmake still has issue with LIBUSB, and in this Fedora 40 OS using "yum provides */libusb.h"
* LIBUSB should be installed.
* STM32F4 Standard Peripheral Library Expansion [STSW-STM32120](https://www.st.com/en/embedded-software/stm32-standard-peripheral-library-expansion/products.html) should be downloaded. Find the one that has "system" under "MCU Peripheral" column.

### Installing

* Check arm-none-eabi-gcc --version on a terminal. If it has not been installed, Fedora will suggest to install, and go with it.
* Check cmake --version on a terminal. If it has not been installed, Fedora will suggest to install, and go with it. Typically, it asks for gcc installation during installing cmake if you have not installed gcc yet.
* Install [ST-LINK](https://www.st.com/en/development-tools/stsw-link007.html) latest firmware using: sudo rpm -Uhv st-stlink-udev-rules-1.0.3-2-linux-noarch.rpm
* In this Fedora 40 OS, install [ST-LINK](https://koji.fedoraproject.org/koji/buildinfo?buildID=2389744) v1/v2 JTAG/SWD debugging/flashing tool for STM32 microcontrollers using: sudo rpm -Uhv stlink-1.7.0-7.fc40.x86_64.rpm
* cmake has issue with LIBUSB, and in this Fedora 40 OS using "yum provides */libusb.h" command find a version of LIBUSB that suit you, and install it using (in this case): sudo dnf install libusb1-devel-1.0.27-2.fc40.x86_64
* Download stlink-testing.zip to open a terminal inside the extracted folder and run cmake.
* Run make

### Executing program

#### Bare metal with no standard library

* Open a terminal inside the folder, and run "make clean" to remove all previousely generated *.hex, *.elf, *.bin, *.srec etc files, and then run "make all". Then, using OPENOCD, you can program the flash,an even debug the code using GDB or Telnet. How to do explanation wiilbe added soon.

#### Bare metal with standard library

* Follow corresponding oder for "Bare metal with no standard library" above. How to do explanation wiilbe added soon.

#### Bare metal SemiHosting

* Follow corresponding oder for "Bare metal with no standard library" above. How to do explanation wiilbe added soon.

#### Bare metal Empty Project with no standard library

* This example placed here for from-scratch practice. Also, Makefile was improved from previous proejcts. Follow corresponding oder for "Bare metal with no standard library" above.

#### Bare metal Empty Project with standard library

* This example placed here for from-scratch practice. Follow corresponding oder for "Bare metal with no standard library" above.

#### Bare metal Empty Project with SemiHosting

* This example placed here for from-scratch practice. Follow corresponding oder for "Bare metal with no standard library" above.

#### LEDs blinking

* 3 LEDs on the board blink using for loop delay. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder, and add the path into the Makefile. Follow corresponding oder for "Bare metal with no standard library" above.

#### LEDs On/Off via Push Button 

* 3 LEDs on the board turn on/off by pressing push button. Follow corresponding oder for "LEDs blinking" above.

#### Virtual COM Port Transmitt Character 

* A virtual COM port to send character to PC. Open a terminal inside the folder, and run "make clean" to remove "build" folder, then run "make -j all". Then, using using "make stflash" flash the board.

#### Virtual COM Port Transmitt Text 

* A virtual COM port to send text to PC. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

#### Virtual COM Port Transmitt Number 

* A virtual COM port to send number to PC. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

#### SysTick Initialization 

* SysTick function is initialized to be used instead of for loop delay. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

#### Virtual COM Port Transmitt/Receive Character 

* A virtual COM port to transmit and receive character to and from PC. The receive is interrupt-based. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

#### Virtual COM Port Transmitt/Receive Text 

* A virtual COM port to transmit and receive text to and from PC. The receive is interrupt-based. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

#### Virtual COM Port Buffer/Sendback 

* A virtual COM port to receive, buffer, and sendback text to PC. The receive is interrupt-based. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

#### UART with Buffer/Sendback 

* Beside a UART for virtual COM port, second UART is initialized to breceive, buffer, and sendback text to PC. The receive is interrupt-based. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

#### ADC

* An ADC channel is established, and read data send to PC via virtual COM port. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

#### ADC READ Float

* An ADC channel is established, and read float data send to PC via virtual COM port. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

#### ADC using DMA

* An ADC channel is initialized using DMA instead of making CPU busy, and read data send to PC via virtual COM port. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

#### ADC using DMA Reads Board Temperature

* An ADC channel is initialized using DMA instead of making CPU busy, and read board temperature data send to PC via virtual COM port. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

#### SPI

* A SPI channel is stablished. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

#### Timer

* A Timer channel is stablished. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

#### PWM

* A PWM channel is stablished. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

#### PWM Dimming

* A PWM channel is stablished, and LEDs are dimmed. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

#### DAC

* A DAC channel is stablished, output signal is read, and sent to PC via virtual COM port. First copy folder"[Libraries](https://github.com/ahasanzadeh/BareMetal/tree/main/008_VirtualComTxCharacter)" into this prject folder. Follow corresponding oder for "Virtual COM Port Transmitt Character" above.

## Help

TBD

## Authors

Contributors names and contact info

TBD

## Version History

* 0.1
    * Initial Release

## License

This project is licensed under the FREE License - see the LICENSE.md file for details

## Acknowledgments

Inspiration, code snippets, etc.
* [ah](https://github.com/ahasanzadeh/)