/**
 * 05_RGBMoodLight  --  RGB ledli mood light (PWM 3 kanal)
 *
 *   3 slider ile R/G/B kanallari (0..255), Apply ile uygulanir.
 *   PWM_0=R, PWM_1=G, PWM_2=B kanalları üzerinden gerçek RGB LED
 *   kontrol edilir. Preset butonları (White / Warm / Sky / Off).
 *   Panel'deki bPreview butonu güncel rengi yaklaşık olarak gösterir.
 *
 * Donanim:
 *   3 PWM çıkışı (panel kart datasheet, 2 kanal yerleşik PWM olabilir;
 *   3. kanal için yazılım PWM ya da harici kart gerekebilir).
 *   PWM_0 -> R, PWM_1 -> G, PWM_2 -> B (her birine 220 ohm seri direnç).
 */

#include <Airhmi.h>

AirSlider sR = AirSlider("sR");
AirSlider sG = AirSlider("sG");
AirSlider sB = AirSlider("sB");

AirButton bApply  = AirButton("bApply");
AirButton bWhite  = AirButton("bWhite");
AirButton bWarm   = AirButton("bWarm");
AirButton bSky    = AirButton("bSky");
AirButton bOff    = AirButton("bOff");
AirButton bPreview = AirButton("bPreview");

AirLabel lRgb = AirLabel("lRgb");
AirGpio  gpio;

uint8_t curR = 0, curG = 0, curB = 0;

uint32_t rgbToColor(uint8_t r, uint8_t g, uint8_t b)
{
    /* AARRGGBB */
    return (uint32_t)0xFF000000 |
           ((uint32_t)r << 16)  |
           ((uint32_t)g << 8)   |
            (uint32_t)b;
}

void applyRgb(uint8_t r, uint8_t g, uint8_t b)
{
    curR = r; curG = g; curB = b;
    /* PWM duty 0..100 (panel API yuzde alir): byte * 100 / 255 */
    gpio.set_pwmfreq(0, 1000, ((uint32_t)r * 100 + 127) / 255);
    gpio.set_pwmfreq(1, 1000, ((uint32_t)g * 100 + 127) / 255);
    gpio.set_pwmfreq(2, 1000, ((uint32_t)b * 100 + 127) / 255);

    bPreview.Set_background_color(rgbToColor(r, g, b));

    char tmp[20];
    snprintf(tmp, sizeof(tmp), "R=%u G=%u B=%u", (unsigned)r, (unsigned)g, (unsigned)b);
    lRgb.setText(tmp);
}

void onApply(void *p)
{
    uint32_t vr = 0, vg = 0, vb = 0;
    sR.Get_Value(&vr); sG.Get_Value(&vg); sB.Get_Value(&vb);
    if (vr > 255) vr = 255;
    if (vg > 255) vg = 255;
    if (vb > 255) vb = 255;
    applyRgb((uint8_t)vr, (uint8_t)vg, (uint8_t)vb);
}

void presetSliders(uint8_t r, uint8_t g, uint8_t b)
{
    sR.Set_Value(r); sG.Set_Value(g); sB.Set_Value(b);
    applyRgb(r, g, b);
}

void onWhite(void *p) { presetSliders(255, 255, 255); }
void onWarm (void *p) { presetSliders(255, 180,  80); }
void onSky  (void *p) { presetSliders( 50, 150, 255); }
void onOff  (void *p) { presetSliders(  0,   0,   0); }

AirTouch *air_listen_list[] = { &bApply, &bWhite, &bWarm, &bSky, &bOff, NULL };

void setup()
{
    Serial.begin(115200);
    airInit();
    bApply.attachPop(onApply, &bApply);
    bWhite.attachPop(onWhite, &bWhite);
    bWarm.attachPop(onWarm,   &bWarm);
    bSky.attachPop(onSky,     &bSky);
    bOff.attachPop(onOff,     &bOff);
    applyRgb(0, 0, 0);
}

void loop()
{
    airLoop(air_listen_list);
}
