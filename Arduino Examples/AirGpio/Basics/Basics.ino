/**
 * Basics.ino  --  AirGpio temel kullanim
 *
 *   digital_write(port, value)      : GPIO_0..7'ye 0/1 yaz
 *   digital_read(port)              : GPIO_0..7'den 0/1 oku
 *   analog_read(port)               : ADC port'undan voltaj oku (double)
 *   set_pwmfreq(port, freq, duty)   : PWM_0..3 ayarla
 *
 * Bu ornek:
 *   - GPIO_0'a 0 / 1 yazar
 *   - GPIO_0'i okur (lDigital)
 *   - ADC_0'i okur, voltaj olarak gosterir (lAdc)
 *   - PWM_0 icin %0 / %50 / %100 / Off (5kHz)
 *
 * Panel tarafi (Basics.ahi):
 *   bWrite0 / bWrite1 / bRead       : digital_write/read
 *   bAdc                            : analog_read
 *   bPwm0 / bPwm50 / bPwm100 / bPwmOff
 *   bReset                          : herseyi sifirla
 *   lDigital / lAdc / lPwm          : okuma/durum etiketleri
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirGpio gpio;

AirLabel lDigital = AirLabel("lDigital");
AirLabel lAdc     = AirLabel("lAdc");
AirLabel lPwm     = AirLabel("lPwm");

AirButton bWrite0  = AirButton("bWrite0");
AirButton bWrite1  = AirButton("bWrite1");
AirButton bRead    = AirButton("bRead");
AirButton bAdc     = AirButton("bAdc");
AirButton bPwm0    = AirButton("bPwm0");
AirButton bPwm50   = AirButton("bPwm50");
AirButton bPwm100  = AirButton("bPwm100");
AirButton bPwmOff  = AirButton("bPwmOff");
AirButton bReset   = AirButton("bReset");

/* ---------- callback'ler ---------- */
void onWrite0(void *ptr) { gpio.digital_write(0, 0); }
void onWrite1(void *ptr) { gpio.digital_write(0, 1); }

void onRead(void *ptr)
{
    char tmp[12];
    uint32_t v = gpio.digital_read(0);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lDigital.setText(tmp);
}

void onAdc(void *ptr)
{
    char tmp[16];
    double v = gpio.analog_read(0);
    /* Arduino UNO icin %f desteklenir; AVR'de gerekirse dtostrf. */
    dtostrf(v, 0, 4, tmp);
    lAdc.setText(tmp);
}

void onPwm0(void *ptr)   { gpio.set_pwmfreq(0, 5000,   0); lPwm.setText("0%");   }
void onPwm50(void *ptr)  { gpio.set_pwmfreq(0, 5000,  50); lPwm.setText("50%");  }
void onPwm100(void *ptr) { gpio.set_pwmfreq(0, 5000, 100); lPwm.setText("100%"); }
void onPwmOff(void *ptr) { gpio.set_pwmfreq(0,    0,   0); lPwm.setText("Off");  }

void onReset(void *ptr)
{
    gpio.digital_write(0, 0);
    gpio.set_pwmfreq(0, 0, 0);
    lDigital.setText("---");
    lAdc.setText("---");
    lPwm.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bWrite0, &bWrite1, &bRead,
    &bAdc,
    &bPwm0, &bPwm50, &bPwm100, &bPwmOff,
    &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bWrite0.attachPop(onWrite0,  &bWrite0);
    bWrite1.attachPop(onWrite1,  &bWrite1);
    bRead.attachPop(onRead,      &bRead);
    bAdc.attachPop(onAdc,        &bAdc);
    bPwm0.attachPop(onPwm0,      &bPwm0);
    bPwm50.attachPop(onPwm50,    &bPwm50);
    bPwm100.attachPop(onPwm100,  &bPwm100);
    bPwmOff.attachPop(onPwmOff,  &bPwmOff);
    bReset.attachPop(onReset,    &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
