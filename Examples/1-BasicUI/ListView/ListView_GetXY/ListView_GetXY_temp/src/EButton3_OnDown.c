#include "stk.h"

char data[20];

ListViewGetXY("ListView1" ,0 , 0 , data);
LabelSet("ELabelBox5" ,"Text" , data );

ListViewGetXY("ListView1" ,0, 1 , data);
LabelSet("ELabelBox6" ,"Text" , data );

ListViewGetXY("ListView1" ,0 , 2 , data);
LabelSet("ELabelBox7" ,"Text" , data );

ListViewGetXY("ListView1" ,0 , 3 , data);
LabelSet("ELabelBox8" ,"Text" , data );
