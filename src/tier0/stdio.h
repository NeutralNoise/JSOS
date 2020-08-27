#ifndef __STDIO_H_INCLUDED
#define __STDIO_H_INCLUDED
#include "sys/terminal.h"
#include "stdarg.h"
#include "printfCore.h"

int printf(char * str, ...) {
  int count = 0;
  va_list arg;
    
  va_start(arg, str);
  for(size_t i = 0; i < strlen(str); i++) {
    switch (str[i]) {
      case '\n':
        Terminal::GetInstance()->LineFeed();
        count++;
        break;
      case '\r':
        Terminal::GetInstance()->ResetColoum();
        count++;
        break;
      case '\\':
        Terminal::GetInstance()->PutChar(str[i]);
        count++;
        break;
      case '%':
        count += ParseFormating(str, i, &arg);
        break;
      default:
        Terminal::GetInstance()->PutChar(str[i]);
        count++;
        break;
    }

  }
  va_end(arg);
  return count;
}

#endif //__STDIO_H_INCLUDED
