#ifndef INCLUDE_IDT_H_
#define INCLUDE_IDT_H_

#include "types.h"

void init_idt();

typedef struct idt_entry_t {
    uint16_t base_lo;      // interrupt function addr 15 ~ 0
    uint16_t sel;          // target code segment descriptor selector
    uint8_t  always0;      // always 0
    uint8_t  flags;        // 0 1 1 D 0 DPL P
    uint16_t base_hi;      // interrupt function addr 31 ~ 16

} __attribute__((packed)) idt_entry_t;

// IDTR
typedef struct idt_ptr_t {
    uint16_t limit;
    uint32_t base;

} __attribute__((packed)) idt_ptr_t;

// register type
typedef struct pt_regs_t {
    uint32_t ds;         // user data segment descriptor
    uint32_t edi;        // push by pusha
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t int_no;     // interrupt number
    uint32_t err_code;   // error code
    uint32_t eip;        // cpu push automatically
    uint32_t cs;
    uint32_t eflags;
    uint32_t useresp;
    uint32_t ss;

} pt_regs;

typedef void (*interrupt_handler_t)(pt_regs *);

void register_interrupt_handler(uint8_t n, interrupt_handler_t h);

// call handler, ISR: interrupt service routine
void isr_handler(pt_regs *regs);

// 0 ~ 19 is CPU interrupts
void isr0();   // #DE zero division
void isr1();   // #DB debug exception
void isr2();   // NMI
void isr3();   // #BD breakpoint exception
void isr4();   // #OF overflow
void isr5();   // #BR array out of bound
void isr6();   // #UD undefined opcode
void isr7();   // #NM device unavailable
void isr8();   // #DF double fault
void isr9();   // coprocessor inter-sgement operate
void isr10();  // #TS invalid TSS
void isr11();  // #NP segment not exist
void isr12();  // #SS stack fault
void isr13();  // #GP general protect
void isr14();  // #PF page fault
void isr15();  // CPU reserved
void isr16();  // #MF float process unit fault
void isr17();  // #AC align check
void isr18();  // #MC machine check
void isr19();  // #XM SIMD float fault

// 20 ~ 31 is Intel interrupts
void isr20();    
void isr21();    
void isr22();   
void isr23();   
void isr24();    
void isr25();    
void isr26();   
void isr27();   
void isr28();   
void isr29();
void isr30();
void isr31();

// 32 ~ 255 user defined
void isr255();

// IRQ handler
void irq_handler(pt_regs *regs);

#define IRQ0    32    // computer system timer
#define IRQ1    33    // keyboard
#define IRQ2    34    // connect with IRQ9,  used by MPU-401 MD
#define IRQ3    35    // uart device
#define IRQ4    36    // uart device
#define IRQ5    37    // sound card use [recommand]
#define IRQ6    38    // soft driver
#define IRQ7    39    // printer
#define IRQ8    40    // clock
#define IRQ9    41    // connect with IRQ2
#define IRQ10   42    // NIC [recommand]
#define IRQ11   43    // AGP GPU
#define IRQ12   44    // PS/2 mouse
#define IRQ13   45    // coprocessor
#define IRQ14   46    // IDE0
#define IRQ15   47    // IDE1

void irq0();
void irq1();
void irq2();
void irq3();
void irq4();
void irq5();
void irq6();
void irq7();
void irq8();
void irq9();
void irq10();
void irq11();
void irq12();
void irq13();
void irq14();
void irq15();

#endif // INCLUDE_IDT_H_

