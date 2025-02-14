#include "stdio.h"
#include "stdlib.h"
#include "stk.h"
#include "string.h"

unsigned char day , month , year , hour , min ;

day   = 1;
month = 1;
year  = 23;
hour  = 15;
min   = 10;
dateSet(&day ,&month , &year);
timeSet(&hour ,&min);

