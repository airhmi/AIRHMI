/**
 * 14_SolarInverter  --  5 sayfali solar inverter SCADA
 *
 *   Screen1 Live    : PV W, AC W, Batt W, Grid W (4 lu kart)
 *   Screen2 Battery : SoC %, V, A, charge/discharge yonu
 *   Screen3 Grid    : V, A, freq, import/export kW (anlik) + son 24h
 *   Screen4 History : 24h grid power graph
 *   Screen5 Settings: Grid limit slider, Battery min SoC slider, EXPORT toggle
 *
 *   Mock MPPT: PV gunes egrisi (sin tabanli), batarya integral, grid balans
 */

#include <Airhmi.h>

/* Live */
AirLabel  lPV = AirLabel("lPV");
AirLabel  lAC = AirLabel("lAC");
AirLabel  lBatt = AirLabel("lBatt");
AirLabel  lGridW = AirLabel("lGridW");

/* Battery */
AirLabel  lSoC = AirLabel("lSoC");
AirLabel  lBV = AirLabel("lBV");
AirLabel  lBA = AirLabel("lBA");
AirLabel  lBDir = AirLabel("lBDir");

/* Grid */
AirLabel  lGV = AirLabel("lGV");
AirLabel  lGA = AirLabel("lGA");
AirLabel  lGF = AirLabel("lGF");
AirLabel  lGFlow = AirLabel("lGFlow");

/* History */
AirGraph  gHist = AirGraph("gHist");

/* Settings */
AirSlider sLimit = AirSlider("sLimit");
AirLabel  lLimit = AirLabel("lLimit");
AirSlider sMinSoC = AirSlider("sMinSoC");
AirLabel  lMinSoC = AirLabel("lMinSoC");
AirButton bExport = AirButton("bExport");
AirLabel  lExport = AirLabel("lExport");

float pvW = 0.0f;
float acLoadW = 1200.0f;
float battW = 0.0f;
float gridW = 0.0f;
float battSoC = 65.0f;
float battV = 51.2f;
float battA = 0.0f;
float gridV = 230.0f;
float gridA = 0.0f;
float gridFreq = 50.0f;
uint16_t gridLimitW = 5000;
uint8_t minSoC = 30;
bool exportEnabled = true;
unsigned long simHour = 6;
unsigned long simMin = 0;
unsigned long lastTick = 0;

const uint32_t COL_OFF = 0xFF202020UL;
const uint32_t COL_ON = 0xFF008000UL;

void renderLive()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%d W", (int)pvW); lPV.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%d W", (int)acLoadW); lAC.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%d W", (int)battW); lBatt.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%d W", (int)gridW); lGridW.setText(tmp);
}

void renderBattery()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%d %%", (int)battSoC); lSoC.setText(tmp);
    dtostrf(battV, 0, 1, tmp); strcat(tmp, " V"); lBV.setText(tmp);
    dtostrf(battA, 0, 1, tmp); strcat(tmp, " A"); lBA.setText(tmp);
    lBDir.setText(battW > 50.0f ? "CHARGING" : (battW < -50.0f ? "DISCHARGE" : "IDLE"));
}

void renderGrid()
{
    char tmp[16];
    dtostrf(gridV, 0, 1, tmp); strcat(tmp, " V"); lGV.setText(tmp);
    dtostrf(gridA, 0, 2, tmp); strcat(tmp, " A"); lGA.setText(tmp);
    dtostrf(gridFreq, 0, 2, tmp); strcat(tmp, " Hz"); lGF.setText(tmp);
    if (gridW > 50.0f) lGFlow.setText("EXPORT");
    else if (gridW < -50.0f) lGFlow.setText("IMPORT");
    else lGFlow.setText("IDLE");
}

void renderSettings()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u W", gridLimitW); lLimit.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u %%", minSoC); lMinSoC.setText(tmp);
    lExport.setText(exportEnabled ? "EXPORT ON" : "EXPORT OFF");
    bExport.Set_background_color(exportEnabled ? COL_ON : COL_OFF);
}

void onLimit(void *p)
{
    uint32_t v = 0; sLimit.Get_Value(&v);
    if (v < 1000) v = 1000; if (v > 10000) v = 10000;
    gridLimitW = (uint16_t)v;
    renderSettings();
}
void onMinSoC(void *p)
{
    uint32_t v = 0; sMinSoC.Get_Value(&v);
    if (v > 90) v = 90;
    minSoC = (uint8_t)v;
    renderSettings();
}
void onExport(void *p)
{
    exportEnabled = !exportEnabled;
    renderSettings();
}

AirTouch *air_listen_list[] = {
    &sLimit, &sMinSoC, &bExport,
    NULL
};

float pvCurve(unsigned long h, unsigned long m)
{
    if (h < 6 || h >= 20) return 0.0f;
    float t = (h - 6) + m / 60.0f;
    float angle = (t / 14.0f) * 3.14159f;
    float s = sin(angle);
    if (s < 0) s = 0;
    return 4500.0f * s;
}

void setup()
{
    Serial.begin(115200);
    airInit();
    sLimit.attachPop(onLimit, &sLimit);
    sMinSoC.attachPop(onMinSoC, &sMinSoC);
    bExport.attachPop(onExport, &bExport);

    sLimit.Set_Value(gridLimitW);
    sMinSoC.Set_Value(minSoC);
    renderLive(); renderBattery(); renderGrid(); renderSettings();
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastTick < 500UL) return;
    float dt = (millis() - lastTick) / 1000.0f;
    lastTick = millis();

    simMin += 30;
    if (simMin >= 60) { simMin -= 60; simHour++; if (simHour >= 24) simHour = 0; }

    pvW = pvCurve(simHour, simMin) + random(-100, 100);
    if (pvW < 0) pvW = 0;
    acLoadW = 1100.0f + random(-200, 300);

    float surplus = pvW - acLoadW;

    if (surplus > 0) {
        if (battSoC < 95.0f) {
            battW = surplus * 0.7f;
            if (battW > 2500.0f) battW = 2500.0f;
        } else {
            battW = 0.0f;
        }
        gridW = surplus - battW;
        if (!exportEnabled) gridW = 0.0f;
        if (gridW > gridLimitW) gridW = gridLimitW;
    } else {
        if (battSoC > minSoC) {
            battW = surplus;
            if (battW < -2500.0f) battW = -2500.0f;
        } else {
            battW = 0.0f;
        }
        gridW = surplus - battW;
    }

    battSoC += (battW * dt) / 3600.0f / 50.0f * 100.0f / 100.0f;
    if (battSoC > 100.0f) battSoC = 100.0f;
    if (battSoC < 0.0f)   battSoC = 0.0f;

    battV = 48.0f + (battSoC / 100.0f) * 6.0f;
    battA = battW / battV;

    gridA = gridW / gridV;
    gridFreq = 50.0f + (random(-50, 50) / 1000.0f);

    int16_t graphV = (int16_t)((gridW + 5000.0f) / 50.0f);
    if (graphV < 0) graphV = 0; if (graphV > 200) graphV = 200;
    gHist.addValue((uint32_t)graphV, 0);

    renderLive(); renderBattery(); renderGrid();
}
