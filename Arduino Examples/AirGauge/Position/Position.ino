/**
 * Position.ino  --  AirGauge konum / yaricap yonetimi
 *
 *   Set/Get_left   : sol kenar (px)
 *   Set/Get_top    : ust kenar (px)
 *   Set/Get_radius : gauge yaricapi (px) - sim'de Width/Height = 2*radius
 *
 * Panel tarafi (Position.ahi):
 *   gDemo                 : test edilen ana gauge
 *   bMoveL/bMoveR         : Left'i -50 / +50 piksel
 *   bMoveU/bMoveD         : Top'u -30 / +30 piksel
 *   bRSmall/bRBig         : Radius'u 60 / 120
 *   bRead                 : Get_left + Get_top + Get_radius
 *   bReset                : Default konum (left=320, top=70, radius=80)
 *   lLeft / lTop / lRad   : Get sonuclari
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirGauge  gDemo  = AirGauge("gDemo");
AirLabel  lLeft  = AirLabel("lLeft");
AirLabel  lTop   = AirLabel("lTop");
AirLabel  lRad   = AirLabel("lRad");

AirButton bMoveL  = AirButton("bMoveL");
AirButton bMoveR  = AirButton("bMoveR");
AirButton bMoveU  = AirButton("bMoveU");
AirButton bMoveD  = AirButton("bMoveD");
AirButton bRSmall = AirButton("bRSmall");
AirButton bRBig   = AirButton("bRBig");
AirButton bRead   = AirButton("bRead");
AirButton bReset  = AirButton("bReset");

/* Local mirror (panel sinirlarini Arduino'da tutmak icin) */
static int32_t curLeft = 320;
static int32_t curTop  = 70;
static int32_t curRad  = 80;

/* ---------- yardimci ---------- */
static int32_t clampInt(int32_t v, int32_t lo, int32_t hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* ---------- callback'ler ---------- */
void onMoveL(void *ptr)
{
    curLeft = clampInt(curLeft - 50, 0, 700);
    gDemo.Set_left((uint32_t)curLeft);
}
void onMoveR(void *ptr)
{
    curLeft = clampInt(curLeft + 50, 0, 700);
    gDemo.Set_left((uint32_t)curLeft);
}
void onMoveU(void *ptr)
{
    curTop = clampInt(curTop - 30, 0, 400);
    gDemo.Set_top((uint32_t)curTop);
}
void onMoveD(void *ptr)
{
    curTop = clampInt(curTop + 30, 0, 400);
    gDemo.Set_top((uint32_t)curTop);
}
void onRSmall(void *ptr)
{
    curRad = 60;
    gDemo.Set_radius((uint32_t)curRad);
}
void onRBig(void *ptr)
{
    curRad = 120;
    gDemo.Set_radius((uint32_t)curRad);
}

void onRead(void *ptr)
{
    char tmp[12];
    uint32_t v = 0;

    gDemo.Get_left(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lLeft.setText(tmp);

    gDemo.Get_top(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lTop.setText(tmp);

    gDemo.Get_radius(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lRad.setText(tmp);
}

void onReset(void *ptr)
{
    curLeft = 320; curTop = 70; curRad = 80;
    gDemo.Set_left(curLeft);
    gDemo.Set_top(curTop);
    gDemo.Set_radius(curRad);
    lLeft.setText("---");
    lTop.setText("---");
    lRad.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bMoveL, &bMoveR, &bMoveU, &bMoveD,
    &bRSmall, &bRBig,
    &bRead, &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bMoveL.attachPop(onMoveL,   &bMoveL);
    bMoveR.attachPop(onMoveR,   &bMoveR);
    bMoveU.attachPop(onMoveU,   &bMoveU);
    bMoveD.attachPop(onMoveD,   &bMoveD);
    bRSmall.attachPop(onRSmall, &bRSmall);
    bRBig.attachPop(onRBig,     &bRBig);
    bRead.attachPop(onRead,     &bRead);
    bReset.attachPop(onReset,   &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
