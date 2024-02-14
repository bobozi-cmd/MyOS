#ifndef INCLUDE_VMM_H_
#define INCLUDE_VMM_H_

#include "types.h"
#include "idt.h"

#define PAGE_OFFSET 0xC0000000

// P -- bit 0 means present
#define PAGE_PRESENT 0x1

// R/W -- bit 1, 0: only-read or only-exec, 1: read + write + exec
// if processor is in superuser, R/W bit not effect
#define PAGE_WRITE 0x2

// U/S -- bit 2, 1: each user can access. 0: only superuser can access
#define PAGE_USER 0x4

#define PAGE_SIZE 4096

#define PAGE_MASK 0xFFFFF000

#define PGD_INDEX(x) (((x) >> 22) & 0x3FF)

#define PTE_INDEX(x) (((x) >> 12) & 0x3FF)

#define OFFSET_INDEX(x) ((x) & 0xFFF)

typedef uint32_t pgd_t;

typedef uint32_t pte_t;

#define PGD_SIZE (PAGE_SIZE/sizeof(pte_t))

#define PTE_SIZE (PAGE_SIZE/sizeof(uint32_t))

// map 512MB need 128 pte
#define PTE_COUNT 128

extern pgd_t pdg_kern[PGD_SIZE];

void init_vmm();

void switch_pgd(uint32_t pd);

void map(pgd_t *pgd_now, uint32_t va, uint32_t pa, uint32_t flags);

void unmap(pgd_t *pgd_now, uint32_t va);

// if mapping va to pa, return 1
// if pa is not null. write physical addr to pa
uint32_t get_mapping(pgd_t *pgd_now, uint32_t va, uint32_t *pa);

void page_fault(pt_regs *regs);

#endif // INCLUDE_VMM_H_
