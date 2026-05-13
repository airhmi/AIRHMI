# 22 - Gym Tracker

5 sayfali fitness takip. **AirProgressBar** rest timer + **AirShape** badge.

## Sayfalar
1. **Today** - 5 egzersiz butonu (Bench/Squat/Deadlift/Pullup/Plank), set sayisi + total
2. **Exercise** - Secili egzersiz adi, set/rep slider, SAVE SET (otomatik timer baslar)
3. **Timer** - Buyuk geri sayim + AirProgressBar + START/RESET + sure slider (10-300 sn)
4. **Stats** - Haftalik graph (gunluk toplam set), week total
5. **Profile** - Current weight slider + Goal weight slider + AirShape badge

## Yeni componentler
- **AirProgressBar (pbRest)**: rest timer kalan sure %
- **AirShape (shBadge)**: profil badge yer tutucusu

## Donanim
| Pin | Modul       | Aciklama                      |
|-----|-------------|-------------------------------|
| TX/RX | Panel UART | 115200 baud                   |
| -   | (mock)     | Donanimsiz, simulator yeterli |

## Mantik
- SAVE SET'e basinca: setCount[curEx] += newSet, weekly[weekDay] += newSet, timer otomatik basla
- Timer 0'a inince buzz 400 ms

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 22_GymTracker
```
