/**
 * Basics.ino  --  AirTimer kontrolu (Arduino tarafindan)
 *
 *   setInterval(uint32_t ms)   : timer interval'ini ayarla
 *   getInterval(uint32_t *out) : mevcut interval'i oku
 *   enable()                   : timer'i baslat
 *   disable()                  : timer'i durdur
 *
 *   Tick olayi panel-side calisir (ETimer'in <event> picoc script'i):
 *   her tick'te `count` EVariable'i bir artar, `lCounter` ELabelBox'una
 *   yazilir. Arduino sadece interval/enable kontrol eder.
 *
 * Panel tarafi (Basics.ahi):
 *   tmr1                  : ETimer (default Interval=1000, Enable=False)
 *   count                 : EVariable (int, 0)
 *   lCounter              : ELabelBox -- her tick'te guncellenir
 *   lInterval             : ELabelBox -- getInterval sonucu yazilir
 *   bEnable / bDisable    : tmr1.enable() / tmr1.disable()
 *   b500 / b1000 / b2000  : 500 / 1000 / 2000 ms
 *   bRead                 : getInterval -> lInterval
 *   bResetCount           : count = 0  (panel'den VarSeti)
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirTimer  tmr1     = AirTimer("tmr1");
AirLabel  lInterval = AirLabel("lInterval");

AirButton bEnable     = AirButton("bEnable");
AirButton bDisable    = AirButton("bDisable");
AirButton b500        = AirButton("b500");
AirButton b1000       = AirButton("b1000");
AirButton b2000       = AirButton("b2000");
AirButton bRead       = AirButton("bRead");
AirButton bResetCount = AirButton("bResetCount");

/* ---------- callback'ler ---------- */
void onEnable(void *ptr)  { tmr1.enable();  }
void onDisable(void *ptr) { tmr1.disable(); }

void onSet500(void *ptr)  { tmr1.setInterval(500);  }
void onSet1000(void *ptr) { tmr1.setInterval(1000); }
void onSet2000(void *ptr) { tmr1.setInterval(2000); }

void onRead(void *ptr)
{
    char tmp[12];
    uint32_t v = 0;
    tmr1.getInterval(&v);
    snprintf(tmp, sizeof(tmp), "%lu ms", (unsigned long)v);
    lInterval.setText(tmp);
}

void onResetCount(void *ptr)
{
    /* Panel-side count EVariable'ini sifirla. AirVariable kullaniyoruz. */
    AirVariable cnt = AirVariable("count");
    cnt.VarSeti(0);
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bEnable, &bDisable,
    &b500, &b1000, &b2000,
    &bRead, &bResetCount,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bEnable.attachPop(onEnable,         &bEnable);
    bDisable.attachPop(onDisable,       &bDisable);
    b500.attachPop(onSet500,            &b500);
    b1000.attachPop(onSet1000,          &b1000);
    b2000.attachPop(onSet2000,          &b2000);
    bRead.attachPop(onRead,             &bRead);
    bResetCount.attachPop(onResetCount, &bResetCount);
}

void loop()
{
    airLoop(air_listen_list);
}
