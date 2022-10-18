#include "../inttypes.h"

unsigned char InPort(unsigned short port);
void OutPort(unsigned short port, unsigned char data);

/**
 * Wait a very small amount of time (1 to 4 microseconds, generally). 
 * Useful for implementing a small delay for PIC remapping on old hardware or generally as a simple but imprecise wait. 
 **/
void IOWait();