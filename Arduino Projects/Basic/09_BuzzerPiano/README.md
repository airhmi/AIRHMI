# 09 — Buzzer Piano

7 piyano tusu seklinde duzenlenmis butonlar; her birine basinca farkli
surede beep cikartir (60 ms .. 380 ms artarak). Gercek piyano frekans
oktavi degil — panel buzzer'i sabit frekansli, sure varyasyonu ile tini
farki yaratiyoruz.

| Tus | Sure |
|---|---|
| DO  | 60 ms  |
| RE  | 80 ms  |
| MI  | 110 ms |
| FA  | 150 ms |
| SOL | 200 ms |
| LA  | 280 ms |
| SI  | 380 ms |

## API Cagrilari

```cpp
buz.Set_Buzzer(110);   // MI -- 110 ms
```

## Notlar

- Sim'de PC speaker beep'i (BuzzerSet handler'inda Console.Beep) duyumsal
  geri donut saglar.
- Gercek frekans kontrolu icin panel firmware'a `BuzzerSetEx(int dur, int freq)`
  imzasi ile genisletme gerekir; bu ornekte tek-arg API kullaniliyor.

![1](1.png)
