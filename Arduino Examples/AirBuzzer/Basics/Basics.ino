/**
 * Basics.ino  --  AirBuzzer kontrolu
 *
 *   Set_Buzzer(uint32_t ms) : panel buzzer'ini "ms" milisaniye boyunca
 *                             cinlat. Tek argumanli; donanim seviyesinde
 *                             panel kart uzerindeki buzzer kullanilir.
 *
 * Panel tarafi (Basics.ahi):
 *   bShort  (50 ms)   - kisa tik
 *   bClick  (100 ms)  - klik
 *   bBeep   (250 ms)  - normal beep
 *   bMid    (500 ms)  - uzun beep
 *   bLong   (1000 ms) - bir saniye
 *
 * Not: panel donaniminda gercek buzzer cinlar; sim'de PC speaker beep'i
 * (BuzzerSet handler'ina eklendi) ile duyumsal geri donut alirsin.
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirBuzzer buz = AirBuzzer("buz");

AirButton bShort = AirButton("bShort");
AirButton bClick = AirButton("bClick");
AirButton bBeep  = AirButton("bBeep");
AirButton bMid   = AirButton("bMid");
AirButton bLong  = AirButton("bLong");

/* ---------- callback'ler ---------- */
void onShort(void *ptr) { buz.Set_Buzzer(50);   }
void onClick(void *ptr) { buz.Set_Buzzer(100);  }
void onBeep(void *ptr)  { buz.Set_Buzzer(250);  }
void onMid(void *ptr)   { buz.Set_Buzzer(500);  }
void onLong(void *ptr)  { buz.Set_Buzzer(1000); }

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bShort, &bClick, &bBeep, &bMid, &bLong,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bShort.attachPop(onShort, &bShort);
    bClick.attachPop(onClick, &bClick);
    bBeep.attachPop(onBeep,   &bBeep);
    bMid.attachPop(onMid,     &bMid);
    bLong.attachPop(onLong,   &bLong);
}

void loop()
{
    airLoop(air_listen_list);
}
