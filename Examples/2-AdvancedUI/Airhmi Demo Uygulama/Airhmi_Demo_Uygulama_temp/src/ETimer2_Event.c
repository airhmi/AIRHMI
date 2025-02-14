#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stk.h"

int time , left , top;
char deger[4];

LocalIntVarGet("timer" , &time);

ImageSet("EImage14" , "Visible" , "0");

time--;

left = rand()%10;
top  = rand()%10;

if(top < 2)
    top = 2;

left = left * 70;
top  = top * 38;

memset(deger , 0x00 , sizeof(deger));
sprintf(deger , "%d" , left);
ImageSet("EImage14" , "Left" , deger);
ImageSet("EImage13" , "Left" , deger);

memset(deger , 0x00 , sizeof(deger));
sprintf(deger , "%d" , top);
ImageSet("EImage14" , "Top" , deger);
ImageSet("EImage13" , "Top" , deger);


ProgressBarSet("EveProgressBar1" , "BackGround_Color" , "4210752");
    
if(time < 15)
    ProgressBarSet("EveProgressBar1" , "Color" , "16711680");
else
    ProgressBarSet("EveProgressBar1" , "Color" , "32768");

if(time < 0)
{
    ProgressBarSet("EveProgressBar1" , "BackGround_Color" , "16711680");
    ImageSet("EImage21" , "Visible" , "1");
    ImageSet("EImage13" , "Visible" , "0");
    time = 0;
}

LocalIntVarSet("timer" , time);

memset(deger , 0x00 , sizeof(deger));
sprintf(deger , "%d" , time*2);

ProgressBarSet("EveProgressBar1" , "Value" , deger);

DrawScreen();