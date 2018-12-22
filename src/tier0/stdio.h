#ifndef __STDIO_H_INCLUDED
#define __STDIO_H_INCLUDED

void printf(char * str) {
  static uint16_t * VideoMemory = (uint16_t*)0xb8000;

  static uint8_t x=0, y=0;

  for(int i = 0; str[i] != '\0'; ++i) {

    switch (str[i]) {
      case '\n':
        y++;
        x=0;
        break;
      case '\r':
        x = 0;
        break;
      default:
        VideoMemory[x + y * 80] = (VideoMemory[x + y * 80] & 0xFF00) | str[i];
        x++;
        break;
    }
    if(x == 80) {
      y++;
      x=0;
    }
    //Clear the screen if at the bottom of the screen
    if(y == 25) {
      for(y = 0; y < 25; y++) {
        for(x = 0; x < 25; x++) {
          VideoMemory[x + y * 80] = (VideoMemory[x + y * 80] & 0xFF00) | ' ';
        }
      }

      x = 0;
      y = 0;
    }
  }
}

#endif //__STDIO_H_INCLUDED
