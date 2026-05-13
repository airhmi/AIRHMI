# 01 — Temperature / Humidity Station (DHT22)

DHT22 sensoru ile sicaklik ve nem okur, panel'deki AirGauge x 2,
AirLabel x 2 ve canli AirGraph (2 hat: kirmizi=temp, mavi=humid)
gosterilir. Min/Max sicaklik ayrica yazilir, Reset ile sifirlanir.

## Donanim

| DHT22 | Arduino UNO |
|---|---|
| VCC | 5V |
| GND | GND |
| DATA | D2 (4.7K pull-up VCC'ye) |

## Kutuphane

Arduino IDE -> Sketch -> Include Library -> Manage Libraries...
- **DHT sensor library** (Adafruit, version 1.4.x)
- **Adafruit Unified Sensor** (DHT bagimliligi)

## Calisma Akisi

Loop her 2 saniyede bir `dht.readTemperature()` ve `readHumidity()`
cagirir, NaN kontrolu yapar. Sonra:
- `lTemp` / `lHumid` - "XX.X C" / "XX.X %"
- `gTemp` (0..500 = 0..50 C), `gHumid` (0..1000 = 0..100 %) gauge'lar
- Min/Max sicaklik takibi (`tMin`/`tMax`), `lTempMin`/`lTempMax`'a yazilir
- `gphTrend.addValue(0, t)` ve `addValue(1, h)` ile graph'a eklenir

## Kullanilan Component'lar

| Nesne | Tur | Islev |
|---|---|---|
| `gTemp`, `gHumid` | EveGauge | sicaklik (0..50 C), nem (0..100 %) |
| `lTemp`, `lHumid` | ELabelBox | sayisal okuma "XX.X C", "XX.X %" |
| `lTempMin`, `lTempMax` | ELabelBox | min/max sicaklik takibi |
| `gphTrend` | EGraph (2 line) | canli trend (kirmizi+mavi) |
| `bReset` | EButton | min/max sayaclarini sifirla |

![1](1.png)
