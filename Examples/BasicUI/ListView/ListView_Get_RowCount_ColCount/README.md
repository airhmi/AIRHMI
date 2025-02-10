# ListViewGet ROWCOUNT ve COLCOUNT Özelliği

ListView airhmi de veri tabanı olarak kullanılır. 
ListView de listedeki kolon sayısı ve row sayısınını alibiliyoruz. 

```
char data[20];

ListViewGet("ListView1" ,"ROWCOUNT", data);
LabelSet("ELabelBox9" ,"Text" , data );

ListViewGet("ListView1" ,"COLCOUNT", data);
LabelSet("ELabelBox10" ,"Text" , data );

```

ListView nesnesi statik dir. Her sayfadan erişilibilinir. 

## Program İlk Açılış Görüntüsü
![Açıklama Metni](1.png)
## ListView den verileri alıp label a set ediyoruz. 
![Açıklama Metni](2.png)
## Sonraki Sayfaya geçiyoruz. 
![Açıklama Metni](3.png)
## Listview den verileri 2. sayfadan da alıyoruz. 
![Açıklama Metni](4.png)

