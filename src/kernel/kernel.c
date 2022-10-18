#include "drivers/vga.h"
#include "inttypes.h"
#include "PIC.h"

extern void AsmHandler();
extern void KeyboardHandlerAsm();

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

void DefaultInterruptHandler()
{
// 	vga_print_string("Interrupt occurred\n");

	char str[16];
	// to_hex_str(gate, str);

	// vga_print_string(str);
	// vga_print_string("\n");

	for(int i = 0; i < IDT_ENTRIES; i++)
		PIC_EndOfInterrupt(i);
}

void KeyboardHandler()
{
	vga_print_string("Key pressed\n");

	for(int i = 0; i < IDT_ENTRIES; i++)
		PIC_EndOfInterrupt(i);

	for(int i = 0; i < 500000000;)
		i++;
}

int kmain()
{
	vga_print_string("Welcome to Sarabi OS\n");

	PIC_Remap(0x20);

	for(int i = 0; i < IDT_ENTRIES; i++)
	{
		SetIDTGateHandler(i, AsmHandler);
	}

	SetIDTGateHandler(0x21, KeyboardHandlerAsm);

	IRQ_Mask(0);
	LoadIDT();

	asm volatile("sti");

	int i  = 0;

	while(1)
	{
		char str[16];
		to_hex_str(i++, str);
		vga_print_string(str);
		vga_print_string("\n");

		for(int k = 0; k < 10000000; )
			k++;
	}

 	return 0;
}