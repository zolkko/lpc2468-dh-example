
#ifndef _i2c_h_
#define _i2c_h_

/**
 * I2C buffers size
 */
#define I2C_OUT_BUFFER_SIZE 512

#define I2C_IN_BUFFER_SIZE  512

/**
 * 7 most significant bits indicates slave address.
 */
#define I2C_SLAVE_ADDRESS 0xA4

/**
 * pinsel1 P0[27]
 */
#define PIN_SDA0_bp 22
#define PIN_SDA0_bm 0xc00000

/**
 * pinsel1 P0[28]
 */
#define PIN_SCL0_bp 24
#define PIN_SCL0_bm 0x3000000  // pinsel1 P0[28]

/**
 * I2C SCL Duty Cycle High register
 */
#define I2SCLH_SCLH 0x00000080
#define I2SCLL_SCLL 0x00000080

/**
 * I2C0 interrupt number
 */
#define I2C0_INT 9

/**
 * Pin functions
 */
#define PIN_FUNC_PRIMARY 00
#define PIN_FUNC_ALT1    01
#define PIN_FUNC_ALT2    10
#define PIN_FUNC_ALT3    11

/**
 * I2CONSET and I2CONCLR bit masks.
 */
#define I2CON_ACK_bm  (1 << 2)   // acknowledge flag
#define I2CON_SINT_bm (1 << 3)   // interrupt flag
#define I2CON_STOP_bm (1 << 4)   // stop flag
#define I2CON_START_bm (1 << 5)  // start flag
#define I2CON_ENABLE_bm (1 << 6) // enable module flag

#ifdef __cplusplus
extern "C" {
#endif

void i2c_interrupt_handler(void) __attribute__(( interrupt("IRQ"), section(".isr_handlers") ));

void i2c_init(void);

#ifdef __cplusplus
}
#endif

#endif
