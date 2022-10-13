#include "driver.h"

// Todo: move this to another file like memory.h or something like this
#define LOW16(address) (uint16_t)((address) & 0xFFFF)
#define HIGH16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

static IDTGate s_IDT[256];

unsigned char in_port(unsigned short port)
{
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void out_port(unsigned short port, unsigned char data)
{
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

void set_idt_gate(int n, uint32_t handler) 
{
    s_IDT[n].LowOffset = LOW16(handler);
    s_IDT[n].Selector = 0x08; // see GDT
    s_IDT[n].Zero = 0;
    // 0x8E = 1  00 0 1  110
    //        P DPL 0 D Type
    s_IDT[n].Flags = 0x8E;
    s_IDT[n].HighOffset = HIGH16(handler);
}