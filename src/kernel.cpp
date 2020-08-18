//#include "types.h"
#include "gdt.h"
#include "tier0/types.h"
#include "tier0/stdio.h"
#include "MemoryManager.h"
#include "tier0/stdlib.h"
//Call constructors for globals
typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors() {
  for(constructor* i = &start_ctors; i != &end_ctors; i++) {
    (*i)();
  }
}

extern "C" void kernelMain(void* multiboot_struct, uint32_t magicNum) {
  char * d = "Hello World\n";
  printf(d);
  printf("JSOS Version: 0.00000000000001\n\0");
  char *testd = "Hello World";
  GlobalDescriptorTable gdt;
  uint32_t* memupper = (uint32_t*)(((size_t)multiboot_struct) + 8);
  size_t heap = 10*1024*1024;
  MemoryManager memManager(heap, (*memupper)*1024 - heap - 10*1024);
  int32_t * memTest = nullptr;
  memTest = (int32_t*)malloc(sizeof(int32_t));
  free((void*)memTest);
  while(1);
}
