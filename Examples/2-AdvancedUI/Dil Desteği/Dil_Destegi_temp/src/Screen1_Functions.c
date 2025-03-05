#include "stk.h"


void LanguageSet()
{
    int language;

    VarGet("language" , &language );


    if( language == 0 ) // TÜrkçe
    {
        LabelSet("ELabelBox1" ,"Text" ,"Sayfa 1");
        LabelSet("ELabelBox2" ,"Text" ,"Ayarlar");
        LabelSet("ELabelBox3" ,"Text" ,"Resetle");
        
        ButtonSet("EButton1" ,"Text" , "Turkce" );
        ButtonSet("EButton2" ,"Text" , "ingilizce" );
    }
    else
    {
        LabelSet("ELabelBox1" ,"Text" ,"Page 1");
        LabelSet("ELabelBox2" ,"Text" ,"Settings");
        LabelSet("ELabelBox3" ,"Text" ,"Reset");    
        
        ButtonSet("EButton1" ,"Text" , "Turkish" );
        ButtonSet("EButton2" ,"Text" , "English" );
    }
}


