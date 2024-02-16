#ifndef INCLUDE_PMM_H_
#define INCLUDE_PMM_H_

#include "multiboot.h"
#include "types.h"

// thread stack size
#define STACK_SIZE 8192

// Support only physical memory (512 MB)
#define PMM_MAX_SIZE 0x20000000

// Physical Memory Frame size
#define PMM_PAGE_SIZE 0x1000

// Max physical page number
#define PAGE_MAX_SIZE (PMM_MAX_SIZE/PMM_PAGE_SIZE)

// page mask, 4k align
#define PHY_PAGE_MASK 0xFFFFF000

// kernel file's start and end addr in memory
// defined in kernel.ld
extern uint8_t kern_start[];
extern uint8_t kern_end[];

// dynamic allocated page number
extern uint32_t phy_page_count;

void show_memory_map();

void init_pmm();

uint32_t pmm_alloc_page();

void pmm_free_page(uint32_t p);

#endif // INCLUDE_PMM_H_