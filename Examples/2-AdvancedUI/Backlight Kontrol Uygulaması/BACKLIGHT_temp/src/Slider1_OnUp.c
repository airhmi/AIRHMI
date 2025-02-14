#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stk.h"


char data[20];
SliderGet("Slider1" , "Value" , data);

printf("data:%s\n",data);

BacklightSet(atoi(data));

char yeniData[20];
sprintf(yeniData, "%%%s", data);
LabelSets("ELabel2" , yeniData );

