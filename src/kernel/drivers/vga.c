#include "vga.h"

static enum VgaColor s_ClearColor = Black;

void vga_set_cursor(unsigned int offset)
{
    char high = (offset >> 8) & 0xff;
    char low = (offset >> 0) & 0xff;
    
    out_port(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    out_port(VGA_DATA_REGISTER, high);
    out_port(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    out_port(VGA_DATA_REGISTER, low);
}

void vga_set_cursor_position(VgaCursorPosition cursor)
{
    if(cursor.Row > VGA_ROWS || cursor.Column > VGA_COLUMNS)
    {
        // TODO: Log this
        return;
    }

    unsigned offset = cursor.Row * VGA_COLUMNS + cursor.Column;

    vga_set_cursor(offset);
}

int vga_get_cursor()
{
    unsigned char high, low;

    out_port(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    high = in_port(VGA_DATA_REGISTER);
    out_port(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    low = in_port(VGA_DATA_REGISTER);

    unsigned int offset = (high << 8) | low;
    return offset;
}

VgaCursorPosition vga_get_cursor_position()
{
    unsigned int offset = vga_get_cursor();
    
    VgaCursorPosition position;

    position.Row = offset / VGA_ROWS;
    position.Column = offset % VGA_ROWS;
}

void vga_put_char_at_video_memory(char character, enum VgaColor foreground, int offset)
{
    unsigned char *vidmem = (unsigned char *) VGA_MEMORY_ADDRESS;
    vidmem[offset * 2] = character;
    vidmem[offset * 2 + 1] = (s_ClearColor << 4) | foreground;
}

void vga_print_string(char* string) 
{
    int offset = vga_get_cursor();
    int i = 0;
    while (string[i] != 0) {
        if(string[i] == '\n')
        {
            VgaCursorPosition cursor = vga_get_cursor_position();
            cursor.Row += 1;
            cursor.Column = 0;
            vga_set_cursor_position(cursor);
            offset = vga_get_cursor();
        }
        else 
        {
            vga_put_char_at_video_memory(string[i], White, offset);
            offset++;
        }
        
        i++;
    }
    vga_set_cursor(offset);
}

void vga_set_clear_color(enum VgaColor color)
{
    s_ClearColor = color;
}

void vga_clear_screen()
{
    vga_set_cursor(0);

    for(int i = 0; i < VGA_ROWS * VGA_COLUMNS; i++)
    {
        vga_put_char_at_video_memory(' ', Black, i);
    }

    vga_set_cursor(0);
}