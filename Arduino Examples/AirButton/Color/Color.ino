/**
 * Color.ino  --  AirButton arka plan rengi yonetimi
 *
 * AirButton'un arka plan rengi (Color) ve gradient bitis rengi (ColorTo)
 * metodlarini gosterir:
 *
 *   SET:
 *     - Set_background_color(uint32_t)    : ana arka plan rengi
 *     - Set_background_colorTo(uint32_t)  : gradient bitis rengi
 *
 *   GET:
 *     - Get_background_color(uint32_t*)   : mevcut Color degerini okur
 *     - Get_background_colorTo(uint32_t*) : mevcut ColorTo degerini okur
 *
 * RENK FORMATI:
 *   Panel renkleri 32-bit ARGB (0xAARRGGBB) bit pattern olarak saklar.
 *   Ornekler:
 *     - Kirmizi (alpha=FF, R=FF, G=00, B=00)  : 0xFFFF0000  ( = -65536 signed)
 *     - Yesil   (alpha=FF, R=00, G=FF, B=00)  : 0xFF00FF00
 *     - Mavi    (alpha=FF, R=00, G=00, B=FF)  : 0xFF0000FF
 *     - Sari    (alpha=FF, R=FF, G=FF, B=00)  : 0xFFFFFF00
 *     - Beyaz                                  : 0xFFFFFFFF  ( = -1 signed)
 *     - Siyah                                  : 0xFF000000  ( = -16777216 signed)
 *
 *   Set_background_color(uint32_t) artik %ld + (int32_t) cast kullanir,
 *   panele dogru signed deger gider.
 *
 * Panel tarafi (Color.ahi):
 *   bDemo       : metni "Demo" olan ana buton (rengi degisecek)
 *   bRed/bGreen/bBlue/bYellow : 4 hazir renk butonu
 *   bGradient   : Color + ColorTo birlikte ayarlar (gradient)
 *   bRead       : Color + ColorTo okuyup lColor / lColorTo'ya yazar
 *   bReset      : Default beyaz/sari renge donduruyor
 *   lColor      : Get sonucu - Color
 *   lColorTo    : Get sonucu - ColorTo
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirButton bDemo     = AirButton("bDemo");
AirButton bRed      = AirButton("bRed");
AirButton bGreen    = AirButton("bGreen");
AirButton bBlue     = AirButton("bBlue");
AirButton bYellow   = AirButton("bYellow");
AirButton bGradient = AirButton("bGradient");
AirButton bRead     = AirButton("bRead");
AirButton bReset    = AirButton("bReset");

AirLabel  lColor    = AirLabel("lColor");
AirLabel  lColorTo  = AirLabel("lColorTo");

/* Renk sabitleri (0xAARRGGBB bit pattern) */
static const uint32_t COLOR_RED     = 0xFFFF0000UL;
static const uint32_t COLOR_GREEN   = 0xFF00FF00UL;
static const uint32_t COLOR_BLUE    = 0xFF0000FFUL;
static const uint32_t COLOR_YELLOW  = 0xFFFFFF00UL;
static const uint32_t COLOR_WHITE   = 0xFFFFFFFFUL;
static const uint32_t COLOR_BLACK   = 0xFF000000UL;
static const uint32_t COLOR_DARKRED = 0xFF800000UL;
static const uint32_t COLOR_GOLD    = 0xFFFFC000UL;

/* ---------- callback'ler ---------- */
void onRed(void *ptr)    { bDemo.Set_background_color(COLOR_RED);    }
void onGreen(void *ptr)  { bDemo.Set_background_color(COLOR_GREEN);  }
void onBlue(void *ptr)   { bDemo.Set_background_color(COLOR_BLUE);   }
void onYellow(void *ptr) { bDemo.Set_background_color(COLOR_YELLOW); }

void onGradient(void *ptr)
{
    /* Sariden kirmiziya gradient */
    bDemo.Set_background_color(COLOR_GOLD);
    bDemo.Set_background_colorTo(COLOR_DARKRED);
}

void onRead(void *ptr)
{
    char tmp[16];
    uint32_t v = 0;

    bDemo.Get_background_color(&v);
    snprintf(tmp, sizeof(tmp), "%ld", (long)(int32_t)v);
    lColor.setText(tmp);

    bDemo.Get_background_colorTo(&v);
    snprintf(tmp, sizeof(tmp), "%ld", (long)(int32_t)v);
    lColorTo.setText(tmp);
}

void onReset(void *ptr)
{
    bDemo.Set_background_color(COLOR_WHITE);
    bDemo.Set_background_colorTo(COLOR_YELLOW);
    lColor.setText("---");
    lColorTo.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bRed,
    &bGreen,
    &bBlue,
    &bYellow,
    &bGradient,
    &bRead,
    &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bRed.attachPop(onRed,           &bRed);
    bGreen.attachPop(onGreen,       &bGreen);
    bBlue.attachPop(onBlue,         &bBlue);
    bYellow.attachPop(onYellow,     &bYellow);
    bGradient.attachPop(onGradient, &bGradient);
    bRead.attachPop(onRead,         &bRead);
    bReset.attachPop(onReset,       &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
