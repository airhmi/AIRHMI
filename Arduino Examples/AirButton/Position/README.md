# Arduino ile AirButton Konum ve Boyut Yönetimi (Position)

Bu örnek, AirHMI butonunun **Left, Top, Width, Height** özelliklerini Arduino tarafından kontrol eder. Yön butonlarıyla butonu hareket ettirir, boyut butonlarıyla yeniden boyutlandırır.

## Klasör Yapısı

```
Position/
├── Position.ino    ← Arduino sketch'i
├── Position.ahi    ← AirHMI panel projesi (800×480)
└── README.md       ← Bu doküman
```

## Kullanılan AirButton Metotları

### Yazma
```cpp
bDemo.Set_left(320);     // X = 320
bDemo.Set_top(100);      // Y = 100
bDemo.Set_width(160);    // genişlik
bDemo.Set_height(70);    // yükseklik
```
Panel komutları: `BtnS(b,Left,N)`, `BtnS(b,Top,N)`, `BtnS(b,Width,N)`, `BtnS(b,Height,N)`

Panel limitleri ([03_button.c](file)):
- `LEFT < 0` veya `LEFT > AIR_LCD_WIDTH` → silently rejected
- `TOP < 0` veya `TOP > AIR_LCD_HEIGHT` → silently rejected
- `WIDTH`, `HEIGHT` aynı sınırlarla denetlenir

### Okuma
```cpp
uint32_t v = 0;
bDemo.Get_left(&v);
bDemo.Get_top(&v);
bDemo.Get_width(&v);
bDemo.Get_height(&v);
```
Panel komutları: `BtnG(b,Left,NULL)` vb. — framed yanıt.

## Panel Tarafı (Position.ahi)

| Nesne | Tür | İşlev |
|---|---|---|
| `bDemo` | EButton | Hareketli/yeniden boyutlanabilir buton (default 320,100 / 160×70) |
| `bUp` / `bDown` | EButton | Top -50 / +50 |
| `bLeft` / `bRight` | EButton | Left -50 / +50 |
| `bSmaller` / `bLarger` | EButton | Width & Height -20 / +20 |
| `bRead` | EButton | Left/Top/Width/Height okuyup etiketlere yazar |
| `bReset` | EButton | Default değerlere döner |
| `lLeft` / `lTop` / `lWidth` / `lHeight` | ELabelBox | Get sonuçları |

## Çalışma Akışı

1. **Yükleme**
   - `Position.ahi`'yi simülatöre veya panele yükle.
   - `Position.ino`'yu Arduino'ya yükle.
2. **Test**
   - **Up/Down/Left/Right** → bDemo hareket eder.
   - **Smaller/Larger** → bDemo boyutu değişir.
   - **Read** → Mevcut değerler etiketlere yazılır.
   - **Reset** → Default konuma döner.

## Genel Özet

| Yön | Arduino Çağrısı | Panel Komutu |
|---|---|---|
| Yazma | `Set_left(uint32_t)` | `BtnS(b,Left,N)` |
| Yazma | `Set_top(uint32_t)` | `BtnS(b,Top,N)` |
| Yazma | `Set_width(uint32_t)` | `BtnS(b,Width,N)` |
| Yazma | `Set_height(uint32_t)` | `BtnS(b,Height,N)` |
| Okuma | `Get_left(uint32_t*)` | `BtnG(b,Left,NULL)` |
| Okuma | `Get_top(uint32_t*)` | `BtnG(b,Top,NULL)` |
| Okuma | `Get_width(uint32_t*)` | `BtnG(b,Width,NULL)` |
| Okuma | `Get_height(uint32_t*)` | `BtnG(b,Height,NULL)` |

## Notlar

- **Sınır kontrolü Arduino'da:** Panel out-of-bounds değerleri sessizce reddediyor. Sketch sınır kontrolü yapıyor (0..LCD_WIDTH/HEIGHT) ki Read sonucu beklenenden sapmasın.
- **WinForms görsel update:** Önceki simulator versiyonu sadece data property'sini güncelliyordu (görsel hareket yok). Yeni `PicocParser.cs` artık `btn.Left/Top/Width/Height`'ı da güncelliyor → buton ekranda hareket eder.
- **Panel statik sayfada hareket:** Panel firmware [03_button.c](file) sadece `widgetInThisPage == 1` ise `WM_MoveTo` / `WM_SetSize` çağırır. Statik (header/footer) butonlarda anlık görsel update olmayabilir.
