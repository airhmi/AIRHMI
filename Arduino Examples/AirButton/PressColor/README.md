# Arduino ile AirButton Basılı Renk Yönetimi (Press Color)

Bu örnek, AirHMI butonunun **basılı durumdaki** renklerini Arduino tarafından kontrol eder. Buton üzerinde parmak basılı tutulduğunda gösterilen `Press_Color` ve gradient bitişi `Press_ColorTo` değerlerini yazar/okur.

## Klasör Yapısı

```
PressColor/
├── PressColor.ino    ← Arduino sketch'i
├── PressColor.ahi    ← AirHMI panel projesi (800×480)
└── README.md         ← Bu doküman
```

## Color vs Press Color

| Durum | Kullanılan alan |
|---|---|
| Buton serbest | `Color` / `ColorTo` |
| Buton basılı | `Press_Color` / `Press_ColorTo` |
| Push-pull modu açıkken (`<PushPull>True</PushPull>`) | `Press_*` toggle state'i 1 olduğunda |

## Kullanılan AirButton Metotları

### Yazma
```cpp
bDemo.Set_press_background_color(0xFFFF0000);     // basılı renk = kırmızı
bDemo.Set_press_background_colorTo(0xFF800000);   // gradient bitiş = koyu kırmızı
```
- Panel komutu: `BtnS(bDemo,Press_Color,-65536)` / `BtnS(bDemo,Press_ColorTo,-8388608)`

### Okuma
```cpp
uint32_t v = 0;
bDemo.Get_press_background_color(&v);
bDemo.Get_press_background_colorTo(&v);
```
- Panel komutu: `BtnG(bDemo,Press_Color,NULL)` / `BtnG(bDemo,Press_ColorTo,NULL)`
- Panel framed yanıt gönderir → `recvRetNumber` parse eder.

## Panel Tarafı (PressColor.ahi)

| Nesne | Tür | İşlev |
|---|---|---|
| `bDemo` | EButton | Demo buton (basılı tutunca press renkleri görünür) |
| `bRedPress` / `bGreenPress` / `bBluePress` / `bDarkPress` | EButton | Press_Color preset'leri |
| `bGradientPress` | EButton | Altın → koyu kırmızı gradient (Press_Color + Press_ColorTo) |
| `bRead` | EButton | Press_Color + Press_ColorTo okur, etiketlere yazar |
| `bReset` | EButton | Açık gri / koyu gri default'a döndürür |
| `lPressColor` | ELabelBox | Press_Color değerinin gösterildiği etiket |
| `lPressColorTo` | ELabelBox | Press_ColorTo değerinin gösterildiği etiket |

## Çalışma Akışı

1. **Yükleme**
   - `PressColor.ahi`'yi simülatöre veya panele yükle.
   - `PressColor.ino`'yu Arduino'ya yükle.
2. **Test**
   - Renk preset butonlarına bas → bDemo'nun press renkleri set edilir.
   - **bDemo butonuna parmağını basılı tut** → press renkleri görünür ("BASILI" yazısıyla).
   - **Read** → Press_Color + Press_ColorTo değerleri etiketlere yazılır.
   - **Reset** → açık gri / koyu gri default'a dön.

## Genel Özet

| Yön | Arduino Çağrısı | Panel Komutu |
|---|---|---|
| Yazma | `Set_press_background_color(uint32_t)` | `BtnS(b,Press_Color,N)` |
| Yazma | `Set_press_background_colorTo(uint32_t)` | `BtnS(b,Press_ColorTo,N)` |
| Okuma | `Get_press_background_color(uint32_t*)` | `BtnG(b,Press_Color,NULL)` |
| Okuma | `Get_press_background_colorTo(uint32_t*)` | `BtnG(b,Press_ColorTo,NULL)` |

## Notlar

- **Press_Color etkisini görmek için bDemo butonuna basılı tutmak gerekir.** Anlık tıklamada press renk anlık görünür ve kaybolur.
- Push-pull modunda (`<PushPull>True</PushPull>`), Press renkleri **toggle state == 1** olduğu sürece kalıcı görünür. Bunun için ayrı bir örnek var (PushPull klasörü).
