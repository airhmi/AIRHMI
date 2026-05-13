/**
 * 10_PomodoroTaskManager  --  3 sayfali pomodoro + gorev takip
 *
 *   Screen1 Timer  : 25 dk pomodoro sayim, START / PAUSE / RESET
 *                    + WORK/BREAK durum, kalan sure mm:ss
 *   Screen2 Tasks  : 5 gorev (her biri done toggle button)
 *   Screen3 Stats  : tamamlanan pomodoro sayisi, tamamlanan gorev sayisi
 *
 *   25 dk WORK -> 5 dk BREAK -> WORK dongusu otomatik.
 *   Donanim gerekmez (mock); sade sure simulasyonu.
 */

#include <Airhmi.h>

/* Timer */
AirLabel  lTimer = AirLabel("lTimer");
AirLabel  lPhase = AirLabel("lPhase");
AirButton bStart = AirButton("bStart");
AirButton bPause = AirButton("bPause");
AirButton bReset = AirButton("bReset");

/* Tasks */
AirButton bT0 = AirButton("bT0");
AirButton bT1 = AirButton("bT1");
AirButton bT2 = AirButton("bT2");
AirButton bT3 = AirButton("bT3");
AirButton bT4 = AirButton("bT4");

/* Stats */
AirLabel lPomos = AirLabel("lPomos");
AirLabel lDone = AirLabel("lDone");

AirBuzzer buz = AirBuzzer("buz");

const uint16_t WORK_SEC  = 25 * 60;
const uint16_t BREAK_SEC = 5 * 60;

bool running = false;
bool isWork  = true;
uint16_t remain = WORK_SEC;
uint16_t pomos  = 0;

const char *taskName[5] = {
    "Read docs", "Write code", "Run tests", "Review PR", "Stand up"
};
bool taskDone[5] = { false, false, false, false, false };

unsigned long lastTick = 0;

const uint32_t COL_OFF  = 0xFF202020UL;
const uint32_t COL_DONE = 0xFF008000UL;

void renderTimer()
{
    char tmp[8];
    snprintf(tmp, sizeof(tmp), "%02u:%02u", remain / 60, remain % 60);
    lTimer.setText(tmp);
    lPhase.setText(isWork ? "WORK" : "BREAK");
}

void renderTask(uint8_t i)
{
    AirButton *b = (i==0)?&bT0:(i==1)?&bT1:(i==2)?&bT2:(i==3)?&bT3:&bT4;
    char tmp[24];
    snprintf(tmp, sizeof(tmp), "%s\n%s", taskName[i], taskDone[i]?"DONE":"...");
    b->setText(tmp);
    b->Set_background_color(taskDone[i] ? COL_DONE : COL_OFF);
}

void renderStats()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u", pomos); lPomos.setText(tmp);
    uint8_t cnt = 0;
    for (uint8_t i = 0; i < 5; i++) if (taskDone[i]) cnt++;
    snprintf(tmp, sizeof(tmp), "%u / 5", cnt); lDone.setText(tmp);
}

void onStart(void *p) { running = true;  lastTick = millis(); }
void onPause(void *p) { running = false; }
void onReset(void *p) { running = false; isWork = true; remain = WORK_SEC; renderTimer(); }

void onT0(void *p) { taskDone[0]=!taskDone[0]; renderTask(0); renderStats(); }
void onT1(void *p) { taskDone[1]=!taskDone[1]; renderTask(1); renderStats(); }
void onT2(void *p) { taskDone[2]=!taskDone[2]; renderTask(2); renderStats(); }
void onT3(void *p) { taskDone[3]=!taskDone[3]; renderTask(3); renderStats(); }
void onT4(void *p) { taskDone[4]=!taskDone[4]; renderTask(4); renderStats(); }

AirTouch *air_listen_list[] = {
    &bStart, &bPause, &bReset,
    &bT0, &bT1, &bT2, &bT3, &bT4,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bStart.attachPop(onStart, &bStart);
    bPause.attachPop(onPause, &bPause);
    bReset.attachPop(onReset, &bReset);
    bT0.attachPop(onT0, &bT0); bT1.attachPop(onT1, &bT1);
    bT2.attachPop(onT2, &bT2); bT3.attachPop(onT3, &bT3);
    bT4.attachPop(onT4, &bT4);

    renderTimer();
    for (uint8_t i = 0; i < 5; i++) renderTask(i);
    renderStats();
}

void loop()
{
    airLoop(air_listen_list);

    if (!running) return;

    if (millis() - lastTick < 1000UL) return;
    lastTick += 1000UL;

    if (remain > 0) {
        remain--;
        renderTimer();
    }
    if (remain == 0) {
        buz.Set_Buzzer(400);
        if (isWork) {
            pomos++;
            isWork = false;
            remain = BREAK_SEC;
        } else {
            isWork = true;
            remain = WORK_SEC;
        }
        renderTimer();
        renderStats();
    }
}
