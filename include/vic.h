
#ifndef _vic_h_
#define _vic_h_

#include <stdint.h>

#define CPSR_IRQ_bm 0x00000080
#define CPSR_FIQ_bm 0x00000040

#define VIC_PRIORITY_HI 0x01
#define VIC_PRIORITY_LO 0x0f

#define WDT_INT         0
#define SWI_INT         1
#define ARM_CORE0_INT   2
#define ARM_CORE1_INT   3
#define TIMER0_INT      4
#define TIMER1_INT      5
#define UART0_INT       6
#define UART1_INT       7
#define PWM0_1_INT      8
#define I2C0_INT        9
#define SPI0_INT        10
#define SSP0_INT        10
#define SSP1_INT        11
#define PLL_INT         12
#define RTC_INT         13
#define EINT0_INT       14
#define EINT1_INT       15
#define EINT2_INT       16
#define EINT3_INT       17
#define ADC0_INT        18
#define I2C1_INT        19
#define BOD_INT         20
#define EMAC_INT        21
#define USB_INT         22
#define CAN_INT         23
#define MCI_INT         24
#define GPDMA_INT       25
#define TIMER2_INT      26
#define TIMER3_INT      27
#define UART2_INT       28
#define UART3_INT       29
#define I2C2_INT        30
#define I2S_INT         31

#define VIC_SIZE 32

#define VECT_ADDR_INDEX  0x100
#define VECT_PRIO_INDEX  0x200

#ifdef __cplusplus
extern "C" {
#endif

uint32_t irq_enable(void);

uint32_t irq_disable(void);

void irq_install(uint32_t interrupt_number, void * handler, uint32_t priority);

void vic_init(void);

#define vic_reset() (VICVectAddr = 0)

#ifdef __cplusplus
}
#endif

#endif
