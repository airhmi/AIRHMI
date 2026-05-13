# 24 - Flight Deck Cockpit

5 sayfali ucak kokpit paneli. **4 AirGauge** + **AirProgressBar** + **AirShape** kombinasyonu.

## Sayfalar
1. **PFD** - Airspeed gauge + Altitude gauge + Heading + Flight mode + AirShape (attitude indicator dekoratif)
2. **Nav** - 4 waypoint listesi + current WP + NEXT WP + distance
3. **Engines** - 2 motor RPM gauge (E1 + E2) + AirProgressBar fuel + Fuel %
4. **Autopilot** - 4 mode toggle (ALT HOLD / HDG HOLD / NAV / APR) + flight mode display
5. **Comms** - COM1/COM2 freq + 4 freq preset (118.10, 121.50, 127.50, 133.40)

## Yeni componentler
- **2x AirGauge** (engine RPM, ayrica PFD'de 2 gauge)
- **AirProgressBar (pbFuel)**: yakit %
- **AirShape (shAtt)**: attitude indicator yer tutucusu

## Donanim
| Pin | Modul       | Aciklama                      |
|-----|-------------|-------------------------------|
| TX/RX | Panel UART | 115200 baud                   |
| -   | (mock)     | Donanimsiz, simulator yeterli |

## Mantik
- Her saniye airspeed +5, altitude +200, RPM +60 ramp
- Heading +0.5/sn yavasca artiyor
- Yakit RPM > 1000 ise her 5 sn -1
- Yakit < 20 -> alarm buzz

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 24_FlightDeck
```
