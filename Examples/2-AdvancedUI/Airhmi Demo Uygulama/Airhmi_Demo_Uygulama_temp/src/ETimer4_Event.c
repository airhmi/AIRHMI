#include "stdio.h"
#include "stk.h"

char val[20];
SliderGet("Slider8","Value",val);
LabelSet("sliderText1" ,"Caption"  , val);
