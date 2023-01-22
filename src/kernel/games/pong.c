#include "pong.h"

#include "../drivers/vga.h"

void Setup()
{
    // VgaNoDecoration();
    VgaClear();
}

void PongGameLoop()
{
    Setup();

    while(1)
    {
        VgaPrint("PONG ");

        for(int i = 0; i < 10000000; )
        {
            i++;
        }
    }
}