/**
 * 13_SCADAFactoryLine  --  5 sayfali fabrika hatti SCADA tarzi panel
 *
 *   Screen1 Overview : 4 istasyon durumu + toplam throughput
 *   Screen2 Stations : her istasyon icin start/stop + parca sayaci
 *   Screen3 OEE      : Availability/Performance/Quality + overall OEE
 *   Screen4 Alarms   : son 5 alarm (oncelik renkli)
 *   Screen5 Recipes  : 3 recete preset, mevcut recete
 *
 *   4 istasyon: Conveyor -> Drill -> Paint -> QA
 *   Run istasyon her recipeCycleMs'de bir parca uretir, bazen QA'da reject
 */

#include <Airhmi.h>

/* Overview */
AirLabel  lOvw0 = AirLabel("lOvw0");
AirLabel  lOvw1 = AirLabel("lOvw1");
AirLabel  lOvw2 = AirLabel("lOvw2");
AirLabel  lOvw3 = AirLabel("lOvw3");
AirLabel  lTotal = AirLabel("lTotal");

/* Stations */
AirButton bSt0 = AirButton("bSt0");
AirButton bSt1 = AirButton("bSt1");
AirButton bSt2 = AirButton("bSt2");
AirButton bSt3 = AirButton("bSt3");
AirLabel  lCnt0 = AirLabel("lCnt0");
AirLabel  lCnt1 = AirLabel("lCnt1");
AirLabel  lCnt2 = AirLabel("lCnt2");
AirLabel  lCnt3 = AirLabel("lCnt3");

/* OEE */
AirLabel  lAvail = AirLabel("lAvail");
AirLabel  lPerf = AirLabel("lPerf");
AirLabel  lQual = AirLabel("lQual");
AirLabel  lOEE = AirLabel("lOEE");

/* Alarms */
AirLabel lA0 = AirLabel("lA0");
AirLabel lA1 = AirLabel("lA1");
AirLabel lA2 = AirLabel("lA2");
AirLabel lA3 = AirLabel("lA3");
AirLabel lA4 = AirLabel("lA4");
AirButton bAckAll = AirButton("bAckAll");

/* Recipes */
AirButton bR0 = AirButton("bR0");
AirButton bR1 = AirButton("bR1");
AirButton bR2 = AirButton("bR2");
AirLabel  lRecipe = AirLabel("lRecipe");

AirBuzzer buz = AirBuzzer("buz");

const char *stName[4] = { "Conveyor", "Drill", "Paint", "QA" };
bool stRun[4] = { false, false, false, false };
uint16_t stCount[4] = { 0, 0, 0, 0 };
uint16_t goodPieces = 0;
uint16_t scrapPieces = 0;

unsigned long lastCycle = 0;
unsigned long sysStart = 0;
unsigned long downtimeMs = 0;
unsigned long lastDownTick = 0;

struct Recipe { const char *name; uint16_t cycleMs; uint8_t scrapRate; };
const Recipe recipes[3] = {
    { "Standard", 1500, 5 },
    { "Fast",      800, 12 },
    { "HighQA",   2200, 2 }
};
uint8_t curRecipe = 0;

const uint32_t COL_RUN = 0xFF008000UL;
const uint32_t COL_STOP = 0xFF202020UL;
const uint32_t COL_FAULT = 0xFFFF0000UL;

char alarmBuf[5][24];

void renderOverview()
{
    AirLabel *labels[4] = { &lOvw0, &lOvw1, &lOvw2, &lOvw3 };
    char tmp[24];
    for (uint8_t i = 0; i < 4; i++) {
        snprintf(tmp, sizeof(tmp), "%s\n%s", stName[i], stRun[i] ? "RUN" : "STOP");
        labels[i]->setText(tmp);
    }
    snprintf(tmp, sizeof(tmp), "%u OK / %u SCRAP", goodPieces, scrapPieces);
    lTotal.setText(tmp);
}

void renderStations()
{
    AirButton *btns[4] = { &bSt0, &bSt1, &bSt2, &bSt3 };
    AirLabel *cnts[4] = { &lCnt0, &lCnt1, &lCnt2, &lCnt3 };
    char tmp[20];
    for (uint8_t i = 0; i < 4; i++) {
        snprintf(tmp, sizeof(tmp), "%s\n%s", stName[i], stRun[i] ? "RUN" : "STOP");
        btns[i]->setText(tmp);
        btns[i]->Set_background_color(stRun[i] ? COL_RUN : COL_STOP);
        snprintf(tmp, sizeof(tmp), "%u", stCount[i]);
        cnts[i]->setText(tmp);
    }
}

