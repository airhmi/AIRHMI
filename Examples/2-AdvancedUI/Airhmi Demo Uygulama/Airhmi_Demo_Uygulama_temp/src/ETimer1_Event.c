#include "stk.h"
#include "stdio.h"
#include "stdlib.h"



int r = rand();      
char buf[100];


sprintf(buf,"%d",r%600);
//printf("Left:%s - ",buf);
ImageSet ("EImage10" , "Left" , buf);

r = rand();
sprintf(buf,"%d",r%350);
//printf("Top:%s\n",buf);
ImageSet ("EImage10" , "Top" , buf);

r = rand();
sprintf(buf,"%d",r%180);
ImageSet ("EImage10" , " RotationAngle" , buf);

//DrawScreen();


