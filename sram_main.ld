/* Linker script for LPC2468 */

MEMORY
{
	flash   : ORIGIN = 0,          LENGTH = 512K
	ram_vec : ORIGIN = 0x40000000, LENGTH = 0x200
	ram     : ORIGIN = 0x40000200, LENGTH = (0xffff - 0x200)
}

SECTIONS
{
	. = ORIGIN(ram);
	.vectors :
	{
		*(.isr_vectors)
	} > ram_vec
	
	.text ORIGIN(ram) :
	{
		*(.isr_handlers)
		*(.text)
		*(.rodata)
		*(.rodata*)
		*(.glue_7)
		*(.glue_7t)
	} > ram
	
	. = ALIGN(4);
	_etext = .;
	
	.data :
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
	} > ram

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
