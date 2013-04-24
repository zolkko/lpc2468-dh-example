
#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>
#include "lpc24xx.h"
#include "i2c.h"
#include "vic.h"
#include "cirbuf.h"

static cirbuf_t in_buff;

static uint8_t in_data_buff[I2C_IN_BUFFER_SIZE];

static cirbuf_t out_buff;

static uint8_t out_data_buff[I2C_OUT_BUFFER_SIZE];

static void i2c_init_buffers(void);


void i2c_interrupt_handler(void)
{
	uint8_t status = I20STAT;
	switch (status) {
		case 0x38:
			// data received.
			break;

		default:
			// unknown action
			break;
	}
	// reset interrupt flag
	I20CONCLR = I2CON_SINTERRUPT_bm;
	VICVectAddr = 0; // acknowledge interrupt
}

void i2c_init_buffers(void)
{
	cirbuf_init(&in_buff, in_data_buff, I2C_IN_BUFFER_SIZE);
	cirbuf_init(&out_buff, out_data_buff, I2C_OUT_BUFFER_SIZE);
}

/**
 * Initializes i2c module in slave mode.
 * Rate settings do not affect I2C module in slave mode.
 */
void i2c_init(void)
{
	i2c_init_buffers();

	// configure corresponding pins to act as alternative function.
	PINSEL1 &= ~(PIN_SDA0_bm | PIN_SCL0_bm);
	PINSEL1 |= (PIN_FUNC_ALT1 << PIN_SDA0_bp) | (PIN_FUNC_ALT1 << PIN_SCL0_bp);

	// Set slave address to monitor
	I20CONCLR = I2CON_ACK_bm | I2CON_SINTERRUPT_bm | I2CON_START_bm | I2CON_STOP_bm | I2CON_ENABLE_bm;

	install_irq(I2C0_INT, (uint32_t *)i2c_interrupt_handler, HIGHEST_PRIORITY);

	// Set slave address. 7 bit MSB.
	I20ADR = I2C_SLAVE_ADDRESS;

	I20CONSET = I2CON_ENABLE_bm | I2CON_ACK_bm;
}
