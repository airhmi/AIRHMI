#include "stk.h"

int s = Video_StatusGet();

if( s )
{
    LabelSets("ELabel1" , "Playing...");
}
else
{
    LabelSets("ELabel1" , "");
}

