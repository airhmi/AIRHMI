#include "stdio.h"
#include "stk.h"


int screenCount;

GlobalIntVarGet("screenCount" , &screenCount);

screenCount++;

GlobalIntVarSet("screenCount" , screenCount);


if( screenCount > 13 )
{
    TimerSet("ETimer3","Enable" ,"0");
    GlobalIntVarSet("screenCount" , 2);
    ChangeScreenSet("Screen1");
}
else
{
    char data[200];
    sprintf(data ,"Screen%d" , screenCount );
    ChangeScreenSet(data);
}

