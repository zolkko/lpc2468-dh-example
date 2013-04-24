
#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>
#include "lpc24xx.h"
#include "i2c.h"
#include "vic.h"
#include "cirbuf.h"

/*static cirbuf_t in_buff;

static uint8_t in_data_buff[I2C_IN_BUFFER_SIZE];

static cirbuf_t out_buff;

static uint8_t out_data_buff[I2C_OUT_BUFFER_SIZE];*/

static cirbuf_t * in_buff;

static cirbuf_t * out_buff;

static void i2c_init_buffers(void);

/**
 * As described in LPC24XX user manual, page 596. (UM10237)
 */
void i2c_interrupt_handler(void)
{
	uint8_t data;
	uint8_t status = I20STAT;
	switch (status) {
		//
		// Slave Receiver Mode
		//

		case 0x60:
			// Own SLA+ has been received.
			// ACK has been returned.
		case 0x70:
			// General call address has been received;
			// ACK has been returned.
			//
			// Data byte will be received and ACK will be returned.
			I20CONCLR = I2CON_STOP_bm | I2CON_SINT_bm;
			I20CONSET = I2CON_ACK_bm;
			break;

		case 0x68:
			// Arbitration lost in SLA+R/W as master; Own SLA+W has been received,
			// ACK returned.
		case 0x78:
			// Arbitration lost in SLA+R/W as master; General call address bas been received,
			// ACK has been returned.
			//
			// Data byte will be received and NOT ACK will be returned.
			I20CONCLR = I2CON_STOP_bm | I2CON_SINT_bm | I2CON_ACK_bm;
			break;

		case 0x80:
			// Previously addressed with own address. DATA has been received;
			// ACK has been returned.
		case 0x90:
			// Previously addressed with general call; DATA byte has been received;
			// ACK has been returned.
			data = I20DAT;
			cirbuf_write(in_buff, data);
			// Data byte will be received and ACK will be returned
			I20CONCLR = I2CON_STOP_bm | I2CON_SINT_bm;
			I20CONSET = I2CON_ACK_bm;
			break;

		case 0x88:
			// Previously addressed with own SLV address; DATA byte has been received;
			// NOT ACK has been returned.
		case 0x98:
			// Previously addressed with General Call; DATA byte has been received;
			// NOT ACK has been returned.
			data = I20DAT;
			cirbuf_read(in_buff, &data);
			// Switched to not addressed SLV mode; Own SLA will be recognized.
			I20CONCLR = I2CON_START_bm | I2CON_STOP_bm | I2CON_SINT_bm;
			I20CONSET = I2CON_ACK_bm;
			break;

		case 0xa0:
			// A STOP condition or repeated START condition has been received while
			// still addressed as SLV/REC or SLV/TRX.
			//
			// Switched to not addressed SLV mode; Own SLA address will be recognized; General call
			// address will be recognized if I2ADR[0] = logic 1.
			I20CONCLR = I2CON_START_bm | I2CON_STOP_bm | I2CON_SINT_bm;
			I20CONSET = I2CON_ACK_bm;
			break;

		//
		// Slave Transmitter mode
		//

		case 0xa8:
			// Own SLA+R has been received; ACK has been returned.
		case 0xb0:
			// Arbitration lost in SLA+R/W as master; Own SLA+R has been received,
			// ACK has been returned.
		case 0xb8:
			// Data byte in I2DAT has been transmitted;
			// ACK has been received.
			//
			data = I20DAT;
			cirbuf_read(out_buff, &data);
			if (cirbuf_is_empty(out_buff)) {
				// Last data byte will be transmitted and
				// ACK bit will be received.
				I20CONCLR = I2CON_STOP_bm | I2CON_SINT_bm |I2CON_ACK_bm;
			} else {
				// Data byte will be transmitted and
				// ACK bit will be received.
				I20CONCLR = I2CON_STOP_bm | I2CON_SINT_bm;
				I20CONSET = I2CON_ACK_bm;
			}
			break;

		case 0xc0:
			// Data byte in I2DAT has been transmitted;
			// NOT ACK has been received.
			//
			// Switched to not addressed SLV mode; Own SLA will be recognized;
			// General call address will be recognized if I2ADR[0] = 1.
			I20CONCLR = I2CON_START_bm | I2CON_STOP_bm | I2CON_SINT_bm;
			I20CONSET = I2CON_ACK_bm;
			break;

		case 0xc8:
			// Last data byte in I2DAT has been transmitted (AA=0);
			// ACK has been received.
			//
			// Switched to not addressed SLV mode; Own SLA will be recognized; General call
			// address will be recognized if I2ADR[0] = logic 1.
			I20CONCLR = I2CON_START_bm | I2CON_STOP_bm | I2CON_SINT_bm;
			I20CONSET = I2CON_ACK_bm;
			break;
	}
	VICVectAddr = 0; // acknowledge interrupt
}

void i2c_init_buffers(void)
{
	in_buff = cirbuf_new(I2C_IN_BUFFER_SIZE);
	out_buff = cirbuf_new(I2C_OUT_BUFFER_SIZE);
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
	I20CONCLR = I2CON_ACK_bm | I2CON_SINT_bm | I2CON_START_bm | I2CON_STOP_bm | I2CON_ENABLE_bm;

	install_irq(I2C0_INT, (uint32_t *)i2c_interrupt_handler, HIGHEST_PRIORITY);

	// Set slave address. 7 bit MSB.
	I20ADR = I2C_SLAVE_ADDRESS;

	I20CONSET = I2CON_ENABLE_bm | I2CON_ACK_bm;
}
