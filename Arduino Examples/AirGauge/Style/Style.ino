/**
 * Style.ino  --  AirGauge ibre / merkez renk yonetimi
 *
 *   Set_needleColor       : ibre rengi (NEEDLE_COLOR / Pen_Color)
 *   Set_needleCenterColor : ibre merkez yuvarlak rengi (NEEDLE_CIRCLE_COLOR)
 *
 * Panel tarafi (Style.ahi):
 *   gDemo                : test edilen ana gauge
 *   bNeedleR/G/B/Y       : Set_needleColor (red/green/blue/yellow)
 *   bCenterR/G/B/K       : Set_needleCenterColor (red/green/blue/black)
 *   bRead                : Get_needleColor + Get_needleCenterColor
 *   bReset               : default (needle red, center gri)
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirGauge  gDemo   = AirGauge("gDemo");
AirLabel  lNeedle = AirLabel("lNeedle");
AirLabel  lCenter = AirLabel("lCenter");

AirButton bNeedleR = AirButton("bNeedleR");
AirButton bNeedleG = AirButton("bNeedleG");
AirButton bNeedleB = AirButton("bNeedleB");
AirButton bNeedleY = AirButton("bNeedleY");
AirButton bCenterR = AirButton("bCenterR");
AirButton bCenterG = AirButton("bCenterG");
AirButton bCenterB = AirButton("bCenterB");
AirButton bCenterK = AirButton("bCenterK");
AirButton bRead    = AirButton("bRead");
AirButton bReset   = AirButton("bReset");

/* Renk sabitleri (signed int, panel uyumlu). */
static const int32_t COL_RED    = -65536;
static const int32_t COL_GREEN  = -16711936;
static const int32_t COL_BLUE   = -16776961;
static const int32_t COL_YELLOW = -256;
static const int32_t COL_BLACK  = -16777216;
static const int32_t COL_GRAY   = -8355712;

/* ---------- callback'ler ---------- */
void onNeedleR(void *ptr) { gDemo.Set_needleColor((uint32_t)COL_RED); }
void onNeedleG(void *ptr) { gDemo.Set_needleColor((uint32_t)COL_GREEN); }
void onNeedleB(void *ptr) { gDemo.Set_needleColor((uint32_t)COL_BLUE); }
void onNeedleY(void *ptr) { gDemo.Set_needleColor((uint32_t)COL_YELLOW); }

void onCenterR(void *ptr) { gDemo.Set_needleCenterColor((uint32_t)COL_RED); }
void onCenterG(void *ptr) { gDemo.Set_needleCenterColor((uint32_t)COL_GREEN); }
void onCenterB(void *ptr) { gDemo.Set_needleCenterColor((uint32_t)COL_BLUE); }
void onCenterK(void *ptr) { gDemo.Set_needleCenterColor((uint32_t)COL_BLACK); }

void onRead(void *ptr)
{
    char tmp[16];
    uint32_t v = 0;

    gDemo.Get_needleColor(&v);
    snprintf(tmp, sizeof(tmp), "%ld", (long)(int32_t)v);
    lNeedle.setText(tmp);

    gDemo.Get_needleCenterColor(&v);
    snprintf(tmp, sizeof(tmp), "%ld", (long)(int32_t)v);
    lCenter.setText(tmp);
}

void onReset(void *ptr)
{
    gDemo.Set_needleColor((uint32_t)COL_RED);
    gDemo.Set_needleCenterColor((uint32_t)COL_GRAY);
    lNeedle.setText("---");
    lCenter.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bNeedleR, &bNeedleG, &bNeedleB, &bNeedleY,
    &bCenterR, &bCenterG, &bCenterB, &bCenterK,
    &bRead, &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bNeedleR.attachPop(onNeedleR, &bNeedleR);
    bNeedleG.attachPop(onNeedleG, &bNeedleG);
    bNeedleB.attachPop(onNeedleB, &bNeedleB);
    bNeedleY.attachPop(onNeedleY, &bNeedleY);
    bCenterR.attachPop(onCenterR, &bCenterR);
    bCenterG.attachPop(onCenterG, &bCenterG);
    bCenterB.attachPop(onCenterB, &bCenterB);
    bCenterK.attachPop(onCenterK, &bCenterK);
    bRead.attachPop(onRead,       &bRead);
    bReset.attachPop(onReset,     &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
