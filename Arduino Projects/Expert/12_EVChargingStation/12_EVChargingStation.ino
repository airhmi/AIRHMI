/**
 * 12_EVChargingStation  --  5 sayfali EV sarj istasyonu kontrolu
 *
 *   Screen1 Status   : connector durumu (AVAILABLE/PLUGGED/CHARGING/COMPLETE)
 *                      buyuk ikon-vari etiket + canli kW
 *   Screen2 Charge   : target kWh slider, kW gauge, kWh label, START/STOP
 *   Screen3 Pricing  : $/kWh slider, anlik tutar
 *   Screen4 Sessions : son 3 sarj (kWh + dakika + tutar)
 *   Screen5 Diag     : kablo sicakligi, contactor cycle, son hata
 *
 *   State machine: AVAILABLE -> PLUGGED -> CHARGING -> COMPLETE -> AVAILABLE
 *   Power 0 -> 22 kW 5 sn ramp, sonra sabit. kWh = integral.
 */

#include <Airhmi.h>

/* Status */
AirLabel  lConn = AirLabel("lConn");
AirLabel  lLiveKW = AirLabel("lLiveKW");
AirButton bPlug = AirButton("bPlug");
AirButton bUnplug = AirButton("bUnplug");

/* Charge */
AirSlider sTarget = AirSlider("sTarget");
AirGauge  gKW = AirGauge("gKW");
AirLabel  lkWh = AirLabel("lkWh");
AirLabel  lTarget = AirLabel("lTarget");
AirButton bStart = AirButton("bStart");
AirButton bStop = AirButton("bStop");

/* Pricing */
AirSlider sPrice = AirSlider("sPrice");
AirLabel  lPrice = AirLabel("lPrice");
AirLabel  lCost = AirLabel("lCost");

/* Sessions */
AirLabel lS0 = AirLabel("lS0");
AirLabel lS1 = AirLabel("lS1");
AirLabel lS2 = AirLabel("lS2");

/* Diag */
AirLabel  lCabT = AirLabel("lCabT");
AirLabel  lCycles = AirLabel("lCycles");
AirLabel  lErr = AirLabel("lErr");
AirButton bClearErr = AirButton("bClearErr");

AirBuzzer buz = AirBuzzer("buz");

enum { ST_AVAILABLE, ST_PLUGGED, ST_CHARGING, ST_COMPLETE, ST_FAULT } st = ST_AVAILABLE;

float curKW = 0.0f;
float maxKW = 22.0f;
float deliveredKWh = 0.0f;
float targetKWh = 20.0f;
uint8_t pricePerKWhCents = 25;
unsigned long lastTick = 0;
unsigned long sessionStart = 0;
float cableTemp = 28.0f;
uint16_t contactorCycles = 0;
char lastErr[24] = "None";

struct Session { float kWh; uint16_t mins; float cost; };
Session sessions[3];
uint8_t sessCount = 0;

void renderStatus()
{
    const char *t = "AVAILABLE";
    switch (st) {
        case ST_PLUGGED:  t = "PLUGGED"; break;
        case ST_CHARGING: t = "CHARGING"; break;
        case ST_COMPLETE: t = "COMPLETE"; break;
        case ST_FAULT:    t = "FAULT"; break;
        default: break;
    }
    lConn.setText(t);
    char tmp[16];
    dtostrf(curKW, 0, 1, tmp); strcat(tmp, " kW"); lLiveKW.setText(tmp);
}

void renderCharge()
{
    char tmp[16];
    gKW.Set_value((uint32_t)curKW);
    dtostrf(deliveredKWh, 0, 2, tmp); strcat(tmp, " kWh"); lkWh.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u kWh", (unsigned)targetKWh); lTarget.setText(tmp);
}

void renderPricing()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u.%02u $", pricePerKWhCents/100, pricePerKWhCents%100);
    lPrice.setText(tmp);
    float cost = deliveredKWh * (pricePerKWhCents / 100.0f);
    dtostrf(cost, 0, 2, tmp); strcat(tmp, " $"); lCost.setText(tmp);
}

void renderSessions()
{
    AirLabel *labels[3] = { &lS0, &lS1, &lS2 };
    char tmp[32];
    for (uint8_t i = 0; i < 3; i++) {
        if (i < sessCount) {
            char k[8]; dtostrf(sessions[i].kWh, 0, 1, k);
            char c[8]; dtostrf(sessions[i].cost, 0, 2, c);
            snprintf(tmp, sizeof(tmp), "%s kWh / %u min / %s $", k, sessions[i].mins, c);
        } else {
            tmp[0] = 0;
        }
        labels[i]->setText(tmp);
    }
}

