/**
 * 18_RailwaySignaling  --  5 sayfali demiryolu sinyalizasyon paneli
 *
 *   Screen1 Track   : 4-block ring uzerinde 2 tren konumu, sinyaller
 *   Screen2 Trains  : T1/T2 hizi, durum (RUN/STOP), START/STOP butonu
 *   Screen3 Signals : 4 sinyal renk kodu (G/Y/R) - blok dolulugu mantigiyla
 *   Screen4 Interlock: rota onaylama, conflict kontrolu, OVERRIDE (riskli!)
 *   Screen5 Log     : son 5 olay (sinyal degisimi, conflict, vb.)
 *
 *   Interlock kurali: bir blok doluysa, o bloga giren sinyal R; ondan onceki Y; geri kalanlar G.
 *   Rota onaylama: sadece hedef blok bos + ara bloklar bos ise onaylanir.
 */

#include <Airhmi.h>

/* Track */
AirLabel  lTrack = AirLabel("lTrack");
AirLabel  lT1Pos = AirLabel("lT1Pos");
AirLabel  lT2Pos = AirLabel("lT2Pos");

/* Trains */
AirButton bT1 = AirButton("bT1");
AirButton bT2 = AirButton("bT2");
AirLabel  lT1State = AirLabel("lT1State");
AirLabel  lT2State = AirLabel("lT2State");
AirSlider sT1Spd = AirSlider("sT1Spd");
AirSlider sT2Spd = AirSlider("sT2Spd");

/* Signals */
AirLabel lSig0 = AirLabel("lSig0");
AirLabel lSig1 = AirLabel("lSig1");
AirLabel lSig2 = AirLabel("lSig2");
AirLabel lSig3 = AirLabel("lSig3");

/* Interlock */
AirButton bRouteT1 = AirButton("bRouteT1");
AirButton bRouteT2 = AirButton("bRouteT2");
AirButton bOverride = AirButton("bOverride");
AirLabel  lInterlock = AirLabel("lInterlock");

/* Log */
AirLabel lL0 = AirLabel("lL0");
AirLabel lL1 = AirLabel("lL1");
AirLabel lL2 = AirLabel("lL2");
AirLabel lL3 = AirLabel("lL3");
AirLabel lL4 = AirLabel("lL4");

AirBuzzer buz = AirBuzzer("buz");

const uint8_t BLOCK_COUNT = 4;
uint8_t t1Block = 0;
uint8_t t2Block = 2;
bool t1Run = false;
bool t2Run = false;
uint8_t t1Speed = 50, t2Speed = 50;
char signal[BLOCK_COUNT];
bool overrideMode = false;
unsigned long lastTick = 0;
unsigned long t1Last = 0, t2Last = 0;

const uint32_t COL_R = 0xFFFF0000UL;
const uint32_t COL_Y = 0xFFFFFF00UL;
const uint32_t COL_G = 0xFF008000UL;
const uint32_t COL_OFF = 0xFF202020UL;

char logBuf[5][24];

void renderTrack()
{
    char tmp[24];
    snprintf(tmp, sizeof(tmp), "[%c][%c][%c][%c]",
        (t1Block==0||t2Block==0)?'#':'_',
        (t1Block==1||t2Block==1)?'#':'_',
        (t1Block==2||t2Block==2)?'#':'_',
        (t1Block==3||t2Block==3)?'#':'_');
    lTrack.setText(tmp);
    snprintf(tmp, sizeof(tmp), "T1 @ Block %u", t1Block); lT1Pos.setText(tmp);
    snprintf(tmp, sizeof(tmp), "T2 @ Block %u", t2Block); lT2Pos.setText(tmp);
}

void renderTrains()
{
    char tmp[20];
    snprintf(tmp, sizeof(tmp), "T1 %s", t1Run?"RUN":"STOP"); lT1State.setText(tmp);
    snprintf(tmp, sizeof(tmp), "T2 %s", t2Run?"RUN":"STOP"); lT2State.setText(tmp);
    bT1.Set_background_color(t1Run ? COL_G : COL_OFF);
    bT2.Set_background_color(t2Run ? COL_G : COL_OFF);
}

void computeSignals()
{
    bool blockOcc[BLOCK_COUNT];
    for (uint8_t i = 0; i < BLOCK_COUNT; i++) blockOcc[i] = false;
    blockOcc[t1Block] = true;
    blockOcc[t2Block] = true;

    for (uint8_t i = 0; i < BLOCK_COUNT; i++) {
        uint8_t next = (i + 1) % BLOCK_COUNT;
        uint8_t after = (i + 2) % BLOCK_COUNT;
        if (blockOcc[next]) signal[i] = 'R';
        else if (blockOcc[after]) signal[i] = 'Y';
        else signal[i] = 'G';
    }
}

