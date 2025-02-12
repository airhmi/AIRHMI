#include "stk.h"

char data[20];

ListViewGet("ListView1" ,"ROWCOUNT", data);
LabelSet("ELabelBox5" ,"Text" , data );

ListViewGet("ListView1" ,"COLCOUNT", data);
LabelSet("ELabelBox7" ,"Text" , data );


