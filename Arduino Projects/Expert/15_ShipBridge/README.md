# 15 - Ship Bridge

5 sayfali gemi koprusu. Autopilot + waypoint navigasyon + radar + MOB.

## Sayfalar
1. **Helm** - Heading, rudder acisi, autopilot toggle, target heading slider
2. **Engine** - RPM gauge (0..3000), throttle slider, FORWARD/STOP/REVERSE
3. **Nav** - GPS lat/lon, hiz, hedef WP'ye mesafe, SKIP WAYPOINT
4. **Radar** - 4 hedef (bearing/distance), <1nm collision alarmi
5. **Alarms** - Buyuk MAN OVER BOARD butonu + son 5 alarm

## Autopilot
- err = tgtHeading - heading (-180..+180 normalize)
- rudder = clamp(0.5 * err, -30, +30)
- Heading = rudder * speed * dt (gemi hareketi sirasinda donus)

## Mantik
- Engine: tgtRpm = throttle * 30, ramp up 60/dt sn^-1
- speed (kn) = (rpm / 3000) * 12 * engineDir
- Lat/lon entegrasyonu: dx/dt = speed * cos(heading), dy/dt = speed * sin(heading)
- Waypoint: 4 nokta ring, <0.5 nm yaklasinca otomatik bir sonrakine
- MOB: motor durur, autopilot kapanir, alarm

## Donanim
| Pin | Modul       | Aciklama                      |
|-----|-------------|-------------------------------|
| TX/RX | Panel UART | 115200 baud                   |
| -   | (mock)     | Donanimsiz, simulator yeterli |

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 15_ShipBridge
```
