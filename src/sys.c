#include <stdlib.h>
#include <stdint.h>
#include "sys.h"
#include "lpc24xx.h"
#include "i2c.h"

/*
 * The function initializes PLL and sets it as a CPU clock source.
 * The PLL it self is clocked from external OSC.
 */
static void pll_init(void);

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

#define PLL_Enable_bm  (1 << 24)
#define PLL_Connect_bm (1 << 25)
#define PLL_Locked_bm  (1 << 26)

#define PLL_IsEnabled()   (PLLSTAT & PLL_Enable_bm)
#define PLL_IsConnected() (PLLSTAT & PLL_Connect_bm)
#define PLL_IsLocked()    (PLLSTAT & PLL_Locked_bm)

/*
 * In order to apply any changes in PLL module a special sequence of bytes
 * should be written into PLLFEED register.
 */
#define PLL_Apply() { PLLFEED = 0xaa; PLLFEED = 0x55;}
#define PLL_Enable() { PLLCON = 0x01; }
#define PLL_Connect() { PLLCON = 0x02; }
#define PLL_EnableAndConnect() { PLLCON = 0x03; }
#define PLL_Disable() { PLLCON = 0x00; }

#define SYS_OSC_EN_bm (1 << 5)
#define SYS_OSC_STAT_bm (1 << 6)
#define SYS_OSC_Enable() (SCS |= SYS_OSC_EN_bm)
#define SYS_IsOSCEnabled() (SCS & SYS_OSC_STAT_bm)
#define SYS_SetFastGPIO() (SCS |= (1 << 0))

/* Divide CPU clock by (VAL + 1) */
#define CPU_SetClockDivider(VAL) (CCLKCFG = VAL)

void pll_init(void)
{
	if (PLL_IsConnected()) {
		PLL_Enable(); // leave PLL enabled but disconnect it
		PLL_Apply();
	}

	PLL_Disable();
	PLL_Apply();

	SYS_OSC_Enable();
	while (!SYS_IsOSCEnabled())

	CLKSRCSEL = 0x01;

	PLLCFG = 11 | ( 0 << 16);
	PLL_Apply();

	PLL_Enable();
	PLL_Apply();
	CPU_SetClockDivider(5);
	while (!PLL_IsLocked());

	volatile unsigned long mval;
	volatile unsigned long nval;
	do {
		mval = PLLSTAT & 0x07fff;
		nval = PLLSTAT & 0xff0000;
	} while ((mval != 11) &&  (nval != 0));

	PLL_EnableAndConnect();
	PLL_Apply();
	while (!PLL_IsConnected());
}

void sys_init(void)
{
	pll_init();
	SYS_SetFastGPIO();
	// i2c_init();
}
