# Arduino Examples — AirHMI ile Arduino Kullanım Kılavuzu

Bu klasör, **AirHMI** dokunmatik ekranlarını **Arduino** ile sürmek için
hazırlanmış kapsamlı örneklerin tamamını içerir. Her örnek bağımsız bir
mini-projedir: Arduino sketch + AirHMI panel projesi (.ahi) + ekran
görüntüsü + ayrıntılı README.

> **AirDuino**, Arduino tarafından AirHMI panele seri komutlar göndererek
> panel üzerindeki nesneleri (button, label, gauge, slider, picture, vb.)
> kontrol etmeyi sağlayan resmi kütüphanedir. Panel ile Arduino arasında
> 115200 baud UART üzerinden iletişim kurulur.

## ⚡ Hızlı Başlangıç

### 1. Gerekli olanlar
- AirHMI dokunmatik ekran panel (örnekler 800×480, 5 inç ve üzeri için)
- Arduino UNO / Mega / ESP32 vb. (örnekler `arduino:avr:uno` ile derlenir)
- USB-TTL veya doğrudan serial bağlantı (115200 baud, RX/TX/GND)
- [Arduino IDE 2.x](https://www.arduino.cc/en/software) veya `arduino-cli`
- **AIRDUINO** kütüphanesi: <https://github.com/AirHmi/AIRDUINO>

### 2. Kütüphaneyi yükle
Arduino IDE → **Sketch ▸ Include Library ▸ Add .ZIP Library** ile AIRDUINO
ZIP'ini ekleyin. Veya manuel olarak `Documents/Arduino/libraries/airduno-main/`
klasörüne kopyalayın.

### 3. Bir örneği aç ve derle
```bash
arduino-cli compile --fqbn arduino:avr:uno  AirButton/Text/Text.ino
arduino-cli upload  --fqbn arduino:avr:uno  -p COM14  AirButton/Text/Text.ino
```

### 4. Panel projesini ekrana yükle
- AirHMI Visual Screen Creator'ı aç
- İlgili klasördeki `.ahi` dosyasını **Open** ile aç
- **Build & Upload** ile panele yükle

### 5. Bağla ve kullan
- Arduino TX → Panel RX, Arduino RX → Panel TX, GND ortak
- Panel ekranındaki butonlara dokun → Arduino sketch tepki verir
- Arduino'dan da panele komut gider (label/gauge/değişken güncellemesi)

## 📁 Klasör Yapısı

Her component (AirButton, AirLabel, AirGauge, ...) için bir alt klasör vardır.
Bazı component'lar tek bir kapsamlı örnekle (`Basics/`), bazıları konuya
ayrılmış birden fazla alt-örnekle gelir (`AirButton/Text/`,
`AirButton/Color/`, vb.).

```
Arduino Examples/
├── AirButton/             8 alt-örnek (Text, Color, Font, Position, ...)
├── AirLabel/              5 alt-örnek
├── AirGauge/              4 alt-örnek
├── AirPicture/            3 alt-örnek
├── AirVariable/           1 örnek (kapsamlı)
├── AirSlider/             1 örnek
├── AirProgressBar/        1 örnek
├── AirGraph/              1 örnek
├── AirShape/              1 örnek
├── AirRtc/                1 örnek
├── AirGpio/               1 örnek (digital + analog + PWM)
├── AirTimer/              1 örnek
├── AirBuzzer/             1 örnek
├── NESNE_OZELLIKLERI.md   Tüm nesnelerin Set/Get özellik matrisi
└── README.md              (bu dosya)
```

## 🔍 Bir Örneğin Anatomisi

Her alt-örnek aynı 4 dosyayı içerir:

| Dosya         | İçerik |
|---------------|--------|
| `<Name>.ino`  | Arduino sketch — HMI nesneleri, callback'ler, dispatcher |
| `<Name>.ahi`  | AirHMI panel projesi (Visual Screen Creator ile açılır) |
| `1.png`       | Çalışan örnekten ekran görüntüsü |
| `README.md`   | Hangi metotlar kullanıldı, panel nesneleri, test sonucu |

Her README aşağıdaki bölümleri içerir:

- **Kullanılan Metotlar** — sketch'teki API çağrılarının listesi
- **Panel Tarafı** — `.ahi` içindeki nesneler ve işlevleri
- **Genel Özet** — Arduino çağrısı ↔ panel komutu eşleşmesi
- **Notlar** — varsa kütüphane/firmware/simülatör düzeltmeleri

## 🧩 Component Listesi

| Component         | Alt-Örnek Sayısı | Kapsanan Özellikler |
|-------------------|------------------|---------------------|
| **AirButton**     | 8 | Text, Color, PressColor, Font, Position, Visible/Active, PushPull, Gradient |
| **AirLabel**      | 5 | Text, Font, Position, Visible/Active, Center/Scroll |
| **AirGauge**      | 4 | Value, Position, Style (needle), Visible |
| **AirPicture**    | 3 | File/Position, Rotation, PushPull |
| **AirSlider**     | 1 | Value, Color, BackgroundColor, ThumbColor |
| **AirProgressBar**| 1 | Value, Color, BackgroundColor, BorderColor |
| **AirGraph**      | 1 | addValue, line color, clear |
| **AirShape**      | 1 | Position (left/top/width/height), Active (touch alanı) |
| **AirVariable**   | 1 | VarSet/VarSetf/VarSeti, VarGet/VarGetf/VarGeti |
| **AirRtc**        | 1 | dateSet, timeSet |
| **AirGpio**       | 1 | digital_write/read, analog_read, set_pwmfreq |
| **AirTimer**      | 1 | setInterval, getInterval, enable, disable |
| **AirBuzzer**     | 1 | Set_Buzzer (ms cinsinden süre) |

> Her component için tüm Set/Get matrisini görmek için
> [NESNE_OZELLIKLERI.md](NESNE_OZELLIKLERI.md) dosyasına bakın.

## 🛠️ Tipik Sketch İskeleti

Bütün örnekler bu pattern'i izler:

```cpp
#include <Airhmi.h>

/* 1) HMI nesnelerini panel-side isimleri ile tanımla */
AirButton bDemo = AirButton("bDemo");
AirLabel  lOut  = AirLabel("lOut");

/* 2) Dokunma callback'leri */
void onPop(void *ptr)
{
    lOut.setText("Touched!");
    bDemo.Set_background_color(RED);
}

/* 3) airLoop dispatcher listesi (panel'den gelen olayları yönlendirir) */
AirTouch *air_listen_list[] = {
    &bDemo,
    NULL  // listenin sonu mutlaka NULL olmalı
};

void setup()
{
    Serial.begin(115200);   // panel ile aynı baud
    airInit();              // kütüphane init

    /* 4) Callback'leri bağla */
    bDemo.attachPop(onPop, &bDemo);
}

void loop()
{
    airLoop(air_listen_list);   // panel mesajlarını dinle ve dağıt
}
```

## 🔑 Sık Kullanılan API Pattern'leri

### Set / Get
Panel nesnelerinin tüm görsel özellikleri Arduino'dan değiştirilebilir:
```cpp
button.setText("Hello");
button.Set_background_color(0xFF0000);
button.Set_font_size(20);
button.Set_visible(0);  // gizle

uint32_t value;
button.Get_background_color(&value);
```

### Olay yakalama
- `attachPush` — dokunma başladığında (parmak değdiğinde)
- `attachPop` — dokunma bittiğinde (parmak kalktığında)
- `attachClick` — tam tıklama
- `attachPress` — basılı tutma

### AirVariable ile veri paylaşımı
Panel script'leri ile Arduino arasında veri iletmek için EVariable kullanın:
```cpp
AirVariable counter = AirVariable("counter");
counter.VarSeti(42);          // panel'e yaz

int v;
counter.VarGeti(&v);           // panel'den oku
```

## 🧪 Sorun Giderme

| Belirti | Olası neden / çözüm |
|---|---|
| Butonlara basılınca Arduino tepki vermiyor | Baud rate uyumsuz; `Serial.begin(115200)` ve panel device baud aynı olmalı |
| `Get_*` çağrıları timeout'a düşüyor | TX/RX kabloları ters; veya panel firmware o attribute için Arduino framing göndermiyor |
| `Serial.print` kullanınca panel komutları bozuluyor | `airSerial = Serial` paylaşımlı; `Serial.print` kullanmayın, debug için `lDebug.setText(...)` kullanın |
| Sketch derlenmiyor / `Air*` bulunamadı | AIRDUINO kütüphanesi yüklü değil — `Documents/Arduino/libraries/airduno-main/` |
| Panel başlamadı / boş ekran | `.ahi` dosyasını **Build & Upload** ile panele yüklemeyi unutmayın |

## 📚 Daha Fazlası

- AIRDUINO kütüphane kaynak kodu: <https://github.com/AirHmi/AIRDUINO>
- AirHMI Visual Screen Creator ve dokümantasyon: ana repo `Docs/` klasörü
- Panel donanım datasheet'leri: ana repo `Datasheets/` klasörü

