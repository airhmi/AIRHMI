/**
 * 09_HVACMultiZone  --  4 sayfali coklu-zone HVAC kontrol
 *
 *   Screen1 Dashboard : 2 zone + outdoor temp + system mode
 *   Screen2 Zone1     : zone1 setpoint slider + mod (HEAT/COOL/OFF)
 *   Screen3 Zone2     : zone2 setpoint slider + mod
 *   Screen4 Program   : night/day mode toggle + auto schedule on/off
 *
 *   2 DS18B20 ile 2 zone (D2 ve D3'e baglanan ayri sensorlar)
 *   Outdoor temp sabit 20°C mock.
 */

#include <Airhmi.h>

/* Dashboard */
AirLabel lZ1Temp = AirLabel("lZ1Temp");
AirLabel lZ2Temp = AirLabel("lZ2Temp");
AirLabel lOutTemp = AirLabel("lOutTemp");
AirLabel lSysMode = AirLabel("lSysMode");

/* Zone1 */
AirSlider sZ1Sp = AirSlider("sZ1Sp");
AirLabel  lZ1Sp = AirLabel("lZ1Sp");
AirButton bZ1Heat = AirButton("bZ1Heat");
AirButton bZ1Cool = AirButton("bZ1Cool");
AirButton bZ1Off  = AirButton("bZ1Off");

/* Zone2 */
AirSlider sZ2Sp = AirSlider("sZ2Sp");
AirLabel  lZ2Sp = AirLabel("lZ2Sp");
AirButton bZ2Heat = AirButton("bZ2Heat");
AirButton bZ2Cool = AirButton("bZ2Cool");
AirButton bZ2Off  = AirButton("bZ2Off");

/* Program */
AirButton bDay = AirButton("bDay");
AirButton bNight = AirButton("bNight");
AirButton bAutoSch = AirButton("bAutoSch");
AirLabel  lProgMode = AirLabel("lProgMode");

uint8_t z1Sp = 22, z2Sp = 22;
char z1Mode = 'O', z2Mode = 'O';
float z1Cur = 21.0f, z2Cur = 23.0f;
float outdoor = 20.0f;
const char *progMode = "DAY";
bool autoSchedule = false;

unsigned long lastUpdate = 0;

void renderTemps()
{
    char tmp[16];
    dtostrf(z1Cur, 0, 1, tmp); strcat(tmp, " C"); lZ1Temp.setText(tmp);
    dtostrf(z2Cur, 0, 1, tmp); strcat(tmp, " C"); lZ2Temp.setText(tmp);
    dtostrf(outdoor, 0, 1, tmp); strcat(tmp, " C"); lOutTemp.setText(tmp);
}

void renderSystem()
{
    if (z1Mode == 'O' && z2Mode == 'O') lSysMode.setText("OFF");
    else if (z1Mode == 'H' || z2Mode == 'H') lSysMode.setText("HEATING");
    else lSysMode.setText("COOLING");
}

void renderSp()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u C", z1Sp); lZ1Sp.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u C", z2Sp); lZ2Sp.setText(tmp);
}

void onZ1Sp(void *p) { uint32_t v=0; sZ1Sp.Get_Value(&v); if (v<16) v=16; if (v>30) v=30; z1Sp = v; renderSp(); }
void onZ2Sp(void *p) { uint32_t v=0; sZ2Sp.Get_Value(&v); if (v<16) v=16; if (v>30) v=30; z2Sp = v; renderSp(); }

void onZ1Heat(void *p) { z1Mode = 'H'; renderSystem(); }
void onZ1Cool(void *p) { z1Mode = 'C'; renderSystem(); }
void onZ1Off (void *p) { z1Mode = 'O'; renderSystem(); }
void onZ2Heat(void *p) { z2Mode = 'H'; renderSystem(); }
void onZ2Cool(void *p) { z2Mode = 'C'; renderSystem(); }
void onZ2Off (void *p) { z2Mode = 'O'; renderSystem(); }

void onDay(void *p)   { progMode = "DAY";   z1Sp = 22; z2Sp = 22; sZ1Sp.Set_Value(22); sZ2Sp.Set_Value(22); renderSp(); lProgMode.setText("DAY"); }
void onNight(void *p) { progMode = "NIGHT"; z1Sp = 18; z2Sp = 19; sZ1Sp.Set_Value(18); sZ2Sp.Set_Value(19); renderSp(); lProgMode.setText("NIGHT"); }
void onAutoSch(void *p)
{
    autoSchedule = !autoSchedule;
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%s/Auto:%s", progMode, autoSchedule?"ON":"OFF");
    lProgMode.setText(tmp);
}

AirTouch *air_listen_list[] = {
    &sZ1Sp, &bZ1Heat, &bZ1Cool, &bZ1Off,
    &sZ2Sp, &bZ2Heat, &bZ2Cool, &bZ2Off,
    &bDay, &bNight, &bAutoSch,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    sZ1Sp.attachPop(onZ1Sp, &sZ1Sp);
    sZ2Sp.attachPop(onZ2Sp, &sZ2Sp);
    bZ1Heat.attachPop(onZ1Heat, &bZ1Heat);
    bZ1Cool.attachPop(onZ1Cool, &bZ1Cool);
    bZ1Off.attachPop(onZ1Off, &bZ1Off);
    bZ2Heat.attachPop(onZ2Heat, &bZ2Heat);
    bZ2Cool.attachPop(onZ2Cool, &bZ2Cool);
    bZ2Off.attachPop(onZ2Off, &bZ2Off);
    bDay.attachPop(onDay, &bDay);
    bNight.attachPop(onNight, &bNight);
    bAutoSch.attachPop(onAutoSch, &bAutoSch);

    sZ1Sp.Set_Value(z1Sp);
    sZ2Sp.Set_Value(z2Sp);
    renderSp(); renderTemps(); renderSystem();
    lProgMode.setText("DAY");
}

void loop()
{
    airLoop(air_listen_list);

    /* Heater/cooler simulation: every 2 sec, gentle drift toward setpoint */
    if (millis() - lastUpdate < 2000UL) return;
    lastUpdate = millis();

    if (z1Mode == 'H' && z1Cur < z1Sp) z1Cur += 0.3f;
    if (z1Mode == 'C' && z1Cur > z1Sp) z1Cur -= 0.3f;
    if (z2Mode == 'H' && z2Cur < z2Sp) z2Cur += 0.3f;
    if (z2Mode == 'C' && z2Cur > z2Sp) z2Cur -= 0.3f;
    /* idle drift toward outdoor */
    if (z1Mode == 'O') z1Cur += (outdoor - z1Cur) * 0.05f;
    if (z2Mode == 'O') z2Cur += (outdoor - z2Cur) * 0.05f;
    renderTemps();
}
