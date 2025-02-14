#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stk.h"

char deger[4];
int score;
LocalIntVarGet("score" , &score);

score += 50;
LocalIntVarSet("score" , score);

memset(deger , 0x00 , sizeof(deger));
sprintf(deger , "%d" , score);

LabelSet("ELabel9" , "Caption" , deger);

ImageSet("EImage14" , "Visible" , "1");

DrawScreen();

BuzzerSet(50);

