#include "stk.h"
#include "stdio.h"
#include "stdlib.h"


int ret = WriteSingleRegister(0,0x8000,1);

printf("Ret:%d \n",ret);

if(ret == 1 )
{
    LabelSet("ELabelBox22" ,"Visible" , "1" );
    TimerSet("ETimer6" ,"Enable" , "True");
}







