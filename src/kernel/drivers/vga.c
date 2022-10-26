#include "vga.h"
#include "../clib/string.h"

static ColorPair s_Color = { VGA_COLOR_WHITE, VGA_COLOR_BLACK };
static ColorPair s_ColorHeader = { VGA_COLOR_RED, VGA_COLOR_LIGHTGRAY };
static ColorPair s_ColorFooter =  { VGA_COLOR_YELLOW, VGA_COLOR_DARKGRAY };

static char s_HeaderMessage[VGA_WIDTH] = { 0 };
static char s_FooterMessage[VGA_WIDTH] = { 0 };

static void s_SetCursorByOffset(uint32_t offset)
{
    char high = (offset >> 8) & 0xff;
    char low = (offset >> 0) & 0xff;
    
    OutPort(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    OutPort(VGA_DATA_REGISTER, high);
    OutPort(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    OutPort(VGA_DATA_REGISTER, low);
}

static uint32_t s_CursorFromCoords(uint8_t x, uint8_t y)
{
    if(x > VGA_WIDTH || y > VGA_HEIGHT)
    {
        // TODO: Log this
        return 0; // return zero for now
    }

    y += VGA_HEADER_SIZE;
    uint32_t offset = y * VGA_COLUMNS + x;
    return offset;
}

static uint32_t s_GetCursorOffsetGlobal()
{
    uint8_t high, low;

    OutPort(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    high = InPort(VGA_DATA_REGISTER);
    OutPort(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    low = InPort(VGA_DATA_REGISTER);

    unsigned int offset = (high << 8) | low;

    return offset;
}

static void s_PutChar(char c, uint32_t offset, ColorPair color)
{
    uint8_t* vidmem = (uint8_t *) VGA_MEMORY_ADDRESS;
    
    vidmem[offset * 2 + 0] = c;
    vidmem[offset * 2 + 1] = (color.Background << 4) | color.Foreground;
}

static void s_ScrollLine(uint32_t* offset)
{
    uint8_t* vidmem = (uint8_t*) VGA_MEMORY_ADDRESS;

    for(int i = 1; i < VGA_HEIGHT; i++)
    {
        int row = i + VGA_HEADER_SIZE;
        for(int col = 0; col < VGA_WIDTH; col++)
        {
            vidmem[((row - 1) * VGA_COLUMNS + col) * 2 + 0] = vidmem[(row * VGA_COLUMNS + col) * 2 + 0];
            vidmem[((row - 1) * VGA_COLUMNS + col) * 2 + 1] = vidmem[(row * VGA_COLUMNS + col) * 2 + 1];
        }
    }

    for(int col = 0; col < VGA_WIDTH; col++)
    {
        int row = VGA_HEIGHT + VGA_HEADER_SIZE;
        vidmem[((row - 1) * VGA_COLUMNS + col) * 2 + 0] = ' ';
        vidmem[((row - 1) * VGA_COLUMNS + col) * 2 + 1] = (s_Color.Background << 4) | s_Color.Foreground;
    }

    *offset = (VGA_ROWS - VGA_FOOTER_SIZE- 1) * VGA_COLUMNS;
}

static void s_PrintHeader()
{
    uint8_t* vidmem = (uint8_t*) VGA_MEMORY_ADDRESS;
    int len = StringLength(s_HeaderMessage);
    int start = VGA_WIDTH / 2 - len / 2;

    for(int i = 0; i < VGA_WIDTH; i++)
        s_PutChar(' ', i, s_ColorHeader);

    for(int i = 0; s_HeaderMessage[i] != '\0'; i++)
        s_PutChar(s_HeaderMessage[i], start + i, s_ColorHeader);
}

static void s_PrintFooter()
{
    uint8_t* vidmem = (uint8_t*) VGA_MEMORY_ADDRESS;
    int len = StringLength(s_FooterMessage);
    int start = VGA_WIDTH - len;

    for(int i = 0; i < VGA_WIDTH; i++)
        s_PutChar(' ', (VGA_ROWS - 1) * VGA_COLUMNS + i, s_ColorFooter);

    for(int i = 0; s_FooterMessage[i] != '\0'; i++)
        s_PutChar(s_FooterMessage[i], (VGA_ROWS - 1) * VGA_COLUMNS + start + i, s_ColorFooter);
}

static void s_ClearSafe()
{
    uint8_t* vidmem = (uint8_t*) VGA_MEMORY_ADDRESS;
    
    // Header
    for(int row = 0; row < VGA_HEADER_SIZE; row++)
    {
        for(int col = 0; col < VGA_WIDTH; col++)
        {
            vidmem[(row * VGA_WIDTH + col) * 2 + 1] = (s_ColorHeader.Background << 4) | s_ColorHeader.Foreground;
        }
    }

    // User
    for(int row = VGA_HEADER_SIZE; row < VGA_HEADER_SIZE + VGA_HEIGHT; row++)
    {
        for(int col = 0; col < VGA_WIDTH; col++)
        {
            vidmem[(row * VGA_WIDTH + col) * 2 + 1] = (s_Color.Background << 4) | s_Color.Foreground;
        }
    }

    // Footer
    for(int row = VGA_HEADER_SIZE + VGA_HEIGHT; row < VGA_ROWS; row++)
    {
        for(int col = 0; col < VGA_WIDTH; col++)
        {
            vidmem[(row * VGA_WIDTH + col) * 2 + 1] = (s_ColorFooter.Background << 4) | s_ColorFooter.Foreground;
        }
    }

    s_PrintHeader();
    s_PrintFooter();
}

///////////////////////////////////////////////////////////////////

void VgaSetCursor(uint8_t x, uint8_t y)
{
    s_SetCursorByOffset(s_CursorFromCoords(x, y));
}

void VgaGetCursor(uint8_t* x, uint8_t* y)
{
    uint32_t offset = s_GetCursorOffsetGlobal();
    offset -= VGA_COLUMNS * VGA_HEADER_SIZE;

    *y = offset / VGA_COLUMNS;
    *x = offset % VGA_COLUMNS;
}

ColorPair VgaGetColor()
{
    return s_Color;
}

ColorPair VgaGetColorHeader()
{
    return s_ColorHeader;
}

ColorPair VgaGetColorFooter()
{
    return s_ColorFooter;
}

void VgaSetColor(ColorPair color)
{
    s_Color = color;
}

void VgaSetColorHeader(ColorPair color)
{
    s_ColorHeader = color;
}

void VgaSetColorFooter(ColorPair color)
{
    s_ColorFooter = color;
}

int VgaGetForegroundColor()
{
    return s_Color.Foreground;
}

int VgaGetBackgroundColor()
{
    return s_Color.Background;
}

void VgaPrint(const char* string)
{
    VgaPrintColored(string, s_Color);
}

void VgaPrintColored(const char* string, ColorPair color)
{
    uint8_t* vidmem = (uint8_t *) VGA_MEMORY_ADDRESS;
    uint32_t offset = s_GetCursorOffsetGlobal();

    for(int i = 0; string[i] != '\0'; i++)
    {
        const char c = string[i];
        if(c == '\n')
        {
            offset += VGA_COLUMNS;
            offset -= offset % VGA_COLUMNS;
        }
        else 
        {
            s_PutChar(c, offset, color);
            offset++;
        }

        if(offset >= (VGA_ROWS - VGA_FOOTER_SIZE) * VGA_COLUMNS)
            s_ScrollLine(&offset);

        s_SetCursorByOffset(offset);
    }
}

void VgaBackspace()
{
    uint32_t offset = s_GetCursorOffsetGlobal();
    s_PutChar(' ', offset - 1, s_Color);
    s_SetCursorByOffset(offset - 1);
}

void VgaHeaderMessage(const char* headerMessage)
{
    StringCopy(s_HeaderMessage, headerMessage);
    s_ClearSafe();
}

void VgaFooterMessage(const char* footerMessage)
{
    StringCopy(s_FooterMessage, footerMessage);
    s_ClearSafe();
}

void VgaClear()
{
    for(int i = 0; i < VGA_ROWS; i++)
    {
        int row = i + VGA_HEADER_SIZE;
        for(int col = 0; col < VGA_WIDTH; col++)
        {
            s_PutChar(' ', i * VGA_WIDTH + col, s_Color);
        }
    }

    s_PrintHeader();
    s_PrintFooter();

    VgaSetCursor(0, 0);
}
