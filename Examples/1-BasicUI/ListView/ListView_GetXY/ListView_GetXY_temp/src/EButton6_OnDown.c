#include "stk.h"

char data[20];

ListViewGetXY("ListView1" ,0 , 0 , data);
LabelSet("ELabelBox9" ,"Text" , data );

ListViewGetXY("ListView1" ,0, 1 , data);
LabelSet("ELabelBox10" ,"Text" , data );

ListViewGetXY("ListView1" ,0 , 2 , data);
LabelSet("ELabelBox11" ,"Text" , data );

ListViewGetXY("ListView1" ,0 , 3 , data);
LabelSet("ELabelBox12" ,"Text" , data );
