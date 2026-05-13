/**
 * 06_RelayScheduler  --  4-kanal role + RTC zamanlayici
 *
 *   4 role (GPIO_0..GPIO_3) saat bazli otomatik acilip kapanir, ya da
 *   manuel butondan toggle edilir. Auto modda her dakikada sketch
 *   kontrol eder ve gerekiyorsa role state'ini gunceller.
 *
 *   Saat Arduino tarafinda millis bazli tutulur (yaklasık RTC); panel
 *   AirRtc ile baslangıçta senkronize edilir (Set Time ile).
 *
 * Otomatik program (sabit, sketch icinde):
 *   Role 0: 06:00 ON  -> 22:00 OFF
 *   Role 1: 18:00 ON  -> 23:30 OFF (akşam ışığı)
 *   Role 2: 12:00 ON  -> 14:00 OFF (öğle pompası)
 *   Role 3: 20:00 ON  -> 23:00 OFF
 */

#include <Airhmi.h>

AirGpio   gpio;
AirRtc    rtc;

AirButton bR0 = AirButton("bR0");
AirButton bR1 = AirButton("bR1");
AirButton bR2 = AirButton("bR2");
AirButton bR3 = AirButton("bR3");

AirButton dR0 = AirButton("dR0");
AirButton dR1 = AirButton("dR1");
AirButton dR2 = AirButton("dR2");
AirButton dR3 = AirButton("dR3");

AirButton bAuto = AirButton("bAuto");
AirButton bSetNoon = AirButton("bSetNoon");
AirLabel  lTime = AirLabel("lTime");
AirLabel  lMode = AirLabel("lMode");

const uint32_t LED_OFF = 0xFF202020UL;
const uint32_t LED_ON  = 0xFF00C000UL;

bool relay[4] = {false, false, false, false};
bool autoMode = true;

/* program saatleri (saat-cinsinden) */
const uint8_t prog_on [4] = {  6, 18, 12, 20 };
const uint8_t prog_off[4] = { 22, 23, 14, 23 };

int hh = 12, mm = 0, ss = 0;
unsigned long lastSecMs = 0;

void renderTime()
{
    char tmp[12];
    snprintf(tmp, sizeof(tmp), "%02d:%02d:%02d", hh, mm, ss);
    lTime.setText(tmp);
}

void renderMode() { lMode.setText(autoMode ? "AUTO" : "MANUAL"); }

void applyRelay(uint8_t i, bool on)
{
    relay[i] = on;
    AirButton *led = (i==0)?&dR0:(i==1)?&dR1:(i==2)?&dR2:&dR3;
    led->Set_background_color(on ? LED_ON : LED_OFF);
    gpio.digital_write(i, on ? 1 : 0);
}

void onR0(void *p) { autoMode = false; renderMode(); applyRelay(0, !relay[0]); }
void onR1(void *p) { autoMode = false; renderMode(); applyRelay(1, !relay[1]); }
void onR2(void *p) { autoMode = false; renderMode(); applyRelay(2, !relay[2]); }
void onR3(void *p) { autoMode = false; renderMode(); applyRelay(3, !relay[3]); }

void onAuto(void *p) { autoMode = !autoMode; renderMode(); }

void onSetNoon(void *p)
{
    hh = 12; mm = 0; ss = 0;
    rtc.timeSet(hh, mm);
    lastSecMs = millis();
    renderTime();
}

void scheduleTick()
{
    if (!autoMode) return;
    for (uint8_t i = 0; i < 4; i++)
    {
        bool shouldBe;
        if (prog_on[i] < prog_off[i])
            shouldBe = (hh >= prog_on[i] && hh < prog_off[i]);
        else
            shouldBe = (hh >= prog_on[i] || hh < prog_off[i]);
        if (shouldBe != relay[i]) applyRelay(i, shouldBe);
    }
}

AirTouch *air_listen_list[] = { &bR0, &bR1, &bR2, &bR3, &bAuto, &bSetNoon, NULL };

void setup()
{
    Serial.begin(115200);
    airInit();
    bR0.attachPop(onR0, &bR0); bR1.attachPop(onR1, &bR1);
    bR2.attachPop(onR2, &bR2); bR3.attachPop(onR3, &bR3);
    bAuto.attachPop(onAuto, &bAuto);
    bSetNoon.attachPop(onSetNoon, &bSetNoon);
    for (uint8_t i = 0; i < 4; i++) applyRelay(i, false);
    renderTime(); renderMode();
    lastSecMs = millis();
    scheduleTick();
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastSecMs >= 1000UL)
    {
        lastSecMs += 1000UL;
        if (++ss >= 60) { ss = 0; if (++mm >= 60) { mm = 0; if (++hh >= 24) hh = 0; }
            scheduleTick();   /* dakika basinda program kontrol */
        }
        renderTime();
    }
}
