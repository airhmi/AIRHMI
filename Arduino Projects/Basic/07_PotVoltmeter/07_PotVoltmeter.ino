/**
 * 07_PotVoltmeter  --  Pot ile voltmetre gostergesi (Arduino ADC)
 *
 *   Potansiyometrenin orta bacagi Arduino UNO'nun A0 pinine baglanir,
 *   ust ve alt bacaklar 5V ve GND'ye gider. analogRead(A0) ile 10-bit
 *   ham deger okunur, 5.0V referansiyla volt'a cevrilir, panel'deki
 *   AirGauge ve AirLabel ile gosterilir.
 *
 * Donanim:
 *   POT_TOP    -> 5V
 *   POT_BOTTOM -> GND
 *   POT_WIPER  -> Arduino A0
 *
 * Panel tarafi (07_PotVoltmeter.ahi):
 *   gVolt   : EveGauge (0..500, deger = volt * 100)
 *   lVolt   : ELabelBox -- "X.XX V"
 */

#include <Airhmi.h>

AirGauge gVolt = AirGauge("gVolt");
AirLabel lVolt = AirLabel("lVolt");

const uint8_t POT_PIN = A0;

unsigned long lastRead = 0;
char lastShown[12] = "";

void setup()
{
    Serial.begin(115200);
    airInit();
    pinMode(POT_PIN, INPUT);
    lVolt.setText("0.00 V");
    gVolt.Set_value(0);
}

void loop()
{
    if (millis() - lastRead >= 250UL)
    {
        lastRead = millis();

        int raw = analogRead(POT_PIN);              /* 0..1023 */
        double v = (double)raw * (5.0 / 1023.0);    /* 0..5.0 V */

        char tmp[16];
        dtostrf(v, 0, 2, tmp);
        strcat(tmp, " V");
        if (strcmp(tmp, lastShown) != 0)
        {
            strcpy(lastShown, tmp);
            lVolt.setText(tmp);
        }

        gVolt.Set_value((uint32_t)(v * 100.0));     /* 0..500 */
    }
}
