#include "stk.h"
#include "stdio.h"

int mTicks;

LocalIntVarGet("mTicks" , &mTicks);

mTicks++;

LocalIntVarSet("mTicks" , mTicks);


char data[200];
sprintf(data,"%02d:%02d",mTicks / 60 , mTicks % 60);

LabelSet("ELabel24" , "Text" , data);

int s = 0;
AudioStatusGet(&s);

if( s == 0 )
{
    TimerSet("ETimer7","Enable" ,"0");
    LabelSet("ELabel24" , "Text" , "00:00");
    ImageSet("EImage28" , "Visible",  "False");
    ShapeSet("EShape2","Active","1");  
    ImageSet("EImage29" ,"Visible" ,"False" );
    LocalIntVarSet("mTicks" , 0);
}

printf("Timer ticks:%d \n",mTicks);

