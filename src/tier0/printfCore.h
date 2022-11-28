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
#define __CORE_TEMP_STRING_SIZE 33

void _ParseNumberFromEscapeCode(const char * str, const uint32_t &index, uint32_t *count, uint32_t *num) {
	char numstr[__CORE_TEMP_STRING_SIZE] = {'\0'}; //This is just a random number.
	uint32_t charNum = 0;
	while (isdigit(str[index + *count])) {
        if(charNum == __CORE_TEMP_STRING_SIZE) {
            //Number too big.
            //TODO throw error?
        }
		numstr[charNum] = str[index + *count];
		charNum++;
		(*count)++;
	}
	//Convert string to number;
	*num = atoi(&numstr[0]);
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

int32_t _ParseEscapeCodes(const char * str, uint32_t &index, uint8_t &error) {
    //TODO change to uint8_t
    uint32_t count = 1; //number of processed chars.
    int32_t numL = -1; //This might have to be changed to uint8_t
    int32_t numR = -1; //This might have to be changed to uint8_t
    char command = '\0';
    size_t strLen = strlen(str);
    //TODO Need to do a check here to make sure we are not accessing memory we shouldn't.
    if(str[index + count] == '[') {
        count++;
        //Check if there is a number
        if(isdigit(str[index + count])) {
            _ParseNumberFromEscapeCode(str, index, &count, (uint32_t*)&numL);
            if(str[index + count] == ';') {
                count++;
                if(isdigit(str[index + count])) {
                    _ParseNumberFromEscapeCode(str, index, &count, (uint32_t*)&numR);
                }
                else {
                    //Error
                    error = 1;
                }
            }
        }
        if(isalpha(str[index + count])) {
            command = str[index + count];
            count++;
        }
        if(error != 1) {
        //Now we check if we have a useable command
        //Cursor Controls
            switch (command)
            {
            case 'H'://moves cursor to line #, column # or if no leading number goes too 0,0 \x1b[12;12H \x1b[H.
                if(numL == -1 && numR == -1) {
                    Terminal::GetInstance()->ResetColoum();
                    Terminal::GetInstance()->ResetRow();
                    break;
                }
            case 'f': //moves cursor to line #, column #
                Terminal::GetInstance()->SetColoum((uint8_t)numL);
                Terminal::GetInstance()->SetRow((uint8_t)numR);
                break;
            case 'A': //moves cursor up # lines
                //Do nothing at the moment.
                //TODO fill this out.
                break;
            case 'B':
                Terminal::GetInstance()->IncreaseRow((uint8_t)numL); //moves cursor down # lines
                break;
            default:
                break;
            }

        }

    }
    
    index += count;
    return count;
}

#endif //PRINTF_CODE_H_INCLUDED