#include "lpc24xx.h"
#include "clk.h"

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
	PLL_Apply();

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
