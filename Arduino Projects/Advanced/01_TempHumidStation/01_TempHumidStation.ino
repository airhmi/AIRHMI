/**
 * 01_TempHumidStation  --  Sicaklik ve nem istasyonu (DHT22)
 *
 *   DHT22 sensoru her 2 saniyede bir okunur, sicaklik ve nem panel'deki
 *   AirGauge'lara, AirLabel'lara yazilir, AirGraph'e eklenir (canli
 *   trend). Min/max degerler de Arduino tarafinda tutulup Reset ile
 *   sifirlanabilir.
 *
 * Donanim:
 *   DHT22 VCC -> 5V, GND -> GND, DATA -> Arduino D2 (4.7K pull-up tavsiye)
 *
 * Kutuphane: "DHT sensor library by Adafruit"
 *           Sketch > Include Library > Manage Libraries > "DHT sensor library"
 *
 * Panel tarafi (01_TempHumidStation.ahi):
 *   gTemp / gHumid                : EveGauge x 2
 *   lTemp / lHumid                : ELabelBox x 2 ("XX.X C", "XX %")
 *   lTempMin / lTempMax           : ELabelBox x 2 (min/max sicaklik)
 *   gphTrend                      : EveGraph (2 line: temp + humid)
 *   bReset                        : EButton (min/max sifirla)
 */

#include <Airhmi.h>
#include <DHT.h>

#define DHTPIN  2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

AirGauge gTemp     = AirGauge("gTemp");
AirGauge gHumid    = AirGauge("gHumid");
AirLabel lTemp     = AirLabel("lTemp");
AirLabel lHumid    = AirLabel("lHumid");
AirLabel lTempMin  = AirLabel("lTempMin");
AirLabel lTempMax  = AirLabel("lTempMax");
AirGraph gphTrend  = AirGraph("gphTrend");

AirButton bReset   = AirButton("bReset");

unsigned long lastRead = 0;
float tMin = 999.0, tMax = -999.0;

void renderMinMax()
{
    char tmp[12];
    dtostrf(tMin, 0, 1, tmp); strcat(tmp, " C"); lTempMin.setText(tmp);
    dtostrf(tMax, 0, 1, tmp); strcat(tmp, " C"); lTempMax.setText(tmp);
}

void onReset(void *p)
{
    tMin = 999.0; tMax = -999.0;
    lTempMin.setText("--");
    lTempMax.setText("--");
}

AirTouch *air_listen_list[] = { &bReset, NULL };

void setup()
{
    Serial.begin(115200);
    airInit();
    dht.begin();
    bReset.attachPop(onReset, &bReset);
    lTemp.setText("-- C"); lHumid.setText("-- %");
    lTempMin.setText("--"); lTempMax.setText("--");
    gTemp.Set_value(0); gHumid.Set_value(0);
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastRead < 2000UL) return;
    lastRead = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (isnan(t) || isnan(h)) return;

    /* Etiketler */
    char tmp[12];
    dtostrf(t, 0, 1, tmp); strcat(tmp, " C"); lTemp.setText(tmp);
    dtostrf(h, 0, 1, tmp); strcat(tmp, " %"); lHumid.setText(tmp);

    /* Gauge: 0..500 = 0..50C, 0..1000 = 0..100% */
    gTemp.Set_value ((uint32_t)constrain(t, 0, 50)   * 10);
    gHumid.Set_value((uint32_t)constrain(h, 0, 100)  * 10);

    /* Min/Max */
    if (t < tMin) tMin = t;
    if (t > tMax) tMax = t;
    renderMinMax();

    /* Graph: 2 hat (0=temp, 1=humid) */
    gphTrend.addValue(0, (uint32_t)constrain(t, 0, 50));
    gphTrend.addValue(1, (uint32_t)constrain(h, 0, 100));
}
