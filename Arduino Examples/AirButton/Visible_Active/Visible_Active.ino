/**
 * Visible_Active.ino  --  AirButton gorunurluk ve etkin/etkin-degil yonetimi
 *
 * Buton'un Visible ve Active ozelliklerini kontrol eder:
 *
 *   Visible: butonun ekranda gorulup gorulmemesi (1 = gorunur, 0 = gizli)
 *   Active : butonun dokunmaya yanit verip vermemesi (1 = etkin, 0 = pasif)
 *
 *   SET:
 *     - Set_visible(uint32_t)  : 0 ya da 1
 *     - Set_active(uint32_t)   : 0 ya da 1
 *
 *   GET:
 *     - Get_visible(uint32_t*)
 *     - Get_active(uint32_t*)
 *
 *   Panel sadece "1" / "0" / "TRUE" / "FALSE" string'lerini kabul eder.
 *
 * Panel tarafi (Visible_Active.ahi):
 *   bDemo                      : test butonu (gorunur/gizli, aktif/pasif)
 *   bShow / bHide              : Set_visible(1/0)
 *   bActivate / bDeactivate    : Set_active(1/0)
 *   bRead                      : Visible + Active okur
 *   bReset                     : Default (visible=1, active=1)
 *   lVisible / lActive         : Get sonuclari
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirButton bDemo        = AirButton("bDemo");
AirButton bShow        = AirButton("bShow");
AirButton bHide        = AirButton("bHide");
AirButton bActivate    = AirButton("bActivate");
AirButton bDeactivate  = AirButton("bDeactivate");
AirButton bRead        = AirButton("bRead");
AirButton bReset       = AirButton("bReset");

AirLabel  lVisible     = AirLabel("lVisible");
AirLabel  lActive      = AirLabel("lActive");

/* ---------- callback'ler ---------- */
void onShow(void *ptr)        { bDemo.Set_visible(1); }
void onHide(void *ptr)        { bDemo.Set_visible(0); }
void onActivate(void *ptr)    { bDemo.Set_active(1);  }
void onDeactivate(void *ptr)  { bDemo.Set_active(0);  }

void onRead(void *ptr)
{
    char tmp[8];
    uint32_t v = 0;

    bDemo.Get_visible(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lVisible.setText(tmp);

    bDemo.Get_active(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lActive.setText(tmp);
}

void onReset(void *ptr)
{
    bDemo.Set_visible(1);
    bDemo.Set_active(1);
    lVisible.setText("---");
    lActive.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bShow, &bHide,
    &bActivate, &bDeactivate,
    &bRead, &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bShow.attachPop(onShow,             &bShow);
    bHide.attachPop(onHide,             &bHide);
    bActivate.attachPop(onActivate,     &bActivate);
    bDeactivate.attachPop(onDeactivate, &bDeactivate);
    bRead.attachPop(onRead,             &bRead);
    bReset.attachPop(onReset,           &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
