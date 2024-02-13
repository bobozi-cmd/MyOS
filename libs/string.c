#include "string.h"
#include "types.h"

void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len) {
    while (len) {
        *dest++ = *src++;
        len--;
    }
}

void memset(void *dest, uint8_t val, uint32_t len) {
    uint8_t* dst = dest;
    while (len) {
        *dst++ = val;
        len--;
    }
}

void bzero(void *dest, uint32_t len) {
    memset(dest, 0, len);
}

int strcmp(const char *str1, const char *str2) {

    while (*str1 && *str2) {
        int dif = *str1++ - *str2++;
        if (dif != 0)
            return dif;
    }

    return *str1 - *str2;
}

char *strcpy(char *dest, const char *src) {
    memcpy((uint8_t *)dest, (uint8_t *)src, strlen(src));
    return dest;
}

char *strcat(char *dest, const char *src) {
    int dest_len = strlen(dest);
    memcpy((uint8_t *)dest + dest_len, (uint8_t *)src, strlen(src));
    return dest;
}

int strlen(const char *src) {
    int ret = 0;
    while (*src) {
        src++;
        ret++;
    }
    return ret;
}