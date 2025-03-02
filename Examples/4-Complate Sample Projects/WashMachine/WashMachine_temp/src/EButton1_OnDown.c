#include "stk.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

// Kapak durumunu kontrol etmek için değişken tanımlıyoruz.
int kapakDurumu;

// Daha önce saklanan kapak durumunu değişkene alıyoruz.
// Eğer kapak kapalıysa (1), yıkama başlatılabilir.
VarGet("VPKapakDurumu", &kapakDurumu);

if(kapakDurumu == 1) { // Kapak kapalıysa işlemi başlat

    char seciliProgram[20]; // Seçili programı saklamak için karakter dizisi tanımlanıyor.

    // Kullanıcının seçtiği yıkama programını list wheel'den alıyoruz.
    ListWheelGet("ListWheel1", "Value", seciliProgram);

    // Seçilen programı bir değişkene kaydediyoruz.
    VarSet("VPProgramSecimi", seciliProgram);

    // Seçilen programın bilgisini konsola yazdırıyoruz (debug amaçlı).
    printf("VPProgramSecimi:%s \n", seciliProgram);

    int sure; // Yıkama süresi için değişken tanımlıyoruz.

    // Kaydedilen program bilgisini değişkenden geri okuyoruz.
    VarGet("VPProgramSecimi", seciliProgram);

    // Seçilen programın süresini belirliyoruz.
    if(strcmp(seciliProgram, "Pamuklu") == 0) {
        sure = 45; // Pamuklu programı için 45 dakika
    } else if(strcmp(seciliProgram, "Hassas") == 0) {
        sure = 30; // Hassas programı için 30 dakika
    } else if(strcmp(seciliProgram, "Sentetik") == 0) {
        sure = 40; // Sentetik programı için 40 dakika
    } else if(strcmp(seciliProgram, "Hizli") == 0) {
        sure = 15; // Hızlı yıkama programı için 15 dakika
    }

    // Yıkama süresini değişken olarak kaydediyoruz.
    VarSeti("VPYikamaSuresi", sure);

    // Yıkama süresini "00:XX" formatında ekrana göstermek için stringe çeviriyoruz.
    char zamanStr[10];
    sprintf(zamanStr, "00:%02d", sure);

    // Zaman bilgisini değişkene kaydediyoruz.
    VarSet("VPZaman", zamanStr);

    // Ekranda zaman bilgisini gösteriyoruz.
    LabelSet("ELabelZaman", "Text", zamanStr);    

    // Kullanıcıya yıkama işleminin başladığını gösteriyoruz.
    LabelSet("ELabelStatus", "Text", "Yikama Baslatiliyor...");

    // Motoru çalıştırmak için SmartIO Modbus'taki röleyi aktifleştiriyoruz.
    WriteSingleCoil(1, 0x0002, 0xFF00);  

    // Yıkama sürecini başlatan Timer'ı etkin hale getiriyoruz.
    TimerSet("TimerYikama", "Enable", "1");

} else {
    // Eğer kapak açıksa, kullanıcıya uyarı veriyoruz.
    LabelSet("ELabelStatus", "Text", "Kapak Acik! Kapatiniz.");
}
