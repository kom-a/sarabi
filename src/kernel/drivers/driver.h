#include "../inttypes.h"

unsigned char in_port(unsigned short port);
void out_port(unsigned short port, unsigned char data);

typedef struct 
{
    uint16_t LowOffset;
    uint16_t Selector;
    uint8_t Zero;
    uint8_t Flags;
    uint16_t HighOffset;
} __attribute__((packed)) IDTGate;

void set_idt_gate(int n, uint32_t handler);