/**
 * 22_GymTracker  --  5 sayfali fitness takip
 *
 *   Screen1 Today   : 5 egzersiz listesi (secili olan yesil) + ilerleme
 *   Screen2 Exercise: secili egzersiz adi + set/rep slider + KAYDET
 *   Screen3 Timer   : rest timer (60sn varsayilan), AirProgressBar count-down
 *   Screen4 Stats   : haftalik graph (gun bazinda toplam set)
 *   Screen5 Profile : kilo, hedef, AirShape (badge yer tutucu)
 *
 *   Yeni component: AirProgressBar (rest timer sayim).
 */

#include <Airhmi.h>

/* Today */
AirButton bE0 = AirButton("bE0");
AirButton bE1 = AirButton("bE1");
AirButton bE2 = AirButton("bE2");
AirButton bE3 = AirButton("bE3");
AirButton bE4 = AirButton("bE4");
AirLabel  lDoneTotal = AirLabel("lDoneTotal");

/* Exercise */
AirLabel  lExName = AirLabel("lExName");
AirSlider sSet = AirSlider("sSet");
AirLabel  lSet = AirLabel("lSet");
AirSlider sRep = AirSlider("sRep");
AirLabel  lRep = AirLabel("lRep");
AirButton bSave = AirButton("bSave");

/* Timer */
AirLabel  lRest = AirLabel("lRest");
AirProgressBar pbRest = AirProgressBar("pbRest");
AirButton bStartT = AirButton("bStartT");
AirButton bResetT = AirButton("bResetT");
AirSlider sRest = AirSlider("sRest");

/* Stats */
AirGraph  gWeek = AirGraph("gWeek");
AirLabel  lWeekly = AirLabel("lWeekly");

/* Profile */
AirLabel  lWeight = AirLabel("lWeight");
AirLabel  lGoal = AirLabel("lGoal");
AirSlider sWeight = AirSlider("sWeight");
AirSlider sGoal = AirSlider("sGoal");

AirBuzzer buz = AirBuzzer("buz");

const uint8_t EX_COUNT = 5;
const char *exName[EX_COUNT] = {
    "Bench Press", "Squat", "Deadlift", "Pull-up", "Plank"
};
uint8_t setCount[EX_COUNT] = { 0, 0, 0, 0, 0 };

uint8_t curEx = 0;
uint8_t newSet = 3, newRep = 10;
uint16_t restSec = 60;
uint16_t restRem = 60;
bool restRunning = false;
unsigned long restTickLast = 0;
uint8_t weekly[7] = { 8, 12, 0, 10, 15, 6, 0 };
uint8_t weekDay = 0;
uint16_t weight = 75;
uint16_t goal = 70;

const uint32_t COL_OFF = 0xFF202020UL;
const uint32_t COL_ON = 0xFF008000UL;

void renderToday()
{
    AirButton *btns[EX_COUNT] = { &bE0, &bE1, &bE2, &bE3, &bE4 };
    char tmp[24];
    for (uint8_t i = 0; i < EX_COUNT; i++) {
        snprintf(tmp, sizeof(tmp), "%s %ux", exName[i], setCount[i]);
        btns[i]->setText(tmp);
        btns[i]->Set_background_color(i == curEx ? COL_ON : COL_OFF);
    }
    uint16_t total = 0;
    for (uint8_t i = 0; i < EX_COUNT; i++) total += setCount[i];
    snprintf(tmp, sizeof(tmp), "Total Sets: %u", total);
    lDoneTotal.setText(tmp);
}

void renderExercise()
{
    lExName.setText(exName[curEx]);
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u sets", newSet); lSet.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u reps", newRep); lRep.setText(tmp);
}

void renderTimer()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u s", restRem);
    lRest.setText(tmp);
    uint8_t pct = restSec ? (uint8_t)((uint32_t)restRem * 100 / restSec) : 0;
    pbRest.Set_Value(pct);
    bStartT.setText(restRunning ? "PAUSE" : "START");
    bStartT.Set_background_color(restRunning ? COL_ON : COL_OFF);
}

