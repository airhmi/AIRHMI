/**
 * 07_ServoPanel  --  4 servo kontrol paneli
 *
 *   4 SG90 (veya benzeri) servo Arduino UNO PWM pinleri 9, 10, 11, 6'ya
 *   bagli. Her birine bir slider (0..180 derece) atanir; slider'in OnPop
 *   event'inde servo o aciya gider. 4 preset buton tum servolari aynı
 *   anda standard pozisyonlara getirir (0 / 90 / 180 / 45).
 *
 * Donanim:
 *   Servo 0 -> D9 (PWM)
 *   Servo 1 -> D10 (PWM)
 *   Servo 2 -> D11 (PWM)
 *   Servo 3 -> D6 (PWM)
 *   Tum servolar harici 5V besleme (UNO 5V kalin yuk altinda yetersiz)
 *   ve ortak GND.
 *
 * Kutuphane: Servo (built-in)
 */

#include <Airhmi.h>
#include <Servo.h>

Servo srv[4];
const uint8_t SRV_PIN[4] = { 9, 10, 11, 6 };

AirSlider sS0 = AirSlider("sS0");
AirSlider sS1 = AirSlider("sS1");
AirSlider sS2 = AirSlider("sS2");
AirSlider sS3 = AirSlider("sS3");

AirLabel  lA0 = AirLabel("lA0");
AirLabel  lA1 = AirLabel("lA1");
AirLabel  lA2 = AirLabel("lA2");
AirLabel  lA3 = AirLabel("lA3");

AirButton bP0   = AirButton("bP0");
AirButton bP90  = AirButton("bP90");
AirButton bP180 = AirButton("bP180");
AirButton bP45  = AirButton("bP45");

void writeServo(uint8_t i, uint32_t deg)
{
    if (deg > 180) deg = 180;
    srv[i].write((int)deg);
    char tmp[12];
    snprintf(tmp, sizeof(tmp), "%lu deg", (unsigned long)deg);
    AirLabel *lbl = (i==0)?&lA0:(i==1)?&lA1:(i==2)?&lA2:&lA3;
    lbl->setText(tmp);
}

void onS0(void *p) { uint32_t v=0; sS0.Get_Value(&v); writeServo(0, v); }
void onS1(void *p) { uint32_t v=0; sS1.Get_Value(&v); writeServo(1, v); }
void onS2(void *p) { uint32_t v=0; sS2.Get_Value(&v); writeServo(2, v); }
void onS3(void *p) { uint32_t v=0; sS3.Get_Value(&v); writeServo(3, v); }

void presetAll(uint8_t deg)
{
    for (uint8_t i = 0; i < 4; i++) writeServo(i, deg);
    sS0.Set_Value(deg); sS1.Set_Value(deg);
    sS2.Set_Value(deg); sS3.Set_Value(deg);
}

void onP0  (void *p) { presetAll(0);   }
void onP90 (void *p) { presetAll(90);  }
void onP180(void *p) { presetAll(180); }
void onP45 (void *p) { presetAll(45);  }

AirTouch *air_listen_list[] = {
    &sS0, &sS1, &sS2, &sS3,
    &bP0, &bP90, &bP180, &bP45,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    for (uint8_t i = 0; i < 4; i++) { srv[i].attach(SRV_PIN[i]); srv[i].write(90); }
    sS0.attachPop(onS0, &sS0); sS1.attachPop(onS1, &sS1);
    sS2.attachPop(onS2, &sS2); sS3.attachPop(onS3, &sS3);
    bP0.attachPop  (onP0,   &bP0);
    bP90.attachPop (onP90,  &bP90);
    bP180.attachPop(onP180, &bP180);
    bP45.attachPop (onP45,  &bP45);
    presetAll(90);
}

void loop()
{
    airLoop(air_listen_list);
}
