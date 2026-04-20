#include <stdint.h>
#include "include/gdt.h"

struct __attribute__((packed)) gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
};

struct __attribute__((packed)) gdt_ptr {
    uint16_t limit;
    uint32_t base;
};

extern void gdt_flush(uint32_t gdt_ptr_addr);

static struct gdt_entry gdt[3];
static struct gdt_ptr gp;

static void gdt_set_gate(int idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[idx].base_low = base & 0xFFFF;
    gdt[idx].base_mid = (base >> 16) & 0xFF;
    gdt[idx].base_high = (base >> 24) & 0xFF;

    gdt[idx].limit_low = limit & 0xFFFF;
    gdt[idx].granularity = (limit >> 16) & 0x0F;
    gdt[idx].granularity |= gran & 0xF0;
    gdt[idx].access = access;
}

void gdt_install(void) {
    gp.limit = sizeof(gdt) - 1;
    gp.base = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdt_flush((uint32_t)&gp);
}
