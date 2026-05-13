/**
 * 24_FlightDeck  --  5 sayfali ucak kokpit paneli
 *
 *   Screen1 PFD     : Airspeed gauge, Altitude gauge, Heading label, attitude
 *                     gostergesi (AirShape dekoratif)
 *   Screen2 Nav     : 4 waypoint listesi, current WP, NEXT WP butonu
 *   Screen3 Engines : 2 motor RPM gauge + AirProgressBar yakit gostergesi
 *   Screen4 Autopilot: ALT HOLD, HDG HOLD, NAV, APR mode toggle butonlari
 *   Screen5 Comms   : COM1/COM2 freq label + 4 freq preset
 *
 *   Yeni componentler kombinasyonu: 4 gauge + 1 progressBar + 1 shape.
 */

#include <Airhmi.h>

/* PFD */
AirGauge gAS = AirGauge("gAS");
AirGauge gAlt = AirGauge("gAlt");
AirLabel lHdg = AirLabel("lHdg");
AirLabel lFlightMode = AirLabel("lFlightMode");

/* Nav */
AirLabel  lWp0 = AirLabel("lWp0");
AirLabel  lWp1 = AirLabel("lWp1");
AirLabel  lWp2 = AirLabel("lWp2");
AirLabel  lWp3 = AirLabel("lWp3");
AirButton bNextWp = AirButton("bNextWp");
AirLabel  lDistTo = AirLabel("lDistTo");

/* Engines */
AirGauge gE1 = AirGauge("gE1");
AirGauge gE2 = AirGauge("gE2");
AirProgressBar pbFuel = AirProgressBar("pbFuel");
AirLabel  lFuel = AirLabel("lFuel");

/* Autopilot */
AirButton bAltHold = AirButton("bAltHold");
AirButton bHdgHold = AirButton("bHdgHold");
AirButton bNavMode = AirButton("bNavMode");
AirButton bAprMode = AirButton("bAprMode");
AirLabel  lApMsg = AirLabel("lApMsg");

/* Comms */
AirLabel  lCom1 = AirLabel("lCom1");
AirLabel  lCom2 = AirLabel("lCom2");
AirButton bF0 = AirButton("bF0");
AirButton bF1 = AirButton("bF1");
AirButton bF2 = AirButton("bF2");
AirButton bF3 = AirButton("bF3");

AirBuzzer buz = AirBuzzer("buz");

uint16_t airspeed = 0;
uint16_t altitude = 0;
float heading = 0.0f;
uint16_t e1Rpm = 0, e2Rpm = 0;
uint8_t fuel = 100;

const uint8_t WP_COUNT = 4;
const char *wpName[WP_COUNT] = { "OMR", "ANT", "BRG", "MUN" };
const uint16_t wpDist[WP_COUNT] = { 120, 250, 80, 340 };
uint8_t curWp = 0;

bool altHold = false;
bool hdgHold = false;
bool navMode = false;
bool aprMode = false;

const char *comFreq[4] = { "118.10", "121.50", "127.50", "133.40" };
uint8_t com1Idx = 0, com2Idx = 1;

unsigned long lastTick = 0;

const uint32_t COL_OFF = 0xFF202020UL;
const uint32_t COL_ON = 0xFF008000UL;

void renderPFD()
{
    gAS.Set_value(airspeed);
    gAlt.Set_value(altitude);
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "HDG %d", (int)heading);
    lHdg.setText(tmp);
    const char *m = "MANUAL";
    if (aprMode) m = "APPROACH";
    else if (navMode) m = "NAV";
    else if (hdgHold && altHold) m = "AP ALT+HDG";
    else if (altHold) m = "ALT HOLD";
    else if (hdgHold) m = "HDG HOLD";
    lFlightMode.setText(m);
}

