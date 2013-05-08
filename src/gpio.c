
#include <stdlib.h>
#include "lpc24xx.h"
#include "gpio.h"
#include "vic.h"


void gpio_init(void * handler)
{
	if (handler != NULL) {
		// p4.17 - stat1 led
		FIO4DIR = 0xffffffff;
		FIO4SET = 0xffffffff;
		FIO4CLR = (1 << 16);

		// User button 1
		// Selects 01 alternative function EXTINT3 (PINSEL4 bit 27:26) for P2.13
		PINSEL4  = (1 << 26);
		EXTINT   = EINT3_bm;
		EXTMODE  = 0;
		EXTPOLAR = 0;
		EXTINT   = 0;
		// Clear external interrupt flag

		irq_install(EINT3_INT, (void *)handler, VIC_PRIORITY_HI);
	}
}
