# 01 — Smart Home Hub (5 sayfa)

5 ekranlı akıllı ev kontrol paneli. Sayfa geçişleri tab butonlarındaki
panel-side `ChangeScreenSet("ScreenN");` script'leri ile yapılır —
Arduino tarafına touch event gitmez, sadece veri butonlarının event'leri
gelir.

## Sayfalar

| Screen | Ad | İçerik |
|---|---|---|
| 1 | Dashboard | Temperatur, ev durumu (Open/Secured), karşılama metni |
| 2 | Lighting  | 4 oda lambası toggle + global brightness slider + Apply |
| 3 | Climate   | Hedef sıcaklık slider (16..30°C) + 3 mod (HEAT/COOL/OFF) |
| 4 | Security  | ARM/DISARM toggle + büyük durum metni |
| 5 | Settings  | Sistem bilgisi + saat/tarih reset butonu |

Her sayfanın üst kısmında **5 tab butonu** vardır (aktif sekme yeşil,
diğerleri gri). Tab tıklaması panel-side `ChangeScreenSet`.

## Sketch State

| Değişken | Tür | Sayfa | Açıklama |
|---|---|---|---|
| `lamp[4]` | bool | Lighting | her odanın ON/OFF durumu |
| `bright` | uint8_t | Lighting | global brightness (0..100, PWM_0) |
| `target` | int8_t | Climate | hedef sıcaklık (16..30°C) |
| `hvacMode` | char | Climate | 'H'/'C'/'O' |
| `armed` | bool | Security | sistem armed mı |

Security state'i `lHomeStatus` (Dashboard) etiketine de yansır:
ARMED iken "Secured", aksi halde "Open".

## Component Sayısı

- 5 tab butonu × 5 sayfa = 25 (panel-side, Arduino tanımlamaz)
- Veri komponentleri: 4 lamba + 1 slider + 1 apply (Lighting) +
  1 slider + 3 mod + 1 mode label (Climate) + 1 arm/disarm (Security) +
  1 set clock (Settings) = 12 Arduino-bağlı komponent

## Donanım

- Yalnızca panel + Arduino + UART
- Lighting Apply → PWM_0'a duty% gider (gerçek panel'de PWM kanalı)
- Settings Set Clock → AirRtc.timeSet/dateSet (panel RTC chip'i)

![1](1.png)
