# 16 - Medical Monitor

5 sayfali hasta basi monitor. Vital signs + ECG-vari waveform + cok-esikli alarm.

## Sayfalar
1. **Vitals** - HR / SpO2 / BP / Temp 4'lu kart + ritm tani (NSR / BRADY / TACHY / HYPOX)
2. **ECG** - P-QRS-T benzeri waveform graph (mock 50 ms tick)
3. **Alarms** - Son 5 alarm + ACK ALL
4. **Trend** - HR son N saniye grafigi (her saniye 1 nokta)
5. **Patient** - Isim, yas, alerji + HR alt/ust esik sliderlari

## Alarm esikleri
- HR < hrMin -> BRADYCARDIA (default 50)
- HR > hrMax -> TACHYCARDIA (default 120)
- SpO2 < 92 -> Low SpO2
- Temp > 38.5 -> Fever
- Her ~30 sn rastgele aritma simulasyonu (HR=35 veya 150)

## ECG mock
- Her 50 ms ecgValue(phase) cagrilir
- Phase periyodu = 60000 / HR / 50 (60 bpm -> 20 sample / beat)
- Sequence: 100 baseline -> P (110/95) -> 100 -> Q (60) -> R (200) -> S (30) -> T (130)

## Donanim
| Pin | Modul       | Aciklama                      |
|-----|-------------|-------------------------------|
| TX/RX | Panel UART | 115200 baud                   |
| -   | (mock)     | Donanimsiz, simulator yeterli |

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 16_MedicalMonitor
```
