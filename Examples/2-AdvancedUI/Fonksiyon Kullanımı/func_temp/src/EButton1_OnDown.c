#include "stk.h"
#include "stdio.h"

int sonuc = topla(3,2);
char data[100];

sprintf(data,"Toplama Sonucu:%d",sonuc);

LabelSet("ELabel1" ,"Text"  , data);
printf("Toplama sounucu:%d \n",sonuc);
