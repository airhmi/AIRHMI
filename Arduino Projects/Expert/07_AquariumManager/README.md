# 07 — Aquarium Manager (4 sayfa)

DS18B20 + analog pH + servo besleyici + PWM aydınlatma. 4 sayfa: Status / Feeding / Lighting / Settings.

## Donanım

| | |
|---|---|
| DS18B20 DATA | D2 |
| pH analog | A0 |
| Light PWM | Panel PWM_0 |
| Filter relay | Panel GPIO_1 |
| Feeder servo | D9 |

Kütüphane: **OneWire**, **DallasTemperature**, **Servo**.

## Sayfalar

| Screen | İçerik |
|---|---|
| 1 Status | Su sıcaklığı + pH + ışık modu (DAY/NIGHT) + Filter ON/OFF |
| 2 Feeding | FEED NOW (servo açar) + 3 öğün saat slider (0..23) |
| 3 Lighting | Brightness slider (0..100, PWM) + DAY/NIGHT preset |
| 4 Settings | Low/High temp limit slider + Calibrate pH |

## Mantık

- 3 sn'de bir DS18B20 + analogRead(A0)
- pH = (raw / 1023) * 14 + offset (kalibrasyon ile düzeltilir)
- FEED NOW: servo 120° → 500 ms bekle → 0° (yem dökme hareketi)
- DAY: bright = 80%, NIGHT: bright = 10%

![1](1.png)
