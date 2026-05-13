# 11 — Music Recorder ⭐

8 piyano tuşu (DO..DO+1) basıldıkça AirBuzzer'da farklı sürede beep cıkartır;
**REC** modundayken nota dizisine eklenir, **PLAY** ile sırayla çalınır,
**CLEAR** ile temizlenir.

## Calisma

- Tuşlara her basışta `Set_Buzzer(NOTE_DUR[idx])` cinlatma
- Recording aktifse `notes[noteCount++] = idx` kayit
- PLAY: tüm notları sırayla cinlat (her birinin arasında 80 ms bosluk)
- CLEAR: dizi sıfırlanır

## Component Listesi

| Nesne | Tur | Islev |
|---|---|---|
| `bDo`..`bDo2` (8) | EButton | piyano tuşları |
| `bRec` / `bPlay` / `bClear` | EButton | kayıt kontrolü |
| `lStatus` | ELabelBox | "IDLE" / "REC" / "PLAY" |
| `lCount`  | ELabelBox | "N/32" kayıtlı nota sayısı |
| `buz` | AirBuzzer | nota cinlatma |

## Notlar

- Kayit kapasitesi 32 nota; sıfırdan başlamak için CLEAR
- PLAY çalarken `delay()` blocking; airLoop tetiklenmez
- Notalar Arduino RAM'de (kalıcı değil); Arduino reset olursa silinir

![1](1.png)
