#include "drivers/vga.h"
#include "inttypes.h"
#include "PIC.h"
#include "clib/string.h"

#define COMMAND_BUFFER_SIZE 60
char s_Command[COMMAND_BUFFER_SIZE] = { 0 };

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

char ScancodeToASCII(uint8_t scancode)
{
    switch (scancode) 
    {
        case 0x00: return '?'; // Error
        case 0x01: return '?'; // Escape
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0A: return '9';
        case 0x0B: return '0';
        case 0x0C: return '-';
        case 0x0D: return '+';
        case 0x0E: return '\b'; // Backspace
        case 0x0F: return '\t';
        case 0x10: return 'Q';
        case 0x11: return 'W';
        case 0x12: return 'E';
        case 0x13: return 'R';
        case 0x14: return 'T';
        case 0x15: return 'Y';
        case 0x16: return 'U';
        case 0x17: return 'I';
        case 0x18: return 'O';
        case 0x19: return 'P';
        case 0x1A: return '[';
        case 0x1B: return ']';
        case 0x1C: return '\n'; // Enter
        case 0x1D: return '?'; // Left control
        case 0x1E: return 'A';
        case 0x1F: return 'S';
        case 0x20: return 'D';
        case 0x21: return 'F';
        case 0x22: return 'G';
        case 0x23: return 'H';
        case 0x24: return 'J';
        case 0x25: return 'K';
        case 0x26: return 'L';
        case 0x27: return ';';
        case 0x28: return '\'';
        case 0x29: return '`';
        case 0x2A: return '?'; // Left shift
        case 0x2B: return '\\';
        case 0x2C: return 'Z';
        case 0x2D: return 'X';
        case 0x2E: return 'C';
        case 0x2F: return 'V';
        case 0x30: return 'B';
        case 0x31: return 'N';
        case 0x32: return 'M';
        case 0x33: return ',';
        case 0x34: return '.';
        case 0x35: return '/';
        case 0x36: return '?'; // Right shift
        case 0x37: return '*'; // Keypad *
        case 0x38: return '?'; // Left alt
        case 0x39: return ' ';
        default:   return '?';
    }
}

