# 05 — RGB Mood Light (PWM 3 channel) ⭐

3 slider (R/G/B, 0..255) ile renk seç, **Apply** ile PWM_0/1/2 kanallarına
duty %0..100 olarak yaz. Panel'de `bPreview` butonu canlı renk önizlemesi
gösterir. 4 preset: White / Warm / Sky / Off.

## Donanim

3 PWM çıkışlı RGB LED + her birine 220 ohm seri direnç.
- PWM_0 → R
- PWM_1 → G
- PWM_2 → B
- LED ortak ucu GND (common cathode)

> Panel kart datasheet'inde 2 yerleşik PWM (IO6, IO7) var; 3. kanal
> harici PWM kart ya da yazılım PWM gerektirir. Sketch yine de
> 3 kanala komut gönderir; panel firmware'i mevcut kanalları işler.

## Component

| Nesne | Tur |
|---|---|
| `sR`, `sG`, `sB` | EveSlider × 3 (0..255) |
| `bApply` | Apply butonu |
| `bPreview` | EButton (Active=False) – canlı renk önizleme |
| `lRgb` | "R=N G=N B=N" |
| `bWhite`, `bWarm`, `bSky`, `bOff` | preset butonları |

## Slider değişimini canlı uygulamak istersen
Apply yerine her slider'ın `attachPop`'una `onApply` bağlanabilir
(UART trafiği daha çok).

![1](1.png)
