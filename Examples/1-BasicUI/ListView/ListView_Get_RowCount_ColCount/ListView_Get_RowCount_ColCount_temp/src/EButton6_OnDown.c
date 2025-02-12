#include "stk.h"

char data[20];

ListViewGet("ListView1" ,"ROWCOUNT", data);
LabelSet("ELabelBox9" ,"Text" , data );

ListViewGet("ListView1" ,"COLCOUNT", data);
LabelSet("ELabelBox10" ,"Text" , data );


