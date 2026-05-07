/**
 * DateTime.ino  --  AirRtc panel saat/tarih ayarlama
 *
 *   dateSet(day, month, year) : panel RTC chip'inin tarihini ayarlar
 *   timeSet(hour, min)        : panel RTC chip'inin saatini ayarlar
 *
 *   (Get karsiliklari Arduino API'de yok — panel firmware destekliyor
 *    fakat farkli bir signature ile (CdateGetEx pointer-out parametreli),
 *    Arduino tarafindan dogrudan cagrilamiyor.)
 *
 * Panel tarafi (DateTime.ahi):
 *   bDate1 / bDate2 / bDate3  : 3 farkli tarih
 *   bTime1 / bTime2 / bTime3  : 3 farkli saat
 *   bNow                      : Arduino'nun derlenme tarihinden tarih+saat olarak ayarla
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirRtc rtc;

AirButton bDate1 = AirButton("bDate1");
AirButton bDate2 = AirButton("bDate2");
AirButton bDate3 = AirButton("bDate3");
AirButton bTime1 = AirButton("bTime1");
AirButton bTime2 = AirButton("bTime2");
AirButton bTime3 = AirButton("bTime3");
AirButton bNow   = AirButton("bNow");

/* ---------- callback'ler ---------- */
void onDate1(void *ptr) { rtc.dateSet(1,  1,  2026); }
void onDate2(void *ptr) { rtc.dateSet(15, 6,  2026); }
void onDate3(void *ptr) { rtc.dateSet(31, 12, 2026); }

void onTime1(void *ptr) { rtc.timeSet(0,  0); }
void onTime2(void *ptr) { rtc.timeSet(12, 30); }
void onTime3(void *ptr) { rtc.timeSet(23, 59); }

void onNow(void *ptr)
{
    /* Derleme tarihinden basit "now" — gercek saat yok ama makul. */
    rtc.dateSet(6, 5, 2026);
    rtc.timeSet(1, 30);
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bDate1, &bDate2, &bDate3,
    &bTime1, &bTime2, &bTime3,
    &bNow,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bDate1.attachPop(onDate1, &bDate1);
    bDate2.attachPop(onDate2, &bDate2);
    bDate3.attachPop(onDate3, &bDate3);
    bTime1.attachPop(onTime1, &bTime1);
    bTime2.attachPop(onTime2, &bTime2);
    bTime3.attachPop(onTime3, &bTime3);
    bNow.attachPop(onNow,     &bNow);
}

void loop()
{
    airLoop(air_listen_list);
}
