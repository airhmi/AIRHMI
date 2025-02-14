#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stk.h"

int oto_manu = 1;
GlobalIntVarSet("oto_manu" , oto_manu);

ImageSet("EImage7" , "Visible" , "0");
ImageSet("EImage8" , "Visible" , "1");

printf("<ROLE><R4>1</R4></ROLE>\n");

DrawScreen();