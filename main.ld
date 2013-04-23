/* zolkko phdc */
/* Linker script for LPC2468 */
MEMORY
{
    flash : ORIGIN = 0,          LENGTH = 512K
    ram   : ORIGIN = 0x40000000, LENGTH = 0xffff /* 64K */
}

SECTIONS
{
    . = 0;
    .vectors :
    {
        *(.isr_vectors)
        . = ALIGN(4);
        *(.isr_handlers)
    } > flash

    .text :
    {
        *(.text)
        *(.rodata)
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
    } > flash

    . = ALIGN(4);
    _etext = .;

    .data : AT (_etext)
    {
        _data = .;
        *(.data)
    } > ram

    . = ALIGN(4);
    _edata = .;

    .bss (NOLOAD) :
    {
        _bss_start = .;
        *(.bss)
        *(COMMON)
    } >ram

    . = ALIGN(4);
    _bss_end = . ;
}
_end = .;

/* define stack positions */
_undef_stack_start = ORIGIN(ram) + LENGTH(ram);
_abort_stack_start = ALIGN(_undef_stack_start - 8, 4);
_fiq_stack_start   = ALIGN(_abort_stack_start - 8, 4);
_irq_stack_start   = ALIGN(_fiq_stack_start - 128, 4);
_svc_stack_start   = ALIGN(_irq_stack_start - 128, 4);
_sys_stack_start   = ALIGN(_svc_stack_start - 128, 4);
_usr_stack_start   = ALIGN(_sys_stack_start - 256, 4);
