    /* zolkko phdc */
    /* PSR mode bits mask */
    .set  MODE_USR, 0x10
    .set  MODE_FIQ, 0x11
    .set  MODE_IRQ, 0x12
    .set  MODE_SVC, 0x13
    .set  MODE_ABT, 0x17
    .set  MODE_UND, 0x1B
    .set  MODE_SYS, 0x1F

    /* disable IRQ and FIQ PSR bit mask */
    .set  I_BIT, 0x80
    .set  F_BIT, 0x40

    .section .isr_vectors, "ax"
    .align  2
    .global _boot
    .func   _boot
_boot:
    ldr pc, reset_addr
    ldr pc, undef_addr
    ldr pc, swi_addr
    ldr pc, undef_addr
    ldr pc, undef_addr
    nop
    ldr pc, irq_addr /* [pc, #-48] 0xFFFFFF00 */
    ldr pc, fiq_addr

reset_addr:
    .word reset_handler
undef_addr:
    .word undef_handler
swi_addr:
    .word software_interrupt_handler
pabt_addr:
    .word prefetch_abort_handler
dabt_addr:
    .word data_abort_handler
irq_addr:
    .word irq_handler
fiq_addr:
    .word fiq_handler
    .word 0
    .endfunc

    .section .isr_handlers, "ax"
    .global reset_handler
    .func reset_handler
reset_handler:
    ldr  r0, undef_stack_addr
    msr  CPSR_c, #MODE_UND | I_BIT | F_BIT
    mov  sp, r0

    ldr  r0, abort_stack_addr
    msr  CPSR_c, #MODE_ABT | I_BIT | F_BIT
    mov  sp, r0

    ldr  r0, fiq_stack_addr
    msr  CPSR_c, #MODE_FIQ | I_BIT | F_BIT
    mov  sp, r0

    ldr  r0, irq_stack_addr
    msr  CPSR_c, #MODE_IRQ | I_BIT | F_BIT
    mov  sp, r0

    ldr  r0, svc_stack_addr
    msr  CPSR_c, #MODE_SVC | I_BIT | F_BIT
    mov  sp, r0

    ldr  r0, sys_stack_addr
    msr  CPSR_c, #MODE_SYS | I_BIT | F_BIT
    mov  sp, r0

    /* if boot from SRAM then skip data initialization */
    ldr  r0, boot_addr
    cmp  r0, #0
    bne  bss_clear

    /* initialize data section */
    ldr  r0, text_end_addr
    ldr  r1, data_start_addr
    ldr  r3, data_end_addr
data_init_loop:
    cmp  r1, r3
    ldrlo r2, [r0], #4
    strlo r2, [r1], #4
    blo   data_init_loop

    /* initialize BSS section */
bss_clear:
    mov  r0, #0
    ldr  r1, bss_start_addr
    ldr  r2, bss_end_addr
bss_clear_loop:
    cmp  r1, r2
    strlo r0, [r1], #4
    blo   bss_clear_loop

    bl   sys_init

    ldr  r0, usr_stack_addr
    msr  CPSR_c, #MODE_USR | I_BIT | F_BIT
    mov  sp, r0

    b    main

    .word 0

undef_stack_addr:
    .word _undef_stack_start

abort_stack_addr:
    .word _abort_stack_start

fiq_stack_addr:
    .word _fiq_stack_start

irq_stack_addr:
    .word _irq_stack_start

svc_stack_addr:
    .word _svc_stack_start

sys_stack_addr:
    .word _sys_stack_start

usr_stack_addr:
    .word _usr_stack_start

boot_addr:
    .word _boot

data_start_addr:
    .word _data

data_end_addr:
    .word _edata

text_end_addr:
    .word _etext

bss_start_addr:
    .word _bss_start

bss_end_addr:
    .word _bss_end

    .endfunc
.end
