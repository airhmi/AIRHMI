/**
 * 08_DigitalClock  --  Dijital saat
 *
 *   Arduino bir baslangic saat ve tarihini AirRtc'ye `dateSet`/`timeSet`
 *   ile gonderir, ayrica kendi state'inde tutar. Her saniyede saat
 *   inkrement edilip lTime "HH:MM:SS" ve lDate "DD/MM/YYYY" etiketlerine
 *   yazilir.
 *
 *   AirRtc Arduino API'sinde Get karsiligi yok; panel'in kendi RTC chip'i
 *   ile uyumun sorumlulugu kullaniciya birakilir (panel'de daha sonra
 *   panel-side script ile RTC okunabilir).
 *
 * Panel tarafi (DigitalClock.ahi):
 *   lTime / lDate           : ELabelBox -- saat / tarih
 *   bSetNoon / bSetMidnight : preset saat kurma
 *   bSetToday               : derleme tarihiyle "now" yaklasimi
 */

#include <Airhmi.h>

AirRtc rtc;
AirLabel  lTime = AirLabel("lTime");
AirLabel  lDate = AirLabel("lDate");
AirButton bSetNoon     = AirButton("bSetNoon");
AirButton bSetMidnight = AirButton("bSetMidnight");
AirButton bSetToday    = AirButton("bSetToday");

struct Clock {
    int h, m, s;
    int day, month, year;
};
Clock clk = {12, 0, 0, 8, 5, 2026};
unsigned long lastSecMs = 0;
bool started = false;

const uint8_t daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};

bool isLeap(int y){ return (y%4==0 && y%100!=0) || (y%400==0); }

uint8_t daysIn(int m, int y)
{
    if (m == 2 && isLeap(y)) return 29;
    if (m >= 1 && m <= 12) return daysInMonth[m - 1];
    return 30;
}

void render()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%02d:%02d:%02d", clk.h, clk.m, clk.s);
    lTime.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%02d/%02d/%04d", clk.day, clk.month, clk.year);
    lDate.setText(tmp);
}

void onSetNoon(void *p)
{
    clk.h = 12; clk.m = 0; clk.s = 0;
    rtc.timeSet(clk.h, clk.m);
    lastSecMs = millis(); started = true;
    render();
}

void onSetMidnight(void *p)
{
    clk.h = 0; clk.m = 0; clk.s = 0;
    rtc.timeSet(clk.h, clk.m);
    lastSecMs = millis(); started = true;
    render();
}

void onSetToday(void *p)
{
    /* Derleme tarihinden basit "now" tahmini. */
    clk.day = 8; clk.month = 5; clk.year = 2026;
    clk.h = 9;  clk.m = 30; clk.s = 0;
    rtc.dateSet(clk.day, clk.month, clk.year);
    rtc.timeSet(clk.h, clk.m);
    lastSecMs = millis(); started = true;
    render();
}

AirTouch *air_listen_list[] = { &bSetNoon, &bSetMidnight, &bSetToday, NULL };

void setup()
{
    Serial.begin(115200);
    airInit();
    bSetNoon.attachPop(onSetNoon,         &bSetNoon);
    bSetMidnight.attachPop(onSetMidnight, &bSetMidnight);
    bSetToday.attachPop(onSetToday,       &bSetToday);
    render();
}

void loop()
{
    airLoop(air_listen_list);

    if (started && millis() - lastSecMs >= 1000UL)
    {
        lastSecMs += 1000UL;
        if (++clk.s >= 60) {
            clk.s = 0;
            if (++clk.m >= 60) {
                clk.m = 0;
                if (++clk.h >= 24) {
                    clk.h = 0;
                    if (++clk.day > daysIn(clk.month, clk.year)) {
                        clk.day = 1;
                        if (++clk.month > 12) {
                            clk.month = 1;
                            clk.year++;
                        }
                    }
                }
            }
        }
        render();
    }
}
