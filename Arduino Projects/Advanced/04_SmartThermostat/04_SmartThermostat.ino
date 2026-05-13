/**
 * 04_SmartThermostat  --  Akilli termostat (DS18B20 + roleli isitma kontrolu)
 *
 *   DS18B20 sensor sicakligi her 2 sn'de bir okur. Slider ile hedef
 *   sicaklik (10..30 C) ayarlanir. Hysteresis ile rolenin (GPIO_0)
 *   acilip kapanmasi:
 *      Anlik < Hedef - 0.5  -> ROLE ON  (isitma)
 *      Anlik > Hedef + 0.5  -> ROLE OFF
 *   Anlik > 35 C -> Buzzer alarm.
 *
 * Donanim:
 *   DS18B20 VCC -> 5V, GND -> GND, DATA -> D2 (4.7K pull-up)
 *   Role modulu IN -> AirHMI panel GPIO_0 cikisi (panel kart datasheet)
 *
 * Kutuphane: OneWire + DallasTemperature
 */

#include <Airhmi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define DS_PIN 2
OneWire ow(DS_PIN);
DallasTemperature ds(&ow);

AirGauge  gTemp   = AirGauge("gTemp");
AirLabel  lTemp   = AirLabel("lTemp");
AirLabel  lTarget = AirLabel("lTarget");
AirSlider sTarget = AirSlider("sTarget");
AirButton bRelay  = AirButton("bRelay");
AirGpio   gpio;
AirBuzzer buz     = AirBuzzer("buz");

const uint32_t COLOR_OFF   = 0xFF202020UL;
const uint32_t COLOR_HEAT  = 0xFFFF6000UL;
const float    HYSTERESIS  = 0.5f;
const float    OVERHEAT    = 35.0f;

float    target = 22.0f;
bool     relayOn = false;
unsigned long lastRead = 0;
unsigned long lastBeep = 0;

void renderTarget()
{
    char tmp[16];
    dtostrf(target, 0, 1, tmp); strcat(tmp, " C");
    lTarget.setText(tmp);
}

void setRelay(bool on)
{
    if (on == relayOn) return;
    relayOn = on;
    gpio.digital_write(0, on ? 1 : 0);
    bRelay.Set_background_color(on ? COLOR_HEAT : COLOR_OFF);
    bRelay.setText(on ? "HEAT ON" : "OFF");
}

void onTargetChange(void *p)
{
    uint32_t v = 0;
    sTarget.Get_Value(&v);
    if (v < 100) v = 100;
    if (v > 300) v = 300;
    target = (float)v / 10.0f;        /* 10.0..30.0 */
    renderTarget();
}

AirTouch *air_listen_list[] = { &sTarget, NULL };

void setup()
{
    Serial.begin(115200);
    airInit();
    ds.begin();
    sTarget.attachPop(onTargetChange, &sTarget);
    sTarget.Set_Value((uint32_t)(target * 10));
    renderTarget();
    setRelay(false);
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastRead < 2000UL) return;
    lastRead = millis();

    ds.requestTemperatures();
    float t = ds.getTempCByIndex(0);
    if (t < -50 || t > 100) return;     /* sensor fail */

    char tmp[16];
    dtostrf(t, 0, 1, tmp); strcat(tmp, " C");
    lTemp.setText(tmp);
    gTemp.Set_value((uint32_t)constrain(t * 10.0f, 0.0f, 500.0f));

    /* Hysteresis */
    if (!relayOn && t < target - HYSTERESIS)      setRelay(true);
    else if (relayOn && t > target + HYSTERESIS)  setRelay(false);

    /* Overheat alarm */
    if (t > OVERHEAT && (millis() - lastBeep > 2000UL))
    {
        lastBeep = millis();
        buz.Set_Buzzer(300);
    }
}
