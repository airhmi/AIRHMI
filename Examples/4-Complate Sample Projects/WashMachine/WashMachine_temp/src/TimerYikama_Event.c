#include "stk.h"
#include "stdio.h"

void YikamaSureci() {
    int asama;
    
    // Variable Get ile aşamayı oku
    VarGet("VPAshama", &asama);

    switch(asama) {
        case 0:
            LabelSet("ELabelStatus", "Text", "Su Alınıyor...");
            WriteSingleCoil(1, 0x0000, 0xFF00); // Su giriş valfini aç
            Delay(5000);
            WriteSingleCoil(1, 0x0000, 0x0000); // Su giriş valfini kapat
            asama++; // Bir sonraki aşamaya geç
            break;

        case 1:
            LabelSet("ELabelStatus", "Text", "Yıkama Yapılıyor...");
            WriteSingleCoil(1, 0x0002, 0xFF00); // Motoru çalıştır
            Delay(10000);
            WriteSingleCoil(1, 0x0002, 0x0000); // Motoru kapat
            asama++;
            break;

        case 2:
            LabelSet("ELabelStatus", "Text", "Durulama Yapılıyor...");
            WriteSingleCoil(1, 0x0001, 0xFF00); // Su tahliye valfini aç
            Delay(5000);
            WriteSingleCoil(1, 0x0001, 0x0000); // Su tahliye valfini kapat
            asama++;
            break;

        case 3:
            LabelSet("ELabelStatus", "Text", "Sıkma Yapılıyor...");
            WriteSingleCoil(1, 0x0003, 0xFF00); // Sıkma motorunu aç
            Delay(8000);
            WriteSingleCoil(1, 0x0003, 0x0000); // Motoru kapat
            asama++;
            break;

        case 4:
            LabelSet("ELabelStatus", "Text", "Yıkama Tamamlandı!");
            TimerSet("TimerYikama", "Enable", "0"); // Timer'ı durdur
            asama = 0; // Aşamayı sıfırla
            break;
    }

    // Güncellenmiş aşamayı variable'a kaydet
    VarSeti("VPAshama", asama);
}


printf("Timer bsaliyor.");
YikamaSureci();


