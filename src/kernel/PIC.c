#include "PIC.h"

#include "drivers/driver.h"
#include "drivers/vga.h"

IDTGate IDT[IDT_ENTRIES];
IDTRegister IDTReg;

isr_t InterruptHandlers[256];

static char* ExceptionMessages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void IRQ_Handler(uint8_t irq)
{
    vga_print_string(ExceptionMessages[irq]);
    vga_print_string("\n");
    PIC_EndOfInterrupt(irq);
}

void PIC_Remap(int offset)
{
	unsigned char a1, a2;
 
	a1 = InPort(PIC1_DATA);                        // save masks
	a2 = InPort(PIC2_DATA);
 
	OutPort(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	IOWait();
	OutPort(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	IOWait();
	OutPort(PIC1_DATA, offset);                 // ICW2: Master PIC vector offset
	IOWait();
	OutPort(PIC2_DATA, offset + 8);                 // ICW2: Slave PIC vector offset
	IOWait();
	OutPort(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	IOWait();
	OutPort(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	IOWait();
 
	OutPort(PIC1_DATA, ICW4_8086);
	IOWait();
	OutPort(PIC2_DATA, ICW4_8086);
	IOWait();
 
	OutPort(PIC1_DATA, a1);   // restore saved masks.
	OutPort(PIC2_DATA, a2);
}

void PIC_EndOfInterrupt(uint8_t irq)
{
    if(irq >= 8)
		OutPort(PIC2_COMMAND, PIC_EOI);
 
	OutPort(PIC1_COMMAND, PIC_EOI);
}

void IRQ_Mask(unsigned short mask)
{
    unsigned char pic1Mask = mask & 0xff;
    unsigned char pic2Mask = (mask >> 8) & 0xff;

    OutPort(PIC1_DATA, pic1Mask);
    OutPort(PIC2_DATA, pic2Mask);
}

void IRQ_SetMaskLine(uint8_t irqLine)
{
    uint16_t port;
 
    if(irqLine < 8)
        port = PIC1_DATA;
    else
    {
        port = PIC2_DATA;
        irqLine -= 8;
    }

    uint8_t value = InPort(port) | (1 << irqLine);
    OutPort(port, value);
}
 
void IRQ_ClearMaskLine(uint8_t irqLine) 
{
    uint16_t port;
 
    if(irqLine < 8)
        port = PIC1_DATA;
    else 
    {
        port = PIC2_DATA;
        irqLine -= 8;
    }

    uint8_t value = InPort(port) & ~(1 << irqLine);
    OutPort(port, value);
}

void LoadIDT()
{
    IDTReg.Base = (uint32_t) &IDT;
    IDTReg.Limit = IDT_ENTRIES * sizeof(IDTGate) - 1;
    /* Don't make the mistake of loading &idt -- always load &idt_reg */
    asm volatile("lidt (%0)" : : "r" (&IDTReg));
}

void SetIDTGateHandler(uint32_t gate, uint32_t handler)
{
    IDT[gate].LowOffset = handler & 0xffff;
    IDT[gate].Selector = 0x08; // GDT kernel code segment
    IDT[gate].Zero = 0;

    // 0x8E = 1  00 0 1  110
    //        P DPL 0 D Type
    IDT[gate].Flags = 0x8E;

    IDT[gate].HighOffset = (handler >> 16) & 0xffff;
}

void isr_handler(Registers *r) 
{
    vga_print_string("received interrupt: ");
    char s[3];
    int_to_string(r->IntNum, s);
    vga_print_string(s);
    vga_print_string("\n");
    vga_print_string(ExceptionMessages[r->IntNum]);
    vga_print_string("\n");
}

int string_length(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = string_length(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void int_to_string(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

void irq_handler(Registers *r) 
{
    /* Handle the interrupt in a more modular way */
    if (InterruptHandlers[r->IntNum] != 0) {
        isr_t handler = InterruptHandlers[r->IntNum];
        handler(r);
    }

    // EOI
    if (r->IntNum >= 40) {
        OutPort(0xA0, 0x20); /* follower */
    }
    OutPort(0x20, 0x20); /* leader */
}

void register_interrupt_handler(uint8_t n, isr_t handler) 
{
    InterruptHandlers[n] = handler;
}