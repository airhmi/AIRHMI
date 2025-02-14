#include "stk.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

char UserName[100];

VarGet("vUserName",UserName);

char Password[100];

VarGet("vPassword",Password);

printf("UserName:%s\n",UserName);
printf("Password:%s\n",Password);
printf("LEN UserName:%d\n",strlen(UserName));
printf("LEN Password:%d\n",strlen(Password));

char dbUserName[100];
char dbPassword[100];

memset(dbUserName,0,sizeof(dbUserName));
memset(dbPassword,0,sizeof(dbPassword));

ListViewGetXY("ListView1",1,0,dbUserName);
ListViewGetXY("ListView1",2,0,dbPassword);

printf("dbUserName:%s\n",dbUserName);
printf("dbPassword:%s\n",dbPassword);

printf("LEN dbUserName:%d\n",strlen(dbUserName));
printf("LEN dbPassword:%d\n",strlen(dbPassword));

if( ( strcmp( Password , dbPassword ) == 0 ) )
{   
    ChangeScreenSet("Screen3");
}
else
{
    int i;
    char Message[100];
    
    for( i = 0 ; i < 3 ; i++)
    {
        char t_e[2];
        
        ListViewGetXY("ListView1",2,1,t_e);
        
        ListViewGetXY("ListView2",atoi(t_e),0,Message);
        printf("Message:%s\n",Message);
        
        LabelSet("ELabel6" , "Text" , Message );
        
        LabelSet("ELabel6" ,"Visible"  ,"True" );
        DrawScreen();
        Delay(300);
        LabelSet("ELabel6" ,"Visible"  ,"False" );
        DrawScreen();
        Delay(300);        
    }
    
}

