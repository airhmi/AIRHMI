/**
 * PushPull.ino  --  AirButton toggle (push-pull) durum yonetimi
 *
 * PushPull modunda olan butonun toggle state'ini Arduino tarafindan
 * kontrol eder. PushPullState 1 ise buton "basili/toggled", 0 ise
 * "serbest/normal" gorunur. Render kodu (emwin/button.c):
 *   - PushPullState == 1 -> Press_Color, PressFontColor, CaptionPushPull
 *   - PushPullState == 0 -> Color, Font_Color, Caption
 *
 *   SET:
 *     - Set_pushpull(uint32_t) : 1 = pressed, 0 = released
 *
 *   GET:
 *     - Get_pushpull(uint32_t*)
 *
 * NOT: bDemo butonu .ahi'de <PushPull>True</PushPull> olmali. Yoksa
 *      buton momentary calisir ve PushPullState kalici degil.
 *
 * NOT 2: Panel firmware'inde eskiden invert mantigi vardi
 *        (atoi(value) ? 0 : 1). 03_button.c:506'da duzeltildi
 *        (atoi(value) ? 1 : 0). Round-trip artik tutarli.
 *
 * Panel tarafi (PushPull.ahi):
 *   bDemo            : <PushPull>True</PushPull> olan toggle buton
 *   bSetOn / bSetOff : Set_pushpull(1) / Set_pushpull(0)
 *   bToggle          : Get -> tersini set (state takip eden Arduino-side)
 *   bRead            : Get_pushpull -> lState
 *   bReset           : state=0
 *   lState           : Get sonucu
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirButton bDemo    = AirButton("bDemo");
AirButton bSetOn   = AirButton("bSetOn");
AirButton bSetOff  = AirButton("bSetOff");
AirButton bToggle  = AirButton("bToggle");
AirButton bRead    = AirButton("bRead");
AirButton bReset   = AirButton("bReset");

AirLabel  lState   = AirLabel("lState");

/* ---------- callback'ler ---------- */
void onSetOn(void *ptr)  { bDemo.Set_pushpull(1); }
void onSetOff(void *ptr) { bDemo.Set_pushpull(0); }

void onToggle(void *ptr)
{
    uint32_t v = 0;
    bDemo.Get_pushpull(&v);
    bDemo.Set_pushpull(v ? 0 : 1);
}

void onRead(void *ptr)
{
    uint32_t v = 0;
    char tmp[8];
    bDemo.Get_pushpull(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lState.setText(tmp);
}

void onReset(void *ptr)
{
    bDemo.Set_pushpull(0);
    lState.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bSetOn,
    &bSetOff,
    &bToggle,
    &bRead,
    &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bSetOn.attachPop(onSetOn,   &bSetOn);
    bSetOff.attachPop(onSetOff, &bSetOff);
    bToggle.attachPop(onToggle, &bToggle);
    bRead.attachPop(onRead,     &bRead);
    bReset.attachPop(onReset,   &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
