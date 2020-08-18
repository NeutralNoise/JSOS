#ifndef __STDIO_H_INCLUDED
#define __STDIO_H_INCLUDED
#include "sys/terminal.h"

void printf(char * str, ...) {
  //static uint16_t * VideoMemory = (uint16_t*)0xb8000;
  for(size_t i = 0; i < strlen(str); i++) {
    switch (str[i]) {
      case '\n':
        Terminal::GetInstance()->IncreaseRow();
        Terminal::GetInstance()->ResetColoum();
        break;
      case '\r':
        Terminal::GetInstance()->ResetColoum();
        break;
      default:
        Terminal::GetInstance()->PutChar(str[i]);
        break;
    }
  }
}

#endif //__STDIO_H_INCLUDED
