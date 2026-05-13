# 10 — Multi Alarm Clock ⭐

Arduino kendi saatini millis() bazlı tutar; 5 sabit alarm slotu (07:00,
08:30, 12:00, 18:00, 22:00) saat eşleşince buzzer 2 sn cinlatır + status
"RING!" yazılır. Her slot ayrı toggle butonu ile aktif/pasif.

## Slot Saatleri (sabit)

| Slot | Saat |
|---|---|
| 0 | 07:00 |
| 1 | 08:30 |
| 2 | 12:00 |
| 3 | 18:00 |
| 4 | 22:00 |

> Saatleri değiştirmek için sketch içindeki `SLOT_H[]` / `SLOT_M[]`
> dizilerini düzenleyin.

## Component

| Nesne | Tur |
|---|---|
| `lTime` | "HH:MM:SS" büyük etiket |
| `lStatus` | "OK" / "RING! slot N" |
| `bSlot0..4` | toggle (saat + ON/OFF) |
| `bSetMorning` | saati 06:55'e ayarla (alarm test için) |
| `bSnooze` | aktif çalan alarmı durdur |

![1](1.png)
