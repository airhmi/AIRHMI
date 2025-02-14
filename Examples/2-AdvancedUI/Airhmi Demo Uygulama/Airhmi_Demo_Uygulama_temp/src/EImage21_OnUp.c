#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stk.h"

int time ,score;

score = 0;
time  = 50;

LocalIntVarSet("score" , score);
LocalIntVarSet("timer" , time);

LabelSet("ELabel9" , "Caption" , "000");

ImageSet("EImage13" , "Visible" , "1");

ImageSet("EImage21" , "Visible" , "0");


DrawScreen();