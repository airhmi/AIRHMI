# Arduino ile AirButton Arka Plan Rengi Yönetimi (Color)

Bu örnek, Arduino tarafından AirHMI panelindeki bir butonun arka plan rengini ve gradient bitiş rengini yazmak ve okumak için `AirButton`'ın Color metotlarını gösterir.

## Klasör Yapısı

```
Color/
├── Color.ino    ← Arduino sketch'i
├── Color.ahi    ← AirHMI Studio panel projesi (800×480)
└── README.md    ← Bu doküman
```

## Renk Formatı

Panel renkleri 32-bit ARGB (`0xAARRGGBB`) bit pattern olarak saklar. Panelin XML projesinde renkler **signed int** olarak yazılır (örn. `-65536` = kırmızı).

| Renk | Hex (ARGB) | Signed int |
|---|---|---|
| Kırmızı | `0xFFFF0000` | `-65536` |
| Yeşil | `0xFF00FF00` | `-16711936` |
| Mavi | `0xFF0000FF` | `-16776961` |
| Sarı | `0xFFFFFF00` | `-256` |
| Beyaz | `0xFFFFFFFF` | `-1` |
| Siyah | `0xFF000000` | `-16777216` |

`Set_background_color(uint32_t)` artık `%ld` + `(int32_t)` cast kullanır → kullanıcı `0xFFFF0000` gönderdiğinde panele "-65536" gider.

## Kullanılan AirButton Metotları

### Yazma — `Set_background_color`, `Set_background_colorTo`
```cpp
bDemo.Set_background_color(0xFFFF0000);     // ana renk = kırmızı
bDemo.Set_background_colorTo(0xFF800000);   // gradient bitiş = koyu kırmızı
```
- Panel komutu: `BtnS(bDemo,Color,-65536)` ve `BtnS(bDemo,ColorTo,-8388608)`

### Okuma — `Get_background_color`, `Get_background_colorTo`
```cpp
uint32_t v = 0;
bDemo.Get_background_color(&v);
// v = bit pattern; (int32_t)v ile signed int olarak yazdırabilirsin
```
- Panel komutu: `BtnG(bDemo,Color,NULL)` → `0x01...0x7E 0x6F` framed yanıt

## Panel Tarafı (Color.ahi)

| Nesne | Tür | İşlev |
|---|---|---|
| `bDemo` | EButton | Rengi değişen ana buton |
| `bRed` / `bGreen` / `bBlue` / `bYellow` | EButton | Hazır renk presetleri → `Set_background_color` |
| `bGradient` | EButton | `Set_background_color` + `Set_background_colorTo` (altın → koyu kırmızı) |
| `bRead` | EButton | Color + ColorTo okuyup etiketlere yazar |
| `bReset` | EButton | Beyaz/sarı default'a döndürür |
| `lColor` | ELabelBox | Color değerinin gösterildiği etiket |
| `lColorTo` | ELabelBox | ColorTo değerinin gösterildiği etiket |

## Çalışma Akışı

1. **Yükleme**
   - `Color.ahi`'yi simülatöre veya panele yükle.
   - `Color.ino`'yu Arduino IDE veya `arduino-cli` ile karta yükle.
2. **Bağlantı**
   - 115200 baud, Arduino'nun bağlı olduğu COM portu (örn. COM14).
3. **Test**
   - Renk butonlarına bas → bDemo o renge döner.
   - **Gradient** → altın → koyu kırmızı geçişi.
   - **Read** → mevcut Color ve ColorTo değerleri etiketlerde görünür.
   - **Reset** → beyaz/sarı default'a dön.

## Genel Özet

| Yön | Arduino Çağrısı | Panel Komutu |
|---|---|---|
| Yazma | `Set_background_color(uint32_t)` | `BtnS(b,Color,N)` |
| Yazma | `Set_background_colorTo(uint32_t)` | `BtnS(b,ColorTo,N)` |
| Okuma | `Get_background_color(uint32_t*)` | `BtnG(b,Color,NULL)` |
| Okuma | `Get_background_colorTo(uint32_t*)` | `BtnG(b,ColorTo,NULL)` |

## Notlar

- **Negatif int olarak okuma:** Get fonksiyonları `uint32_t*` doldurur. Signed olarak göstermek için `(int32_t)v` cast et.
- **AVR Arduino:** `recvRetNumber` artık `strtol` kullanıyor; -16728065 gibi büyük negatif değerler doğru parse ediliyor.
- **Get framing:** Panel firmware ve simulator (PicocParser.cs) numeric Get yanıtlarını `0x01...0x7E 0x6F` ile framed gönderir.
