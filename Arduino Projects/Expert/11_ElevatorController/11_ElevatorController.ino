/**
 * 11_ElevatorController  --  5 sayfali asansor kontrolu
 *
 *   Screen1 Cabin   : kat numarasi, yon, kapi durumu
 *   Screen2 Floors  : 8 kat cagri butonu (G..7)
 *   Screen3 Diag    : motor sicakligi, kapi cycle, run time
 *   Screen4 Maint   : Emergency Stop, Door Test, Reset Diag
 *   Screen5 Log     : son 5 olay
 *
 *   State machine: IDLE -> DOOR_CLOSE -> MOVING -> DOOR_OPEN -> IDLE
 *   Floor degisimi her 1500 ms 1 kat. Kapi 2000 ms acik.
 */

#include <Airhmi.h>

/* Cabin */
AirLabel  lFloor = AirLabel("lFloor");
AirLabel  lDir = AirLabel("lDir");
AirLabel  lDoor = AirLabel("lDoor");
AirLabel  lState = AirLabel("lState");

/* Floors (G..7 = 0..7) */
AirButton bF0 = AirButton("bF0");
AirButton bF1 = AirButton("bF1");
AirButton bF2 = AirButton("bF2");
AirButton bF3 = AirButton("bF3");
AirButton bF4 = AirButton("bF4");
AirButton bF5 = AirButton("bF5");
AirButton bF6 = AirButton("bF6");
AirButton bF7 = AirButton("bF7");

/* Diag */
AirLabel lMotorT = AirLabel("lMotorT");
AirLabel lCycles = AirLabel("lCycles");
AirLabel lRunTm = AirLabel("lRunTm");

/* Maint */
AirButton bEStop = AirButton("bEStop");
AirButton bDoorTest = AirButton("bDoorTest");
AirButton bResetDiag = AirButton("bResetDiag");
AirLabel  lMaintMsg = AirLabel("lMaintMsg");

/* Log */
AirLabel lLog0 = AirLabel("lLog0");
AirLabel lLog1 = AirLabel("lLog1");
AirLabel lLog2 = AirLabel("lLog2");
AirLabel lLog3 = AirLabel("lLog3");
AirLabel lLog4 = AirLabel("lLog4");

AirBuzzer buz = AirBuzzer("buz");

enum { ST_IDLE, ST_DOOR_CLOSING, ST_MOVING, ST_DOOR_OPENING, ST_DOOR_OPEN, ST_ESTOP } st = ST_IDLE;

uint8_t curFloor = 0;
int8_t  dir = 0;
bool    pending[8] = { false, false, false, false, false, false, false, false };
unsigned long stateEnter = 0;

uint16_t doorCycles = 0;
float    motorTemp = 28.0f;
unsigned long runStart = 0;
bool eStopActive = false;

const uint32_t COL_OFF = 0xFF202020UL;
const uint32_t COL_PEND = 0xFFFFA500UL;
const uint32_t COL_HERE = 0xFF008000UL;

char logBuf[5][24];

void renderFloorBtns()
{
    AirButton *b[8] = { &bF0, &bF1, &bF2, &bF3, &bF4, &bF5, &bF6, &bF7 };
    for (uint8_t i = 0; i < 8; i++) {
        uint32_t c = COL_OFF;
        if (i == curFloor) c = COL_HERE;
        else if (pending[i]) c = COL_PEND;
        b[i]->Set_background_color(c);
    }
}

void renderCabin()
{
    char tmp[8];
    if (curFloor == 0) lFloor.setText("G");
    else { snprintf(tmp, sizeof(tmp), "%u", curFloor); lFloor.setText(tmp); }
    lDir.setText(dir > 0 ? "UP" : (dir < 0 ? "DOWN" : "--"));
    lDoor.setText(st == ST_DOOR_OPEN ? "OPEN" : (st == ST_DOOR_OPENING || st == ST_DOOR_CLOSING ? "..." : "CLOSED"));
    const char *sn = "IDLE";
    switch (st) {
        case ST_DOOR_CLOSING: sn = "DOOR CLOSING"; break;
        case ST_MOVING:       sn = "MOVING"; break;
        case ST_DOOR_OPENING: sn = "DOOR OPENING"; break;
        case ST_DOOR_OPEN:    sn = "DOOR OPEN"; break;
        case ST_ESTOP:        sn = "E-STOP"; break;
        default: break;
    }
    lState.setText(sn);
}

void renderDiag()
{
    char tmp[16];
    dtostrf(motorTemp, 0, 1, tmp); strcat(tmp, " C"); lMotorT.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u", doorCycles); lCycles.setText(tmp);
    unsigned long s = (millis() - runStart) / 1000UL;
    snprintf(tmp, sizeof(tmp), "%lu s", s); lRunTm.setText(tmp);
}

void renderLog()
{
    AirLabel *labels[5] = { &lLog0, &lLog1, &lLog2, &lLog3, &lLog4 };
    for (uint8_t i = 0; i < 5; i++) labels[i]->setText(logBuf[i]);
}

void addLog(const char *msg)
{
    for (uint8_t i = 4; i > 0; i--) strcpy(logBuf[i], logBuf[i-1]);
    strncpy(logBuf[0], msg, 23);
    logBuf[0][23] = 0;
    renderLog();
}

void enterState(uint8_t s)
{
    st = (decltype(st))s;
    stateEnter = millis();
    renderCabin();
    renderFloorBtns();
}

