/**
 * 03_Countdown  --  Geri sayim sayaci
 *
 *   sSec slider'iyla 1..60 sn arasi sure secilir.
 *   bStart basinca geri sayim baslar; her saniyede lTime "MM:SS" yazar.
 *   Son 3 sn'de buzzer kisa beep, 0'da uzun beep + reset.
 *   bStop basinca geri sayim durur ve etiket "00:00"'a duser.
 *
 * Panel tarafi (Countdown.ahi):
 *   sSec      : ESlider (1..60)
 *   bStart    : EButton
 *   bStop     : EButton
 *   lTime     : ELabelBox -- "MM:SS"
 *   lInfo     : ELabelBox -- secilen sure
 */

#include <Airhmi.h>

AirSlider sSec   = AirSlider("sSec");
AirButton bStart = AirButton("bStart");
AirButton bStop  = AirButton("bStop");
AirLabel  lTime  = AirLabel("lTime");
AirLabel  lInfo  = AirLabel("lInfo");

AirBuzzer buz = AirBuzzer("buz");

bool running = false;
int  remainSec = 0;
unsigned long lastTickMs = 0;

void renderTime(int sec)
{
    char tmp[8];
    int mm = sec / 60;
    int ss = sec % 60;
    snprintf(tmp, sizeof(tmp), "%02d:%02d", mm, ss);
    lTime.setText(tmp);
}

void onStart(void *p)
{
    if (running) return;
    uint32_t v = 0;
    sSec.Get_Value(&v);
    if (v < 1)  v = 1;
    if (v > 60) v = 60;
    remainSec = (int)v;
    running   = true;
    lastTickMs = millis();
    renderTime(remainSec);

    char tmp[24];
    snprintf(tmp, sizeof(tmp), "Started %d s", remainSec);
    lInfo.setText(tmp);
}

void onStop(void *p)
{
    running   = false;
    remainSec = 0;
    renderTime(0);
    lInfo.setText("Stopped");
}

AirTouch *air_listen_list[] = { &bStart, &bStop, NULL };

void setup()
{
    Serial.begin(115200);
    airInit();

    bStart.attachPop(onStart, &bStart);
    bStop.attachPop(onStop,   &bStop);

    renderTime(0);
    lInfo.setText("Idle");
}

void loop()
{
    airLoop(air_listen_list);

    if (running && (millis() - lastTickMs >= 1000UL))
    {
        lastTickMs += 1000UL;
        remainSec--;
        renderTime(remainSec);

        if (remainSec > 0 && remainSec <= 3)
        {
            buz.Set_Buzzer(80);    /* son 3 sn kisa tik */
        }
        else if (remainSec == 0)
        {
            buz.Set_Buzzer(800);   /* bitis */
            running = false;
            lInfo.setText("Done!");
        }
    }
}
