/**
 * 08_WaterTank  --  Su tanki seviyesi + pompa kontrolu (HC-SR04)
 *
 *   Ultrasonik sensor su yuzeyine olan mesafeyi olcer; tankin toplam
 *   yuksekligi (TANK_H_CM) sabit kabul edilir, seviye yuzdesi:
 *      level% = (TANK_H_CM - distanceCm) * 100 / TANK_H_CM
 *
 *   Auto mod:
 *     level < LOW_TH (%30)  -> pompa AC
 *     level > HIGH_TH (%95) -> pompa KAPAT + buzzer alarm
 *
 * Donanim:
 *   HC-SR04 TRIG -> D9, ECHO -> D10, VCC -> 5V, GND -> GND
 *   Pompa modul IN -> Panel GPIO_0
 */

#include <Airhmi.h>

const uint8_t TRIG_PIN  = 9;
const uint8_t ECHO_PIN  = 10;
const uint8_t TANK_H_CM = 100;
const uint8_t LOW_TH    = 30;
const uint8_t HIGH_TH   = 95;

AirGpio        gpio;
AirProgressBar pbar    = AirProgressBar("pbar");
AirGraph       gph     = AirGraph("gph");
AirLabel       lLevel  = AirLabel("lLevel");
AirLabel       lDist   = AirLabel("lDist");
AirButton      bPump   = AirButton("bPump");
AirButton      bAuto   = AirButton("bAuto");
AirButton      bOn     = AirButton("bOn");
AirButton      bOff    = AirButton("bOff");
AirBuzzer      buz     = AirBuzzer("buz");

bool autoMode = true;
bool pumpOn   = false;
unsigned long lastRead = 0;
unsigned long lastBeep = 0;

void renderPump()
{
    bPump.Set_background_color(pumpOn ? 0xFF00C000UL : 0xFF202020UL);
    bPump.setText(pumpOn ? "PUMP ON" : "PUMP OFF");
}

void setPump(bool on)
{
    if (on == pumpOn) return;
    pumpOn = on;
    gpio.digital_write(0, on ? 1 : 0);
    renderPump();
}

float measureCm()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    unsigned long dur = pulseIn(ECHO_PIN, HIGH, 30000UL);
    if (dur == 0) return -1.0f;
    return dur * 0.0343f / 2.0f;
}

void onAuto(void *p) { autoMode = !autoMode; }
void onOn  (void *p) { autoMode = false; setPump(true); }
void onOff (void *p) { autoMode = false; setPump(false); }

AirTouch *air_listen_list[] = { &bAuto, &bOn, &bOff, NULL };

void setup()
{
    Serial.begin(115200);
    airInit();
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    bAuto.attachPop(onAuto, &bAuto);
    bOn.attachPop  (onOn,   &bOn);
    bOff.attachPop (onOff,  &bOff);
    setPump(false);
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastRead < 500UL) return;
    lastRead = millis();

    float cm = measureCm();
    if (cm < 0 || cm > TANK_H_CM + 30) return;       /* hata */

    int level = (int)((TANK_H_CM - cm) * 100.0f / TANK_H_CM);
    if (level < 0) level = 0; else if (level > 100) level = 100;

    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%d %%", level);
    lLevel.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%d cm", (int)cm);
    lDist.setText(tmp);

    pbar.Set_Value((uint32_t)level);
    gph.addValue(0, (uint32_t)level);

    if (autoMode)
    {
        if (level < LOW_TH)  setPump(true);
        if (level > HIGH_TH) setPump(false);
    }

    if (level > HIGH_TH && (millis() - lastBeep > 2000UL))
    {
        lastBeep = millis();
        buz.Set_Buzzer(400);
    }
}
