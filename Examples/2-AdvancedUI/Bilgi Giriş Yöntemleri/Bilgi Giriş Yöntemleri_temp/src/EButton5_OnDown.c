#include "stk.h"
#include "stdio.h"


char text[100];
char data[100];

ButtonGet("EButton5" , "Text" ,  text);

// KeypadAlphaExt(char *inData, char *outData , int timeout , int maxCharacter )
KeypadAlphaExt(text, data , 30000, 10 , "Ürün Kodu" );

ButtonSet("EButton5" ,"Text", data);