int8_t pickNext()
{
    if (dir >= 0) {
        for (int8_t i = curFloor + 1; i < 8; i++) if (pending[i]) return i;
        for (int8_t i = curFloor - 1; i >= 0; i--) if (pending[i]) return i;
    } else {
        for (int8_t i = curFloor - 1; i >= 0; i--) if (pending[i]) return i;
        for (int8_t i = curFloor + 1; i < 8; i++) if (pending[i]) return i;
    }
    return -1;
}

void callFloor(uint8_t f)
{
    if (eStopActive) return;
    if (f == curFloor && st == ST_IDLE) {
        enterState(ST_DOOR_OPENING);
        return;
    }
    pending[f] = true;
    char tmp[20];
    snprintf(tmp, sizeof(tmp), "Call %s", f==0?"G":(f==1?"1":(f==2?"2":(f==3?"3":(f==4?"4":(f==5?"5":(f==6?"6":"7")))))));
    addLog(tmp);
    renderFloorBtns();
}

void onF0(void *p) { callFloor(0); } void onF1(void *p) { callFloor(1); }
void onF2(void *p) { callFloor(2); } void onF3(void *p) { callFloor(3); }
void onF4(void *p) { callFloor(4); } void onF5(void *p) { callFloor(5); }
void onF6(void *p) { callFloor(6); } void onF7(void *p) { callFloor(7); }

void onEStop(void *p)
{
    eStopActive = !eStopActive;
    if (eStopActive) {
        enterState(ST_ESTOP);
        lMaintMsg.setText("E-STOP ENGAGED");
        addLog("E-STOP ON");
        buz.Set_Buzzer(800);
    } else {
        enterState(ST_IDLE);
        lMaintMsg.setText("E-STOP RELEASED");
        addLog("E-STOP OFF");
    }
}

void onDoorTest(void *p)
{
    if (st != ST_IDLE || eStopActive) return;
    enterState(ST_DOOR_OPENING);
    addLog("Door test");
}

void onResetDiag(void *p)
{
    doorCycles = 0;
    motorTemp = 28.0f;
    runStart = millis();
    renderDiag();
    lMaintMsg.setText("Diag reset");
    addLog("Diag reset");
}

AirTouch *air_listen_list[] = {
    &bF0, &bF1, &bF2, &bF3, &bF4, &bF5, &bF6, &bF7,
    &bEStop, &bDoorTest, &bResetDiag,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bF0.attachPop(onF0,&bF0); bF1.attachPop(onF1,&bF1);
    bF2.attachPop(onF2,&bF2); bF3.attachPop(onF3,&bF3);
    bF4.attachPop(onF4,&bF4); bF5.attachPop(onF5,&bF5);
    bF6.attachPop(onF6,&bF6); bF7.attachPop(onF7,&bF7);
    bEStop.attachPop(onEStop, &bEStop);
    bDoorTest.attachPop(onDoorTest, &bDoorTest);
    bResetDiag.attachPop(onResetDiag, &bResetDiag);

    runStart = millis();
    for (uint8_t i = 0; i < 5; i++) logBuf[i][0] = 0;
    renderCabin();
    renderFloorBtns();
    renderDiag();
    renderLog();
    lMaintMsg.setText("");
}

void loop()
{
    airLoop(air_listen_list);

    static unsigned long lastDiag = 0;
    if (millis() - lastDiag >= 500UL) {
        lastDiag = millis();
        if (st == ST_MOVING) motorTemp += 0.05f;
        else if (motorTemp > 28.0f) motorTemp -= 0.02f;
        if (motorTemp > 80.0f) motorTemp = 80.0f;
        renderDiag();
    }

    if (eStopActive) return;
    unsigned long el = millis() - stateEnter;

    switch (st) {
        case ST_IDLE: {
            int8_t nxt = pickNext();
            if (nxt >= 0) {
                dir = (nxt > curFloor) ? 1 : -1;
                enterState(ST_DOOR_CLOSING);
            }
            break;
        }
        case ST_DOOR_CLOSING:
            if (el >= 1000UL) { doorCycles++; enterState(ST_MOVING); }
            break;
        case ST_MOVING:
            if (el >= 1500UL) {
                curFloor += dir;
                if (curFloor > 7) curFloor = 7;
                if (pending[curFloor]) {
                    pending[curFloor] = false;
                    enterState(ST_DOOR_OPENING);
                    char tmp[16];
                    snprintf(tmp, sizeof(tmp), "Stop %u", curFloor);
                    addLog(tmp);
                } else {
                    int8_t nxt = pickNext();
                    if (nxt < 0) { dir = 0; enterState(ST_IDLE); }
                    else {
                        dir = (nxt > curFloor) ? 1 : (nxt < curFloor ? -1 : 0);
                        enterState(ST_MOVING);
                    }
                }
            }
            break;
        case ST_DOOR_OPENING:
            if (el >= 800UL) { enterState(ST_DOOR_OPEN); }
            break;
        case ST_DOOR_OPEN:
            if (el >= 2000UL) {
                int8_t nxt = pickNext();
                if (nxt < 0) { dir = 0; enterState(ST_IDLE); }
                else {
                    dir = (nxt > curFloor) ? 1 : -1;
                    enterState(ST_DOOR_CLOSING);
                }
            }
            break;
        default: break;
    }
}
