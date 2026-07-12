# Arc Value Set Özelliği

Arc (dairesel gösterge) değerini yazılım ile set etmek için kullanılır.

```
ArcSet("Arc1" ,"Value" , "80"); // Arc1 dolumunu %80 yapar.

ArcSet("Arc2" ,"Value" , "30"); // Arc2 dolumunu %30 yapar.
```

Arc nesnesi, arkada sabit bir **track (zemin yayı)** ve önde değere göre dolan bir
**value yayı** olmak üzere iki katmandan oluşur. `Value`, `MinValue` ve `MaxValue`
değerlerine göre value yayının doluluk oranı hesaplanır. Ortadaki sayısal değer arc'a
gömülü değildir; ayrı bir `ELabelBox` ile gösterilir ve butonda `LabelSet` ile arc
değeriyle birlikte güncellenir.

Statik ve dinamik olmak üzere iki farklı Arc Value Set durumu üzerinde etkili olan
faktörler incelenmiştir. Statik Arc lar her sayfadan tüm özelliklerine ulaşılıp
değiştirilebilen nesnelerdir. Static(false) yani dinamik Arc lar ise sayfaya özgüdür.
Sayfa değiştiği zaman hiçbir özelliği tutulmaz. Sayfa değişip tekrar aynı sayfaya
gidildiği zaman Arc ilk hali ile baştan meydana getirilir.

## 📌 1. Arc Tanımı
- **🟢 Statik Arc**: Static özelliği true olan Arc dır. Value Set özelliği **hem aynı sayfadan hem de diğer sayfalardan** değiştirilebilir.
- **🔵 Dinamik Arc**: Static özelliği false olan Arc dır. Value Set özelliği **yalnızca aynı sayfada** değiştirilebilir, diğer sayfalardan değiştirilemez.

## 🎛️ 2. Arc Özellikleri
Arc nesnesinin `ArcSet` / `ArcGet` ile ayarlanabilen belli başlı özellikleri:

| Özellik | Açıklama |
|---|---|
| `Value` | Gösterilen değer (MinValue–MaxValue arası) |
| `MinValue` / `MaxValue` | Değer aralığı |
| `StartAngle` / `SweepAngle` | Yayın başlangıç açısı ve toplam açı genişliği (derece) |
| `TrackColor` / `TrackWidth` | Zemin (arkadaki) yayın rengi ve kalınlığı |
| `TrackVisible` | Zemin yayının görünürlüğü (0/1) |
| `ValueColor` / `ValueWidth` | Değer (öndeki) yayın rengi ve kalınlığı |
| `ValueAlign` | Değer yayının hizası: `Inner` / `Center` / `Outer` |
| `RoundedEnds` | Yay uçlarının yuvarlatılması (0/1) |
| `Visible` / `Active` / `Left` / `Top` | Ortak özellikler |

## 🔍 3. Örnek Senaryo
Bu örnekte **2 sayfa** vardır.

**Page 1** — solda **statik** (mavi, Center hizalı), sağda **dinamik** (yeşil, Inner
hizalı = kenarlık efekti) iki Arc bulunur. Her arc'ın ortasında değeri gösteren bir
ELabelBox vardır. Alttaki butonlar her iki Arc ın değerini `ArcSet` ile 80/30 yapar ve
`LabelSet` ile ortadaki sayıyı da günceller. "Page 2" butonu ikinci sayfaya geçer.

- `Value 80`: `ArcSet(Arc1/Arc2,"Value","80")` + `LabelSet(ArcVal1/ArcVal2,"Text","80")`
- `Value 30`: değerleri 30 yapar
- `Page 2`: `ChangeScreenSet("Screen2")`

**Page 2** — buradaki "Static 60" butonu **statik Arc1**'i uzaktan (farklı sayfadan)
60 yapar. "Page 1" ile geri dönülünce statik Arc1'in yeni değeri **korunur**; dinamik
Arc2 ise sayfaya özgü olduğu için buradan değiştirilemez ve Page 1'e dönünce ilk haliyle
yeniden oluşur.

## 🎯 4. Sonuç
✔️ `Value` değiştikçe Arc dolumu ve ortadaki sayı anında güncellenir.
✔️ **Statik Arc Value Set** diğer sayfalardan değiştirilebilir ve değeri korunur.
✔️ **Dinamik Arc Value Set** yalnızca oluşturulduğu sayfada değiştirilebilir.

### Başlangıç durumu (Value 30)

![Başlangıç durumu](1.png)

### Value 80 butonuna basıldıktan sonra

![Value 80 sonrası](2.png)
