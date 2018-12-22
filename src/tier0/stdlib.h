#ifndef STD_LIB_H_INCLUDED
#define STD_LIB_H_INCLUDED
//This is jsut here when working on this file. Its a bitch to read other wise.
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

#endif
