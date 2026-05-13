/**
 * 02_LedDimmer  --  PWM LED dimmer (slider ile %)
 *
 *   Slider sBright (0-100) panel'de surukleyince Apply butonu ile son
 *   degeri al, PWM_0 frekansini 5 kHz'de duty olarak ayarla, etiketi
 *   yuzde olarak guncelle.
 *
 * Panel tarafi (LedDimmer.ahi):
 *   sBright   : ESlider (0..100, dikey degil yatay)
 *   bApply    : EButton -- son slider degerini PWM'e uygula
 *   lPercent  : ELabelBox -- "X%"
 *
 * Donanim notu: gercek panel'de PWM_0 cikisina LED bagliysa parlaklik
 * degisir. Sim'de gercek pin yok, fakat komut MCU return alaninda
 * gozukur.
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirSlider sBright = AirSlider("sBright");
AirButton bApply  = AirButton("bApply");
AirLabel  lPercent = AirLabel("lPercent");

AirGpio gpio;

/* ---------- callback ---------- */
void onApply(void *p)
{
    uint32_t v = 0;
    sBright.Get_Value(&v);
    if (v > 100) v = 100;

    char tmp[8];
    snprintf(tmp, sizeof(tmp), "%lu%%", (unsigned long)v);
    lPercent.setText(tmp);

    gpio.set_pwmfreq(0, 5000, v);
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bApply,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bApply.attachPop(onApply, &bApply);

    /* baslangic: %0 */
    gpio.set_pwmfreq(0, 5000, 0);
    lPercent.setText("0%");
}

void loop()
{
    airLoop(air_listen_list);
}
