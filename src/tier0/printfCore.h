#ifndef PRINTF_CODE_H_INCLUDED
#define PRINTF_CODE_H_INCLUDED
#include "stdarg.h"
#include "sys/terminal.h"
#include "stdlib.h"
#include "ctype.h"
//used for debugging.
/*
//WARN can go off screen and crash
void __DisplayCurrentNumber(uint32_t index, vga_Colour colour = VGA_COLOR_LIGHT_BLUE) {
    static int x = 0;
    static int y = 14;
    char testStr[32] = {'\0'};
    itoa(index, testStr, 10);
    for(int i =0 ; i < strlen(testStr);i++) {
        Terminal::GetInstance()->PutCharAt(testStr[i], colour, x, y);
        x++;
    }
    Terminal::GetInstance()->PutCharAt(':', VGA_COLOR_GREEN, x, y);
    x++;
}

void __DisplayCurrentString(char *str) {
    static int x = 0;
    static int y = 15;
    char testStr[32] = {'\0'};
    for(int i =0 ; i < strlen(testStr);i++) {
        if(str[i] != '\0')
        {
            Terminal::GetInstance()->PutCharAt(str[i], VGA_COLOR_GREEN, x, y);
            x++;
        }
        
    }
    Terminal::GetInstance()->PutCharAt(':', VGA_COLOR_GREEN, x, y);
    x++;
}
*/
//#define __CORE_TEMP_STRING_SIZE 33

int32_t _ParseNumberFromEscapeCode(const char * str, uint32_t &index, uint32_t &count) {
    int32_t rtn = -1; // -1 for error. Not a number.
    char temp[33] = {'\0'};
    size_t strLen = strlen(str);
    //Convert the cache chars then convert to int.
    
    for(uint32_t i = 0; /*will break else where*/ ;i++) {
        // sanity check
        if(i == 33 || i >= strLen) {
            count++;
            rtn = atoi(temp);
            break;
        }
        //There is no more numbers
        if(!isdigit(str[index + count])) {
            rtn = atoi(temp);
            break;
        }
        temp[i] = str[index + count];
        count++;
    }
    return rtn;
}

uint32_t _ParseFormating(const char * str, uint32_t &index, va_list *arg) {
    int32_t count = 0;
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
            char tmpStr[33]; //why this this 33? cause the max in a 
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

int32_t _ParseEscapeCodes(const char * str, uint32_t &index) {
    uint32_t count = 0; //number of processed chars.
    int32_t numX = -1;
    int32_t numY = -1;
    size_t strLen = strlen(str);
    //TODO Need to do a check here to make sure we are not accessing memory we shouldn't.
    if(str[index + 1] == '[') {
        count++;
        char testStr[32] = {'\0'};
        //Check if there are numbers after the '['
        if(isdigit(str[index + count + 1])) {
            count++;
            numX = _ParseNumberFromEscapeCode(str, index, count);
            //Check if there is something after the number.
            if((index + count + 1) < strLen || str[index + count] != '\0' ) {
                //check if there are more numbers.
                if(str[index + count] == ';') {
                    count++;
                    numY = _ParseNumberFromEscapeCode(str, index, count);
                }
            }
        }
        //Cursor Controls
        //we have some numbers to do shit with.
        if(numX != -1 || numY != -1) {
            //Terminal::GetInstance()->PutCharAt('y',VGA_COLOR_LIGHT_BLUE, 14, 14);
            if (str[index + count ] == 'H' || str[index + count] == 'f') { //moves cursor to line #, column #
                if(numX != -1) {
                    Terminal::GetInstance()->SetColoum((uint8_t)numX);
                }
                if(numY != -1) {
                    Terminal::GetInstance()->SetRow((uint8_t)numY);
                }
                count++;
            }
        }
        else if (str[index + count + 1] == 'H') //moves cursor to home position (0, 0)
        {
        
            Terminal::GetInstance()->ResetColoum();
            Terminal::GetInstance()->ResetRow();
            count++;
        }
        else if (str[index + count + 1] == 's') //save cursor position (SCO)
        {
            Terminal::GetInstance()->SaveCursorPos();
            count++;
        }
        else if (str[index + count + 1] == 'u') //restores the cursor to the last saved position (SCO)
        {
            Terminal::GetInstance()->RestoreCursorPos();
            count++;
        }

    }
    else {
        //sometimes theres other things. ignore them for now
    }
    index += count;
    return count;
}

#endif //PRINTF_CODE_H_INCLUDED