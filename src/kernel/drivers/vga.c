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

    // if(character == '\n')
    // {
    //     VgaCursorPosition cursor = vga_get_cursor_position();
    //     cursor.Row += 1;
    //     cursor.Column = 0;

    //     if(cursor.Row >= VGA_ROWS)
    //         vga_scroll_line(&cursor);

    //     vga_set_cursor_position(cursor);
    //     offset = vga_get_cursor();
    // }

    vidmem[offset * 2 + 0] = character;
    vidmem[offset * 2 + 1] = (s_ClearColor << 4) | foreground;
}

void vga_print_char(char c, enum VgaColor color)
{
    int offset = vga_get_cursor();

    if(c == '\n')
    {
        offset += VGA_COLUMNS;
        offset -= offset % VGA_COLUMNS;
    }
    else 
    {
        vga_put_char_at_video_memory(c, color, offset);
        offset += 1;
    }

    if(offset >= VGA_ROWS * VGA_COLUMNS)
        vga_scroll_line(&offset);
    
    vga_set_cursor(offset);
}

void vga_print_string(char* string) 
{
    int i = 0;
    while (string[i] != 0) 
    {
        vga_print_char(string[i], White);
        
        i++;
    }
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

void vga_scroll_line(int* offset)
{
    unsigned char* vidmem = (unsigned char *) VGA_MEMORY_ADDRESS;

    for(int row = 1; row < VGA_ROWS; row++)
    {
        for(int col = 0; col < VGA_COLUMNS; col++)
        {
            vidmem[((row - 1) * VGA_COLUMNS + col) * 2 + 0] = vidmem[(row * VGA_COLUMNS + col) * 2 + 0];
            vidmem[((row - 1) * VGA_COLUMNS + col) * 2 + 1] = vidmem[(row * VGA_COLUMNS + col) * 2 + 1];
        }
    }

    for(int i = 0; i < VGA_COLUMNS; i++)
    {
        vidmem[((VGA_ROWS - 1) * VGA_COLUMNS + i) * 2 + 0] = ' ';
        vidmem[((VGA_ROWS - 1) * VGA_COLUMNS + i) * 2 + 1] = (s_ClearColor << 4) | Black;
    }

    *offset = (VGA_ROWS - 1) * VGA_COLUMNS;
}

int vga_cursor_to_offset(VgaCursorPosition cursor)
{
    return cursor.Row * VGA_COLUMNS + cursor.Column;
}