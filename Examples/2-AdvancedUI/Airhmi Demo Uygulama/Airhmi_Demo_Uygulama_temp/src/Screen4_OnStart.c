#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stk.h"

char deger[25];
int fa_deger , W_Freq;

GlobalIntVarGet("Frekans" , &fa_deger);


memset(deger , 0x00 , sizeof(deger));
sprintf(deger , "%d Hz" , fa_deger);
LabelSet("ELabel4" , "Caption" , deger);

GlobalIntVarGet("W_Freq" , &W_Freq);

switch(W_Freq)
{
    case 1 : ImageSet("EImage36" , "Visible" , "1"); printf("<ROLE><R1>1</R1><R2>0</R2><R3>0</R3></ROLE>\n"); break;
    case 2 : ImageSet("EImage37" , "Visible" , "1"); printf("<ROLE><R1>0</R1><R2>1</R2><R3>0</R3></ROLE>\n"); break;
    case 3 : ImageSet("EImage38" , "Visible" , "1"); printf("<ROLE><R1>0</R1><R2>0</R2><R3>1</R3></ROLE>\n"); break;
}


DrawScreen();


