# HMI Değişken Yönetimi: VarSet, VarGet ve VariableSave Fonksiyonları

## 1. `VarSet` Fonksiyonları (Veri Saklama - RAM)
- **Amaç:** Değişkenlere değer atamak ve RAM üzerinde saklamak.
- **Kullanımı:**
  - `VarSeti("DeğişkenAdı", int_değer);` → **Integer** veri saklar.
  - `VarSet("DeğişkenAdı", char_dizi);` → **String** veri saklar.
  - `VarSetf("DeğişkenAdı", float_değer);` → **Float/Double** veri saklar.
- **Özellik:** RAM üzerinde çalışır, **sistem kapanınca kaybolur**.

## 2. `VarGet` Fonksiyonları (Veri Okuma - RAM)
- **Amaç:** Daha önce `VarSet` ile saklanan değişkenleri almak.
- **Kullanımı:**
  - `VarGet("DeğişkenAdı", &int_değişken);` → **Integer** değer alır.
  - `VarGet("DeğişkenAdı", char_dizi);` → **String** değer alır.
  - `VarGet("DeğişkenAdı", &float_değişken);` → **Float/Double** değer alır.
- **Özellik:** Sadece **RAM'de bulunan** değişkenleri alabilir, sistem kapanınca veri kaybolur.

## 3. `VariableSave` Fonksiyonu (Kalıcı Hafızaya Kaydetme)
- **Amaç:** Değişkeni **kalıcı hafızaya (EEPROM, Flash, NVRAM)** kaydetmek.
- **Kullanımı:**
  - `VariableSave("DeğişkenAdı");` → Belirtilen değişken **kalıcı olarak** saklanır.
- **Özellik:** **Sistem kapansa bile** veri korunur, `VarGet` ile geri çağrılabilir.

## Özet Karşılaştırma
| Fonksiyon | Amacı | Saklama Türü | Sistem Yeniden Başlatıldığında Korunur mu? |
|-----------|------|-------------|----------------------------------|
| `VarSet` | Değişkeni RAM'e kaydetmek | RAM | ❌ Kaybolur |
| `VarGet` | RAM'deki değişkeni okumak | RAM | ❌ Kaybolur |
| `VariableSave` | Değişkeni kalıcı hafızaya kaydetmek | EEPROM/Flash/NVRAM | ✅ Korunur |

Bu yapı, **HMI sistemlerinde verilerin saklanmasını ve korunmasını sağlar.** 🚀
