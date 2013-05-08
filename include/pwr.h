
#ifndef _pwr_h_
#define _pwr_h_

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

void power_init(void);

#endif
