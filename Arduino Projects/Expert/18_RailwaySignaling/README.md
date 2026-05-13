# 18 - Railway Signaling

5 sayfali demiryolu sinyalizasyon. 4-block ring + 2 tren + interlock kurallari.

## Sayfalar
1. **Track** - 4-block ring gosterimi `[#][_][_][#]`, T1/T2 konum
2. **Trains** - T1/T2 START/STOP toggle + hiz slider (10..100)
3. **Signals** - 4 sinyal renk kodu (G=green / Y=yellow / R=red)
4. **Interlock** - ROUTE T1/T2 onayi + OVERRIDE INTERLOCK (riskli)
5. **Log** - Son 5 olay (sinyal, durus, collision)

## Sinyal mantigi (3-aspect)
- next blok dolu -> R (kirmizi: dur)
- after blok dolu (1 sonraki) -> Y (sari: dikkat)
- ikisi de bos -> G (yesil: serbest)

## Interlock
- `routeOk(from, train)`: hedef blok bos mu kontrol eder
- ROUTE butonu: blok bossa onaylar ve treni RUN'a alir
- OVERRIDE: kontroleri devre disi birakir (riskli, COLLISION mumkun)

## Tren hareketi
- Hiz 10..100, period = (101 - speed) * 50 ms (dusuk = yavas)
- Her period bir bloga ilerler
- next == diger tren ve override yoksa -> otomatik durur (sinyal RED)
- Iki tren ayni blokta -> COLLISION!! buzz 800

## Donanim
| Pin | Modul       | Aciklama                      |
|-----|-------------|-------------------------------|
| TX/RX | Panel UART | 115200 baud                   |
| -   | (mock)     | Donanimsiz, simulator yeterli |

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 18_RailwaySignaling
```
