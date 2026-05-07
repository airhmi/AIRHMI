/**
 * Font.ino  --  AirButton font (yazi tipi) yonetimi
 *
 * Buton metninin yazi tipi adi, boyutu ve rengini kontrol eder:
 *
 *   SET:
 *     - setFont(String)            : Font adi (Roboto, Arial, vb.)
 *     - Set_font_size(uint32_t)    : Font boyutu (panel: 4..500)
 *     - Set_font_color(uint32_t)   : Font rengi (signed int, ARGB)
 *     - Set_font_colorRGB(String)  : Font rengi hex string ("#FF0000")
 *
 *   GET:
 *     - getFont(char*, int)        : Mevcut font adini okur
 *     - Get_font_size(uint32_t*)   : Mevcut font boyutunu okur
 *     - Get_font_color(uint32_t*)  : Mevcut font rengini okur
 *
 * Panel tarafi (Font.ahi):
 *   bDemo                : test butonu (font ozellikleri burada gorulur)
 *   bRoboto / bArial     : font adi
 *   bSize16/22/28/36     : font boyutu (panel limitleri 4..500)
 *   bBlack/White/Red/Blue: font rengi (signed int)
 *   bRedHex              : font rengi hex string ile (#FF0000)
 *   bRead                : font adi + boyut + renk okur
 *   bReset               : Default Roboto/22/siyah'a doner
 *   lFontName / lFontSize / lFontColor : Get sonuclari
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirButton bDemo      = AirButton("bDemo");
AirButton bRoboto    = AirButton("bRoboto");
AirButton bArial     = AirButton("bArial");
AirButton bSize16    = AirButton("bSize16");
AirButton bSize22    = AirButton("bSize22");
AirButton bSize28    = AirButton("bSize28");
AirButton bSize36    = AirButton("bSize36");
AirButton bBlack     = AirButton("bBlack");
AirButton bWhite     = AirButton("bWhite");
AirButton bRed       = AirButton("bRed");
AirButton bBlue      = AirButton("bBlue");
AirButton bRedHex    = AirButton("bRedHex");
AirButton bRead      = AirButton("bRead");
AirButton bReset     = AirButton("bReset");

AirLabel  lFontName  = AirLabel("lFontName");
AirLabel  lFontSize  = AirLabel("lFontSize");
AirLabel  lFontColor = AirLabel("lFontColor");

/* Renk sabitleri */
static const uint32_t COLOR_BLACK = 0xFF000000UL;
static const uint32_t COLOR_WHITE = 0xFFFFFFFFUL;
static const uint32_t COLOR_RED   = 0xFFFF0000UL;
static const uint32_t COLOR_BLUE  = 0xFF0000FFUL;

/* ---------- Font name callbacks ---------- */
void onRoboto(void *ptr) { bDemo.setFont(String("Roboto")); }
void onArial(void *ptr)  { bDemo.setFont(String("Arial"));  }

/* ---------- Font size callbacks ---------- */
void onSize16(void *ptr) { bDemo.Set_font_size(16); }
void onSize22(void *ptr) { bDemo.Set_font_size(22); }
void onSize28(void *ptr) { bDemo.Set_font_size(28); }
void onSize36(void *ptr) { bDemo.Set_font_size(36); }

/* ---------- Font color callbacks ---------- */
void onBlack(void *ptr) { bDemo.Set_font_color(COLOR_BLACK); }
void onWhite(void *ptr) { bDemo.Set_font_color(COLOR_WHITE); }
void onRed(void *ptr)   { bDemo.Set_font_color(COLOR_RED);   }
void onBlue(void *ptr)  { bDemo.Set_font_color(COLOR_BLUE);  }

void onRedHex(void *ptr)
{
    /* RGB hex string (panel atoi yerine strtol(...,16) kullanir) */
    bDemo.Set_font_colorRGB(String("#FF0000"));
}

/* ---------- Read callback ---------- */
void onRead(void *ptr)
{
    char tmp[32];
    uint32_t v = 0;

    /* Font adi */
    char fontBuf[24] = {0};
    bDemo.getFont(fontBuf, sizeof(fontBuf));
    lFontName.setText(fontBuf);

    /* Font boyutu */
    bDemo.Get_font_size(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lFontSize.setText(tmp);

    /* Font rengi */
    bDemo.Get_font_color(&v);
    snprintf(tmp, sizeof(tmp), "%ld", (long)(int32_t)v);
    lFontColor.setText(tmp);
}

void onReset(void *ptr)
{
    bDemo.setFont(String("Roboto"));
    bDemo.Set_font_size(22);
    bDemo.Set_font_color(COLOR_BLACK);
    lFontName.setText("---");
    lFontSize.setText("---");
    lFontColor.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bRoboto, &bArial,
    &bSize16, &bSize22, &bSize28, &bSize36,
    &bBlack, &bWhite, &bRed, &bBlue, &bRedHex,
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

    bBlack.attachPop(onBlack,   &bBlack);
    bWhite.attachPop(onWhite,   &bWhite);
    bRed.attachPop(onRed,       &bRed);
    bBlue.attachPop(onBlue,     &bBlue);
    bRedHex.attachPop(onRedHex, &bRedHex);

    bRead.attachPop(onRead,     &bRead);
    bReset.attachPop(onReset,   &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
