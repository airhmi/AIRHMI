#include "stk.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

char tmp[2];
VarGet("vRemember",tmp);

if(strcmp(tmp,"0") == 0 )
{
    ImageSet("ImageChecked" ,"Visible" ,"True" );
    ImageSet("ImageUnchecked" ,"Visible" ,"False" );
    
    tmp[0] = '1';
    VarSet("vRemember",tmp);
    
    ListViewSetXY("ListView1",2,2,"1");
    ListViewSet("ListView1","update",0);
}
else
{
    ImageSet("ImageChecked" ,"Visible" ,"False" );
    ImageSet("ImageUnchecked" ,"Visible" ,"True" ); 
    tmp[0] = '0';
    VarSet("vRemember",tmp);
}

DrawScreen();

VariableSave("vRemember");