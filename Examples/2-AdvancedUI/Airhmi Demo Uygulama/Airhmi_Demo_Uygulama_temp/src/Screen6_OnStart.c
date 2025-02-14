#include "stdio.h"
#include "stdlib.h"
#include "stk.h"
#include "string.h"

unsigned char day , month , year , hour , min ;
char pos[25];

dateGet(&day , &month , &year);
timeGet(&hour , &min);

memset(pos , 0x00 , sizeof(pos));
sprintf(pos , "%02d:%02d  %02d.%02d.20%02d" , hour , min , day , month , year);


LabelSet("ELabel25" , "Caption" , pos);

