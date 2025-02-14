#include "stk.h"
#include "stdlib.h"

char data[200];
SliderGet("Slider1" ,"Value" , data );
LabelSets("ELabel2" , data );
BacklightSet(atoi(data) ); 


