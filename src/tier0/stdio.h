#ifndef __STDIO_H_INCLUDED
#define __STDIO_H_INCLUDED
#include "sys/terminal.h"
#include "stdarg.h"
#include "printfCore.h"

int printf(char * str, ...) {
  int count = 0;
  uint8_t err = 0;
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
        count += _ParseFormating(str, i, &arg);
        break;
      case '\x1b': 
      //case '\33': //is valid.
      //case '\e': //is valid.
        count += _ParseEscapeCodes(str, i, err);
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
