#include "lpc24xx.h"
#include "pwr.h"

/**
 * Enable power only for certain peripheral.
 */
void power_init(void)
{
	PCONP = PCTL_TIM0_bm | PCTL_UART0_bm | PCTL_I2C0_bm | PCTL_RTC_bm | PCTL_GPDMA_bm;
}
