#include "stk.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

LabelSet("ELabel1" ,"Text"  ,"" );
LabelSet("ELabel2" ,"Text"  ,"" );

char UserName[50];
ListViewGetXY("ListView1",1,0,UserName);
LabelSet("ELabel1" ,"Text"  ,UserName );


char tmp[2];
VarGet("vRemember",tmp);

if(strcmp(tmp,"0") != 0 )
{
    ImageSet("ImageChecked" ,"Visible" ,"True" );
    ImageSet("ImageUnchecked" ,"Visible" ,"False" );
}
else
{
    ImageSet("ImageChecked" ,"Visible" ,"False" );
    ImageSet("ImageUnchecked" ,"Visible" ,"True" ); 
}