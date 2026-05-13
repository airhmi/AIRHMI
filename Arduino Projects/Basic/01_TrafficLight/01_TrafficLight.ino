/**
 * 01_TrafficLight  --  Trafik isigi simulasyonu (auto / manuel)
 *
 *   Auto modda 2'ser saniye araliklarla kirmizi -> sari -> yesil donerek
 *   ilerler (Arduino loop'unda millis() ile state machine).
 *
 *   Manuel modda sBR / sBY / sBG butonlarina dokunarak ilgili lambayi
 *   yakar, digerlerini soner.
 *
 *   bMode butonu auto/manuel modu degistirir, etiketini gunceller.
 *
 * Panel tarafi (TrafficLight.ahi):
 *   bRed / bYellow / bGreen  : isik gostergeleri (renkli buyuk butonlar)
 *   bMode                    : Auto <-> Manuel toggle
 *   lMode                    : aktif modu yazan etiket
 */

#include <Airhmi.h>

/* ---------- renkler (AARRGGBB) ---------- */
const uint32_t COLOR_OFF       = 0xFF202020UL;  /* koyu gri */
const uint32_t COLOR_RED_ON    = 0xFFFF0000UL;
const uint32_t COLOR_YELLOW_ON = 0xFFFFFF00UL;
const uint32_t COLOR_GREEN_ON  = 0xFF00FF00UL;

/* ---------- HMI nesneleri ---------- */
AirButton bRed    = AirButton("bRed");
AirButton bYellow = AirButton("bYellow");
AirButton bGreen  = AirButton("bGreen");
AirButton bMode   = AirButton("bMode");
AirLabel  lMode   = AirLabel("lMode");

/* ---------- state ---------- */
enum Mode { AUTO_MODE, MANUAL_MODE };
Mode mode = AUTO_MODE;
uint8_t lampState = 0;          /* 0=red, 1=yellow, 2=green */
unsigned long lastChange = 0;
const unsigned long PHASE_MS = 2000;

/* ---------- helpers ---------- */
void updateLights()
{
    bRed.Set_background_color   (lampState == 0 ? COLOR_RED_ON    : COLOR_OFF);
    bYellow.Set_background_color(lampState == 1 ? COLOR_YELLOW_ON : COLOR_OFF);
    bGreen.Set_background_color (lampState == 2 ? COLOR_GREEN_ON  : COLOR_OFF);
}

/* ---------- callback'ler ---------- */
void onRed(void *p)    { if (mode == MANUAL_MODE) { lampState = 0; updateLights(); } }
void onYellow(void *p) { if (mode == MANUAL_MODE) { lampState = 1; updateLights(); } }
void onGreen(void *p)  { if (mode == MANUAL_MODE) { lampState = 2; updateLights(); } }

void onMode(void *p)
{
    mode = (mode == AUTO_MODE) ? MANUAL_MODE : AUTO_MODE;
    lMode.setText(mode == AUTO_MODE ? "AUTO" : "MANUEL");
    lastChange = millis();
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bRed, &bYellow, &bGreen, &bMode,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bRed.attachPop(onRed,       &bRed);
    bYellow.attachPop(onYellow, &bYellow);
    bGreen.attachPop(onGreen,   &bGreen);
    bMode.attachPop(onMode,     &bMode);

    lampState  = 0;
    lastChange = millis();
    updateLights();
    lMode.setText("AUTO");
}

void loop()
{
    airLoop(air_listen_list);

    if (mode == AUTO_MODE && (millis() - lastChange) >= PHASE_MS)
    {
        lastChange = millis();
        lampState  = (lampState + 1) % 3;
        updateLights();
    }
}
