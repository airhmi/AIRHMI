/**
 * Value_Color.ino  --  AirSlider deger ve 3 renk yonetimi
 *
 *   Set_Value           : slider degeri (0..Range)
 *   Set_Color           : track'in **oncesi** (thumb'in solu, dolu kisim)
 *   Set_BackgroundColor : track'in **sonrasi** (thumb'in sagi, bos kisim)
 *   Set_ThumbColor      : ortadaki **yuvarlak** (surukleyici dugme)
 *
 *   Get karsiliklari ile panel'den geri okunur.
 *
 * Panel tarafi (Value_Color.ahi):
 *   sDemo                       : test edilen ana slider (Range=100)
 *   bV0/bV50/bV100              : Set_Value(0/50/100)
 *   bColR/G/B/Y                 : Set_Color (oncesi)
 *   bBackR/G/B/Y                : Set_BackgroundColor (sonrasi)
 *   bThumbR/G/B/Y               : Set_ThumbColor (yuvarlak)
 *   bRead                       : 4 Get -> lValue / lCol / lBack / lThumb
 *   bReset                      : default'a don
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirSlider sDemo  = AirSlider("sDemo");
AirLabel  lValue = AirLabel("lValue");
AirLabel  lCol   = AirLabel("lCol");
AirLabel  lBack  = AirLabel("lBack");
AirLabel  lThumb = AirLabel("lThumb");

AirButton bV0     = AirButton("bV0");
AirButton bV50    = AirButton("bV50");
AirButton bV100   = AirButton("bV100");

AirButton bColR    = AirButton("bColR");
AirButton bColG    = AirButton("bColG");
AirButton bColB    = AirButton("bColB");
AirButton bColY    = AirButton("bColY");

AirButton bBackR   = AirButton("bBackR");
AirButton bBackG   = AirButton("bBackG");
AirButton bBackB   = AirButton("bBackB");
AirButton bBackK   = AirButton("bBackK");

AirButton bThumbR  = AirButton("bThumbR");
AirButton bThumbG  = AirButton("bThumbG");
AirButton bThumbB  = AirButton("bThumbB");
AirButton bThumbW  = AirButton("bThumbW");

AirButton bRead   = AirButton("bRead");
AirButton bReset  = AirButton("bReset");

/* Renk sabitleri (signed int). */
static const int32_t COL_RED    = -65536;
static const int32_t COL_GREEN  = -16711936;
static const int32_t COL_BLUE   = -16776961;
static const int32_t COL_YELLOW = -256;
static const int32_t COL_BLACK  = -16777216;
static const int32_t COL_WHITE  = -1;
static const int32_t COL_GRAY   = -8355712;

/* ---------- callback'ler ---------- */
void onV0(void *ptr)   { sDemo.Set_Value(0); }
void onV50(void *ptr)  { sDemo.Set_Value(50); }
void onV100(void *ptr) { sDemo.Set_Value(100); }

void onColR(void *ptr) { sDemo.Set_Color((uint32_t)COL_RED); }
void onColG(void *ptr) { sDemo.Set_Color((uint32_t)COL_GREEN); }
void onColB(void *ptr) { sDemo.Set_Color((uint32_t)COL_BLUE); }
void onColY(void *ptr) { sDemo.Set_Color((uint32_t)COL_YELLOW); }

void onBackR(void *ptr) { sDemo.Set_BackgroundColor((uint32_t)COL_RED); }
void onBackG(void *ptr) { sDemo.Set_BackgroundColor((uint32_t)COL_GREEN); }
void onBackB(void *ptr) { sDemo.Set_BackgroundColor((uint32_t)COL_BLUE); }
void onBackK(void *ptr) { sDemo.Set_BackgroundColor((uint32_t)COL_BLACK); }

void onThumbR(void *ptr) { sDemo.Set_ThumbColor((uint32_t)COL_RED); }
void onThumbG(void *ptr) { sDemo.Set_ThumbColor((uint32_t)COL_GREEN); }
void onThumbB(void *ptr) { sDemo.Set_ThumbColor((uint32_t)COL_BLUE); }
void onThumbW(void *ptr) { sDemo.Set_ThumbColor((uint32_t)COL_WHITE); }

void onRead(void *ptr)
{
    char tmp[16];
    uint32_t v = 0;

    sDemo.Get_Value(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lValue.setText(tmp);

    sDemo.Get_Color(&v);
    snprintf(tmp, sizeof(tmp), "%ld", (long)(int32_t)v);
    lCol.setText(tmp);

    sDemo.Get_BackgroundColor(&v);
    snprintf(tmp, sizeof(tmp), "%ld", (long)(int32_t)v);
    lBack.setText(tmp);

    sDemo.Get_ThumbColor(&v);
    snprintf(tmp, sizeof(tmp), "%ld", (long)(int32_t)v);
    lThumb.setText(tmp);
}

void onReset(void *ptr)
{
    sDemo.Set_Value(0);
    sDemo.Set_Color((uint32_t)COL_RED);
    sDemo.Set_BackgroundColor((uint32_t)COL_GRAY);
    sDemo.Set_ThumbColor((uint32_t)COL_WHITE);
    lValue.setText("---");
    lCol.setText("---");
    lBack.setText("---");
    lThumb.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bV0, &bV50, &bV100,
    &bColR, &bColG, &bColB, &bColY,
    &bBackR, &bBackG, &bBackB, &bBackK,
    &bThumbR, &bThumbG, &bThumbB, &bThumbW,
    &bRead, &bReset,
    &sDemo,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bV0.attachPop(onV0,         &bV0);
    bV50.attachPop(onV50,       &bV50);
    bV100.attachPop(onV100,     &bV100);

    bColR.attachPop(onColR,     &bColR);
    bColG.attachPop(onColG,     &bColG);
    bColB.attachPop(onColB,     &bColB);
    bColY.attachPop(onColY,     &bColY);

    bBackR.attachPop(onBackR,   &bBackR);
    bBackG.attachPop(onBackG,   &bBackG);
    bBackB.attachPop(onBackB,   &bBackB);
    bBackK.attachPop(onBackK,   &bBackK);

    bThumbR.attachPop(onThumbR, &bThumbR);
    bThumbG.attachPop(onThumbG, &bThumbG);
    bThumbB.attachPop(onThumbB, &bThumbB);
    bThumbW.attachPop(onThumbW, &bThumbW);

    bRead.attachPop(onRead,     &bRead);
    bReset.attachPop(onReset,   &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
