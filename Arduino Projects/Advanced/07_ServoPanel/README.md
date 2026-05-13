# 07 — 4-Servo Control Panel

4 SG90 (veya benzeri) servoyu Arduino UNO PWM pinleri D9, D10, D11, D6
üzerinden kontrol eder. Her servoya bir slider (0..180°) atanmış,
slider'in OnPop event'inde servo o açıya gider. 4 preset butonu (0/45/90/180)
tüm servoları aynı anda gönderir.

## Donanim

| Servo | Sinyal | Besleme |
|---|---|---|
| 0 | D9  | 5V (harici) |
| 1 | D10 | 5V (harici) |
| 2 | D11 | 5V (harici) |
| 3 | D6  | 5V (harici) |

> UNO 5V çıkışı tek servo için yeterli olabilir; 4 servo birlikte çalışırken
> akım yetmez → harici 5V/2A güç kaynağı kullan, GND ortak olmalı.

## Kutuphane

- **Servo** (built-in, ek yükleme gerekmez)

## Component

| Nesne | Tur |
|---|---|
| `sS0..sS3` | EveSlider × 4 (0..180) |
| `lA0..lA3` | ELabelBox × 4 ("XX deg") |
| `bP0`, `bP45`, `bP90`, `bP180` | preset butonları |

![1](1.png)
