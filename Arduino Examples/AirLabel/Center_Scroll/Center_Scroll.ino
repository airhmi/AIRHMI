/**
 * Center_Scroll.ino  --  AirLabel hizalama ve kaydirma yonetimi
 *
 *   Center : metnin yatay hizalamasi
 *            0 = sola, 1 = ortala, 2 = saga
 *
 *   ScrollEnable : panel firmware'inin animasyon modu
 *                  (yalnizca panel'de etkin; simulator'de gorunmez)
 *
 *   ScrollSpeed  : animasyon suresi catpani (sure = N * 50ms)
 *
 *   API:
 *     SET: Set_center, Set_scrollEnable, Set_scrollSpeed
 *     GET: Get_center
 *     (ScrollEnable / ScrollSpeed icin Get yoktur — panel-side
 *      write-only; biz local state tutariz.)
 *
 * Panel tarafi (Center_Scroll.ahi):
 *   bDemo           : test edilen ELabelBox (sabit kutu — Center gorunur)
 *   bLeft/bCenter/bRight : Set_center(0/1/2)
 *   bScrollOn       : Set_scrollEnable(5) + Set_scrollSpeed(6) varsayilan
 *   bScrollOff      : Set_scrollEnable(0)
 *   bUpdate         : setText'i toggle eder (panel'de scroll varsa
 *                     yeni metin animasyon ile gelir)
 *   bRead           : Get_center sonucunu lCenter'a yazar
 *   bReset          : center=0, scroll=off
 *   lCenter         : Get_center sonucu
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirLabel  bDemo       = AirLabel("bDemo");
AirLabel  lCenter     = AirLabel("lCenter");

AirButton bLeft       = AirButton("bLeft");
AirButton bCenter     = AirButton("bCenter");
AirButton bRight      = AirButton("bRight");
AirButton bScrollOn   = AirButton("bScrollOn");
AirButton bScrollOff  = AirButton("bScrollOff");
AirButton bUpdate     = AirButton("bUpdate");
AirButton bRead       = AirButton("bRead");
AirButton bReset      = AirButton("bReset");

/* setText toggle icin counter */
static uint8_t textIdx = 0;
static const char *textTable[] = {
    "AirHMI Center Test",
    "Scrolling Long Text",
    "Hizalama / Kaydirma",
};

/* ---------- callback'ler ---------- */
void onLeft(void *ptr)       { bDemo.Set_center(0); }
void onCenterAlign(void *ptr){ bDemo.Set_center(1); }
void onRight(void *ptr)      { bDemo.Set_center(2); }

void onScrollOn(void *ptr)
{
    /* Mode 5 = saga dogru kayma; Speed 6 = 300 ms. */
    bDemo.Set_scrollEnable(5);
    bDemo.Set_scrollSpeed(6);
}

void onScrollOff(void *ptr)
{
    bDemo.Set_scrollEnable(0);
}

void onUpdate(void *ptr)
{
    textIdx = (textIdx + 1) % (sizeof(textTable) / sizeof(textTable[0]));
    bDemo.setText(textTable[textIdx]);
}

void onRead(void *ptr)
{
    char tmp[8];
    uint32_t v = 0;

    bDemo.Get_center(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lCenter.setText(tmp);
}

void onReset(void *ptr)
{
    bDemo.Set_center(0);
    bDemo.Set_scrollEnable(0);
    bDemo.setText(textTable[0]);
    textIdx = 0;
    lCenter.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bLeft, &bCenter, &bRight,
    &bScrollOn, &bScrollOff, &bUpdate,
    &bRead, &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bLeft.attachPop(onLeft,             &bLeft);
    bCenter.attachPop(onCenterAlign,    &bCenter);
    bRight.attachPop(onRight,           &bRight);
    bScrollOn.attachPop(onScrollOn,     &bScrollOn);
    bScrollOff.attachPop(onScrollOff,   &bScrollOff);
    bUpdate.attachPop(onUpdate,         &bUpdate);
    bRead.attachPop(onRead,             &bRead);
    bReset.attachPop(onReset,           &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
