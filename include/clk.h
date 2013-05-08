
#ifndef _clk_h_
#define _clk_h_

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

#define SYS_OSC_EN_bm      (1 << 5)
#define SYS_OSC_STAT_bm    (1 << 6)
#define SYS_OSC_Enable()   (SCS |= SYS_OSC_EN_bm)
#define SYS_IsOSCEnabled() (SCS & SYS_OSC_STAT_bm)
#define SYS_SetFastGPIO()  (SCS |= (1 << 0))

/* Divide CPU clock by (VAL + 1) */
#define CPU_SetClockDivider(VAL) (CCLKCFG = VAL)

/*
 * The function initializes PLL and sets it as a CPU clock source.
 * The PLL it self is clocked from external OSC.
 */
void pll_init(void);

#endif
