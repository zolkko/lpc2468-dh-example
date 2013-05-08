#include "config.h"
#include <stdlib.h>
#include <stdint.h>
#include "sys.h"
#include "lpc24xx.h"
#include "clk.h"
#include "i2c.h"
#include "vic.h"
#include "pwr.h"
#include "gpio.h"

static void test_handler(void) __attribute__ ((interrupt("IRQ"))) ;

void fiq_handler(void)
{
	while (1) {
	}
}

void irq_handler(void)
{
	while (1) {
	}
}

void software_interrupt_handler(void)
{
	while (1) {
	}
}

void prefetch_abort_handler(void)
{
	while (1) {
	}
}

void data_abort_handler(void)
{
	while (1) {
	}
}

void undef_handler(void)
{
	while (1) {
	}
}

void test_handler(void)
{
	void (*handler)() = (void(*)())VICVectAddr;
	if (handler != NULL) {
		FIO4CLR = (1 << 17);
	}
	EXTINT  = EINT3_bm | EINT2_bm | EINT1_bm | EINT0_bm;

	VICVectAddr = 0;
}

void sys_init(void)
{
	pll_init();
	power_init();
	vic_init();
	SYS_SetFastGPIO();
	gpio_init(test_handler);
	i2c_init();

	irq_enable();
}
