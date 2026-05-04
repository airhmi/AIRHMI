# Label Scroll Animation

LabelBox'da kat numarasi gibi degerlerin kayarak degismesini saglar. Asansor HMI gibi uygulamalarda kullanilir.

## Kullanim

### OnStart'ta Animasyonu Ac
```c
#include "stk.h"

LabelSet("L_Sayi", "ScrollEnable", "1");  /* Dikey otomatik yon */
LabelSet("L_Sayi", "ScrollSpeed", "6");   /* Orta hiz */
LabelSet("L_Sayi", "Text", "0");
```

### Text Degistiginde Animasyon Otomatik Baslar
```c
LabelSet("L_Sayi", "Text", "5");  /* 0 -> 5: yukari kayar */
LabelSet("L_Sayi", "Text", "3");  /* 5 -> 3: asagi kayar */
```

## ScrollEnable Modlari

| Deger | Aciklama |
|-------|----------|
| 0 | Kapali (normal davranis) |
| 1 | Dikey, otomatik yon (deger artarsa yukari, azalirsa asagi) |
| 2 | Yatay, otomatik yon |
| 3 | Her zaman yukari |
| 4 | Her zaman asagi |
| 5 | Her zaman sol |
| 6 | Her zaman sag |

## ScrollSpeed

Animasyonun kac adimda tamamlanacagini belirler.

| Deger | Hiz |
|-------|-----|
| 3 | Hizli |
| 6 | Orta (varsayilan) |
| 12 | Yavas |
| 20 | Cok yavas |

```c
LabelSet("L_Sayi", "ScrollSpeed", "3");   /* Hizli */
LabelSet("L_Sayi", "ScrollSpeed", "12");  /* Yavas */
```

## Ornek Proje

Bu ornekte:
- **L_Sayi**: 120px buyuklukte LabelBox, scroll animasyonu acik
- **UP (+1)**: Kat numarasini arttirir (yukari kayar)
- **DOWN (-1)**: Kat numarasini azaltir (asagi kayar)
- **Slow / Fast**: Animasyon hizini degistirir
