ENTRY(Reset_Handler)

MEMORY
{
    FLASH(rx) :ORIGIN =0x08000000,LENGTH =2048K
    SRAM(rwx):ORIGIN =0x20000000,LENGTH =192K
/*    
    SRAM1(rwx):ORIGIN =0x20000000,LENGTH =112K
    SRAM2(rwx):ORIGIN =0x20000000+112K,LENGTH =16K
    SRAM3(rwx):ORIGIN =0x20000000+112K+16K,LENGTH =64K
*/
}

SECTIONS
{
    .text :
    {
        *(.isr_vector)
        *(.text)
        *(.rodata)
        . = ALIGN(4);
        _etext = .;
    }> FLASH

    .data :
    {
        _sdata = .;
        *(.data)
        . = ALIGN(4);
        _edata = .;
    }> SRAM AT> FLASH

    .bss :
    {
        _sbss = .;
        *(.bss)
        . = ALIGN(4);
        _ebss = .;
    }> SRAM
}
