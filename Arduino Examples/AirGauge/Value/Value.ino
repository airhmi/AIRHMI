/**
 * Value.ino  --  AirGauge deger yonetimi
 *
 *   Set_value : gauge ibresinin yansittigi sayi degeri (0..Range)
 *   Get_value : panel'den o anki degeri okur
 *
 *   Range varsayilani panel tarafindan belirlenir (.ahi'da Range=100).
 *
 * Panel tarafi (Value.ahi):
 *   gDemo                 : test edilen ana gauge (EveGauge, R=80, Range=100)
 *   bSet0/bSet50/bSet100  : Set_value(0/50/100)
 *   bInc/bDec             : ortakli +10 / -10
 *   bRead                 : Get_value -> lValue
 *   bReset                : Set_value(0), local sayac sifirla
 *   lValue                : Get sonucu
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirGauge  gDemo  = AirGauge("gDemo");
AirLabel  lValue = AirLabel("lValue");

AirButton bSet0   = AirButton("bSet0");
AirButton bSet50  = AirButton("bSet50");
AirButton bSet100 = AirButton("bSet100");
AirButton bInc    = AirButton("bInc");
AirButton bDec    = AirButton("bDec");
AirButton bRead   = AirButton("bRead");
AirButton bReset  = AirButton("bReset");

/* Local mirror: panel + Arduino senkron tutmak icin (clamp 0..100). */
static int32_t curVal = 0;

/* ---------- callback'ler ---------- */
static void writeValue(int32_t v)
{
    if (v < 0)   v = 0;
    if (v > 100) v = 100;
    curVal = v;
    gDemo.Set_value((uint32_t)v);
}

void onSet0(void *ptr)   { writeValue(0); }
void onSet50(void *ptr)  { writeValue(50); }
void onSet100(void *ptr) { writeValue(100); }
void onInc(void *ptr)    { writeValue(curVal + 10); }
void onDec(void *ptr)    { writeValue(curVal - 10); }

void onRead(void *ptr)
{
    char tmp[12];
    uint32_t v = 0;
    gDemo.Get_value(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lValue.setText(tmp);
}

void onReset(void *ptr)
{
    writeValue(0);
    lValue.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bSet0, &bSet50, &bSet100,
    &bInc, &bDec,
    &bRead, &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bSet0.attachPop(onSet0,     &bSet0);
    bSet50.attachPop(onSet50,   &bSet50);
    bSet100.attachPop(onSet100, &bSet100);
    bInc.attachPop(onInc,       &bInc);
    bDec.attachPop(onDec,       &bDec);
    bRead.attachPop(onRead,     &bRead);
    bReset.attachPop(onReset,   &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
