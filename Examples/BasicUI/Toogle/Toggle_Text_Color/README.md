# Buton Font Color Özelliği

Bu dokümanda, statik ve dinamik olmak üzere iki farklı butonun Text rengi özelliği üzerinde etkili olan faktörler incelenmiştir.
Statik butonlar her sayfadan tüm özelliklerine ulaşılıp değiştirilebilen butonlardır. Static(false) yani dinamik butonlar ise sayfaya özgüdür.
Sayfa değiştiği zaman hiçbir özelliği tutulmaz. Sayfa değişip tekrar aynı sayfaya gidildiği zaman buton ilk hali ile baştan meydana getirilir. 

## 📌 1. Butonların Tanımı
- **🟢 Statik Buton**: Static özelliği true olan butondur. Font_Color özelliği **hem aynı sayfadan hem de diğer sayfalardan** değiştirilebilir.
- **🔵 Dinamik Buton**: Statik özelliği false olan butondur. Font_Color  özelliği **yalnızca aynı sayfada** değiştirilebilir, diğer sayfalardan değiştirilemez.

## 🔍 2. Buton Font_Color Değiştirme Durumları
### 🏠 Aynı Sayfada Olası Senaryolar
- Kullanıcı **statik butonun Text değerini** değiştirebilir.
- Kullanıcı **dinamik butonun Text değerini** değiştirebilir.


### 🔄 Farklı Sayfadan Olası Senaryolar
- Kullanıcı **statik butonun Font_Color değerini** değiştirebilir.
- Kullanıcı **dinamik butonun Font_Color değerini değiştiremez.**
- **Statik buton**, farklı sayfadan Font_Color değerini değiştirirsek, aynı sayfaya dönüldüğünde **yeni değiştirilen Font_Color değeri gelir.**
- **Dinamik butonun Font_Color değerini değiştirsek bile etki etmez.**

## 🎯 4. Sonuç
✔️ Aynı sayfada **her iki butonun Font_Color değerini değiştirilebilir**.  
✔️ **Statik butonun Font_Color değerini** diğer sayfalardan değiştirilebilir.  
✔️ **Dinamik butonun Font_Color değerini** yalnızca oluşturulduğu sayfada değiştirilebilir.  

![Açıklama Metni](1.png)
![Açıklama Metni](2.png)
![Açıklama Metni](3.png)



