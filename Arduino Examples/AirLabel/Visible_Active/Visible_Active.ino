/**
 * Visible_Active.ino  --  AirLabel gorunurluk ve etkinlik yonetimi
 *
 *   Visible: etiketin ekranda gorulup gorulmemesi (1 = gorunur, 0 = gizli)
 *   Active : etiketin dokunma olaylarina yanit verip vermemesi
 *
 *   SET:
 *     - Set_visible(uint32_t)
 *     - Set_active(uint32_t)
 *
 *   GET:
 *     - Get_visible(uint32_t*)
 *     - Get_active(uint32_t*)
 *
 *   Panel sadece "1"/"0"/"TRUE"/"FALSE" string'lerini kabul eder.
 *
 * Panel tarafi (Visible_Active.ahi):
 *   lDemo                     : test edilen ana etiket (ELabel)
 *   bShow/bHide               : Set_visible(1/0)
 *   bActivate/bDeactivate     : Set_active(1/0)
 *   bRead                     : Visible + Active okur
 *   bReset                    : Default (1, 1)
 *   lVisible/lActive          : Get sonuclari
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirLabel  lDemo       = AirLabel("lDemo");
AirLabel  lVisible    = AirLabel("lVisible");
AirLabel  lActive     = AirLabel("lActive");

AirButton bShow       = AirButton("bShow");
AirButton bHide       = AirButton("bHide");
AirButton bActivate   = AirButton("bActivate");
AirButton bDeactivate = AirButton("bDeactivate");
AirButton bRead       = AirButton("bRead");
AirButton bReset      = AirButton("bReset");

/* ---------- callback'ler ---------- */
void onShow(void *ptr)       { lDemo.Set_visible(1); }
void onHide(void *ptr)       { lDemo.Set_visible(0); }
void onActivate(void *ptr)   { lDemo.Set_active(1);  }
void onDeactivate(void *ptr) { lDemo.Set_active(0);  }

void onRead(void *ptr)
{
    char tmp[8];
    uint32_t v = 0;

    lDemo.Get_visible(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lVisible.setText(tmp);

    lDemo.Get_active(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lActive.setText(tmp);
}

void onReset(void *ptr)
{
    lDemo.Set_visible(1);
    lDemo.Set_active(1);
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