void renderNav()
{
    AirLabel *labels[4] = { &lWp0, &lWp1, &lWp2, &lWp3 };
    char tmp[24];
    for (uint8_t i = 0; i < 4; i++) {
        snprintf(tmp, sizeof(tmp), "%c %s %u nm", (i==curWp?'>':' '), wpName[i], wpDist[i]);
        labels[i]->setText(tmp);
    }
    snprintf(tmp, sizeof(tmp), "%s %u nm", wpName[curWp], wpDist[curWp]);
    lDistTo.setText(tmp);
}

void renderEngines()
{
    gE1.Set_value(e1Rpm);
    gE2.Set_value(e2Rpm);
    pbFuel.Set_Value(fuel);
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "Fuel %u %%", fuel);
    lFuel.setText(tmp);
}

void renderAutopilot()
{
    bAltHold.setText(altHold ? "ALT HOLD ON" : "ALT HOLD");
    bAltHold.Set_background_color(altHold ? COL_ON : COL_OFF);
    bHdgHold.setText(hdgHold ? "HDG HOLD ON" : "HDG HOLD");
    bHdgHold.Set_background_color(hdgHold ? COL_ON : COL_OFF);
    bNavMode.setText(navMode ? "NAV ON" : "NAV");
    bNavMode.Set_background_color(navMode ? COL_ON : COL_OFF);
    bAprMode.setText(aprMode ? "APR ON" : "APR");
    bAprMode.Set_background_color(aprMode ? COL_ON : COL_OFF);
}

void renderComms()
{
    char tmp[20];
    snprintf(tmp, sizeof(tmp), "COM1 %s", comFreq[com1Idx]); lCom1.setText(tmp);
    snprintf(tmp, sizeof(tmp), "COM2 %s", comFreq[com2Idx]); lCom2.setText(tmp);
}

void onNextWp(void *p) { curWp = (curWp+1) % WP_COUNT; renderNav(); buz.Set_Buzzer(60); }

void onAltHold(void *p) { altHold = !altHold; renderAutopilot(); renderPFD(); }
void onHdgHold(void *p) { hdgHold = !hdgHold; renderAutopilot(); renderPFD(); }
void onNavMode(void *p) { navMode = !navMode; renderAutopilot(); renderPFD(); }
void onAprMode(void *p) { aprMode = !aprMode; renderAutopilot(); renderPFD(); }

void onF0(void *p) { com1Idx = 0; renderComms(); }
void onF1(void *p) { com1Idx = 1; renderComms(); }
void onF2(void *p) { com1Idx = 2; renderComms(); }
void onF3(void *p) { com1Idx = 3; renderComms(); }

AirTouch *air_listen_list[] = {
    &bNextWp,
    &bAltHold, &bHdgHold, &bNavMode, &bAprMode,
    &bF0, &bF1, &bF2, &bF3,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bNextWp.attachPop(onNextWp, &bNextWp);
    bAltHold.attachPop(onAltHold, &bAltHold);
    bHdgHold.attachPop(onHdgHold, &bHdgHold);
    bNavMode.attachPop(onNavMode, &bNavMode);
    bAprMode.attachPop(onAprMode, &bAprMode);
    bF0.attachPop(onF0, &bF0); bF1.attachPop(onF1, &bF1);
    bF2.attachPop(onF2, &bF2); bF3.attachPop(onF3, &bF3);

    pbFuel.Set_Value(fuel);
    renderPFD(); renderNav(); renderEngines(); renderAutopilot(); renderComms();
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastTick < 1000UL) return;
    lastTick = millis();

    if (airspeed < 250) airspeed += 5;
    if (altitude < 30000) altitude += 200;
    heading += 0.5f;
    if (heading >= 360) heading -= 360;
    if (e1Rpm < 2400) e1Rpm += 60;
    if (e2Rpm < 2400) e2Rpm += 60;
    if (fuel > 0 && (e1Rpm > 1000 || e2Rpm > 1000)) {
        static uint8_t fc = 0;
        if (++fc >= 5) { fc = 0; fuel--; }
    }
    if (fuel < 20) buz.Set_Buzzer(150);
    renderPFD(); renderEngines();
}
