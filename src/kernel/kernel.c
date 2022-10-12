#include "drivers/vga.h"

int kmain()
{
	vga_clear_screen(LightGray);

	vga_print_string("Hello world\nlol");
	
 	return 0;
}