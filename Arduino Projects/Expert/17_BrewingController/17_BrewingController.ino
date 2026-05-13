/**
 * 17_BrewingController  --  5 sayfali bira yapim kontrolu
 *
 *   Screen1 Mash    : current temp + target + step name + remaining time
 *   Screen2 Boil    : boil timer + hop additions count + heater state
 *   Screen3 Ferment : ferment temp/days + airlock count
 *   Screen4 Recipe  : 3 recete + step listesi
 *   Screen5 Log     : son 5 olay
 *
 *   Recipe: 4 stepli sicaklik profili (60->68->75->100). Her step kalan dk.
 */

#include <Airhmi.h>

/* Mash */
AirLabel  lMashTemp = AirLabel("lMashTemp");
AirLabel  lMashTgt = AirLabel("lMashTgt");
AirLabel  lStepName = AirLabel("lStepName");
AirLabel  lStepRem = AirLabel("lStepRem");
AirButton bStartMash = AirButton("bStartMash");
AirButton bStopMash = AirButton("bStopMash");
AirButton bSkipStep = AirButton("bSkipStep");

/* Boil */
AirLabel  lBoilT = AirLabel("lBoilT");
AirLabel  lBoilRem = AirLabel("lBoilRem");
AirLabel  lHops = AirLabel("lHops");
AirLabel  lHeater = AirLabel("lHeater");
AirButton bStartBoil = AirButton("bStartBoil");

/* Ferment */
AirLabel  lFermT = AirLabel("lFermT");
AirLabel  lFermDays = AirLabel("lFermDays");
AirLabel  lAirlock = AirLabel("lAirlock");

/* Recipe */
AirButton bR0 = AirButton("bR0");
AirButton bR1 = AirButton("bR1");
AirButton bR2 = AirButton("bR2");
AirLabel  lRecipeName = AirLabel("lRecipeName");
AirLabel  lRecipeSteps = AirLabel("lRecipeSteps");

/* Log */
AirLabel lL0 = AirLabel("lL0");
AirLabel lL1 = AirLabel("lL1");
AirLabel lL2 = AirLabel("lL2");
AirLabel lL3 = AirLabel("lL3");
AirLabel lL4 = AirLabel("lL4");

AirBuzzer buz = AirBuzzer("buz");

struct Step { const char *name; uint8_t targetC; uint8_t mins; };
struct Recipe { const char *name; uint8_t stepCount; const Step *steps; };

const Step steps_pale[4]  = { {"Strike", 50, 1}, {"Beta", 65, 5}, {"Alpha", 72, 5}, {"Mashout", 76, 1} };
const Step steps_ipa[4]   = { {"Strike", 52, 1}, {"Beta", 64, 4}, {"Alpha", 68, 6}, {"Mashout", 78, 1} };
const Step steps_stout[4] = { {"Strike", 55, 1}, {"Beta", 66, 4}, {"Alpha", 75, 6}, {"Mashout", 80, 1} };

const Recipe recipes[3] = {
    { "Pale Ale", 4, steps_pale },
    { "IPA",      4, steps_ipa },
    { "Stout",    4, steps_stout }
};
uint8_t curRecipe = 0;
uint8_t curStep = 0;
bool mashActive = false;
unsigned long stepStart = 0;
float mashTemp = 22.0f;
bool heater = false;

bool boilActive = false;
unsigned long boilStart = 0;
const uint8_t BOIL_MIN = 60;
uint8_t hopAdded = 0;
const uint8_t HOP_TIMES[3] = { 60, 30, 5 };

float fermTemp = 18.5f;
uint16_t fermDays = 0;
uint16_t airlockCount = 0;

const uint32_t COL_OFF = 0xFF202020UL;
const uint32_t COL_ON = 0xFF008000UL;
const uint32_t COL_RED = 0xFFFF0000UL;

unsigned long lastTick = 0;
unsigned long lastFermTick = 0;
char logBuf[5][24];

void renderMash()
{
    char tmp[16];
    dtostrf(mashTemp, 0, 1, tmp); strcat(tmp, " C"); lMashTemp.setText(tmp);
    if (mashActive) {
        const Recipe *r = &recipes[curRecipe];
        snprintf(tmp, sizeof(tmp), "%u C", r->steps[curStep].targetC); lMashTgt.setText(tmp);
        lStepName.setText(r->steps[curStep].name);
        unsigned long el = (millis() - stepStart) / 1000UL;
        unsigned long total = r->steps[curStep].mins * 60UL;
        unsigned long rem = el >= total ? 0 : total - el;
        snprintf(tmp, sizeof(tmp), "%lu s", rem); lStepRem.setText(tmp);
    } else {
        lMashTgt.setText("--");
        lStepName.setText("IDLE");
        lStepRem.setText("--");
    }
}

void renderBoil()
{
    char tmp[16];
    dtostrf(mashTemp, 0, 1, tmp); strcat(tmp, " C"); lBoilT.setText(tmp);
    if (boilActive) {
        unsigned long el = (millis() - boilStart) / 1000UL;
        unsigned long total = BOIL_MIN * 60UL;
        unsigned long rem = el >= total ? 0 : total - el;
        snprintf(tmp, sizeof(tmp), "%lu s", rem); lBoilRem.setText(tmp);
    } else {
        lBoilRem.setText("--");
    }
    snprintf(tmp, sizeof(tmp), "%u/3", hopAdded); lHops.setText(tmp);
    lHeater.setText(heater ? "ON" : "OFF");
}

