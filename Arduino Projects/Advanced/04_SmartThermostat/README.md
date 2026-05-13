# 04 — Smart Thermostat (DS18B20 + Relay)

DS18B20 1-wire sıcaklık sensörünü her 2 sn okur, slider'la ayarlanan
hedef sıcaklığa göre röleyi (panel GPIO_0) hysteresis ile açıp kapatır.
35°C üzerinde overheat alarm.

## Donanim

| Modul | Arduino UNO |
|---|---|
| DS18B20 DATA | D2 (4.7K pull-up VCC'ye) |
| DS18B20 VCC/GND | 5V / GND |
| Röle IN | Panel GPIO_0 (panel datasheet) |

## Kutuphane

- **OneWire** (Paul Stoffregen)
- **DallasTemperature** (Miles Burton)

## Calisma

- Hedef: slider 100..300 (= 10.0..30.0°C, 0.1°C adım)
- Hysteresis ±0.5°C: titreşim önler
- t > 35°C → buzzer 2 sn'de bir 300 ms beep

## Component

| Nesne | Tur |
|---|---|
| `gTemp`, `lTemp` | EveGauge (0..50°C ×10) + ELabelBox |
| `sTarget`, `lTarget` | EveSlider + ELabelBox |
| `bRelay` | EButton (Active=False) – OFF/HEAT ON gösterge |

![1](1.png)
