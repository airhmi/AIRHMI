/**
 * PushPull.ino  --  AirPicture push-pull (toggle) yonetimi
 *
 *   Set/Get_pushpull : resim'in basili tutma durumu (PushPullState)
 *
 *   Panel'de PushPull design-time flag = True olan resmin,
 *   "<name>_press" suffix'li ikinci image'i ile takas davranisi olur.
 *   API tarafindan Set_pushpull(1) "press" gorseline, Set_pushpull(0)
 *   varsayilan gorsele dondurur.
 *
 * Panel tarafi (PushPull.ahi):
 *   pDemo                : test edilen resim (PushPull=True)
 *   bOn / bOff           : Set_pushpull(1/0)
 *   bRead                : Get_pushpull -> lState
 *   bReset               : Set_pushpull(0)
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirPicture pDemo  = AirPicture("pDemo");
AirLabel   lState = AirLabel("lState");

AirButton bOn    = AirButton("bOn");
AirButton bOff   = AirButton("bOff");
AirButton bRead  = AirButton("bRead");
AirButton bReset = AirButton("bReset");

/* ---------- callback'ler ---------- */
void onOn(void *ptr)    { pDemo.Set_pushpull(1); }
void onOff(void *ptr)   { pDemo.Set_pushpull(0); }

void onRead(void *ptr)
{
    char tmp[12];
    uint32_t v = 0;
    pDemo.Get_pushpull(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lState.setText(tmp);
}

void onReset(void *ptr)
{
    pDemo.Set_pushpull(0);
    lState.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bOn, &bOff, &bRead, &bReset,
    &pDemo,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bOn.attachPop(onOn,       &bOn);
    bOff.attachPop(onOff,     &bOff);
    bRead.attachPop(onRead,   &bRead);
    bReset.attachPop(onReset, &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
