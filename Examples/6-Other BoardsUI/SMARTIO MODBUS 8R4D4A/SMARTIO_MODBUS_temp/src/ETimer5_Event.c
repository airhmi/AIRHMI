#include "stk.h"
#include "stdio.h"

char buffer[200];

short Registers[4];

ReadHoldingRegisters(1, 0x0000, 4 , Registers); 

double adc1 = Registers[0] / 100.0;
double adc2 = Registers[1] / 100.0;
double adc3 = Registers[2] / 100.0;
double adc4 = Registers[3] / 100.0;


sprintf(buffer,"%.02f",adc1);

LabelSets("ELabelBox9" , buffer );

sprintf(buffer,"%.02f",adc2);

LabelSets("ELabelBox10" , buffer );

sprintf(buffer,"%.02f",adc3);

LabelSets("ELabelBox11" , buffer );

sprintf(buffer,"%.02f",adc4);

LabelSets("ELabelBox12" , buffer );




int data;

data = ReadCoils(1, 0x0001, 4);

//printf("Data:%x \n",data);

if( data & 0x01 )
    LabelSets("ELabelBox13" , "Close");
else
    LabelSets("ELabelBox13" , "Open");


if( data & 0x02 )
    LabelSets("ELabelBox14" , "Close");
else
    LabelSets("ELabelBox14" , "Open");

if( data & 0x04 )
    LabelSets("ELabelBox15" , "Close");
else
    LabelSets("ELabelBox15" , "Open");

if( data & 0x08 )
    LabelSets("ELabelBox16" , "Close");
else
    LabelSets("ELabelBox16" , "Open");

