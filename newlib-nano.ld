/*
 * LPC2468-ROM with newlib-nano library
 */

ENTRY(_boot)

MEMORY
{
    flash : ORIGIN = 0,          LENGTH = 512K
    ram   : ORIGIN = 0x40000000, LENGTH = 0xffff /* 64K */
}

SECTIONS
{
	.text :
	{
		KEEP(*(.isr_vector))
		KEEP(*(.isr_handlers))
		*(.text*)
		
		KEEP(*(.init))
		KEEP(*(.fini))
		
		/* .ctors */
		*crtbegin.o(.ctors)
		*crtbegin?.o(.ctors)
		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
		*(SORT(.ctors.*))
		*(.ctors)
		
		/* .dtors */
		*crtbegin.o(.dtors)
		*crtbegin?.o(.dtors)
		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
		*(SORT(.dtors.*))
		*(.dtors)
		
		*(.rodata*)
		
		KEEP(*(.eh_frame*))
	} > flash
	
	.ARM.extab :
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > flash
	
	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > flash
	__exidx_end = .;
	
	__etext = .;
	
	.data : AT (__etext)
	{
		__data_start__ = .;
		*(vtable)
		*(.data*)
		
		. = ALIGN(4);
		/* preinit data */
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
		
		. = ALIGN(4);
		/* init data */
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);
		
		. = ALIGN(4);
		/* finit data */
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);
		
		KEEP(*(.jcr*))
		. = ALIGN(4);
		/* All data end */
		__data_end__ = .;
	} > ram

	.bss :
	{
		. = ALIGN(4);
		__bss_start__ = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;
	} > ram

	.heap (COPY):
	{
		__end__ = .;
		end = __end__;
		*(.heap*)
		__HeapLimit = .;
	} > ram

	/* .stack_dummy section doesn't contains any symbols. It is only
	 * used for linker to calculate size of stack sections, and assign
	 * values to stack symbols later */
	.stack_dummy (COPY):
	{
		*(.stack*)
	} > ram
	
	/* Set stack top to end of RAM, and stack limit move down by
	 * size of stack_dummy section */
	__StackTop = ALIGN(ORIGIN(ram) + LENGTH(ram), 4);
	__StackLimit = __StackTop - SIZEOF(.stack_dummy);
	PROVIDE(__stack = __StackTop);

	/* Check if data + heap + stack exceeds RAM limit */
	ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")

	/* Define stack sizes and their locations for different CPU modes */
	_undef_stack_start = __StackTop;
	_abort_stack_start = ALIGN(_undef_stack_start - 8, 4);
	_fiq_stack_start   = ALIGN(_abort_stack_start - 8, 4);
	_irq_stack_start   = ALIGN(_fiq_stack_start - 128, 4);
	_svc_stack_start   = ALIGN(_irq_stack_start - 128, 4);
	_sys_stack_start   = ALIGN(_svc_stack_start - 128, 4);
	_usr_stack_start   = ALIGN(_sys_stack_start - 256, 4);
}
