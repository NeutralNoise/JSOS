#ifndef STD_ARG_H_INCLUDED
#define STD_ARG_H_INCLUDED
#include "vadefs.h"


#define va_start __CRT_VA_START
#define va_arg __CRT_VA_ARG
#define va_end __CRT_VA_END

#define va_copy(des, src) ((des) = (src))

#endif //STD_ARG_H_INCLUDED
