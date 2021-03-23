#ifndef STD_LIB_H_INCLUDED
#define STD_LIB_H_INCLUDED

//This needs to be defined with the -D compile flag.
#ifdef __JS_OS
#include "../MemoryManager.h"
#endif //__JS_OS

#include "ctype.h"
#include "cstddef.h"

//Need to work out what to do with windows and linux.
#if defined(__linux__) && !defined(__JS_OS)
#endif

#if defined(__WIN32) && !defined(__JS_OS)
#endif

void* malloc(const unsigned int &size) {
  #if defined(__JS_OS)
  if(MemoryManager::p_activeMemoryManager) {
    return MemoryManager::p_activeMemoryManager->Malloc(size);
  }
  else {
    return NULL;
  }
  #elif defined(__linux__) && !defined(__JS_OS)
  #error "No malloc for linux build!"
  #elif defined(__WIN32) && !defined(__JS_OS)
  #error "No malloc for windows build!"
  #else
  #error "unknown OS so there is no way there is a malloc()."
  #endif //__JS_OS this is what we do on JSOS
  //TODO Work out windows and linux codes.
  return NULL;
}

void free(void* ptr) {
  #if defined(__JS_OS)
  if(MemoryManager::p_activeMemoryManager) {
    MemoryManager::p_activeMemoryManager->Free(ptr);
  }
  #elif defined(__linux__) && !defined(__JS_OS)
  #error "No free() for linux build!"
  #elif defined(__WIN32) && !defined(__JS_OS)
  #error "No free() for windows build!"
  #else
  #error "unknown OS so there is no way there is a free()."
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

char* uitoa(unsigned int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    unsigned int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );
    //Add the null terminator.
    *ptr-- = '\0';
    while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
    }
    return result;
}

int atoi(const char *s) {
  int n=0, neg=0;
	while (isspace(*s)) s++;
	switch (*s) {
	case '-': neg=1;
	case '+': s++;
	}
	/* Compute n as a negative number to avoid overflow on INT_MIN */
	while (isdigit(*s))
		n = 10*n - (*s++ - '0');
	return neg ? n : -n;
}
#ifdef __cplusplus
void* operator new(size_t size) {
  return malloc(size);
}

void* operator new[](size_t size) {
  return malloc(size);
}

//Placement new.
void* operator new(size_t size, void *ptr) {
  return ptr;
}

void* operator new[](size_t size, void *ptr) {
  return ptr;
}

void operator delete(void* ptr) {
  free(ptr);
}

void operator delete[](void* ptr) {
  free(ptr);
}
#endif //__cplusplus
#endif
