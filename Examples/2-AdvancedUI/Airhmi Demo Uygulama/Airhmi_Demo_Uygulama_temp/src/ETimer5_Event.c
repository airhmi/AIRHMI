#include "stdio.h"
#include "stk.h"

int progresVal;
LocalIntVarGet("progresVal" , &progresVal);
progresVal++;
progresVal = progresVal % 100;
LocalIntVarSet("progresVal" , progresVal);

char val[20];
sprintf(val,"%d",progresVal);
ProgressBarSet("ProgressBar1" , "Value" , val);
sprintf(val,"%d",100 - progresVal);
ProgressBarSet("ProgressBar2" , "Value" , val);
