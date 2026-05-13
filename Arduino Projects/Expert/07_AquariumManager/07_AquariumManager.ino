/**
 * 07_AquariumManager  --  4 sayfali akvaryum yonetimi
 *
 *   Screen1 Status   : su sicakligi (DS18B20), pH (analog A0), light state
 *   Screen2 Feeding  : Feed Now + 3 ogun saati slider (8/14/20)
 *   Screen3 Lighting : aciklik slider (0..100 PWM_0), gun/gece toggle
 *   Screen4 Settings : low/high temp limit slider + calibrate pH
 *
 *   Donanim:
 *     DS18B20  -> D2
 *     pH analog -> A0
 *     Light PWM -> Panel PWM_0
 *     Filter relay -> Panel GPIO_1
 *     Feeder servo -> D9
 */

#include <Airhmi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>

#define DS_PIN 2
OneWire ow(DS_PIN);
DallasTemperature ds(&ow);
Servo feeder;
const uint8_t FEEDER_PIN = 9;

AirGpio gpio;

/* Status */
AirLabel lTemp = AirLabel("lTemp");
AirLabel lPh   = AirLabel("lPh");
AirLabel lLight = AirLabel("lLight");
AirButton bFilter = AirButton("bFilter");

/* Feeding */
AirButton bFeedNow = AirButton("bFeedNow");
AirSlider sMeal1 = AirSlider("sMeal1");
AirSlider sMeal2 = AirSlider("sMeal2");
AirSlider sMeal3 = AirSlider("sMeal3");
AirLabel  lMeal1 = AirLabel("lMeal1");
AirLabel  lMeal2 = AirLabel("lMeal2");
AirLabel  lMeal3 = AirLabel("lMeal3");

/* Lighting */
AirSlider sBright = AirSlider("sBright");
AirButton bDay = AirButton("bDay");
AirButton bNight = AirButton("bNight");
AirLabel  lBright = AirLabel("lBright");

/* Settings */
AirSlider sLow = AirSlider("sLow");
AirSlider sHigh = AirSlider("sHigh");
AirLabel  lLow = AirLabel("lLow");
AirLabel  lHigh = AirLabel("lHigh");
AirButton bCalPh = AirButton("bCalPh");

bool filterOn = true;
uint8_t mealHr[3] = { 8, 14, 20 };
uint8_t bright = 80;
bool dayMode = true;
int8_t lowLim = 22, highLim = 28;
float phOffset = 0.0f;
unsigned long lastRead = 0;

const uint32_t LED_OFF = 0xFF202020UL;
const uint32_t LED_ON  = 0xFF008000UL;

void renderFilter()
{
    bFilter.Set_background_color(filterOn ? LED_ON : LED_OFF);
    bFilter.setText(filterOn ? "Filter ON" : "Filter OFF");
    gpio.digital_write(1, filterOn ? 1 : 0);
}

void renderMeals()
{
    char tmp[12];
    AirLabel *l[3] = { &lMeal1, &lMeal2, &lMeal3 };
    for (uint8_t i = 0; i < 3; i++)
    {
        snprintf(tmp, sizeof(tmp), "%02u:00", mealHr[i]);
        l[i]->setText(tmp);
    }
}

void renderBright()
{
    char tmp[12];
    snprintf(tmp, sizeof(tmp), "%u %%", bright);
    lBright.setText(tmp);
    gpio.set_pwmfreq(0, 5000, bright);
}

void renderLimits()
{
    char tmp[12];
    snprintf(tmp, sizeof(tmp), "%d C", lowLim);  lLow.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%d C", highLim); lHigh.setText(tmp);
}

void onFeed(void *p)
{
    feeder.write(120); delay(500); feeder.write(0);
}

void onFilter(void *p) { filterOn = !filterOn; renderFilter(); }

void onMeal1(void *p) { uint32_t v=0; sMeal1.Get_Value(&v); mealHr[0] = (uint8_t)(v>23?23:v); renderMeals(); }
void onMeal2(void *p) { uint32_t v=0; sMeal2.Get_Value(&v); mealHr[1] = (uint8_t)(v>23?23:v); renderMeals(); }
void onMeal3(void *p) { uint32_t v=0; sMeal3.Get_Value(&v); mealHr[2] = (uint8_t)(v>23?23:v); renderMeals(); }

void onBright(void *p) { uint32_t v=0; sBright.Get_Value(&v); bright = (uint8_t)(v>100?100:v); renderBright(); }
void onDay  (void *p) { dayMode = true;  bright = 80; sBright.Set_Value(bright); renderBright(); lLight.setText("DAY"); }
void onNight(void *p) { dayMode = false; bright = 10; sBright.Set_Value(bright); renderBright(); lLight.setText("NIGHT"); }

void onLow (void *p) { uint32_t v=0; sLow.Get_Value(&v);  lowLim  = (int8_t)(v>40?40:v); renderLimits(); }
void onHigh(void *p) { uint32_t v=0; sHigh.Get_Value(&v); highLim = (int8_t)(v>40?40:v); renderLimits(); }

void onCalPh(void *p)
{
    int raw = analogRead(A0);
    float ph = raw / 1023.0f * 14.0f;
    phOffset = 7.0f - ph;
    lPh.setText("Calibrated");
}

AirTouch *air_listen_list[] = {
    &bFilter, &bFeedNow,
    &sMeal1, &sMeal2, &sMeal3,
    &sBright, &bDay, &bNight,
    &sLow, &sHigh, &bCalPh,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    ds.begin();
    feeder.attach(FEEDER_PIN); feeder.write(0);

    bFilter.attachPop(onFilter, &bFilter);
    bFeedNow.attachPop(onFeed, &bFeedNow);
    sMeal1.attachPop(onMeal1, &sMeal1);
    sMeal2.attachPop(onMeal2, &sMeal2);
    sMeal3.attachPop(onMeal3, &sMeal3);
    sBright.attachPop(onBright, &sBright);
    bDay.attachPop(onDay, &bDay);
    bNight.attachPop(onNight, &bNight);
    sLow.attachPop(onLow, &sLow);
    sHigh.attachPop(onHigh, &sHigh);
    bCalPh.attachPop(onCalPh, &bCalPh);

    sMeal1.Set_Value(mealHr[0]); sMeal2.Set_Value(mealHr[1]); sMeal3.Set_Value(mealHr[2]);
    sBright.Set_Value(bright);
    sLow.Set_Value(lowLim); sHigh.Set_Value(highLim);
    renderFilter(); renderMeals(); renderBright(); renderLimits();
    lLight.setText(dayMode ? "DAY" : "NIGHT");
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastRead < 3000UL) return;
    lastRead = millis();

    ds.requestTemperatures();
    float t = ds.getTempCByIndex(0);
    int   raw = analogRead(A0);
    float ph = raw / 1023.0f * 14.0f + phOffset;

    char tmp[16];
    if (t > -50 && t < 100) {
        dtostrf(t, 0, 1, tmp); strcat(tmp, " C");
        lTemp.setText(tmp);
    }
    dtostrf(ph, 0, 2, tmp);
    lPh.setText(tmp);
}
