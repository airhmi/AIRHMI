/**
 * Visible.ino  --  AirGauge gorunurluk yonetimi (Visible)
 *
 *   Set_visible : 1 = goster, 0 = gizle
 *   Get_visible : panel'den o anki gorunurluk durumunu oku
 *
 * Panel tarafi (Visible.ahi):
 *   gDemo                 : test edilen ana gauge
 *   bShow / bHide         : Set_visible(1/0)
 *   bRead                 : Get_visible -> lVisible
 *   bReset                : Set_visible(1)
 *   lVisible              : Get sonucu (0 / 1)
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirGauge  gDemo    = AirGauge("gDemo");
AirLabel  lVisible = AirLabel("lVisible");

AirButton bShow  = AirButton("bShow");
AirButton bHide  = AirButton("bHide");
AirButton bRead  = AirButton("bRead");
AirButton bReset = AirButton("bReset");

/* ---------- callback'ler ---------- */
void onShow(void *ptr)  { gDemo.Set_visible(1); }
void onHide(void *ptr)  { gDemo.Set_visible(0); }

void onRead(void *ptr)
{
    char tmp[8];
    uint32_t v = 0;
    gDemo.Get_visible(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lVisible.setText(tmp);
}

void onReset(void *ptr)
{
    gDemo.Set_visible(1);
    lVisible.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bShow, &bHide,
    &bRead, &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bShow.attachPop(onShow,   &bShow);
    bHide.attachPop(onHide,   &bHide);
    bRead.attachPop(onRead,   &bRead);
    bReset.attachPop(onReset, &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
