#include "stk.h"
#include "stdio.h"

char buffer[200];

short Registers[4];


ReadHoldingRegisters(1, 0x4000, 1 , Registers); 

sprintf(buffer,"%d",Registers[0]);

LabelSets("ELabelBox19" , buffer );



ReadHoldingRegisters(1, 0x8000, 1 , Registers); 

sprintf(buffer,"%d",Registers[0]);

LabelSets("ELabelBox20" , buffer );