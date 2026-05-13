# 12 - EV Charging Station

5 sayfali EV sarj istasyonu kontrolu. CCS-vari state machine + kWh integral.

## Sayfalar
1. **Status** - Connector durumu (AVAILABLE/PLUGGED/CHARGING/COMPLETE/FAULT) + canli kW + PLUG IN / UNPLUG
2. **Charge** - kW gauge (0..22), delivered kWh, target slider (5..80 kWh), START/STOP
3. **Pricing** - $/kWh slider (0..1.00), anlik tutar
4. **Sessions** - Son 3 sarj (kWh / dakika / tutar)
5. **Diag** - Kablo sicakligi, contactor cycle sayaci, son hata + CLEAR

## State Machine
```
AVAILABLE -> PLUGGED  (PLUG IN)
PLUGGED   -> CHARGING (START)
CHARGING  -> COMPLETE (target kWh ulasildi veya STOP)
CHARGING  -> FAULT    (cable temp > 65 C "OverTemp")
*         -> AVAILABLE (UNPLUG)
FAULT     -> AVAILABLE (CLEAR ERR)
```

## Donanim
| Pin | Modul       | Aciklama                      |
|-----|-------------|-------------------------------|
| TX/RX | Panel UART | 115200 baud                   |
| -   | (mock)     | Donanimsiz, simulator yeterli |

## Mantik
- Power ramp: ilk 5 sn 0 -> 22 kW lineer, sonra sabit
- kWh integral: kWh += kW * dt / 3600
- Cable temp: CHARGING'da +0.4 C/sn, idle'da -0.6 C/sn (28..65 C)
- 65 C'de OverTemp arizasi -> FAULT
- Cost = deliveredKWh * pricePerKWh

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 12_EVChargingStation
```
