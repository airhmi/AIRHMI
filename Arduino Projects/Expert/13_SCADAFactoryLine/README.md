# 13 - SCADA Factory Line

5 sayfali fabrika hatti SCADA tarzi panel. 4 istasyon + OEE + alarm + recipe yonetimi.

## Sayfalar
1. **Overview** - 4 istasyon (Conveyor / Drill / Paint / QA) durumu + toplam OK/SCRAP
2. **Stations** - Her istasyon icin Start/Stop toggle + parca sayaci
3. **OEE** - Availability / Performance / Quality + overall OEE %
4. **Alarms** - Son 5 alarm (renkli oncelik) + ACK ALL
5. **Recipes** - 3 preset (Standard / Fast / HighQA), aktif recete

## Recipe parametreleri
| Recipe   | Cycle (ms) | Scrap (%) |
|----------|-----------:|----------:|
| Standard |       1500 |         5 |
| Fast     |        800 |        12 |
| HighQA   |       2200 |         2 |

## OEE
- **Availability** = uptime / totaltime (tum istasyon RUN olmadigi sure downtime)
- **Performance** = sabit %95 mock (gercek: cycle * count / runtime)
- **Quality** = good / total
- **OEE** = A * P * Q / 10000

## Donanim
| Pin | Modul       | Aciklama                      |
|-----|-------------|-------------------------------|
| TX/RX | Panel UART | 115200 baud                   |
| -   | (mock)     | Donanimsiz, simulator yeterli |

## Mantik
- Tum istasyonlar RUN ise her cycleMs bir parca uretir
- random(100) < scrapRate -> QA reject, scrap sayaci artar
- random(200)==0 -> bir istasyon FAULT, otomatik durur, alarm
- Alarm rengi: en yeni kirmizi -> sari -> beyaz -> gri (yas-renk skalasi)

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 13_SCADAFactoryLine
```
