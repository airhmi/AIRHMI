/**
 * 12_PIDTuning  --  PID parametre ayar simülatörü
 *
 *   Donanim YOK -- icimizde basit bir 1. derece (kapali devre tipi
 *   sicaklik) proses simulasyonu calisir. PID kontrolcusu Kp/Ki/Kd
 *   slider'lari ile ayarlanir, setpoint slider ile hedef belirlenir.
 *   Step input gozlemleyebilirsin (Apply Step ile).
 *
 *   Proses modeli:
 *     pv[k+1] = pv[k] + dt * (a * heater - b * pv[k])
 *   heater = clamp(PID, 0, 100)
 *
 * Panel tarafi (12_PIDTuning.ahi):
 *   sKp / sKi / sKd     : 0..100 (her birinin x10 olarak yorumlanir
 *                          -> Kp=2.5 icin slider=25)
 *   sSetpoint           : 0..200 (set point)
 *   gPv                 : EveGauge process value (0..200)
 *   lPv / lSp           : ELabelBox
 *   gph                 : EGraph 2 line (sp + pv)
 *   bStep               : step input apply
 *   bReset              : pv = 0
 */

#include <Airhmi.h>

AirSlider sKp = AirSlider("sKp");
AirSlider sKi = AirSlider("sKi");
AirSlider sKd = AirSlider("sKd");
AirSlider sSp = AirSlider("sSp");

AirGauge  gPv  = AirGauge("gPv");
AirLabel  lPv  = AirLabel("lPv");
AirLabel  lSp  = AirLabel("lSp");
AirLabel  lOut = AirLabel("lOut");
AirGraph  gph  = AirGraph("gph");

AirButton bStep  = AirButton("bStep");
AirButton bReset = AirButton("bReset");

float Kp = 2.5f, Ki = 0.5f, Kd = 0.1f;
float setpoint = 100.0f;
float pv = 0.0f;
float intErr = 0.0f, lastErr = 0.0f;
const float DT_S = 0.1f;
const float A = 1.5f;     /* heater gain */
const float B = 0.05f;    /* cooling */

unsigned long lastTick = 0;
unsigned long lastUi   = 0;

void readKxx()
{
    uint32_t v = 0;
    sKp.Get_Value(&v); Kp = v / 10.0f;
    sKi.Get_Value(&v); Ki = v / 100.0f;
    sKd.Get_Value(&v); Kd = v / 100.0f;
    sSp.Get_Value(&v); setpoint = (float)v;
}

void onAnyParam(void *p) { readKxx(); }
void onStep    (void *p) { setpoint = 150.0f; sSp.Set_Value(150); }
void onReset   (void *p) { pv = 0.0f; intErr = 0.0f; lastErr = 0.0f; }

AirTouch *air_listen_list[] = { &sKp, &sKi, &sKd, &sSp, &bStep, &bReset, NULL };

void setup()
{
    Serial.begin(115200);
    airInit();
    sKp.attachPop(onAnyParam, &sKp);
    sKi.attachPop(onAnyParam, &sKi);
    sKd.attachPop(onAnyParam, &sKd);
    sSp.attachPop(onAnyParam, &sSp);
    bStep.attachPop(onStep,   &bStep);
    bReset.attachPop(onReset, &bReset);

    sKp.Set_Value(25); sKi.Set_Value(50); sKd.Set_Value(10); sSp.Set_Value(100);
    readKxx();
}

void loop()
{
    airLoop(air_listen_list);

    /* simulasyon adımı 100 ms */
    if (millis() - lastTick < 100UL) return;
    lastTick = millis();

    float err = setpoint - pv;
    intErr += err * DT_S;
    if (intErr >  500) intErr =  500;
    if (intErr < -500) intErr = -500;
    float dErr = (err - lastErr) / DT_S;
    lastErr = err;
    float u = Kp * err + Ki * intErr + Kd * dErr;
    if (u <   0) u =   0;
    if (u > 100) u = 100;

    /* proses dinamigi */
    pv += DT_S * (A * u - B * pv);
    if (pv < 0) pv = 0;
    if (pv > 250) pv = 250;

    /* UI 200 ms'de bir */
    if (millis() - lastUi >= 200UL)
    {
        lastUi = millis();
        char tmp[16];
        dtostrf(pv,  0, 1, tmp); lPv.setText(tmp);
        dtostrf(setpoint, 0, 0, tmp); lSp.setText(tmp);
        dtostrf(u, 0, 0, tmp); strcat(tmp, " %"); lOut.setText(tmp);
        gPv.Set_value((uint32_t)pv);
        gph.addValue(0, (uint32_t)setpoint);
        gph.addValue(1, (uint32_t)pv);
    }
}
