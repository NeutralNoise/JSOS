#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED
#include "../types.h" //Get those types you.
#include "../string.h"
#include "../stdlib.h"
#define USE_SCREEN_BUFFER
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define BUFFER_LINE_COUNT 26
#define BUFFER_SIZE (VGA_WIDTH * BUFFER_LINE_COUNT)
#define BUFFER_MEM_SIZE BUFFER_SIZE * sizeof(uint16_t)

//TODO display buffer.

enum vga_Colour {

    VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15

};

class Terminal {

public:
    Terminal() {

    }

    void SetColour( vga_Colour fg, vga_Colour bg) {
        m_colour = fg | bg << 4;
    }

    void SetCursorColour( vga_Colour c) {
        m_cursorColour = c | c << 4;
    }

   void PutCursorAt() {
       const size_t index = m_x + (m_bufferLine + m_y) * (int8_t)VGA_WIDTH;
       #ifndef USE_SCREEN_BUFFER
        p_buffer[index] = vga_Entry(' ', m_cursorColour);
       #else
        p_screenBuffer[index] = vga_Entry(' ', m_cursorColour);
        m_needsRedisplay = true;
        #endif
        
    }

    void PutCharAt(char c, vga_Colour colour, int8_t x, int8_t y) {
        #ifndef USE_SCREEN_BUFFER
        const size_t index = x + y * (int8_t)VGA_WIDTH;
        p_buffer[index] = vga_Entry(c, colour);
        #else
        const size_t index = x + (m_bufferLine + y) * (int8_t)VGA_WIDTH;
        p_screenBuffer[index] = vga_Entry(c, colour);
        m_needsRedisplay = true;
        #endif
    }

    void PutChar(char c, vga_Colour colour) {
        #ifndef USE_SCREEN_BUFFER
        vga_ClearBuff();
        #endif
        PutCharAt(c, colour, m_x, m_y);
        m_x++;
        m_viewPoint = m_bufferLine;
        if(m_x == VGA_WIDTH) {
            LineFeed();
        }
    }

    void PutChar(char c) {
        PutChar(c, (vga_Colour)m_colour);
    }

    void Write(const char *data, size_t size) {
        for(size_t i = 0; i < size; i++) {
            PutChar(data[i], (vga_Colour)m_colour);
        }
    }

    void WriteSrting(const char * data) {
        Write(data, strlen(data));
    }

