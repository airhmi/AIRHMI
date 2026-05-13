/**
 * 15_ShipBridge  --  5 sayfali gemi koprusu paneli
 *
 *   Screen1 Helm   : pusula heading, dumen acisi, autopilot toggle, target
 *                    heading slider
 *   Screen2 Engine : RPM gauge, throttle slider, ileri/geri/stop
 *   Screen3 Nav    : GPS lat/lon, hiz, mesafeyi-hedef, waypoint sayisi
 *   Screen4 Radar  : 4 hedef (mock) + bearing/distance
 *   Screen5 Alarms : MOB toggle, son 5 alarm
 *
 *   Autopilot: hedef heading'e dogru P-kontrolu (rudder = clamp(K * err, -30, 30))
 *   Boat hareketi: speed * cos/sin(heading) -> lat/lon entegrasyonu
 */

#include <Airhmi.h>

/* Helm */
AirLabel  lHdg = AirLabel("lHdg");
AirLabel  lRud = AirLabel("lRud");
AirLabel  lAP = AirLabel("lAP");
AirSlider sTgtHdg = AirSlider("sTgtHdg");
AirLabel  lTgtHdg = AirLabel("lTgtHdg");
AirButton bAP = AirButton("bAP");

/* Engine */
AirGauge  gRPM = AirGauge("gRPM");
AirSlider sThrot = AirSlider("sThrot");
AirLabel  lThrot = AirLabel("lThrot");
AirButton bFwd = AirButton("bFwd");
AirButton bRev = AirButton("bRev");
AirButton bStop = AirButton("bStop");

/* Nav */
AirLabel lLat = AirLabel("lLat");
AirLabel lLon = AirLabel("lLon");
AirLabel lSpd = AirLabel("lSpd");
AirLabel lDist = AirLabel("lDist");
AirButton bNextWp = AirButton("bNextWp");

/* Radar */
AirLabel lT0 = AirLabel("lT0");
AirLabel lT1 = AirLabel("lT1");
AirLabel lT2 = AirLabel("lT2");
AirLabel lT3 = AirLabel("lT3");

/* Alarms */
AirButton bMOB = AirButton("bMOB");
AirLabel  lMOB = AirLabel("lMOB");
AirLabel  lA0 = AirLabel("lA0");
AirLabel  lA1 = AirLabel("lA1");
AirLabel  lA2 = AirLabel("lA2");
AirLabel  lA3 = AirLabel("lA3");
AirLabel  lA4 = AirLabel("lA4");

AirBuzzer buz = AirBuzzer("buz");

float heading = 0.0f;
float tgtHeading = 0.0f;
float rudder = 0.0f;
bool  autopilot = false;
int8_t engineDir = 0;
uint8_t throttle = 0;
uint16_t rpm = 0;
float lat = 41.0f, lon = 29.0f;
float speed = 0.0f;
bool  mobActive = false;
unsigned long lastTick = 0;

const uint8_t WP_COUNT = 4;
const float wpLat[WP_COUNT] = { 41.05f, 41.10f, 41.05f, 41.00f };
const float wpLon[WP_COUNT] = { 29.05f, 29.10f, 29.15f, 29.10f };
uint8_t curWp = 0;

struct Target { float bearing; float dist; };
Target rdr[4];

const uint32_t COL_OFF = 0xFF202020UL;
const uint32_t COL_ON = 0xFF008000UL;
const uint32_t COL_RED = 0xFFFF0000UL;

char alarmBuf[5][24];

void renderHelm()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%d deg", (int)heading); lHdg.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%d deg", (int)rudder); lRud.setText(tmp);
    lAP.setText(autopilot ? "AUTOPILOT" : "MANUAL");
    bAP.Set_background_color(autopilot ? COL_ON : COL_OFF);
    snprintf(tmp, sizeof(tmp), "%d deg", (int)tgtHeading); lTgtHdg.setText(tmp);
}

void renderEngine()
{
    char tmp[16];
    gRPM.Set_value(rpm);
    snprintf(tmp, sizeof(tmp), "%u %%", throttle); lThrot.setText(tmp);
}

void renderNav()
{
    char tmp[16];
    dtostrf(lat, 0, 4, tmp); lLat.setText(tmp);
    dtostrf(lon, 0, 4, tmp); lLon.setText(tmp);
    dtostrf(speed, 0, 1, tmp); strcat(tmp, " kn"); lSpd.setText(tmp);
    float dx = (wpLon[curWp] - lon) * 60.0f;
    float dy = (wpLat[curWp] - lat) * 60.0f;
    float d = sqrt(dx*dx + dy*dy);
    snprintf(tmp, sizeof(tmp), "WP%u %d nm", curWp, (int)d); lDist.setText(tmp);
}

void renderRadar()
{
    AirLabel *labels[4] = { &lT0, &lT1, &lT2, &lT3 };
    char tmp[24];
    for (uint8_t i = 0; i < 4; i++) {
        snprintf(tmp, sizeof(tmp), "T%u: %d deg / %d nm", i, (int)rdr[i].bearing, (int)rdr[i].dist);
        labels[i]->setText(tmp);
    }
}

