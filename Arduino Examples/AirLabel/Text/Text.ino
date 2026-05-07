/**
 * Text.ino  --  AirLabel metin (caption) yonetimi
 *
 * AirLabel sinifinin metin metodlarini gosterir:
 *
 *   SET:
 *     - setText(const char*) : duz metin (otomatik tirnaklanir)
 *     - setTexti(uint32_t)   : tamsayi (LabelSeti komutu)
 *     - setTextf(double)     : ondalik (LabelSetd komutu, dtostrf)
 *
 *   GET:
 *     - getText(char*, len)  : mevcut metni okur
 *
 * Panel tarafi (Text.ahi):
 *   lDemo                    : test edilen ana etiket
 *   bSetText / bSetInt / bSetFloat / bRead / bReset
 *   lResult                  : Get sonucunu gosterir
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirLabel  lDemo     = AirLabel("lDemo");
AirLabel  lResult   = AirLabel("lResult");

AirButton bSetText  = AirButton("bSetText");
AirButton bSetInt   = AirButton("bSetInt");
AirButton bSetFloat = AirButton("bSetFloat");
AirButton bRead     = AirButton("bRead");
AirButton bReset    = AirButton("bReset");

/* ---------- callback'ler ---------- */
void onSetText(void *ptr)
{
    /* setText cift tirnak ekler — virgul iceren stringler de calisir
       (AirButton'da panel parser strtok virgul yutuyordu, AirLabel'da
       da ayni durum gecerli — virgulden kacin). */
    lDemo.setText("Merhaba AirHMI");
}

void onSetInt(void *ptr)
{
    static uint32_t counter = 0;
    counter++;
    lDemo.setTexti(counter);
}

void onSetFloat(void *ptr)
{
    static double temperature = 24.5;
    temperature += 0.13;
    lDemo.setTextf(temperature);
}

void onRead(void *ptr)
{
    char buf[32] = {0};
    lDemo.getText(buf, sizeof(buf));
    lResult.setText(buf);
}

void onReset(void *ptr)
{
    lDemo.setText("Test Label");
    lResult.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bSetText,
    &bSetInt,
    &bSetFloat,
    &bRead,
    &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bSetText.attachPop(onSetText,   &bSetText);
    bSetInt.attachPop(onSetInt,     &bSetInt);
    bSetFloat.attachPop(onSetFloat, &bSetFloat);
    bRead.attachPop(onRead,         &bRead);
    bReset.attachPop(onReset,       &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
