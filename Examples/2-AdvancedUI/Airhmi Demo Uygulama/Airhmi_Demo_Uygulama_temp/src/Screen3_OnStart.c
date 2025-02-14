#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stk.h"

int Sin_Ucgen;
GlobalIntVarGet("Sin_Ucgen" , &Sin_Ucgen);

if( Sin_Ucgen == 1 )
{
    ImageSet("EImage12" , "Visible" , "1");
    ImageSet("EImage14" , "Visible" , "1");

    ImageSet("EImage13" , "Visible" , "0");
    ImageSet("EImage15" , "Visible" , "0");
    
    printf("<ROLE><R5>0</R5></ROLE>\n");
}
else
{
    ImageSet("EImage12" , "Visible" , "0");
    ImageSet("EImage14" , "Visible" , "0");

    ImageSet("EImage13" , "Visible" , "1");
    ImageSet("EImage15" , "Visible" , "1");
    
    printf("<ROLE><R5>1</R5></ROLE>\n");
}

DrawScreen();