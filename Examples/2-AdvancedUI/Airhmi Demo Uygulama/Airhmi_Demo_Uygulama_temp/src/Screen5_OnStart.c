#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stk.h"

char deger[25];
int fa_deger , choose_FA;
double label_value;

GlobalIntVarGet("Genlik" , &fa_deger);


memset(deger , 0x00 , sizeof(deger));
label_value = fa_deger;
sprintf(deger , "%.1f V" , label_value / 2);
LabelSet("ELabel3" , "Caption" , deger);


DrawScreen();