void renderAlarms()
{
    AirLabel *labels[5] = { &lA0, &lA1, &lA2, &lA3, &lA4 };
    for (uint8_t i = 0; i < 5; i++) labels[i]->setText(alarmBuf[i]);
    lMOB.setText(mobActive ? "MOB ACTIVE" : "MOB OFF");
    bMOB.Set_background_color(mobActive ? COL_RED : COL_OFF);
}

void addAlarm(const char *msg)
{
    for (uint8_t i = 4; i > 0; i--) strcpy(alarmBuf[i], alarmBuf[i-1]);
    strncpy(alarmBuf[0], msg, 23);
    alarmBuf[0][23] = 0;
    renderAlarms();
}

void onTgtHdg(void *p)
{
    uint32_t v = 0; sTgtHdg.Get_Value(&v);
    if (v > 359) v = 359;
    tgtHeading = v;
    renderHelm();
}
void onAP(void *p) { autopilot = !autopilot; renderHelm(); addAlarm(autopilot?"AP ON":"AP OFF"); }
void onThrot(void *p)
{
    uint32_t v = 0; sThrot.Get_Value(&v);
    if (v > 100) v = 100;
    throttle = v;
    renderEngine();
}
void onFwd(void *p)  { engineDir = 1;  addAlarm("Engine FWD"); }
void onRev(void *p)  { engineDir = -1; addAlarm("Engine REV"); }
void onStop(void *p) { engineDir = 0;  addAlarm("Engine STOP"); }

void onNextWp(void *p)
{
    curWp = (curWp + 1) % WP_COUNT;
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "WP%u", curWp);
    addAlarm(tmp);
    renderNav();
}

void onMOB(void *p)
{
    mobActive = !mobActive;
    if (mobActive) {
        addAlarm("MOB!! Stop!");
        engineDir = 0;
        autopilot = false;
        buz.Set_Buzzer(800);
    } else {
        addAlarm("MOB clear");
    }
    renderHelm();
    renderAlarms();
}

AirTouch *air_listen_list[] = {
    &sTgtHdg, &bAP,
    &sThrot, &bFwd, &bRev, &bStop,
    &bNextWp,
    &bMOB,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    sTgtHdg.attachPop(onTgtHdg, &sTgtHdg);
    bAP.attachPop(onAP, &bAP);
    sThrot.attachPop(onThrot, &sThrot);
    bFwd.attachPop(onFwd, &bFwd);
    bRev.attachPop(onRev, &bRev);
    bStop.attachPop(onStop, &bStop);
    bNextWp.attachPop(onNextWp, &bNextWp);
    bMOB.attachPop(onMOB, &bMOB);

    sTgtHdg.Set_Value(0);
    sThrot.Set_Value(0);
    for (uint8_t i = 0; i < 5; i++) alarmBuf[i][0] = 0;
    addAlarm("System ready");
    for (uint8_t i = 0; i < 4; i++) {
        rdr[i].bearing = random(360);
        rdr[i].dist = 1.0f + random(80) / 10.0f;
    }
    renderHelm(); renderEngine(); renderNav(); renderRadar(); renderAlarms();
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastTick < 200UL) return;
    float dt = (millis() - lastTick) / 1000.0f;
    lastTick = millis();

    if (autopilot && !mobActive) {
        float err = tgtHeading - heading;
        while (err > 180) err -= 360;
        while (err < -180) err += 360;
        rudder = err * 0.5f;
        if (rudder > 30) rudder = 30;
        if (rudder < -30) rudder = -30;
    }

    uint16_t tgtRpm = throttle * 30;
    if (engineDir == 0 || mobActive) tgtRpm = 0;
    if (rpm < tgtRpm) rpm += 60;
    else if (rpm > tgtRpm) rpm -= 30;

    speed = (rpm / 3000.0f) * 12.0f * (engineDir);
    heading += rudder * speed * 0.02f * dt * 5.0f;
    while (heading < 0) heading += 360;
    while (heading >= 360) heading -= 360;

    float rad = heading * 3.14159f / 180.0f;
    lat += cos(rad) * speed * dt * 0.0001f;
    lon += sin(rad) * speed * dt * 0.0001f;

    float dx = (wpLon[curWp] - lon) * 60.0f;
    float dy = (wpLat[curWp] - lat) * 60.0f;
    float d = sqrt(dx*dx + dy*dy);
    if (d < 0.5f) {
        char tmp[16];
        snprintf(tmp, sizeof(tmp), "WP%u reached", curWp);
        addAlarm(tmp);
        buz.Set_Buzzer(120);
        curWp = (curWp + 1) % WP_COUNT;
    }

    static unsigned long lastRdr = 0;
    if (millis() - lastRdr >= 2000UL) {
        lastRdr = millis();
        for (uint8_t i = 0; i < 4; i++) {
            rdr[i].dist += (random(20) - 10) / 10.0f;
            if (rdr[i].dist < 0.5f) rdr[i].dist = 0.5f;
            if (rdr[i].dist > 10.0f) rdr[i].dist = 10.0f;
            if (rdr[i].dist < 1.0f) {
                char tmp[20];
                snprintf(tmp, sizeof(tmp), "Collision T%u", i);
                addAlarm(tmp);
                buz.Set_Buzzer(400);
            }
        }
        renderRadar();
    }

    renderHelm(); renderEngine(); renderNav();
}
