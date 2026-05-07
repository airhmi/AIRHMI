/**
 * Basics.ino  --  AirGraph temel veri ve renk yonetimi
 *
 *   addValue(ch, value)        : graf'a kanal/deger noktasi ekler
 *   Set_line_color(ch, "#hex") : kanal cizgisinin rengi
 *   clear()                    : tum kanal verilerini temizler
 *
 * Panel tarafi (Basics.ahi):
 *   gDemo                : test edilen ana graph (Line_Count=4)
 *   bAdd0/bAdd1/bAdd2/bAdd3 : channel 0..3'e artan counter degeri ekler
 *   bColR/bColG/bColB/bColY : channel 0 line rengini degistirir
 *   bClear                : graf'i temizler
 *   bReset                : counterlari sifirla + clear
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirGraph  gDemo  = AirGraph("gDemo");

AirButton bAdd0   = AirButton("bAdd0");
AirButton bAdd1   = AirButton("bAdd1");
AirButton bAdd2   = AirButton("bAdd2");
AirButton bAdd3   = AirButton("bAdd3");

AirButton bColR   = AirButton("bColR");
AirButton bColG   = AirButton("bColG");
AirButton bColB   = AirButton("bColB");
AirButton bColY   = AirButton("bColY");

AirButton bClear  = AirButton("bClear");
AirButton bReset  = AirButton("bReset");

/* Kanallar icin testere desenli counter (0..200 arasi). */
static uint32_t cnt[4] = {0, 0, 0, 0};

/* ---------- callback'ler ---------- */
static void pushCh(uint8_t ch)
{
    cnt[ch] = (cnt[ch] + 20) % 200;
    gDemo.addValue(ch, cnt[ch]);
}

void onAdd0(void *ptr) { pushCh(0); }
void onAdd1(void *ptr) { pushCh(1); }
void onAdd2(void *ptr) { pushCh(2); }
void onAdd3(void *ptr) { pushCh(3); }

void onColR(void *ptr) { gDemo.Set_line_color(0, "#FFFF0000"); }
void onColG(void *ptr) { gDemo.Set_line_color(0, "#FF00FF00"); }
void onColB(void *ptr) { gDemo.Set_line_color(0, "#FF0000FF"); }
void onColY(void *ptr) { gDemo.Set_line_color(0, "#FFFFFF00"); }

void onClear(void *ptr) { gDemo.clear(); }

void onReset(void *ptr)
{
    cnt[0] = cnt[1] = cnt[2] = cnt[3] = 0;
    gDemo.clear();
    gDemo.Set_line_color(0, "#FF0000FF");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bAdd0, &bAdd1, &bAdd2, &bAdd3,
    &bColR, &bColG, &bColB, &bColY,
    &bClear, &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bAdd0.attachPop(onAdd0,   &bAdd0);
    bAdd1.attachPop(onAdd1,   &bAdd1);
    bAdd2.attachPop(onAdd2,   &bAdd2);
    bAdd3.attachPop(onAdd3,   &bAdd3);

    bColR.attachPop(onColR,   &bColR);
    bColG.attachPop(onColG,   &bColG);
    bColB.attachPop(onColB,   &bColB);
    bColY.attachPop(onColY,   &bColY);

    bClear.attachPop(onClear, &bClear);
    bReset.attachPop(onReset, &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
