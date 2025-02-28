#include "stk.h"


int kapakDurumu;
VarGet("VPKapakDurumu", &kapakDurumu);

if(kapakDurumu == 0) { // Kapak kapalıysa başlat
    LabelSet("ELabelStatus", "Text", "Yikama Baslatiliyor...");
    WriteSingleCoil(1, 0x0002, 0xFF00);  // Motoru çalıştır
    TimerSet("TimerYikama", "Enable", "1");
} else {
    LabelSet("ELabelStatus", "Text", "Kapak Acik! Kapatiniz.");
}