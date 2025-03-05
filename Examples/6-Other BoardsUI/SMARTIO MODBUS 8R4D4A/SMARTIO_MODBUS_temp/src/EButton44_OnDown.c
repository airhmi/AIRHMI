#include "stk.h"
#include "stdio.h"
#include "stdlib.h"

char data[100];
KeypadNum("" , data );

LabelSets("ELabelBox21",data);



int idata = atoi(data);

WriteSingleRegister(0,0x1000,idata);







