/**
 * 10_ButtonLed  --  4 buton -> 4 GPIO LED esleseni
 *
 *   bL0..bL3 panel butonlari (push-pull) GPIO_0..GPIO_3 cikislarini
 *   yakar/sonduruir. Her butonun durumunu yansitan dRect (panel-side
 *   gosterge dikdortgeni) state'ine gore renk degistirir.
 *
 * Panel tarafi (ButtonLed.ahi):
 *   bL0..bL3   : EButton push-pull (toggle)
 *   dL0..dL3   : EButton (sadece gosterge, active=False) -- LED gorseli
 */

#include <Airhmi.h>

const uint32_t LED_OFF = 0xFF202020UL;
const uint32_t LED_ON  = 0xFF00FF00UL;

AirGpio gpio;

AirButton bL0 = AirButton("bL0");
AirButton bL1 = AirButton("bL1");
AirButton bL2 = AirButton("bL2");
AirButton bL3 = AirButton("bL3");

AirButton dL0 = AirButton("dL0");
AirButton dL1 = AirButton("dL1");
AirButton dL2 = AirButton("dL2");
AirButton dL3 = AirButton("dL3");

bool ledState[4] = {false, false, false, false};

void apply(uint8_t i)
{
    AirButton *led = (i == 0) ? &dL0 : (i == 1) ? &dL1 : (i == 2) ? &dL2 : &dL3;
    led->Set_background_color(ledState[i] ? LED_ON : LED_OFF);
    gpio.digital_write(i, ledState[i] ? 1 : 0);
}

void onL0(void *p){ ledState[0] = !ledState[0]; apply(0); }
void onL1(void *p){ ledState[1] = !ledState[1]; apply(1); }
void onL2(void *p){ ledState[2] = !ledState[2]; apply(2); }
void onL3(void *p){ ledState[3] = !ledState[3]; apply(3); }

AirTouch *air_listen_list[] = { &bL0, &bL1, &bL2, &bL3, NULL };

void setup()
{
    Serial.begin(115200);
    airInit();
    bL0.attachPop(onL0, &bL0);
    bL1.attachPop(onL1, &bL1);
    bL2.attachPop(onL2, &bL2);
    bL3.attachPop(onL3, &bL3);
    for (uint8_t i = 0; i < 4; i++) apply(i);
}

void loop()
{
    airLoop(air_listen_list);
}
