#include "console.h"
#include "gdt.h"
#include "idt.h"
#include "string.h"
#include "timer.h"
#include "vargs.h"
#include "types.h"
#include "debug.h"

int kern_entry() {
    init_debug();
    init_gdt();
    init_idt();

    console_clear();

    printk_color(rc_black, rc_green, "Hello, OS kernel!\n");

    init_timer(200);

    asm volatile ("sti");

    return 0;
}