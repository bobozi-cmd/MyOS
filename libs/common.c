#include "common.h"

// write a byte to port
// inline asm: asm volatile("assembly code:output:input")
inline void outb(uint16_t port, uint8_t value) {
    // %0: port,  dN: save var into generic register (EAX, EBX)
    // %1: value, a : save var into accumulator register (AL, AX, EAX) 
    // value -> port
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

// read a byte
inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    // %0: ret, =a: save var into accumulator register, = means ouput operand
    // %1: port
    // port -> ret
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// read a word
inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}



