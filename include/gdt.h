#ifndef INCLUDE_GDT_H_
#define INCLUDE_GDT_H_

#include "types.h"

// https://css.csail.mit.edu/6.858/2014/readings/i386/s05_01.htm
typedef struct gdt_entry_t {
    uint16_t limit_low;   // segment limit 15 ~ 0
    uint16_t base_low;    // segment base addr 15 ~ 0
    uint8_t  base_middle; // segment base addr 23 ~ 16
    uint8_t  access;      // Type, S, DPL, P
    uint8_t  granularity; // seg-limit 19 ~ 16, AVL, L, D/B, G
    uint8_t  base_high;   // segment base addr 32 ~ 24

} __attribute__((packed)) gdt_entry_t;

// 48-bit GDTR
typedef struct gdt_ptr_t {
    uint16_t limit;
    uint32_t base;

} __attribute__((packed)) gdt_ptr_t;

// init GDT
void init_gdt();

// load GDT into GDTR (asm)
extern void gdt_flush(uint32_t);

#endif // INCLUDE_GDT_H_