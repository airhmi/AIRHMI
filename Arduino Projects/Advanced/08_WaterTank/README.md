# 08 — Water Tank Level (HC-SR04)

HC-SR04 ultrasonik sensor su yüzeyine olan mesafeyi ölçer; sketch tank
yüksekliğine (`TANK_H_CM=100`) göre seviye yüzdesi hesaplar. AirProgressBar
+ AirGraph + iki etiket ile gösterim. Auto modunda %30 altı pompa açar,
%95 üstü pompa kapatır + buzzer alarmı.

## Donanim

| Modul | Arduino UNO |
|---|---|
| HC-SR04 TRIG | D9 |
| HC-SR04 ECHO | D10 |
| HC-SR04 VCC/GND | 5V / GND |
| Pompa Röle IN | Panel GPIO_0 |

> Sensor su yüzeyinin **üstüne** monte edilir; ölçtüğü değer "havadan
> suya kadar olan mesafe" → seviye = `TANK_H - distance`.

## Component

| Nesne | Tur |
|---|---|
| `pbar` | EveProgressBar (0..100%) |
| `lLevel` / `lDist` | "% NN" / "NN cm" |
| `gph` | EGraph 1 line |
| `bPump` | EButton (Active=False) – PUMP ON/OFF göstergesi |
| `bOn`, `bOff`, `bAuto` | manuel kontrol + mod toggle |
| `buz` | overflow alarm |

![1](1.png)
