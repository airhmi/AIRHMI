/**
 * 19_DroneGCS  --  5 sayfali drone Ground Control Station
 *
 *   Screen1 Map      : drone konum, hedef WP, base konumu (text-vari)
 *   Screen2 Telemetry: altitude, speed, battery %, satellite count
 *   Screen3 Mission  : 4 waypoint listesi, current WP, NEXT WP
 *   Screen4 Camera   : "live view" simulasyon, snapshot count, recording
 *   Screen5 Settings : ARM/DISARM, RTH (return-to-home), Failsafe esikleri
 *
 *   Auto-RTH: batt < 20 ise eve donus, batt < 10 ise emergency land.
 */

#include <Airhmi.h>

/* Map */
AirLabel  lDronePos = AirLabel("lDronePos");
AirLabel  lTgtWp = AirLabel("lTgtWp");
AirLabel  lBase = AirLabel("lBase");
AirLabel  lDistHome = AirLabel("lDistHome");

/* Telemetry */
AirLabel  lAlt = AirLabel("lAlt");
AirLabel  lSpd = AirLabel("lSpd");
AirLabel  lBatt = AirLabel("lBatt");
AirLabel  lSats = AirLabel("lSats");
AirLabel  lFlightMode = AirLabel("lFlightMode");

/* Mission */
AirLabel  lWp0 = AirLabel("lWp0");
AirLabel  lWp1 = AirLabel("lWp1");
AirLabel  lWp2 = AirLabel("lWp2");
AirLabel  lWp3 = AirLabel("lWp3");
AirButton bNextWp = AirButton("bNextWp");
AirButton bStartMission = AirButton("bStartMission");

/* Camera */
AirLabel  lCamView = AirLabel("lCamView");
AirLabel  lSnaps = AirLabel("lSnaps");
AirButton bSnap = AirButton("bSnap");
AirButton bRec = AirButton("bRec");

/* Settings */
AirButton bArm = AirButton("bArm");
AirButton bRTH = AirButton("bRTH");
AirLabel  lArmState = AirLabel("lArmState");
AirLabel  lFailsafe = AirLabel("lFailsafe");

AirBuzzer buz = AirBuzzer("buz");

bool armed = false;
bool missionActive = false;
bool rthActive = false;
bool recording = false;
uint16_t snaps = 0;

float droneLat = 41.0f, droneLon = 29.0f;
const float baseLat = 41.0f, baseLon = 29.0f;

const uint8_t WP_COUNT = 4;
const float wpLat[WP_COUNT] = { 41.005f, 41.010f, 41.005f, 41.000f };
const float wpLon[WP_COUNT] = { 29.005f, 29.010f, 29.015f, 29.010f };
uint8_t curWp = 0;

float alt = 0.0f;
float spd = 0.0f;
uint8_t batt = 100;
uint8_t sats = 12;
const char *flightMode = "DISARMED";

unsigned long lastTick = 0;
unsigned long lastBattTick = 0;

const uint32_t COL_OFF = 0xFF202020UL;
const uint32_t COL_ON = 0xFF008000UL;
const uint32_t COL_RED = 0xFFFF0000UL;

void renderMap()
{
    char tmp[24];
    char la[8], lo[8];
    dtostrf(droneLat, 0, 4, la); dtostrf(droneLon, 0, 4, lo);
    snprintf(tmp, sizeof(tmp), "%s,%s", la, lo);
    lDronePos.setText(tmp);
    dtostrf(wpLat[curWp], 0, 4, la); dtostrf(wpLon[curWp], 0, 4, lo);
    snprintf(tmp, sizeof(tmp), "WP%u %s,%s", curWp, la, lo);
    lTgtWp.setText(tmp);
    dtostrf(baseLat, 0, 4, la); dtostrf(baseLon, 0, 4, lo);
    snprintf(tmp, sizeof(tmp), "%s,%s", la, lo);
    lBase.setText(tmp);
    float dx = (baseLon - droneLon) * 60000.0f;
    float dy = (baseLat - droneLat) * 60000.0f;
    float d = sqrt(dx*dx + dy*dy);
    snprintf(tmp, sizeof(tmp), "%d m", (int)d);
    lDistHome.setText(tmp);
}

void renderTelemetry()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%d m", (int)alt); lAlt.setText(tmp);
    dtostrf(spd, 0, 1, tmp); strcat(tmp, " m/s"); lSpd.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u %%", batt); lBatt.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u sats", sats); lSats.setText(tmp);
    lFlightMode.setText(flightMode);
}

void renderMission()
{
    AirLabel *labels[4] = { &lWp0, &lWp1, &lWp2, &lWp3 };
    char tmp[24];
    char la[8], lo[8];
    for (uint8_t i = 0; i < 4; i++) {
        dtostrf(wpLat[i], 0, 4, la); dtostrf(wpLon[i], 0, 4, lo);
        snprintf(tmp, sizeof(tmp), "%c WP%u %s,%s", (i==curWp?'>':' '), i, la, lo);
        labels[i]->setText(tmp);
    }
}

