# 02 — Industrial Control Panel (4 sayfa)

Endüstriyel üretim hattı kontrol paneli. 4 sayfa: Dashboard / Recipe / Alarms / Settings.

## Sayfalar

| Screen | İçerik |
|---|---|
| 1 Dashboard | Üretim sayacı, vardiya, hat durumu, START/STOP butonu |
| 2 Recipe | 5 reçete preset, seçilince ad + P1/P2 parametreleri gösterilir |
| 3 Alarms | 4 zone alarm göstergesi (yeşil/kırmızı), Acknowledge All butonu |
| 4 Settings | Sayaç sıfırla, vardiya değiştir (DAY/NIGHT) |

## Reçete Tablosu

| # | Ad | P1 | P2 |
|---|---|---|---|
| 1 | Bottle 250 | 250 | 80 |
| 2 | Bottle 500 | 500 | 120 |
| 3 | Box A | 100 | 200 |
| 4 | Box B | 200 | 300 |
| 5 | Custom | 50 | 40 |

## Simülasyon

- START → hat çalışmaya başlar, her 3 sn'de üretim sayacı +1
- Her 50 parçada bir alarm zonu otomatik tetiklenir + buzzer beep
- Acknowledge tüm alarmları temizler

## Sayfa Geçişi

Tab butonlarındaki panel-side `ChangeScreenSet("ScreenN");` script'leri ile.

![1](1.png)
