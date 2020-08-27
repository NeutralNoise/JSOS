//#include "types.h"
#include "gdt.h"
#include "tier0/types.h"
#include "tier0/stdio.h"
#include "MemoryManager.h"
#include "tier0/stdlib.h"
#include "tier0/sys/terminal.h"
#include "KernelDefines.h"

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
  GlobalDescriptorTable gdt;
  uint32_t* memupper = (uint32_t*)(((size_t)multiboot_struct) + 8);
  size_t heap = 10*1024*1024;
  MemoryManager memManager(heap, (*memupper)*1024 - heap - 10*1024);

  Terminal::CreateInstance();
  char welcomeMsg[13] = "Hello World\n";
  char verMsg[19] = "JSOS Version: %s \n";
  char dateMsg[20] = "Built on: %s at %s\n";
  printf(welcomeMsg);
  printf(verMsg, __OS_VERSION);
  printf(dateMsg, __OS_BUILD_DATE, __OS_BUILD_TIME);
  while(1) {
    Terminal::GetInstance()->DisplayTerminal();    
  }
}
