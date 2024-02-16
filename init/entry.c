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
#include "heap.h"

void kern_init();

// Multiboot pointer after paging
multiboot_t *glb_mboot_ptr;

// kernel stack after paging
char kern_stack[STACK_SIZE];

// tempory page table, 4k-align, 4KB/4B = 1K entries
__attribute__((section(".init.data"))) pgd_t *pgd_tmp = (pgd_t *)0x1000;
__attribute__((section(".init.data"))) pgd_t *pte_low = (pgd_t *)0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_high = (pgd_t *)0x3000;

/* https://pic3.zhimg.com/80/v2-b6e8b93e3a459dd7f3b4c044322e5ace_1440w.webp
* -> 0x1000 ... pgd_tmp[0]      -> pte_low
*    0x1004 ... pgd_tmp[1]
*    ...    ... ...
*    0x1ffb ... pgd_tmp[0x300]  -> pte_high
*                                      pdg_tmp_idx  | pte_idx      | offset
* -> 0x2000 ... pte_low[0]      -> va: 0000 0000 00 | 00 0000 0000 | 0000 0000 0000
*    0x2004 ... pte_low[1]      -> va: 0000 0000 00 | 00 0000 0001 | 0000 0000 0000
*    ...    ... ...
*    0x2ffb ... pte_low[1024]   -> va: 0000 0000 00 | 10 0000 0000 | 0000 0000 0000
* 
* -> 0x3000 ... pte_high[0]     -> va: 1100 0000 00 | 00 0000 0000 | 0000 0000 0000
*    0x3004 ... pte_high[1]     -> va: 1100 0000 00 | 00 0000 0001 | 0000 0000 0000
*    ...    ... ...
*    0x3ffb ... pte_high[1024]  -> va: 1100 0000 00 | 10 0000 0000 | 0000 0000 0000
*
* In our project: kernel va = pa + PAGE_OFFSET
*/
__attribute__((section(".init.text"))) void kern_entry() {
    pgd_tmp[0] = (uint32_t)pte_low | PAGE_PRESENT | PAGE_WRITE;
    // pgd_tmp[0x300] -> start va is PAGE_OFFSET
    pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (uint32_t)pte_high | PAGE_PRESENT | PAGE_WRITE;

    // map kernel va 4MB to pa first 4MB
    // 0 -> 0; 1 -> 4K; 2 -> 8K; 1024 -> 4MB
    for (int i = 0; i < 1024; i++) {
        pte_low[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
    }
    // map pa 0x00000000 ~ 0x00400000 to va 0xC0000000 ~ 0xC0400000
    // kernel 512 MB, 0 -> PAGE_OFFSET|0|0; 1 -> PAGE_OFFSET|1|0; 1024 -> PAGE_OFFSET|1024|0
    for (int i = 0; i < 1024; i++) {
        pte_high[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
    }
    
    // now 0 ~ 4MB(va) and 0xC0000000 ~ 0xC0400000(va) map to 0 ~ 4MB(pa) at the same time
    // because when you set cr0 paging bit to 1, CPU will use va immediately 
    // but kern_entry() map to pa & va 1MB, cannot be accessed only map 0xC0000000 ~ 0xC0400000(va)
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

    init_pmm();
    init_vmm();
    init_heap();

    printk_color(rc_black, rc_red, "\nThe Count of Physical Memory Page is: %u\n\n", phy_page_count);

    test_heap();

    while (1) {
        asm volatile ("hlt");
    }
}