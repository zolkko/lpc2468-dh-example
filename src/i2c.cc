
#include <stdint.h>
#include "lpc24xx.h"
#include "i2c.h"
#include "vic.h"

void i2c_interrupt_handler(void)
{
	uint8_t status = I20STAT;
	switch (status) {
		default:
			// unknown action
			break;
	}
}

/**
 * Initializes i2c module in slave mode.
 */
void init_i2c(void)
{
	PINSEL1 &= ~(PIN_SDA0_bm | PIN_SCL0_bm);
	PINSEL1 |= (PIN_FUNC_ALT1 << PIN_SDA0_bp) | (PIN_FUNC_ALT1 << PIN_SCL0_bp);

	// Set slave address to monitor
	I20CONCLR = I2CON_ACK_bm | I2CON_SINTERRUPT_bm | I2CON_START_bm | I2CON_ENABLE_bm;

	// Set time time constraints
	I20SCLL = 0x00;
	I20SCLH = 0x00;

	// Set slave address. 7 bit MSB.
	I20ADR = I2C_SLAVE_ADDRESS;

	install_irq(I2C0_INT, (uint32_t *)i2c_interrupt_handler, HIGHEST_PRIORITY);

	I20CONSET = I2CON_ENABLE_bm;
}
