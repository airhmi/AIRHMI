# 19 - Drone GCS

5 sayfali drone Ground Control Station. Mock GPS + waypoint nav + battery failsafe + RTH.

## Sayfalar
1. **Map** - Drone konumu (lat/lon), hedef WP, base, eve mesafe (m)
2. **Telemetry** - Altitude, speed, battery %, GPS sat sayisi, flight mode
3. **Mission** - 4 waypoint listesi, current WP isaretli, START MISSION + NEXT WP
4. **Camera** - Live view ("OFF/LIVE/REC"), snapshot sayaci, RECORD toggle
5. **Settings** - ARM/DISARM, RTH (return-to-home), failsafe esiklerini gosterir

## Flight modes
- DISARMED -> motor kapali
- ARMED -> hover, motor aktif
- AUTO -> mission active, waypoint takibi
- RTH -> base'e donus
- RTH-LOWBATT -> batt < 20 otomatik tetiklenir
- LAND-EMERGENCY -> batt < 10 (panel uzeri buzz)
- LANDED -> base'e ulasti, alt=0, otomatik DISARM

## Mission lojigi
- 4 waypoint ring
- Hiz 5 m/s, dt'de drone hedefe dogru ilerler
- Mesafe < 1 m -> next WP (auto)
- RTH active iken hedef base, mesafe < 5 m -> alt azalt -> 0'da land

## Failsafe
- batt < 20 % -> RTH otomatik baslar (kullanici manuel kapatamaz, override yok)
- batt < 10 % -> emergency land mode
- batt -> her 3 sn -1

## Donanim
| Pin | Modul       | Aciklama                      |
|-----|-------------|-------------------------------|
| TX/RX | Panel UART | 115200 baud                   |
| -   | (mock)     | Donanimsiz, simulator yeterli |

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 19_DroneGCS
```
