# 06 — Home Security (4 sayfa)

4 sayfalı ev güvenlik paneli: Status / Zones / PIN / Log.

## Sayfalar

| Screen | İçerik |
|---|---|
| 1 Status | ARMED/DISARMED büyük metni + 4 zone göstergesi (Door/Window/Garage/Backyard) |
| 2 Zones | 4 zone enable/disable toggle (büyük butonlar) |
| 3 PIN | 0-9 numpad (3×3+) + display (mask "*") + OK / Cancel + mesaj satırı |
| 4 Log | Son 5 olay (üst en yeni) |

## PIN

- Sabit: **`1234`**
- Doğru girince armed durumu toggle olur
- Yanlış PIN → "Wrong PIN" + 500 ms buzzer + log'a yazılır

## Log

Ring buffer 5 satır × 24 karakter. Her PIN denemesinde / arm-disarm
işleminde log'a satır eklenir, eski kayıtlar aşağı kaydırılır.

## Zone'lar

```
Door / Window / Garage / Backyard
```

Manuel toggle ile aktif/pasif. Şu an gerçek sensör girişine bağlı değil
(donanım gerektirmez prototip).

![1](1.png)
