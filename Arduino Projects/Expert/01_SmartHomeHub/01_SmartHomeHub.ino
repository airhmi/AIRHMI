/**
 * 01_SmartHomeHub  --  5-sayfali akilli ev kontrol paneli
 *
 *   5 ekran: Dashboard, Aydinlatma, Klima, Guvenlik, Ayarlar
 *
 *   Sayfalar arasi gecisin tetiklenmesi panel-side `ChangeScreenSet(ScreenN)`
 *   script'leri ile yapilir; Arduino tarafindan komut gonderilmez. Arduino
 *   sadece her sayfanin "veri" butonlarinin event'lerini dinler ve ilgili
 *   state'i guncelleyip panel etiketlerine yazar.
 *
 * Sayfa - Komponent eslesmesi:
 *   Screen1 (Dashboard)
 *     lTime           : panel-side veya RTC ile dolduruluyor
 *     lTemp           : ev sicakligi (mock)
 *     lHomeStatus     : "Open" / "Secured" (Guvenlik state'inden gelir)
 *
 *   Screen2 (Aydinlatma)
 *     bL1..bL4        : 4 oda toggle (renkli gosterge dahil)
 *     sBright         : global parlaklik 0..100 (PWM_0)
 *     bApplyBright    : slider degerini PWM'e uygula
 *
 *   Screen3 (Klima)
 *     sTarget         : hedef sicaklik 16..30
 *     lTarget         : "XX C"
 *     bModeHeat / Cool / Off
 *     lMode
 *
 *   Screen4 (Guvenlik)
 *     bArmDisarm      : ARM <-> DISARM toggle
 *     lArmStatus
 *
 *   Screen5 (Ayarlar)
 *     bSetClock       : panel RTC'sini 12:00'a ayarla
 */

#include <Airhmi.h>

/* Aydinlatma */
AirButton bL1 = AirButton("bL1");
AirButton bL2 = AirButton("bL2");
AirButton bL3 = AirButton("bL3");
AirButton bL4 = AirButton("bL4");
AirSlider sBright = AirSlider("sBright");
AirButton bApplyBright = AirButton("bApplyBright");

/* Klima */
AirSlider sTarget = AirSlider("sTarget");
AirLabel  lTarget = AirLabel("lTarget");
AirButton bModeHeat = AirButton("bModeHeat");
AirButton bModeCool = AirButton("bModeCool");
AirButton bModeOff  = AirButton("bModeOff");
AirLabel  lMode     = AirLabel("lMode");

/* Guvenlik */
AirButton bArmDisarm = AirButton("bArmDisarm");
AirLabel  lArmStatus = AirLabel("lArmStatus");

/* Ayarlar */
AirButton bSetClock = AirButton("bSetClock");

/* Dashboard etiketleri (sadece yazilir) */
AirLabel lTemp       = AirLabel("lTemp");
AirLabel lHomeStatus = AirLabel("lHomeStatus");

AirGpio gpio;
AirRtc  rtc;

const uint32_t LED_OFF = 0xFF202020UL;
const uint32_t LED_ON  = 0xFFFFCC00UL;

bool    lamp[4]   = { false, false, false, false };
uint8_t bright    = 50;
int8_t  target    = 22;
char    hvacMode  = 'O';
bool    armed     = false;

void renderLamp(uint8_t i)
{
    AirButton *b = (i==0)?&bL1:(i==1)?&bL2:(i==2)?&bL3:&bL4;
    b->Set_background_color(lamp[i] ? LED_ON : LED_OFF);
}

void renderHomeStatus()
{
    lHomeStatus.setText(armed ? "Secured" : "Open");
}

void onL1(void *p) { lamp[0] = !lamp[0]; renderLamp(0); }
void onL2(void *p) { lamp[1] = !lamp[1]; renderLamp(1); }
void onL3(void *p) { lamp[2] = !lamp[2]; renderLamp(2); }
void onL4(void *p) { lamp[3] = !lamp[3]; renderLamp(3); }

void onApplyBright(void *p)
{
    uint32_t v = 0;
    sBright.Get_Value(&v);
    if (v > 100) v = 100;
    bright = (uint8_t)v;
    gpio.set_pwmfreq(0, 5000, bright);
}

void onTargetChange(void *p)
{
    uint32_t v = 0;
    sTarget.Get_Value(&v);
    if (v < 16) v = 16;
    if (v > 30) v = 30;
    target = (int8_t)v;
    char tmp[12];
    snprintf(tmp, sizeof(tmp), "%d C", target);
    lTarget.setText(tmp);
}

void renderMode()
{
    if      (hvacMode == 'H') lMode.setText("HEAT");
    else if (hvacMode == 'C') lMode.setText("COOL");
    else                      lMode.setText("OFF");
}

void onModeHeat(void *p) { hvacMode = 'H'; renderMode(); }
void onModeCool(void *p) { hvacMode = 'C'; renderMode(); }
void onModeOff (void *p) { hvacMode = 'O'; renderMode(); }

void onArmDisarm(void *p)
{
    armed = !armed;
    lArmStatus.setText(armed ? "ARMED" : "DISARMED");
    bArmDisarm.Set_background_color(armed ? 0xFFC00000UL : 0xFF008000UL);
    bArmDisarm.setText(armed ? "Disarm" : "Arm");
    renderHomeStatus();
}

void onSetClock(void *p)
{
    rtc.timeSet(12, 0);
    rtc.dateSet(9, 5, 2026);
}

AirTouch *air_listen_list[] = {
    &bL1, &bL2, &bL3, &bL4, &bApplyBright,
    &sTarget, &bModeHeat, &bModeCool, &bModeOff,
    &bArmDisarm,
    &bSetClock,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bL1.attachPop(onL1, &bL1); bL2.attachPop(onL2, &bL2);
    bL3.attachPop(onL3, &bL3); bL4.attachPop(onL4, &bL4);
    bApplyBright.attachPop(onApplyBright, &bApplyBright);
    sTarget.attachPop(onTargetChange,     &sTarget);
    bModeHeat.attachPop(onModeHeat,       &bModeHeat);
    bModeCool.attachPop(onModeCool,       &bModeCool);
    bModeOff.attachPop(onModeOff,         &bModeOff);
    bArmDisarm.attachPop(onArmDisarm,     &bArmDisarm);
    bSetClock.attachPop(onSetClock,       &bSetClock);

    /* baslangic state */
    for (uint8_t i = 0; i < 4; i++) renderLamp(i);
    sBright.Set_Value(bright);
    sTarget.Set_Value(target);
    char tmp[12]; snprintf(tmp, sizeof(tmp), "%d C", target); lTarget.setText(tmp);
    renderMode();
    lArmStatus.setText("DISARMED");
    bArmDisarm.Set_background_color(0xFF008000UL);
    bArmDisarm.setText("Arm");
    lTemp.setText("22.5 C");
    renderHomeStatus();
}

void loop()
{
    airLoop(air_listen_list);
}
