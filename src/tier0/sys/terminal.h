#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED
#include "../types.h" //Get those types you.
#include "../string.h"
#include "../stdlib.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

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

    void PutCharAt(char c, vga_Colour colour, int8_t x, int8_t y) {
        const size_t index = x + y * (int8_t)VGA_WIDTH;
        p_buffer[index] = vga_Entry(c, colour);
    }

    void PutChar(char c, vga_Colour colour) {
        vga_ClearBuff();
        PutCharAt(c, colour, m_x, m_y);
        m_x++;
        if(m_x == VGA_WIDTH) {
            m_x = 0;
            m_y++;
        }
        if(m_y == VGA_HEIGHT) {
            m_y = 0;
            m_clearBuffer = true;
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
        p_instance = (Terminal*)malloc(sizeof(Terminal));
        p_instance->m_x = 0;
        p_instance->m_y = 0;
        p_instance->p_buffer = (uint16_t*)0xB8000;
        p_instance->SetColour(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
        p_instance->m_clearBuffer = false;
    }

    static Terminal* GetInstance() {
        return p_instance;
    }

    static void Destroy() {
        free((void*)p_instance);
    }

    void IncreaseColoum() { 
        if(++m_x == VGA_WIDTH) {
             m_x = 0;
            //Should this increase y?
        }
    }
    void IncreaseRow() {         
        if(++m_y == VGA_HEIGHT) {
            m_y = 0;
            //Should this reset x?
            m_clearBuffer = true;
        } 
    }

    void ResetColoum() { m_x = 0; }
    void ResetRow() { m_y = 0; }

    void SetColoum(uint8_t c) { m_x = c; }
    void SetRow(uint8_t r) { m_y = r; }

    void SetRowColoum(uint8_t x, uint8_t y) { m_x = x; m_y = y; }
    void GetRowColoum(uint8_t* x, uint8_t* y) { *x = m_x; *y = m_y; }

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

    int8_t m_x;
    int8_t m_y;
    int8_t m_colour;
    uint16_t * p_buffer;
    static Terminal * p_instance;
    bool m_clearBuffer;
};

Terminal * Terminal::p_instance = nullptr;

#endif //TERMINAL_H_INCLUDED