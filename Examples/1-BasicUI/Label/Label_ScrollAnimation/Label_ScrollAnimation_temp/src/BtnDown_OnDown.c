#include "stk.h"

int kat;
VarGet("kat", &kat);
kat--;
VarSet("kat", &kat);

char buf[10];
sprintf(buf, "%d", kat);
LabelSet("L_Sayi", "Text", buf);
