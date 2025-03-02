#include "stk.h"
#include "stdio.h"

void YikamaSureci() {
    int asama;
    int sure;
    char zamanStr[10];

    // Aşama ve süre değişkenlerini oku
    VarGet("VPAshama", &asama);
    VarGet("VPYikamaSuresi", &sure);

    switch(asama) {
        case 0:
        LabelSet("ELabelStatus", "Text", "Su Aliniyor...");
            sure -= 5; // 5 saniye azalt
            WriteSingleCoil(1, 0x0000, 0xFF00); // Su giriş valfini aç
            Delay(5000);
            WriteSingleCoil(1, 0x0000, 0x0000); // Su giriş valfini kapat
            asama++; // Bir sonraki aşamaya geç
            break;

        case 1:
            LabelSet("ELabelStatus", "Text", "Yikama Yapiliyor...");
            sure -= 10; // 10 saniye azalt
            WriteSingleCoil(1, 0x0002, 0xFF00); // Motoru çalıştır
            Delay(10000);
            WriteSingleCoil(1, 0x0002, 0x0000); // Motoru kapat
            asama++;
            break;

        case 2:
            LabelSet("ELabelStatus", "Text", "Durulama Yapiliyor...");
            sure -= 5; // 5 saniye azalt
            WriteSingleCoil(1, 0x0001, 0xFF00); // Su tahliye valfini aç
            Delay(5000);
            WriteSingleCoil(1, 0x0001, 0x0000); // Su tahliye valfini kapat
            asama++;
            break;

        case 3:
            LabelSet("ELabelStatus", "Text", "Sikma Yapiliyor...");
            sure -= 8; // 8 saniye azalt
            WriteSingleCoil(1, 0x0003, 0xFF00); // Sıkma motorunu aç
            Delay(8000);
            WriteSingleCoil(1, 0x0003, 0x0000); // Motoru kapat
            asama++;
            break;

        case 4:
            LabelSet("ELabelStatus", "Text", "Yikama Tamamlandi!");
            sure = 0; // Süreyi sıfırla
            TimerSet("TimerYikama", "Enable", "False"); // Timer'ı durdur
            asama = 0; // Aşamayı sıfırla
            break;
    }

    // Süreyi ekrana yazdır
    sprintf(zamanStr, "00:%02d", sure);
    LabelSet("ELabelZaman", "Text", zamanStr);

    // Güncellenmiş aşama ve süreyi değişkenlere kaydet
    VarSeti("VPAshama", asama);
    VarSeti("VPYikamaSuresi", sure);
}


YikamaSureci();
