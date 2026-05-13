/**
 * 02_IndustrialControl  --  4 sayfali endustriyel uretim kontrol paneli
 *
 *   Screen1 (Dashboard) : uretim sayaci, vardiya zamani, hat durumu
 *   Screen2 (Recipe)    : 5 recete preset, sec ve parametreleri uygula
 *   Screen3 (Alarms)    : 4 alarm zone gostergesi + ack butonu
 *   Screen4 (Settings)  : reset sayac, vardiya degistir, sistem reset
 *
 *   Sayfa gecisi panel-side ChangeScreenSet ile.
 */

#include <Airhmi.h>

/* Dashboard */
AirLabel  lProd      = AirLabel("lProd");
AirLabel  lShift     = AirLabel("lShift");
AirLabel  lLineState = AirLabel("lLineState");
AirButton bStartLine = AirButton("bStartLine");
AirButton bStopLine  = AirButton("bStopLine");

/* Recipe */
AirButton bRecipe1 = AirButton("bRecipe1");
AirButton bRecipe2 = AirButton("bRecipe2");
AirButton bRecipe3 = AirButton("bRecipe3");
AirButton bRecipe4 = AirButton("bRecipe4");
AirButton bRecipe5 = AirButton("bRecipe5");
AirLabel  lRecipeName = AirLabel("lRecipeName");
AirLabel  lRecipeP1   = AirLabel("lRecipeP1");
AirLabel  lRecipeP2   = AirLabel("lRecipeP2");

/* Alarms */
AirButton bAlarm1 = AirButton("bAlarm1");
AirButton bAlarm2 = AirButton("bAlarm2");
AirButton bAlarm3 = AirButton("bAlarm3");
AirButton bAlarm4 = AirButton("bAlarm4");
AirButton bAck    = AirButton("bAck");
AirBuzzer buz     = AirBuzzer("buz");

/* Settings */
AirButton bResetCount = AirButton("bResetCount");
AirButton bShiftDay   = AirButton("bShiftDay");
AirButton bShiftNight = AirButton("bShiftNight");

/* Recipe table (sketch-side) */
const char *RECIPE_NAMES[5] = { "Bottle 250", "Bottle 500", "Box A", "Box B", "Custom" };
const int   RECIPE_P1[5]    = {     250,            500,         100,    200,      50 };
const int   RECIPE_P2[5]    = {      80,            120,         200,    300,      40 };

uint16_t prodCount = 0;
bool lineRunning = false;
bool alarmFlags[4] = { false, false, false, false };
const char *shiftName = "DAY";
unsigned long lastTickMs = 0;

const uint32_t LED_OFF   = 0xFF202020UL;
const uint32_t LED_GREEN = 0xFF008000UL;
const uint32_t LED_RED   = 0xFFC00000UL;

void renderProd()
{
    char tmp[20];
    snprintf(tmp, sizeof(tmp), "%u pcs", prodCount);
    lProd.setText(tmp);
}

void renderLine()
{
    if (lineRunning) { lLineState.setText("RUNNING"); }
    else             { lLineState.setText("STOPPED"); }
}

void renderShift() { lShift.setText(shiftName); }

void renderAlarm(uint8_t i)
{
    AirButton *b = (i==0)?&bAlarm1:(i==1)?&bAlarm2:(i==2)?&bAlarm3:&bAlarm4;
    b->Set_background_color(alarmFlags[i] ? LED_RED : LED_OFF);
    b->setText(alarmFlags[i] ? "ALARM" : "OK");
}

void selectRecipe(uint8_t i)
{
    lRecipeName.setText(RECIPE_NAMES[i]);
    char tmp[20];
    snprintf(tmp, sizeof(tmp), "P1: %d", RECIPE_P1[i]); lRecipeP1.setText(tmp);
    snprintf(tmp, sizeof(tmp), "P2: %d", RECIPE_P2[i]); lRecipeP2.setText(tmp);
}

void onR1(void *p) { selectRecipe(0); }
void onR2(void *p) { selectRecipe(1); }
void onR3(void *p) { selectRecipe(2); }
void onR4(void *p) { selectRecipe(3); }
void onR5(void *p) { selectRecipe(4); }

void onStart(void *p) { lineRunning = true;  renderLine(); }
void onStop (void *p) { lineRunning = false; renderLine(); }

void onAck(void *p)
{
    for (uint8_t i = 0; i < 4; i++) { alarmFlags[i] = false; renderAlarm(i); }
}

void onResetCount(void *p) { prodCount = 0; renderProd(); }
void onShiftDay  (void *p) { shiftName = "DAY";   renderShift(); }
void onShiftNight(void *p) { shiftName = "NIGHT"; renderShift(); }

AirTouch *air_listen_list[] = {
    &bStartLine, &bStopLine,
    &bRecipe1, &bRecipe2, &bRecipe3, &bRecipe4, &bRecipe5,
    &bAck,
    &bResetCount, &bShiftDay, &bShiftNight,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bStartLine.attachPop(onStart, &bStartLine);
    bStopLine.attachPop (onStop,  &bStopLine);
    bRecipe1.attachPop(onR1, &bRecipe1); bRecipe2.attachPop(onR2, &bRecipe2);
    bRecipe3.attachPop(onR3, &bRecipe3); bRecipe4.attachPop(onR4, &bRecipe4);
    bRecipe5.attachPop(onR5, &bRecipe5);
    bAck.attachPop(onAck, &bAck);
    bResetCount.attachPop(onResetCount, &bResetCount);
    bShiftDay.attachPop  (onShiftDay,   &bShiftDay);
    bShiftNight.attachPop(onShiftNight, &bShiftNight);

    renderProd(); renderLine(); renderShift();
    selectRecipe(0);
    for (uint8_t i = 0; i < 4; i++) renderAlarm(i);
    lastTickMs = millis();
}

void loop()
{
    airLoop(air_listen_list);

    /* Hat calisinca her 3 sn'de bir uretim sayaci artar (simulasyon). */
    if (lineRunning && millis() - lastTickMs >= 3000UL)
    {
        lastTickMs = millis();
        prodCount++;
        renderProd();

        /* her 50 parcada simule bir alarm zonunu tetikle */
        if (prodCount % 50 == 0)
        {
            uint8_t z = (prodCount / 50) % 4;
            alarmFlags[z] = true;
            renderAlarm(z);
            buz.Set_Buzzer(300);
        }
    }
}
