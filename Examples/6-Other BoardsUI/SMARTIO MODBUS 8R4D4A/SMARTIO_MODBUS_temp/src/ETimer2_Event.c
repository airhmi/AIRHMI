#include "stk.h"
#include "stdio.h"

int data;

data = ReadCoils(1, 0x0000, 8);

if( data & 0x01 )
    LabelSets("ELabelBox1" , "Close");
else
    LabelSets("ELabelBox1" , "Open");


if( data & 0x02 )
    LabelSets("ELabelBox2" , "Close");
else
    LabelSets("ELabelBox2" , "Open");

if( data & 0x04 )
    LabelSets("ELabelBox3" , "Close");
else
    LabelSets("ELabelBox3" , "Open");

if( data & 0x08 )
    LabelSets("ELabelBox4" , "Close");
else
    LabelSets("ELabelBox4" , "Open");

if( data & 0x10 )
    LabelSets("ELabelBox5" , "Close");
else
    LabelSets("ELabelBox5" , "Open");

if( data & 0x20 )
    LabelSets("ELabelBox6" , "Close");
else
    LabelSets("ELabelBox6" , "Open");

if( data & 0x40 )
    LabelSets("ELabelBox7" , "Close");
else
    LabelSets("ELabelBox7" , "Open");

if( data & 0x80 )
    LabelSets("ELabelBox8" , "Close");
else
    LabelSets("ELabelBox8" , "Open");

