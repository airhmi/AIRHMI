#include "stk.h"
#include "stdio.h"

int data;

// WriteSingleCoil işlevi çağrılıyor.
// İşlevin amacı: Tek bir coil'in (1 bitlik veri) durumunu belirli bir adreste değiştirmek.
// Parametreler:
// 1. `1`: Slave cihaz adresi (Slave ID). Bu, iletişim kurulacak cihazı belirler.
// 2. `0x0000`: Coil adresi. Bu, yazılacak coil'in adresidir.
// 3. `0x0000`: Yazılacak veri. Burada `0x0000` coil'in durumunun `OFF` (veya kapalı) olması gerektiğini belirtir.
// İşlevin dönüş değeri `data` değişkenine atanır.
// Dönüş değeri, işlemin başarı durumu hakkında bilgi verir (örneğin, 1 başarı, 0 hatalı durumu belirtir).
data = WriteSingleCoil(1, 0x0000, 0x0000);