void renderDiag()
{
    char tmp[16];
    dtostrf(cableTemp, 0, 1, tmp); strcat(tmp, " C"); lCabT.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u", contactorCycles); lCycles.setText(tmp);
    lErr.setText(lastErr);
}

void pushSession(float kWh, uint16_t mins)
{
    float cost = kWh * (pricePerKWhCents / 100.0f);
    for (uint8_t i = 2; i > 0; i--) sessions[i] = sessions[i-1];
    sessions[0].kWh = kWh; sessions[0].mins = mins; sessions[0].cost = cost;
    if (sessCount < 3) sessCount++;
    renderSessions();
}

void onPlug(void *p)
{
    if (st != ST_AVAILABLE) return;
    st = ST_PLUGGED;
    deliveredKWh = 0.0f;
    renderStatus();
}
void onUnplug(void *p)
{
    if (st == ST_AVAILABLE) return;
    if (st == ST_CHARGING) {
        unsigned long mins = (millis() - sessionStart) / 60000UL;
        pushSession(deliveredKWh, (uint16_t)mins);
        contactorCycles++;
    }
    st = ST_AVAILABLE;
    curKW = 0.0f;
    renderStatus(); renderCharge(); renderDiag();
}
void onStart(void *p)
{
    if (st != ST_PLUGGED) return;
    st = ST_CHARGING;
    sessionStart = millis();
    lastTick = millis();
    contactorCycles++;
    renderStatus();
}
void onStop(void *p)
{
    if (st != ST_CHARGING) return;
    unsigned long mins = (millis() - sessionStart) / 60000UL;
    pushSession(deliveredKWh, (uint16_t)mins);
    st = ST_COMPLETE;
    curKW = 0.0f;
    contactorCycles++;
    buz.Set_Buzzer(200);
    renderStatus(); renderCharge();
}
void onTarget(void *p)
{
    uint32_t v = 0; sTarget.Get_Value(&v);
    if (v < 5) v = 5; if (v > 80) v = 80;
    targetKWh = v;
    renderCharge();
}
void onPrice(void *p)
{
    uint32_t v = 0; sPrice.Get_Value(&v);
    pricePerKWhCents = (uint8_t)v;
    renderPricing();
}
void onClearErr(void *p)
{
    strcpy(lastErr, "None");
    if (st == ST_FAULT) st = ST_AVAILABLE;
    renderDiag(); renderStatus();
}

AirTouch *air_listen_list[] = {
    &bPlug, &bUnplug, &bStart, &bStop, &sTarget, &sPrice, &bClearErr,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bPlug.attachPop(onPlug, &bPlug);
    bUnplug.attachPop(onUnplug, &bUnplug);
    bStart.attachPop(onStart, &bStart);
    bStop.attachPop(onStop, &bStop);
    sTarget.attachPop(onTarget, &sTarget);
    sPrice.attachPop(onPrice, &sPrice);
    bClearErr.attachPop(onClearErr, &bClearErr);

    sTarget.Set_Value((uint32_t)targetKWh);
    sPrice.Set_Value(pricePerKWhCents);
    gKW.Set_value(0);
    renderStatus(); renderCharge(); renderPricing(); renderSessions(); renderDiag();
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastTick < 200UL) return;
    float dt = (millis() - lastTick) / 1000.0f;
    lastTick = millis();

    if (st == ST_CHARGING) {
        unsigned long el = millis() - sessionStart;
        float ramp = el < 5000UL ? (el / 5000.0f) : 1.0f;
        curKW = maxKW * ramp;
        deliveredKWh += (curKW * dt) / 3600.0f;
        cableTemp += dt * 0.4f;
        if (cableTemp > 65.0f) {
            strcpy(lastErr, "OverTemp");
            st = ST_FAULT;
            curKW = 0.0f;
            buz.Set_Buzzer(800);
        }
        if (deliveredKWh >= targetKWh) {
            unsigned long mins = (millis() - sessionStart) / 60000UL;
            pushSession(deliveredKWh, (uint16_t)mins);
            st = ST_COMPLETE;
            curKW = 0.0f;
            buz.Set_Buzzer(150);
        }
    } else {
        if (cableTemp > 28.0f) cableTemp -= dt * 0.6f;
        if (cableTemp < 28.0f) cableTemp = 28.0f;
        curKW = 0.0f;
    }

    renderStatus(); renderCharge(); renderPricing(); renderDiag();
}
