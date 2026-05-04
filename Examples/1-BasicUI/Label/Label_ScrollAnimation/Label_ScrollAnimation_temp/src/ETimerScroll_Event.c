#include "stk.h"
#include "stdio.h"


int kat;
int yon;
VarGet("kat", &kat);
VarGet("yon", &yon);

if( yon == 0 )
{
    kat++;
    if( kat >= 20 ) yon = 1;
}
else
{
    kat--;
    if( kat <= -5 ) yon = 0;
}

VarSet("kat", &kat);
VarSet("yon", &yon);

char buf[10];
sprintf(buf, "%d", kat);
LabelSet("L_Sayi", "Text", buf);
