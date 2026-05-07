/**
 * Text.ino  --  AirButton metin (caption) yonetimi
 *
 * AirButton sinifinin metin (Text/Caption) ile ilgili tum metodlarini
 * gosterir:
 *
 *   SET:
 *     - setText(const char*) : duz metin (virgul/parantez icerse de
 *                              tirnaklanir, panelde dogru parse edilir)
 *     - setTexti(uint32_t)   : tamsayi -> button metni  (ButtonSeti)
 *     - setTextf(double)     : ondalik -> button metni  (ButtonSetd)
 *
 *   GET:
 *     - getText(char*, len)  : panelden mevcut metni okur ve lResult
 *                              etiketine yazar (round-trip dogrulamasi)
 *
 * Panel tarafi (Text.ahi):
 *   bDemo     : "Test Button"  -- metni yazilan/okunan ana buton
 *   bSetText  : "Set Text"     -- onPop -> setText("Merhaba, AirHMI")
 *   bSetInt   : "Set Int"      -- onPop -> sayac yazar
 *   bSetFloat : "Set Float"    -- onPop -> ondalik sayi yazar
 *   bReadText : "Read"         -- onPop -> getText -> lResult'a yazar
 *   bReset    : "Reset"        -- onPop -> default metne donduruyor
 *   lResult   : ELabelBox      -- okunan metni gosterir
 *
 * Notlar:
 *   - airSerial = Serial; USB Serial paneli/simulatoru bekler.
 *   - getText panel firmware'inin string handler'i zaten framing
 *     yapiyor (CButtonGetEx -- TEXT/CAPTION dali), dolayisiyla simulator
 *     ile dogrudan calismali.
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirButton bDemo      = AirButton("bDemo");
AirButton bSetText   = AirButton("bSetText");
AirButton bSetInt    = AirButton("bSetInt");
AirButton bSetFloat  = AirButton("bSetFloat");
AirButton bReadText  = AirButton("bReadText");
AirButton bReset     = AirButton("bReset");

AirLabel  lResult    = AirLabel("lResult");

/* ---------- callback'ler ---------- */
void onSetText(void *ptr)
{
    /* Panel parser strtok(",") kullaniyor (library_common.c:2833) ve
       virgulu tirnak icinde bile ayirici sayar. Test icin virgulsuz metin
       gonderiyoruz. Panel parser duzeltilince virgullu da calisacak. */
    bDemo.setText("Merhaba AirHMI");
}

void onSetInt(void *ptr)
{
    static uint32_t counter = 0;
    counter++;
    bDemo.setTexti(counter);
}

void onSetFloat(void *ptr)
{
    static double temperature = 24.5;
    temperature += 0.13;
    bDemo.setTextf(temperature);
}

void onReadText(void *ptr)
{
    char buf[32] = {0};
    bDemo.getText(buf, sizeof(buf));
    lResult.setText(buf);
}

void onReset(void *ptr)
{
    bDemo.setText("Test Button");
    lResult.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bSetText,
    &bSetInt,
    &bSetFloat,
    &bReadText,
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
    bReadText.attachPop(onReadText, &bReadText);
    bReset.attachPop(onReset,       &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
