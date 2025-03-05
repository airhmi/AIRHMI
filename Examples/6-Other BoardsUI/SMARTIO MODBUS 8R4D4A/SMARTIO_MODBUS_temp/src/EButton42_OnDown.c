#include "stk.h"


int baud    = 115200;
int setBaud = baud / 100; 

WriteSingleRegister(0,0x2000,setBaud);

