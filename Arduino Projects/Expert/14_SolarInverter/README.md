# 14 - Solar Inverter

5 sayfali solar inverter SCADA. PV / Batarya / Grid akis kontrolu + 24h trend.

## Sayfalar
1. **Live** - 4 lu kart: Solar PV / House Load / Battery / Grid (W)
2. **Battery** - SoC %, V, A, charge/discharge yonu
3. **Grid** - V, A, frequency, EXPORT/IMPORT/IDLE
4. **History** - 24h grid power grafik (anlik gridW degeri)
5. **Settings** - Grid limit slider (1000..10000 W), Battery min SoC (0..90 %), EXPORT toggle

## Donanim
| Pin | Modul       | Aciklama                      |
|-----|-------------|-------------------------------|
| TX/RX | Panel UART | 115200 baud                   |
| -   | (mock)     | Donanimsiz, simulator yeterli |

## Mock MPPT
- PV egrisi: 06:00..20:00 arasi 4500W tepe alan sin egrisi (+/- 100W noise)
- Sim saat: her 500 ms +30 dk simule
- Surplus = PV - Load
  - Surplus > 0: batarya %95'in altinda ise %70'i bataryaya, kalan grid'e (export)
  - Surplus < 0: batarya minSoC'nin ustunde ise bataryadan, aksi halde grid'den (import)
- exportEnabled=false: grid'e enerji gondermez (surplus heba olur)
- gridLimit > 0: export grid'e en fazla bu kadar verir

## Battery integral
- SoC integral: dt'de battW kadar enerji geliyor (basit model)
- battV = 48 + (SoC/100)*6 V
- battA = battW / battV

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 14_SolarInverter
```
