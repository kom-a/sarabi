#include "drivers/vga.h"
#include "inttypes.h"
#include "PIC.h"
#include "clib/string.h"

void print_letter(uint8_t scancode) {
    switch (scancode) {
        case 0x0:
            VgaPrint("ERROR");
            break;
        case 0x1:
            VgaPrint("ESC");
            break;
        case 0x2:
            VgaPrint("1");
            break;
        case 0x3:
            VgaPrint("2");
            break;
        case 0x4:
            VgaPrint("3");
            break;
        case 0x5:
            VgaPrint("4");
            break;
        case 0x6:
            VgaPrint("5");
            break;
        case 0x7:
            VgaPrint("6");
            break;
        case 0x8:
            VgaPrint("7");
            break;
        case 0x9:
            VgaPrint("8");
            break;
        case 0x0A:
            VgaPrint("9");
            break;
        case 0x0B:
            VgaPrint("0");
            break;
        case 0x0C:
            VgaPrint("-");
            break;
        case 0x0D:
            VgaPrint("+");
            break;
        case 0x0E:
            VgaPrint("Backspace");
            break;
        case 0x0F:
            VgaPrint("Tab");
            break;
        case 0x10:
            VgaPrint("Q");
            break;
        case 0x11:
            VgaPrint("W");
            break;
        case 0x12:
            VgaPrint("E");
            break;
        case 0x13:
            VgaPrint("R");
            break;
        case 0x14:
            VgaPrint("T");
            break;
        case 0x15:
            VgaPrint("Y");
            break;
        case 0x16:
            VgaPrint("U");
            break;
        case 0x17:
            VgaPrint("I");
            break;
        case 0x18:
            VgaPrint("O");
            break;
        case 0x19:
            VgaPrint("P");
            break;
        case 0x1A:
            VgaPrint("[");
            break;
        case 0x1B:
            VgaPrint("]");
            break;
        case 0x1C:
            VgaPrint("\n");
            break;
        case 0x1D:
            VgaPrint("LCtrl");
            break;
        case 0x1E:
            VgaPrint("A");
            break;
        case 0x1F:
            VgaPrint("S");
            break;
        case 0x20:
            VgaPrint("D");
            break;
        case 0x21:
            VgaPrint("F");
            break;
        case 0x22:
            VgaPrint("G");
            break;
        case 0x23:
            VgaPrint("H");
            break;
        case 0x24:
            VgaPrint("J");
            break;
        case 0x25:
            VgaPrint("K");
            break;
        case 0x26:
            VgaPrint("L");
            break;
        case 0x27:
            VgaPrint(";");
            break;
        case 0x28:
            VgaPrint("'");
            break;
        case 0x29:
            VgaPrint("`");
            break;
        case 0x2A:
            VgaPrint("LShift");
            break;
        case 0x2B:
            VgaPrint("\\");
            break;
        case 0x2C:
            VgaPrint("Z");
            break;
        case 0x2D:
            VgaPrint("X");
            break;
        case 0x2E:
            VgaPrint("C");
            break;
        case 0x2F:
            VgaPrint("V");
            break;
        case 0x30:
            VgaPrint("B");
            break;
        case 0x31:
            VgaPrint("N");
            break;
        case 0x32:
            VgaPrint("M");
            break;
        case 0x33:
            VgaPrint(",");
            break;
        case 0x34:
            VgaPrint(".");
            break;
        case 0x35:
            VgaPrint("/");
            break;
        case 0x36:
            VgaPrint("Rshift");
            break;
        case 0x37:
            VgaPrint("Keypad *");
            break;
        case 0x38:
            VgaPrint("LAlt");
            break;
        case 0x39:
            VgaPrint(" ");
            break;
        default:
            /* 'keuyp' event corresponds to the 'keydown' + 0x80
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            if (scancode <= 0x7f) {
                VgaPrint("Unknown key down");
            } else if (scancode <= 0x39 + 0x80) {
                //VgaPrint("key up ");
                //print_letter(scancode - 0x80);
            } else VgaPrint("Unknown key up");
            break;
    }
}

void keyboard_callback(Registers *regs) {
    uint8_t scancode = InPort(0x60);

    if (scancode <= 0x7f)
        print_letter(scancode);
}

int kmain()
{    
	VgaHeaderMessage("Sarabi OS");
    VgaFooterMessage("This is some useful and helpful information");
    VgaClear();
    
    VgaPrint("Hello world");

	PIC_Init(0x20);
    
	register_interrupt_handler(IRQ1, keyboard_callback);
	
    asm volatile ("hlt");

 	return 0;
}
