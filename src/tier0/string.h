#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include "types.h"

size_t strlen(const char *s) {
    const char *a = s;
    for(; *s;s++);
    return s-a;
}

void * memcpy(void * dest, void * src, size_t num) {
    uint8_t * d = (uint8_t*)dest;
    const uint8_t * s = (uint8_t*)src;
    for(; num; num--) *d++ = *s++;
    return (void*)dest;
}

#endif //STRING_H_INCLUDED