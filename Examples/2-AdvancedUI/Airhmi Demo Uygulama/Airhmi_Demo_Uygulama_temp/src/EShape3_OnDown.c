#include "stk.h"
#include "stdio.h"

int s = 0;
AudioStatusGet(&s);

if( s == 0 )
{
   
    int SoundFile;
    LocalIntVarGet("SoundFile" , &SoundFile);
    SoundFile--;
    if( SoundFile < 1 )
        SoundFile = 1;
    LocalIntVarSet("SoundFile" , SoundFile);
    
    char data[200];
    
    sprintf(data,"Music%d.wav",SoundFile);
    LabelSet("MusicName" , "Text" , data);
    
    ImageSet("prew" , "Visible", "True");
    

}
 