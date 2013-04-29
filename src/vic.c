
#include <stdint.h>
#include "lpc24xx.h"
#include "vic.h"

static inline uint32_t asm_get_cpsr(void);

static inline void asm_set_cpsr(uint32_t val);

uint32_t asm_get_cpsr(void)
{
	uint32_t result;
	asm volatile ("mrs %0, cpsr" : "=r" (result) : );
	return result;
}

void asm_set_cpsr(uint32_t val)
{
	asm volatile ("msr cpsr, %0" : : "r" (val) );
}

uint32_t irq_enable(void)
{
	uint32_t _cpsr;
	_cpsr = asm_get_cpsr();
	asm_set_cpsr(_cpsr & ~CPSR_IRQ_bm);
	return _cpsr;
}

uint32_t irq_disable(void)
{
	uint32_t _cpsr;
	_cpsr = asm_get_cpsr();
	asm_set_cpsr(_cpsr | CPSR_IRQ_bm);
	return _cpsr;
}

uint32_t irq_restore(uint32_t oldCPSR)
{
	uint32_t _cpsr;
	_cpsr = asm_get_cpsr();
	asm_set_cpsr((_cpsr & ~CPSR_IRQ_bm) | (oldCPSR & CPSR_IRQ_bm));
	return _cpsr;
}

void irq_install(uint32_t interrupt_number, void * handler, uint32_t priority)
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

void vic_init(void)
{
	uint32_t * vect_addr, * vect_prio;

	VICIntEnClr = 0xffffffff;
	VICVectAddr = 0;
	VICIntSelect = 0;

	/* set all the vector and vector control register to 0 */
	for (uint32_t i = 0; i < VIC_SIZE; i++) {
		vect_addr = (uint32_t *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + i*4);
		vect_prio = (uint32_t *)(VIC_BASE_ADDR + VECT_PRIO_INDEX + i*4);
		*vect_addr = 0x0;
		*vect_prio = 0xF;
	}
}
