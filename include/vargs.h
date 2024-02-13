#ifndef INCLUDE_VARGS_H_
#define INCLUDE_VARGS_H_

#include "console.h"

typedef __builtin_va_list va_list;

// https://en.cppreference.com/w/c/variadic
#define va_start(ap, last) (__builtin_va_start(ap, last))
#define va_arg(ap, type) (__builtin_va_arg(ap, type))
#define va_end(ap)

// kernel print func
void printk(const char *fmt, ...);

// kernel print func with color
void printk_color(real_color_t back, real_color_t fore, const char * format, ...);

#endif // INCLUDE_VARGS_H_
