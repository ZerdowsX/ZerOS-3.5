#ifndef IDT_H
#define IDT_H

#include <stdint.h>

void idt_install(void);
void irq_install(void);
void isr_handler_c(uint32_t int_no, uint32_t err_code);

#endif
