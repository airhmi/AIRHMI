# Arduino ile AirButton Metin Yönetimi (Text)

Bu örnek, Arduino tarafından AirHMI panelindeki bir butonun metnini (caption) yazmak ve okumak için `AirButton` sınıfının metin metotlarını gösterir.

## Klasör Yapısı

```
Text/
├── Text.ino    ← Arduino sketch'i
├── Text.ahi    ← AirHMI Studio panel projesi (800×480)
└── README.md   ← Bu doküman
```

## Kullanılan AirButton Metotları

### 1. Düz Metin Yazma — `setText`
```cpp
AirButton bDemo = AirButton("bDemo");

bDemo.setText("Merhaba AirHMI");
```
- Panel komutu: `BtnS(bDemo,Text,"Merhaba AirHMI")`
- Metni çift tırnak içinde gönderir; panel `remove_quotes` ile sıyırır.

### 2. Tamsayıdan Metin — `setTexti`
```cpp
bDemo.setTexti(12345);
```
- Panel komutu: `ButtonSeti(bDemo,12345)` (3-parametreli kısayol)
- Sayaç, sensör değeri vb. tamsayıları doğrudan butona yazar.

### 3. Ondalıktan Metin — `setTextf`
```cpp
bDemo.setTextf(24.78);
```
- Panel komutu: `ButtonSetd(bDemo,24.7800)` (`dtostrf` ile 4 ondalık)
- Sıcaklık, voltaj gibi `double` değerleri yazar; AVR Arduino'da da çalışır (`sprintf("%f")`'in aksine).

### 4. Mevcut Metni Okuma — `getText`
```cpp
char buf[32] = {0};
bDemo.getText(buf, sizeof(buf));
lResult.setText(buf);
```
- Panel komutu: `BtnG(bDemo,Text,NULL)`
- Panel framed yanıt gönderir (`0x01...0x7E 0x6F`), buffer'a yazılır.

## Panel Tarafı (Text.ahi)

| Nesne | Tür | İşlev |
|---|---|---|
| `bDemo` | EButton | Metni yazılan/okunan ana buton |
| `bSetText` | EButton | Arduino'ya callback → `setText` |
| `bSetInt` | EButton | Arduino'ya callback → `setTexti(counter++)` |
| `bSetFloat` | EButton | Arduino'ya callback → `setTextf(temperature += 0.13)` |
| `bReadText` | EButton | Arduino'ya callback → `getText` → `lResult` |
| `bReset` | EButton | bDemo'yu "Test Button"'a, lResult'ı "---"a sıfırlar |
| `lResult` | ELabelBox | Read sonucunu gösteren etiket |

## Çalışma Akışı

1. **Yükleme**
   - `Text.ahi`'yi AirHMI Studio simülatöründe veya gerçek panelde aç.
   - `Text.ino`'yu Arduino IDE veya `arduino-cli` ile karta yükle.
2. **Bağlantı**
   - Simülatörde: Arduino'nun bağlı olduğu COM portunu (örn. COM14) seç, 115200 baud.
   - Gerçek panelde: USB-Serial üzerinden bağlan.
3. **Test**
   - **Set Text** → bDemo'da "Merhaba AirHMI" görünür.
   - **Set Int** → her tıklamada artan tamsayı yazılır.
   - **Set Float** → her tıklamada artan ondalık değer yazılır.
   - **Read** → bDemo'nun mevcut metni `lResult`'a kopyalanır.
   - **Reset** → bDemo "Test Button"'a, lResult "---"a döner.

## Genel Özet

| Yön | Arduino Çağrısı | Panel Komutu | Tip |
|---|---|---|---|
| Yazma | `setText(const char*)` | `BtnS(b,Text,"...")` | C-string |
| Yazma | `setTexti(uint32_t)` | `ButtonSeti(b,N)` | tamsayı |
| Yazma | `setTextf(double)` | `ButtonSetd(b,F)` | ondalık |
| Okuma | `getText(char*, len)` | `BtnG(b,Text,NULL)` | C-string |

## Notlar / Sınırlamalar

- **Virgüllü metin (örn. "Merhaba, Dünya"):** Panel parser'ı `strtok(",")` kullandığı için çift tırnak içindeki virgüller bile ayırıcı olarak yorumlanıyor; metin kesilir. Şimdilik virgülsüz metin gönderilmesi gerekir; panel parser quote-aware hale getirilirse tam virgüllü metinler çalışacak.
- **`airSerial = Serial`:** `Serial.print` çağrıları AirHMI protokolüne çöp olarak gider — bu örnekte hiç kullanılmamış. Debug için ya farklı bir hardware Serial port (ör. ESP32 `Serial2`), ya da `dbSerial` (AirConfig.h içinden) açılmalı.
