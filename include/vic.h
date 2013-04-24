
#ifndef _vic_h_
#define _vic_h_

#define VECT_ADDR_INDEX  0x100
#define VECT_PRIO_INDEX  0x200

#define HIGHEST_PRIORITY 0x01

#ifdef __cplusplus
extern "C" {
#endif

void install_irq(uint32_t interrupt_number, void * handler, uint32_t priority);

#ifdef __cplusplus
}
#endif

#endif
