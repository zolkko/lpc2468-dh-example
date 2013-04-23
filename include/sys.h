
#ifndef BOOT_H_
#define BOOT_H_

extern "C" {

void undef_handler(void) __attribute__(( interrupt("UNDEF"), section(".isr_handlers") )) ;

void software_interrupt_handler(void) __attribute__(( interrupt("SWI"), section(".isr_handlers") )) ;

void prefetch_abort_handler(void) __attribute__(( interrupt("ABORT"), section(".isr_handlers") ));

void data_abort_handler(void) __attribute__(( interrupt("ABORT"), section(".isr_handlers") ));

void fiq_handler(void) __attribute__(( interrupt("FIQ"), section(".isr_handlers") ));

void irq_handler(void) __attribute__(( interrupt("IRQ"), section(".isr_handlers") ));

void sys_init(void);

}

#endif /* BOOT_H_ */
