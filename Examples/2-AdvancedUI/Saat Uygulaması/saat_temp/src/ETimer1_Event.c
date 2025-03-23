#include "stdio.h"
#include "stdlib.h"
#include "stk.h"
#include "string.h"

unsigned char day , month , year , hour , min ;
char pos[25];
double saat, dakika;
int saniye;
double san;
GlobalIntVarGet("saniye", &saniye);
 char sa[30], dak[30];


dateGet(&day , &month , &year);
timeGet(&hour , &min);

memset(pos , 0x00 , sizeof(pos));
sprintf(pos , "%02d.%02d.20%02d" , day , month , year);

LabelSet("ELabel1" , "Caption" , pos);

memset(pos , 0x00 , sizeof(pos));
sprintf(pos , "%02d:%02d" , hour , min);

LabelSet("ELabel2" , "Caption" , pos);

memset(sa , 0x00 , sizeof(sa));
memset(dak , 0x00 , sizeof(dak));
sprintf(sa, "%d", hour);
sprintf(dak, "%d", min);
saat   = atof(sa);
dakika = atof(dak);

memset(pos , 0x00 , sizeof(pos));
saat = ((saat * 11)+50);
sprintf(pos , "%.01f" , saat);
GaugeSet("Akrep", "Value", pos);

memset(pos , 0x00 , sizeof(pos));
dakika = ((dakika * 2.2)+50);
sprintf(pos , "%.01f" , dakika);
GaugeSet("Yelkovan", "Value", pos );

memset(pos , 0x00 , sizeof(pos));
san = (((double)saniye * 2.2)+50);
sprintf(pos , "%.01f" , san);
GaugeSet("Saniye", "Value", pos );

saniye++;
GlobalIntVarSet("saniye", saniye);
