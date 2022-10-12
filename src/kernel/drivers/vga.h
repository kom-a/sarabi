#include "driver.h"

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

#define VGA_MEMORY_ADDRESS 0xb8000
#define VGA_ROWS 25
#define VGA_COLUMNS 80

typedef struct
{
    unsigned int Row;
    unsigned int Column;
} VgaCursorPosition;

enum VgaColor
{
    Black = 0,
    Blue,
    Green,
    Cyan,
    Red,
    Purple,
    Brown,
    LightGray,
    DarkGray,
    LightBlue,
    LightGreen,
    LightCyan,
    LightRed,
    LightMagenta,
    Yellow,
    White
};

void vga_set_cursor(unsigned int offset);
void vga_set_cursor_position(VgaCursorPosition cursor);

int vga_get_cursor();
VgaCursorPosition vga_get_cursor_position();

void vga_put_char_at_video_memory(char character, enum VgaColor foreground, enum VgaColor background, int offset);
void vga_print_string(char* string);

void vga_clear_screen(enum VgaColor clear_color);