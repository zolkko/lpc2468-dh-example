#include "config.h"
#include <stdlib.h>
#include <stdint.h>
#include "sys.h"
#include "lpc24xx.h"
#include "i2c.h"
#include "vic.h"

/*
 * The function initializes PLL and sets it as a CPU clock source.
 * The PLL it self is clocked from external OSC.
 */
static void pll_init(void);

static void power_init(void);

static void gpio_init(void);

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

#define PCTL_TIM0_bm   (1 << 1)
#define PCTL_TIM1_bm   (1 << 2)
#define PCTL_UART0_bm  (1 << 3)
#define PCTL_UART1_bm  (1 << 4)
#define PCTL_PWM0_bm   (1 << 5)
#define PCTL_PWM1_bm   (1 << 6)
#define PCTL_I2C0_bm   (1 << 7)
#define PCTL_SPI_bm    (1 << 8)
#define PCTL_RTC_bm    (1 << 9)
#define PCTL_SSP1_bm   (1 << 10)
#define PCTL_EMC_bm    (1 << 11)
#define PCTL_CAD_bm    (1 << 12)
#define PCTL_CAN1_bm   (1 << 13)
#define PCTL_CAN2_bm   (1 << 14)
#define PCTL_I2C1_bm   (1 << 19)
#define PCTL_LDC_bm    (1 << 20)
#define PCTL_SSP0_bm   (1 << 21)
#define PCTL_TIM2_bm   (1 << 22)
#define PCTL_TIM3_bm   (1 << 23)
#define PCTL_UART2_bm  (1 << 24)
#define PCTL_UART3_bm  (1 << 25)
#define PCTL_I2C2_bm   (1 << 26)
#define PCTL_I2S_bm    (1 << 27)
#define PCTL_SDC_bm    (1 << 28)
#define PCTL_GPDMA_bm  (1 << 29)
#define PCTL_ENET_bm   (1 << 30)
#define PCTL_USB_bm    (1 << 31)


/**
 * Enable power only for certain peripheral.
 */
void power_init(void)
{
	PCONP = PCTL_TIM0_bm | PCTL_UART0_bm | PCTL_I2C0_bm | PCTL_RTC_bm | PCTL_GPDMA_bm;
}

static void test_handler(void) __attribute__ ((interrupt("IRQ"))) ;

void test_handler(void)
{
	void (*handler)() = (void(*)())VICVectAddr;
	if (handler != NULL) {
		FIO4CLR = (1 << 17);
	}
	EXTINT  = EINT3_bm | EINT2_bm | EINT1_bm | EINT0_bm;

	VICVectAddr = 0;
}

/**
 * Testing GPIO
 */
void gpio_init(void)
{
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

	irq_install(EINT3_INT, (uint32_t *)test_handler, VIC_PRIORITY_HI);
}

void sys_init(void)
{
	pll_init();
	power_init();
	vic_init();

	SYS_SetFastGPIO();

	gpio_init();
	i2c_init();

	// IRQ_enable and disable should be called through SWI
	irq_enable();
}
