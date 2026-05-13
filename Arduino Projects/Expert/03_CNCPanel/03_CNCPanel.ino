/**
 * 03_CNCPanel  --  4 sayfali CNC kontrol paneli
 *
 *   Screen1 Jog          : X+/X-/Y+/Y-/Z+/Z- jog butonlari + Home + koordinatlar
 *   Screen2 Auto         : Run / Pause / Stop + ilerleme barı + progress %
 *   Screen3 Settings     : feed rate slider, accel slider
 *   Screen4 Diagnostics  : motor enable + limit sensor mock + estop
 */

#include <Airhmi.h>

/* Jog */
AirButton bXp = AirButton("bXp"); AirButton bXm = AirButton("bXm");
AirButton bYp = AirButton("bYp"); AirButton bYm = AirButton("bYm");
AirButton bZp = AirButton("bZp"); AirButton bZm = AirButton("bZm");
AirButton bHome = AirButton("bHome");
AirLabel  lX = AirLabel("lX");
AirLabel  lY = AirLabel("lY");
AirLabel  lZ = AirLabel("lZ");

/* Auto */
AirButton bRun = AirButton("bRun");
AirButton bPause = AirButton("bPause");
AirButton bStop = AirButton("bStop");
AirProgressBar pbar = AirProgressBar("pbar");
AirLabel lProgress = AirLabel("lProgress");

/* Settings */
AirSlider sFeed  = AirSlider("sFeed");
AirSlider sAccel = AirSlider("sAccel");
AirLabel  lFeed  = AirLabel("lFeed");
AirLabel  lAccel = AirLabel("lAccel");

/* Diagnostics */
AirButton bMotorEn = AirButton("bMotorEn");
AirButton bEstop   = AirButton("bEstop");
AirLabel  lLimitX  = AirLabel("lLimitX");
AirLabel  lLimitY  = AirLabel("lLimitY");
AirLabel  lLimitZ  = AirLabel("lLimitZ");

float posX = 0.0f, posY = 0.0f, posZ = 0.0f;
const float JOG_STEP = 1.0f;
bool autoRun = false, autoPaused = false;
uint8_t progress = 0;
unsigned long lastAutoMs = 0;
uint16_t feedRate = 1000;
uint8_t  accel = 50;
bool motorEn = true, estop = false;

const uint32_t LED_OFF = 0xFF202020UL;
const uint32_t LED_GREEN = 0xFF008000UL;
const uint32_t LED_RED = 0xFFC00000UL;

void renderPos()
{
    char tmp[16];
    dtostrf(posX, 0, 1, tmp); strcat(tmp, " mm"); lX.setText(tmp);
    dtostrf(posY, 0, 1, tmp); strcat(tmp, " mm"); lY.setText(tmp);
    dtostrf(posZ, 0, 1, tmp); strcat(tmp, " mm"); lZ.setText(tmp);
}

void renderProgress()
{
    pbar.Set_Value(progress);
    char tmp[12];
    snprintf(tmp, sizeof(tmp), "%u %%", progress);
    lProgress.setText(tmp);
}

void renderMotor()
{
    bMotorEn.Set_background_color(motorEn ? LED_GREEN : LED_OFF);
    bMotorEn.setText(motorEn ? "Motors ON" : "Motors OFF");
}

void renderEstop()
{
    bEstop.Set_background_color(estop ? LED_RED : LED_OFF);
    bEstop.setText(estop ? "E-STOP" : "Ready");
}

void onXp(void *p) { posX += JOG_STEP; renderPos(); }
void onXm(void *p) { posX -= JOG_STEP; renderPos(); }
void onYp(void *p) { posY += JOG_STEP; renderPos(); }
void onYm(void *p) { posY -= JOG_STEP; renderPos(); }
void onZp(void *p) { posZ += JOG_STEP; renderPos(); }
void onZm(void *p) { posZ -= JOG_STEP; renderPos(); }
void onHome(void *p) { posX = posY = posZ = 0; renderPos(); }

void onRun(void *p)   { autoRun = true; autoPaused = false; lastAutoMs = millis(); }
void onPause(void *p) { autoPaused = !autoPaused; }
void onStop(void *p)  { autoRun = false; autoPaused = false; progress = 0; renderProgress(); }

void onFeed(void *p)
{
    uint32_t v = 0; sFeed.Get_Value(&v);
    feedRate = (uint16_t)v;
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u mm/min", feedRate);
    lFeed.setText(tmp);
}

void onAccel(void *p)
{
    uint32_t v = 0; sAccel.Get_Value(&v);
    accel = (uint8_t)v;
    char tmp[12];
    snprintf(tmp, sizeof(tmp), "%u %%", accel);
    lAccel.setText(tmp);
}

void onMotorEn(void *p) { motorEn = !motorEn; renderMotor(); }
void onEstop(void *p)
{
    estop = !estop;
    if (estop) { autoRun = false; motorEn = false; renderMotor(); }
    renderEstop();
}

AirTouch *air_listen_list[] = {
    &bXp, &bXm, &bYp, &bYm, &bZp, &bZm, &bHome,
    &bRun, &bPause, &bStop,
    &sFeed, &sAccel,
    &bMotorEn, &bEstop,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bXp.attachPop(onXp, &bXp); bXm.attachPop(onXm, &bXm);
    bYp.attachPop(onYp, &bYp); bYm.attachPop(onYm, &bYm);
    bZp.attachPop(onZp, &bZp); bZm.attachPop(onZm, &bZm);
    bHome.attachPop(onHome, &bHome);
    bRun.attachPop(onRun, &bRun); bPause.attachPop(onPause, &bPause);
    bStop.attachPop(onStop, &bStop);
    sFeed.attachPop(onFeed, &sFeed);
    sAccel.attachPop(onAccel, &sAccel);
    bMotorEn.attachPop(onMotorEn, &bMotorEn);
    bEstop.attachPop(onEstop, &bEstop);

    sFeed.Set_Value(feedRate); onFeed(NULL);
    sAccel.Set_Value(accel);   onAccel(NULL);
    renderPos(); renderProgress(); renderMotor(); renderEstop();
    lLimitX.setText("OK"); lLimitY.setText("OK"); lLimitZ.setText("OK");
}

void loop()
{
    airLoop(air_listen_list);

    if (autoRun && !autoPaused && !estop && motorEn)
    {
        if (millis() - lastAutoMs >= 500UL)
        {
            lastAutoMs = millis();
            if (progress < 100) { progress++; renderProgress(); }
            else { autoRun = false; }
        }
    }
}
