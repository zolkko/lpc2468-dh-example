
#include <stdint.h>
#include "lpc24xx.h"
#include "vic.h"

void install_irq(uint32_t interrupt_number, void * handler, uint32_t priority)
{
	uint32_t * vect_addr;
	uint32_t * vect_prio;

	VICIntEnClr = 1 << interrupt_number;

	vect_addr = (uint32_t *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + interrupt_number * 4);
	vect_prio = (uint32_t *)(VIC_BASE_ADDR + VECT_PRIO_INDEX + interrupt_number * 4);

	*vect_addr = (uint32_t)handler;
	*vect_prio = priority;

	VICIntEnable = 1 << interrupt_number;
}
