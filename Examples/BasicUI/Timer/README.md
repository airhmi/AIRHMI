## Timer Türleri

### Private Timer
Private timer'lar, sadece tanımlandıkları sayfa veya bileşen içinde çalışır. Sayfa değiştirildiğinde veya ilgili bileşen kapatıldığında timer otomatik olarak durur. Kullanıcı aynı sayfaya geri döndüğünde tekrar başlatılabilir.

### Public Timer
Public timer'lar, HMI sisteminde **sayfa değişimlerinden bağımsız olarak çalışan** zamanlayıcılardır. Uygulama genelinde sürekli çalışan görevler için kullanılır. Sayfalar arası geçiş yapıldığında durmaz, tüm sistem boyunca çalışmaya devam eder.

## Timer Interval Değiştirme

HMI sistemlerinde bir timer'ın çalışmasını özelleştirmenin bir diğer yolu da **interval süresini** değiştirmektir. Timer interval süresi, belirlenen zaman aralığında işlemin tekrar edilmesini sağlar. **TimerSet()** fonksiyonu ile timer'ın interval süresi değiştirilerek farklı periyotlarda çalıştırılması sağlanabilir.

Örnek kullanım:

```c
TimerSet("ETimer1", "Interval", "1000");
```

### Fonksiyon Parametreleri

1. **"ETimer1"**: Değiştirilecek timer'ın adı.
2. **"Interval"**: Timer'ın tekrar süresinin değiştirileceğini belirtir.
3. **"1000"**: Timer'ın yeni interval süresi (milisaniye cinsinden). Burada 1000 ms (1 saniye) olarak ayarlanmıştır.

Bu komut ile **ETimer1** her **1000 ms (1 saniye)** aralıklarla çalışacaktır. Eğer süre **500 ms** olarak ayarlanırsa, timer daha sık tetiklenir.

```c
TimerSet("ETimer1", "Interval", "500");
```

### Timer Enable ve Disable

Bir timer'ın aktif olup olmadığını kontrol etmek için **TimerSet()** fonksiyonunun **Enable** parametresi kullanılır. Örneğin:

```c
TimerSet("ETimer1", "Enable", "True"); // Timer'ı başlatır
TimerSet("ETimer1", "Enable", "False"); // Timer'ı durdurur
```

Bu sayede timer'lar dinamik olarak kontrol edilebilir ve belirli koşullara bağlı olarak başlatılıp durdurulabilir.
