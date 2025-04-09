#include "stk.h"
#include "stdio.h"
#include "string.h"

char uartData[3000]; 
int uartsize; 
int i = 1;

uartDataGet(uartData , &uartsize); 

if( uartsize > 0 )
{
    
    ButtonSet ("EButton2" , "Caption" , uartData);  
    
    if( uartsize > 2 && ( uartData[uartsize - 1] == '#' ) && (uartData[uartsize - 2] == '#') )
    {
        char s[2];
        char *token;

        s[0] = ',';
        s[1] = 0;
        /* get the first token */
        token = strtok(uartData, s);

        /* walk through other tokens */
        while( token != NULL ) {
            printf( "\n%d. Veri : %s\n",i++, token );

            token = strtok(NULL, s);
        }
  
    }
}

