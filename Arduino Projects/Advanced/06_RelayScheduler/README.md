# 06 — 4-Channel Relay Scheduler

4 röleyi panel GPIO_0..GPIO_3 üzerinden, RTC saat tabanlı otomatik
programa göre ya da manuel toggle ile kontrol eder.

## Otomatik Program (sketch sabit)

| Röle | ON | OFF |
|---|---|---|
| 0 | 06:00 | 22:00 |
| 1 | 18:00 | 23:30 |
| 2 | 12:00 | 14:00 |
| 3 | 20:00 | 23:00 |

**12:00** butonu Arduino saatini öğlene resetler (panel RTC'ye de yazılır).
**Auto** butonu mod toggle (AUTO/MANUAL).
**Toggle 0..3** butonları manuel mod'a geçirip o röleyi tersine çevirir.

## Donanim

| Röle | Pin |
|---|---|
| 0 | Panel GPIO_0 |
| 1 | Panel GPIO_1 |
| 2 | Panel GPIO_2 |
| 3 | Panel GPIO_3 |

## Component

| Nesne | Tur |
|---|---|
| `dR0..dR3` | EButton (Active=False) – LED gösterge |
| `bR0..bR3` | EButton – manuel toggle |
| `bAuto`, `bSetNoon` | mod / saat reset |
| `lTime`, `lMode` | "HH:MM:SS" / "AUTO" / "MANUAL" |

![1](1.png)
