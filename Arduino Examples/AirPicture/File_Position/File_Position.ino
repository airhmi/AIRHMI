/**
 * File_Position.ino  --  AirPicture konum, gorunurluk, etkinlik
 *
 *   Set/Get_left, Set/Get_top, Set/Get_width, Set/Get_height
 *   Set/Get_visible, Set_active
 *
 * Panel tarafi (File_Position.ahi):
 *   pDemo                : test edilen ana resim (EImage)
 *   bMoveL/R/U/D         : Left/Top -50/+50, -30/+30
 *   bShow / bHide        : Set_visible(1/0)
 *   bActivate / bDeactivate : Set_active(1/0)
 *   bRead                : 5 Get -> lLeft / lTop / lWidth / lHeight / lVisible
 *   bReset               : default'a don
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirPicture pDemo    = AirPicture("pDemo");
AirLabel   lLeft    = AirLabel("lLeft");
AirLabel   lTop     = AirLabel("lTop");
AirLabel   lWidth   = AirLabel("lWidth");
AirLabel   lHeight  = AirLabel("lHeight");
AirLabel   lVisible = AirLabel("lVisible");

AirButton bMoveL   = AirButton("bMoveL");
AirButton bMoveR   = AirButton("bMoveR");
AirButton bMoveU   = AirButton("bMoveU");
AirButton bMoveD   = AirButton("bMoveD");
AirButton bShow    = AirButton("bShow");
AirButton bHide    = AirButton("bHide");
AirButton bActivate   = AirButton("bActivate");
AirButton bDeactivate = AirButton("bDeactivate");
AirButton bRead    = AirButton("bRead");
AirButton bReset   = AirButton("bReset");

/* Local mirror (Arduino-side clamp). */
static int32_t curLeft = 100;
static int32_t curTop  = 60;

/* ---------- callback'ler ---------- */
static int32_t clampInt(int32_t v, int32_t lo, int32_t hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void onMoveL(void *ptr) { curLeft = clampInt(curLeft - 50, 0, 700); pDemo.Set_left((uint32_t)curLeft); }
void onMoveR(void *ptr) { curLeft = clampInt(curLeft + 50, 0, 700); pDemo.Set_left((uint32_t)curLeft); }
void onMoveU(void *ptr) { curTop  = clampInt(curTop - 30, 0, 400);  pDemo.Set_top((uint32_t)curTop); }
void onMoveD(void *ptr) { curTop  = clampInt(curTop + 30, 0, 400);  pDemo.Set_top((uint32_t)curTop); }

void onShow(void *ptr)       { pDemo.Set_visible(1); }
void onHide(void *ptr)       { pDemo.Set_visible(0); }
void onActivate(void *ptr)   { pDemo.Set_active(1); }
void onDeactivate(void *ptr) { pDemo.Set_active(0); }

void onRead(void *ptr)
{
    char tmp[12];
    uint32_t v = 0;

    pDemo.Get_left(&v);    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v); lLeft.setText(tmp);
    pDemo.Get_top(&v);     snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v); lTop.setText(tmp);
    pDemo.Get_width(&v);   snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v); lWidth.setText(tmp);
    pDemo.Get_height(&v);  snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v); lHeight.setText(tmp);
    pDemo.Get_visible(&v); snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v); lVisible.setText(tmp);
}

void onReset(void *ptr)
{
    curLeft = 100; curTop = 60;
    pDemo.Set_left(curLeft);
    pDemo.Set_top(curTop);
    pDemo.Set_visible(1);
    pDemo.Set_active(1);
    lLeft.setText("---"); lTop.setText("---");
    lWidth.setText("---"); lHeight.setText("---");
    lVisible.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bMoveL, &bMoveR, &bMoveU, &bMoveD,
    &bShow, &bHide,
    &bActivate, &bDeactivate,
    &bRead, &bReset,
    &pDemo,
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
    bShow.attachPop(onShow,               &bShow);
    bHide.attachPop(onHide,               &bHide);
    bActivate.attachPop(onActivate,       &bActivate);
    bDeactivate.attachPop(onDeactivate,   &bDeactivate);
    bRead.attachPop(onRead,               &bRead);
    bReset.attachPop(onReset,             &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
