#ifndef PRINTF_CODE_H_INCLUDED
#define PRINTF_CODE_H_INCLUDED
#include "stdarg.h"
#include "sys/terminal.h"
#include "stdlib.h"

int ParseFormating(char * str, unsigned int &index, va_list *arg) {
    int count = 0;
    switch (str[index + 1])
    {
    case 's':
        {
            char * tmpStr = va_arg(*arg, char*);
            while(*tmpStr != '\0') {
                Terminal::GetInstance()->PutChar(*tmpStr);
                tmpStr++;
                count++;
            }
            index++;
        }
        break;
    case 'i':
        {
            int tmpInt = va_arg(*arg, int);
            char tmpStr[33];
            itoa(tmpInt, tmpStr, 10);
            int i = 0;
            while(tmpStr[i] != '\0') {
                Terminal::GetInstance()->PutChar(tmpStr[i]);
                tmpStr[i];
                i++;
                count++;
            }
            index++;
        }
        break;
    default:
        break;
    }
    return count;
}


#endif //PRINTF_CODE_H_INCLUDED