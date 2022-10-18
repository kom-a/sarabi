#include "PIC.h"

#include "drivers/driver.h"

static IDTGate s_IDT[IDT_ENTRIES];

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
    struct {
        uint16_t Length;
        void*    Base;
    } __attribute__((packed)) IDTR = { sizeof(IDTGate) * IDT_ENTRIES - 1, s_IDT };
 
    asm ( "lidt %0" : : "m"(IDTR) );
}

void SetIDTGateHandler(uint32_t gate, void (*handler)(void))
{
    uint32_t address = (uint32_t)handler;

    s_IDT[gate].LowOffset = address & 0xffff;
    s_IDT[gate].Selector = 0x08; // GDT kernel code segment
    s_IDT[gate].Zero = 0;

    // 0x8E = 1  00 0 1  110
    //        P DPL 0 D Type
    s_IDT[gate].Flags = 0x8E;

    s_IDT[gate].HighOffset = (address >> 16) & 0xffff;
}