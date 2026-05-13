# 02 — Air Quality Monitor (MQ135)

MQ135 gas sensor (veya benzeri analog) ham 0..1023 degerini Arduino A0'da
okur, panel'de gauge + label + canli grafik gosterir. Slider ile alarm
esigi ayarlanabilir; deger esigi gecince `bAlarm` gostergesi kirmiziya
doner ve buzzer her 1.5 sn'de bir kisa beep cikartir.

## Donanim

| MQ135 | Arduino UNO |
|---|---|
| VCC | 5V |
| GND | GND |
| A0  | A0 |
| D0  | (kullanilmiyor) |

> MQ135 ilk acilista 24-48 saat preheat ister; kabaca 1-3 dakika sicindiktan
> sonra deger sabitlesir.

## Calisma Akisi

- `analogRead(A0)` - 500 ms'de bir
- `lPpm`, `gPpm` (0..1023), `gphTrend` (1 hat) update
- Threshold > olunca: `bAlarm` rengi/metni degisir, buz beep
- Slider degeri 50..1000 araliginda alarm esigini ayarlar

## Component Listesi

| Nesne | Tur | Islev |
|---|---|---|
| `gPpm`     | EveGauge   | 0..1023 ham deger |
| `lPpm`     | ELabelBox  | sayisal okuma |
| `bAlarm`   | EButton (Active=False) | OK/ALARM gosterge (yesil/kirmizi) |
| `lLimit`   | ELabelBox  | "Limit: NN" |
| `sLimit`   | EveSlider  | alarm esigi (50..1000) |
| `gphTrend` | EGraph     | 1 hat trend |
| `buz`      | AirBuzzer  | alarm beep |

![1](1.png)
