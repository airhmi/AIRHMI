# 13 — Reaction Multiplayer ⭐

İki oyunculu reaksiyon yarışı. Start sonrası 1..4 sn rastgele bekleme,
sonra `bGo` yeşile döner ("GO!") ve "P1 HIT" / "P2 HIT" butonlarından
hangisi önce basarsa kazanır. Tepki süresi ms olarak yazılır.

- Erken basan oyuncu **FOUL** alır → rakibe 1 puan.
- En iyi süre `best` AirVariable'da panel-side kalıcı tutulur.

## Component

| Nesne | Tur |
|---|---|
| `bStart` | EButton – yeni round başlat |
| `bGo` | EButton (Active=False) – GO ışığı (gri/yeşil/kırmızı) |
| `bP1`, `bP2` | büyük oyuncu butonları (mavi / kırmızı) |
| `lInfo` | mesaj satırı |
| `lLastMs` | son tepki süresi |
| `lScore` | "P1 N : M P2" |
| `lBest` | "Best: NN ms" |
| `best` | EVariable kalıcı en iyi süre |

![1](1.png)
