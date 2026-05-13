/**
 * 03_VoltCurrentMonitor  --  INA219 ile voltaj/akim monitoru
 *
 *   INA219 I2C uzerinden bus voltaji (V) ve akim (mA) okur, 2 gauge ve
 *   2 etiket ile gosterir. Akim slider ile ayarlanan limiti gectiginde
 *   buzzer cinlar ve durum etiketi kirmiziya doner.
 *
 * Donanim:
 *   INA219 SCL -> A5, SDA -> A4, VCC -> 5V, GND -> GND
 *   Yuk baglantisi modul sema'sina gore (V+ -> source, V- -> load)
 *
 * Kutuphane: "Adafruit INA219" + "Adafruit BusIO"
 */

#include <Airhmi.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

AirGauge  gVolt    = AirGauge("gVolt");
AirGauge  gCurr    = AirGauge("gCurr");
AirLabel  lVolt    = AirLabel("lVolt");
AirLabel  lCurr    = AirLabel("lCurr");
AirLabel  lPower   = AirLabel("lPower");
AirLabel  lLimit   = AirLabel("lLimit");
AirSlider sLimit   = AirSlider("sLimit");
AirButton bAlarm   = AirButton("bAlarm");
AirBuzzer buz      = AirBuzzer("buz");

const uint32_t COLOR_OK    = 0xFF008000UL;
const uint32_t COLOR_ALARM = 0xFFFF0000UL;

uint32_t alarmLimitMa = 500;
unsigned long lastRead = 0;
unsigned long lastBeep = 0;
bool inAlarm = false;

void onLimitChange(void *p)
{
    uint32_t v = 0;
    sLimit.Get_Value(&v);
    alarmLimitMa = v;
    char tmp[20];
    snprintf(tmp, sizeof(tmp), "Limit: %lu mA", (unsigned long)alarmLimitMa);
    lLimit.setText(tmp);
}

AirTouch *air_listen_list[] = { &sLimit, NULL };

void setup()
{
    Serial.begin(115200);
    airInit();
    if (!ina219.begin())
    {
        lVolt.setText("INA219?");
    }
    sLimit.attachPop(onLimitChange, &sLimit);
    sLimit.Set_Value(alarmLimitMa);
    onLimitChange(NULL);
    bAlarm.Set_background_color(COLOR_OK);
    bAlarm.setText("OK");
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastRead < 500UL) return;
    lastRead = millis();

    float bv  = ina219.getBusVoltage_V();
    float ma  = ina219.getCurrent_mA();
    if (ma < 0) ma = 0;
    float pwr = bv * ma;       /* mW */

    char tmp[20];
    dtostrf(bv, 0, 2, tmp); strcat(tmp, " V");   lVolt.setText(tmp);
    dtostrf(ma, 0, 1, tmp); strcat(tmp, " mA");  lCurr.setText(tmp);
    dtostrf(pwr, 0, 0, tmp); strcat(tmp, " mW"); lPower.setText(tmp);

    /* gVolt 0..3000 = 0..30V (x100), gCurr 0..2000 mA (raw mA) */
    gVolt.Set_value((uint32_t)constrain(bv * 100.0f, 0.0f, 3000.0f));
    gCurr.Set_value((uint32_t)constrain(ma,        0.0f, 2000.0f));

    bool nowAlarm = ((uint32_t)ma > alarmLimitMa);
    if (nowAlarm != inAlarm)
    {
        inAlarm = nowAlarm;
        bAlarm.Set_background_color(inAlarm ? COLOR_ALARM : COLOR_OK);
        bAlarm.setText(inAlarm ? "OVER" : "OK");
    }
    if (inAlarm && (millis() - lastBeep >= 1500UL))
    {
        lastBeep = millis();
        buz.Set_Buzzer(150);
    }
}
