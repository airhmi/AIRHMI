#include "stk.h"
#include "stdio.h"

char data[50];

int size = File_Size("EFile1_fileName.txt");

sprintf(data,"Size:%d",size);
LabelSet("ELabelBox6" ,"Caption"  , data );

