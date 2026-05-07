/**
 * Font.ino  --  AirLabel font yonetimi
 *
 *   SET:
 *     - Set_fontName(String)        : font ailesi adi (quote'lu)
 *     - Set_fontSize(uint32_t)      : panel limit 4..500
 *     - Set_fontColor(uint32_t)     : signed int (%ld cast)
 *
 *   GET:
 *     - getFontName(char*, int)
 *     - Get_fontSize(uint32_t*)
 *     - Get_fontColor(uint32_t*)
 *
 * Panel tarafi (Font.ahi):
 *   lDemo                          : test edilen ana etiket
 *   bRoboto / bArial               : font adi presetleri
 *   bSize16/22/28/36               : font boyutu
 *   bBlack/White/Red/Blue          : font rengi
 *   bRead                          : tum font ozelliklerini okur
 *   bReset                         : default'a doner
 *   lFontName / lFontSize / lFontColor : Get sonuclari
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirLabel  lDemo      = AirLabel("lDemo");
AirLabel  lFontName  = AirLabel("lFontName");
AirLabel  lFontSize  = AirLabel("lFontSize");
AirLabel  lFontColor = AirLabel("lFontColor");

AirButton bRoboto = AirButton("bRoboto");
AirButton bArial  = AirButton("bArial");
AirButton bSize16 = AirButton("bSize16");
AirButton bSize22 = AirButton("bSize22");
AirButton bSize28 = AirButton("bSize28");
AirButton bSize36 = AirButton("bSize36");
AirButton bBlack  = AirButton("bBlack");
AirButton bWhite  = AirButton("bWhite");
AirButton bRed    = AirButton("bRed");
AirButton bBlue   = AirButton("bBlue");
AirButton bRead   = AirButton("bRead");
AirButton bReset  = AirButton("bReset");

/* Renk sabitleri (signed int, .ahi format) */
static const uint32_t COLOR_BLACK = 0xFF000000UL;
static const uint32_t COLOR_WHITE = 0xFFFFFFFFUL;
static const uint32_t COLOR_RED   = 0xFFFF0000UL;
static const uint32_t COLOR_BLUE  = 0xFF0000FFUL;

/* ---------- callback'ler ---------- */
void onRoboto(void *ptr) { lDemo.Set_fontName(String("Roboto")); }
void onArial(void *ptr)  { lDemo.Set_fontName(String("Arial"));  }

void onSize16(void *ptr) { lDemo.Set_fontSize(16); }
void onSize22(void *ptr) { lDemo.Set_fontSize(22); }
void onSize28(void *ptr) { lDemo.Set_fontSize(28); }
void onSize36(void *ptr) { lDemo.Set_fontSize(36); }

void onBlack(void *ptr) { lDemo.Set_fontColor(COLOR_BLACK); }
void onWhite(void *ptr) { lDemo.Set_fontColor(COLOR_WHITE); }
void onRed(void *ptr)   { lDemo.Set_fontColor(COLOR_RED);   }
void onBlue(void *ptr)  { lDemo.Set_fontColor(COLOR_BLUE);  }

void onRead(void *ptr)
{
    char tmp[24] = {0};
    uint32_t v = 0;

    lDemo.getFontName(tmp, sizeof(tmp));
    lFontName.setText(tmp);

    lDemo.Get_fontSize(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lFontSize.setText(tmp);

    lDemo.Get_fontColor(&v);
    snprintf(tmp, sizeof(tmp), "%ld", (long)(int32_t)v);
    lFontColor.setText(tmp);
}

void onReset(void *ptr)
{
    lDemo.Set_fontName(String("Roboto"));
    lDemo.Set_fontSize(28);
    lDemo.Set_fontColor(COLOR_WHITE);
    lFontName.setText("---");
    lFontSize.setText("---");
    lFontColor.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bRoboto, &bArial,
    &bSize16, &bSize22, &bSize28, &bSize36,
    &bBlack, &bWhite, &bRed, &bBlue,
    &bRead, &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bRoboto.attachPop(onRoboto, &bRoboto);
    bArial.attachPop(onArial,   &bArial);

    bSize16.attachPop(onSize16, &bSize16);
    bSize22.attachPop(onSize22, &bSize22);
    bSize28.attachPop(onSize28, &bSize28);
    bSize36.attachPop(onSize36, &bSize36);

    bBlack.attachPop(onBlack, &bBlack);
    bWhite.attachPop(onWhite, &bWhite);
    bRed.attachPop(onRed,     &bRed);
    bBlue.attachPop(onBlue,   &bBlue);

    bRead.attachPop(onRead,   &bRead);
    bReset.attachPop(onReset, &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
