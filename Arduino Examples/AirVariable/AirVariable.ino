/**
 * AirVariable.ino
 *
 * AirHMI panelindeki Variable nesneleriyle Arduino tarafindan veri
 * alis-verisi yapan ornek proje. AirVariable sinifinin tum metodlarini
 * kullanir:
 *
 *   - VarSeti  : 32-bit isaretsiz tamsayi yazma
 *   - VarSetf  : ondalikli sayi (double) yazma
 *   - VarSet   : metin (String) yazma
 *   - VarGeti  : tamsayi okuma
 *   - VarGetf  : ondalikli sayi okuma
 *   - VarGet   : ham metin okuma
 *
 * Beraberindeki AirVariable.ahi dosyasi paneldeki nesneleri tanimlar:
 *
 *   bWrite (EButton)   -> "Write" butonu, Arduino'ya pop callback gonderir
 *   bRead  (EButton)   -> "Read"  butonu, Arduino'ya pop callback gonderir
 *   vInt   (EVariable) -> Type: int
 *   vFloat (EVariable) -> Type: float
 *   vText  (EVariable) -> Type: String
 *   lInt   (ELabelBox) -> okunan tamsayinin gosterildigi etiket
 *   lFloat (ELabelBox) -> okunan ondaligin gosterildigi etiket
 *   lText  (ELabelBox) -> okunan metnin gosterildigi etiket
 *
 * NOT: airSerial = Serial oldugundan, Serial.print/println kullanilmaz;
 *      USB-Serial portu yalnizca AirHMI protokolu icin ayrilmistir.
 *      (Simulator veya panel COM portunu acar.)
 *
 * Yazar : Eyza Teknoloji
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirButton   bWrite = AirButton("bWrite");
AirButton   bRead  = AirButton("bRead");

AirVariable vInt   = AirVariable("vInt");
AirVariable vFloat = AirVariable("vFloat");
AirVariable vText  = AirVariable("vText");

AirLabel    lInt   = AirLabel("lInt");
AirLabel    lFloat = AirLabel("lFloat");
AirLabel    lText  = AirLabel("lText");

/* ---------- "Write" butonu callback ---------- */
void onWritePressed(void *ptr)
{
    /* Tamsayi yazma */
    vInt.VarSeti(12345UL);

    /* Ondalikli sayi yazma — VarSetf icinde dtostrf kullanilir, AVR'de de calisir */
    vFloat.VarSetf(-3.14159);

    /* Metin yazma */
    vText.VarSet(String("Merhaba AirHMI"));
}

/* ---------- "Read" butonu callback ---------- */
void onReadPressed(void *ptr)
{
    char tmp[32];

    /* Tamsayi oku ve etikete bas */
    uint32_t i = vInt.VarGeti();
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)i);
    lInt.setText(tmp);

    /* Ondalikli oku ve etikete bas */
    double f = vFloat.VarGetf();
    dtostrf(f, 0, 4, tmp);
    lFloat.setText(tmp);

    /* Ham metin oku ve etikete bas */
    char buf[32] = {0};
    vText.VarGet(buf, sizeof(buf));
    lText.setText(buf);
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bWrite,
    &bRead,
    NULL
};

void setup()
{
    /* airInit() bazi konfigurasyonlarda port'u acmadigi icin elle aciyoruz.
       Serial.print/println kullanilmamali — port AirHMI protokoluna ayrilmistir. */
    Serial.begin(115200);
    airInit();

    bWrite.attachPop(onWritePressed, &bWrite);
    bRead.attachPop(onReadPressed,   &bRead);
}

void loop()
{
    airLoop(air_listen_list);
}
