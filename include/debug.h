#ifndef INCLUDE_DEBUG_H_
#define INCLUDE_DEBUG_H_

#include "console.h"
#include "vargs.h"
#include "elf.h"

#define assert(x, info)    \
    do{                    \
        if (!(x)) {        \
            panic(info);   \
        }                  \
    } while (0)

// compile static check, check x must be 0 or const expr
#define static_assert(x)   \
    switch (x) { case 0: case (x): ;}

// initial debug info
void init_debug();

// print current function stack
void panic(const char *msg);

// print current segment register value
void print_cur_status();

// kernel print
void printk(const char *fmt, ...);

void printk_color(real_color_t back, real_color_t fore, const char *format, ...);

#endif // INCLUDE_DEBUG_H_