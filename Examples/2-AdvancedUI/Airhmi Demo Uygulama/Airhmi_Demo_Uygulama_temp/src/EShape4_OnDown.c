#include "stk.h"
#include "stdio.h"

int s = 0;
AudioStatusGet(&s);

if( s == 0 )
{
    
    int SoundFile;
    LocalIntVarGet("SoundFile" , &SoundFile);
    SoundFile++;
    if( SoundFile > 10 )
        SoundFile = 10;
    LocalIntVarSet("SoundFile" , SoundFile);
    
    char data[200];
    
    sprintf(data,"Music%d.wav",SoundFile);
    LabelSet("MusicName" , "Text" , data);    
    
    ImageSet("Next" ,"Visible" , "True");

}
 