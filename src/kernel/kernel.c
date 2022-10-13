#include "drivers/vga.h"

int kmain()
{
	vga_set_clear_color(DarkGray);

	vga_clear_screen();

	vga_print_string("Hello world\nlol");
	
 	return 0;
}