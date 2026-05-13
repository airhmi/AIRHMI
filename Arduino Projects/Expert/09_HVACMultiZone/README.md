# 09 - HVAC Multi-Zone Controller

4 sayfali coklu zone HVAC (isitma/sogutma) kontrol paneli.

## Sayfalar
1. **Dashboard** - 2 zone sicaklik, outdoor sicaklik, sistem modu
2. **Zone1** - Setpoint slider (16-30 C) + HEAT/COOL/OFF butonlari
3. **Zone2** - Setpoint slider + HEAT/COOL/OFF butonlari
4. **Program** - DAY/NIGHT preset + Auto schedule toggle

## Donanim
| Pin | Modul       | Aciklama                     |
|-----|-------------|------------------------------|
| TX/RX | Panel UART | 115200 baud                  |
| -   | (mock)     | Donanimsiz, simulator yeterli |

Sketch zone sicakligini setpoint'e dogru kaydiriyor (her 2 sn 0.3 C). HEAT modu altinda zone setpoint altindaysa isiniyor; COOL ustundeyse soguyor; OFF outdoor sicakligina dogru kayiyor.

## Logic
- Setpoint slider: 16..30 C
- z1Mode/z2Mode: 'H' Heat, 'C' Cool, 'O' Off
- Sistem modu:
  - Iki zone OFF -> "OFF"
  - Bir zone HEAT -> "HEATING"
  - Aksi halde -> "COOLING"
- DAY preset: setpoint 22 C
- NIGHT preset: zone1 18 C, zone2 19 C
- AutoSchedule toggle: panel ustunde "DAY/Auto:ON" gibi gosterir

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 09_HVACMultiZone
```

Donanim test etmeden sadece simulator uzerinde calistirilabilir.
