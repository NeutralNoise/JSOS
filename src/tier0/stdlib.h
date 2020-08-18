#ifndef STD_LIB_H_INCLUDED
#define STD_LIB_H_INCLUDED
//This is just here when working on this file. Its a bitch to read other wise.
//#define __JS_OS

//This needs to be defined with the -D compile flag.
#ifdef __JS_OS
#include "../MemoryManager.h"
#endif //__JS_OS

//Need to work out what to do with windows and linux.
#if defined(__linux__) && !defined(__JS_OS)
#endif

#if defined(__WIN32) && !defined(__JS_OS)
#endif

void* malloc(const unsigned int &size) {
  #ifdef __JS_OS
  if(MemoryManager::p_activeMemoryManager) {
    return MemoryManager::p_activeMemoryManager->Malloc(size);
  }
  else {
    return nullptr;
  }
  #endif //__JS_OS this is what we do on JSOS
  //TODO Work out windows and linux codes.
}

void free(void* ptr) {
  #ifdef __JS_OS
  if(MemoryManager::p_activeMemoryManager) {
    MemoryManager::p_activeMemoryManager->Free(ptr);
  }
  #endif //__JS_OS this is what we do on JSOS
  //TODO Work out windows and linux codes.
}


/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 * Found at https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c
 */
char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

#endif
