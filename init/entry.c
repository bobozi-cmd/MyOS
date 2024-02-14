#include "console.h"
#include "gdt.h"
#include "idt.h"
#include "multiboot.h"
#include "string.h"
#include "timer.h"
#include "vargs.h"
#include "types.h"
#include "debug.h"
#include "pmm.h"
#include "vmm.h"

void kern_init();

// Multiboot pointer after paging
multiboot_t *glb_mboot_ptr;

// kernel stack after paging
char kern_stack[STACK_SIZE];

// tempory page table, 4k-align
__attribute__((section(".init.data"))) pgd_t *pgd_tmp = (pgd_t *)0x1000;
__attribute__((section(".init.data"))) pgd_t *pte_low = (pgd_t *)0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_high = (pgd_t *)0x3000;

__attribute__((section(".init.text"))) void kern_entry() {
    pgd_tmp[0] = (uint32_t)pte_low | PAGE_PRESENT | PAGE_WRITE;
    pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (uint32_t)pte_high | PAGE_PRESENT | PAGE_WRITE;

    // map kernel va 4MB to pa first 4MB
    for (int i = 0; i < 1024; i++) {
        pte_low[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
    }
    // map pa 0x00000000 ~ 0x00400000 to va 0xC0000000 ~ 0xC0400000
    for (int i = 0; i < 1024; i++) {
        pte_high[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
    }

    // set tempory page table
    asm volatile ("mov %0, %%cr3" : : "r" (pgd_tmp));

    uint32_t cr0;

    // use paging, set cr0 paging bit to 1
    asm volatile ("mov %%cr0, %0" : "=r" (cr0) : );
    cr0 |= 0x80000000;
    asm volatile ("mov %0, %%cr0" : : "r" (cr0));

    // switch kernel stack
    uint32_t kern_stack_top = ((uint32_t)kern_stack + STACK_SIZE) & 0xFFFFFFF0;
    asm volatile("mov %0, %%esp\n\t"
            "xor %%ebp, %%ebp" : : "r" (kern_stack_top));
    
    glb_mboot_ptr = mboot_ptr_tmp + PAGE_OFFSET;

    kern_init();
}

void kern_init() {
    init_debug();
    init_gdt();
    init_idt();

    console_clear();
    printk_color(rc_black, rc_green, "Hello, OS kernel!\n");

    init_timer(200);

    printk("kernel in memory start: 0x%x\n", kern_start);
    printk("kernel in memory end:   0x%x\n", kern_end);
    printk("kernel in memory used:   %d KB\n\n", (kern_end - kern_start + 1023) / 1024);    

    show_memory_map();
    init_pmm();

    printk_color(rc_black, rc_red, "\nThe Count of Physical Memory Page is: %u\n\n", phy_page_count);

    uint32_t allc_addr = NULL;
    printk_color(rc_black, rc_light_brown, "Test Physical Memory Alloc :\n");
    allc_addr = pmm_alloc_page();
    printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);
    allc_addr = pmm_alloc_page();
    printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);
    allc_addr = pmm_alloc_page();
    printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);
    allc_addr = pmm_alloc_page();
    printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);

    while (1) {
        asm volatile ("hlt");
    }
}