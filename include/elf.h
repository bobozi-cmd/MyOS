#ifndef INCLUDE_ELF_H_
#define INCLUDE_ELF_H_

#include "types.h"
#include "multiboot.h"

#define ELF32_ST_TYPE(i) ((i) & 0xf)

// ELF section header
typedef struct elf_section_header_t {
    uint32_t name;
    uint32_t type;
    uint32_t flags;
    uint32_t addr;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t addralign;
    uint32_t entsize;

} __attribute__((packed))  elf_section_header_t;

// ELF symbol
typedef struct elf_symbol_t {
    uint32_t name;
    uint32_t value;
    uint32_t size;
    uint32_t info;
    uint32_t other;
    uint32_t shndx;

} __attribute__((packed)) elf_symbol_t;

// ELF info
typedef struct elf_t {
    elf_symbol_t *symtab;
    uint32_t      symtabsz;
    const char   *strtab;
    uint32_t      strtabsz;

} elf_t;

// get ELF info from multiboot_t
elf_t elf_from_multiboot(multiboot_t *mb);

// check ELF symbool
const char *elf_lookup_symbol(uint32_t addr, elf_t *elf);

#endif // INCLUDE_ELF_H_
