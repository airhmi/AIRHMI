#include "stk.h"

int s = Video_StatusGet();

if( s )
{
    LabelSets("ELabel2" , "Playing...");
}
else
{
    LabelSets("ELabel2" , "");
}

