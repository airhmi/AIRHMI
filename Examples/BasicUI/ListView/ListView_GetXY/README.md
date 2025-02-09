# ListView Get XY Özelliği

ListView airhmi de veri tabanı olarak kullanılır. 
ListView den isteğidiğimiz bir alandan veri alabiliriz. Listeden aldığımız veriyi label a set ediyoruz.

```
char data[20];

ListViewGetXY("ListView1" ,0 , 0 , data);
LabelSet("ELabelBox5" ,"Text" , data );

ListViewGetXY("ListView1" ,0, 1 , data);
LabelSet("ELabelBox6" ,"Text" , data );

ListViewGetXY("ListView1" ,0 , 2 , data);
LabelSet("ELabelBox7" ,"Text" , data );

ListViewGetXY("ListView1" ,0 , 3 , data);
LabelSet("ELabelBox8" ,"Text" , data );
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

