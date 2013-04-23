
#ifndef VIC_H_
#define VIC_H_

#define VECT_ADDR_INDEX  0x100
#define VECT_PRIO_INDEX  0x200

#define HIGHEST_PRIORITY 0x01

extern "C" {

void install_irq(uint32_t interrupt_number, void * handler, uint32_t priority);

}

#endif
