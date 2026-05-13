# 17 - Brewing Controller

5 sayfali bira yapim kontrolu. Cok-asamali sicaklik profili + recete + boil zamanlayici.

## Sayfalar
1. **Mash** - Current/target temp + step adi + kalan sure + START/STOP/NEXT
2. **Boil** - Wort temp + boil timer + 3 hop ekleme + heater state
3. **Ferment** - Fermenter sicakligi + gun sayaci + airlock burp counter
4. **Recipe** - 3 recete preset (Pale Ale / IPA / Stout)
5. **Log** - Son 5 olay

## Mash step machine
| Recipe   | Strike | Beta-amilaz | Alpha-amilaz | Mashout |
|----------|-------:|------------:|-------------:|--------:|
| Pale Ale | 50 C   | 65 C / 5 dk | 72 C / 5 dk  | 76 C    |
| IPA      | 52 C   | 64 C / 4 dk | 68 C / 6 dk  | 78 C    |
| Stout    | 55 C   | 66 C / 4 dk | 75 C / 6 dk  | 80 C    |

Her step:
- Heater = mashTemp < target - 0.3 C ise ON
- Sicaklik 0.6 C/sn ramp (heater ON), 0.1 C/sn dusus (OFF)
- Step suresi dolup target+/-0.5 C ulasinca otomatik next step

## Boil
- 60 dk timer
- Hop ekleme: 60 dk kala (T=0), 30 dk kala (T=30), 5 dk kala (T=55) - panel uzeri buzz
- mashTemp 100 C'ye ramp olur

## Donanim
| Pin | Modul       | Aciklama                      |
|-----|-------------|-------------------------------|
| TX/RX | Panel UART | 115200 baud                   |
| -   | (mock)     | Donanimsiz, simulator yeterli |

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 17_BrewingController
```
