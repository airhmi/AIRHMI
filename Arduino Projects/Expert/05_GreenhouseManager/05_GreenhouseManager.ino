/**
 * 05_GreenhouseManager  --  4 sayfali sera yonetimi
 *
 *   Screen1 Status   : sicaklik, nem, toprak nemi, isik (sensor okumalari)
 *   Screen2 Control  : fan / heat / water / light manuel toggle + auto
 *   Screen3 Program  : haftalik sulama saati slider
 *   Screen4 Trend    : 24h sicaklik grafigi
 *
 * Donanim:
 *   DHT22  -> D2 (sicaklik+nem)
 *   Soil   -> A0 (toprak nem analog)
 *   LDR    -> A1 (isik analog)
 *   Fan    -> Panel GPIO_0
 *   Heat   -> Panel GPIO_1
 *   Water  -> Panel GPIO_2
 *   Light  -> Panel GPIO_3
 *
 * Kutuphane: DHT sensor library
 */

#include <Airhmi.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

AirGpio gpio;

/* Status */
AirLabel lTemp = AirLabel("lTemp");
AirLabel lHumid = AirLabel("lHumid");
AirLabel lSoil = AirLabel("lSoil");
AirLabel lLight = AirLabel("lLight");

/* Control */
AirButton bFan = AirButton("bFan");
AirButton bHeat = AirButton("bHeat");
AirButton bWater = AirButton("bWater");
AirButton bLight = AirButton("bLight");
AirButton bAuto = AirButton("bAuto");
AirLabel  lMode = AirLabel("lMode");

/* Program */
AirSlider sWaterHr = AirSlider("sWaterHr");
AirSlider sLightHr = AirSlider("sLightHr");
AirLabel  lWaterHr = AirLabel("lWaterHr");
AirLabel  lLightHr = AirLabel("lLightHr");

/* Trend */
AirGraph gph = AirGraph("gph");

bool relay[4] = { false, false, false, false };
bool autoMode = true;
uint8_t waterHr = 6, lightHr = 20;
unsigned long lastRead = 0;
unsigned long lastTrend = 0;

const uint32_t LED_OFF = 0xFF202020UL;
const uint32_t LED_ON  = 0xFF00C000UL;

void renderRelay(uint8_t i, AirButton *b)
{
    b->Set_background_color(relay[i] ? LED_ON : LED_OFF);
}

void setRelay(uint8_t i, bool on, AirButton *b)
{
    if (on == relay[i]) return;
    relay[i] = on;
    gpio.digital_write(i, on ? 1 : 0);
    renderRelay(i, b);
}

void onFan  (void *p) { autoMode = false; lMode.setText("MANUAL"); setRelay(0, !relay[0], &bFan);   }
void onHeat (void *p) { autoMode = false; lMode.setText("MANUAL"); setRelay(1, !relay[1], &bHeat);  }
void onWater(void *p) { autoMode = false; lMode.setText("MANUAL"); setRelay(2, !relay[2], &bWater); }
void onLight(void *p) { autoMode = false; lMode.setText("MANUAL"); setRelay(3, !relay[3], &bLight); }
void onAuto (void *p) { autoMode = !autoMode; lMode.setText(autoMode ? "AUTO" : "MANUAL"); }

void onWaterHr(void *p)
{
    uint32_t v = 0; sWaterHr.Get_Value(&v);
    if (v > 23) v = 23;
    waterHr = (uint8_t)v;
    char tmp[16]; snprintf(tmp, sizeof(tmp), "%02u:00", waterHr);
    lWaterHr.setText(tmp);
}

void onLightHr(void *p)
{
    uint32_t v = 0; sLightHr.Get_Value(&v);
    if (v > 23) v = 23;
    lightHr = (uint8_t)v;
    char tmp[16]; snprintf(tmp, sizeof(tmp), "%02u:00", lightHr);
    lLightHr.setText(tmp);
}

AirTouch *air_listen_list[] = {
    &bFan, &bHeat, &bWater, &bLight, &bAuto,
    &sWaterHr, &sLightHr,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    dht.begin();
    bFan.attachPop(onFan, &bFan);
    bHeat.attachPop(onHeat, &bHeat);
    bWater.attachPop(onWater, &bWater);
    bLight.attachPop(onLight, &bLight);
    bAuto.attachPop(onAuto, &bAuto);
    sWaterHr.attachPop(onWaterHr, &sWaterHr);
    sLightHr.attachPop(onLightHr, &sLightHr);

    sWaterHr.Set_Value(waterHr); onWaterHr(NULL);
    sLightHr.Set_Value(lightHr); onLightHr(NULL);
    lMode.setText("AUTO");
    renderRelay(0, &bFan); renderRelay(1, &bHeat);
    renderRelay(2, &bWater); renderRelay(3, &bLight);
    lTemp.setText("-- C"); lHumid.setText("-- %");
    lSoil.setText("-- %"); lLight.setText("--");
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastRead < 3000UL) return;
    lastRead = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();
    int   s = analogRead(A0);  /* 0..1023 */
    int   l = analogRead(A1);
    int soilPct = map(constrain(s, 0, 1023), 0, 1023, 0, 100);

    char tmp[16];
    if (!isnan(t)) { dtostrf(t, 0, 1, tmp); strcat(tmp, " C"); lTemp.setText(tmp); }
    if (!isnan(h)) { dtostrf(h, 0, 1, tmp); strcat(tmp, " %"); lHumid.setText(tmp); }
    snprintf(tmp, sizeof(tmp), "%d %%", soilPct); lSoil.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%d", l);          lLight.setText(tmp);

    /* trend graph her 30 sn */
    if (!isnan(t) && millis() - lastTrend > 30000UL)
    {
        lastTrend = millis();
        gph.addValue(0, (uint32_t)constrain(t, 0, 60));
    }

    /* auto mode kontrolu */
    if (autoMode)
    {
        if (!isnan(t)) {
            setRelay(0, t > 30.0f, &bFan);   /* fan */
            setRelay(1, t < 18.0f, &bHeat);  /* heat */
        }
        setRelay(2, soilPct < 30, &bWater);  /* water */
    }
}