void renderCamera()
{
    char tmp[24];
    if (armed) lCamView.setText(recording ? "REC" : "LIVE");
    else lCamView.setText("OFF");
    snprintf(tmp, sizeof(tmp), "%u snaps", snaps);
    lSnaps.setText(tmp);
    bRec.Set_background_color(recording ? COL_RED : COL_OFF);
}

void renderSettings()
{
    lArmState.setText(armed ? "ARMED" : "DISARMED");
    bArm.Set_background_color(armed ? COL_ON : COL_OFF);
    char tmp[32];
    snprintf(tmp, sizeof(tmp), "RTH<20%%, Land<10%%");
    lFailsafe.setText(tmp);
    bRTH.Set_background_color(rthActive ? COL_RED : COL_OFF);
}

void onArm(void *p)
{
    armed = !armed;
    if (armed) {
        flightMode = "ARMED";
        alt = 0.0f;
    } else {
        flightMode = "DISARMED";
        alt = 0.0f;
        spd = 0.0f;
        missionActive = false;
        rthActive = false;
    }
    buz.Set_Buzzer(120);
    renderSettings(); renderTelemetry();
}

void onRTH(void *p)
{
    if (!armed) return;
    rthActive = !rthActive;
    flightMode = rthActive ? "RTH" : "ARMED";
    renderSettings(); renderTelemetry();
}

void onNextWp(void *p)
{
    curWp = (curWp + 1) % WP_COUNT;
    renderMission(); renderMap();
}

void onStartMission(void *p)
{
    if (!armed) return;
    missionActive = !missionActive;
    flightMode = missionActive ? "AUTO" : "ARMED";
    renderTelemetry();
}

void onSnap(void *p)
{
    if (!armed) return;
    snaps++;
    buz.Set_Buzzer(80);
    renderCamera();
}

void onRec(void *p)
{
    if (!armed) return;
    recording = !recording;
    renderCamera();
}

AirTouch *air_listen_list[] = {
    &bArm, &bRTH, &bNextWp, &bStartMission, &bSnap, &bRec,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bArm.attachPop(onArm, &bArm);
    bRTH.attachPop(onRTH, &bRTH);
    bNextWp.attachPop(onNextWp, &bNextWp);
    bStartMission.attachPop(onStartMission, &bStartMission);
    bSnap.attachPop(onSnap, &bSnap);
    bRec.attachPop(onRec, &bRec);

    renderMap(); renderTelemetry(); renderMission(); renderCamera(); renderSettings();
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastTick < 200UL) return;
    float dt = (millis() - lastTick) / 1000.0f;
    lastTick = millis();

    if (!armed) return;

    float tgtLat, tgtLon;
    if (rthActive) { tgtLat = baseLat; tgtLon = baseLon; }
    else { tgtLat = wpLat[curWp]; tgtLon = wpLon[curWp]; }

    float dx = (tgtLon - droneLon) * 60000.0f;
    float dy = (tgtLat - droneLat) * 60000.0f;
    float d = sqrt(dx*dx + dy*dy);

    if (alt < 30.0f && !rthActive) alt += 2.0f * dt;
    if (rthActive && d < 5.0f) {
        alt -= 3.0f * dt;
        if (alt <= 0.0f) {
            alt = 0.0f;
            rthActive = false;
            armed = false;
            flightMode = "LANDED";
            buz.Set_Buzzer(400);
        }
    }

    if (d > 0.5f && missionActive) {
        spd = 5.0f;
        droneLat += (dy / d) * 0.00005f;
        droneLon += (dx / d) * 0.00005f;
    } else if (rthActive) {
        spd = 5.0f;
        if (d > 0.5f) {
            droneLat += (dy / d) * 0.00005f;
            droneLon += (dx / d) * 0.00005f;
        } else {
            spd = 0.0f;
        }
    } else {
        spd = 0.0f;
    }

    if (missionActive && d < 1.0f && !rthActive) {
        curWp = (curWp + 1) % WP_COUNT;
        renderMission();
        buz.Set_Buzzer(100);
    }

    if (millis() - lastBattTick >= 3000UL) {
        lastBattTick = millis();
        if (batt > 0 && armed) batt--;
        if (batt < 20 && !rthActive) {
            rthActive = true;
            flightMode = "RTH-LOWBATT";
            buz.Set_Buzzer(400);
        }
        if (batt < 10) {
            flightMode = "LAND-EMERGENCY";
            buz.Set_Buzzer(800);
        }
    }

    sats = 8 + (random(8));

    renderMap(); renderTelemetry();
}
