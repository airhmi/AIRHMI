#include "stk.h"

int kapakDurumu;

// 1. girişin değerini oku (Kapak Açık: 1, Kapak Kapalı: 0)
kapakDurumu = ReadCoils(1, 0x0001, 1);

if(kapakDurumu & 0x01) {
    LabelSet("ELabelBox5", "Text", "Acik");
} else {
    LabelSet("ELabelBox5", "Text", "Kapali");
}


// Kapak durumunu değişkene ata
VarSeti("VPKapakDurumu", kapakDurumu);
    
    