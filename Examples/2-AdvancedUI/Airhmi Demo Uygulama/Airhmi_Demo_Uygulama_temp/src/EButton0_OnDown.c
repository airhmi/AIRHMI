#include "stk.h"
#include "stdio.h"

char row[200];

ListViewGet("List1","Row1",row);
LabelSet("ELabel1_9U9","Caption",row);

ListViewGet("List1","Row2",row);
LabelSet("ELabel2_9U9","Caption",row);

ListViewGet("List1","Row3",row);
LabelSet("ELabel3_9U9","Caption",row);
