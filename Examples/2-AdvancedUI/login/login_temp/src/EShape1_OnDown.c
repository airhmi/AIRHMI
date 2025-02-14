#include "stk.h"
#include "stdio.h"

char UserName[100];


KeypadAlpha("",UserName);

LabelSet("ELabel1","Text",UserName);

VarSet("vUserName",UserName);
