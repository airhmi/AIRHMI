#include "stk.h"
#include "stdio.h"


char text[100];
char data[100];

ButtonGet("EButton4" , "Text" ,  text);

// KeypadAlphaExt(char *inData, char *outData , int timeout , int maxCharacter )
KeypadAlphaExt(text, data , 30000, 30 , "Operatör Adi" );

ButtonSet("EButton4" ,"Text", data);