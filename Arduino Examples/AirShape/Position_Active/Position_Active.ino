/**
 * Position_Active.ino  --  AirShape konum + etkinlik yonetimi
 *
 *   Set/Get_left, Set/Get_top, Set/Get_width, Set/Get_height
 *   Set_active, Get_Active
 *
 *   Shape: ekrandaki touch alanidir (TransShape) — tipik olarak
 *   gorunmez ama "Active=1" oldugunda dokunma olaylari yakalar.
 *
 * Panel tarafi (Position_Active.ahi):
 *   sDemo                : test edilen ana shape
 *   bMoveL/R/U/D         : Set_left -50/+50, Set_top -30/+30
 *   bSizeS/bSizeL        : Width=120 / Width=240 (orta degerli ornek)
 *   bActivate / bDeactivate : Set_active(1/0)
 *   bRead                : 5 Get -> lLeft / lTop / lWidth / lHeight / lActive
 *   bReset               : default'a don
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirShape sDemo   = AirShape("sDemo");
AirLabel lLeft   = AirLabel("lLeft");
AirLabel lTop    = AirLabel("lTop");
AirLabel lWidth  = AirLabel("lWidth");
AirLabel lHeight = AirLabel("lHeight");
AirLabel lActive = AirLabel("lActive");

AirButton bMoveL  = AirButton("bMoveL");
AirButton bMoveR  = AirButton("bMoveR");
AirButton bMoveU  = AirButton("bMoveU");
AirButton bMoveD  = AirButton("bMoveD");
AirButton bSizeS  = AirButton("bSizeS");
AirButton bSizeL  = AirButton("bSizeL");
AirButton bActivate   = AirButton("bActivate");
AirButton bDeactivate = AirButton("bDeactivate");
AirButton bRead   = AirButton("bRead");
AirButton bReset  = AirButton("bReset");

/* Local mirror (Arduino-side clamp). */
static int32_t curLeft = 200;
static int32_t curTop  = 60;

static int32_t clampInt(int32_t v, int32_t lo, int32_t hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* ---------- callback'ler ---------- */
void onMoveL(void *ptr) { curLeft = clampInt(curLeft - 50, 0, 700); sDemo.Set_left((uint32_t)curLeft); }
void onMoveR(void *ptr) { curLeft = clampInt(curLeft + 50, 0, 700); sDemo.Set_left((uint32_t)curLeft); }
void onMoveU(void *ptr) { curTop  = clampInt(curTop - 30, 0, 400);  sDemo.Set_top((uint32_t)curTop); }
void onMoveD(void *ptr) { curTop  = clampInt(curTop + 30, 0, 400);  sDemo.Set_top((uint32_t)curTop); }
void onSizeS(void *ptr) { sDemo.Set_width(120); sDemo.Set_height(80); }
void onSizeL(void *ptr) { sDemo.Set_width(240); sDemo.Set_height(160); }

void onActivate(void *ptr)   { sDemo.Set_active(1); }
void onDeactivate(void *ptr) { sDemo.Set_active(0); }

void onRead(void *ptr)
{
    char tmp[12];
    uint32_t v = 0;

    sDemo.Get_left(&v);   snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v); lLeft.setText(tmp);
    sDemo.Get_top(&v);    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v); lTop.setText(tmp);
    sDemo.Get_width(&v);  snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v); lWidth.setText(tmp);
    sDemo.Get_height(&v); snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v); lHeight.setText(tmp);
    sDemo.Get_Active(&v); snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v); lActive.setText(tmp);
}

void onReset(void *ptr)
{
    curLeft = 200; curTop = 60;
    sDemo.Set_left(curLeft);
    sDemo.Set_top(curTop);
    sDemo.Set_width(160);
    sDemo.Set_height(120);
    sDemo.Set_active(1);
    lLeft.setText("---"); lTop.setText("---");
    lWidth.setText("---"); lHeight.setText("---");
    lActive.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bMoveL, &bMoveR, &bMoveU, &bMoveD,
    &bSizeS, &bSizeL,
    &bActivate, &bDeactivate,
    &bRead, &bReset,
    &sDemo,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bMoveL.attachPop(onMoveL,             &bMoveL);
    bMoveR.attachPop(onMoveR,             &bMoveR);
    bMoveU.attachPop(onMoveU,             &bMoveU);
    bMoveD.attachPop(onMoveD,             &bMoveD);
    bSizeS.attachPop(onSizeS,             &bSizeS);
    bSizeL.attachPop(onSizeL,             &bSizeL);
    bActivate.attachPop(onActivate,       &bActivate);
    bDeactivate.attachPop(onDeactivate,   &bDeactivate);
    bRead.attachPop(onRead,               &bRead);
    bReset.attachPop(onReset,             &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
