#include "drivers/vga.h"
#include "inttypes.h"
#include "PIC.h"

void isr_install()
{
SetIDTGateHandler(0, (uint32_t) isr0);
    SetIDTGateHandler(1, (uint32_t) isr1);
    SetIDTGateHandler(2, (uint32_t) isr2);
    SetIDTGateHandler(3, (uint32_t) isr3);
    SetIDTGateHandler(4, (uint32_t) isr4);
    SetIDTGateHandler(5, (uint32_t) isr5);
    SetIDTGateHandler(6, (uint32_t) isr6);
    SetIDTGateHandler(7, (uint32_t) isr7);
    SetIDTGateHandler(8, (uint32_t) isr8);
    SetIDTGateHandler(9, (uint32_t) isr9);
    SetIDTGateHandler(10, (uint32_t) isr10);
    SetIDTGateHandler(11, (uint32_t) isr11);
    SetIDTGateHandler(12, (uint32_t) isr12);
    SetIDTGateHandler(13, (uint32_t) isr13);
    SetIDTGateHandler(14, (uint32_t) isr14);
    SetIDTGateHandler(15, (uint32_t) isr15);
    SetIDTGateHandler(16, (uint32_t) isr16);
    SetIDTGateHandler(17, (uint32_t) isr17);
    SetIDTGateHandler(18, (uint32_t) isr18);
    SetIDTGateHandler(19, (uint32_t) isr19);
    SetIDTGateHandler(20, (uint32_t) isr20);
    SetIDTGateHandler(21, (uint32_t) isr21);
    SetIDTGateHandler(22, (uint32_t) isr22);
    SetIDTGateHandler(23, (uint32_t) isr23);
    SetIDTGateHandler(24, (uint32_t) isr24);
    SetIDTGateHandler(25, (uint32_t) isr25);
    SetIDTGateHandler(26, (uint32_t) isr26);
    SetIDTGateHandler(27, (uint32_t) isr27);
    SetIDTGateHandler(28, (uint32_t) isr28);
    SetIDTGateHandler(29, (uint32_t) isr29);
    SetIDTGateHandler(30, (uint32_t) isr30);
    SetIDTGateHandler(31, (uint32_t) isr31);

    // Remap the PIC
    OutPort(0x20, 0x11);
    OutPort(0xA0, 0x11);
    OutPort(0x21, 0x20);
    OutPort(0xA1, 0x28);
    OutPort(0x21, 0x04);
    OutPort(0xA1, 0x02);
    OutPort(0x21, 0x01);
    OutPort(0xA1, 0x01);
    OutPort(0x21, 0x0);
    OutPort(0xA1, 0x0);

    // Install the IRQs
    SetIDTGateHandler(32, (uint32_t)irq0);
    SetIDTGateHandler(33, (uint32_t)irq1);
    SetIDTGateHandler(34, (uint32_t)irq2);
    SetIDTGateHandler(35, (uint32_t)irq3);
    SetIDTGateHandler(36, (uint32_t)irq4);
    SetIDTGateHandler(37, (uint32_t)irq5);
    SetIDTGateHandler(38, (uint32_t)irq6);
    SetIDTGateHandler(39, (uint32_t)irq7);
    SetIDTGateHandler(40, (uint32_t)irq8);
    SetIDTGateHandler(41, (uint32_t)irq9);
    SetIDTGateHandler(42, (uint32_t)irq10);
    SetIDTGateHandler(43, (uint32_t)irq11);
    SetIDTGateHandler(44, (uint32_t)irq12);
    SetIDTGateHandler(45, (uint32_t)irq13);
    SetIDTGateHandler(46, (uint32_t)irq14);
    SetIDTGateHandler(47, (uint32_t)irq15);

	LoadIDT(); // Load with ASM
}

