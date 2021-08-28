#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include "types.h"

inline void * memcpy(void * dest, void * src, size_t num) {
    uint8_t * d = (uint8_t*)dest;
    const uint8_t * s = (uint8_t*)src;
    for(; num; num--) *d++ = *s++;
    return (void*)dest;
}

inline void * memset(void * dest, int c, size_t n) {
    unsigned char *s = (unsigned char *)dest;
    for (; n; n--, s++) *s = c;
    return dest;
}

inline size_t strlen(const char *s) {
    const char *a = s;
    for(; *s;s++);
    return s-a;
}

inline char * strcat(char * dest, const char * src) {
    char * d = dest + strlen(dest);
    for(; (*d = *src); src++, d++);
    return d;
}

inline int strcmp(const char*l, const char *r) {
    for(; *l == *r && *l; l++, r++);
    return *(unsigned char*)l - *(unsigned char*)r;
    }
#endif //STRING_H_INCLUDED