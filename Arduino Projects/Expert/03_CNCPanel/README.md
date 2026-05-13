# 03 — CNC Panel (4 sayfa)

CNC tezgah kontrol HMI prototipi. 4 sayfa: Jog / Auto / Settings / Diagnostics.

## Sayfalar

| Screen | İçerik |
|---|---|
| 1 Jog | X/Y/Z +/- jog (1mm step), HOME, anlık koordinatlar |
| 2 Auto | RUN / PAUSE / STOP + ilerleme barı + % gösterge |
| 3 Settings | Feed rate (0..3000 mm/min) + Acceleration (0..100%) sliderlar |
| 4 Diagnostics | Motors ON/OFF, E-Stop toggle, X/Y/Z limit sensor durumları |

## Mantık

- Jog adım sabit 1.0 mm; her tıkta `posX/posY/posZ` güncellenir
- Auto modda her 500 ms'de %1 ilerleme (simülasyon, 50 sn'de tamamlar)
- E-Stop motorları kapatır + Auto çalışmasını durdurur
- Tüm pozisyon/koordinat hesabı Arduino'da tutulur, panel sadece UI

> Gerçek motor sürücü için (TB6600/A4988 gibi) STEP/DIR sinyalleri eklenmeli;
> bu prototipte sadece HMI tarafı modellenmiştir.

![1](1.png)
