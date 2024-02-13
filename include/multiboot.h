#ifndef INCLUDE_MULTIBOOT_H_
#define INCLUDE_MULTIBOOT_H_

#include "types.h"

typedef struct multiboot_t {
    uint32_t flags;         // multiboot version
    // lower mem addr is 0, upper mem addr is 1M, the unit is K
    uint32_t mem_lower;     // max possible value is 640K
    uint32_t mem_upper;     // max possible value is Max Val - 1M, no guarantee

    uint32_t boot_device;   // device which OS img loaded from
    uint32_t cmdline;       // kernel command line
    uint32_t mods_count;    // boot modules list
    uint32_t mods_addr;

    // ELF image section header table
    uint32_t num;           // section header table entry count
    uint32_t size;          // section header table entry size
    uint32_t addr;
    uint32_t shndx;         // section header string table index

    // BIOS memory buffer
    uint32_t mmap_length;   // buffer size
    uint32_t mmap_addr;     // buffer addr

    uint32_t drives_length; // the first driver struct size
    uint32_t drives_addr;   // the first driver physical addr
    uint32_t config_table;  // ROM config table
    uint32_t boot_loader_name;
    uint32_t apm_table;
    
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint32_t vbe_mode;
    uint32_t vbe_interface_seg;
    uint32_t vbe_interface_off;
    uint32_t vbe_interface_len;

} __attribute__((packed)) multiboot_t;

typedef struct mmap_entry_t {
    uint32_t size;            // not contains self size
    uint32_t base_addr_low;   // boot addr low 32-bit
    uint32_t base_addr_high;
    uint32_t length_low;      // memory size low 32-bit
    uint32_t length_high;
    uint32_t type;            // addr region type, 1: available RAM

} __attribute__((packed)) mmap_entry_t;

// global ptr
extern multiboot_t *glb_mboot_ptr;

#endif // INCLUDE_MULTIBOOT_H_