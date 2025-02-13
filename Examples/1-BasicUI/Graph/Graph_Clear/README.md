# GRAPH_Clear Özelliği

Bu dokümanda, statik ve dinamik olmak üzere iki farklı GRAPH_Clear durumları üzerinde etkili olan faktörler incelenmiştir.
Statik Graph, her sayfadan tüm özelliklerine ulaşılıp değiştirilebilen Graph'dır. **Static(false)** yani dinamik Graph'lar ise sayfaya özgüdür.
Sayfa değiştiği zaman hiçbir özelliği tutulmaz. Sayfa değişip tekrar aynı sayfaya gidildiği zaman Graph ilk hali ile baştan oluşturulur. 

```
GraphSet("Graph1" ,"Value" , "45");
```

## 📌 1. Graph Tanımı
- **🟢 Statik Graph**: Static özelliği **true** olan Graph'dir. Value Set özelliği **hem aynı sayfadan hem de diğer sayfalardan** değiştirilebilir.
- **🔵 Dinamik Graph**: Static özelliği **false** olan Graph'dir. Value Set özelliği **yalnızca aynı sayfada** değiştirilebilir, diğer sayfalardan değiştirilemez.

## 🔍 2. GRAPH_Clear Durumları
### 🏠 Aynı Sayfada Olası Senaryolar
- Kullanıcı **statik GRAPH_Clear** yapabilir.
- Kullanıcı **dinamik GRAPH_Clear** yapabilir.

### 🔄 Farklı Sayfadan Olası Senaryolar
- Kullanıcı **statik GRAPH_Clear** yapabilir.
- Kullanıcı **dinamik GRAPH_Clear** ile değiştiremez.

## 🎯 3. Sonuç
✔️ Aynı sayfada **her iki GRAPH_Clear durumu değiştirilebilir**.  
✔️ **Statik GRAPH_Clear** diğer sayfalardan değiştirilebilir.  
✔️ **Dinamik GRAPH_Clear** yalnızca oluşturulduğu sayfada değiştirilebilir.  

Bu bilgiler ışığında, **Value Set değişikliklerinin beklenen sonuçları doğru şekilde ele alınmalıdır.** 🚀

## Program İlk Açılış Görüntüsü
![Açıklama Metni](1.png)

## Her iki Graph'in içeriği siliniyor.
![Açıklama Metni](2.png)

## Sonraki sayfaya geçiyoruz.
![Açıklama Metni](3.png)

![Açıklama Metni](4.png)

![Açıklama Metni](5.png)