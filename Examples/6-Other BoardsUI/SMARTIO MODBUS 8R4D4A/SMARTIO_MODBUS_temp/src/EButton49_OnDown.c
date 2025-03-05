#include "stk.h"
#include "stdio.h"
#include "stdlib.h"


// WriteSingleRegister işlevi çağrılıyor. 
// İlk parametre: 0 -> Cihaz adresi (slave ID).
// İkinci parametre: 0xA000 -> Yazılacak register adresi.
// Üçüncü parametre: 1 -> Yazılacak değer. (0 olur ise Rölelerin son durumunu tutma işlemini yapmaz , 1 ise yapar.)
// İşlev sonucu `ret` değişkenine atanıyor. Bu dönüş değeri, işlemin başarı durumu hakkında bilgi verir.
int ret = WriteSingleRegister(0,0xA000,1);

printf("Ret:%d \n",ret);




