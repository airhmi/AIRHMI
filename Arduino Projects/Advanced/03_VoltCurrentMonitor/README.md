# 03 — Voltage/Current Monitor (INA219)

INA219 I2C sensor ile bus voltaji + akim + güç ölçümü, panel'de 2 gauge
ve 3 etiket ile gösterim. Slider ile akım limiti ayarlanır; aşıldığında
buzzer cinlar ve OK -> OVER geçer.

## Donanim

| INA219 | Arduino UNO |
|---|---|
| VCC | 5V |
| GND | GND |
| SCL | A5 |
| SDA | A4 |
| V+/V- | yük devresi (modül şemasına göre) |

## Kutuphane

- **Adafruit INA219**
- **Adafruit BusIO** (bağımlılık)

## Component Listesi

| Nesne | Tur | Islev |
|---|---|---|
| `gVolt` | EveGauge | 0..30V (x100) |
| `gCurr` | EveGauge | 0..2000 mA |
| `lVolt` / `lCurr` / `lPower` | ELabelBox | sayısal okumalar |
| `lLimit` / `sLimit` | ELabelBox + EveSlider | akım alarm eşiği (0..2000 mA) |
| `bAlarm` | EButton (Active=False) | OK / OVER (yeşil/kırmızı) |
| `buz` | AirBuzzer | alarm beep |

![1](1.png)