void print_letter(uint8_t scancode) {
    switch (scancode) {
        case 0x0:
            vga_print_string("ERROR");
            break;
        case 0x1:
            vga_print_string("ESC");
            break;
        case 0x2:
            vga_print_string("1");
            break;
        case 0x3:
            vga_print_string("2");
            break;
        case 0x4:
            vga_print_string("3");
            break;
        case 0x5:
            vga_print_string("4");
            break;
        case 0x6:
            vga_print_string("5");
            break;
        case 0x7:
            vga_print_string("6");
            break;
        case 0x8:
            vga_print_string("7");
            break;
        case 0x9:
            vga_print_string("8");
            break;
        case 0x0A:
            vga_print_string("9");
            break;
        case 0x0B:
            vga_print_string("0");
            break;
        case 0x0C:
            vga_print_string("-");
            break;
        case 0x0D:
            vga_print_string("+");
            break;
        case 0x0E:
            vga_print_string("Backspace");
            break;
        case 0x0F:
            vga_print_string("Tab");
            break;
        case 0x10:
            vga_print_string("Q");
            break;
        case 0x11:
            vga_print_string("W");
            break;
        case 0x12:
            vga_print_string("E");
            break;
        case 0x13:
            vga_print_string("R");
            break;
        case 0x14:
            vga_print_string("T");
            break;
        case 0x15:
            vga_print_string("Y");
            break;
        case 0x16:
            vga_print_string("U");
            break;
        case 0x17:
            vga_print_string("I");
            break;
        case 0x18:
            vga_print_string("O");
            break;
        case 0x19:
            vga_print_string("P");
            break;
        case 0x1A:
            vga_print_string("[");
            break;
        case 0x1B:
            vga_print_string("]");
            break;
        case 0x1C:
            vga_print_string("\n");
            break;
        case 0x1D:
            vga_print_string("LCtrl");
            break;
        case 0x1E:
            vga_print_string("A");
            break;
        case 0x1F:
            vga_print_string("S");
            break;
        case 0x20:
            vga_print_string("D");
            break;
        case 0x21:
            vga_print_string("F");
            break;
        case 0x22:
            vga_print_string("G");
            break;
        case 0x23:
            vga_print_string("H");
            break;
        case 0x24:
            vga_print_string("J");
            break;
        case 0x25:
            vga_print_string("K");
            break;
        case 0x26:
            vga_print_string("L");
            break;
        case 0x27:
            vga_print_string(";");
            break;
        case 0x28:
            vga_print_string("'");
            break;
        case 0x29:
            vga_print_string("`");
            break;
        case 0x2A:
            vga_print_string("LShift");
            break;
        case 0x2B:
            vga_print_string("\\");
            break;
        case 0x2C:
            vga_print_string("Z");
            break;
        case 0x2D:
            vga_print_string("X");
            break;
        case 0x2E:
            vga_print_string("C");
            break;
        case 0x2F:
            vga_print_string("V");
            break;
        case 0x30:
            vga_print_string("B");
            break;
        case 0x31:
            vga_print_string("N");
            break;
        case 0x32:
            vga_print_string("M");
            break;
        case 0x33:
            vga_print_string(",");
            break;
        case 0x34:
            vga_print_string(".");
            break;
        case 0x35:
            vga_print_string("/");
            break;
        case 0x36:
            vga_print_string("Rshift");
            break;
        case 0x37:
            vga_print_string("Keypad *");
            break;
        case 0x38:
            vga_print_string("LAlt");
            break;
        case 0x39:
            vga_print_string(" ");
            break;
        default:
            /* 'keuyp' event corresponds to the 'keydown' + 0x80
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            if (scancode <= 0x7f) {
                vga_print_string("Unknown key down");
            } else if (scancode <= 0x39 + 0x80) {
                //vga_print_string("key up ");
                //print_letter(scancode - 0x80);
            } else vga_print_string("Unknown key up");
            break;
    }
}

void keyboard_callback(Registers *regs) {
    uint8_t scancode = InPort(0x60);
    print_letter(scancode);

}

int kmain()
{
	vga_clear_screen();
    vga_print_string("Installing interrupt service routines (ISRs).\n");

	isr_install();

	vga_print_string("Enabling external interrupts.\n");
    asm volatile("sti");

	register_interrupt_handler(33, keyboard_callback);
	
 	return 0;
}