void renderStats()
{
    char tmp[24];
    uint16_t weekTotal = 0;
    for (uint8_t i = 0; i < 7; i++) weekTotal += weekly[i];
    snprintf(tmp, sizeof(tmp), "Week %u sets", weekTotal);
    lWeekly.setText(tmp);
}

void renderProfile()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u kg", weight); lWeight.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u kg", goal); lGoal.setText(tmp);
}

void onE0(void *p) { curEx = 0; renderToday(); renderExercise(); }
void onE1(void *p) { curEx = 1; renderToday(); renderExercise(); }
void onE2(void *p) { curEx = 2; renderToday(); renderExercise(); }
void onE3(void *p) { curEx = 3; renderToday(); renderExercise(); }
void onE4(void *p) { curEx = 4; renderToday(); renderExercise(); }

void onSet(void *p)
{
    uint32_t v = 0; sSet.Get_Value(&v);
    if (v < 1) v = 1; if (v > 10) v = 10;
    newSet = v;
    renderExercise();
}
void onRep(void *p)
{
    uint32_t v = 0; sRep.Get_Value(&v);
    if (v < 1) v = 1; if (v > 30) v = 30;
    newRep = v;
    renderExercise();
}
void onSave(void *p)
{
    setCount[curEx] += newSet;
    weekly[weekDay] += newSet;
    if (weekly[weekDay] > 200) weekly[weekDay] = 200;
    buz.Set_Buzzer(80);
    restRem = restSec;
    restRunning = true;
    restTickLast = millis();
    renderToday(); renderTimer(); renderStats();
}

void onStartT(void *p)
{
    restRunning = !restRunning;
    if (restRunning) restTickLast = millis();
    renderTimer();
}
void onResetT(void *p) { restRunning = false; restRem = restSec; renderTimer(); }
void onRest(void *p)
{
    uint32_t v = 0; sRest.Get_Value(&v);
    if (v < 10) v = 10; if (v > 300) v = 300;
    restSec = v;
    restRem = v;
    renderTimer();
}

void onWeight(void *p)
{
    uint32_t v = 0; sWeight.Get_Value(&v);
    if (v < 30) v = 30; if (v > 200) v = 200;
    weight = v;
    renderProfile();
}
void onGoal(void *p)
{
    uint32_t v = 0; sGoal.Get_Value(&v);
    if (v < 30) v = 30; if (v > 200) v = 200;
    goal = v;
    renderProfile();
}

AirTouch *air_listen_list[] = {
    &bE0, &bE1, &bE2, &bE3, &bE4,
    &sSet, &sRep, &bSave,
    &bStartT, &bResetT, &sRest,
    &sWeight, &sGoal,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bE0.attachPop(onE0,&bE0); bE1.attachPop(onE1,&bE1);
    bE2.attachPop(onE2,&bE2); bE3.attachPop(onE3,&bE3);
    bE4.attachPop(onE4,&bE4);
    sSet.attachPop(onSet, &sSet);
    sRep.attachPop(onRep, &sRep);
    bSave.attachPop(onSave, &bSave);
    bStartT.attachPop(onStartT, &bStartT);
    bResetT.attachPop(onResetT, &bResetT);
    sRest.attachPop(onRest, &sRest);
    sWeight.attachPop(onWeight, &sWeight);
    sGoal.attachPop(onGoal, &sGoal);

    sSet.Set_Value(newSet);
    sRep.Set_Value(newRep);
    sRest.Set_Value(restSec);
    sWeight.Set_Value(weight);
    sGoal.Set_Value(goal);
    pbRest.Set_Value(100);
    for (uint8_t i = 0; i < 7; i++) gWeek.addValue((uint32_t)(weekly[i] * 6), 0);
    renderToday(); renderExercise(); renderTimer(); renderStats(); renderProfile();
}

void loop()
{
    airLoop(air_listen_list);

    if (restRunning && millis() - restTickLast >= 1000UL) {
        restTickLast += 1000UL;
        if (restRem > 0) restRem--;
        if (restRem == 0) {
            restRunning = false;
            buz.Set_Buzzer(400);
        }
        renderTimer();
    }
}
