/**
 * 16_MedicalMonitor  --  5 sayfali hasta basi monitor
 *
 *   Screen1 Vitals  : HR, SpO2, BP (sis/dia), Temp - 4 buyuk kart + arrhythmia
 *   Screen2 ECG     : P-QRS-T benzeri waveform graph (mock)
 *   Screen3 Alarms  : son 5 alarm, ACK butonu
 *   Screen4 Trend   : son N HR ornegi grafik
 *   Screen5 Patient : isim, yas, alerji, baslangic zamani + alarm esiklerini SET
 *
 *   Cok-esikli alarm: HR < 50 bradikardi, > 120 takikardi, SpO2 < 92, vs.
 */

#include <Airhmi.h>

/* Vitals */
AirLabel  lHR = AirLabel("lHR");
AirLabel  lSpO2 = AirLabel("lSpO2");
AirLabel  lBP = AirLabel("lBP");
AirLabel  lTemp = AirLabel("lTemp");
AirLabel  lRhythm = AirLabel("lRhythm");

/* ECG */
AirGraph  gECG = AirGraph("gECG");

/* Alarms */
AirLabel  lAl0 = AirLabel("lAl0");
AirLabel  lAl1 = AirLabel("lAl1");
AirLabel  lAl2 = AirLabel("lAl2");
AirLabel  lAl3 = AirLabel("lAl3");
AirLabel  lAl4 = AirLabel("lAl4");
AirButton bAck = AirButton("bAck");

/* Trend */
AirGraph  gHR = AirGraph("gHR");

/* Patient */
AirLabel  lPName = AirLabel("lPName");
AirLabel  lPAge = AirLabel("lPAge");
AirLabel  lPAllergy = AirLabel("lPAllergy");
AirSlider sHRMin = AirSlider("sHRMin");
AirSlider sHRMax = AirSlider("sHRMax");
AirLabel  lHRThr = AirLabel("lHRThr");

AirBuzzer buz = AirBuzzer("buz");

uint8_t hr = 78;
uint8_t spO2 = 98;
uint8_t bpSys = 120, bpDia = 80;
float   tempC = 36.7f;
uint8_t hrMin = 50, hrMax = 120;
uint8_t spO2Min = 92;
uint8_t arrCounter = 0;
unsigned long lastTick = 0;
unsigned long lastBeat = 0;
uint16_t beatPhase = 0;

const uint32_t COL_RED = 0xFFFF0000UL;
const uint32_t COL_OFF = 0xFF202020UL;

char alarmBuf[5][24];

void renderVitals()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u", hr); lHR.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u", spO2); lSpO2.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u/%u", bpSys, bpDia); lBP.setText(tmp);
    dtostrf(tempC, 0, 1, tmp); strcat(tmp, " C"); lTemp.setText(tmp);
    bool brady = hr < hrMin;
    bool tachy = hr > hrMax;
    bool hypox = spO2 < spO2Min;
    if (brady) lRhythm.setText("BRADYCARDIA");
    else if (tachy) lRhythm.setText("TACHYCARDIA");
    else if (hypox) lRhythm.setText("HYPOXEMIA");
    else lRhythm.setText("NSR");
}

void renderAlarms()
{
    AirLabel *labels[5] = { &lAl0, &lAl1, &lAl2, &lAl3, &lAl4 };
    for (uint8_t i = 0; i < 5; i++) labels[i]->setText(alarmBuf[i]);
}

void renderPatient()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "HR %u..%u", hrMin, hrMax); lHRThr.setText(tmp);
}

void addAlarm(const char *msg)
{
    for (uint8_t i = 4; i > 0; i--) strcpy(alarmBuf[i], alarmBuf[i-1]);
    strncpy(alarmBuf[0], msg, 23);
    alarmBuf[0][23] = 0;
    renderAlarms();
}

void onAck(void *p)
{
    for (uint8_t i = 0; i < 5; i++) alarmBuf[i][0] = 0;
    renderAlarms();
}

void onHRMin(void *p)
{
    uint32_t v = 0; sHRMin.Get_Value(&v);
    if (v < 30) v = 30; if (v > 80) v = 80;
    hrMin = (uint8_t)v;
    renderPatient();
}
void onHRMax(void *p)
{
    uint32_t v = 0; sHRMax.Get_Value(&v);
    if (v < 90) v = 90; if (v > 180) v = 180;
    hrMax = (uint8_t)v;
    renderPatient();
}

AirTouch *air_listen_list[] = {
    &bAck, &sHRMin, &sHRMax,
    NULL
};

uint8_t ecgValue(uint16_t phase)
{
    if (phase < 30) return 100;
    if (phase < 35) return 110;
    if (phase < 40) return 95;
    if (phase < 50) return 100;
    if (phase < 55) return 60;
    if (phase < 60) return 200;
    if (phase < 65) return 30;
    if (phase < 70) return 100;
    if (phase < 90) return 100;
    if (phase < 110) return 130;
    return 100;
}

void setup()
{
    Serial.begin(115200);
    airInit();
    bAck.attachPop(onAck, &bAck);
    sHRMin.attachPop(onHRMin, &sHRMin);
    sHRMax.attachPop(onHRMax, &sHRMax);

    sHRMin.Set_Value(hrMin);
    sHRMax.Set_Value(hrMax);
    lPName.setText("DOE, JOHN");
    lPAge.setText("Age: 54");
    lPAllergy.setText("Allergy: Penicillin");
    for (uint8_t i = 0; i < 5; i++) alarmBuf[i][0] = 0;
    addAlarm("Monitor start");
    renderVitals(); renderAlarms(); renderPatient();
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastBeat >= 50UL) {
        lastBeat = millis();
        uint16_t period = 60000UL / (hr ? hr : 60) / 50;
        beatPhase = (beatPhase + 1) % (period < 120 ? 120 : period);
        gECG.addValue(ecgValue(beatPhase), 0);
    }

    if (millis() - lastTick < 1000UL) return;
    lastTick = millis();

    int8_t hrDelta = random(7) - 3;
    int16_t newHr = hr + hrDelta;
    if (newHr < 35) newHr = 35;
    if (newHr > 180) newHr = 180;
    hr = (uint8_t)newHr;

    int8_t spO2Delta = random(3) - 1;
    int16_t newSpO2 = spO2 + spO2Delta;
    if (newSpO2 < 80) newSpO2 = 80;
    if (newSpO2 > 100) newSpO2 = 100;
    spO2 = (uint8_t)newSpO2;

    bpSys = 110 + random(20);
    bpDia = 70 + random(15);
    tempC += (random(20) - 10) / 100.0f;
    if (tempC < 35.0f) tempC = 35.0f;
    if (tempC > 39.5f) tempC = 39.5f;

    arrCounter++;
    if (arrCounter > 30) {
        arrCounter = 0;
        if (random(3) == 0) {
            hr = random(2) ? 35 : 150;
            addAlarm("Arrhythmia");
            buz.Set_Buzzer(300);
        }
    }

    if (hr < hrMin) { addAlarm("BRADYCARDIA"); buz.Set_Buzzer(80); }
    if (hr > hrMax) { addAlarm("TACHYCARDIA"); buz.Set_Buzzer(80); }
    if (spO2 < spO2Min) { addAlarm("Low SpO2"); buz.Set_Buzzer(120); }
    if (tempC > 38.5f) addAlarm("Fever");

    gHR.addValue(hr, 0);
    renderVitals();
}
