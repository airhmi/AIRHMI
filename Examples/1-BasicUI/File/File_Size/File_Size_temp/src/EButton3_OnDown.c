#include "stk.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

char buffer[100];


LabelGet("ELabelBox5" , "Text" , buffer );
File_write("EFile1_fileName.txt",buffer,strlen(buffer),1);
