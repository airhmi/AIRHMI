# 10 - Pomodoro & Task Manager

3 sayfali pomodoro timer + gorev takip uygulamasi.

## Sayfalar
1. **Timer** - Buyuk MM:SS sayim, WORK/BREAK durumu, START/PAUSE/RESET
2. **Tasks** - 5 gorev butonu (her biri done toggle)
3. **Stats** - Tamamlanan pomodoro sayisi + tamamlanan gorev sayaci

## Donanim
| Pin | Modul       | Aciklama                        |
|-----|-------------|---------------------------------|
| TX/RX | Panel UART | 115200 baud                     |
| -   | (mock)     | Donanimsiz, simulator yeterli   |

## Logic
- 25 dakika WORK -> 5 dakika BREAK -> tekrar WORK dongusu otomatik
- Her gecisinde buzzer 400 ms (panel uzeri)
- Gorev butonuna basinca DONE -> baslik degisir, arka plan yesil olur
- Stats sayfasi tamamlanan pomodoro ve "X / 5" sayar

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 10_PomodoroTaskManager
```

Donanimsiz, sade simulator uzerinde calistirmak yeterli.
