#ifndef CSTDDEF_H_INCLUDED
#define CSTDDEF_H_INCLUDED

#ifndef NULL
    #if !defined(__cplusplus) //We are using some c compiler.
    #define NULL 0
    #elif defined(__cplusplus)
        #if __cplusplus <= 199711L
        #define NULL 0
        #warning Please use a c++11 compiler!
        #elif __cplusplus >= 201103L
        #define NULL nullptr
        #endif
    #endif
#endif

#endif //CSTDDEF_H_INCLUDED