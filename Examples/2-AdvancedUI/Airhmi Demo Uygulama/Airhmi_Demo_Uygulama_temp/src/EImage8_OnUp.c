#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stk.h"

int oto_manu = 0;
GlobalIntVarSet("oto_manu" , oto_manu);

ImageSet("EImage7" , "Visible" , "1");
ImageSet("EImage8" , "Visible" , "0");

printf("<ROLE><R4>0</R4></ROLE>\n");

DrawScreen();