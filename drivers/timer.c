#include "timer.h"
#include "console.h"
#include "debug.h"
#include "common.h"
#include "idt.h"
#include "types.h"
#include "vargs.h"
#include "sched.h"

void timer_callback(pt_regs *regs) {
    schedule();
}

void init_timer(uint32_t frequency) {
    register_interrupt_handler(IRQ0, timer_callback);

    // 8253/8254 Timer I/O port addr: 40H ~ 43H
    // input freq(interrupt times/second): 1193180
    uint32_t divisor = 1193180 / frequency;

    // D7 D6 D5 D4 D3 D2 D1 D0
    // 0  0  1  1  0  1  1  0  : 36H
    // set chip work at mode 3
    outb(0x43, 0x36);

    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    outb(0x40, low);
    outb(0x40, high);

}


