# 09 — Pomodoro Timer ⭐

Klasik Pomodoro tekniği: **25 dk** çalışma + **5 dk** mola döngüsü.
Donanım gerektirmez — sadece panel + Arduino.

## Calisma Akisi

1. `Start` -> `phase = WORK`, 25 dk geri sayim baslar (`MM:SS` etiketi
   her saniyede update, `pbar` 0..100% faz icindeki ilerleme)
2. WORK bittiginde:
   - `todayCount++`
   - `count` AirVariable'a yazilir (panel'de kalici sayim)
   - Buzzer 800 ms cinlar
   - `phase = BREAK`, 5 dk geri sayim
3. BREAK bittiginde:
   - Buzzer cinlar, `phase = WORK`, dongu devam
4. `Stop` herhangi bir anda kesintisiz iptal eder
5. `Reset Day` `todayCount = 0` yapar (yeni gun)

## Component Listesi

| Nesne | Tur | Islev |
|---|---|---|
| `lPhase` | ELabelBox | "WORK" / "BREAK" / "IDLE" |
| `lTime`  | ELabelBox | "MM:SS" geri sayim |
| `lCount` | ELabelBox | "Today: N" (tamamlanan pomodoro) |
| `pbar`   | EveProgressBar | faz ilerlemesi (0..100%) |
| `bStart` / `bStop` / `bResetDay` | EButton | kontrol |
| `count`  | EVariable | panel-side kalici sayim |
| `buz`    | AirBuzzer | faz bitis cinlatmasi |

## Notlar

- `lTime` 1 sn'de bir update edilir (panele cok az UART trafigi)
- `count` AirVariable ile yazildigi icin panel reboot olsa bile son
  sayim hatirlanir (panel firmware AirVariable persistent destegine bagli)

![1](1.png)
