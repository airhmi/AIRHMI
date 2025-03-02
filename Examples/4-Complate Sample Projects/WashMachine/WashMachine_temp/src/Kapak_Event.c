#include "stk.h"

int kapakDurumu; // Kapak durumunu saklamak için değişken tanımlanıyor.

// SmartIO Modbus'un 1. dijital girişini (0x0001) okuyarak kapak durumunu alıyoruz.
// Eğer giriş değeri 1 ise kapak açıktır, 0 ise kapalıdır.
kapakDurumu = ReadCoils(1, 0x0001, 1);

if(kapakDurumu & 0x01) { 
    // Eğer kapak açık (1) ise ekrana "Acik" yazdır
    LabelSet("ELabelBox5", "Text", "Acik");
} else {
    // Eğer kapak kapalı (0) ise ekrana "Kapali" yazdır
    LabelSet("ELabelBox5", "Text", "Kapali");
}


// Kapak durumunu AirHMI değişkenine kaydediyoruz (VPKapakDurumu değişkenine atama yapılıyor).
VarSeti("VPKapakDurumu", kapakDurumu);
