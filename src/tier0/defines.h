#ifndef DEFINES_H_STD_INCLUDED
#define DEFINES_H_STD_INCLUDED

#define MB 1000000 //!< 1 MB as bytes.

#ifdef NULL
#undef NULL
//#define NULL 0
#define NULL nullptr;
#else
//#define NULL 0
#define NULL nullptr;
#endif

#endif //DEFINES_H_STD_INCLUDED