void renderOEE()
{
    unsigned long total = millis() - sysStart;
    if (total < 1) total = 1;
    unsigned long up = total - downtimeMs;
    uint8_t a = (uint8_t)((up * 100UL) / total);
    uint16_t totalP = goodPieces + scrapPieces;
    uint8_t q = totalP ? (uint8_t)((goodPieces * 100UL) / totalP) : 100;
    uint8_t p = totalP ? 95 : 100;
    uint8_t oee = (uint16_t)a * q / 100 * p / 100;
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u %%", a); lAvail.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u %%", p); lPerf.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u %%", q); lQual.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u %%", oee); lOEE.setText(tmp);
}

void renderAlarms()
{
    AirLabel *labels[5] = { &lA0, &lA1, &lA2, &lA3, &lA4 };
    for (uint8_t i = 0; i < 5; i++) labels[i]->setText(alarmBuf[i]);
}

void renderRecipes()
{
    AirButton *btns[3] = { &bR0, &bR1, &bR2 };
    char tmp[20];
    for (uint8_t i = 0; i < 3; i++) {
        snprintf(tmp, sizeof(tmp), "%s", recipes[i].name);
        btns[i]->setText(tmp);
        btns[i]->Set_background_color(i == curRecipe ? COL_RUN : COL_STOP);
    }
    snprintf(tmp, sizeof(tmp), "%s (%u ms)", recipes[curRecipe].name, recipes[curRecipe].cycleMs);
    lRecipe.setText(tmp);
}

void addAlarm(const char *msg)
{
    for (uint8_t i = 4; i > 0; i--) strcpy(alarmBuf[i], alarmBuf[i-1]);
    strncpy(alarmBuf[0], msg, 23);
    alarmBuf[0][23] = 0;
    renderAlarms();
}

void onSt0(void *p) { stRun[0] = !stRun[0]; renderStations(); renderOverview(); }
void onSt1(void *p) { stRun[1] = !stRun[1]; renderStations(); renderOverview(); }
void onSt2(void *p) { stRun[2] = !stRun[2]; renderStations(); renderOverview(); }
void onSt3(void *p) { stRun[3] = !stRun[3]; renderStations(); renderOverview(); }

void onAckAll(void *p)
{
    for (uint8_t i = 0; i < 5; i++) alarmBuf[i][0] = 0;
    renderAlarms();
}

void onR0(void *p) { curRecipe = 0; renderRecipes(); addAlarm("Recipe Std"); }
void onR1(void *p) { curRecipe = 1; renderRecipes(); addAlarm("Recipe Fast"); }
void onR2(void *p) { curRecipe = 2; renderRecipes(); addAlarm("Recipe HighQA"); }

AirTouch *air_listen_list[] = {
    &bSt0, &bSt1, &bSt2, &bSt3,
    &bAckAll,
    &bR0, &bR1, &bR2,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bSt0.attachPop(onSt0, &bSt0);
    bSt1.attachPop(onSt1, &bSt1);
    bSt2.attachPop(onSt2, &bSt2);
    bSt3.attachPop(onSt3, &bSt3);
    bAckAll.attachPop(onAckAll, &bAckAll);
    bR0.attachPop(onR0, &bR0);
    bR1.attachPop(onR1, &bR1);
    bR2.attachPop(onR2, &bR2);

    sysStart = millis();
    lastDownTick = millis();
    for (uint8_t i = 0; i < 5; i++) alarmBuf[i][0] = 0;
    addAlarm("System Start");
    renderOverview(); renderStations(); renderOEE(); renderAlarms(); renderRecipes();
}

void loop()
{
    airLoop(air_listen_list);

    bool allRun = stRun[0] && stRun[1] && stRun[2] && stRun[3];
    if (!allRun) downtimeMs += millis() - lastDownTick;
    lastDownTick = millis();

    if (allRun && millis() - lastCycle >= recipes[curRecipe].cycleMs) {
        lastCycle = millis();
        for (uint8_t i = 0; i < 4; i++) stCount[i]++;
        bool scrap = (random(100) < recipes[curRecipe].scrapRate);
        if (scrap) {
            scrapPieces++;
            addAlarm("QA reject");
            buz.Set_Buzzer(80);
        } else {
            goodPieces++;
        }
        if (random(200) == 0) {
            uint8_t s = random(4);
            stRun[s] = false;
            char tmp[24];
            snprintf(tmp, sizeof(tmp), "FAULT %s", stName[s]);
            addAlarm(tmp);
            buz.Set_Buzzer(400);
        }
        renderStations(); renderOverview();
    }

    static unsigned long lastUpd = 0;
    if (millis() - lastUpd >= 1000UL) {
        lastUpd = millis();
        renderOEE();
        renderOverview();
    }
}
