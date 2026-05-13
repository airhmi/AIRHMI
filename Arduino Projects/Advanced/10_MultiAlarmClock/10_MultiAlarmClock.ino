/**
 * 10_MultiAlarmClock  --  5 alarm slotlu saat
 *
 *   Arduino kendi saatini millis bazli tutar, panel AirRtc'ye periyodik
 *   olarak yazilir. 5 alarm slot her biri (HH:MM) icin onceden tanimli;
 *   toggle butonlari ile her slot aktif/pasif yapilabilir. Alarm saati
 *   geldiginde + dakika eslesince + slot aktifse buzzer 2 sn cinlar ve
 *   "RING!" status'a yazilir.
 *
 *   Snooze butonu tum alarm tetiklenmelerini "ringing" durumdan cikarir.
 */

#include <Airhmi.h>

AirRtc    rtc;
AirBuzzer buz = AirBuzzer("buz");

AirLabel  lTime    = AirLabel("lTime");
AirLabel  lStatus  = AirLabel("lStatus");

AirButton bSlot0 = AirButton("bSlot0");
AirButton bSlot1 = AirButton("bSlot1");
AirButton bSlot2 = AirButton("bSlot2");
AirButton bSlot3 = AirButton("bSlot3");
AirButton bSlot4 = AirButton("bSlot4");
AirButton bSetMorning = AirButton("bSetMorning");
AirButton bSnooze     = AirButton("bSnooze");

const uint8_t SLOT_H[5] = {  7,  8, 12, 18, 22 };
const uint8_t SLOT_M[5] = {  0, 30,  0,  0,  0 };
bool slotEnabled[5] = { true, true, true, true, true };

const uint32_t COLOR_OFF = 0xFF202020UL;
const uint32_t COLOR_ON  = 0xFF00C000UL;

int hh = 6, mm = 55, ss = 0;
unsigned long lastSecMs = 0;
bool ringing = false;
unsigned long ringStart = 0;
int8_t lastRangSlot = -1;
int    lastRangMin = -1;

void renderTime()
{
    char tmp[12];
    snprintf(tmp, sizeof(tmp), "%02d:%02d:%02d", hh, mm, ss);
    lTime.setText(tmp);
}

void renderSlot(uint8_t i)
{
    AirButton *b = (i==0)?&bSlot0:(i==1)?&bSlot1:(i==2)?&bSlot2:(i==3)?&bSlot3:&bSlot4;
    char tmp[20];
    snprintf(tmp, sizeof(tmp), "%02u:%02u  %s",
             (unsigned)SLOT_H[i], (unsigned)SLOT_M[i],
             slotEnabled[i] ? "ON" : "OFF");
    b->setText(tmp);
    b->Set_background_color(slotEnabled[i] ? COLOR_ON : COLOR_OFF);
}

void onSlot(uint8_t i)
{
    slotEnabled[i] = !slotEnabled[i];
    renderSlot(i);
}

void on0(void *p) { onSlot(0); }
void on1(void *p) { onSlot(1); }
void on2(void *p) { onSlot(2); }
void on3(void *p) { onSlot(3); }
void on4(void *p) { onSlot(4); }

void onSetMorning(void *p)
{
    hh = 6; mm = 55; ss = 0;
    rtc.timeSet(hh, mm);
    lastSecMs = millis();
    renderTime();
}

void onSnooze(void *p)
{
    ringing = false;
    lStatus.setText("OK");
}

AirTouch *air_listen_list[] = {
    &bSlot0, &bSlot1, &bSlot2, &bSlot3, &bSlot4,
    &bSetMorning, &bSnooze, NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bSlot0.attachPop(on0, &bSlot0); bSlot1.attachPop(on1, &bSlot1);
    bSlot2.attachPop(on2, &bSlot2); bSlot3.attachPop(on3, &bSlot3);
    bSlot4.attachPop(on4, &bSlot4);
    bSetMorning.attachPop(onSetMorning, &bSetMorning);
    bSnooze.attachPop(onSnooze, &bSnooze);
    for (uint8_t i = 0; i < 5; i++) renderSlot(i);
    renderTime();
    lStatus.setText("OK");
    lastSecMs = millis();
}

void checkAlarms()
{
    int curMin = hh * 60 + mm;
    if (curMin == lastRangMin) return;       /* dakika ayni, tekrarlama */
    lastRangMin = curMin;
    for (uint8_t i = 0; i < 5; i++)
    {
        if (!slotEnabled[i]) continue;
        if (SLOT_H[i] == hh && SLOT_M[i] == mm)
        {
            ringing = true;
            ringStart = millis();
            lastRangSlot = i;
            char tmp[20];
            snprintf(tmp, sizeof(tmp), "RING! slot %u", (unsigned)i);
            lStatus.setText(tmp);
            buz.Set_Buzzer(2000);
            return;
        }
    }
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastSecMs >= 1000UL)
    {
        lastSecMs += 1000UL;
        if (++ss >= 60) { ss = 0; if (++mm >= 60) { mm = 0; if (++hh >= 24) hh = 0; } }
        renderTime();
        checkAlarms();
    }

    if (ringing && millis() - ringStart > 3000UL)
    {
        ringing = false;
        lStatus.setText("OK");
    }
}
