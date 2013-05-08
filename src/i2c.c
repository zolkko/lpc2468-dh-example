
#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>
#include "lpc24xx.h"
#include "i2c.h"
#include "vic.h"


#define BUF_SIZE 10

static uint8_t  buffer[BUF_SIZE];

static int32_t index = -1;

/**
 * As described in LPC24XX user manual, page 596. (UM10237)
 */
void i2c_interrupt_handler(void)
{
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
			if (index < (BUF_SIZE - 1)) {
				I20CONCLR = I2CON_STOP_bm | I2CON_SINT_bm;
				I20CONSET = I2CON_ACK_bm;
			} else {
				I20CONCLR = I2CON_STOP_bm | I2CON_SINT_bm | I2CON_ACK_bm;
			}
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

			if (index < (BUF_SIZE - 1)) {
				// Data byte will be received and ACK will be returned
				buffer[++index] = I20DAT;
				I20CONCLR = I2CON_STOP_bm | I2CON_SINT_bm;
				I20CONSET = I2CON_ACK_bm;
			} else {
				buffer[index] = I20DAT;
				I20CONCLR = I2CON_STOP_bm | I2CON_SINT_bm | I2CON_ACK_bm;
			}
			break;

		case 0x88:
			// Previously addressed with own SLV address; DATA byte has been received;
			// NOT ACK has been returned.
		case 0x98:
			// Previously addressed with General Call; DATA byte has been received;
			// NOT ACK has been returned.
			//
			// These is no space left for incoming data.
			//
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
			if (index > 0) {
				I20DAT = buffer[index--];
				I20CONCLR = I2CON_STOP_bm | I2CON_SINT_bm;
				I20CONSET = I2CON_ACK_bm;
			} else if (index == 0) {
				I20DAT = buffer[index--];
				I20CONCLR = I2CON_STOP_bm | I2CON_SINT_bm | I2CON_ACK_bm;
			} else {
				index = -1;
				I20DAT = 0x00;
				I20CONCLR = I2CON_STOP_bm | I2CON_SINT_bm | I2CON_ACK_bm;
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

/**
 * Initializes i2c module in slave mode.
 * Rate settings do not affect I2C module in slave mode.
 */
void i2c_init(void)
{
	// configure corresponding pins to act as alternative function.
	PINSEL1 &= ~(PIN_SDA0_bm | PIN_SCL0_bm);
	PINSEL1 |= (PIN_FUNC_ALT1 << PIN_SDA0_bp) | (PIN_FUNC_ALT1 << PIN_SCL0_bp);

	// Set slave address to monitor
	I20CONCLR = I2CON_ACK_bm | I2CON_SINT_bm | I2CON_START_bm | I2CON_STOP_bm | I2CON_ENABLE_bm;

	irq_install(I2C0_INT, (uint32_t *)i2c_interrupt_handler, VIC_PRIORITY_HI);

	// Set slave address. 7 bit MSB.
	I20ADR = I2C_SLAVE_ADDRESS;

	I20CONSET = I2CON_ENABLE_bm | I2CON_ACK_bm;
}
