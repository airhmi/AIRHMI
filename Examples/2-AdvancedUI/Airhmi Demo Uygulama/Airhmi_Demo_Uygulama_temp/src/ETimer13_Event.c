#include "stk.h"
#include "math.h"
#include "stdio.h"

int Angle;
LocalIntVarGet("Angle" , &Angle );
Angle++;
LocalIntVarSet("Angle" , Angle );
int val ;

val = Angle % 100;

double Value;
double radian;
radian = ((Angle * 4)  % 360 ) * (3.14 / 180.0);

Value = sin(radian) * 20;

int i = 0;
char graphName[100];

for(i=1;i<5;i++)
{
    sprintf(graphName,"Graph%d",i);
    
    
    GRAPH_AddValue(graphName,0, 130 + Value);
    
    
    // testere
    GRAPH_AddValue(graphName,2, 75 + Angle % 25);
    
    
    // triangle
    if( (Angle % 50) < 25 )
    {
        GRAPH_AddValue(graphName,1,40 + Angle % 25);
    }
    else
    {
        GRAPH_AddValue(graphName,1,40 + 25 - Angle % 25);
    }
    

    // kare
    if( (Angle % 50) > 25 ) 
    {
        GRAPH_AddValue(graphName,3,30);
    }
    else
    {
        GRAPH_AddValue(graphName,3,0);
    }
    
    
    

}

