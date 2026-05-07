
int i;
VarGet("count", &i);
char data[20];
sprintf(data,"%d",i);
LabelSet("lCounter" , "Text", data);
i++;
VarSeti("count" , i);
