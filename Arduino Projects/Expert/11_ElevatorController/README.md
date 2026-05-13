# 11 - Elevator Controller

5 sayfali asansor kontrolu. State machine + 8-katli cagri kuyrugu.

## Sayfalar
1. **Cabin** - Buyuk kat numarasi, yon (UP/DOWN), kapi durumu, state metni
2. **Floors** - 8 kat cagri butonu (G..7) - secili: yesil, kuyrukta: turuncu, mevcut: yesil
3. **Diagnostics** - Motor sicakligi, kapi cycle sayisi, run time
4. **Maintenance** - EMERGENCY STOP toggle, DOOR TEST, RESET DIAG
5. **Log** - Son 5 olay (cagri, durus, e-stop)

## State Machine
```
IDLE -> DOOR_CLOSING (1.0s)
     -> MOVING (her kat 1.5s)
     -> DOOR_OPENING (0.8s)
     -> DOOR_OPEN (2.0s)
     -> IDLE
ESTOP: butun durumlardan girilir, Maintenance sayfasindan cikilir
```

`pickNext()` mevcut yondeki cagrilari oncelikler (SCAN benzeri).

## Donanim
| Pin | Modul       | Aciklama                      |
|-----|-------------|-------------------------------|
| TX/RX | Panel UART | 115200 baud                   |
| -   | (mock)     | Donanimsiz, simulator yeterli |

## Diag mantigi
- Motor sicakligi MOVING durumunda her 500 ms +0.05 C, IDLE'da -0.02 C (28..80 C)
- Door cycle: her DOOR_CLOSING bitisinde +1
- Run time: setup'tan beri saniye

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 11_ElevatorController
```