    static void CreateInstance() {
        p_instance = new Terminal;
        p_instance->p_screenBuffer = new uint16_t[BUFFER_MEM_SIZE];
        memset((void *)p_instance->p_screenBuffer, 0, BUFFER_MEM_SIZE);
        p_instance->m_x = 0;
        p_instance->m_y = 0;
        p_instance->m_savedX = 0;
        p_instance->m_savedY = 0;
        p_instance->p_buffer = (uint16_t*)0xB8000;
        p_instance->SetColour(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
        p_instance->m_clearBuffer = false;
        p_instance->m_bufferLine = 0;
        p_instance->m_viewPoint = 0;
        p_instance->m_needsRedisplay = false;
        p_instance->SetCursorColour(VGA_COLOR_WHITE);
    }

    static Terminal* GetInstance() {
        return p_instance;
    }

    static void Destroy() {
       delete p_instance->p_screenBuffer;
       delete p_instance;
    }

    void LineFeed(){
        #ifndef USE_SCREEN_BUFFER
        m_x = 0;
        if(++m_y == VGA_HEIGHT) {
            m_y = 0;
            m_clearBuffer = true;
        }
        #else
        m_x = 0;
        m_y++;
        if(m_y == VGA_HEIGHT) {
            m_y = VGA_HEIGHT - 1;
            //small sanity check to make sure we are not over flowing the uint that is m_bufferLine. Its a snow flake!
            if((BUFFER_LINE_COUNT - VGA_HEIGHT) - 1 >= 0) {
                if(m_bufferLine < (BUFFER_LINE_COUNT - VGA_HEIGHT) - 1) {
                    m_bufferLine++;
                    //m_needsRedisplay = true;
                }
            }
            ScrollBuffer();
        }
        #endif
    }

    void IncreaseColoum() { 
        if(++m_x == VGA_WIDTH) {
             m_x = 0;
            //Should this increase y?
        }
    }
    void IncreaseRow() {         
        #ifndef USE_SCREEN_BUFFER
        if(++m_y == VGA_HEIGHT) {
            m_clearBuffer = true;
        }
        #else
        if(++m_y == VGA_HEIGHT) {
            m_y = VGA_HEIGHT - 1;
            if(m_bufferLine < (BUFFER_LINE_COUNT - VGA_HEIGHT)) {
                m_bufferLine++;
            }
            ScrollBuffer();
        } 
        #endif
        
    }

    void ResetColoum() { m_x = 0; }
    void ResetRow() { m_y = 0; }

    void SetColoum(uint8_t c) { m_x = c; }
    void SetRow(uint8_t r) { m_y = r; }

    void SetRowColoum(uint8_t x, uint8_t y) { m_x = x; m_y = y; }
    void GetRowColoum(uint8_t* x, uint8_t* y) { *x = m_x; *y = m_y; }

    void DisplayTerminal() {
        if(m_needsRedisplay) {
            uint32_t offset = 0 + m_viewPoint * VGA_WIDTH;
            memcpy(p_buffer, &p_screenBuffer[offset], (size_t)((VGA_WIDTH * VGA_HEIGHT) * sizeof(uint16_t)));
            PutCursorAt();
            size_t index = m_x + (m_bufferLine + m_y) * (int8_t)VGA_WIDTH;
            size_t cursorOffset = m_x + m_y * (int8_t)VGA_WIDTH;
            memcpy(&p_buffer[cursorOffset], &p_screenBuffer[index], sizeof(uint16_t));
            m_needsRedisplay = false;
        }
    }

    void SaveCursorPos() {
        m_savedX = m_x;
        m_savedY = m_y;
    }

    void RestoreCursorPos() {
        m_x = m_savedX;
        m_y = m_savedY;
    }

private:

    uint16_t vga_Entry(uint8_t uc, uint8_t colour) {
        return (uint16_t)uc | (uint16_t)colour << 8;
    }

    void vga_ClearBuff() {
        if(m_clearBuffer) {
            //Clear the entire buffer
            for(size_t x = 0; x < VGA_WIDTH; x++) {
                for(size_t y = 0; y < VGA_HEIGHT; y++) {
                    PutCharAt(' ', (vga_Colour)m_colour, x, y);
                }
            }
            m_clearBuffer = false;
        }
    }

    void ScrollBuffer() {
        size_t bufferOffset = 0 + 1 * VGA_WIDTH;
        size_t maxBufferOffset = 0 + (BUFFER_LINE_COUNT - 1) * VGA_WIDTH;
        size_t bufferSize = BUFFER_MEM_SIZE - (size_t)(sizeof(uint16_t) * VGA_WIDTH);
        memcpy(p_screenBuffer, &p_screenBuffer[bufferOffset], bufferSize);
        memset(&p_screenBuffer[maxBufferOffset], 0, VGA_WIDTH * sizeof(uint16_t));
        m_needsRedisplay = true;
    }

    int8_t m_x;
    int8_t m_y;
    int8_t m_savedX;
    int8_t m_savedY;
    int8_t m_colour;
    int8_t m_cursorColour;
    uint16_t * p_buffer;
    static Terminal * p_instance;
    bool m_clearBuffer;
    uint16_t * p_screenBuffer;
    uint32_t m_bufferLine; //What line in buffer is at the top of the screen.
    uint32_t m_viewPoint; //What is the current line we are viewing
    bool m_needsRedisplay;
};

Terminal * Terminal::p_instance = nullptr;

#endif //TERMINAL_H_INCLUDED