void ProcessCommand()
{
    if(!StringCompare(s_Command, "HELP"))
    {
        VgaPrint("Welcome to Sarabi OS.\nIf you need any help, you can't get it :(\n");
    }
    else if(!StringCompare(s_Command, "CLEAR"))
    {
        VgaClear();
    }
    else if(!StringCompare(s_Command, "LOL"))
    {
        const char* message = "    .-''''-.\n"
                            "   /        \\\n"
                            "  /_        _\\\n"
                            " // \\      / \\\\\n"
                            " |\\__\\    /__/|\n"
                            "  \\    ||    /\n"
                            "   \\        /\n"
                            "    \\  __  / \n"
                            "     '.__.'\n"
                            "      |  |\n"
                            "      |  |\n";

        ColorPair color;
        color.Foreground = VGA_COLOR_GREEN;
        color.Background = VgaGetBackgroundColor();

        VgaPrintColored(message, color);
    }
    else if (!StringCompare(s_Command, "SARABI"))
    {
        ColorPair brown = { VGA_COLOR_BROWN, VgaGetBackgroundColor() };
        ColorPair yellow = { VGA_COLOR_YELLOW, VgaGetBackgroundColor() };
        ColorPair white = { VGA_COLOR_WHITE, VgaGetBackgroundColor() };
        ColorPair red = { VGA_COLOR_LIGHTRED, VgaGetBackgroundColor() };

        VgaPrintColored("       =4c,.\n", brown);

        VgaPrintColored("   ,dP""-`$$$c\n", brown);

        VgaPrintColored(" ,$\" ,!!! $$$$$c,. .,,.\n", brown);

        VgaPrintColored(",P',!!!!!>?$$$$$$$$$$$$$$$$$cc,\n", brown);

        VgaPrintColored("d  <!!!'-  $$$$$$$$$$$$$$$$$$$$$$c, .,,,ccccc,\n", brown);

        VgaPrintColored("4$'`!!!!>  `$P\".,.\"$$$$$$$$$$$$$$$$b,\"$$$$\"  ?c,\n", brown);

        VgaPrintColored(" ?c '!!!:',$\"", brown);
        VgaPrintColored(",$$$$$c", yellow);
        VgaPrintColored("\"$$$$$$$$$$$$$$$$c`?""-!> $F\n", brown);

        VgaPrintColored("  `\"bc,``,$F", brown);
        VgaPrintColored(",$\"\" `\"$b", yellow);
        VgaPrintColored("`$$$$$$$$$$$$$$$P\" <;`!> $'\n", brown);

        VgaPrintColored("      `\" $$ ,\"", brown);
        VgaPrintColored(",??$c", white);
        VgaPrintColored("`$c", yellow);
        VgaPrintColored("~$$$$$$$$$$$$\"", brown);
        VgaPrintColored(",c$b ~ ',d'\n", brown);

        VgaPrintColored("        d$$F", brown);
        VgaPrintColored("3     ", yellow);
        VgaPrintColored("3F", white);
        VgaPrintColored("$$ ", yellow);
        VgaPrintColored("$$$$$$$$$$P  ", brown);
        VgaPrintColored("\"?$ ", white);
        VgaPrintColored("<r=\"\n", brown);

        VgaPrintColored("        $$$$c", brown);
        VgaPrintColored("/.   ", yellow);
        VgaPrintColored(",\"", white);
        VgaPrintColored("$'", yellow);
        VgaPrintColored("J$$$$$$$$$$  ", brown);
        VgaPrintColored("\"?$ ", white);
        VgaPrintColored("<r=\"\n", brown);

        VgaPrintColored("        $$$$$$c   \"`", brown);
        VgaPrintColored("P", yellow);
        VgaPrintColored(",$$$$$$$$$$F    ", brown);
        VgaPrintColored(",\"\n", yellow);
        
        VgaPrintColored("        `$$$$$$$$be..$?$$$$$$$$$b .,,c$\n", brown);

        VgaPrintColored("         `?$$$$$$$$$$\",\"???$$$$$$c3$$$P\n", brown);

        VgaPrintColored("           \"?$$$$$\" ", brown);
        VgaPrintColored("4$$c,", yellow);
        VgaPrintColored("`::.", red);
        VgaPrintColored("\"\"", brown);
        VgaPrintColored(".- ", red);
        VgaPrintColored("4$P\"\n", brown);

        VgaPrintColored("              \"?$$$ ", brown);
        VgaPrintColored("~$$$$$c,", yellow);
        VgaPrintColored("`", red);
        VgaPrintColored(",ccP ", yellow);
        VgaPrintColored("`\n", brown);

        VgaPrintColored("                ", brown);
        VgaPrintColored("`\"\"db,\"\"\"\"`\"`??\"\n", yellow);

        VgaPrintColored("                   $$$$$$$$$$$$'\n", yellow);

        VgaPrintColored("                   \"$$$$$$$$$$\"\n", yellow);

        VgaPrintColored("                      .\"??\"\"\"\n\n", yellow);
    }
    else 
    {
        ColorPair currentColor = VgaGetColor();
        ColorPair messageColor;
        messageColor.Foreground = VGA_COLOR_YELLOW;
        messageColor.Background = currentColor.Background;

        VgaPrintColored("Unknown command '", messageColor);
        VgaPrintColored(s_Command, messageColor);
        VgaPrintColored("'!\n", messageColor);
    }
}

void keyboard_callback(Registers *regs) {
    uint8_t scancode = InPort(0x60);
    char c = ScancodeToASCII(scancode);

    if(c == '\n')
    {
        VgaPrint("\n");

        if(!StringEmpty(s_Command))
        {    
            ProcessCommand();

            s_Command[0] = '\0';
        }

        VgaPrint(">> ");
        
    }
    else if(c == '\b')
    {
        int len = StringLength(s_Command);
        if(len > 0)
        {
            s_Command[len - 1] = '\0';
            VgaBackspace();
        }
            
    }
    else if (scancode <= 0x7f)
    {
        if(StringLength(s_Command) < COMMAND_BUFFER_SIZE - 1)
        {
            StringAppend(s_Command, c);

            char str[2] = { c, '\0' };
            VgaPrint(str);
        }
    }        
}

int kmain()
{   
    VgaClear();
    VgaHeaderMessage("Sarabi OS");
    VgaFooterMessage("This is some useful and helpful information");
    
	PIC_Init(0x20);
    
	register_interrupt_handler(IRQ1, keyboard_callback);

    VgaPrint(">> ");
    asm volatile ("hlt");

 	return 0;
}