void renderSignals()
{
    AirLabel *labels[4] = { &lSig0, &lSig1, &lSig2, &lSig3 };
    char tmp[16];
    for (uint8_t i = 0; i < 4; i++) {
        snprintf(tmp, sizeof(tmp), "S%u: %c", i, signal[i]);
        labels[i]->setText(tmp);
    }
}

void renderInterlock()
{
    if (overrideMode) lInterlock.setText("OVERRIDE!");
    else lInterlock.setText("Normal");
    bOverride.Set_background_color(overrideMode ? COL_R : COL_OFF);
}

void renderLog()
{
    AirLabel *labels[5] = { &lL0, &lL1, &lL2, &lL3, &lL4 };
    for (uint8_t i = 0; i < 5; i++) labels[i]->setText(logBuf[i]);
}

void addLog(const char *msg)
{
    for (uint8_t i = 4; i > 0; i--) strcpy(logBuf[i], logBuf[i-1]);
    strncpy(logBuf[0], msg, 23);
    logBuf[0][23] = 0;
    renderLog();
}

void onT1(void *p) { t1Run = !t1Run; renderTrains(); addLog(t1Run?"T1 RUN":"T1 STOP"); }
void onT2(void *p) { t2Run = !t2Run; renderTrains(); addLog(t2Run?"T2 RUN":"T2 STOP"); }
void onT1Spd(void *p)
{
    uint32_t v = 0; sT1Spd.Get_Value(&v);
    if (v < 10) v = 10; if (v > 100) v = 100;
    t1Speed = v;
}
void onT2Spd(void *p)
{
    uint32_t v = 0; sT2Spd.Get_Value(&v);
    if (v < 10) v = 10; if (v > 100) v = 100;
    t2Speed = v;
}

bool routeOk(uint8_t from, uint8_t skipTrain)
{
    uint8_t to = (from + 1) % BLOCK_COUNT;
    if (skipTrain == 1 && t2Block == to) return false;
    if (skipTrain == 2 && t1Block == to) return false;
    return true;
}

void onRouteT1(void *p)
{
    if (overrideMode || routeOk(t1Block, 1)) {
        addLog("Route T1 OK");
        t1Run = true;
        renderTrains();
    } else {
        addLog("T1 route BLOCKED");
        buz.Set_Buzzer(300);
    }
}
void onRouteT2(void *p)
{
    if (overrideMode || routeOk(t2Block, 2)) {
        addLog("Route T2 OK");
        t2Run = true;
        renderTrains();
    } else {
        addLog("T2 route BLOCKED");
        buz.Set_Buzzer(300);
    }
}
void onOverride(void *p)
{
    overrideMode = !overrideMode;
    addLog(overrideMode?"OVERRIDE ON":"OVERRIDE OFF");
    renderInterlock();
}

AirTouch *air_listen_list[] = {
    &bT1, &bT2, &sT1Spd, &sT2Spd,
    &bRouteT1, &bRouteT2, &bOverride,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bT1.attachPop(onT1, &bT1);
    bT2.attachPop(onT2, &bT2);
    sT1Spd.attachPop(onT1Spd, &sT1Spd);
    sT2Spd.attachPop(onT2Spd, &sT2Spd);
    bRouteT1.attachPop(onRouteT1, &bRouteT1);
    bRouteT2.attachPop(onRouteT2, &bRouteT2);
    bOverride.attachPop(onOverride, &bOverride);

    sT1Spd.Set_Value(t1Speed);
    sT2Spd.Set_Value(t2Speed);
    for (uint8_t i = 0; i < 5; i++) logBuf[i][0] = 0;
    addLog("System ready");
    computeSignals();
    renderTrack(); renderTrains(); renderSignals(); renderInterlock();
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastTick < 200UL) return;
    lastTick = millis();

    if (t1Run) {
        unsigned long ms = (101 - t1Speed) * 50UL;
        if (millis() - t1Last >= ms) {
            t1Last = millis();
            uint8_t next = (t1Block + 1) % BLOCK_COUNT;
            if (next == t2Block && !overrideMode) {
                t1Run = false;
                addLog("T1 STOP at signal");
                buz.Set_Buzzer(150);
            } else {
                t1Block = next;
                if (t1Block == t2Block) {
                    addLog("COLLISION!!");
                    t1Run = false; t2Run = false;
                    buz.Set_Buzzer(800);
                }
            }
        }
    }
    if (t2Run) {
        unsigned long ms = (101 - t2Speed) * 50UL;
        if (millis() - t2Last >= ms) {
            t2Last = millis();
            uint8_t next = (t2Block + 1) % BLOCK_COUNT;
            if (next == t1Block && !overrideMode) {
                t2Run = false;
                addLog("T2 STOP at signal");
                buz.Set_Buzzer(150);
            } else {
                t2Block = next;
                if (t1Block == t2Block) {
                    addLog("COLLISION!!");
                    t1Run = false; t2Run = false;
                    buz.Set_Buzzer(800);
                }
            }
        }
    }
    computeSignals();
    renderTrack(); renderTrains(); renderSignals();
}
