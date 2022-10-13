#include "drivers/vga.h"
#include "inttypes.h"

void to_hex_str(uint32_t value, char* str)
{
	int str_index = 0;
	str[str_index++] = '0';
	str[str_index++] = 'x';

	for(int i = 7; i >= 0; i--)
	{
		unsigned char digit = (value >> (i * 4)) & 0xf;
		str[str_index++] = (digit <= 9) ? digit + '0' : digit - 9 + 'a' - 1;
	}

	str[str_index] = '\0';
}

int kmain()
{
	vga_set_clear_color(DarkGray);

	vga_clear_screen();

	uint32_t test = 0x00;

	vga_print_string("Hello w\norld");

	while(1)
	{
		uint8_t data = in_port(0x60);
		uint8_t status = in_port(0x64);

		char str[16];
		to_hex_str(data, str);

		vga_set_cursor(0);

		vga_print_string("data: ");
		vga_print_string(str);
		vga_print_string("\n");

		to_hex_str(status, str);
		vga_print_string("status: ");
		vga_print_string(str);
		vga_print_string("\n");
	}

 	return 0;
}