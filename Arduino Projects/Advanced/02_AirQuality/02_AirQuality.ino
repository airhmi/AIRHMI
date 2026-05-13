/**
 * 02_AirQuality  --  Hava kalitesi monitoru (MQ135 / generic gas sensor)
 *
 *   MQ135 (veya benzeri analog gas sensor) Arduino A0'a baglanir,
 *   her 500 ms'de bir ham deger (0..1023) okunur, AirGauge ve AirGraph'e
 *   yazilir. Slider ile alarm esigi ayarlanir; deger esigi gecince
 *   AirShape rengi degisir ve AirBuzzer cinlar.
 *
 * Donanim:
 *   MQ135 VCC -> 5V, GND -> GND, A0 -> Arduino A0
 *   (Sensor uzun sicinma suresi ister, ham deger sabitlesinceye kadar
 *    bekleyin ~ birkac dakika)
 *
 * Panel tarafi (02_AirQuality.ahi):
 *   gPpm    : EveGauge (0..1023)
 *   lPpm    : ELabelBox -- ham deger
 *   lLimit  : ELabelBox -- "Limit: NN"
 *   sLimit  : EveSlider (alarm esigi)
 *   bAlarm  : EButton (alarm aciklama gostergesi - kirmizi/yesil)
 *   gphTrend: EGraph (1 line)
 */

#include <Airhmi.h>

AirGauge   gPpm     = AirGauge("gPpm");
AirGraph   gphTrend = AirGraph("gphTrend");
AirLabel   lPpm     = AirLabel("lPpm");
AirLabel   lLimit   = AirLabel("lLimit");
AirSlider  sLimit   = AirSlider("sLimit");
AirButton  bAlarm   = AirButton("bAlarm");
AirBuzzer  buz      = AirBuzzer("buz");

const uint8_t MQ_PIN = A0;
const uint32_t COLOR_OK    = 0xFF008000UL;  /* yesil */
const uint32_t COLOR_ALARM = 0xFFFF0000UL;  /* kirmizi */

uint32_t alarmLimit = 600;
unsigned long lastRead = 0;
unsigned long lastBeep = 0;
bool inAlarm = false;

void onLimitChange(void *p)
{
    uint32_t v = 0;
    sLimit.Get_Value(&v);
    if (v < 50)   v = 50;
    if (v > 1000) v = 1000;
    alarmLimit = v;
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "Limit: %lu", (unsigned long)alarmLimit);
    lLimit.setText(tmp);
}

AirTouch *air_listen_list[] = { &sLimit, NULL };

void setup()
{
    Serial.begin(115200);
    airInit();
    pinMode(MQ_PIN, INPUT);
    sLimit.attachPop(onLimitChange, &sLimit);
    sLimit.Set_Value(alarmLimit);
    onLimitChange(NULL);
    bAlarm.Set_background_color(COLOR_OK);
    bAlarm.setText("OK");
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastRead >= 500UL)
    {
        lastRead = millis();
        int raw = analogRead(MQ_PIN);

        char tmp[16];
        snprintf(tmp, sizeof(tmp), "%d", raw);
        lPpm.setText(tmp);

        gPpm.Set_value((uint32_t)raw);
        gphTrend.addValue(0, (uint32_t)raw);

        bool nowAlarm = ((uint32_t)raw > alarmLimit);
        if (nowAlarm != inAlarm)
        {
            inAlarm = nowAlarm;
            bAlarm.Set_background_color(inAlarm ? COLOR_ALARM : COLOR_OK);
            bAlarm.setText(inAlarm ? "ALARM" : "OK");
        }
        if (inAlarm && (millis() - lastBeep >= 1500UL))
        {
            lastBeep = millis();
            buz.Set_Buzzer(120);
        }
    }
}
