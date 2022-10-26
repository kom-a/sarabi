#include "driver.h"
#include "../inttypes.h"

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

#define VGA_MEMORY_ADDRESS 0xb8000
#define VGA_ROWS 25
#define VGA_COLUMNS 80

#define VGA_HEADER_SIZE 1
#define VGA_FOOTER_SIZE 1

#define VGA_WIDTH VGA_COLUMNS
#define VGA_HEIGHT (VGA_ROWS - VGA_HEADER_SIZE - VGA_FOOTER_SIZE)

#define VGA_COLOR_BLACK 0
#define VGA_COLOR_BLUE 1
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_CYAN 3
#define VGA_COLOR_RED 4
#define VGA_COLOR_PURPLE 5
#define VGA_COLOR_BROWN 6
#define VGA_COLOR_LIGHTGRAY 7
#define VGA_COLOR_DARKGRAY 8
#define VGA_COLOR_LIGHTBLUE 9
#define VGA_COLOR_LIGHTGREEN 10
#define VGA_COLOR_LIGHTCYAN 11
#define VGA_COLOR_LIGHTRED 12
#define VGA_COLOR_LIGHTMAGENTA 13
#define VGA_COLOR_YELLOW 14
#define VGA_COLOR_WHITE 15

typedef struct
{
    uint32_t Foreground;
    uint32_t Background;
} ColorPair;

void VgaGetCursor(uint8_t* x, uint8_t* y);
void VgaSetCursor(uint8_t x, uint8_t y);

ColorPair VgaGetColor();
ColorPair VgaGetColorHeader();
ColorPair VgaGetColorFooter();
void VgaSetColor(ColorPair color);
void VgaSetColorHeader(ColorPair color);
void VgaSetColorFooter(ColorPair color);
int VgaGetForegroundColor();
int VgaGetBackgroundColor();

void VgaPrint(const char* string);
void VgaPrintColored(const char* string, ColorPair color);
void VgaBackspace();

void VgaHeaderMessage(const char* headerMessage);
void VgaFooterMessage(const char* footerMessage);

void VgaClear();
