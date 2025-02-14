#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stk.h"

int oto_manu;
GlobalIntVarGet("oto_manu" , &oto_manu);

if ( oto_manu )
{
    ImageSet("EImage7" , "Visible" , "0");
    ImageSet("EImage8" , "Visible" , "1");
}   

else
{
    ImageSet("EImage7" , "Visible" , "1");
    ImageSet("EImage8" , "Visible" , "0");
}

DrawScreen();   