void renderFerment()
{
    char tmp[16];
    dtostrf(fermTemp, 0, 1, tmp); strcat(tmp, " C"); lFermT.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u days", fermDays); lFermDays.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u burps", airlockCount); lAirlock.setText(tmp);
}

void renderRecipe()
{
    AirButton *btns[3] = { &bR0, &bR1, &bR2 };
    for (uint8_t i = 0; i < 3; i++) {
        btns[i]->setText(recipes[i].name);
        btns[i]->Set_background_color(i == curRecipe ? COL_ON : COL_OFF);
    }
    lRecipeName.setText(recipes[curRecipe].name);
    char tmp[24];
    snprintf(tmp, sizeof(tmp), "%u steps", recipes[curRecipe].stepCount);
    lRecipeSteps.setText(tmp);
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

void onStartMash(void *p)
{
    if (mashActive) return;
    mashActive = true;
    curStep = 0;
    stepStart = millis();
    addLog("Mash start");
    buz.Set_Buzzer(120);
}

void onStopMash(void *p) { mashActive = false; heater = false; addLog("Mash stop"); }

void onSkipStep(void *p)
{
    if (!mashActive) return;
    curStep++;
    if (curStep >= recipes[curRecipe].stepCount) {
        mashActive = false;
        addLog("Mash done");
        buz.Set_Buzzer(300);
    } else {
        stepStart = millis();
        char tmp[20];
        snprintf(tmp, sizeof(tmp), "Step %u", curStep);
        addLog(tmp);
    }
}

void onStartBoil(void *p)
{
    boilActive = !boilActive;
    if (boilActive) { boilStart = millis(); hopAdded = 0; addLog("Boil start"); }
    else { addLog("Boil stop"); }
}

void onR0(void *p) { curRecipe = 0; renderRecipe(); addLog("Recipe Pale"); }
void onR1(void *p) { curRecipe = 1; renderRecipe(); addLog("Recipe IPA"); }
void onR2(void *p) { curRecipe = 2; renderRecipe(); addLog("Recipe Stout"); }

AirTouch *air_listen_list[] = {
    &bStartMash, &bStopMash, &bSkipStep,
    &bStartBoil,
    &bR0, &bR1, &bR2,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bStartMash.attachPop(onStartMash, &bStartMash);
    bStopMash.attachPop(onStopMash, &bStopMash);
    bSkipStep.attachPop(onSkipStep, &bSkipStep);
    bStartBoil.attachPop(onStartBoil, &bStartBoil);
    bR0.attachPop(onR0, &bR0);
    bR1.attachPop(onR1, &bR1);
    bR2.attachPop(onR2, &bR2);

    for (uint8_t i = 0; i < 5; i++) logBuf[i][0] = 0;
    addLog("System ready");
    renderMash(); renderBoil(); renderFerment(); renderRecipe();
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastTick < 500UL) return;
    float dt = (millis() - lastTick) / 1000.0f;
    lastTick = millis();

    if (mashActive) {
        const Step *s = &recipes[curRecipe].steps[curStep];
        heater = mashTemp < s->targetC - 0.3f;
        if (heater) mashTemp += 0.6f * dt;
        else if (mashTemp > s->targetC) mashTemp -= 0.1f * dt;
        unsigned long el = (millis() - stepStart) / 1000UL;
        if (el >= (unsigned long)s->mins * 60UL && mashTemp >= s->targetC - 0.5f) {
            curStep++;
            if (curStep >= recipes[curRecipe].stepCount) {
                mashActive = false;
                heater = false;
                addLog("Mash done");
                buz.Set_Buzzer(300);
            } else {
                stepStart = millis();
                char tmp[20];
                snprintf(tmp, sizeof(tmp), "Step %u", curStep);
                addLog(tmp);
                buz.Set_Buzzer(120);
            }
        }
    } else if (boilActive) {
        if (mashTemp < 100.0f) mashTemp += 0.8f * dt;
        else mashTemp = 100.0f;
        heater = mashTemp < 100.0f;
        unsigned long el = (millis() - boilStart) / 1000UL;
        unsigned long mins = el / 60UL;
        for (uint8_t i = 0; i < 3; i++) {
            if (hopAdded == i && mins >= (BOIL_MIN - HOP_TIMES[i])) {
                hopAdded++;
                char tmp[20];
                snprintf(tmp, sizeof(tmp), "Hop %u", i+1);
                addLog(tmp);
                buz.Set_Buzzer(200);
            }
        }
        if (mins >= BOIL_MIN) {
            boilActive = false;
            heater = false;
            addLog("Boil done");
            buz.Set_Buzzer(500);
        }
    } else {
        if (mashTemp > 22.0f) mashTemp -= 0.05f * dt;
        heater = false;
    }

    if (millis() - lastFermTick >= 5000UL) {
        lastFermTick = millis();
        fermTemp = 18.0f + (random(20) / 10.0f);
        airlockCount++;
        if (random(20) == 0) fermDays++;
    }

    renderMash(); renderBoil(); renderFerment();
}
