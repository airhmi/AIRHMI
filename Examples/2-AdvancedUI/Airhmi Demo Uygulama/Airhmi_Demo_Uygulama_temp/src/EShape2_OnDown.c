#include "stk.h"
#include "stdio.h"


ImageSet("EImage28" , "Visible",  "True");
ShapeSet("EShape2","Active","0");
ImageSet("EImage29" ,"Visible" ,"True" );

char data[200];
    
LabelGet("MusicName" , "Text" , data);    

    
AudioPlay(data, 100);

Delay(100);

TimerSet("ETimer7","Enable" ,"1");

