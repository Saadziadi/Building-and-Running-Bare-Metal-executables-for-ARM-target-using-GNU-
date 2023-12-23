ENTRY(Reset_Handler)

MEMORY
{
	FLASH (rx) : ORIGIN = 0x08005000, LENGTH = 5K
	SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}



SECTIONS
{
    .text :
    {
        _text = .;
        *(.isr_vector)
        *(.text)
        *(.rodata)
        _etext = .;
    } > FLASH

    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        _sdata = .;
        *(.data*)
        _edata = .;
    } > SRAM

    .bss :
    {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM
}
