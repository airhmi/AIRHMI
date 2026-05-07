# Arduino ile AirButton Yazı Tipi Yönetimi (Font)

Bu örnek, AirHMI butonunun **yazı tipi adını, boyutunu ve rengini** Arduino tarafından kontrol eder.

## Klasör Yapısı

```
Font/
├── Font.ino    ← Arduino sketch'i
├── Font.ahi    ← AirHMI panel projesi (800×480)
└── README.md   ← Bu doküman
```

## Kullanılan AirButton Metotları

### Yazma
```cpp
// Font adı (panelde tanımlı olan TTF/sistem fontu)
bDemo.setFont(String("Roboto"));

// Font boyutu (panel limitleri: 4..500)
bDemo.Set_font_size(22);

// Font rengi (signed int olarak gönderilir, ARGB)
bDemo.Set_font_color(0xFFFF0000);   // kırmızı

// Font rengi hex string ile (panel strtol(...,16) ile parse eder)
bDemo.Set_font_colorRGB(String("#FF0000"));
```
- Panel komutları:
  - `BtnS(b,FontName,"Roboto")`
  - `BtnS(b,Font_Size,22)`
  - `BtnS(b,Font_Color,-65536)`
  - `BtnS(b,Font_Color,"#FF0000")`

### Okuma
```cpp
char fontBuf[24] = {0};
bDemo.getFont(fontBuf, sizeof(fontBuf));   // → "Roboto"

uint32_t size = 0;
bDemo.Get_font_size(&size);                // → 22

uint32_t color = 0;
bDemo.Get_font_color(&color);              // → bit pattern
```

## Panel Tarafı (Font.ahi)

| Nesne | Tür | İşlev |
|---|---|---|
| `bDemo` | EButton | "The quick brown fox" demo metni |
| `bRoboto` / `bArial` | EButton | Font adı presetleri |
| `bSize16` / `bSize22` / `bSize28` / `bSize36` | EButton | Font boyutu presetleri |
| `bBlack` / `bWhite` / `bRed` / `bBlue` | EButton | Font rengi presetleri (signed int) |
| `bRedHex` | EButton | `#FF0000` ile hex string font rengi |
| `bRead` | EButton | FontName + Font_Size + Font_Color okur |
| `bReset` | EButton | Roboto / 22 / siyah default'a döner |
| `lFontName` / `lFontSize` / `lFontColor` | ELabelBox | Get sonuçlarının gösterildiği etiketler |

## Çalışma Akışı

1. **Yükleme**
   - `Font.ahi`'yi simülatöre veya panele yükle.
   - `Font.ino`'yu Arduino'ya yükle.
2. **Test**
   - Font adı butonlarına bas → bDemo metni o fontla görünür.
   - Font boyutu butonlarına bas → metin büyür/küçülür.
   - Font rengi butonlarına bas → metin rengi değişir.
   - **#FF0000** butonu → hex string ile renk (decimal int yerine).
   - **Read** → mevcut font ayarları etiketlere yazılır.
   - **Reset** → Roboto / 22 / siyah default'a dön.

## Genel Özet

| Yön | Arduino Çağrısı | Panel Komutu |
|---|---|---|
| Yazma | `setFont(String)` | `BtnS(b,FontName,"...")` |
| Yazma | `Set_font_size(uint32_t)` | `BtnS(b,Font_Size,N)` |
| Yazma | `Set_font_color(uint32_t)` | `BtnS(b,Font_Color,N)` |
| Yazma | `Set_font_colorRGB(String)` | `BtnS(b,Font_Color,"#RGB")` |
| Okuma | `getFont(char*, int)` | `BtnG(b,FontName,NULL)` |
| Okuma | `Get_font_size(uint32_t*)` | `BtnG(b,Font_Size,NULL)` |
| Okuma | `Get_font_color(uint32_t*)` | `BtnG(b,Font_Color,NULL)` |

## Notlar

- **Font adı:** `setFont` artık çift tırnak içinde gönderir → boşluklu adlar (örn. "Times New Roman") destekler.
- **Hex string renk:** `#RRGGBB` formatı 0..0xFFFFFF arası RGB (alpha yok). Panel `strtol(hex, NULL, 16)` ile parse eder, `btn->Font_Color` alanına yazar.
- **Font boyut limitleri:** Panel `< 4` veya `> 500` değerleri sınıra çeker.
- **getFont** panel firmware'da framed gönderildiği için (CButtonGetEx — FONTNAME dalında `isArduinoConnected()` checkleme zaten vardı), bu metot mevcut firmware ile sorunsuz çalışır.
