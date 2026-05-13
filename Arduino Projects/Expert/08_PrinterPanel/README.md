# 08 — 3D Printer Panel (4 sayfa)

3D yazıcı kontrol paneli prototipi. Donanım yok — heater ısıtması simülasyon.

## Sayfalar

| Screen | İçerik |
|---|---|
| 1 Status | Nozzle/Bed sıcaklık (current/setpoint), file name, progress bar + % |
| 2 Print | START/PAUSE/STOP/RESUME + state metni (IDLE/PRINTING/PAUSED) |
| 3 Heaters | Nozzle/Bed setpoint slider + PREHEAT/COOLDOWN preset |
| 4 Settings | Material seçici (PLA/PETG/ABS) — material'a göre setpoint preset |

## Material Presetleri

| | Nozzle | Bed |
|---|---|---|
| PLA  | 200°C | 60°C |
| PETG | 230°C | 80°C |
| ABS  | 240°C | 100°C |

## Mantık

- Print: 2 sn'de bir progress +1 (%100'de IDLE)
- Heater: 1 sn'de bir current → setpoint'e doğru +3°C nozzle, +1.5°C bed
- PREHEAT → PLA preset, COOLDOWN → 25°C

> Gerçek 3D yazıcı için thermistor ADC + PWM heater + step motor sürücü
> bağlantıları eklenmeli. Bu prototip sadece HMI tarafını gösterir.

![1](1.png)
