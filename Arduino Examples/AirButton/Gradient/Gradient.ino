/**
 * Gradient.ino  --  AirButton gradient ve kalem (pen) yonetimi
 *
 * Buton'un Gradient ve Pen_* (kenar) ozelliklerini Arduino tarafindan
 * kontrol eder.
 *
 *   GRADIENT degerleri (panel atoi):
 *     0 = None (duz dolgu, yalnizca Color)
 *     1 = Horizontal (Color -> ColorTo, soldan saga)
 *     2 = Vertical   (Color -> ColorTo, yukaridan asagi)
 *
 *   SET:
 *     - Set_gradient(uint32_t)        : 0/1/2
 *     - Set_pen_width(uint32_t)       : kenar kalinligi (0..N px)
 *     - Set_pen_color(uint32_t)       : kenar rengi (signed int)
 *     - Set_pen_colorRGB(String)      : kenar rengi hex ("#RRGGBB")
 *
 *   GET:
 *     - Get_gradient(uint32_t*)
 *     - Get_pen_width(uint32_t*)
 *     - Get_pen_color(uint32_t*)
 *
 * Panel tarafi (Gradient.ahi):
 *   bDemo                   : test butonu (Color sari, ColorTo turuncu)
 *   bGradNone/bGradH/bGradV : 0/1/2 set
 *   bPenThin/bPenThick      : Pen_Width 1 / 5
 *   bPenBlack/bPenRed       : Pen_Color
 *   bPenHexBlue             : Pen_Color_RGB ile #0000FF
 *   bRead                   : gradient/pen_width/pen_color okur
 *   bReset                  : default'a doner
 *   lGradient/lPenWidth/lPenColor : Get sonuclari
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirButton bDemo        = AirButton("bDemo");
AirButton bGradNone    = AirButton("bGradNone");
AirButton bGradH       = AirButton("bGradH");
AirButton bGradV       = AirButton("bGradV");
AirButton bPenThin     = AirButton("bPenThin");
AirButton bPenThick    = AirButton("bPenThick");
AirButton bPenBlack    = AirButton("bPenBlack");
AirButton bPenRed      = AirButton("bPenRed");
AirButton bPenHexBlue  = AirButton("bPenHexBlue");
AirButton bRead        = AirButton("bRead");
AirButton bReset       = AirButton("bReset");

AirLabel  lGradient    = AirLabel("lGradient");
AirLabel  lPenWidth    = AirLabel("lPenWidth");
AirLabel  lPenColor    = AirLabel("lPenColor");

/* Renk sabitleri */
static const uint32_t COLOR_BLACK = 0xFF000000UL;
static const uint32_t COLOR_RED   = 0xFFFF0000UL;

/* ---------- callback'ler ---------- */
void onGradNone(void *ptr)   { bDemo.Set_gradient(0); }
void onGradH(void *ptr)      { bDemo.Set_gradient(1); }
void onGradV(void *ptr)      { bDemo.Set_gradient(2); }

void onPenThin(void *ptr)    { bDemo.Set_pen_width(1); }
void onPenThick(void *ptr)   { bDemo.Set_pen_width(5); }

void onPenBlack(void *ptr)   { bDemo.Set_pen_color(COLOR_BLACK); }
void onPenRed(void *ptr)     { bDemo.Set_pen_color(COLOR_RED);   }

void onPenHexBlue(void *ptr) { bDemo.Set_pen_colorRGB(String("#0000FF")); }

void onRead(void *ptr)
{
    char tmp[16];
    uint32_t v = 0;

    bDemo.Get_gradient(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lGradient.setText(tmp);

    bDemo.Get_pen_width(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lPenWidth.setText(tmp);

    bDemo.Get_pen_color(&v);
    snprintf(tmp, sizeof(tmp), "%ld", (long)(int32_t)v);
    lPenColor.setText(tmp);
}

void onReset(void *ptr)
{
    bDemo.Set_gradient(2);
    bDemo.Set_pen_width(1);
    bDemo.Set_pen_color(COLOR_BLACK);
    lGradient.setText("---");
    lPenWidth.setText("---");
    lPenColor.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bGradNone, &bGradH, &bGradV,
    &bPenThin, &bPenThick,
    &bPenBlack, &bPenRed, &bPenHexBlue,
    &bRead, &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bGradNone.attachPop(onGradNone,     &bGradNone);
    bGradH.attachPop(onGradH,           &bGradH);
    bGradV.attachPop(onGradV,           &bGradV);

    bPenThin.attachPop(onPenThin,       &bPenThin);
    bPenThick.attachPop(onPenThick,     &bPenThick);

    bPenBlack.attachPop(onPenBlack,     &bPenBlack);
    bPenRed.attachPop(onPenRed,         &bPenRed);
    bPenHexBlue.attachPop(onPenHexBlue, &bPenHexBlue);

    bRead.attachPop(onRead,             &bRead);
    bReset.attachPop(onReset,           &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
