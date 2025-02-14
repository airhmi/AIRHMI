#include "stk.h"
#include "stdio.h"

int val;
LocalIntVarGet("val" , &val);
val += 3;
val = val % 100;
LocalIntVarSet("val" , val);


char data[10];
sprintf(data,"%d",val);

GaugeSet("Gauge1","Value" ,data );