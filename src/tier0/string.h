#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include "types.h"

size_t strlen(const char *s) {
    const char *a = s;
    for(; *s;s++);
    return s-a;
}

#endif //STRING_H_INCLUDED