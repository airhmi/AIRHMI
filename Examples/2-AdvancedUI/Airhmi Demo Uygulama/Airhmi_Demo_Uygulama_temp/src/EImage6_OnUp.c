#include "stdio.h"
#include "stk.h"

int oto_manu;
GlobalIntVarGet("oto_manu" , &oto_manu);

if ( oto_manu )
    ChangeScreenSet("Screen5");