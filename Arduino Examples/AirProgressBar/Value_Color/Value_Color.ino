/**
 * Value_Color.ino  --  AirProgressBar deger ve 3 renk yonetimi
 *
 *   Set_Value           : progress dolum yuzdesi (0..Range)
 *   Set_Color           : dolu kismin rengi (oncesi)
 *   Set_BackgroundColor : bos kismin rengi (sonrasi/arka)
 *   Set_BorderColor     : kenar cizgisi rengi
 *
 * Panel tarafi (Value_Color.ahi):
 *   pDemo                       : test edilen ana progress bar (Range=100)
 *   bV0/bV25/bV50/bV75/bV100    : Set_Value
 *   bColR/G/B/Y                 : Set_Color (dolu)
 *   bBackR/G/B/K                : Set_BackgroundColor (bos)
 *   bBordR/G/B/W                : Set_BorderColor (kenar)
 *   bRead                       : 4 Get -> lValue / lCol / lBack / lBord
 *   bReset                      : default
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirProgressBar pDemo  = AirProgressBar("pDemo");
AirLabel       lValue = AirLabel("lValue");
AirLabel       lCol   = AirLabel("lCol");
AirLabel       lBack  = AirLabel("lBack");
AirLabel       lBord  = AirLabel("lBord");

AirButton bV0    = AirButton("bV0");
AirButton bV25   = AirButton("bV25");
AirButton bV50   = AirButton("bV50");
AirButton bV75   = AirButton("bV75");
AirButton bV100  = AirButton("bV100");

AirButton bColR  = AirButton("bColR");
AirButton bColG  = AirButton("bColG");
AirButton bColB  = AirButton("bColB");
AirButton bColY  = AirButton("bColY");

AirButton bBackR = AirButton("bBackR");
AirButton bBackG = AirButton("bBackG");
AirButton bBackB = AirButton("bBackB");
AirButton bBackK = AirButton("bBackK");

AirButton bBordR = AirButton("bBordR");
AirButton bBordG = AirButton("bBordG");
AirButton bBordB = AirButton("bBordB");
AirButton bBordW = AirButton("bBordW");

AirButton bRead  = AirButton("bRead");
AirButton bReset = AirButton("bReset");

/* Renk sabitleri (signed int). */
static const int32_t COL_RED    = -65536;
static const int32_t COL_GREEN  = -16711936;
static const int32_t COL_BLUE   = -16776961;
static const int32_t COL_YELLOW = -256;
static const int32_t COL_BLACK  = -16777216;
static const int32_t COL_WHITE  = -1;
static const int32_t COL_GRAY   = -9868951;

/* ---------- callback'ler ---------- */
void onV0(void *ptr)   { pDemo.Set_Value(0); }
void onV25(void *ptr)  { pDemo.Set_Value(25); }
void onV50(void *ptr)  { pDemo.Set_Value(50); }
void onV75(void *ptr)  { pDemo.Set_Value(75); }
void onV100(void *ptr) { pDemo.Set_Value(100); }

void onColR(void *ptr) { pDemo.Set_Color((uint32_t)COL_RED); }
void onColG(void *ptr) { pDemo.Set_Color((uint32_t)COL_GREEN); }
void onColB(void *ptr) { pDemo.Set_Color((uint32_t)COL_BLUE); }
void onColY(void *ptr) { pDemo.Set_Color((uint32_t)COL_YELLOW); }

void onBackR(void *ptr) { pDemo.Set_BackgroundColor((uint32_t)COL_RED); }
void onBackG(void *ptr) { pDemo.Set_BackgroundColor((uint32_t)COL_GREEN); }
void onBackB(void *ptr) { pDemo.Set_BackgroundColor((uint32_t)COL_BLUE); }
void onBackK(void *ptr) { pDemo.Set_BackgroundColor((uint32_t)COL_BLACK); }

void onBordR(void *ptr) { pDemo.Set_BorderColor((uint32_t)COL_RED); }
void onBordG(void *ptr) { pDemo.Set_BorderColor((uint32_t)COL_GREEN); }
void onBordB(void *ptr) { pDemo.Set_BorderColor((uint32_t)COL_BLUE); }
void onBordW(void *ptr) { pDemo.Set_BorderColor((uint32_t)COL_WHITE); }

void onRead(void *ptr)
{
    char tmp[16];
    uint32_t v = 0;

    pDemo.Get_Value(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lValue.setText(tmp);

    pDemo.Get_Color(&v);
    snprintf(tmp, sizeof(tmp), "%ld", (long)(int32_t)v);
    lCol.setText(tmp);

    pDemo.Get_BackgroundColor(&v);
    snprintf(tmp, sizeof(tmp), "%ld", (long)(int32_t)v);
    lBack.setText(tmp);

    pDemo.Get_BorderColor(&v);
    snprintf(tmp, sizeof(tmp), "%ld", (long)(int32_t)v);
    lBord.setText(tmp);
}

void onReset(void *ptr)
{
    pDemo.Set_Value(0);
    pDemo.Set_Color((uint32_t)COL_RED);
    pDemo.Set_BackgroundColor((uint32_t)COL_GRAY);
    pDemo.Set_BorderColor((uint32_t)COL_BLUE);
    lValue.setText("---");
    lCol.setText("---");
    lBack.setText("---");
    lBord.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bV0, &bV25, &bV50, &bV75, &bV100,
    &bColR, &bColG, &bColB, &bColY,
    &bBackR, &bBackG, &bBackB, &bBackK,
    &bBordR, &bBordG, &bBordB, &bBordW,
    &bRead, &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bV0.attachPop(onV0,         &bV0);
    bV25.attachPop(onV25,       &bV25);
    bV50.attachPop(onV50,       &bV50);
    bV75.attachPop(onV75,       &bV75);
    bV100.attachPop(onV100,     &bV100);

    bColR.attachPop(onColR,     &bColR);
    bColG.attachPop(onColG,     &bColG);
    bColB.attachPop(onColB,     &bColB);
    bColY.attachPop(onColY,     &bColY);

    bBackR.attachPop(onBackR,   &bBackR);
    bBackG.attachPop(onBackG,   &bBackG);
    bBackB.attachPop(onBackB,   &bBackB);
    bBackK.attachPop(onBackK,   &bBackK);

    bBordR.attachPop(onBordR,   &bBordR);
    bBordG.attachPop(onBordG,   &bBordG);
    bBordB.attachPop(onBordB,   &bBordB);
    bBordW.attachPop(onBordW,   &bBordW);

    bRead.attachPop(onRead,     &bRead);
    bReset.attachPop(onReset,   &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
